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

#include "Geometry/XSVector3D2.hpp"
#include "SIMD/XSSIMD3x4.hpp"

namespace Shift {
template<typename T, bool Packed, SIMDWidth Width>
class Vector3D4;

template<typename T, bool Packed = false>
class Vector3D4Data
{
    template<bool IsPacked>
    struct InternData
    {
        using Vector3D4InternData = SIMD12Data<T>;
    };

    template<>
    struct InternData<false>
    {
        using Vector3D4InternData = SIMD3x4Data<T>;
    };

public:
    typename InternData<Packed>::Vector3D4InternData values;

    /** Default constructor. */
    XS_INLINE Vector3D4Data() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Vector3D4Data(const Vector3D4Data& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE Vector3D4Data(Vector3D4Data&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Vector3D4Data& operator=(const Vector3D4Data& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Vector3D4Data& operator=(Vector3D4Data&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit Vector3D4Data(const Vector3D4<T, Packed, Width>& other) noexcept
        : values(other.vectors)
    {}

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param v0X The X value of the first vector.
     * @param v1X The X value of the second vector.
     * @param v2X The X value of the third vector.
     * @param v3X The X value of the fourth vector.
     * @param v0Y The Y value of the first vector.
     * @param v1Y The Y value of the second vector.
     * @param v2Y The Y value of the third vector.
     * @param v3Y The Y value of the fourth vector.
     * @param v0Z The Z value of the first vector.
     * @param v1Z The Z value of the second vector.
     * @param v2Z The Z value of the third vector.
     * @param v3Z The Z value of the fourth vector.
     */
    XS_INLINE void setData(T v0X, T v1X, T v2X, T v3X, T v0Y, T v1Y, T v2Y, T v3Y, T v0Z, T v1Z, T v2Z, T v3Z) noexcept
    {
        if constexpr (!Packed) {
            this->values.setData(v0X, v0Y, v0Z, v1X, v1Y, v1Z, v2X, v2Y, v2Z, v3X, v3Y, v3Z);
        } else {
            this->values.setData(v0X, v1X, v2X, v3X, v0Y, v1Y, v2Y, v3Y, v0Z, v1Z, v2Z, v3Z);
        }
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const Vector3D4<T, Packed, Width>& other) noexcept
    {
        this->values.store(other.vectors);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD512<T>>
    XS_INLINE Vector3D4<T, Packed, Width> load() const noexcept
    {
        return Vector3D4<T, Packed, Width>(this->values.template load<Vector3D4<T, Packed, Width>::widthImpl>());
    }
};

template<typename T, bool Packed = false, typename T2 = T>
class Vector3D4DataPad
{
    template<bool IsPacked>
    struct InternData
    {
        using Vector3D4InternData = SIMD12DataPad<T, T2>;
    };

    template<>
    struct InternData<false>
    {
        using Vector3D4InternData = SIMD3x4DataPad<T, T2>;
    };

public:
    typename InternData<Packed>::Vector3D4InternData values;

    /** Default constructor. */
    XS_INLINE Vector3D4DataPad() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Vector3D4DataPad(const Vector3D4DataPad& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE Vector3D4DataPad(Vector3D4DataPad&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Vector3D4DataPad& operator=(const Vector3D4DataPad& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Vector3D4DataPad& operator=(Vector3D4DataPad&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit Vector3D4DataPad(const Vector3D4<T, Packed, Width>& other) noexcept
        : values(other.vectors)
    {}

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param v0X The X value of the first vector.
     * @param v1X The X value of the second vector.
     * @param v2X The X value of the third vector.
     * @param v3X The X value of the fourth vector.
     * @param v0Y The Y value of the first vector.
     * @param v1Y The Y value of the second vector.
     * @param v2Y The Y value of the third vector.
     * @param v3Y The Y value of the fourth vector.
     * @param v0Z The Z value of the first vector.
     * @param v1Z The Z value of the second vector.
     * @param v2Z The Z value of the third vector.
     * @param v3Z The Z value of the fourth vector.
     */
    XS_INLINE void setData(T v0X, T v1X, T v2X, T v3X, T v0Y, T v1Y, T v2Y, T v3Y, T v0Z, T v1Z, T v2Z, T v3Z) noexcept
    {
        if constexpr (!Packed) {
            this->values.setData(v0X, v0Y, v0Z, v1X, v1Y, v1Z, v2X, v2Y, v2Z, v3X, v3Y, v3Z);
        } else {
            this->values.setData(v0X, v1X, v2X, v3X, v0Y, v1Y, v2Y, v3Y, v0Z, v1Z, v2Z, v3Z);
        }
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const Vector3D4<T, Packed, Width>& other) noexcept
    {
        this->values.store(other.vectors);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD512<T>>
    XS_INLINE Vector3D4<T, Packed, Width> load() const noexcept
    {
        return Vector3D4<T, Packed, Width>(this->values.template load<Vector3D4<T, Packed, Width>::widthImpl>());
    }
};

/**
 * A base type used to store 4 3-D vectors.
 * @note Cross product operations on 3-D vectors contained in this library are
 *   performed in a right-hand coordinate system.
 * @tparam T      Generic type parameter.
 * @tparam Width  Type of SIMD being used.
 * @tparam Packed Type of storage used for internal data.
 */
template<typename T, bool Packed = false, SIMDWidth Width = defaultWidthSIMD512<T>>
class Vector3D4
{
    template<bool IsPacked>
    struct InternData
    {
        using Vector3D4Intern = SIMD12<T, SIMD12<T, Width>::widthImpl>;
        using SIMD12Def = Vector3D4Intern;
        using SIMD3x4Def = SIMD3x4<T, SIMD3x4<T, Vector3D4Intern::widthImpl>::widthImpl>;
    };

    template<>
    struct InternData<false>
    {
        using Vector3D4Intern = SIMD3x4<T, SIMD3x4<T, Width>::widthImpl>;
        using SIMD12Def = typename Vector3D4Intern::SIMD12Def;
        using SIMD3x4Def = Vector3D4Intern;
    };

public:
    using Type = T;
    using Vector3D4Def = typename InternData<Packed>::Vector3D4Intern;
    using Data = Vector3D4Data<T, Packed>;
    template<typename T2 = Type>
    using DataPad = Vector3D4DataPad<T, Packed, T2>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Vector3D4Def::widthImpl;
    static constexpr bool packed = Packed;
    using BaseDef = typename Vector3D4Def::BaseDef;
    using InBaseDef = typename Vector3D4Def::InBaseDef;
    using SIMD12Def = typename InternData<Packed>::SIMD12Def;
    using SIMD3x4Def = typename InternData<Packed>::SIMD3x4Def;
    using Vector3DDef = Vector3D<T, Vector3D<T, widthImpl>::widthImpl>;
    using Vector3D2Def = Vector3D2<T, Packed, Vector3D2<T, Packed, widthImpl>::widthImpl>;
    using SIMD4Def = typename Vector3D4Def::SIMD4Def;

    Vector3D4Def vectors;

    /** Default constructor. */
    XS_INLINE Vector3D4() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Vector3D4(const Vector3D4& other) noexcept = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE Vector3D4(Vector3D4&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Vector3D4& operator=(const Vector3D4& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Vector3D4& operator=(Vector3D4&& other) noexcept = default;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_INLINE explicit Vector3D4(const Vector3D4<T, Packed, Width2>& other) noexcept
        : vectors(other.vectors)
    {}

    /**
     * Construct a vector3d4 from 2 vector2s.
     * @param vector0 The first vector2.
     * @param vector1 The second vector2.
     */
    XS_INLINE Vector3D4(const Vector3D2Def& vector0, const Vector3D2Def& vector1) noexcept
        : vectors(vector0.vectors, vector1.vectors)
    {}

    /**
     * Construct a vector3d4 from 4 vectors.
     * @param vector0 The first vector.
     * @param vector1 The second vector.
     * @param vector2 The third vector.
     * @param vector3 The fourth vector.
     */
    XS_INLINE Vector3D4(const Vector3DDef& vector0, const Vector3DDef& vector1, const Vector3DDef& vector2,
        const Vector3DDef& vector3) noexcept
        : vectors(vector0.values, vector1.values, vector2.values, vector3.values)
    {}

    /**
     * Construct a vector3d4 from a single vector.
     * @param vector The vector.
     */
    XS_INLINE explicit Vector3D4(const Vector3DDef& vector) noexcept
        : vectors(vector.values)
    {}

    /**
     * Construct a vector3d4 from 12 values representing 4 different vector3s.
     * @param v0X The X value of the first vector.
     * @param v1X The X value of the second vector.
     * @param v2X The X value of the third vector.
     * @param v3X The X value of the fourth vector.
     * @param v0Y The Y value of the first vector.
     * @param v1Y The Y value of the second vector.
     * @param v2Y The Y value of the third vector.
     * @param v3Y The Y value of the fourth vector.
     * @param v0Z The Z value of the first vector.
     * @param v1Z The Z value of the second vector.
     * @param v2Z The Z value of the third vector.
     * @param v3Z The Z value of the fourth vector.
     */
    XS_INLINE Vector3D4(T v0X, T v1X, T v2X, T v3X, T v0Y, T v1Y, T v2Y, T v3Y, T v0Z, T v1Z, T v2Z, T v3Z) noexcept
    {
        if constexpr (!Packed) {
            vectors = Vector3D4Def(v0X, v0Y, v0Z, v1X, v1Y, v1Z, v2X, v2Y, v2Z, v3X, v3Y, v3Z);
        } else {
            vectors = Vector3D4Def(v0X, v1X, v2X, v3X, v0Y, v1Y, v2Y, v3Y, v0Z, v1Z, v2Z, v3Z);
        }
    }

    /**
     * Construct a vector3d4 from a SIMD12.
     * @param values The SIMD12.
     */
    XS_INLINE explicit Vector3D4(const SIMD12Def& values) noexcept
        : vectors(values)
    {}

    /**
     * Construct a vector3d4 from a SIMD3x4.
     * @param values The SIMD3x4.
     */
    XS_INLINE explicit Vector3D4(const SIMD3x4Def& values) noexcept
        : vectors(values)
    {}

    /**
     * Get a contained 3-D vector.
     * @tparam Index The index of the vector to retrieve (must be between 0 and 3).
     * @returns Vector3D containing the desired value.
     */
    template<uint32_t Index>
    XS_INLINE Vector3DDef getVector() const noexcept
    {
        return Vector3DDef(this->vectors.template getValue3<Index>());
    }

    /**
     * Set the first 3-D vector.
     * @tparam Index The index of the vector to set (must be between 0 and 3).
     * @param vector The new vector.
     */
    template<uint32_t Index>
    XS_INLINE void setVector(const Vector3DDef& vector) noexcept
    {
        this->vectors.template setValue3<Index>(vector.values);
    }

    /**
     * Multiply a 3-D vector4 by a scalar and then add another vector3d4.
     * @param value    Value to multiply the vector by.
     * @param vector Second vector to add.
     * @returns The result of the operation.
     */
    XS_INLINE Vector3D4 mad(const SIMD4Def& value, const Vector3D4& vector) const noexcept
    {
        return Vector3D4(this->vectors.mad(value, vector.vectors));
    }

    /**
     * Compute the square of the length of a 3-D vector4.
     * @returns The result of the operation.
     */
    XS_INLINE SIMD4Def lengthSqr() const noexcept
    {
        return this->vectors.lengthSqr3();
    }

    /**
     * Compute the length of a 3-D vector4.
     * @returns The result of the operation.
     */
    XS_INLINE SIMD4Def length() const noexcept
    {
        return this->vectors.length3();
    }

    /**
     * Normalise a vector4.
     * @note The result is unpredictable when all elements of vector are at or near zero.
     * @returns A new vector corresponding to the normalised vector.
     */
    XS_INLINE Vector3D4 normalize() const noexcept
    {
        return Vector3D4(this->vectors.normalize3());
    }

    /**
     * Compute the dot product of two 3-D vector4s.
     * @param vector The second vector3d4.
     * @returns SIMD4Def containing the dot product of each stored vector.
     */
    XS_INLINE SIMD4Def dot3(const Vector3D4& vector) const noexcept
    {
        return this->vectors.dot3(vector.vectors);
    }

    /**
     * Compute cross product of two 3-D vector4s.
     * @note Uses Right-handed coordinate system.
     * @param vector The second vector3d4.
     * @returns Vector3D4 containing the cross product of each stored vector.
     */
    XS_INLINE Vector3D4 cross3(const Vector3D4& vector) const noexcept
    {
        return Vector3D4(this->vectors.cross3(vector.vectors));
    }
};

/**
 * Add a vector to each component vector in a vector4.
 * @param vector1 The vector3d4.
 * @param vector2 Vector3d to add to the vector3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width> operator+(const Vector3D4<T, Packed, Width>& vector1,
    const typename Vector3D4<T, Packed, Width>::Vector3DDef& vector2) noexcept
{
    return Vector3D4<T, Packed, Width>(vector1.vectors + vector2.values);
}

/**
 * Add a vector2 to each component vector2 in a vector4.
 * @param vector1 The vector3d4.
 * @param vector2 Vector3d2 to add to the vector3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width> operator+(const Vector3D4<T, Packed, Width>& vector1,
    const typename Vector3D4<T, Packed, Width>::Vector3D2Def& vector2) noexcept
{
    return Vector3D4<T, Packed, Width>(vector1.vectors + vector2.vectors);
}

/**
 * Add two 3-D vector4s.
 * @param vector1 The first vector3d4.
 * @param vector2 Vector3d4 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width> operator+(
    const Vector3D4<T, Packed, Width>& vector1, const Vector3D4<T, Packed, Width>& vector2) noexcept
{
    return Vector3D4<T, Packed, Width>(vector1.vectors + vector2.vectors);
}

/**
 * Subtract a vector from each component vector in a vector4.
 * @param vector1 The vector3d4.
 * @param vector2 Vector3d to add to the vector3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width> operator-(const Vector3D4<T, Packed, Width>& vector1,
    const typename Vector3D4<T, Packed, Width>::Vector3DDef& vector2) noexcept
{
    return Vector3D4<T, Packed, Width>(vector1.vectors - vector2.values);
}

/**
 * Subtract a vector2 from each component vector2 in a vector4.
 * @param vector1 The vector3d4.
 * @param vector2 Vector3d2 to add to the vector3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width> operator-(const Vector3D4<T, Packed, Width>& vector1,
    const typename Vector3D4<T, Packed, Width>::Vector3D2Def& vector2) noexcept
{
    return Vector3D4<T, Packed, Width>(vector1.vectors - vector2.vectors);
}

/**
 * Subtract a 3-D vector4 from another 3-D vector4.
 * @param vector1 The first vector3d4.
 * @param vector2 Vector3d4 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width> operator-(
    const Vector3D4<T, Packed, Width>& vector1, const Vector3D4<T, Packed, Width>& vector2) noexcept
{
    return Vector3D4<T, Packed, Width>(vector1.vectors - vector2.vectors);
}

/**
 * Multiply a 3-D vector4 by a scalar.
 * @param vector The vector3d4.
 * @param value  Value to multiply the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width> operator*(
    const Vector3D4<T, Packed, Width>& vector, typename Vector3D4<T, Packed, Width>::BaseDef value) noexcept
{
    return Vector3D4<T, Packed, Width>(vector.vectors * value);
}

/**
 * Multiply a 3-D vector4 by a scalar.
 * @param vector The vector3d4.
 * @param value  Value to multiply the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width> operator*(
    const Vector3D4<T, Packed, Width>& vector, const typename Vector3D4<T, Packed, Width>::SIMD4Def& value) noexcept
{
    return Vector3D4<T, Packed, Width>(vector.vectors * value);
}

/**
 * Multiply a 3-D vector4 by a vector3d.
 * @param vector1 The vector3d4.
 * @param vector2 Value to multiply the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width> operator*(const Vector3D4<T, Packed, Width>& vector1,
    const typename Vector3D4<T, Packed, Width>::Vector3DDef& vector2) noexcept
{
    return Vector3D4<T, Packed, Width>(vector1.vectors * vector2.values);
}

/**
 * Multiply a vector2 by each component vector2 in a vector4.
 * @param vector1 The vector3d4.
 * @param vector2 Vector3d2 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width> operator*(const Vector3D4<T, Packed, Width>& vector1,
    const typename Vector3D4<T, Packed, Width>::Vector3D2Def& vector2) noexcept
{
    return Vector3D4<T, Packed, Width>(vector1.vectors * vector2.vectors);
}

/**
 * Multiply a 3-D vector4 by another vector.
 * @param vector1 The vector3d4.
 * @param vector2 Value to multiply the fist vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width> operator*(
    const Vector3D4<T, Packed, Width>& vector1, const Vector3D4<T, Packed, Width>& vector2) noexcept
{
    return Vector3D4<T, Packed, Width>(vector1.vectors * vector2.vectors);
}

/**
 * Divide a 3-D vector4 by a scalar.
 * @param vector The vector3d4.
 * @param value  Value to divide the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width> operator/(
    const Vector3D4<T, Packed, Width>& vector, typename Vector3D4<T, Packed, Width>::BaseDef value) noexcept
{
    return Vector3D4<T, Packed, Width>(vector.vectors / value);
}

/**
 * Divide a 3-D vector4 by a scalar.
 * @param vector The vector3d4.
 * @param value  Value to divide the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width> operator/(
    const Vector3D4<T, Packed, Width>& vector, const typename Vector3D4<T, Packed, Width>::SIMD4Def& value) noexcept
{
    return Vector3D4<T, Packed, Width>(vector.vectors / value);
}

/**
 * Negate all elements of a 3-D vector4.
 * @param vector The vector3d4 to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width> operator-(const Vector3D4<T, Packed, Width>& vector) noexcept
{
    return Vector3D4<T, Packed, Width>(-vector.vectors);
}

/**
 * Perform compound assignment and addition with a vector and a vector4.
 * @param [in,out] vector1 The vector3d4.
 * @param          vector2 Vector3d to add to the vector3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width>& operator+=(
    Vector3D4<T, Packed, Width>& vector1, const typename Vector3D4<T, Packed, Width>::Vector3DDef& vector2) noexcept
{
    vector1.vectors += vector2.values;
    return vector1;
}

/**
 * Perform compound assignment and addition with a vector2 and a vector4.
 * @param [in,out] vector1 The vector3d4.
 * @param          vector2 Vector3d2 to add to the vector3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width>& operator+=(
    Vector3D4<T, Packed, Width>& vector1, const typename Vector3D4<T, Packed, Width>::Vector3D2Def& vector2) noexcept
{
    vector1.vectors += vector2.vectors;
    return vector1;
}

/**
 * Perform compound assignment and addition with a 3-D vector4.
 * @param [in,out] vector1 The first vector3d4.
 * @param          vector2 Vector3D4 to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width>& operator+=(
    Vector3D4<T, Packed, Width>& vector1, const Vector3D4<T, Packed, Width>& vector2) noexcept
{
    vector1.vectors += vector2.vectors;
    return vector1;
}

/**
 * Perform compound assignment and subtraction with a vector and a vector4.
 * @param [in,out] vector1 The vector3d4.
 * @param          vector2 Vector3d to add to the vector3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width>& operator-=(
    Vector3D4<T, Packed, Width>& vector1, const typename Vector3D4<T, Packed, Width>::Vector3DDef& vector2) noexcept
{
    vector1.vectors -= vector2.values;
    return vector1;
}

/**
 * Perform compound assignment and subtraction with a vector2 and a vector4.
 * @param [in,out] vector1 The vector3d4.
 * @param          vector2 Vector3d2 to add to the vector3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width>& operator-=(
    Vector3D4<T, Packed, Width>& vector1, const typename Vector3D4<T, Packed, Width>::Vector3D2Def& vector2) noexcept
{
    vector1.vectors -= vector2.vectors;
    return vector1;
}

/**
 * Perform compound assignment and subtraction by a 3-D vector4.
 * @param [in,out] vector1 The first vector3d4.
 * @param          vector2 Vector3D4 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width>& operator-=(
    Vector3D4<T, Packed, Width>& vector1, const Vector3D4<T, Packed, Width>& vector2) noexcept
{
    vector1.vectors -= vector2.vectors;
    return vector1;
}

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] vector The vector3d4.
 * @param          value  Value to multiply the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width>& operator*=(
    Vector3D4<T, Packed, Width>& vector, typename Vector3D4<T, Packed, Width>::BaseDef value) noexcept
{
    vector.vectors *= value;
    return vector;
}

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] vector The vector3d4.
 * @param          value  Value to multiply the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width>& operator*=(
    Vector3D4<T, Packed, Width>& vector, const typename Vector3D4<T, Packed, Width>::SIMD4Def& value) noexcept
{
    vector.vectors *= value;
    return vector;
}

/**
 * Perform compound assignment and multiplication with a vector2 and a vector4.
 * @param [in,out] vector1 The vector3d4.
 * @param          vector2 Vector3d to multiply the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width>& operator*=(
    Vector3D4<T, Packed, Width>& vector1, const typename Vector3D4<T, Packed, Width>::Vector3DDef& vector2) noexcept
{
    vector1.vectors *= vector2.values;
    return vector1;
}

/**
 * Perform compound assignment and multiplication with a vector2 and a vector4.
 * @param [in,out] vector1 The vector3d4.
 * @param          vector2 Vector3d2 to multiply the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width>& operator*=(
    Vector3D4<T, Packed, Width>& vector1, const typename Vector3D4<T, Packed, Width>::Vector3D2Def& vector2) noexcept
{
    vector1.vectors *= vector2.vectors;
    return vector1;
}

/**
 * Perform compound assignment and multiplication by another vector.
 * @param [in,out] vector1 The vector3d4.
 * @param          vector2 Value to multiply the fist vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width>& operator*=(
    Vector3D4<T, Packed, Width>& vector1, const Vector3D4<T, Packed, Width>& vector2) noexcept
{
    vector1.vectors *= vector2.vectors;
    return vector1;
}

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] vector The vector3d4.
 * @param          value  Value to divide the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width>& operator/=(
    Vector3D4<T, Packed, Width>& vector, typename Vector3D4<T, Packed, Width>::BaseDef value) noexcept
{
    vector.vectors /= value;
    return vector;
}

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] vector The vector3d4.
 * @param          value  Value to divide the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Vector3D4<T, Packed, Width>& operator/=(
    Vector3D4<T, Packed, Width>& vector, const typename Vector3D4<T, Packed, Width>::SIMD4Def& value) noexcept
{
    vector.vectors /= value;
    return vector;
}
} // namespace Shift
