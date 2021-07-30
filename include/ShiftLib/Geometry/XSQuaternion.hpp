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

#include "SIMD/XSSIMD4.hpp"

// Additional includes
namespace Shift {
template<typename T, SIMDWidth Width>
class Matrix3x3;
} // namespace Shift
#include "Geometry/XSMatrix3x3.hpp"
#include "Geometry/XSPoint3D.hpp"
#include "Geometry/XSPoint3D2.hpp"
#include "Geometry/XSPoint3D4.hpp"
#include "Geometry/XSVector3D.hpp"
#include "Geometry/XSVector3D2.hpp"
#include "Geometry/XSVector3D4.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class Quaternion;

template<typename T>
class QuaternionData
{
public:
    SIMD4Data<T> values;

    /** Default constructor. */
    XS_FUNCTION QuaternionData() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit QuaternionData(const Quaternion<T, Width>& other) noexcept;

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param x The first element of the vector component of the quaternion.
     * @param y The second element of the vector component of the quaternion.
     * @param z The third element of the vector component of the quaternion.
     * @param w The scalar component of the quaternion.
     */
    XS_FUNCTION void setData(T x, T y, T z, T w) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const Quaternion<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Quaternion<T, Width> load() const noexcept;
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
template<typename T, SIMDWidth Width = widthSIMD<T>>
class Quaternion
{
public:
    using Type = T;
    using SIMD4Def = SIMD4<T, SIMD4<T, Width>::widthImpl>;
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
    XS_FUNCTION Quaternion() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION Quaternion(const Quaternion& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Quaternion(Quaternion&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Quaternion& operator=(const Quaternion& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Quaternion& operator=(Quaternion&& other) noexcept = default;

    /**
     * Construct a Quaternion from a SIMD4.
     * @param values Input SIMD4.
     */
    XS_FUNCTION explicit Quaternion(const SIMD4Def& values) noexcept;

    /**
     * Construct a quaternion from 4 values.
     * @param x The first element of the vector component of the quaternion.
     * @param y The second element of the vector component of the quaternion.
     * @param z The third element of the vector component of the quaternion.
     * @param w The scalar component of the quaternion.
     */
    XS_FUNCTION Quaternion(T x, T y, T z, T w) noexcept;

    /**
     * Construct a Quaternion from a Matrix3x3.
     * @note This requires that the input matrix is special orthogonal. (i.e. sum of diagonal elements>1 ->
     * M00+M11+M22>1). If not special orthogonal then need to use static function instead.
     * @param matrix Reference to matrix to copy.
     */
    XS_FUNCTION explicit Quaternion(const Matrix3x3Def& matrix) noexcept;

    /**
     * Constructor to build a Identity Quaternion.
     * @returns Newly constructed Quaternion with required attributes.
     */
    XS_FUNCTION static Quaternion Identity() noexcept;

    /**
     * Constructor to build a Quaternion with fixed rotation about X axis.
     * @param rotation The amount to rotate the quaternion by (in radians).
     * @returns Newly constructed Quaternion with required attributes.
     */
    XS_FUNCTION static Quaternion RotationX(BaseDef rotation) noexcept;

    /**
     * Constructor to build a Quaternion with fixed rotation about Y axis.
     * @param rotation The amount to rotate the quaternion by (in radians).
     * @returns Newly constructed Quaternion with required attributes.
     */
    XS_FUNCTION static Quaternion RotationY(BaseDef rotation) noexcept;

    /**
     * Constructor to build a Quaternion with fixed rotation about Z axis.
     * @param rotation The amount to rotate the quaternion by (in radians).
     * @returns Newly constructed Quaternion with required attributes.
     */
    XS_FUNCTION static Quaternion RotationZ(BaseDef rotation) noexcept;

    /**
     * Constructor to build a Quaternion with fixed rotation about arbitrary axis.
     * @param axis     The axis to rotate around.
     * @param rotation The amount to rotate the quaternion by (in radians).
     * @returns Newly constructed Quaternion with required attributes.
     */
    XS_FUNCTION static Quaternion RotationAxis(const Vector3DDef& axis, BaseDef rotation) noexcept;

    /**
     * Constructor to build a Quaternion from a Matrix3x3.
     * @note This is not as optimised as the constructor version but it does allow
     *  support for non special orthogonal matrices.
     * @param matrix Reference to matrix to copy.
     * @returns An Quaternion.
     */
    XS_FUNCTION static Quaternion Matrix3x3(const Matrix3x3Def& matrix) noexcept;

    /**
     * Get an element of a Quaternion.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (must be between 0 and 3).
     * @returns InBaseDef containing the desired value.
     */
    template<uint32_t Index>
    XS_FUNCTION InBaseDef getValueInBase() const noexcept;

    /**
     * Get an element of a Quaternion.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (must be between 0 and 1).
     * @returns SIMDBase containing the desired value.
     */
    template<uint32_t Index>
    XS_FUNCTION BaseDef getValue() const noexcept;

    /**
     * Set an element of a Quaternion.
     * @tparam Index The index of the element to set (must be between 0 and 3).
     * @param value The new value.
     */
    template<uint32_t Index>
    XS_FUNCTION void setValue(InBaseDef value) noexcept;

    /**
     * Set an element of a Quaternion.
     * @tparam Index The index of the element to set (must be between 0 and 3).
     * @param value The new value.
     */
    template<uint32_t Index>
    XS_FUNCTION void setValue(BaseDef value) noexcept;

    /**
     * Multiply a Quaternion by a Quaternion and then add another Quaternion.
     * @param other1 Quaternion to multiply the first by.
     * @param other2 Second quaternion to add.
     * @returns Result of operation.
     */
    XS_FUNCTION Quaternion mad(const Quaternion& other1, const Quaternion& other2) const noexcept;

    /**
     * Compute the square of the length of a quaternion.
     * @returns Squared length of vector stored in BaseDef.
     */
    XS_FUNCTION BaseDef lengthSqr() const noexcept;

    /**
     * Compute the length of a quaternion.
     * @returns Length of quaternion stored in BaseDef.
     */
    XS_FUNCTION BaseDef length() const noexcept;

    /**
     * Compute the square of the length of a quaternion.
     * @returns Squared length of quaternion stored in InBaseDef.
     */
    XS_FUNCTION InBaseDef lengthSqrInBase() const noexcept;

    /**
     * Compute the length of a quaternion.
     * @returns Length of quaternion stored in InBaseDef.
     */
    XS_FUNCTION InBaseDef lengthInBase() const noexcept;

    /**
     * Normalised the quaternion.
     * @note The result is unpredictable when all elements of quaternion are at or near zero.
     * @returns A new quaternion corresponding to the normalised quaternion.
     */
    XS_FUNCTION Quaternion normalize() noexcept;

    /**
     * Transform a 3-D point using specified quaternion.
     * @param point The point to be transformed.
     * @returns A new transformed point.
     */
    XS_FUNCTION Point3DDef transform(const Point3DDef& point) const noexcept;

    /**
     * Transform a 3-D vector using specified quaternion.
     * @param vector The vector to be transformed.
     * @returns A new transformed vector.
     */
    XS_FUNCTION Vector3DDef transform(const Vector3DDef& vector) const noexcept;

    /**
     * Transform two packed 3-D points using specified quaternion.
     * @tparam Packed Whether the point is using packed storage.
     * @param point The points to be transformed.
     * @returns A new transformed point.
     */
    template<bool Packed>
    XS_FUNCTION Point3D2Def<Packed> transform(const Point3D2Def<Packed>& point) const noexcept;

    /**
     * Transform two packed 3-D vectors using specified quaternion.
     * @tparam Packed Whether the vector is using packed storage.
     * @param vector The vectors to be transformed.
     * @returns A new transformed vector.
     */
    template<bool Packed>
    XS_FUNCTION Vector3D2Def<Packed> transform(const Vector3D2Def<Packed>& vector) const noexcept;

    /**
     * Transform four packed 3-D points using specified quaternion.
     * @tparam Packed Whether the point is using packed storage.
     * @param point The points to be transformed.
     * @returns A new transformed point.
     */
    template<bool Packed>
    XS_FUNCTION Point3D4Def<Packed> transform(const Point3D4Def<Packed>& point) const noexcept;

    /**
     * Transform four packed 3-D vectors using specified quaternion.
     * @tparam Packed Whether the vector is using packed storage.
     * @param vector The vectors to be transformed.
     * @returns A new transformed vector.
     */
    template<bool Packed>
    XS_FUNCTION Vector3D4Def<Packed> transform(const Vector3D4Def<Packed>& vector) const noexcept;

    /**
     * Determine the inverse of a quaternion.
     * @returns A new inverse quaternion.
     */
    XS_FUNCTION Quaternion inverse() const noexcept;

    /**
     * Fast multiply the quaternion by a rotation around x axis (i.e Q*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated quaternion.
     */
    XS_FUNCTION Quaternion postRotateX(BaseDef rotation) const noexcept;

    /**
     * Fast multiply the quaternion by a rotation around y axis (i.e Q*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated quaternion.
     */
    XS_FUNCTION Quaternion postRotateY(BaseDef rotation) const noexcept;

    /**
     * Fast multiply the quaternion by a rotation around z axis (i.e M*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated quaternion.
     */
    XS_FUNCTION Quaternion postRotateZ(BaseDef rotation) const noexcept;

    /**
     * Fast multiply a rotation around x axis by the quaternion (i.e rotation*Q).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated quaternion.
     */
    XS_FUNCTION Quaternion preRotateX(BaseDef rotation) const noexcept;

    /**
     * Fast multiply a rotation around y axis by the quaternion (i.e rotation*Q).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated quaternion.
     */
    XS_FUNCTION Quaternion preRotateY(BaseDef rotation) const noexcept;

    /**
     * Fast multiply a rotation around z axis by the quaternion (i.e rotation*Q).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated quaternion.
     */
    XS_FUNCTION Quaternion preRotateZ(BaseDef rotation) const noexcept;
};

/**
 * Add two Quaternions.
 * @param other1 The first quaternion.
 * @param other2 Quaternion to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Quaternion<T, Width> operator+(
    const Quaternion<T, Width>& other1, const Quaternion<T, Width>& other2) noexcept;

/**
 * Subtract a Quaternion from another Quaternion.
 * @param other1 The first quaternion.
 * @param other2 Quaternion to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Quaternion<T, Width> operator-(
    const Quaternion<T, Width>& other1, const Quaternion<T, Width>& other2) noexcept;

/**
 * Multiply two Quaternions together.
 * @param other1 The first quaternion.
 * @param other2 Quaternion to multiply the first by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Quaternion<T, Width> operator*(
    const Quaternion<T, Width>& other1, const Quaternion<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and addition with a Quaternion.
 * @param [in,out] other1 The first quaternion.
 * @param          other2 Quaternion to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Quaternion<T, Width>& operator+=(Quaternion<T, Width>& other1, const Quaternion<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and subtraction with a Quaternion.
 * @param [in,out] other1 The first quaternion.
 * @param          other2 Quaternion to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Quaternion<T, Width>& operator-=(Quaternion<T, Width>& other1, const Quaternion<T, Width>& other2) noexcept;

/**
 * Perform compound assignment and multiplication with a Quaternion.
 * @param [in,out] other1 The first quaternion.
 * @param          other2 Quaternion to multiply the first by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Quaternion<T, Width>& operator*=(Quaternion<T, Width>& other1, const Quaternion<T, Width>& other2) noexcept;
} // namespace Shift
