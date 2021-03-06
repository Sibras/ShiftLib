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

#include "SIMD/XSSIMD3x3.hpp"

// Additional includes
namespace Shift {
template<typename T, SIMDWidth Width>
class Matrix4x3;
template<typename T, SIMDWidth Width>
class Matrix4x4;
template<typename T, SIMDWidth Width>
class Quaternion;
} // namespace Shift
#include "Geometry/XSMatrix4x3.hpp"
#include "Geometry/XSMatrix4x4.hpp"
#include "Geometry/XSPoint3D.hpp"
#include "Geometry/XSPoint3D2.hpp"
#include "Geometry/XSPoint3D4.hpp"
#include "Geometry/XSQuaternion.hpp"
#include "Geometry/XSVector3D.hpp"
#include "Geometry/XSVector3D2.hpp"
#include "Geometry/XSVector3D4.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class Matrix3x3;

template<typename T>
class Matrix3x3Data
{
public:
    SIMD3x3Data<T> values;

    /** Default constructor. */
    XS_FUNCTION Matrix3x3Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit Matrix3x3Data(const Matrix3x3<T, Width>& other) noexcept;

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param fC0R0 The first element of the first column of the matrix.
     * @param fC0R1 The second element of the first column of the matrix.
     * @param fC0R2 The third element of the first column of the matrix.
     * @param fC1R0 The first element of the second column of the matrix.
     * @param fC1R1 The second element of the second column of the matrix.
     * @param fC1R2 The third element of the second column of the matrix.
     * @param fC2R0 The first element of the third column of the matrix.
     * @param fC2R1 The second element of the third column of the matrix.
     * @param fC2R2 The third element of the third column of the matrix.
     */
    XS_FUNCTION void setData(T fC0R0, T fC0R1, T fC0R2, T fC1R0, T fC1R1, T fC1R2, T fC2R0, T fC2R1, T fC2R2) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const Matrix3x3<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Matrix3x3<T, Width> load() const noexcept;
};

template<typename T>
class Matrix3x3DataPad
{
public:
    SIMD3x3DataPad<T> values;

    /** Default constructor. */
    XS_FUNCTION Matrix3x3DataPad() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit Matrix3x3DataPad(const Matrix3x3<T, Width>& other) noexcept;

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param fC0R0 The first element of the first column of the matrix.
     * @param fC0R1 The second element of the first column of the matrix.
     * @param fC0R2 The third element of the first column of the matrix.
     * @param fC1R0 The first element of the second column of the matrix.
     * @param fC1R1 The second element of the second column of the matrix.
     * @param fC1R2 The third element of the second column of the matrix.
     * @param fC2R0 The first element of the third column of the matrix.
     * @param fC2R1 The second element of the third column of the matrix.
     * @param fC2R2 The third element of the third column of the matrix.
     */
    XS_FUNCTION void setData(T fC0R0, T fC0R1, T fC0R2, T fC1R0, T fC1R1, T fC1R2, T fC2R0, T fC2R1, T fC2R2) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const Matrix3x3<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Matrix3x3<T, Width> load() const noexcept;
};

/**
 * A  base type used to represent a 3x3 matrix. This is a specialised version of Matrix4x4 designed specially for
 * affine matrices without a translational component.
 * @note This uses standard right handed coordinate notation
 *    i.e. Vector (V) transforms with Matrix (M) occur in the following order:
 *         V2 = M * V
 *    Matrix multiplications between 2 matrices (M1, M2) gives the following ordering:
 *         M3 = M1 * M2        V2 = M3 * V    equivalent to->  V2 = M1 * (M2 * V)
 * @tparam T     Generic type parameter.
 * @tparam Width Type of SIMD being used.
 */
template<typename T, SIMDWidth Width = widthSIMD<T>>
class Matrix3x3
{
public:
    using Type = T;
    using SIMD3x3Def = SIMD3x3<T, SIMD3x3<T, Width>::widthImpl>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = SIMD3x3Def::widthImpl;
    using BaseDef = typename SIMD3x3Def::BaseDef;
    using InBaseDef = typename SIMD3x3Def::InBaseDef;
    using SIMD3Def = typename SIMD3x3Def::SIMD3Def;
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
    using Matrix4x3Def = Matrix4x3<T, Matrix4x3<T, widthImpl>::widthImpl>;
    using QuaternionDef = Quaternion<T, Quaternion<T, widthImpl>::widthImpl>;

    SIMD3x3Def columns;

    /** Default constructor. */
    XS_FUNCTION Matrix3x3() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION Matrix3x3(const Matrix3x3& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Matrix3x3(Matrix3x3&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Matrix3x3& operator=(const Matrix3x3& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Matrix3x3& operator=(Matrix3x3&& other) noexcept = default;

    /**
     * Construct a 3x3 matrix from a SIMD3x3.
     * @param values The 3 columns of the matrix.
     */
    XS_FUNCTION explicit Matrix3x3(const SIMD3x3Def& values) noexcept;

    /**
     * Construct a 3x3 matrix from 3 SIMD3s.
     * @param column0 The first column of the matrix.
     * @param column1 The second column of the matrix.
     * @param column2 The third column of the matrix.
     */
    XS_FUNCTION Matrix3x3(const SIMD3Def& column0, const SIMD3Def& column1, const SIMD3Def& column2) noexcept;

    /**
     * Construct a 3x3 matrix from 9 values.
     * @param col0Row0 The first element of the first column of the matrix.
     * @param col0Row1 The second element of the first column of the matrix.
     * @param col0Row2 The third element of the first column of the matrix.
     * @param col1Row0 The first element of the second column of the matrix.
     * @param col1Row1 The second element of the second column of the matrix.
     * @param col1Row2 The third element of the second column of the matrix.
     * @param col2Row0 The first element of the third column of the matrix.
     * @param col2Row1 The second element of the third column of the matrix.
     * @param col2Row2 The third element of the third column of the matrix.
     */
    XS_FUNCTION Matrix3x3(T col0Row0, T col0Row1, T col0Row2, T col1Row0, T col1Row1, T col1Row2, T col2Row0,
        T col2Row1, T col2Row2) noexcept;

    /**
     * Construct from a Matrix4x4.
     * @param matrix Reference to matrix to copy.
     */
    XS_FUNCTION explicit Matrix3x3(const Matrix4x4Def& matrix) noexcept;

    /**
     * Construct from a Matrix4x3.
     * @param matrix Reference to matrix to copy.
     */
    XS_FUNCTION explicit Matrix3x3(const Matrix4x3Def& matrix) noexcept;

    /**
     * Construct from a Quaternion.
     * @param quaternion Reference to quaternion to copy.
     */
    XS_FUNCTION explicit Matrix3x3(const QuaternionDef& quaternion) noexcept;

    /**
     * Constructor to build a 3x3 Identity Matrix.
     * @returns Newly constructed Matrix3x3 with required attributes.
     */
    XS_FUNCTION static Matrix3x3 Identity() noexcept;

    /**
     * Constructor to build a 3x3 Matrix with fixed uniform scale.
     * @param scale The amount to scale in the X/Y/Z direction.
     * @returns Newly constructed Matrix4x3 with required attributes.
     */
    XS_FUNCTION static Matrix3x3 UniformScale(const InBaseDef& scale) noexcept;

    /**
     * Constructor to build a 3x3 Matrix with fixed scale.
     * @param scale The amount to scale in the X/Y/Z direction.
     * @returns Newly constructed Matrix3x3 with required attributes.
     */
    XS_FUNCTION static Matrix3x3 Scale(const SIMD3Def& scale) noexcept;

    /**
     * Constructor to build a 3x3 Matrix with fixed rotation about X axis.
     * @param rotation The amount to rotate the matrix by (in radians).
     * @returns Newly constructed Matrix3x3 with required attributes.
     */
    XS_FUNCTION static Matrix3x3 RotationX(const typename SIMD3Def::BaseDef& rotation) noexcept;

    /**
     * Constructor to build a 3x3 Matrix with fixed rotation about Y axis.
     * @param rotation The amount to rotate the matrix by (in radians).
     * @returns Newly constructed Matrix3x3 with required attributes.
     */
    XS_FUNCTION static Matrix3x3 RotationY(const typename SIMD3Def::BaseDef& rotation) noexcept;

    /**
     * Constructor to build a 3x3 Matrix with fixed rotation about Z axis.
     * @param rotation The amount to rotate the matrix by (in radians).
     * @returns Newly constructed Matrix3x3 with required attributes.
     */
    XS_FUNCTION static Matrix3x3 RotationZ(const typename SIMD3Def::BaseDef& rotation) noexcept;

    /**
     * Constructor to build a 3x3 Matrix with fixed rotation about arbitrary axis.
     * @param axis     The axis to rotate around.
     * @param rotation The amount to rotate the matrix by (in radians).
     * @returns Newly constructed Matrix3x3 with required attributes.
     */
    XS_FUNCTION static Matrix3x3 RotationAxis(
        const Vector3DDef& axis, const typename SIMD3Def::BaseDef& rotation) noexcept;

    /**
     * Get a column of a Matrix3x3.
     * @tparam Index The index of the column to retrieve (must be between 0 and 2).
     * @returns SIMD3Def containing the desired values.
     */
    template<uint32_t Index>
    XS_FUNCTION SIMD3Def getColumn() const;

    /**
     * Get a row of a Matrix3x3.
     * @tparam Index The index of the row to retrieve (must be between 0 and 2).
     * @returns SIMD3Def containing the desired values.
     */
    template<uint32_t Index>
    XS_FUNCTION SIMD3Def getRow() const;

    /**
     * Set the first column of a Matrix3x3.
     * @tparam Index Index The index of the column to set (must be between 0 and 2).
     * @param column The new column values.
     */
    template<uint32_t Index>
    XS_FUNCTION void setColumn(const SIMD3Def& column) noexcept;

    /**
     * Multiply a 3x3 Matrix by a 3x3 Matrix and then add another 3x3 Matrix.
     * @param matrix1 Matrix to multiply the first by.
     * @param matrix2 Second matrix to add.
     * @returns Result of operation.
     */
    XS_FUNCTION Matrix3x3 mad(const Matrix3x3<T, Width>& matrix1, const Matrix3x3<T, Width>& matrix2) const;

    /**
     * Transform a 3-D point using specified matrix.
     * @param point The point to be transformed.
     * @returns A new transformed point.
     */
    XS_FUNCTION Point3DDef transform(const Point3DDef& point) const;

    /**
     * Transform a 3-D vector using specified matrix.
     * @param vector The vector to be transformed.
     * @returns A new transformed vector.
     */
    XS_FUNCTION Vector3DDef transform(const Vector3DDef& vector) const;

    /**
     * Transform two packed 3-D points using specified matrix.
     * @tparam Packed Whether the point is using packed storage.
     * @param point The points to be transformed.
     * @returns A new transformed point.
     */
    template<bool Packed>
    XS_FUNCTION Point3D2Def<Packed> transform(const Point3D2Def<Packed>& point) const;

    /**
     * Transform two packed 3-D vectors using specified matrix.
     * @tparam Packed Whether the vector is using packed storage.
     * @param vector The vectors to be transformed.
     * @returns A new transformed vector.
     */
    template<bool Packed>
    XS_FUNCTION Vector3D2Def<Packed> transform(const Vector3D2Def<Packed>& vector) const;

    /**
     * Transform four packed 3-D points using specified matrix.
     * @tparam Packed Whether the point is using packed storage.
     * @param point The points to be transformed.
     * @returns A new transformed point.
     */
    template<bool Packed>
    XS_FUNCTION Point3D4Def<Packed> transform(const Point3D4Def<Packed>& point) const;

    /**
     * Transform four packed 3-D vectors using specified matrix.
     * @tparam Packed Whether the vector is using packed storage.
     * @param vector The vectors to be transformed.
     * @returns A new transformed vector.
     */
    template<bool Packed>
    XS_FUNCTION Vector3D4Def<Packed> transform(const Vector3D4Def<Packed>& vector) const;

    /**
     * Transpose transform a 3-D point using specified matrix.
     * @param point The point to be transformed.
     * @returns A new transpose transformed point.
     */
    XS_FUNCTION Point3DDef transformTransposed(const Point3DDef& point) const;

    /**
     * Transpose transform a 3-D vector using specified matrix.
     * @param vector The vector to be transformed.
     * @returns A new transpose transformed vector.
     */
    XS_FUNCTION Vector3DDef transformTransposed(const Vector3DDef& vector) const;

    /**
     * Determine the transpose of a matrix.
     * @returns A new transposed matrix.
     */
    XS_FUNCTION Matrix3x3 transpose() const;

    /**
     * Determine the inverse of a matrix.
     * @returns A new inverse matrix.
     */
    XS_FUNCTION Matrix3x3 inverse() const;

    /**
     * Determine the determinant of an matrix.
     * @returns The determinant in a BaseDef.
     */
    XS_FUNCTION BaseDef determinant() const;

    /**
     * Determine the determinant of an matrix.
     * @returns The determinant in a InBaseDef.
     */
    XS_FUNCTION InBaseDef determinantInBase() const;

    /**
     * Fast multiply the matrix by a rotation around x axis (i.e M*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix3x3 postRotateX(const typename SIMD3Def::BaseDef& rotation) const;

    /**
     * Fast multiply the matrix by a rotation around y axis (i.e M*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix3x3 postRotateY(const typename SIMD3Def::BaseDef& rotation) const;

    /**
     * Fast multiply the matrix by a rotation around z axis (i.e M*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix3x3 postRotateZ(const typename SIMD3Def::BaseDef& rotation) const;

    /**
     * Fast multiply a rotation around x axis by the matrix (i.e rotation*M).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix3x3 preRotateX(const typename SIMD3Def::BaseDef& rotation) const;

    /**
     * Fast multiply a rotation around y axis by the matrix (i.e rotation*M).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix3x3 preRotateY(const typename SIMD3Def::BaseDef& rotation) const;

    /**
     * Fast multiply a rotation around z axis by the matrix (i.e rotation*M).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix3x3 preRotateZ(const typename SIMD3Def::BaseDef& rotation) const;

    /**
     * Fast multiply the matrix by a scale (i.e M*scale).
     * @param scale The amount to scale along X/Y/Z-axis.
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix3x3 postScale(const SIMD3Def& scale) const;

    /**
     * Fast multiply a scale by a matrix (i.e scale*M).
     * @param scale The 3 scale values for each axis.
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix3x3 preScale(const SIMD3Def& scale) const;

    /**
     * Fast multiply the matrix by a uniform scale.
     * @param scale The amount to scale along all axis.
     * @returns The rotated matrix.
     */
    XS_FUNCTION Matrix3x3 uniformScale(const BaseDef& scale) const;
};

/**
 * Add two 3x3 Matrices.
 * @param matrix1 The first matrix.
 * @param matrix2 Matrix to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Matrix3x3<T, Width> operator+(
    const Matrix3x3<T, Width>& matrix1, const Matrix3x3<T, Width>& matrix2) noexcept;

/**
 * Subtract a 3x3 Matrix from another 3x3 Matrix.
 * @param matrix1 The first matrix.
 * @param matrix2 Matrix to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Matrix3x3<T, Width> operator-(
    const Matrix3x3<T, Width>& matrix1, const Matrix3x3<T, Width>& matrix2) noexcept;

/**
 * Multiply two 3x3 Matrices together.
 * @param matrix1 The first matrix.
 * @param matrix2 Matrix to multiply the first by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Matrix3x3<T, Width> operator*(
    const Matrix3x3<T, Width>& matrix1, const Matrix3x3<T, Width>& matrix2) noexcept;

/**
 * Perform compound assignment and addition with a 3x3 Matrix.
 * @param [in,out] matrix1 The first matrix.
 * @param          matrix2 Matrix to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Matrix3x3<T, Width>& operator+=(Matrix3x3<T, Width>& matrix1, const Matrix3x3<T, Width>& matrix2) noexcept;

/**
 * Perform compound assignment and subtraction with a 3x3 Matrix.
 * @param [in,out] matrix1 The first matrix.
 * @param          matrix2 Matrix to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Matrix3x3<T, Width>& operator-=(Matrix3x3<T, Width>& matrix1, const Matrix3x3<T, Width>& matrix2) noexcept;

/**
 * Perform compound assignment and multiplication with a 3x3 Matrix.
 * @param [in,out] matrix1 The first matrix.
 * @param          matrix2 Matrix to multiply the first by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Matrix3x3<T, Width>& operator*=(Matrix3x3<T, Width>& matrix1, const Matrix3x3<T, Width>& matrix2) noexcept;
} // namespace Shift
