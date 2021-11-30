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

#include "Geometry/XSQuaternion.hpp"
#include "SIMD/XSSIMD3x4.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class Matrix4x3;
template<typename T, SIMDWidth Width>
class Matrix4x4;

template<typename T>
class Matrix4x3Data
{
public:
    SIMD3x4Data<T> values;

    /** Default constructor. */
    XS_INLINE Matrix4x3Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit Matrix4x3Data(const Matrix4x3<T, Width>& other) noexcept
        : values(other.columns)
    {}

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param col0Row0 The first element of the first column of the matrix.
     * @param col0Row1 The second element of the first column of the matrix.
     * @param col0Row2 The third element of the first column of the matrix.
     * @param col1Row0 The first element of the second column of the matrix.
     * @param col1Row1 The second element of the second column of the matrix.
     * @param col1Row2 The third element of the second column of the matrix.
     * @param col2Row0 The first element of the third column of the matrix.
     * @param col2Row1 The second element of the third column of the matrix.
     * @param col2Row2 The third element of the third column of the matrix.
     * @param col3Row0 The first element of the fourth column of the matrix.
     * @param col3Row1 The second element of the fourth column of the matrix.
     * @param col3Row2 The third element of the fourth column of the matrix.
     */
    XS_INLINE void setData(T col0Row0, T col0Row1, T col0Row2, T col1Row0, T col1Row1, T col1Row2, T col2Row0,
        T col2Row1, T col2Row2, T col3Row0, T col3Row1, T col3Row2) noexcept
    {
        this->values.setData(col0Row0, col0Row1, col0Row2, col1Row0, col1Row1, col1Row2, col2Row0, col2Row1, col2Row2,
            col3Row0, col3Row1, col3Row2);
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const Matrix4x3<T, Width>& other) noexcept
    {
        this->values.store(other.columns);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_INLINE Matrix4x3<T, Width> load() const noexcept
    {
        return Matrix4x3<T, Width>(this->values.template load<Matrix4x3<T, Width>::widthImpl>());
    }
};

template<typename T>
class Matrix4x3DataPad
{
public:
    SIMD3x4DataPad<T> values;

    /** Default constructor. */
    XS_INLINE Matrix4x3DataPad() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit Matrix4x3DataPad(const Matrix4x3<T, Width>& other) noexcept
        : values(other.columns)
    {}

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param col0Row0 The first element of the first column of the matrix.
     * @param col0Row1 The second element of the first column of the matrix.
     * @param col0Row2 The third element of the first column of the matrix.
     * @param col1Row0 The first element of the second column of the matrix.
     * @param col1Row1 The second element of the second column of the matrix.
     * @param col1Row2 The third element of the second column of the matrix.
     * @param col2Row0 The first element of the third column of the matrix.
     * @param col2Row1 The second element of the third column of the matrix.
     * @param col2Row2 The third element of the third column of the matrix.
     * @param col3Row0 The first element of the fourth column of the matrix.
     * @param col3Row1 The second element of the fourth column of the matrix.
     * @param col3Row2 The third element of the fourth column of the matrix.
     */
    XS_INLINE void setData(T col0Row0, T col0Row1, T col0Row2, T col1Row0, T col1Row1, T col1Row2, T col2Row0,
        T col2Row1, T col2Row2, T col3Row0, T col3Row1, T col3Row2) noexcept
    {
        this->values.setData(col0Row0, col0Row1, col0Row2, col1Row0, col1Row1, col1Row2, col2Row0, col2Row1, col2Row2,
            col3Row0, col3Row1, col3Row2);
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const Matrix4x3<T, Width>& other) noexcept
    {
        this->values.store(other.columns);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_INLINE Matrix4x3<T, Width> load() const noexcept
    {
        return Matrix4x3<T, Width>(this->values.template load<Matrix4x3<T, Width>::widthImpl>());
    }
};

/**
 * A  base type used to represent a 4x3 matrix. This is a specialised version of Matrix4x4 designed specially for
 * affine matrices with a translational component.
 * @note This uses standard right handed coordinate notation
 *    i.e. Vector (V) transforms with Matrix (M) occur in the following order:
 *         V2 = M * V
 *    Matrix multiplications between 2 matrices (M1, M2) gives the following ordering:
 *         M3 = M1 * M2        V2 = M3 * V    equivalent to->  V2 = M1 * (M2 * V)
 * @tparam T     Generic type parameter.
 * @tparam Width Type of SIMD being used.
 */
template<typename T, SIMDWidth Width = widthSIMD<T>>
class Matrix4x3
{
public:
    using Type = T;
    using SIMD3x4Def = SIMD3x4<T, SIMD3x4<T, Width>::widthImpl>;
    using Data = Matrix4x3Data<T>;
    using DataPad = Matrix4x3DataPad<T>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = SIMD3x4Def::widthImpl;
    using BaseDef = typename SIMD3x4Def::BaseDef;
    using InBaseDef = typename SIMD3x4Def::InBaseDef;
    using SIMD3Def = typename SIMD3x4Def::SIMD3Def;
    using SIMD4Def = typename SIMD3x4Def::SIMD4Def;
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
    using Matrix4x4Def = Matrix4x4<T, Matrix4x4<T, widthImpl>::widthImpl>;
    using Matrix3x3Def = Matrix3x3<T, Matrix3x3<T, widthImpl>::widthImpl>;
    using QuaternionDef = Quaternion<T, Quaternion<T, widthImpl>::widthImpl>;

    SIMD3x4Def columns;

    /** Default constructor. */
    XS_INLINE Matrix4x3() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Matrix4x3(const Matrix4x3& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE Matrix4x3(Matrix4x3&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Matrix4x3& operator=(const Matrix4x3& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Matrix4x3& operator=(Matrix4x3&& other) noexcept = default;

    /**
     * Construct a 4x3 matrix from a SIMD3x4.
     * @param values The 3 columns of the matrix.
     */
    XS_INLINE explicit Matrix4x3(const SIMD3x4Def& values) noexcept
        : columns(values)
    {}

    /**
     * Construct a 4x3 matrix from 4 SIMD3s.
     * @param column0 The first column of the matrix.
     * @param column1 The second column of the matrix.
     * @param column2 The third column of the matrix.
     * @param column3 The fourth column of the matrix.
     */
    XS_INLINE Matrix4x3(
        const SIMD3Def& column0, const SIMD3Def& column1, const SIMD3Def& column2, const SIMD3Def& column3) noexcept
        : columns(column0, column1, column2, column3)
    {}

    /**
     * Construct a 4x3 matrix from 12 values.
     * @param col0Row0 The first element of the first column of the matrix.
     * @param col0Row1 The second element of the first column of the matrix.
     * @param col0Row2 The third element of the first column of the matrix.
     * @param col1Row0 The first element of the second column of the matrix.
     * @param col1Row1 The second element of the second column of the matrix.
     * @param col1Row2 The third element of the second column of the matrix.
     * @param col2Row0 The first element of the third column of the matrix.
     * @param col2Row1 The second element of the third column of the matrix.
     * @param col2Row2 The third element of the third column of the matrix.
     * @param col3Row0 The first element of the fourth column of the matrix.
     * @param col3Row1 The second element of the fourth column of the matrix.
     * @param col3Row2 The third element of the fourth column of the matrix.
     */
    XS_INLINE Matrix4x3(T col0Row0, T col0Row1, T col0Row2, T col1Row0, T col1Row1, T col1Row2, T col2Row0, T col2Row1,
        T col2Row2, T col3Row0, T col3Row1, T col3Row2) noexcept
        : columns(col0Row0, col0Row1, col0Row2, col1Row0, col1Row1, col1Row2, col2Row0, col2Row1, col2Row2, col3Row0,
              col3Row1, col3Row2)
    {}

    /**
     * Construct from a Matrix4x4.
     * @param matrix Reference to matrix to copy.
     */
    XS_INLINE explicit Matrix4x3(const Matrix4x4Def& matrix) noexcept
        : columns(matrix.columns.template getValue3x4<0, 1, 2>())
    {}

    /**
     * Construct from a Matrix3x3.
     * @param matrix Reference to matrix to copy.
     */
    XS_INLINE explicit Matrix4x3(const Matrix3x3Def& matrix) noexcept
        : columns(matrix.columns, SIMD3Def::Zero())
    {}

    /**
     * Construct from a Matrix3x3.
     * @param matrix Reference to matrix to copy.
     * @param column3 The fourth column of the matrix.
     */
    XS_INLINE Matrix4x3(const Matrix3x3Def& matrix, const SIMD3Def& column3) noexcept
        : columns(matrix.columns, column3)
    {}

    /**
     * Construct from a Quaternion.
     * @param quaternion Reference to quaternion to copy.
     * @param column3    The fourth column of the matrix.
     */
    XS_INLINE Matrix4x3(const QuaternionDef& quaternion, const SIMD3Def& column3) noexcept
        : columns(Matrix3x3Def(quaternion).columns, column3)
    {}

    /**
     * Construct a Matrix3x3 from a Matrix4x3.
     * @return Newly constructed object.
     */
    XS_INLINE explicit operator Matrix3x3Def() const noexcept
    {
        return Matrix3x3Def(this->columns.template getValue3x3<0, 1, 2>());
    }

    /**
     * Constructor to build a 4x3 Identity Matrix.
     * @returns Newly constructed Matrix4x3 with required attributes.
     */
    XS_INLINE static Matrix4x3 Identity() noexcept
    {
        return Matrix4x3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
    }

    /**
     * Constructor to build a 4x3 Matrix with fixed translation.
     * @param translation The amount to translate in the X/Y/Z directions.
     * @returns Newly constructed Matrix4x3 with required attributes.
     */
    XS_INLINE static Matrix4x3 Translation(const SIMD3Def& translation) noexcept
    {
        return Matrix4x3(
            SIMD3x4Def(SIMD3x4Def::SIMD3x3Def(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f), translation));
    }

    /**
     * Constructor to build a 4x3 Matrix with fixed uniform scale.
     * @param scale The amount to scale in the X/Y/Z direction.
     * @returns Newly constructed Matrix4x3 with required attributes.
     */
    XS_INLINE static Matrix4x3 UniformScale(InBaseDef scale) noexcept
    {
        const SIMD3Def zero(SIMD3Def::Zero());
        SIMD3Def scale3(zero);
        scale3.template setValue<0>(scale);
        return Matrix4x3(scale3, scale3.template shuffle<1, 0, 2>(), scale3.template shuffle<1, 2, 0>(), zero);
    }

    /**
     * Constructor to build a 4x3 Matrix with fixed scale.
     * @param scale The amount to scale in the X/Y/Z direction.
     * @returns Newly constructed Matrix4x3 with required attributes.
     */
    XS_INLINE static Matrix4x3 Scale(const SIMD3Def& scale) noexcept
    {
        const SIMD4Def scale4(scale, SIMD4Def::BaseDef::Zero());
        return Matrix4x3(scale4.template getValue3<0, 3, 3>(), scale4.template getValue3<3, 1, 3>(),
            scale4.template getValue3<3, 3, 2>(), SIMD3Def::Zero());
    }

    /**
     * Constructor to build a 4x3 Matrix with fixed rotation about X axis.
     * @param rotation The amount to rotate the matrix by (in radians).
     * @returns Newly constructed Matrix4x3 with required attributes.
     */
    XS_INLINE static Matrix4x3 RotationX(typename SIMD3Def::BaseDef rotation) noexcept
    {
        using SIMD2Def = typename SIMD3Def::SIMD2Def;
        using BaseDef2 = typename SIMD2Def::BaseDef;
        SIMD2Def sinCos;
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            sinCos = sin(SIMD2Def(rotation, rotation + BaseDef2(valPi2<T>)));
        } else {
            BaseDef2 cos;
            const BaseDef2 sin(sincos(rotation, cos));
            sinCos = SIMD2Def(sin, cos);
        }
        const SIMD3Def ident(1.0, 0.0, 0.0f);
        const SIMD3Def sinCos3(sinCos, SIMD3Def::InBaseDef::Zero());
        return Matrix4x3(ident, sinCos3.template shuffle<2, 1, 0>(),
            sinCos3.template shuffle<2, 0, 1>().template negate<false, true, false>(), SIMD3Def::Zero());
    }

    /**
     * Constructor to build a 4x3 Matrix with fixed rotation about Y axis.
     * @param rotation The amount to rotate the matrix by (in radians).
     * @returns Newly constructed Matrix4x3 with required attributes.
     */
    XS_INLINE static Matrix4x3 RotationY(typename SIMD3Def::BaseDef rotation) noexcept
    {
        using SIMD2Def = typename SIMD3Def::SIMD2Def;
        using BaseDef2 = typename SIMD2Def::BaseDef;
        SIMD2Def sinCos;
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            sinCos = sin(SIMD2Def(rotation, rotation + BaseDef2(valPi2<T>)));
        } else {
            BaseDef2 cos;
            const BaseDef2 sin(sincos(rotation, cos));
            sinCos = SIMD2Def(sin, cos);
        }
        const SIMD3Def col2(0.0f, 1.0f, 0.0f);
        const SIMD3Def sinCos3(sinCos, SIMD3Def::InBaseDef::Zero());
        return Matrix4x3(sinCos3.template shuffle<1, 2, 0>().template negate<false, false, true>(), col2,
            sinCos3.template shuffle<0, 2, 1>(), SIMD3Def::Zero());
    }

    /**
     * Constructor to build a 4x3 Matrix with fixed rotation about Z axis.
     * @param rotation The amount to rotate the matrix by (in radians).
     * @returns Newly constructed Matrix4x3 with required attributes.
     */
    XS_INLINE static Matrix4x3 RotationZ(typename SIMD3Def::BaseDef rotation) noexcept
    {
        using SIMD2Def = typename SIMD3Def::SIMD2Def;
        using BaseDef2 = typename SIMD2Def::BaseDef;
        SIMD2Def sinCos;
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            sinCos = sin(SIMD2Def(rotation, rotation + BaseDef2(valPi2<T>)));
        } else {
            BaseDef2 cos;
            const BaseDef2 sin(sincos(rotation, cos));
            sinCos = SIMD2Def(sin, cos);
        }
        const SIMD3Def col3(0.0f, 0.0f, 1.0f);
        const SIMD3Def sinCos3(sinCos, SIMD3Def::InBaseDef::Zero());
        return Matrix4x3(
            sinCos3.template shuffle<1, 0, 2>(), sinCos3.template negate<true, false, false>(), col3, SIMD3Def::Zero());
    }

    /**
     * Constructor to build a 4x3 Matrix with fixed rotation about arbitrary axis.
     * @param axis     The axis to rotate around.
     * @param rotation The amount to rotate the matrix by (in radians).
     * @returns Newly constructed Matrix4x3 with required attributes.
     */
    XS_INLINE static Matrix4x3 RotationAxis(const Vector3DDef& axis, typename SIMD3Def::BaseDef rotation) noexcept
    {
        // The equivalent matrix for a rotation around an axis can be described by:
        //      xx(1-c)+ s xy(1-c)-zs xz(1-c)+ys
        //      yx(1-c)+zs yy(1-c)+ c yz(1-c)-xs
        //      zx(1-c)-ys zy(1-c)-xs zz(1-c)+ c
        using SIMD2Def = typename SIMD3Def::SIMD2Def;
        using BaseDef2 = typename SIMD2Def::BaseDef;
        SIMD2Def sinCos;
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            sinCos = sin(SIMD2Def(rotation, rotation + BaseDef2(valPi2<T>)));
        } else {
            BaseDef2 cos;
            const BaseDef2 sin(sincos(rotation, cos));
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

    /**
     * Get a column of a Matrix4x3.
     * @tparam Index The index of the column to retrieve (must be between 0 and 3).
     * @returns SIMD3Defcontaining the desired values.
     */
    template<uint32_t Index>
    XS_INLINE SIMD3Def getColumn() const noexcept
    {
        return this->columns.template getValue3<Index>();
    }

    /**
     * Get a row of a Matrix4x3.
     * @tparam Index The index of the row to retrieve (must be between 0 and 2).
     * @returns SIMD4Def containing the desired values.
     */
    template<uint32_t Index>
    XS_INLINE SIMD4Def getRow() const noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
        // Note: this is not well optimised as it is only intended for viewing contents
        return this->transpose().columns.template getValue4<Index>();
    }

    /**
     * Set the first column of a Matrix4x3.
     * @tparam Index The index of the column to set (must be between 0 and 3).
     * @param column The new column values.
     */
    template<uint32_t Index>
    XS_INLINE void setColumn(const SIMD3Def& column) noexcept
    {
        this->columns.template setValue3<Index>(column);
    }

    /**
     * Multiply a 4x3 Matrix by a 4x3 Matrix and then add another 4x3 Matrix.
     * @param matrix1 Matrix to multiply the first by.
     * @param matrix2 Second matrix to add.
     * @returns Result of the operation.
     */
    XS_INLINE Matrix4x3 mad(const Matrix4x3<T, Width>& matrix1, const Matrix4x3<T, Width>& matrix2) const noexcept
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
            SIMD3x4Def ret(matrix1.columns.template shuffle3<2, 2, 2>().mad(
                this->columns.template getValue3<2>(), matrix2.columns));
            ret = matrix1.columns.template shuffle3<1, 1, 1>().template mad<false>(
                this->columns.template getValue3<1>(), ret);
            ret = matrix1.columns.template shuffle3<0, 0, 0>().template mad<false>(
                this->columns.template getValue3<0>(), ret);
            ret.template setValue3<3>(ret.template getValue3<3>() + this->columns.template getValue3<3>());
            return Matrix4x3(ret);
        }
    }

    /**
     * Transform a 3-D point using specified matrix.
     * @param point The point to be transformed.
     * @returns A new transformed point.
     */
    XS_INLINE Point3DDef transform(const Point3DDef& point) const noexcept
    {
        SIMD3Def ret(this->columns.template getValue3<0>().mad(
            point.values.template getValue<0>(), this->columns.template getValue3<3>()));
        ret = this->columns.template getValue3<1>().template mad<false>(point.values.template getValue<1>(), ret);
        return Point3DDef(
            this->columns.template getValue3<2>().template mad<false>(point.values.template getValue<2>(), ret));
    }

    /**
     * Transform a 3-D vector using specified matrix.
     * @note This is equivalent to transforming by the matrices 3x3 component.
     * @param vector The vector to be transformed.
     * @returns A new transformed vector.
     */
    XS_INLINE Vector3DDef transform(const Vector3DDef& vector) const noexcept
    {
        SIMD3Def ret(this->columns.template getValue3<0>() * vector.values.template getValue<0>());
        ret = this->columns.template getValue3<1>().template mad<false>(vector.values.template getValue<1>(), ret);
        ret = this->columns.template getValue3<2>().template mad<false>(vector.values.template getValue<2>(), ret);
        return Vector3DDef(ret);
    }

    /**
     * Transform two packed 3-D points using specified matrix.
     * @tparam Packed Whether the point is using packed storage.
     * @param point The points to be transformed.
     * @returns A new transformed point.
     */
    template<bool Packed>
    XS_INLINE Point3D2Def<Packed> transform(const Point3D2Def<Packed>& point) const noexcept
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
            ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(
                this->columns.template getValue3<1>(), ret);
            ret = point.points.template shuffle3<2, 2, 2>().template mad<false>(
                this->columns.template getValue3<2>(), ret);
            return Point3D2Def<Packed>(ret);
        }
    }

    /**
     * Transform two packed 3-D vectors using specified matrix.
     * @note This is equivalent to transforming by the matrices 3x3 component.
     * @tparam Packed Whether the vector is using packed storage.
     * @param vector The vectors to be transformed.
     * @returns A new transformed vector.
     */
    template<bool Packed>
    XS_INLINE Vector3D2Def<Packed> transform(const Vector3D2Def<Packed>& vector) const noexcept
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
            ret = vector.vectors.template shuffle3<1, 1, 1>().template mad<false>(
                this->columns.template getValue3<1>(), ret);
            ret = vector.vectors.template shuffle3<2, 2, 2>().template mad<false>(
                this->columns.template getValue3<2>(), ret);
            return Vector3D2Def<Packed>(ret);
        }
    }

    /**
     * Transform four packed 3-D points using specified matrix.
     * @tparam Packed Whether the point is using packed storage.
     * @param point The points to be transformed.
     * @returns A new transformed point.
     */
    template<bool Packed>
    XS_INLINE Point3D4Def<Packed> transform(const Point3D4Def<Packed>& point) const noexcept
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
            ret = point.points.template shuffle3<1, 1, 1>().template mad<false>(
                this->columns.template getValue3<1>(), ret);
            ret = point.points.template shuffle3<2, 2, 2>().template mad<false>(
                this->columns.template getValue3<2>(), ret);
            return Point3D4Def<Packed>(ret);
        }
    }

    /**
     * Transform four packed 3-D vectors using specified matrix.
     * @note This is equivalent to transforming by the matrices 3x3 component.
     * @tparam Packed Whether the vector is using packed storage.
     * @param vector The vectors to be transformed.
     * @returns A new transformed vector.
     */
    template<bool Packed>
    XS_INLINE Vector3D4Def<Packed> transform(const Vector3D4Def<Packed>& vector) const noexcept
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
            ret = vector.vectors.template shuffle3<1, 1, 1>().template mad<false>(
                this->columns.template getValue3<1>(), ret);
            ret = vector.vectors.template shuffle3<2, 2, 2>().template mad<false>(
                this->columns.template getValue3<2>(), ret);
            return Vector3D4Def<Packed>(ret);
        }
    }

    /**
     * Transpose transform a 3-D point using specified matrix.
     * @param point The point to be transformed.
     * @returns A new transpose transformed point.
     */
    XS_INLINE Point3DDef transformTransposed(const Point3DDef& point) const noexcept
    {
        return this->transpose().transform(point);
    }

    /**
     * Transpose transform a 3-D vector using specified matrix.
     * @param vector The vector to be transformed.
     * @returns A new transpose transformed vector.
     */
    XS_INLINE Vector3DDef transformTransposed(const Vector3DDef& vector) const noexcept
    {
        return this->transpose().transform(vector);
    }

    /**
     * Determine the transpose of a matrix.
     * @returns A new transposed matrix.
     */
    XS_INLINE Matrix4x4Def transpose() const noexcept
    {
        return Matrix4x4Def(Matrix4x4Def::SIMD4x4Def(
            this->columns.transpose(), Matrix4x4Def::SIMD4x4Def::SIMD4Def(0.0f, 0.0f, 0.0f, 1.0f)));
    }

    /**
     * Determine the inverse of a matrix.
     * @returns A new inverse matrix.
     */
    XS_INLINE Matrix4x3 inverse() const noexcept
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

    /**
     * Determine the determinant of an matrix.
     * @returns The determinant in a BaseDef.
     */
    XS_INLINE BaseDef determinant() const noexcept
    {
        const typename SIMD3x4Def::SIMD3x2Def det(
            this->columns.template getValue3x2<1, 2>().template shuffle3<1, 2, 0>() *
            this->columns.template getValue3x2<2, 1>().template shuffle3<2, 0, 1>());
        SIMD3Def ret(det.sub3());
        ret *= this->columns.template getValue3<0>();
        return BaseDef(ret.hadd());
    }

    /**
     * Determine the determinant of an matrix.
     * @returns The determinant in a InBaseDef.
     */
    XS_INLINE InBaseDef determinantInBase() const noexcept
    {
        const typename SIMD3x4Def::SIMD3x2Def det(
            this->columns.template getValue3x2<1, 2>().template shuffle3<1, 2, 0>() *
            this->columns.template getValue3x2<2, 1>().template shuffle3<2, 0, 1>());
        SIMD3Def ret(det.sub3());
        ret *= this->columns.template getValue3<0>();
        return ret.haddInBase();
    }

    /**
     * Fast multiply the matrix by a rotation around x axis (i.e M*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated matrix.
     */
    XS_INLINE Matrix4x3 postRotateX(typename SIMD3Def::BaseDef rotation) const noexcept
    {
        using SIMD3x2Def = typename SIMD3x4Def::SIMD3x2Def;
        using BaseDef3 = typename SIMD3Def::BaseDef;
        BaseDef3 cos;
        const BaseDef3 sin(sincos(rotation, cos));
        SIMD3x4Def ret(this->columns);
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            ret.template setValue3x2<1, 2>(
                ret.template getValue3x2<1, 2>().template mad<false>(SIMD3x2Def::BaseDef(cos),
                    ret.template getValue3x2<2, 1>() *
                        SIMD3x2Def(SIMD3x2Def::BaseDef(sin)).template negate<false, false, false, true, true, true>()));
        } else {
            ret.template setValue3x2<1, 2>(
                ret.template getValue3x2<1, 2>().template mad<false>(SIMD3x2Def::BaseDef(cos),
                    ret.template getValue3x2<2, 1>() * SIMD3x2Def::SIMD2Def(sin).template negate<false, true>()));
        }
        return Matrix4x3(ret);
    }

    /**
     * Fast multiply the matrix by a rotation around y axis (i.e M*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated matrix.
     */
    XS_INLINE Matrix4x3 postRotateY(typename SIMD3Def::BaseDef rotation) const noexcept
    {
        using SIMD3x2Def = typename SIMD3x4Def::SIMD3x2Def;
        using BaseDef3 = typename SIMD3Def::BaseDef;
        BaseDef3 cos;
        const BaseDef3 sin(sincos(rotation, cos));
        SIMD3x4Def ret(this->columns);
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            ret.template setValue3x2<0, 2>(
                ret.template getValue3x2<0, 2>().template mad<false>(SIMD3x2Def::BaseDef(cos),
                    ret.template getValue3x2<2, 0>() *
                        SIMD3x2Def(SIMD3x2Def::BaseDef(sin)).template negate<true, true, true, false, false, false>()));
        } else {
            ret.template setValue3x2<0, 2>(
                ret.template getValue3x2<0, 2>().template mad<false>(SIMD3x2Def::BaseDef(cos),
                    ret.template getValue3x2<2, 0>() * SIMD3x2Def::SIMD2Def(sin).template negate<true, false>()));
        }
        return Matrix4x3(ret);
    }

    /**
     * Fast multiply the matrix by a rotation around z axis (i.e M*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated matrix.
     */
    XS_INLINE Matrix4x3 postRotateZ(typename SIMD3Def::BaseDef rotation) const noexcept
    {
        using SIMD3x2Def = typename SIMD3x4Def::SIMD3x2Def;
        using BaseDef3 = typename SIMD3Def::BaseDef;
        BaseDef3 cos;
        const BaseDef3 sin(sincos(rotation, cos));
        SIMD3x4Def ret(this->columns);
        if constexpr (widthImpl > SIMDWidth::Scalar) {
            ret.template setValue3x2<0, 1>(
                ret.template getValue3x2<0, 1>().template mad<false>(SIMD3x2Def::BaseDef(cos),
                    ret.template getValue3x2<1, 0>() *
                        SIMD3x2Def(SIMD3x2Def::BaseDef(sin)).template negate<false, false, false, true, true, true>()));
        } else {
            ret.template setValue3x2<0, 1>(
                ret.template getValue3x2<0, 1>().template mad<false>(SIMD3x2Def::BaseDef(cos),
                    ret.template getValue3x2<1, 0>() * SIMD3x2Def::SIMD2Def(sin).template negate<false, true>()));
        }
        return Matrix4x3(ret);
    }

    /**
     * Fast multiply a rotation around x axis by the matrix (i.e rotation*M).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated matrix.
     */
    XS_INLINE Matrix4x3 preRotateX(typename SIMD3Def::BaseDef rotation) const noexcept
    {
        using SIMD4x2Def = typename Matrix4x4Def::SIMD4x4Def::SIMD8Def;
        using BaseDef3 = typename SIMD3Def::BaseDef;
        // Use R*M = transpose(transpose(M)*transpose(R))
        BaseDef3 cos;
        const BaseDef3 sin(sincos(rotation, cos));
        typename Matrix4x4Def::SIMD4x4Def ret(this->transpose().columns);
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
        return Matrix4x3(Matrix4x4Def(ret).transpose());
    }

    /**
     * Fast multiply a rotation around y axis by the matrix (i.e rotation*M).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated matrix.
     */
    XS_INLINE Matrix4x3 preRotateY(typename SIMD3Def::BaseDef rotation) const noexcept
    {
        using SIMD4x2Def = typename Matrix4x4Def::SIMD4x4Def::SIMD8Def;
        using BaseDef3 = typename SIMD3Def::BaseDef;
        // Use R*M = transpose(transpose(M)*transpose(R))
        BaseDef3 cos;
        const BaseDef3 sin(sincos(rotation, cos));
        typename Matrix4x4Def::SIMD4x4Def ret(this->transpose().columns);
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
        return Matrix4x3(Matrix4x4Def(ret).transpose());
    }

    /**
     * Fast multiply a rotation around z axis by the matrix (i.e rotation*M).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated matrix.
     */
    XS_INLINE Matrix4x3 preRotateZ(typename SIMD3Def::BaseDef rotation) const noexcept
    {
        using SIMD4x2Def = typename Matrix4x4Def::SIMD4x4Def::SIMD8Def;
        using BaseDef3 = typename SIMD3Def::BaseDef;
        // Use R*M = transpose(transpose(M)*transpose(R))
        BaseDef3 cos;
        const BaseDef3 sin(sincos(rotation, cos));
        typename Matrix4x4Def::SIMD4x4Def ret(this->transpose().columns);
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
        return Matrix4x3(Matrix4x4Def(ret).transpose());
    }

    /**
     * Fast multiply the matrix by a scale (i.e M*scale).
     * @param scale The amount to scale along X/Y/Z-axis.
     * @returns The rotated matrix.
     */
    XS_INLINE Matrix4x3 postScale(const SIMD3Def& scale) const noexcept
    {
        if constexpr (widthImpl >= SIMDWidth::B64) {
            return Matrix4x3(this->columns * SIMD3x4Def::SIMD4Def(scale, SIMD3x4Def::SIMD4Def::BaseDef::One()));
        } else {
            return Matrix4x3(
                SIMD3x4Def(this->columns.template getValue3x3<0, 1, 2>().component3(scale), this->getColumn<3>()));
        }
    }

    /**
     * Fast multiply a scale by a matrix (i.e scale*M).
     * @param scale The 3 scale values for each axis.
     * @returns The rotated matrix.
     */
    XS_INLINE Matrix4x3 preScale(const SIMD3Def& scale) const noexcept
    {
        return Matrix4x3(this->columns * scale);
    }

    /**
     * Fast multiply the matrix by a uniform scale (i.e M*scale).
     * @param scale The amount to scale along all axis.
     * @returns The rotated matrix.
     */
    XS_INLINE Matrix4x3 postUniformScale(BaseDef scale) const noexcept
    {
        return Matrix4x3(SIMD3x4Def(this->columns.template getValue3x3<0, 1, 2>() * scale, this->getColumn<3>()));
    }

    /**
     * Fast multiply the a uniform scale by a matrix (i.e scale*M).
     * @param scale The amount to scale along all axis.
     * @returns The rotated matrix.
     */
    XS_INLINE Matrix4x3 preUniformScale(BaseDef scale) const noexcept
    {
        return Matrix4x3(this->columns * scale);
    }

    /**
     * Fast multiply the matrix by a translation (i.e M*translate).
     * @param translation The amount to translate in the X/Y/Z directions.
     * @returns The rotated matrix.
     */
    XS_INLINE Matrix4x3 postTranslate(const SIMD3Def& translation) const noexcept
    {
        const typename SIMD3x4Def::SIMD3x3Def ret(this->columns.template getValue3x3<0, 1, 2>());
        const SIMD3Def ret2(ret.component3(translation).add3());
        return Matrix4x3(SIMD3x4Def(ret, this->getColumn<3>() + ret2));
    }

    /**
     * Fast multiply a translation by a matrix (i.e translate*M).
     * @param translation The 3 translate values for each axis.
     * @returns The rotated matrix.
     */
    XS_INLINE Matrix4x3 preTranslate(const SIMD3Def& translation) const noexcept
    {
        return Matrix4x3(SIMD3x4Def(this->columns.template getValue3x3<0, 1, 2>(), this->getColumn<3>() + translation));
    }
};

/**
 * Add two 4x3 Matrices.
 * @param matrix1 The first matrix.
 * @param matrix2 Matrix to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> operator+(const Matrix4x3<T, Width>& matrix1, const Matrix4x3<T, Width>& matrix2) noexcept
{
    return Matrix4x3<T, Width>(matrix1.columns + matrix2.columns);
}

/**
 * Subtract a 4x3 Matrix from another 4x3 Matrix.
 * @param matrix1 The first matrix.
 * @param matrix2 Matrix to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width> operator-(const Matrix4x3<T, Width>& matrix1, const Matrix4x3<T, Width>& matrix2) noexcept
{
    return Matrix4x3<T, Width>(matrix1.columns - matrix2.columns);
}

/**
 * Multiply two 4x3 Matrices together.
 * @param matrix1 The first matrix.
 * @param matrix2 Matrix to multiply the first by.
 * @returns The result of the operation.
 */
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

/**
 * Perform compound assignment and addition with a 4x3 Matrix.
 * @param [in,out] matrix1 The first matrix.
 * @param          matrix2 Matrix to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width>& operator+=(Matrix4x3<T, Width>& matrix1, const Matrix4x3<T, Width>& matrix2) noexcept
{
    matrix1.columns += matrix2.columns;
    return matrix1;
}

/**
 * Perform compound assignment and subtraction with a 4x3 Matrix.
 * @param [in,out] matrix1 The first matrix.
 * @param          matrix2 Matrix to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width>& operator-=(Matrix4x3<T, Width>& matrix1, const Matrix4x3<T, Width>& matrix2) noexcept
{
    matrix1.columns -= matrix2.columns;
    return matrix1;
}

/**
 * Perform compound assignment and multiplication with a 4x3 Matrix.
 * @param [in,out] matrix1 The first matrix.
 * @param          matrix2 Matrix to multiply the first by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Matrix4x3<T, Width>& operator*=(Matrix4x3<T, Width>& matrix1, const Matrix4x3<T, Width>& matrix2) noexcept
{
    matrix1 = matrix1 * matrix2;
    return matrix1;
}
} // namespace Shift
