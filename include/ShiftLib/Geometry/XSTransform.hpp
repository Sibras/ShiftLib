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

#include "Geometry/XSMatrix3x4.hpp"
#include "Geometry/XSPoint3D4.hpp"
#include "Geometry/XSQuaternion.hpp"
#include "Geometry/XSVector3D4.hpp"

namespace Shift {
template<typename T, bool Matrix, SIMDWidth Width>
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
        Matrix3x4Data<T> matrix;
    };

public:
    InternData<Matrix> data;

    /** Default constructor. */
    XS_INLINE TransformData() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE TransformData(const TransformData& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE TransformData(TransformData&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE TransformData& operator=(const TransformData& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE TransformData& operator=(TransformData&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit TransformData(const Transform<T, Matrix, Width>& other) noexcept
    {
        store(other);
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const Transform<T, Matrix, Width>& other) noexcept
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

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = Matrix ? defaultWidthSIMD512<T> : defaultWidthSIMD128<T>>
    XS_INLINE Transform<T, Matrix, Width> load() const noexcept
    {
        if constexpr (Matrix) {
            return Transform<T, Matrix, Width>(
                this->data.matrix.template load<Transform<T, Matrix, Width>::Matrix3x4Def::widthImpl>());
        } else {
            const typename Transform<T, Matrix, Width>::QuaternionDef quaternion(this->data.quaternion.load());
            // Pack in single float4 for faster load/store
            const SIMD4<T, Width> translateScale(this->data.translationScale.template load<Width>());
            return Transform<T, Matrix, Width>(quaternion,
                typename Transform<T, Matrix, Width>::Vector3DDef(translateScale.template getValue3<0, 1, 2>()),
                translateScale.template getValue<3>());
        }
    }
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
        Matrix3x4DataPad<T> matrix;
    };

public:
    InternData<Matrix> data;

    /** Default constructor. */
    XS_INLINE TransformDataPad() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE TransformDataPad(const TransformDataPad& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE TransformDataPad(TransformDataPad&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE TransformDataPad& operator=(const TransformDataPad& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE TransformDataPad& operator=(TransformDataPad&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit TransformDataPad(const Transform<T, Matrix, Width>& other) noexcept
    {
        store(other);
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const Transform<T, Matrix, Width>& other) noexcept
    {
        if constexpr (Matrix) {
            this->data.matrix.store(other.data.matrix);
        } else {
            this->data.quaternion.store(other.data.quaternion);
            this->data.translation.store(other.data.translation);
            this->data.scale.store(other.data.scale);
        }
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = Matrix ? defaultWidthSIMD512<T> : defaultWidthSIMD128<T>>
    XS_INLINE Transform<T, Matrix, Width> load() const noexcept
    {
        if constexpr (Matrix) {
            return Transform<T, Matrix, Width>(
                this->data.matrix.template load<Transform<T, Matrix, Width>::Matrix3x4Def::widthImpl>());
        } else {
            return Transform<T, Matrix, Width>(
                this->data.quaternion.template load<Transform<T, Matrix, Width>::QuaternionDef::widthImpl>(),
                this->data.translation.template load<Transform<T, Matrix, Width>::Vector3DDef::widthImpl>(),
                this->data.scale.template load<Transform<T, Matrix, Width>::BaseDef::widthImpl>());
        }
    }
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
template<typename T, bool Matrix = false, SIMDWidth Width = Matrix ? defaultWidthSIMD512<T> : defaultWidthSIMD128<T>>
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
        using Matrix3x4Def = Matrix3x4<T, Quaternion<T, Width>::widthImpl>;
        static constexpr SIMDWidth widthImpl = Matrix3x4Def::widthImpl;
        using BaseDef = typename Matrix3x4Def::BaseDef;
        using InBaseDef = typename Matrix3x4Def::InBaseDef;
        using SIMD3Def = typename Matrix3x4Def::SIMD3Def;

        Matrix3x4Def matrix; /**< The transforms internal representation. */
    };

public:
    using Type = T;
    static constexpr SIMDWidth width = Width;
    using Matrix3x4Def = Matrix3x4<T, Width>;
    using QuaternionDef = Quaternion<T, Width>;
    using InternalData = InternData<Matrix>;
    static constexpr SIMDWidth widthImpl = InternalData::widthImpl;
    using Data = TransformData<T, Matrix>;
    using DataPad = TransformDataPad<T, Matrix>;
    using BaseDef = typename InternalData::BaseDef;
    using InBaseDef = typename InternalData::InBaseDef;
    using SIMD3Def = typename InternalData::SIMD3Def;
    using Point3DDef = Point3D<T, widthImpl>;
    template<bool Packed>
    using Point3D2Def = Point3D2<T, Packed, widthImpl>;
    template<bool Packed>
    using Point3D4Def = Point3D4<T, Packed, widthImpl>;
    using Vector3DDef = Vector3D<T, widthImpl>;
    template<bool Packed>
    using Vector3D2Def = Vector3D2<T, Packed, widthImpl>;
    template<bool Packed>
    using Vector3D4Def = Vector3D4<T, Packed, widthImpl>;

    InternalData data;

    /** Default constructor. */
    XS_INLINE Transform() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Transform(const Transform& other) noexcept = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE Transform(Transform&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Transform& operator=(const Transform& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Transform& operator=(Transform&& other) noexcept = default;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_INLINE explicit Transform(const Transform<T, Matrix, Width2>& other) noexcept
    {
        if constexpr (Matrix) {
            this->data.matrix = Matrix3x4Def(other.data.matrix);
        } else {
            this->data.quaternion = QuaternionDef(other.data.quaternion);
            this->data.translation = SIMD3Def(other.data.translation);
            this->data.scale = BaseDef(other.data.scale);
        }
    }

    /**
     * Construct a transform from a Matrix3x4.
     * @param matrix The new transform transform.
     */
    XS_INLINE explicit Transform(const Matrix3x4Def& matrix) noexcept
    {
        if constexpr (Matrix) {
            this->data.matrix = matrix;
        } else {
            this->data.quaternion = QuaternionDef(matrix);
            this->data.translation = matrix.template getColumn<3>();
            this->data.scale = matrix.template getColumn<0>().length();
        }
    }

    /**
     * Construct a transform from a Quaternion, translation and scale.
     * @param quaternion  The new rotation component of transform.
     * @param translation The new translation component.
     * @param scale       The new scale component.
     */
    XS_INLINE Transform(const QuaternionDef& quaternion, const Vector3DDef& translation, BaseDef scale) noexcept
    {
        if constexpr (Matrix) {
            this->data.matrix = Matrix3x4Def(quaternion, translation).postUniformScale(scale);
        } else {
            this->data.quaternion = quaternion;
            this->data.translation = translation;
            this->data.scale = scale;
        }
    }

    /**
     * Constructor to build a Identity Matrix.
     * @returns Newly constructed Transform with required attributes.
     */
    XS_INLINE static Transform Identity() noexcept
    {
        if constexpr (Matrix) {
            return Transform(Matrix3x4Def::Identity());
        } else {
            return Transform(QuaternionDef::Identity(), Vector3DDef::Zero(), BaseDef::One());
        }
    }

    /**
     * Constructor to build a Transform with fixed translation.
     * @param translation The amount to translate in the X/Y/Z directions.
     * @returns Newly constructed Transform with required attributes.
     */
    XS_INLINE static Transform Translation(const SIMD3Def& translation) noexcept
    {
        if constexpr (Matrix) {
            return Transform(Matrix3x4Def::Translation(translation));
        } else {
            return Transform(QuaternionDef::Identity(), Vector3DDef(translation), BaseDef::One());
        }
    }

    /**
     * Constructor to build a Transform with fixed scale.
     * @param scale The amount to scale in the X/Y/Z direction.
     * @returns Newly constructed Transform with required attributes.
     */
    XS_INLINE static Transform UniformScale(InBaseDef scale) noexcept
    {
        if constexpr (Matrix) {
            return Transform(Matrix3x4Def::UniformScale(scale));
        } else {
            return Transform(QuaternionDef::Identity(), Vector3DDef::Zero(), BaseDef(scale));
        }
    }

    /**
     * Constructor to build a Transform with fixed rotation about X axis.
     * @param rotation The amount to rotate the transform by (in radians).
     * @returns Newly constructed Transform with required attributes.
     */
    XS_INLINE static Transform RotationX(InBaseDef rotation) noexcept
    {
        if constexpr (Matrix) {
            return Transform(Matrix3x4Def::RotationX(rotation));
        } else {
            return Transform(QuaternionDef::RotationX(rotation), Vector3DDef::Zero(), BaseDef(1.0f));
        }
    }

    /**
     * Constructor to build a Transform with fixed rotation about Y axis.
     * @param rotation The amount to rotate the transform by (in radians).
     * @returns Newly constructed Transform with required attributes.
     */
    XS_INLINE static Transform RotationY(InBaseDef rotation) noexcept
    {
        if constexpr (Matrix) {
            return Transform(Matrix3x4Def::RotationY(rotation));
        } else {
            return Transform(QuaternionDef::RotationY(rotation), Vector3DDef::Zero(), BaseDef(1.0f));
        }
    }

    /**
     * Constructor to build a Transform with fixed rotation about Z axis.
     * @param rotation The amount to rotate the transform by (in radians).
     * @returns Newly constructed Transform with required attributes.
     */
    XS_INLINE static Transform RotationZ(InBaseDef rotation) noexcept
    {
        if constexpr (Matrix) {
            return Transform(Matrix3x4Def::RotationZ(rotation));
        } else {
            return Transform(QuaternionDef::RotationZ(rotation), Vector3DDef::Zero(), BaseDef(1.0f));
        }
    }

    /**
     * Constructor to build a Transform with fixed rotation about arbitrary axis.
     * @param axis     The axis to rotate around.
     * @param rotation The amount to rotate the transform by (in radians).
     * @returns Newly constructed Transform with required attributes.
     */
    XS_INLINE static Transform RotationAxis(const Vector3DDef& axis, InBaseDef rotation) noexcept
    {
        if constexpr (Matrix) {
            return Transform(Matrix3x4Def::RotationAxis(axis, rotation));
        } else {
            return Transform(QuaternionDef::RotationAxis(axis, rotation), Vector3DDef::Zero(), BaseDef(1.0f));
        }
    }

    /**
     * Transform a 3-D point using specified transform.
     * @param point The point to be transformed.
     * @returns A new transformed point.
     */
    XS_INLINE Point3DDef transform(const Point3DDef& point) const noexcept
    {
        if constexpr (Matrix) {
            return this->data.matrix.transform(point);
        } else {
            return (this->data.quaternion.transform(point) * Point3DDef::BaseDef(this->data.scale)) +
                this->data.translation;
        }
    }

    /**
     * Transform a 3-D vector using specified transform.
     * @param vector The vector to be transformed.
     * @returns A new transformed vector.
     */
    XS_INLINE Vector3DDef transform(const Vector3DDef& vector) const noexcept
    {
        if constexpr (Matrix) {
            return this->data.matrix.transform(vector);
        } else {
            return this->data.quaternion.transform(vector) * Vector3DDef::BaseDef(this->data.scale);
        }
    }

    /**
     * Transform two 3-D points using specified transform.
     * @param point The points to be transformed.
     * @returns The new transformed points.
     */
    template<bool Packed>
    XS_INLINE Point3D2Def<Packed> transform(const Point3D2Def<Packed>& point) const noexcept
    {
        if constexpr (Matrix) {
            return this->data.matrix.transform(point);
        } else {
            return Point3D2Def<Packed>(
                       this->data.quaternion.transform(point).points * Point3D2Def<Packed>::BaseDef(this->data.scale)) +
                this->data.translation;
        }
    }

    /**
     * Transform two 3-D vectors using specified transform.
     * @param vector The vectors to be transformed.
     * @returns The new transformed vectors.
     */
    template<bool Packed>
    XS_INLINE Vector3D2Def<Packed> transform(const Vector3D2Def<Packed>& vector) const noexcept
    {
        if constexpr (Matrix) {
            return this->data.matrix.transform(vector);
        } else {
            return this->data.quaternion.transform(vector) * Vector3D2Def<Packed>::BaseDef(this->data.scale);
        }
    }

    /**
     * Transform four 3-D points using specified transform.
     * @param point The points to be transformed.
     * @returns The new transformed points.
     */
    template<bool Packed>
    XS_INLINE Point3D4Def<Packed> transform(const Point3D4Def<Packed>& point) const noexcept
    {
        if constexpr (Matrix) {
            return this->data.matrix.transform(point);
        } else {
            return Point3D4Def<Packed>(
                       this->data.quaternion.transform(point).points * Point3D4Def<Packed>::BaseDef(this->data.scale)) +
                this->data.translation;
        }
    }

    /**
     * Transform four 3-D vectors using specified transform.
     * @param vector The vectors to be transformed.
     * @returns The new transformed vectors.
     */
    template<bool Packed>
    XS_INLINE Vector3D4Def<Packed> transform(const Vector3D4Def<Packed>& vector) const noexcept
    {
        if constexpr (Matrix) {
            return this->data.matrix.transform(vector);
        } else {
            return this->data.quaternion.transform(vector) * Vector3D4Def<Packed>::BaseDef(this->data.scale);
        }
    }

    /**
     * Determine the inverse of a transform.
     * @returns A new inverse transform.
     */
    XS_INLINE Transform inverse() const noexcept
    {
        if constexpr (Matrix) {
            return Transform(this->data.matrix.inverse());
        } else {
            return Transform(this->data.quaternion.inverse(), -this->data.translation, this->data.scale.reciprocal());
        }
    }

    /**
     * Fast multiply the transform by a rotation around x axis (i.e M*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated transform.
     */
    XS_INLINE Transform postRotateX(InBaseDef rotation) const noexcept
    {
        if constexpr (Matrix) {
            return Transform(this->data.matrix.postRotateX(rotation));
        } else {
            return Transform(this->data.quaternion.postRotateX(rotation), this->data.translation, this->data.scale);
        }
    }

    /**
     * Fast multiply the transform by a rotation around y axis (i.e M*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated transform.
     */
    XS_INLINE Transform postRotateY(InBaseDef rotation) const noexcept
    {
        if constexpr (Matrix) {
            return Transform(this->data.matrix.postRotateY(rotation));
        } else {
            return Transform(this->data.quaternion.postRotateY(rotation), this->data.translation, this->data.scale);
        }
    }

    /**
     * Fast multiply the transform by a rotation around z axis (i.e M*rotation).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated transform.
     */
    XS_INLINE Transform postRotateZ(InBaseDef rotation) const noexcept
    {
        if constexpr (Matrix) {
            return Transform(this->data.matrix.postRotateZ(rotation));
        } else {
            return Transform(this->data.quaternion.postRotateZ(rotation), this->data.translation, this->data.scale);
        }
    }

    /**
     * Fast multiply a rotation around x axis by the transform (i.e rotation*M).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated transform.
     */
    XS_INLINE Transform preRotateX(InBaseDef rotation) const noexcept
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

    /**
     * Fast multiply a rotation around y axis by the transform (i.e rotation*M).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated transform.
     */
    XS_INLINE Transform preRotateY(InBaseDef rotation) const noexcept
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

    /**
     * Fast multiply a rotation around z axis by the transform (i.e rotation*M).
     * @param rotation The angle to rotate by (in radians).
     * @returns The rotated transform.
     */
    XS_INLINE Transform preRotateZ(InBaseDef rotation) const noexcept
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

    /**
     * Fast multiply the transform by a uniform scale (i.e M*scale).
     * @param scale The amount to scale along all axis.
     * @returns The rotated transform.
     */
    XS_INLINE Transform postUniformScale(BaseDef scale) const noexcept
    {
        if constexpr (Matrix) {
            return Transform(this->data.matrix.postUniformScale(scale));
        } else {
            return Transform(this->data.quaternion, this->data.translation, this->data.scale * scale);
        }
    }

    /**
     * Fast multiply the a uniform scale by a transform (i.e scale*M).
     * @param scale The amount to scale along all axis.
     * @returns The rotated transform.
     */
    XS_INLINE Transform preUniformScale(BaseDef scale) const noexcept
    {
        if constexpr (Matrix) {
            return Transform(this->data.matrix.preUniformScale(scale));
        } else {
            return Transform(this->data.quaternion, this->data.translation * Vector3DDef::BaseDefDef(scale),
                this->data.scale * scale);
        }
    }

    /**
     * Fast multiply the transform by a translation (i.e M*translate).
     * @param translation The amount to translate in the X/Y/Z directions.
     * @returns The rotated transform.
     */
    XS_INLINE Transform postTranslate(const SIMD3Def& translation) const noexcept
    {
        if constexpr (Matrix) {
            return Transform(this->data.matrix.postTranslate(translation));
        } else {
            const Vector3DDef trans(this->data.quaternion.transform(Vector3DDef(translation)));
            return Transform(this->data.quaternion,
                trans.mad(Vector3DDef::BaseDefDef(this->data.scale), this->data.translation), this->data.scale);
        }
    }

    /**
     * Fast multiply a translation by a transform (i.e translate*M).
     * @param translation The 3 translate values for each axis.
     * @returns The rotated transform.
     */
    XS_INLINE Transform preTranslate(const SIMD3Def& translation) const noexcept
    {
        if constexpr (Matrix) {
            return Transform(this->data.matrix.preTranslate(translation));
        } else {
            return Transform(
                this->data.quaternion, this->data.translation + Vector3DDef(translation), this->data.scale);
        }
    }
};

/**
 * Multiply two Transforms together.
 * @param transform1 The first transform.
 * @param transform2 Transform to multiply the first by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Matrix, Width> operator*(
    const Transform<T, Matrix, Width>& transform1, const Transform<T, Matrix, Width>& transform2) noexcept
{
    if constexpr (Matrix) {
        return Transform(transform1.data.matrix * transform2.data.matrix);
    } else {
        //(T1 * T2) * V == T1 * (T2 * V)
        // (1T * (1S * (1Q * (2T * (2S * (2Q * V))))))
        // Effectively this results in a preRotate, preScale followed by preTranslate
        // on transform2;

        // Pre Rotate transform1 by transform2
        const typename Transform<T, Matrix, Width>::QuaternionDef temp(
            transform1.data.quaternion * transform2.data.quaternion);
        typename Transform<T, Matrix, Width>::Vector3DDef translate(
            transform2.data.quaternion.transform(transform1.data.translation));

        // Pre Scale transform1 by transform2 and add new translation to existing
        translate = translate.mad(typename Transform<T, Matrix, Width>::Vector3DDef::BaseDef(transform2.data.scale),
            transform2.data.translation);

        return Transform(temp, translate,
            typename Transform<T, Matrix, Width>::BaseDef(transform2.data.scale * transform1.data.scale));
    }
}

/**
 * Perform compound assignment and multiplication with a Transform.
 * @param [in,out] transform1 The first transform.
 * @param          transform2 Transform to multiply the first by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Matrix>
XS_INLINE Transform<T, Matrix, Width>& operator*=(
    Transform<T, Matrix, Width>& transform1, const Transform<T, Matrix, Width>& transform2) noexcept
{
    if constexpr (Matrix) {
        transform1.data.matrix *= transform2.data.matrix;
    } else {
        // Pre Rotate transform1 by transform2
        transform1.data.quaternion *= transform2.data.quaternion;
        transform1.data.translation = transform2.data.quaternion.transform(transform1.data.translation);

        // Pre Scale transform1 by transform2 and add new translation to existing
        transform1.data.translation = transform1.data.translation.mad(
            typename Transform<T, Matrix, Width>::Vector3DDef::BaseDef(transform2.data.scale),
            transform2.data.translation);
        transform1.data.scale *= transform2.data.scale;
    }
    return transform1;
}
} // namespace Shift
