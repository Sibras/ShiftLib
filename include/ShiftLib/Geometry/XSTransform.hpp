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
#include "Geometry/XSQuaternion.hpp"
#include "Geometry/XSVector3D.hpp"
#include "SIMD/XSSIMDBase.hpp"

// Additional includes
#include "Geometry/XSPoint3D.hpp"
#include "Geometry/XSPoint3D2.hpp"
#include "Geometry/XSPoint3D4.hpp"
#include "Geometry/XSVector3D2.hpp"
#include "Geometry/XSVector3D4.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class Point3D;
template<typename T, SIMDWidth Width, bool Packed>
class Point3D2;
template<typename T, SIMDWidth Width, bool Packed>
class Point3D4;
template<typename T, SIMDWidth Width, bool Packed>
class Vector3D2;
template<typename T, SIMDWidth Width, bool Packed>
class Vector3D4;

template<typename T, SIMDWidth Width, bool Matrix>
class Transform;

template<typename T, bool Matrix>
class TransformData
{
    template<bool IsMatrix>
    struct InternData
    {
        QuaternionData<T> quaternion;
        SIMD4Data<T> translationScale;
    };

    template<>
    struct InternData<true>
    {
        Matrix4x3Data<T> matrix;
    };

public:
    InternData<Matrix> data;

    /** Default constructor. */
    XS_FUNCTION TransformData() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit TransformData(const Transform<T, Width, Matrix>& other) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const Transform<T, Width, Matrix>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Transform<T, Width, Matrix> load() const noexcept;
};

template<typename T, bool Matrix>
class TransformDataPad
{
    template<bool IsMatrix>
    struct InternData
    {
        QuaternionDataPad<T> quaternion;
        Vector3DDataPad<T> translation;
        SIMDBaseDataPad<T> scale;
    };

    template<>
    struct InternData<true>
    {
        Matrix4x3DataPad<T> matrix;
    };

public:
    InternData<Matrix> data;

    /** Default constructor. */
    XS_FUNCTION TransformDataPad() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit TransformDataPad(const Transform<T, Width, Matrix>& other) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const Transform<T, Width, Matrix>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Transform<T, Width, Matrix> load() const noexcept;
};

/**
 * A transform object. The transform type is used as a generic construct used to represent any 3D transformation by
 * abstracting the lower implementation details.
 * @note This uses standard right handed coordinate notation
 *    i.e. Vector (V) transforms with Transform (T) occur in the following order:
 *         V2 = T * V
 *    Transform multiplications between 2 transforms (T1, T2) gives the following ordering:
 *         T3 = T1 * T2        V2 = T3 * V    equivalent to->  V2 = T1 * (T2 * V)
 * @tparam T     Generic type parameter.
 * @tparam Width Type of SIMD being used.
 */
template<typename T, SIMDWidth Width = widthSIMD<T>, bool Matrix = false>
class Transform
{
    template<bool IsMatrix>
    struct InternData
    {
        using QuaternionDef = Quaternion<T, Quaternion<T, Width>::widthImpl>;
        static constexpr SIMDWidth widthImpl = QuaternionDef::widthImpl;
        using BaseDef = typename QuaternionDef::BaseDef;
        using InBaseDef = typename QuaternionDef::InBaseDef;
        using Vector3DDef = typename QuaternionDef::Vector3DDef;
        using SIMD3Def = typename Vector3DDef::SIMD3Def;

        QuaternionDef quaternion; /**< The transforms rotation representation. */
        Vector3DDef translation;  /**< The transforms translation representation. */
        BaseDef scale;            /**< The transforms uniform scale component. */
    };

    template<>
    struct InternData<true>
    {
        using Matrix4x3Def = Matrix4x3<T, Quaternion<T, Width>::widthImpl>;
        static constexpr SIMDWidth widthImpl = Matrix4x3Def::widthImpl;
        using BaseDef = typename Matrix4x3Def::BaseDef;
        using InBaseDef = typename Matrix4x3Def::InBaseDef;
        using SIMD3Def = typename Matrix4x3Def::SIMD3Def;

        Matrix4x3Def matrix; /**< The transforms internal representation. */
    };

public:
    using Type = T;
    static constexpr SIMDWidth width = Width;
    using Matrix4x3Def = Matrix4x3<T, Width>;
    using QuaternionDef = Quaternion<T, Width>;
    using Data = InternData<Matrix>;
    static constexpr SIMDWidth widthImpl = Data::widthImpl;
    using BaseDef = typename Data::BaseDef;
    using InBaseDef = typename Data::InBaseDef;
    using SIMD3Def = typename Data::SIMD3Def;
    using Point3DDef = Point3D<T, widthImpl>;
    template<bool Packed>
    using Point3D2Def = Point3D2<T, widthImpl, Packed>;
    template<bool Packed>
    using Point3D4Def = Point3D4<T, widthImpl, Packed>;
    using Vector3DDef = Vector3D<T, widthImpl>;
    template<bool Packed>
    using Vector3D2Def = Vector3D2<T, widthImpl, Packed>;
    template<bool Packed>
    using Vector3D4Def = Vector3D4<T, widthImpl, Packed>;

    Data data;

    /** Default constructor. */
    XS_FUNCTION Transform() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION Transform(const Transform& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Transform(Transform&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Transform& operator=(const Transform& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Transform& operator=(Transform&& other) noexcept = default;

    /**
     * Construct a transform from a Matrix4x3.
     * @param matrix The new transform transform.
     */
    XS_FUNCTION explicit Transform(const Matrix4x3Def& matrix) noexcept;

    /**
     * Construct a transform from a Quaternion, translation and scale.
     * @param quaternion  The new rotation component of transform.
     * @param translation The new translation component.
     * @param scale       The new scale component.
     */
    XS_FUNCTION Transform(const QuaternionDef& quaternion, const Vector3DDef& translation, BaseDef scale) noexcept;

    /**
     * Constructor to build a Identity Matrix.
     * @returns Newly constructed Transform with required attributes.
     */
    XS_FUNCTION static Transform Identity() noexcept;

    /**
     * Constructor to build a Transform with fixed translation.
     * @param translation The amount to translate in the X/Y/Z directions.
     * @returns Newly constructed Transform with required attributes.
     */
    XS_FUNCTION static Transform Translation(const SIMD3Def& translation) noexcept;

    /**
     * Constructor to build a Transform with fixed scale.
     * @param scale The amount to scale in the X/Y/Z direction.
     * @returns Newly constructed Transform with required attributes.
     */
    XS_FUNCTION static Transform UniformScale(InBaseDef scale) noexcept;

    /**
     * Constructor to build a Transform with fixed rotation about X axis.
     * @param rotation The amount to rotate the transform by (in radians).
     * @returns Newly constructed Transform with required attributes.
     */
    XS_FUNCTION static Transform RotationX(InBaseDef rotation) noexcept;

    /**
     * Constructor to build a Transform with fixed rotation about Y axis.
     * @param rotation The amount to rotate the transform by (in radians).
     * @returns Newly constructed Transform with required attributes.
     */
    XS_FUNCTION static Transform RotationY(InBaseDef rotation) noexcept;

    /**
     * Constructor to build a Transform with fixed rotation about Z axis.
     * @param rotation The amount to rotate the transform by (in radians).
     * @returns Newly constructed Transform with required attributes.
     */
    XS_FUNCTION static Transform RotationZ(InBaseDef rotation) noexcept;

    /**
     * Constructor to build a Transform with fixed rotation about arbitrary axis.
     * @param axis     The axis to rotate around.
     * @param rotation The amount to rotate the transform by (in radians).
     * @returns Newly constructed Transform with required attributes.
     */
    XS_FUNCTION static Transform RotationAxis(const Vector3DDef& axis, InBaseDef rotation) noexcept;

    /**
     * Transform a 3-D point using specified transform.
     * @param point The point to be transformed.
     * @returns A new transformed point.
     */
    XS_FUNCTION Point3DDef transform(const Point3DDef& point) const noexcept;

    /**
     * Transform a 3-D vector using specified transform.
     * @param vector The vector to be transformed.
     * @returns A new transformed vector.
     */
    XS_FUNCTION Vector3DDef transform(const Vector3DDef& vector) const noexcept;

    /**
     * Transform two 3-D points using specified transform.
     * @param point The points to be transformed.
     * @returns The new transformed points.
     */
    template<bool Packed>
    XS_FUNCTION Point3D2Def<Packed> transform(const Point3D2Def<Packed>& point) const noexcept;

    /**
     * Transform two 3-D vectors using specified transform.
     * @param vector The vectors to be transformed.
     * @returns The new transformed vectors.
     */
    template<bool Packed>
    XS_FUNCTION Vector3D2Def<Packed> transform(const Vector3D2Def<Packed>& vector) const noexcept;

    /**
     * Transform four 3-D points using specified transform.
     * @param point The points to be transformed.
     * @returns The new transformed points.
     */
    template<bool Packed>
    XS_FUNCTION Point3D4Def<Packed> transform(const Point3D4Def<Packed>& point) const noexcept;

    /**
     * Transform four 3-D vectors using specified transform.
     * @param vector The vectors to be transformed.
     * @returns The new transformed vectors.
     */
    template<bool Packed>
    XS_FUNCTION Vector3D4Def<Packed> transform(const Vector3D4Def<Packed>& vector) const noexcept;

    /**
     * Determine the inverse of a transform.
     * @returns A new inverse transform.
     */
    XS_FUNCTION Transform inverse() const noexcept;

    /**
     * Fast multiply the transform by a rotation around x axis (i.e M*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated transform.
     */
    XS_FUNCTION Transform postRotateX(InBaseDef rotation) const noexcept;

    /**
     * Fast multiply the transform by a rotation around y axis (i.e M*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated transform.
     */
    XS_FUNCTION Transform postRotateY(InBaseDef rotation) const noexcept;

    /**
     * Fast multiply the transform by a rotation around z axis (i.e M*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated transform.
     */
    XS_FUNCTION Transform postRotateZ(InBaseDef rotation) const noexcept;

    /**
     * Fast multiply a rotation around x axis by the transform (i.e rotation*M).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated transform.
     */
    XS_FUNCTION Transform preRotateX(InBaseDef rotation) const noexcept;

    /**
     * Fast multiply a rotation around y axis by the transform (i.e rotation*M).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated transform.
     */
    XS_FUNCTION Transform preRotateY(InBaseDef rotation) const noexcept;

    /**
     * Fast multiply a rotation around z axis by the transform (i.e rotation*M).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated transform.
     */
    XS_FUNCTION Transform preRotateZ(InBaseDef rotation) const noexcept;

    /**
     * Fast multiply the transform by a uniform scale (i.e M*scale).
     * @param scale The amount to scale along all axis.
     * @returns The rotated transform.
     */
    XS_FUNCTION Transform postUniformScale(BaseDef scale) const noexcept;

    /**
     * Fast multiply the a uniform scale by a transform (i.e scale*M).
     * @param scale The amount to scale along all axis.
     * @returns The rotated transform.
     */
    XS_FUNCTION Transform preUniformScale(BaseDef scale) const noexcept;

    /**
     * Fast multiply the transform by a translation (i.e M*translate).
     * @param translation The amount to translate in the X/Y/Z directions.
     * @returns The rotated transform.
     */
    XS_FUNCTION Transform postTranslate(const SIMD3Def& translation) const noexcept;

    /**
     * Fast multiply a translation by a transform (i.e translate*M).
     * @param translation The 3 translate values for each axis.
     * @returns The rotated transform.
     */
    XS_FUNCTION Transform preTranslate(const SIMD3Def& translation) const noexcept;
};

/**
 * Multiply two Transforms together.
 * @param transform1 The first transform.
 * @param transform2 Transform to multiply the first by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Matrix>
XS_FUNCTION Transform<T, Width, Matrix> operator*(
    const Transform<T, Width, Matrix>& transform1, const Transform<T, Width, Matrix>& transform2) noexcept;

/**
 * Perform compound assignment and multiplication with a Transform.
 * @param [in,out] transform1 The first transform.
 * @param          transform2 Transform to multiply the first by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Matrix>
XS_FUNCTION Transform<T, Width, Matrix>& operator*=(
    Transform<T, Width, Matrix>& transform1, const Transform<T, Width, Matrix>& transform2) noexcept;
} // namespace Shift
