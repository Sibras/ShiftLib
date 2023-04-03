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

#include "Geometry/XSVector3D.hpp"
#include "SIMD/XSSIMD3x2.hpp"

namespace Shift {
template<typename T, bool Packed, SIMDWidth Width>
class Vector3D2;

template<typename T, bool Packed = false>
class Vector3D2Data
{
    template<bool IsPacked>
    struct InternData
    {
        using Vector3D2InternData = SIMD6Data<T>;
    };

    template<>
    struct InternData<false>
    {
        using Vector3D2InternData = SIMD3x2Data<T>;
    };

public:
    typename InternData<Packed>::Vector3D2InternData values;

    /** Default constructor. */
    XS_INLINE Vector3D2Data() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Vector3D2Data(const Vector3D2Data& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE Vector3D2Data(Vector3D2Data&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Vector3D2Data& operator=(const Vector3D2Data& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Vector3D2Data& operator=(Vector3D2Data&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit Vector3D2Data(const Vector3D2<T, Packed, Width>& other) noexcept
        : values(other.vectors)
    {}

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param v0X The X value of the first vector.
     * @param v1X The X value of the second vector.
     * @param v0Y The Y value of the first vector.
     * @param v1Y The Y value of the second vector.
     * @param v0Z The Z value of the first vector.
     * @param v1Z The Z value of the second vector.
     */
    XS_INLINE void setData(T v0X, T v1X, T v0Y, T v1Y, T v0Z, T v1Z) noexcept
    {
        if constexpr (!Packed) {
            this->values.setData(v0X, v0Y, v0Z, v1X, v1Y, v1Z);
        } else {
            this->values.setData(v0X, v1X, v0Y, v1Y, v0Z, v1Z);
        }
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const Vector3D2<T, Packed, Width>& other) noexcept
    {
        this->values.store(other.vectors);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD256<T>>
    XS_INLINE Vector3D2<T, Packed, Width> load() const noexcept
    {
        return Vector3D2<T, Packed, Width>(this->values.template load<Vector3D2<T, Packed, Width>::widthImpl>());
    }
};

template<typename T, bool Packed = false, typename T2 = T>
class Vector3D2DataPad
{
    template<bool IsPacked>
    struct InternData
    {
        using Vector3D2InternDataPad = SIMD6DataPad<T, T2>;
    };

    template<>
    struct InternData<false>
    {
        using Vector3D2InternDataPad = SIMD3x2DataPad<T, T2>;
    };

public:
    typename InternData<Packed>::Vector3D2InternDataPad values;

    /** Default constructor. */
    XS_INLINE Vector3D2DataPad() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Vector3D2DataPad(const Vector3D2DataPad& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE Vector3D2DataPad(Vector3D2DataPad&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Vector3D2DataPad& operator=(const Vector3D2DataPad& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Vector3D2DataPad& operator=(Vector3D2DataPad&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit Vector3D2DataPad(const Vector3D2<T, Packed, Width>& other) noexcept
        : values(other.vectors)
    {}

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param v0X The X value of the first vector.
     * @param v1X The X value of the second vector.
     * @param v0Y The Y value of the first vector.
     * @param v1Y The Y value of the second vector.
     * @param v0Z The Z value of the first vector.
     * @param v1Z The Z value of the second vector.
     */
    XS_INLINE void setData(T v0X, T v1X, T v0Y, T v1Y, T v0Z, T v1Z) noexcept
    {
        if constexpr (!Packed) {
            this->values.setData(v0X, v0Y, v0Z, v1X, v1Y, v1Z);
        } else {
            this->values.setData(v0X, v1X, v0Y, v1Y, v0Z, v1Z);
        }
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const Vector3D2<T, Packed, Width>& other) noexcept
    {
        this->values.store(other.vectors);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD256<T>>
    XS_INLINE Vector3D2<T, Packed, Width> load() const noexcept
    {
        return Vector3D2<T, Packed, Width>(this->values.template load<Vector3D2<T, Packed, Width>::widthImpl>());
    }
};

/**
 * A  base type used to store 2 3-D vectors.
 * @note Cross product operations on 3-D vectors contained in this library are
 *  performed in a right-hand coordinate system.
 * @tparam T      Generic type parameter.
 * @tparam Width  Type of SIMD being used.
 * @tparam Packed Type of storage used for internal data.
 */
template<typename T, bool Packed = false, SIMDWidth Width = defaultWidthSIMD256<T>>
class Vector3D2
{
    template<bool IsPacked>
    struct InternData
    {
        using Vector3D2Intern = SIMD6<T, SIMD6<T, Width>::widthImpl>;
        using SIMD6Def = Vector3D2Intern;
        using SIMD3x2Def = SIMD3x2<T, SIMD3x2<T, Vector3D2Intern::widthImpl>::widthImpl>;
    };

    template<>
    struct InternData<false>
    {
        using Vector3D2Intern = SIMD3x2<T, SIMD3x2<T, Width>::widthImpl>;
        using SIMD6Def = typename Vector3D2Intern::SIMD6Def;
        using SIMD3x2Def = Vector3D2Intern;
    };

public:
    using Type = T;
    using Vector3D2Def = typename InternData<Packed>::Vector3D2Intern;
    using Data = Vector3D2Data<T, Packed>;
    template<typename T2 = Type>
    using DataPad = Vector3D2DataPad<T, Packed, T2>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Vector3D2Def::widthImpl;
    static constexpr bool packed = Packed;
    using BaseDef = typename Vector3D2Def::BaseDef;
    using InBaseDef = typename Vector3D2Def::InBaseDef;
    using SIMD6Def = typename InternData<Packed>::SIMD6Def;
    using SIMD3x2Def = typename InternData<Packed>::SIMD3x2Def;
    using Vector3DDef = Vector3D<T, Vector3D<T, widthImpl>::widthImpl>;
    using SIMD2Def = typename Vector3D2Def::SIMD2Def;

    Vector3D2Def vectors;

    /** Default constructor. */
    XS_INLINE Vector3D2() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Vector3D2(const Vector3D2& other) noexcept = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE Vector3D2(Vector3D2&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Vector3D2& operator=(const Vector3D2& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Vector3D2& operator=(Vector3D2&& other) noexcept = default;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_INLINE explicit Vector3D2(const Vector3D2<T, Packed, Width2>& other) noexcept
        : vectors(other.vectors)
    {}

    /**
     * Construct a vector3d2 from a SIMD6.
     * @param values The SIMD6.
     */
    XS_INLINE explicit Vector3D2(const SIMD6Def& values) noexcept
        : vectors(values)
    {}

    /**
     * Construct a vector3d2 from a SIMD3x2.
     * @param values The SIMD3x2.
     */
    XS_INLINE explicit Vector3D2(const SIMD3x2Def& values) noexcept
        : vectors(values)
    {}

    /**
     * Construct a vector3d2 from 4 vector3s.
     * @param vector0 The first vector.
     * @param vector1 The second vector.
     */
    XS_INLINE Vector3D2(const Vector3DDef& vector0, const Vector3DDef& vector1) noexcept
        : vectors(vector0.values, vector1.values)
    {}

    /**
     * Construct a vector3d2 from 6 values representing 2 different vector3s.
     * @param v0X The X value of the first vector.
     * @param v1X The X value of the second vector.
     * @param v0Y The Y value of the first vector.
     * @param v1Y The Y value of the second vector.
     * @param v0Z The Z value of the first vector.
     * @param v1Z The Z value of the second vector.
     */
    XS_INLINE Vector3D2(T v0X, T v1X, T v0Y, T v1Y, T v0Z, T v1Z) noexcept
    {
        if constexpr (!Packed) {
            vectors = Vector3D2Def(v0X, v0Y, v0Z, v1X, v1Y, v1Z);
        } else {
            vectors = Vector3D2Def(v0X, v1X, v0Y, v1Y, v0Z, v1Z);
        }
    }

    /**
     * Construct a vector3d2 from a single vector.
     * @param vector The vector.
     */
    XS_INLINE explicit Vector3D2(const Vector3DDef& vector) noexcept
        : vectors(vector.values)
    {}

    /**
     * Get a contained 3-D vector.
     * @tparam Index The index of the vector to retrieve (must be between 0 and 1).
     * @returns Vector3D containing the desired value.
     */
    template<uint32_t Index>
    XS_INLINE Vector3DDef getVector() const noexcept
    {
        return Vector3DDef(this->vectors.template getValue3<Index>());
    }

    /**
     * Set the first 3-D vector.
     * @tparam Index The index of the vector to set (must be between 0 and 1).
     * @param vector The new vector.
     */
    template<uint32_t Index>
    XS_INLINE void setVector(const Vector3DDef& vector) noexcept
    {
        this->vectors.template setValue3<Index>(vector.values);
    }

    /**
     * Multiply a 3-D vector2 by a scalar and then add another vector3d2.
     * @param value    Value to multiply the vector by.
     * @param vector Second vector to add.
     * @returns The result of the operation.
     */
    XS_INLINE Vector3D2 mad(const SIMD2Def& value, const Vector3D2<T, Packed, Width>& vector) const noexcept
    {
        return Vector3D2(this->vectors.mad(value, vector.vectors));
    }

    /**
     * Compute the square of the length of a 3-D vector2.
     * @returns The result of the operation.
     */
    XS_INLINE SIMD2Def lengthSqr() const noexcept
    {
        return this->vectors.lengthSqr3();
    }

    /**
     * Compute the length of a 3-D vector2.
     * @returns The result of the operation.
     */
    XS_INLINE SIMD2Def length() const noexcept
    {
        return this->vectors.length3();
    }

    /**
     * Normalise a vector2.
     * @note The result is unpredictable when all elements of vector are at or near zero.
     * @returns A new vector corresponding to the normalised vector.
     */
    XS_INLINE Vector3D2 normalize() const noexcept
    {
        return Vector3D2(this->vectors.normalize3());
    }

    /**
     * Compute the dot product of two 3-D vector2s.
     * @param vector The second vector3d2.
     * @returns SIMD2Def containing the dot product of each stored vector.
     */
    XS_INLINE SIMD2Def dot3(const Vector3D2<T, Packed, Width>& vector) const noexcept
    {
        return this->vectors.dot3(vector.vectors);
    }

    /**
     * Compute cross product of two 3-D vector2s.
     * @note Uses Right-handed coordinate system.
     * @param vector The second vector3d2.
     * @returns Vector3D2 containing the cross product of each stored vector.
     */
    XS_INLINE Vector3D2 cross3(const Vector3D2<T, Packed, Width>& vector) const noexcept
    {
        return Vector3D2(this->vectors.cross3(vector.vectors));
    }
};

/**
 * Add a vector2 to each component vector in a vector2.
 * @param vector1 The vector3d2.
 * @param vector2 Vector3d to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width> operator+(const Vector3D2<T, Packed, Width>& vector1,
    const typename Vector3D2<T, Packed, Width>::Vector3DDef& vector2) noexcept
{
    return Vector3D2<T, Packed, Width>(vector1.vectors + vector2.values);
}

/**
 * Add two 3-D vector2s.
 * @param vector1 The first vector3d2.
 * @param vector2 Vector3d2 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width> operator+(
    const Vector3D2<T, Packed, Width>& vector1, const Vector3D2<T, Packed, Width>& vector2) noexcept
{
    return Vector3D2<T, Packed, Width>(vector1.vectors + vector2.vectors);
}

/**
 * Subtract a 3-D vector from each component vector in a vector2.
 * @param vector1 The vector3d.
 * @param vector2 Vector3d to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width> operator-(const Vector3D2<T, Packed, Width>& vector1,
    const typename Vector3D2<T, Packed, Width>::Vector3DDef& vector2) noexcept
{
    return Vector3D2<T, Packed, Width>(vector1.vectors - vector2.values);
}

/**
 * Subtract a 3-D vector2 from another 3-D vector2.
 * @param vector1 The first vector3d2.
 * @param vector2 Vector3d2 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width> operator-(
    const Vector3D2<T, Packed, Width>& vector1, const Vector3D2<T, Packed, Width>& vector2) noexcept
{
    return Vector3D2<T, Packed, Width>(vector1.vectors - vector2.vectors);
}

/**
 * Multiply a 3-D vector2 by a scalar.
 * @param vector The vector3d2.
 * @param value  Value to multiply the vector3d2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width> operator*(
    const Vector3D2<T, Packed, Width>& vector, typename Vector3D2<T, Packed, Width>::BaseDef value) noexcept
{
    return Vector3D2<T, Packed, Width>(vector.vectors * value);
}

/**
 * Multiply a 3-D vector2 by a float2.
 * @param vector The vector3d2.
 * @param value  Value to multiply the vector3d2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width> operator*(
    const Vector3D2<T, Packed, Width>& vector, const typename Vector3D2<T, Packed, Width>::SIMD2Def& value) noexcept
{
    return Vector3D2<T, Packed, Width>(vector.vectors * value);
}

/**
 * Multiply each component vector in a vector2 by a vector.
 * @param vector1 The vector3d2.
 * @param vector2 Value to multiply the vector3d2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width> operator*(const Vector3D2<T, Packed, Width>& vector1,
    const typename Vector3D2<T, Packed, Width>::Vector3DDef& vector2) noexcept
{
    return Vector3D2<T, Packed, Width>(vector1.vectors * vector2.values);
}

/**
 * Multiply a 3-D vector2 by another vector.
 * @param vector1 The vector3d2.
 * @param vector2 Value to multiply the first vector3d2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width> operator*(
    const Vector3D2<T, Packed, Width>& vector1, const Vector3D2<T, Packed, Width>& vector2) noexcept
{
    return Vector3D2<T, Packed, Width>(vector1.vectors * vector2.vectors);
}

/**
 * Divide a 3-D vector2 by a scalar.
 * @param vector The vector3d2.
 * @param value  Value to divide the vector3d2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width> operator/(
    const Vector3D2<T, Packed, Width>& vector, typename Vector3D2<T, Packed, Width>::BaseDef value) noexcept
{
    return Vector3D2<T, Packed, Width>(vector.vectors / value);
}

/**
 * Divide a 3-D vector2 by a float2.
 * @param vector The vector3d2.
 * @param value  Value to divide the vector3d2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width> operator/(
    const Vector3D2<T, Packed, Width>& vector, const typename Vector3D2<T, Packed, Width>::SIMD2Def& value) noexcept
{
    return Vector3D2<T, Packed, Width>(vector.vectors / value);
}

/**
 * Negate all elements of a 3-D vector2.
 * @param vector The vector3d2 to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width> operator-(const Vector3D2<T, Packed, Width>& vector) noexcept
{
    return Vector3D2<T, Packed, Width>(-vector.vectors);
}

/**
 * Perform compound assignment and addition with a vector and a vector2.
 * @param [in,out] vector1 The vector3d2.
 * @param          vector2 Vector3d to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width>& operator+=(
    Vector3D2<T, Packed, Width>& vector1, const typename Vector3D2<T, Packed, Width>::Vector3DDef& vector2) noexcept
{
    vector1.vectors += vector2.values;
    return vector1;
}

/**
 * Perform compound assignment and addition with a 3-D vector2.
 * @param [in,out] vector1 The first vector3d2.
 * @param          vector2 Vector3D2 to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width>& operator+=(
    Vector3D2<T, Packed, Width>& vector1, const Vector3D2<T, Packed, Width>& vector2) noexcept
{
    vector1.vectors += vector2.vectors;
    return vector1;
}

/**
 * Perform compound assignment and subtraction with a vector and a vector2.
 * @param [in,out] vector1 The vector3d2.
 * @param          vector2 Vector3d to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width>& operator-=(
    Vector3D2<T, Packed, Width>& vector1, const typename Vector3D2<T, Packed, Width>::Vector3DDef& vector2) noexcept
{
    vector1.vectors -= vector2.values;
    return vector1;
}

/**
 * Perform compound assignment and subtraction by a 3-D vector2.
 * @param [in,out] vector1 The first vector3d2.
 * @param          vector2 Vector3D2 to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width>& operator-=(
    Vector3D2<T, Packed, Width>& vector1, const Vector3D2<T, Packed, Width>& vector2) noexcept
{
    vector1.vectors -= vector2.vectors;
    return vector1;
}

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] vector The vector3d2.
 * @param          value  Value to multiply the vector3d2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width>& operator*=(
    Vector3D2<T, Packed, Width>& vector, typename Vector3D2<T, Packed, Width>::BaseDef value) noexcept
{
    vector.vectors *= value;
    return vector;
}

/**
 * Perform compound assignment and multiplication by a SIMD2Def.
 * @param [in,out] vector The vector3d2.
 * @param          value  Value to multiply the vector3d2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width>& operator*=(
    Vector3D2<T, Packed, Width>& vector, const typename Vector3D2<T, Packed, Width>::SIMD2Def& value) noexcept
{
    vector.vectors *= value;
    return vector;
}

/**
 * Perform compound assignment and multiplication with a vector and a vector2.
 * @param [in,out] vector1 The vector3d2.
 * @param          vector2 Vector3d to multiply the fist vector3d2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width>& operator*=(
    Vector3D2<T, Packed, Width>& vector1, const typename Vector3D2<T, Packed, Width>::Vector3DDef& vector2) noexcept
{
    vector1.vectors *= vector2.values;
    return vector1;
}

/**
 * Perform compound assignment and multiplication by another vector.
 * @param [in,out] vector1 The vector3d2.
 * @param          vector2 Value to multiply the fist vector3d2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width>& operator*=(
    Vector3D2<T, Packed, Width>& vector1, const Vector3D2<T, Packed, Width>& vector2) noexcept
{
    vector1.vectors *= vector2.vectors;
    return vector1;
}

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] vector The vector3d2.
 * @param          value  Value to divide the vector3d2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width>& operator/=(
    Vector3D2<T, Packed, Width>& vector, typename Vector3D2<T, Packed, Width>::BaseDef value) noexcept
{
    vector.vectors /= value;
    return vector;
}

/**
 * Perform compound assignment and division by a SIMD2Def.
 * @param [in,out] vector The vector3d2.
 * @param          value  Value to divide the vector3d2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D2<T, Packed, Width>& operator/=(
    Vector3D2<T, Packed, Width>& vector, const typename Vector3D2<T, Packed, Width>::SIMD2Def& value) noexcept
{
    vector.vectors /= value;
    return vector;
}
} // namespace Shift
