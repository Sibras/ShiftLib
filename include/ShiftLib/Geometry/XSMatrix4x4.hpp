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

#include "Geometry/XSMatrix4x3.hpp"
#include "SIMD/XSSIMD16.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class Matrix4x4;

template<typename T>
class Matrix4x4Data
{
public:
    SIMD16Data<T> values;

    /** Default constructor. */
    XS_FUNCTION Matrix4x4Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit Matrix4x4Data(const Matrix4x4<T, Width>& other) noexcept
        : values(other.columns)
    {}

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param col0Row0 The first element of the first column of the matrix.
     * @param col0Row1 The second element of the first column of the matrix.
     * @param col0Row2 The third element of the first column of the matrix.
     * @param col0Row3 The fourth element of the first column of the matrix.
     * @param col1Row0 The first element of the second column of the matrix.
     * @param col1Row1 The second element of the second column of the matrix.
     * @param col1Row2 The third element of the second column of the matrix.
     * @param col1Row3 The fourth element of the second column of the matrix.
     * @param col2Row0 The first element of the third column of the matrix.
     * @param col2Row1 The second element of the third column of the matrix.
     * @param col2Row2 The third element of the third column of the matrix.
     * @param col2Row3 The fourth element of the third column of the matrix.
     * @param col3Row0 The first element of the fourth column of the matrix.
     * @param col3Row1 The second element of the fourth column of the matrix.
     * @param col3Row2 The third element of the fourth column of the matrix.
     * @param col3Row3 The fourth element of the fourth column of the matrix.
     */
    XS_FUNCTION void setData(T col0Row0, T col0Row1, T col0Row2, T col0Row3, T col1Row0, T col1Row1, T col1Row2,
        T col1Row3, T col2Row0, T col2Row1, T col2Row2, T col2Row3, T col3Row0, T col3Row1, T col3Row2,
        T col3Row3) noexcept
    {
        this->values.setData(col0Row0, col0Row1, col0Row2, col0Row3, col1Row0, col1Row1, col1Row2, col1Row3, col2Row0,
            col2Row1, col2Row2, col2Row3, col3Row0, col3Row1, col3Row2, col3Row3);
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const Matrix4x4<T, Width>& other) noexcept
    {
        this->values.store(other.columns);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Matrix4x4<T, Width> load() const noexcept
    {
        return Matrix4x4<T, Width>(this->values.template load<Matrix4x4<T, Width>::widthImpl>());
    }
};

template<typename T>
class Matrix4x4DataPad
{
public:
    SIMD16DataPad<T> values;

    /** Default constructor. */
    XS_FUNCTION Matrix4x4DataPad() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit Matrix4x4DataPad(const Matrix4x4<T, Width>& other) noexcept
        : values(other.columns)
    {}

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param col0Row0 The first element of the first column of the matrix.
     * @param col0Row1 The second element of the first column of the matrix.
     * @param col0Row2 The third element of the first column of the matrix.
     * @param col0Row3 The fourth element of the first column of the matrix.
     * @param col1Row0 The first element of the second column of the matrix.
     * @param col1Row1 The second element of the second column of the matrix.
     * @param col1Row2 The third element of the second column of the matrix.
     * @param col1Row3 The fourth element of the second column of the matrix.
     * @param col2Row0 The first element of the third column of the matrix.
     * @param col2Row1 The second element of the third column of the matrix.
     * @param col2Row2 The third element of the third column of the matrix.
     * @param col2Row3 The fourth element of the third column of the matrix.
     * @param col3Row0 The first element of the fourth column of the matrix.
     * @param col3Row1 The second element of the fourth column of the matrix.
     * @param col3Row2 The third element of the fourth column of the matrix.
     * @param col3Row3 The fourth element of the fourth column of the matrix.
     */
    XS_FUNCTION void setData(T col0Row0, T col0Row1, T col0Row2, T col0Row3, T col1Row0, T col1Row1, T col1Row2,
        T col1Row3, T col2Row0, T col2Row1, T col2Row2, T col2Row3, T col3Row0, T col3Row1, T col3Row2,
        T col3Row3) noexcept
    {
        this->values.setData(col0Row0, col0Row1, col0Row2, col0Row3, col1Row0, col1Row1, col1Row2, col1Row3, col2Row0,
            col2Row1, col2Row2, col2Row3, col3Row0, col3Row1, col3Row2, col3Row3);
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const Matrix4x4<T, Width>& other) noexcept
    {
        this->values.store(other.columns);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Matrix4x4<T, Width> load() const noexcept
    {
        return Matrix4x4<T, Width>(this->values.template load<Matrix4x4<T, Width>::widthImpl>());
    }
};

/**
 * A a base type used to represent a 4x4 matrix.
 * @note This uses standard right handed coordinate notation
 *    i.e. Vector (V) transforms with Matrix (M) occur in the following order:
 *         V2 = M * V
 *    Matrix multiplications between 2 matrices (M1, M2) gives the following ordering:
 *         M3 = M1 * M2        V2 = M3 * V    equivalent to->  V2 = M1 * (M2 * V)
 * @tparam T     Generic type parameter.
 * @tparam Width Type of SIMD being used.
 */
template<typename T, SIMDWidth Width = widthSIMD<T>>
class Matrix4x4
{
public:
    using Type = T;
    using SIMD4x4Def = SIMD16<T, SIMD16<T, Width>::widthImpl>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = SIMD4x4Def::widthImpl;
    using BaseDef = typename SIMD4x4Def::BaseDef;
    using InBaseDef = typename SIMD4x4Def::InBaseDef;
    using SIMD4Def = typename SIMD4x4Def::SIMD4Def;
    using SIMD3Def = typename SIMD4Def::SIMD3Def;
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
    using Matrix4x3Def = Matrix4x3<T, Matrix4x3<T, widthImpl>::widthImpl>;
    using Matrix3x3Def = Matrix3x3<T, Matrix3x3<T, widthImpl>::widthImpl>;
    using QuaternionDef = Quaternion<T, Quaternion<T, widthImpl>::widthImpl>;

    SIMD4x4Def columns;

    /** Default constructor. */
    XS_FUNCTION Matrix4x4() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION Matrix4x4(const Matrix4x4& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Matrix4x4(Matrix4x4&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Matrix4x4& operator=(const Matrix4x4& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Matrix4x4& operator=(Matrix4x4&& other) noexcept = default;

    /**
     * Construct a 4x4 matrix from a SIMD16.
     * @param values The 4 columns of the matrix.
     */
    XS_FUNCTION explicit Matrix4x4(const SIMD4x4Def& values) noexcept
        : columns(values)
    {}

    /**
     * Construct a 4x4 matrix from 4 SIMD4s.
     * @param column0 The first column of the matrix.
     * @param column1 The second column of the matrix.
     * @param column2 The third column of the matrix.
     * @param column3 The fourth column of the matrix.
     */
    XS_FUNCTION Matrix4x4(
        const SIMD4Def& column0, const SIMD4Def& column1, const SIMD4Def& column2, const SIMD4Def& column3) noexcept
        : columns(column0, column1, column2, column3)
    {}

    /**
     * Construct a 4x4 matrix from 16 values.
     * @param col0Row0 The first element of the first column of the matrix.
     * @param col0Row1 The second element of the first column of the matrix.
     * @param col0Row2 The third element of the first column of the matrix.
     * @param col0Row3 The fourth element of the first column of the matrix.
     * @param col1Row0 The first element of the second column of the matrix.
     * @param col1Row1 The second element of the second column of the matrix.
     * @param col1Row2 The third element of the second column of the matrix.
     * @param col1Row3 The fourth element of the second column of the matrix.
     * @param col2Row0 The first element of the third column of the matrix.
     * @param col2Row1 The second element of the third column of the matrix.
     * @param col2Row2 The third element of the third column of the matrix.
     * @param col2Row3 The fourth element of the third column of the matrix.
     * @param col3Row0 The first element of the fourth column of the matrix.
     * @param col3Row1 The second element of the fourth column of the matrix.
     * @param col3Row2 The third element of the fourth column of the matrix.
     * @param col3Row3 The fourth element of the fourth column of the matrix.
     */
    XS_FUNCTION Matrix4x4(T col0Row0, T col0Row1, T col0Row2, T col0Row3, T col1Row0, T col1Row1, T col1Row2,
        T col1Row3, T col2Row0, T col2Row1, T col2Row2, T col2Row3, T col3Row0, T col3Row1, T col3Row2,
        T col3Row3) noexcept
        : columns(col0Row0, col0Row1, col0Row2, col0Row3, col1Row0, col1Row1, col1Row2, col1Row3, col2Row0, col2Row1,
              col2Row2, col2Row3, col3Row0, col3Row1, col3Row2, col3Row3)
    {}

    /**
     * Construct from a Matrix3x3.
     * @param matrix Reference to matrix to copy.
     */
    XS_FUNCTION explicit Matrix4x4(const Matrix3x3Def& matrix) noexcept
        : columns(Matrix4x3Def::SIMD3x4Def::SIMD12Def(
                      matrix.columns, Matrix4x3Def::SIMD3x4Def::SIMD12Def::SIMD3Def::Zero()),
              SIMD4Def(0.0f, 0.0f, 0.0f, 1.0f))
    {}

    /**
     * Construct from a Matrix4x3.
     * @param matrix Reference to matrix to copy.
     * @param row3   The fourth row of the matrix.
     */
    XS_FUNCTION explicit Matrix4x4(const Matrix4x3Def& matrix, const SIMD4Def& row3) noexcept
        : columns(matrix.columns, row3)
    {}

    /**
     * Construct from a Quaternion.
     * @param quaternion Reference to quaternion to copy.
     */
    XS_FUNCTION explicit Matrix4x4(const QuaternionDef& quaternion) noexcept
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
        //    2zy-2yz  2xz-2zx  2yx-2xy <- 4x4 row fix
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            const typename QuaternionDef::SIMD4Def quat2(quaternion.values + quaternion.values);
            typename QuaternionDef::SIMD4Def col0(
                quaternion.values.template shuffle<2, 3, 0, 1>().template negate<true, false, false, false>());
            typename QuaternionDef::SIMD4Def col1(
                quaternion.values.template shuffle<0, 3, 1, 2>().template negate<true, false, false, false>());
            typename QuaternionDef::SIMD4Def col2(
                quaternion.values.template shuffle<1, 3, 2, 0>().template negate<true, false, false, false>());
            const typename QuaternionDef::SIMD4Def one(1.0f, 0.0f, 0.0f, 0.0f);
            col0 = col0.mad(quat2.template getValue<2>(), one);
            col1 = col1.mad(quat2.template getValue<0>(), one);
            col2 = col2.mad(quat2.template getValue<1>(), one);
            col0 = quaternion.values.template shuffle<1, 0, 3, 2>().msub(
                quat2.template getValue<1>(), col0.template negate<false, true, false, false>());
            col1 = quaternion.values.template shuffle<2, 1, 3, 0>().msub(
                quat2.template getValue<2>(), col1.template negate<false, true, false, false>());
            col2 = quaternion.values.template shuffle<0, 2, 3, 1>().msub(
                quat2.template getValue<0>(), col2.template negate<false, true, false, false>());

            this->columns = SIMD4x4Def(col0, col1.template shuffle<2, 0, 1, 3>(), col2.template shuffle<1, 2, 0, 3>(),
                SIMD4Def(0.0f, 0.0f, 0.0f, 1.0f));
        } else {
            const typename QuaternionDef::SIMD4Def quat2(quaternion.values + quaternion.values);
            typename QuaternionDef::SIMD4Def::SIMD3Def col0(
                quaternion.values.template getValue3<2, 3, 0>().template negate<true, false, false>());
            typename QuaternionDef::SIMD4Def::SIMD3Def col1(
                quaternion.values.template getValue3<0, 3, 1>().template negate<true, false, false>());
            typename QuaternionDef::SIMD4Def::SIMD3Def col2(
                quaternion.values.template getValue3<1, 3, 2>().template negate<true, false, false>());
            const typename QuaternionDef::SIMD4Def::SIMD3Def::InBaseDef one(
                QuaternionDef::SIMD4Def::SIMD3Def::InBaseDef::One());
            col0 *= quat2.template getValue<2>();
            col1 *= quat2.template getValue<0>();
            col2 *= quat2.template getValue<1>();
            col0.template addValue<0>(one);
            col1.template addValue<0>(one);
            col2.template addValue<0>(one);
            col0 = col0.subAdd(quaternion.values.template getValue3<1, 0, 3>() * quat2.template getValue<1>());
            col1 = col1.subAdd(quaternion.values.template getValue3<2, 1, 3>() * quat2.template getValue<2>());
            col2 = col2.subAdd(quaternion.values.template getValue3<0, 2, 3>() * quat2.template getValue<0>());

            const typename QuaternionDef::SIMD4Def::SIMD3Def::BaseDef zero(
                QuaternionDef::SIMD4Def::SIMD3Def::BaseDef::Zero());
            this->columns = SIMD4x4Def(SIMD4Def(col0, zero), SIMD4Def(col1.template shuffle<2, 0, 1>(), zero),
                SIMD4Def(col2.template shuffle<1, 2, 0>(), zero), SIMD4Def(0.0f, 0.0f, 0.0f, 1.0f));
        }
    }

    /**
     * Construct from a Matrix4x4.
     * @return Newly constructed object.
     */
    XS_FUNCTION explicit operator Matrix3x3Def() const noexcept
    {
        return Matrix3x3Def(this->columns.template getValue3x4<0, 1, 2>().template getValue3x3<0, 1, 2>());
    }

    /**
     * Constructor to build a 4x4 Identity Matrix.
     * @returns Newly constructed Matrix4x4 with required attributes.
     */
    XS_FUNCTION static Matrix4x4 Identity() noexcept
    {
        return Matrix4x4(
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    }

    /**
     * Constructor to build a 4x4 Matrix with fixed translation.
     * @param translation The amount to translate in the X/Y/Z directions.
     * @returns Newly constructed Matrix4x4 with required attributes.
     */
    XS_FUNCTION static Matrix4x4 Translation(const SIMD3Def& translation) noexcept
    {
        return Matrix4x4(
            SIMD4x4Def(SIMD4x4Def::SIMD12Def(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f),
                SIMD4Def(translation, SIMD4Def::BaseDef::One())));
    }

    /**
     * Constructor to build a 4x4 Matrix with fixed uniform scale.
     * @param scale The amount to scale in the X/Y/Z direction.
     * @returns Newly constructed Matrix4x3 with required attributes.
     */
    XS_FUNCTION static Matrix4x4 UniformScale(InBaseDef scale) noexcept
    {
        SIMD4Def scale4(SIMD4Def::Zero());
        scale4.template setValue<0>(scale);
        return Matrix4x4(scale4, scale4.template shuffle<1, 0, 3, 2>(), scale4.template shuffle<2, 3, 0, 1>(),
            SIMD4Def(0.0f, 0.0f, 0.0f, 1.0f));
    }

    /**
     * Constructor to build a 4x4 Matrix with fixed scale.
     * @param scale The amount to scale in the X/Y/Z direction.
     * @returns Newly constructed Matrix4x4 with required attributes.
     */
    XS_FUNCTION static Matrix4x4 Scale(const SIMD3Def& scale) noexcept
    {
        const SIMD4Def scale4(scale, SIMD4Def::BaseDef::Zero());
        return Matrix4x4(scale4.template shuffle<0, 3, 3, 3>(), scale4.template shuffle<3, 1, 3, 3>(),
            scale4.template shuffle<3, 3, 2, 3>(), SIMD4Def(0.0f, 0.0f, 0.0f, 1.0f));
    }

    /**
     * Constructor to build a 4x4 Matrix with fixed rotation about X axis.
     * @param rotation The amount to rotate the matrix by (in radians).
     * @returns Newly constructed Matrix4x4 with required attributes.
     */
    XS_FUNCTION static Matrix4x4 RotationX(typename SIMD4Def::BaseDef rotation) noexcept
    {
        using SIMD2Def = typename SIMD4Def::SIMD2Def;
        using BaseDef2 = typename SIMD2Def::BaseDef;
        SIMD2Def sinCos;
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            sinCos = sin(SIMD2Def(rotation, rotation + BaseDef2(valPi2<T>)));
        } else {
            BaseDef2 cos;
            const BaseDef2 sin(sincos(rotation, cos));
            sinCos = SIMD2Def(sin, cos);
        }
        const SIMD4Def sinCosZero(sinCos, SIMD2Def::Zero());
        const SIMD4Def ident(1.0, 0.0, 0.0f, 0.0f);
        return Matrix4x4(ident, sinCosZero.template shuffle<3, 1, 0, 2>(),
            sinCosZero.template shuffle<2, 0, 1, 3>().template negate<false, true, false, false>(),
            ident.template shuffle<3, 2, 1, 0>());
    }

    /**
     * Constructor to build a 4x4 Matrix with fixed rotation about Y axis.
     * @param rotation The amount to rotate the matrix by (in radians).
     * @returns Newly constructed Matrix4x4 with required attributes.
     */
    XS_FUNCTION static Matrix4x4 RotationY(typename SIMD4Def::BaseDef rotation) noexcept
    {
        using SIMD2Def = typename SIMD4Def::SIMD2Def;
        using BaseDef2 = typename SIMD2Def::BaseDef;
        SIMD2Def sinCos;
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            sinCos = sin(SIMD2Def(rotation, rotation + BaseDef2(valPi2<T>)));
        } else {
            BaseDef2 cos;
            const BaseDef2 sin(sincos(rotation, cos));
            sinCos = SIMD2Def(sin, cos);
        }
        const SIMD4Def sinCosZero(sinCos, SIMD2Def::Zero());
        const SIMD4Def col2(0.0f, 1.0f, 0.0f, 0.0f);
        return Matrix4x4(sinCosZero.template shuffle<1, 2, 0, 3>().template negate<false, false, true, false>(), col2,
            sinCosZero.template shuffle<0, 2, 1, 3>(), col2.template shuffle<3, 0, 2, 1>());
    }

    /**
     * Constructor to build a 4x4 Matrix with fixed rotation about Z axis.
     * @param rotation The amount to rotate the matrix by (in radians).
     * @returns Newly constructed Matrix4x4 with required attributes.
     */
    XS_FUNCTION static Matrix4x4 RotationZ(typename SIMD4Def::BaseDef rotation) noexcept
    {
        using SIMD2Def = typename SIMD4Def::SIMD2Def;
        using BaseDef2 = typename SIMD2Def::BaseDef;
        SIMD2Def sinCos;
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            sinCos = sin(SIMD2Def(rotation, rotation + BaseDef2(valPi2<T>)));
        } else {
            BaseDef2 cos;
            const BaseDef2 sin(sincos(rotation, cos));
            sinCos = SIMD2Def(sin, cos);
        }
        const SIMD4Def sinCosZero(sinCos, SIMD2Def::Zero());
        return Matrix4x4(SIMD4x4Def(SIMD4x4Def::SIMD8Def(sinCosZero.template shuffle<1, 0, 3, 2>(),
                                        sinCosZero.template negate<true, false, false, false>()),
            SIMD4x4Def::SIMD8Def(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)));
    }

    /**
     * Constructor to build a 4x4 Matrix with fixed rotation about arbitrary axis.
     * @param axis     The axis to rotate around.
     * @param rotation The amount to rotate the matrix by (in radians).
     * @returns Newly constructed Matrix4x4 with required attributes.
     */
    XS_FUNCTION static Matrix4x4 RotationAxis(const Vector3DDef& axis, typename SIMD4Def::BaseDef rotation) noexcept
    {
        // The equivalent matrix for a rotation around an axis can be described by:
        //      xx(1-c)+ s xy(1-c)-zs xz(1-c)+ys
        //      yx(1-c)+zs yy(1-c)+ c yz(1-c)-xs
        //      zx(1-c)-ys zy(1-c)-xs zz(1-c)+ c
        using SIMD2Def = typename SIMD4Def::SIMD2Def;
        using BaseDef2 = typename SIMD2Def::BaseDef;
        SIMD2Def sinCos;
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            sinCos = sin(SIMD2Def(rotation, rotation + BaseDef2(valPi2<T>)));
        } else {
            BaseDef2 cos;
            const BaseDef2 sin(sincos(rotation, cos));
            sinCos = SIMD2Def(sin, cos);
        }
        const typename SIMD4Def::SIMD3Def::BaseDef oneMinusCos(
            SIMD4Def::SIMD3Def::BaseDef::One() - sinCos.template getValue<1>());

        const typename SIMD4Def::SIMD3Def col0(axis.values * axis.values.template getValue<0>() * oneMinusCos);
        const typename SIMD4Def::SIMD3Def col1(axis.values * axis.values.template getValue<1>() * oneMinusCos);
        const typename SIMD4Def::SIMD3Def col2(axis.values * axis.values.template getValue<2>() * oneMinusCos);
        const SIMD4Def mods(axis.values, SIMD4Def::BaseDef::One());

        if constexpr (widthImpl > SIMDWidth::Scalar) {
            const SIMD4Def negSinCosSinZero(SIMD4Def(sinCos, SIMD4Def::SIMD2Def::Zero())
                                                .template shuffle<0, 1, 0, 2>()
                                                .template negate<true, false, false, false>());

            const SIMD4Def temp1(mods.template shuffle<3, 2, 1, 3>());
            const SIMD4Def temp2(mods.template shuffle<2, 3, 0, 3>());
            const SIMD4Def temp3(mods.template shuffle<1, 0, 3, 3>());
            return Matrix4x4(temp1.mad(negSinCosSinZero.template shuffle<1, 2, 0, 3>(), SIMD4Def(col0.values)),
                temp2.mad(negSinCosSinZero, SIMD4Def(col1.values)),
                temp3.mad(negSinCosSinZero.template shuffle<2, 0, 1, 3>(), SIMD4Def(col2.values)),
                SIMD4Def(0.0f, 0.0f, 0.0f, 1.0f));
        } else {
            const typename SIMD4Def::SIMD3Def negSinCosSin(
                sinCos.template negate<true, false>(), sinCos.template getValueInBase<0>());

            const typename SIMD4Def::BaseDef zero(SIMD4Def::BaseDef::Zero());
            return Matrix4x4(
                SIMD4Def(mods.template getValue3<3, 2, 1>().mad(negSinCosSin.template shuffle<1, 2, 0>(), col0), zero),
                SIMD4Def(mods.template getValue3<2, 3, 0>().mad(negSinCosSin, col1), zero),
                SIMD4Def(mods.template getValue3<1, 0, 3>().mad(negSinCosSin.template shuffle<2, 0, 1>(), col2), zero),
                SIMD4Def(0.0f, 0.0f, 0.0f, 1.0f));
        }
    }

    /**
     * Get the first column of a Matrix4x4.
     * @tparam Index The index of the column to retrieve (must be between 0 and 3).
     * @returns SIMD4Def containing the desired values.
     */
    template<uint32_t Index>
    XS_FUNCTION SIMD4Def getColumn() const noexcept
    {
        return this->columns.template getValue4<Index>();
    }

    /**
     * Get a row of a Matrix4x4.
     * @tparam Index The index of the row to retrieve (must be between 0 and 3).
     * @returns SIMD4Def containing the desired values.
     */
    template<uint32_t Index>
    XS_FUNCTION SIMD4Def getRow() const noexcept
    {
        // Note: this is not well optimised as it is only intended for viewing contents
        return this->columns.transpose4().template getValue4<Index>();
    }

    /**
     * Set the first column of a Matrix4x4.
     * @tparam Index The index of the column to set (must be between 0 and 3).
     * @param column The new column values.
     */
    template<uint32_t Index>
    XS_FUNCTION void setColumn(const SIMD4Def& column) noexcept
    {
        this->columns.template setValue4<Index>(column);
    }

    /**
     * Multiply a 4x4 Matrix by a 4x4 Matrix and then add another 4x4 Matrix.
     * @param matrix1 Matrix to multiply the first by.
     * @param matrix2 Second matrix to add.
     * @returns Result of operation.
     */
    XS_FUNCTION Matrix4x4 mad(const Matrix4x4<T, Width>& matrix1, const Matrix4x4<T, Width>& matrix2) const noexcept
    {
        if constexpr (numValues<T, SIMD4x4Def::widthImpl> >= 16) {
            SIMD4x4Def ret(matrix1.columns.template shuffle4<3, 3, 3, 3>().mad(
                this->columns.template shuffleH4<3, 3, 3, 3>(), matrix2.columns));
            ret = matrix1.columns.template shuffle4<2, 2, 2, 2>().template mad<false>(
                this->columns.template shuffleH4<2, 2, 2, 2>(), ret);
            ret = matrix1.columns.template shuffle4<1, 1, 1, 1>().template mad<false>(
                this->columns.template shuffleH4<1, 1, 1, 1>(), ret);
            return Matrix4x4(matrix1.columns.template shuffle4<0, 0, 0, 0>().template mad<false>(
                this->columns.template shuffleH4<0, 0, 0, 0>(), ret));
        } else if constexpr (numValues<T, SIMD4x4Def::widthImpl> >= 8) {
            SIMD4x4Def ret(matrix1.columns.template shuffle4<3, 3, 3, 3>().mad(
                this->columns.template getValue4x2<3, 3>(), matrix2.columns));
            ret = matrix1.columns.template shuffle4<2, 2, 2, 2>().template mad<false>(
                this->columns.template getValue4x2<2, 2>(), ret);
            ret = matrix1.columns.template shuffle4<1, 1, 1, 1>().template mad<false>(
                this->columns.template getValue4x2<1, 1>(), ret);
            return Matrix4x4(matrix1.columns.template shuffle4<0, 0, 0, 0>().template mad<false>(
                this->columns.template getValue4x2<0, 0>(), ret));
        } else {
            SIMD4x4Def ret(matrix1.columns.template shuffle4<3, 3, 3, 3>().mad(
                this->columns.template getValue4<3>(), matrix2.columns));
            ret = matrix1.columns.template shuffle4<2, 2, 2, 2>().template mad<false>(
                this->columns.template getValue4<2>(), ret);
            ret = matrix1.columns.template shuffle4<1, 1, 1, 1>().template mad<false>(
                this->columns.template getValue4<1>(), ret);
            return Matrix4x4(matrix1.columns.template shuffle4<0, 0, 0, 0>().template mad<false>(
                this->columns.template getValue4<0>(), ret));
        }
    }

    /**
     * Transform a 3-D point using specified matrix.
     * @param point The point to be transformed.
     * @returns A new transformed point.
     */
    XS_FUNCTION Point3DDef transform(const Point3DDef& point) const noexcept
    {
        SIMD4Def ret(this->columns.template getValue4<0>().mad(
            point.values.template getValue<0>(), this->columns.template getValue4<3>()));
        ret = this->columns.template getValue4<1>().template mad<false>(point.values.template getValue<1>(), ret);
        ret = this->columns.template getValue4<2>().template mad<false>(point.values.template getValue<2>(), ret);
        return Point3DDef(ret.template getValue3<0, 1, 2>() / ret.template getValue<3>());
    }

    /**
     * Transform a 3-D vector using specified matrix.
     * @note This is equivalent to transforming by the matrices 3x3 component.
     * @param vector The vector to be transformed.
     * @returns A new transformed vector.
     */
    XS_FUNCTION Vector3DDef transform(const Vector3DDef& vector) const noexcept
    {
        SIMD3Def ret(
            this->columns.template getValue4<0>().template getValue3<0, 1, 2>() * vector.values.template getValue<0>());
        ret = this->columns.template getValue4<1>().template getValue3<0, 1, 2>().template mad<false>(
            vector.values.template getValue<1>(), ret);
        return Vector3DDef(this->columns.template getValue4<2>().template getValue3<0, 1, 2>().template mad<false>(
            vector.values.template getValue<2>(), ret));
    }

    /**
     * Transform two packed 3-D points using specified matrix.
     * @tparam Packed Whether the point is using packed storage.
     * @param point The points to be transformed.
     * @returns New transformed points.
     */
    template<bool Packed>
    XS_FUNCTION Point3D2Def<Packed> transform(const Point3D2Def<Packed>& point) const noexcept
    {
        using SIMD8Def = typename SIMD4x4Def::SIMD8Def;
        if constexpr (numValues<T, SIMD4x4Def::widthImpl> >= 8 && !Packed) {
            SIMD8Def ret(
                SIMD8Def::template Shuffle4<0, 0, 0, 0>(point.points)
                    .mad(this->columns.template getValue4x2<0, 0>(), this->columns.template getValue4x2<3, 3>()));
            ret = SIMD8Def::template Shuffle4<1, 1, 1, 1>(point.points)
                      .template mad<false>(this->columns.template getValue4x2<1, 1>(), ret);
            ret = SIMD8Def::template Shuffle4<2, 2, 2, 2>(point.points)
                      .template mad<false>(this->columns.template getValue4x2<2, 2>(), ret);
            return Point3D2Def<Packed>(ret.template getValue3x2<0, 1, 2>() / ret.template getValue3x2<3, 3, 3>());
        } else {
            SIMD8Def ret(SIMD8Def::template Shuffle4<0, 0, 0, 0>(point.points)
                             .mad(this->columns.template getValue4<0>(), this->columns.template getValue4<3>()));
            ret = SIMD8Def::template Shuffle4<1, 1, 1, 1>(point.points)
                      .template mad<false>(this->columns.template getValue4<1>(), ret);
            ret = SIMD8Def::template Shuffle4<2, 2, 2, 2>(point.points)
                      .template mad<false>(this->columns.template getValue4<2>(), ret);
            return Point3D2Def<Packed>(ret.template getValue3x2<0, 1, 2>() / ret.template getValue3x2<3, 3, 3>());
        }
    }

    /**
     * Transform two packed 3-D vectors using specified matrix.
     * @note This is equivalent to transforming by the matrices 3x3 component.
     * @tparam Packed Whether the vector is using packed storage.
     * @param vector The vectors to be transformed.
     * @returns New transformed vectors.
     */
    template<bool Packed>
    XS_FUNCTION Vector3D2Def<Packed> transform(const Vector3D2Def<Packed>& vector) const noexcept
    {
        const typename SIMD4x4Def::SIMD3x4Def::SIMD3x3Def mat3(
            this->columns.template getValue3x4<0, 1, 2>().template getValue3x3<0, 1, 2>());
        if constexpr (numValues<T, SIMD4x4Def::SIMD3x4Def::SIMD3x3Def::widthImpl> >= 6 && !Packed) {
            auto ret(vector.vectors.template shuffle3<0, 0, 0>() * mat3.template getValue3x2<0, 0>());
            ret =
                vector.vectors.template shuffle3<1, 1, 1>().template mad<false>(mat3.template getValue3x2<1, 1>(), ret);
            return Vector3D2Def<Packed>(vector.vectors.template shuffle3<2, 2, 2>().template mad<false>(
                mat3.template getValue3x2<2, 2>(), ret));
        } else {
            auto ret(vector.vectors.template shuffle3<0, 0, 0>() * mat3.template getValue3<0>());
            ret = vector.vectors.template shuffle3<1, 1, 1>().template mad<false>(mat3.template getValue3<1>(), ret);
            return Vector3D2Def<Packed>(
                vector.vectors.template shuffle3<2, 2, 2>().template mad<false>(mat3.template getValue3<2>(), ret));
        }
    }

    /**
     * Transform four packed 3-D points using specified matrix.
     * @tparam Packed Whether the point is using packed storage.
     * @param point The points to be transformed.
     * @returns New transformed points.
     */
    template<bool Packed>
    XS_FUNCTION Point3D4Def<Packed> transform(const Point3D4Def<Packed>& point) const noexcept
    {
        if constexpr (numValues<T, SIMD4x4Def::widthImpl> >= 16 && !Packed) {
            SIMD4x4Def ret(SIMD4x4Def::template Shuffle4<0, 0, 0, 0>(point.points)
                               .mad(this->columns.template shuffleH4<0, 0, 0, 0>(),
                                   this->columns.template shuffleH4<3, 3, 3, 3>()));
            ret = SIMD4x4Def::template Shuffle4<1, 1, 1, 1>(point.points)
                      .template mad<false>(this->columns.template shuffleH4<1, 1, 1, 1>(), ret);
            ret = SIMD4x4Def::template Shuffle4<2, 2, 2, 2>(point.points)
                      .template mad<false>(this->columns.template shuffleH4<2, 2, 2, 2>(), ret);
            return Point3D4Def<Packed>(ret.template getValue3x4<0, 1, 2>() / ret.template getValue3x4<3, 3, 3>());
        } else if constexpr (numValues<T, SIMD4x4Def::widthImpl> >= 8 && !Packed) {
            SIMD4x4Def ret(SIMD4x4Def::template Shuffle4<0, 0, 0, 0>(point.points)
                               .mad(columns.template getValue4x2<0, 0>(), columns.template getValue4x2<3, 3>()));
            ret = SIMD4x4Def::template Shuffle4<1, 1, 1, 1>(point.points)
                      .template mad<false>(this->columns.template getValue4x2<1, 1>(), ret);
            ret = SIMD4x4Def::template Shuffle4<2, 2, 2, 2>(point.points)
                      .template mad<false>(this->columns.template getValue4x2<2, 2>(), ret);
            return Point3D4Def<Packed>(ret.template getValue3x4<0, 1, 2>() / ret.template getValue3x4<3, 3, 3>());
        } else {
            SIMD4x4Def ret(SIMD4x4Def::template Shuffle4<0, 0, 0, 0>(point.points)
                               .mad(this->columns.template getValue4<0>(), this->columns.template getValue4<3>()));
            ret = SIMD4x4Def::template Shuffle4<1, 1, 1, 1>(point.points)
                      .template mad<false>(this->columns.template getValue4<1>(), ret);
            ret = SIMD4x4Def::template Shuffle4<2, 2, 2, 2>(point.points)
                      .template mad<false>(this->columns.template getValue4<2>(), ret);
            return Point3D4Def<Packed>(ret.template getValue3x4<0, 1, 2>() / ret.template getValue3x4<3, 3, 3>());
        }
    }

    /**
     * Transform four packed 3-D vectors using specified matrix.
     * @note This is equivalent to transforming by the matrices 3x3 component.
     * @tparam Packed Whether the vector is using packed storage.
     * @param vector The vectors to be transformed.
     * @returns New transformed vectors.
     */
    template<bool Packed>
    XS_FUNCTION Vector3D4Def<Packed> transform(const Vector3D4Def<Packed>& vector) const noexcept
    {
        if constexpr (numValues<T, SIMD4x4Def::SIMD3x4Def::SIMD3x3Def::widthImpl> >= 12 && !Packed) {
            const typename SIMD4x4Def::SIMD3x4Def mat43(this->columns.template getValue3x4<0, 1, 2>());
            auto ret(vector.vectors.template shuffle3<0, 0, 0>() * mat43.template shuffleH3<0, 0, 0, 0>());
            ret = vector.vectors.template shuffle3<1, 1, 1>().template mad<false>(
                mat43.template shuffleH3<1, 1, 1, 1>(), ret);
            return Vector3D4Def<Packed>(vector.vectors.template shuffle3<2, 2, 2>().template mad<false>(
                mat43.template shuffleH3<2, 2, 2, 2>(), ret));
        } else if constexpr (numValues<T, SIMD4x4Def::SIMD3x4Def::SIMD3x3Def::widthImpl> >= 6 && !Packed) {
            const typename SIMD4x4Def::SIMD3x4Def::SIMD3x3Def mat3(
                this->columns.template getValue3x4<0, 1, 2>().template getValue3x3<0, 1, 2>());
            auto ret(vector.vectors.template shuffle3<0, 0, 0>() * mat3.template getValue3x2<0, 0>());
            ret =
                vector.vectors.template shuffle3<1, 1, 1>().template mad<false>(mat3.template getValue3x2<1, 1>(), ret);
            return Vector3D4Def<Packed>(vector.vectors.template shuffle3<2, 2, 2>().template mad<false>(
                mat3.template getValue3x2<2, 2>(), ret));
        } else {
            const typename SIMD4x4Def::SIMD3x4Def::SIMD3x3Def mat3(
                this->columns.template getValue3x4<0, 1, 2>().template getValue3x3<0, 1, 2>());
            auto ret(vector.vectors.template shuffle3<0, 0, 0>() * mat3.template getValue3<0>());
            ret = vector.vectors.template shuffle3<1, 1, 1>().template mad<false>(mat3.template getValue3<1>(), ret);
            return Vector3D4Def<Packed>(
                vector.vectors.template shuffle3<2, 2, 2>().template mad<false>(mat3.template getValue3<2>(), ret));
        }
    }

    /**
     * Transform a 3-D point using specified affine matrix.
     * @note Optimised based on the statement that the input matrix is affine (i.e. row4 = 0,0,0,1).
     * @param point The point to be transformed.
     * @returns A new transformed point.
     */
    XS_FUNCTION Point3DDef transformAffine(const Point3DDef& point) const noexcept
    {
        const typename SIMD4x4Def::SIMD3x4Def val(this->columns.template getValue3x4<0, 1, 2>());
        SIMD3Def ret(val.template getValue3<0>().mad(point.values.template getValue<0>(), val.template getValue3<3>()));
        ret = val.template getValue3<1>().template mad<false>(point.values.template getValue<1>(), ret);
        return Point3DDef(val.template getValue3<2>().template mad<false>(point.values.template getValue<2>(), ret));
    }

    /**
     * Transform two packed 3-D points using specified affine matrix.
     * @param point The points to be transformed.
     * @returns New transformed points.
     */
    template<bool Packed>
    XS_FUNCTION Point3D2Def<Packed> transformAffine(const Point3D2Def<Packed>& point) const noexcept
    {
        const typename SIMD4x4Def::SIMD3x4Def val(this->columns.template getValue3x4<0, 1, 2>());
        if constexpr (numValues<T, SIMD4x4Def::SIMD3x4Def::widthImpl> >= 6 && !Packed) {
            auto ret(point.points.template shuffle3<0, 0, 0>().mad(
                val.template getValue3x2<0, 0>(), val.template getValue3x2<3, 3>()));
            ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(val.template getValue3x2<1, 1>(), ret);
            return Point3D2Def<Packed>(
                point.points.template shuffle3<2, 2, 2>().template mad<false>(val.template getValue3x2<2, 2>(), ret));
        } else {
            auto ret(point.points.template shuffle3<0, 0, 0>().mad(
                val.template getValue3<0>(), val.template getValue3<3>()));
            ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(val.template getValue3<1>(), ret);
            return Point3D2Def<Packed>(
                point.points.template shuffle3<2, 2, 2>().template mad<false>(val.template getValue3<2>(), ret));
        }
    }

    /**
     * Transform four packed 3-D points using specified affine matrix.
     * @param point The points to be transformed.
     * @returns New transformed points.
     */
    template<bool Packed>
    XS_FUNCTION Point3D4Def<Packed> transformAffine(const Point3D4Def<Packed>& point) const noexcept
    {
        const typename SIMD4x4Def::SIMD3x4Def val(this->columns.template getValue3x4<0, 1, 2>());
        if constexpr (numValues<T, SIMD4x4Def::SIMD3x4Def::widthImpl> >= 12 && !Packed) {
            auto ret(point.points.template shuffle3<0, 0, 0>().mad(
                val.template shuffleH3<0, 0, 0, 0>(), val.template shuffleH3<3, 3, 3, 3>()));
            ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(
                val.template shuffleH3<1, 1, 1, 1>(), ret);
            return Point3D4Def<Packed>(point.points.template shuffle3<2, 2, 2>().template mad<false>(
                val.template shuffleH3<2, 2, 2, 2>(), ret));
        } else if constexpr (numValues<T, SIMD4x4Def::SIMD3x4Def::widthImpl> >= 6 && !Packed) {
            auto ret(point.points.template shuffle3<0, 0, 0>().mad(
                val.template getValue3x2<0, 0>(), val.template getValue3x2<3, 3>()));
            ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(val.template getValue3x2<1, 1>(), ret);
            return Point3D4Def<Packed>(
                point.points.template shuffle3<2, 2, 2>().template mad<false>(val.template getValue3x2<2, 2>(), ret));
        } else {
            auto ret(point.points.template shuffle3<0, 0, 0>().mad(
                val.template getValue3<0>(), val.template getValue3<3>()));
            ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(val.template getValue3<1>(), ret);
            return Point3D4Def<Packed>(
                point.points.template shuffle3<2, 2, 2>().template mad<false>(val.template getValue3<2>(), ret));
        }
    }

    /**
     * Transform a 3-D point using only the top 3x3 section of specified matrix.
     * @note Optimised transform that assumes an affine matrix with no translational component.
     * @param point The point to be transformed.
     * @returns A new transformed point.
     */
    XS_FUNCTION Point3DDef transform3x3(const Point3DDef& point) const noexcept
    {
        const typename SIMD4x4Def::SIMD3x4Def::SIMD3x3Def mat3(
            this->columns.template getValue3x4<0, 1, 2>().template getValue3x3<0, 1, 2>());
        SIMD3Def ret(mat3.template getValue3<0>() * point.values.template getValue<0>());
        ret = mat3.template getValue3<1>().template mad<false>(point.values.template getValue<1>(), ret);
        return Point3DDef(mat3.template getValue3<2>().template mad<false>(point.values.template getValue<2>(), ret));
    }

    /**
     * Transform two packed 3-D points using only the top 3x3 section of specified matrix.
     * @note Optimised transform that assumes an affine matrix with no translational component.
     * @param point The points to be transformed.
     * @returns New transformed points.
     */
    template<bool Packed>
    XS_FUNCTION Point3D2Def<Packed> transform3x3(const Point3D2Def<Packed>& point) const noexcept
    {
        const typename SIMD4x4Def::SIMD3x4Def::SIMD3x3Def mat3(
            this->columns.template getValue3x4<0, 1, 2>().template getValue3x3<0, 1, 2>());
        if constexpr (numValues<T, SIMD4x4Def::SIMD3x4Def::SIMD3x3Def::widthImpl> >= 6 && !Packed) {
            auto ret(point.points.template shuffle3<0, 0, 0>() * mat3.template getValue3x2<0, 0>());
            ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(mat3.template getValue3x2<1, 1>(), ret);
            return Point3D2Def<Packed>(
                point.points.template shuffle3<2, 2, 2>().template mad<false>(mat3.template getValue3x2<2, 2>(), ret));
        } else {
            auto ret(point.points.template shuffle3<0, 0, 0>() * mat3.template getValue3<0>());
            ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(mat3.template getValue3<1>(), ret);
            return Point3D2Def<Packed>(
                point.points.template shuffle3<2, 2, 2>().template mad<false>(mat3.template getValue3<2>(), ret));
        }
    }

    /**
     * Transform four packed 3-D points using only the top 3x3 section of specified matrix.
     * @note Optimised transform that assumes an affine matrix with no translational component.
     * @param point The points to be transformed.
     * @returns New transformed points.
     */
    template<bool Packed>
    XS_FUNCTION Point3D4Def<Packed> transform3x3(const Point3D4Def<Packed>& point) const noexcept
    {
        if constexpr (numValues<T, SIMD4x4Def::SIMD3x4Def::SIMD3x3Def::widthImpl> >= 12 && !Packed) {
            const typename SIMD4x4Def::SIMD3x4Def mat43(this->columns.template getValue3x4<0, 1, 2>());
            auto ret(point.points.template shuffle3<0, 0, 0>() * mat43.template shuffleH3<0, 0, 0, 0>());
            ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(
                mat43.template shuffleH3<1, 1, 1, 1>(), ret);
            return Point3D4Def<Packed>(point.points.template shuffle3<2, 2, 2>().template mad<false>(
                mat43.template shuffleH3<2, 2, 2, 2>(), ret));
        } else if constexpr (numValues<T, SIMD4x4Def::SIMD3x4Def::SIMD3x3Def::widthImpl> >= 6 && !Packed) {
            const typename SIMD4x4Def::SIMD3x4Def::SIMD3x3Def mat3(
                this->columns.template getValue3x4<0, 1, 2>().template getValue3x3<0, 1, 2>());
            auto ret(point.points.template shuffle3<0, 0, 0>() * mat3.template getValue3x2<0, 0>());
            ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(mat3.template getValue3x2<1, 1>(), ret);
            return Point3D4Def<Packed>(
                point.points.template shuffle3<2, 2, 2>().template mad<false>(mat3.template getValue3x2<2, 2>(), ret));
        } else {
            const typename SIMD4x4Def::SIMD3x4Def::SIMD3x3Def mat3(
                this->columns.template getValue3x4<0, 1, 2>().template getValue3x3<0, 1, 2>());
            auto ret(point.points.template shuffle3<0, 0, 0>() * mat3.template getValue3<0>());
            ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(mat3.template getValue3<1>(), ret);
            return Point3D4Def<Packed>(
                point.points.template shuffle3<2, 2, 2>().template mad<false>(mat3.template getValue3<2>(), ret));
        }
    }

    /**
     * Transpose transform a 3-D point using specified matrix.
     * @param point The point to be transformed.
     * @returns A new transpose transformed point.
     */
    XS_FUNCTION Point3DDef transformTransposed(const Point3DDef& point) const noexcept
    {
        return this->transpose().transform(point);
    }

    /**
     * Transpose transform a 3-D vector using specified matrix.
     * @param vector The vector to be transformed.
     * @returns A new transpose transformed vector.
     */
    XS_FUNCTION Vector3DDef transformTransposed(const Vector3DDef& vector) const noexcept
    {
        return this->transpose().transform(vector);
    }

    /**
     * Transpose affine transform a 3-D point using specified matrix.
     * @note Optimised based on the statement that the transposed matrix will be affine (i.e. row4 = 0,0,0,1).
     * @param point The point to be transformed.
     * @returns A new transpose transformed point.
     */
    XS_FUNCTION Point3DDef transformTransposedAffine(const Point3DDef& point) const noexcept
    {
        const typename SIMD4x4Def::SIMD3x4Def mat43(this->columns.transpose4().template getValue3x4<0, 1, 2>());
        SIMD3Def ret(
            mat43.template getValue3<0>().mad(point.values.template getValue<0>(), mat43.template getValue3<3>()));
        ret = mat43.template getValue3<1>().template mad<false>(point.values.template getValue<1>(), ret);
        return Point3DDef(mat43.template getValue3<2>().template mad<false>(point.values.template getValue<2>(), ret));
    }

    /**
     * Transpose transform a 3-D point using 3x3 component of specified matrix.
     * @note Optimised transform that assumes an affine matrix with no translational component.
     * @param point The point to be transformed.
     * @returns A new transpose transformed point.
     */
    XS_FUNCTION Point3DDef transformTransposed3x3(const Point3DDef& point) const noexcept
    {
        const typename SIMD4x4Def::SIMD3x4Def::SIMD3x3Def mat3(
            this->columns.template getValue3x4<0, 1, 2>().template getValue3x3<0, 1, 2>().transpose());
        SIMD3Def ret(mat3.template getValue3<0>() * point.values.template getValue<0>());
        ret = mat3.template getValue3<1>().template mad<false>(point.values.template getValue<1>(), ret);
        return Point3DDef(mat3.template getValue3<2>().template mad<false>(point.values.template getValue<2>(), ret));
    }

    /**
     * Determine the transpose of a matrix.
     * @returns A new transposed matrix.
     */
    XS_FUNCTION Matrix4x4 transpose() const noexcept
    {
        return Matrix4x4(this->columns.transpose4());
    }

    /**
     * Determine the inverse of a matrix.
     * @returns A new inverse matrix.
     */
    XS_FUNCTION Matrix4x4 inverse() const noexcept
    {
        // Determine inverse using Matrix partitioning such that
        // M=    [ P   Q ]
        //      [ R   S ]
        //
        // The matrix inverse can then be determined by:
        // M^-1= [ P_  Q_]
        //      [ R_  S_]
        //
        // where:
        //  P_ = P^-1 -(P^-1Q)R_
        //  Q_ = -(P^-1Q)S_
        //  R_ = -S_(RP^-1)
        //  S_ = (S-RP^-1Q)^-1

        const SIMD4Def col0(this->columns.template getValue4<0>());
        const SIMD4Def col1(this->columns.template getValue4<1>());
        const SIMD4Def col2(this->columns.template getValue4<2>());
        const SIMD4Def col3(this->columns.template getValue4<3>());

        // determine |P|
        using SIMD2Def = typename SIMD4Def::SIMD2Def;
        const SIMD2Def detP(col0.template getValue2<0>() * col1.template getValue2<0>().template shuffle<1, 0>());
        const typename SIMD2Def::InBaseDef detP2(detP.hsubInBase());

        // if |P|==0 then we can not determine inverse using this method

        // determine P^-1
        const SIMD4Def adjointP(col0.template combine<5, 1, 4, 0>(col1));
        const SIMD4Def invP(adjointP.template negate<false, true, true, false>() / SIMD4Def::BaseDef(detP2));

        // determine RP^-1
        const SIMD4Def rInvP(
            (col0.template shuffle<2, 3, 2, 3>().template mad<false>(invP.template shuffle<0, 0, 2, 2>(),
                col1.template shuffle<2, 3, 2, 3>() * invP.template shuffle<1, 1, 3, 3>())));

        // determine P^-1Q
        const SIMD4Def q(col2.template combine<0, 1, 4, 5>(col3));
        const SIMD4Def invPQ((invP.template shuffle<0, 1, 0, 1>().template mad<false>(
            q.template shuffle<0, 0, 2, 2>(), invP.template shuffle<2, 3, 2, 3>() * q.template shuffle<1, 1, 3, 3>())));

        // determine S-RP^-1Q
        const SIMD4Def s(col2.template combine<2, 3, 6, 7>(col3));
        const SIMD4Def sRInvPQ(
            rInvP.template shuffle<0, 1, 0, 1>().template nmad<false>(q.template shuffle<0, 0, 2, 2>(), s) -
            (rInvP.template shuffle<2, 3, 2, 3>() * q.template shuffle<1, 1, 3, 3>()));

        // determine |S-RP^-1Q|
        const SIMD2Def detPSRinvPQ(
            sRInvPQ.template getValue2<0>() * sRInvPQ.template getValue2<1>().template shuffle<1, 0>());
        const InBaseDef detSRinvPQ(detPSRinvPQ.hsubInBase());

        // if |S-RP^-1Q|==0 then matrix is non-invertable

        // determine inverse S_ = (S-RP^-1Q)^-1
        const SIMD4Def s2(sRInvPQ.template shuffle<3, 1, 2, 0>().template negate<false, true, true, false>() /
            SIMD4Def::BaseDef(detSRinvPQ));

        // determine R_ = -S_(RP^-1)
        const SIMD4Def negS2(-s2);
        const SIMD4Def r2(negS2.template shuffle<0, 1, 0, 1>().template mad<false>(rInvP.template shuffle<0, 0, 2, 2>(),
            negS2.template shuffle<2, 3, 2, 3>() * rInvP.template shuffle<1, 1, 3, 3>()));

        // determine Q_ = -(P^-1Q)S_
        const SIMD4Def invPQ01(invPQ.template shuffle<0, 1, 0, 1>());
        const SIMD4Def invPQ23(invPQ.template shuffle<2, 3, 2, 3>());
        const SIMD4Def q2(
            (invPQ01.mad(negS2.template shuffle<0, 0, 2, 2>(), invPQ23 * negS2.template shuffle<1, 1, 3, 3>())));

        // determine P_ = P^-1 -(P^-1Q)R_
        const SIMD4Def p2(invPQ01.template nmad<false>(r2.template shuffle<0, 0, 2, 2>(), invP) -
            (invPQ23 * r2.template shuffle<1, 1, 3, 3>()));

        // Resort into columns of new matrix
        return Matrix4x4(p2.template combine<0, 1, 4, 5>(r2), p2.template combine<2, 3, 6, 7>(r2),
            q2.template combine<0, 1, 4, 5>(s2), q2.template combine<2, 3, 6, 7>(s2));
    }

    /**
     * Determine the inverse of an affine matrix.
     * @note Optimised based on the statement that the matrix is affine (i.e. row4 = 0,0,0,1).
     * @returns A new inverse matrix.
     */
    XS_FUNCTION Matrix4x4 inverseAffine() const noexcept
    {
        // Inverse of affine matrix M=[    A     b ]  can be determined using:
        //                           [  0,..0   1 ]
        //                      M^-1=[  A^-1   -A^-1b ]
        //                           [  0,..0    1    ]

        // determine adjoint transpose matrix
        using SIMD3x3Def = typename SIMD4x4Def::SIMD3x4Def::SIMD3x3Def;
        const SIMD3x3Def mat3(this->columns.template getValue3x4<0, 1, 2>().template getValue3x3<0, 1, 2>());
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
        const SIMD3Def col3(this->columns.template getValue4<3>().template getValue3<0, 1, 2>());
        SIMD3Def ret(adjoint.template getValue3<0>() * col3.template getValue<0>());
        ret = adjoint.template getValue3<1>().template mad<false>(col3.template getValue<1>(), ret);
        ret = adjoint.template getValue3<2>().template mad<false>(col3.template getValue<2>(), ret);

        return Matrix4x4(SIMD4x4Def(SIMD4x4Def::SIMD3x4Def(adjoint, -ret), SIMD4Def(0.0f, 0.0f, 0.0f, 1.0f)));
    }

    /**
     * Determine the inverse of an orthogonal matrix.
     * @note Optimised based on the statement that the matrix is orthogonal (i.e. det(M)=+-1 and cols/rown are
     * orthogonal unit vectors). These matrices are found in pure rotation matrices.
     * @returns A new inverse matrix.
     */
    XS_FUNCTION Matrix4x4 inverseOrthogonal() const noexcept
    {
        return this->transpose();
    }

    /**
     * Determine the determinant of an matrix.
     * @returns The determinant in a BaseDef.
     */
    XS_FUNCTION BaseDef determinant() const noexcept
    {
        using SIMD12Def = typename SIMD4x4Def::SIMD12Def;
        const SIMD12Def cols(this->columns.template getValue4x3<1, 2, 3>());
        if constexpr (numValues<T, SIMD4x4Def::widthImpl> >= 16) {
            SIMD12Def ret(
                cols.template shuffleH4<1, 1, 0>().template shuffle4<2, 3, 0, 1>() * cols.template shuffle3<2, 0, 2>());
            ret -= ret.template shuffle4<2, 3, 0, 1>();
            ret *= cols.template shuffleH4<0, 2, 1>().template shuffle4<3, 2, 1, 0>();
            SIMD4Def ret2(ret.sub4());
            ret2 *= this->columns.template getValue4<0>().template shuffle<1, 0, 3, 2>();
            return BaseDef(ret2.hadd());
        } else {
            const typename SIMD12Def::SIMD8Def rets(cols.template getValue4x2<0, 1>().template shuffle4<2, 3, 0, 1>());
            SIMD12Def ret(SIMD12Def(rets.template shuffleH4<1, 1>(), rets.template getValue4<0>()) *
                cols.template shuffle3<2, 0, 2>());
            ret -= ret.template shuffle4<2, 3, 0, 1>();
            ret *= cols.template shuffleH4<0, 2, 1>().template shuffle4<3, 2, 1, 0>();
            SIMD4Def ret2(ret.sub4());
            ret2 *= this->columns.template getValue4<0>().template shuffle<1, 0, 3, 2>();
            return BaseDef(ret2.hadd());
        }
    }

    /**
     * Determine the determinant of an matrix.
     * @returns The determinant in a InBaseDef.
     */
    XS_FUNCTION InBaseDef determinantInBase() const noexcept
    {
        using SIMD12Def = typename SIMD4x4Def::SIMD12Def;
        const SIMD12Def cols(this->columns.template getValue4x3<1, 2, 3>());
        if constexpr (numValues<T, SIMD4x4Def::widthImpl> >= 16) {
            SIMD12Def ret(
                cols.template shuffleH4<1, 1, 0>().template shuffle4<2, 3, 0, 1>() * cols.template shuffle3<2, 0, 2>());
            ret -= ret.template shuffle4<2, 3, 0, 1>();
            ret *= cols.template shuffleH4<0, 2, 1>().template shuffle4<3, 2, 1, 0>();
            SIMD4Def ret2(ret.sub4());
            ret2 *= this->columns.template getValue4<0>().template shuffle<1, 0, 3, 2>();
            return ret2.haddInBase();
        } else {
            const typename SIMD12Def::SIMD8Def rets(cols.template getValue4x2<0, 1>().template shuffle4<2, 3, 0, 1>());
            SIMD12Def ret(SIMD12Def(rets.template shuffleH4<1, 1>(), rets.template getValue4<0>()) *
                cols.template shuffle3<2, 0, 2>());
            ret -= ret.template shuffle4<2, 3, 0, 1>();
            ret *= cols.template shuffleH4<0, 2, 1>().template shuffle4<3, 2, 1, 0>();
            SIMD4Def ret2(ret.sub4());
            ret2 *= this->columns.template getValue4<0>().template shuffle<1, 0, 3, 2>();
            return ret2.haddInBase();
        }
    }

    /**
     * Fast multiply the matrix by a rotation around x axis (i.e M*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix4x4 postRotateX(typename SIMD4Def::BaseDef rotation) const noexcept
    {
        using SIMD4x2Def = typename SIMD4x4Def::SIMD8Def;
        using BaseDef4 = typename SIMD4Def::BaseDef;
        BaseDef4 cos;
        const BaseDef4 sin(sincos(rotation, cos));
        SIMD4x4Def ret(this->columns);
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            ret.template setValue4x2<1, 2>(
                this->columns.template getValue4x2<1, 2>().template mad<false>(SIMD4x2Def::BaseDef(cos),
                    this->columns.template getValue4x2<2, 1>() *
                        SIMD4x2Def(SIMD4x2Def::BaseDef(sin))
                            .template negate<false, false, false, false, true, true, true, true>()));
        } else {
            ret.template setValue4x2<1, 2>(
                this->columns.template getValue4x2<1, 2>().template mad<false>(SIMD4x2Def::BaseDef(cos),
                    this->columns.template getValue4x2<2, 1>().mulH4(
                        SIMD4x2Def::SIMD2Def(sin).template negate<false, true>())));
        }
        return Matrix4x4(ret);
    }

    /**
     * Fast multiply the matrix by a rotation around y axis (i.e M*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix4x4 postRotateY(typename SIMD4Def::BaseDef rotation) const noexcept
    {
        using SIMD4x2Def = typename SIMD4x4Def::SIMD8Def;
        using BaseDef4 = typename SIMD4Def::BaseDef;
        BaseDef4 cos;
        const BaseDef4 sin(sincos(rotation, cos));
        SIMD4x4Def ret(this->columns);
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            ret.template setValue4x2<0, 2>(
                this->columns.template getValue4x2<0, 2>().template mad<false>(SIMD4x2Def::BaseDef(cos),
                    this->columns.template getValue4x2<2, 0>() *
                        SIMD4x2Def(SIMD4x2Def::BaseDef(sin))
                            .template negate<true, true, true, true, false, false, false, false>()));
        } else {
            ret.template setValue4x2<0, 2>(
                this->columns.template getValue4x2<0, 2>().template mad<false>(SIMD4x2Def::BaseDef(cos),
                    this->columns.template getValue4x2<2, 0>().mulH4(
                        SIMD4x2Def::SIMD2Def(sin).template negate<true, false>())));
        }
        return Matrix4x4(ret);
    }

    /**
     * Fast multiply the matrix by a rotation around z axis (i.e M*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix4x4 postRotateZ(typename SIMD4Def::BaseDef rotation) const noexcept
    {
        using SIMD4x2Def = typename SIMD4x4Def::SIMD8Def;
        using BaseDef4 = typename SIMD4Def::BaseDef;
        BaseDef4 cos;
        const BaseDef4 sin(sincos(rotation, cos));
        SIMD4x4Def ret(this->columns);
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            ret.template setValue4x2<0, 1>(
                this->columns.template getValue4x2<0, 1>().template mad<false>(SIMD4x2Def::BaseDef(cos),
                    this->columns.template getValue4x2<1, 0>() *
                        SIMD4x2Def(SIMD4x2Def::BaseDef(sin))
                            .template negate<false, false, false, false, true, true, true, true>()));
        } else {
            ret.template setValue4x2<0, 1>(
                this->columns.template getValue4x2<0, 1>().template mad<false>(SIMD4x2Def::BaseDef(cos),
                    this->columns.template getValue4x2<1, 0>().mulH4(
                        SIMD4x2Def::SIMD2Def(sin).template negate<false, true>())));
        }
        return Matrix4x4(ret);
    }

    /**
     * Fast multiply a rotation around x axis by the matrix (i.e rotation*M).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix4x4 preRotateX(typename SIMD4Def::BaseDef rotation) const noexcept
    {
        using SIMD4x2Def = typename SIMD4x4Def::SIMD8Def;
        using BaseDef4 = typename SIMD4Def::BaseDef;
        // Use R*M = transpose(transpose(M)*transpose(R))
        BaseDef4 cos;
        const BaseDef4 sin(sincos(rotation, cos));
        SIMD4x4Def ret(this->transpose().columns);
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            ret.template setValue4x2<1, 2>(
                ret.template getValue4x2<1, 2>().template mad<false>(SIMD4x2Def::BaseDef(cos),
                    ret.template getValue4x2<2, 1>() *
                        SIMD4x2Def(SIMD4x2Def::BaseDef(sin))
                            .template negate<true, true, true, true, false, false, false, false>()));
        } else {
            ret.template setValue4x2<1, 2>(
                ret.template getValue4x2<1, 2>().template mad<false>(SIMD4x2Def::BaseDef(cos),
                    ret.template getValue4x2<2, 1>().mulH4(SIMD4x2Def::SIMD2Def(sin).template negate<true, false>())));
        }
        return Matrix4x4(ret).transpose();
    }

    /**
     * Fast multiply a rotation around y axis by the matrix (i.e rotation*M).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix4x4 preRotateY(typename SIMD4Def::BaseDef rotation) const noexcept
    {
        using SIMD4x2Def = typename SIMD4x4Def::SIMD8Def;
        using BaseDef4 = typename SIMD4Def::BaseDef;
        // Use R*M = transpose(transpose(M)*transpose(R))
        BaseDef4 cos;
        const BaseDef4 sin(sincos(rotation, cos));
        SIMD4x4Def ret(this->transpose().columns);
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            ret.template setValue4x2<0, 2>(
                ret.template getValue4x2<0, 2>().template mad<false>(SIMD4x2Def::BaseDef(cos),
                    ret.template getValue4x2<2, 0>() *
                        SIMD4x2Def(SIMD4x2Def::BaseDef(sin))
                            .template negate<false, false, false, false, true, true, true, true>()));
        } else {
            ret.template setValue4x2<0, 2>(
                ret.template getValue4x2<0, 2>().template mad<false>(SIMD4x2Def::BaseDef(cos),
                    ret.template getValue4x2<2, 0>().mulH4(SIMD4x2Def::SIMD2Def(sin).template negate<false, true>())));
        }
        return Matrix4x4(ret).transpose();
    }

    /**
     * Fast multiply a rotation around z axis by the matrix (i.e rotation*M).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix4x4 preRotateZ(typename SIMD4Def::BaseDef rotation) const noexcept
    {
        using SIMD4x2Def = typename SIMD4x4Def::SIMD8Def;
        using BaseDef4 = typename SIMD4Def::BaseDef;
        // Use R*M = transpose(transpose(M)*transpose(R))
        BaseDef4 cos;
        const BaseDef4 sin(sincos(rotation, cos));
        SIMD4x4Def ret(this->transpose().columns);
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            ret.template setValue4x2<0, 1>(
                ret.template getValue4x2<0, 1>().template mad<false>(SIMD4x2Def::BaseDef(cos),
                    ret.template getValue4x2<1, 0>() *
                        SIMD4x2Def(SIMD4x2Def::BaseDef(sin))
                            .template negate<true, true, true, true, false, false, false, false>()));
        } else {
            ret.template setValue4x2<0, 1>(
                ret.template getValue4x2<0, 1>().template mad<false>(SIMD4x2Def::BaseDef(cos),
                    ret.template getValue4x2<1, 0>().mulH4(SIMD4x2Def::SIMD2Def(sin).template negate<true, false>())));
        }
        return Matrix4x4(ret).transpose();
    }

    /**
     * Fast multiply the matrix by a scale (i.e M*scale).
     * @param scale The amount to scale along X/Y/Z-axis.
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix4x4 postScale(const SIMD3Def& scale) const noexcept
    {
        return Matrix4x4(SIMD4x4Def(this->columns.template getValue4x3<0, 1, 2>() * scale, this->getColumn<3>()));
    }

    /**
     * Fast multiply a scale by a matrix (i.e scale*M).
     * @param scale The 3 scale values for each axis.
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix4x4 preScale(const SIMD3Def& scale) const noexcept
    {
        return Matrix4x4(this->columns * SIMD4Def(scale, SIMD4Def::BaseDef::One()));
    }

    /**
     * Fast multiply the matrix by a uniform scale (i.e M*scale).
     * @param scale The amount to scale along all axis.
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix4x4 postUniformScale(BaseDef scale) const noexcept
    {
        return Matrix4x4(SIMD4x4Def(this->columns.template getValue4x3<0, 1, 2>() * scale, this->getColumn<3>()));
    }

    /**
     * Fast multiply the a uniform scale by a matrix (i.e scale*M).
     * @param scale The amount to scale along all axis.
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix4x4 preUniformScale(BaseDef scale) const noexcept
    {
        if constexpr (numValues<T, SIMD4x4Def::widthImpl> >= 16) {
            return Matrix4x4(
                this->columns * SIMD4x4Def(scale).template blend4<false, false, false, true>(SIMD4x4Def::One()));
        } else if constexpr (numValues<T, SIMD4x4Def::widthImpl> >= 8) {
            return Matrix4x4(this->columns *
                SIMD4x4Def::SIMD8Def(scale).template blend4<false, false, false, true>(SIMD4x4Def::SIMD8Def::One()));
        } else {
            return Matrix4x4(this->columns * SIMD4Def(SIMD4Def::SIMD3Def(scale), SIMD4Def::BaseDef::One()));
        }
    }

    /**
     * Fast multiply the matrix by a translation (i.e M*translate).
     * @param translation The amount to translate in the X/Y/Z directions.
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix4x4 postTranslate(const SIMD3Def& translation) const noexcept
    {
        const SIMD4Def ret((this->columns.template getValue4x3<0, 1, 2>() * translation).add4());
        return Matrix4x4(SIMD4x4Def(this->columns.template getValue4x3<0, 1, 2>(), this->getColumn<3>() + ret));
    }

    /**
     * Fast multiply a translation by a matrix (i.e translate*M).
     * @param translation The 3 translate values for each axis.
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix4x4 preTranslate(const SIMD3Def& translation) const noexcept
    {
        const SIMD4Def f4Translate(translation, SIMD4Def::BaseDef::Zero());
        return Matrix4x4(this->columns.template shuffle4<3, 3, 3, 3>().mad(f4Translate, this->columns));
    }
};

/**
 * Add two 4x4 Matrices.
 * @param matrix1 The first matrix.
 * @param matrix2 Matrix to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Matrix4x4<T, Width> operator+(
    const Matrix4x4<T, Width>& matrix1, const Matrix4x4<T, Width>& matrix2) noexcept
{
    return Matrix4x4<T, Width>(matrix1.columns + matrix2.columns);
}

/**
 * Subtract a 4x4 Matrix from another 4x4 Matrix.
 * @param matrix1 The first matrix.
 * @param matrix2 Matrix to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Matrix4x4<T, Width> operator-(
    const Matrix4x4<T, Width>& matrix1, const Matrix4x4<T, Width>& matrix2) noexcept
{
    return Matrix4x4<T, Width>(matrix1.columns - matrix2.columns);
}

/**
 * Multiply two 4x4 Matrices together.
 * @param matrix1 The first matrix.
 * @param matrix2 Matrix to multiply the first by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Matrix4x4<T, Width> operator*(
    const Matrix4x4<T, Width>& matrix1, const Matrix4x4<T, Width>& matrix2) noexcept
{
    if constexpr (numValues<T, Matrix4x4<T, Width>::SIMD4x4Def::widthImpl> >= 16) {
        typename Matrix4x4<T, Width>::SIMD4x4Def ret(
            matrix2.columns.template shuffle4<3, 3, 3, 3>() * matrix1.columns.template shuffleH4<3, 3, 3, 3>());
        ret = matrix2.columns.template shuffle4<2, 2, 2, 2>().template mad<false>(
            matrix1.columns.template shuffleH4<2, 2, 2, 2>(), ret);
        ret = matrix2.columns.template shuffle4<1, 1, 1, 1>().template mad<false>(
            matrix1.columns.template shuffleH4<1, 1, 1, 1>(), ret);
        return Matrix4x4<T, Width>(matrix2.columns.template shuffle4<0, 0, 0, 0>().template mad<false>(
            matrix1.columns.template shuffleH4<0, 0, 0, 0>(), ret));
    } else if constexpr (numValues<T, Matrix4x4<T, Width>::SIMD4x4Def::widthImpl> >= 8) {
        typename Matrix4x4<T, Width>::SIMD4x4Def ret(
            matrix2.columns.template shuffle4<3, 3, 3, 3>() * matrix1.columns.template getValue4x2<3, 3>());
        ret = matrix2.columns.template shuffle4<2, 2, 2, 2>().template mad<false>(
            matrix1.columns.template getValue4x2<2, 2>(), ret);
        ret = matrix2.columns.template shuffle4<1, 1, 1, 1>().template mad<false>(
            matrix1.columns.template getValue4x2<1, 1>(), ret);
        return Matrix4x4<T, Width>(matrix2.columns.template shuffle4<0, 0, 0, 0>().template mad<false>(
            matrix1.columns.template getValue4x2<0, 0>(), ret));
    } else {
        typename Matrix4x4<T, Width>::SIMD4x4Def ret(
            matrix2.columns.template shuffle4<3, 3, 3, 3>() * matrix1.columns.template getValue4<3>());
        ret = matrix2.columns.template shuffle4<2, 2, 2, 2>().template mad<false>(
            matrix1.columns.template getValue4<2>(), ret);
        ret = matrix2.columns.template shuffle4<1, 1, 1, 1>().template mad<false>(
            matrix1.columns.template getValue4<1>(), ret);
        return Matrix4x4<T, Width>(matrix2.columns.template shuffle4<0, 0, 0, 0>().template mad<false>(
            matrix1.columns.template getValue4<0>(), ret));
    }
}

/**
 * Perform compound assignment and addition with a 4x4 Matrix.
 * @param [in,out] matrix1 The first matrix.
 * @param          matrix2 Matrix to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Matrix4x4<T, Width>& operator+=(Matrix4x4<T, Width>& matrix1, const Matrix4x4<T, Width>& matrix2) noexcept
{
    matrix1 = matrix1 + matrix2;
    return matrix1;
}

/**
 * Perform compound assignment and subtraction with a 4x4 Matrix.
 * @param [in,out] matrix1 The first matrix.
 * @param          matrix2 Matrix to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Matrix4x4<T, Width>& operator-=(Matrix4x4<T, Width>& matrix1, const Matrix4x4<T, Width>& matrix2) noexcept
{
    matrix1 = matrix1 - matrix2;
    return matrix1;
}

/**
 * Perform compound assignment and multiplication with a 4x4 Matrix.
 * @param [in,out] matrix1 The first matrix.
 * @param          matrix2 Matrix to multiply the first by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Matrix4x4<T, Width>& operator*=(Matrix4x4<T, Width>& matrix1, const Matrix4x4<T, Width>& matrix2) noexcept
{
    matrix1 = matrix1 * matrix2;
    return matrix1;
}
} // namespace Shift
