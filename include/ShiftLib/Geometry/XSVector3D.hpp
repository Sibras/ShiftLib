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

#include "SIMD/XSSIMD3.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class Vector3D;

template<typename T>
class Vector3DData
{
public:
    SIMD3Data<T> values;

    /** Default constructor. */
    XS_INLINE Vector3DData() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Vector3DData(const Vector3DData& other) = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE Vector3DData(Vector3DData&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Vector3DData& operator=(const Vector3DData& other) = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Vector3DData& operator=(Vector3DData&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit Vector3DData(const Vector3D<T, Width>& other) noexcept
        : values(other.values)
    {}

    /**
     * Construct from member variables.
     * @param value0In The first pre-calculated value.
     * @param value1In The second pre-calculated value.
     * @param value2In The third pre-calculated value.
     */
    XS_INLINE Vector3DData(T&& value0In, T&& value1In, T&& value2In)
        : values(forward<T>(value0In), forward<T>(value1In), forward<T>(value2In))
    {}

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param x Points pre-calculated x value.
     * @param y Points pre-calculated y value.
     * @param z Points pre-calculated z value.
     */
    XS_INLINE void setData(T x, T y, T z) noexcept
    {
        this->values.setData(x, y, z);
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const Vector3D<T, Width>& other) noexcept
    {
        this->values.store(other.values);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD128<T>>
    XS_INLINE Vector3D<T, Width> load() const noexcept
    {
        return Vector3D<T, Width>(this->values.template load<Vector3D<T, Width>::widthImpl>());
    }
};

template<typename T, typename T2 = T>
class Vector3DDataPad
{
public:
    SIMD3DataPad<T, T2> values;

    /** Default constructor. */
    XS_INLINE Vector3DDataPad() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Vector3DDataPad(const Vector3DDataPad& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE Vector3DDataPad(Vector3DDataPad&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Vector3DDataPad& operator=(const Vector3DDataPad& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Vector3DDataPad& operator=(Vector3DDataPad&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit Vector3DDataPad(const Vector3D<T, Width>& other) noexcept
        : values(other.values)
    {}

    /**
     * Construct from member variables.
     * @param value0In The first pre-calculated value.
     * @param value1In The second pre-calculated value.
     * @param value2In The third pre-calculated value.
     */
    XS_INLINE Vector3DDataPad(T&& value0In, T&& value1In, T&& value2In)
        : values(forward<T>(value0In), forward<T>(value1In), forward<T>(value2In))
    {}

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param x Points pre-calculated x value.
     * @param y Points pre-calculated y value.
     * @param z Points pre-calculated z value.
     */
    XS_INLINE void setData(T x, T y, T z) noexcept
    {
        this->values.setData(x, y, z);
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const Vector3D<T, Width>& other) noexcept
    {
        this->values.store(other.values);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD128<T>>
    XS_INLINE Vector3D<T, Width> load() const noexcept
    {
        return Vector3D<T, Width>(this->values.template load<Vector3D<T, Width>::widthImpl>());
    }
};

/**
 * A base type used to store a 3-D vector.
 * @note Cross product operations on 3-D vectors contained in this library are
 *  performed in a right-hand coordinate system.
 * @tparam T     Generic type parameter.
 * @tparam Width Type of SIMD being used.
 */
template<typename T, SIMDWidth Width = defaultWidthSIMD128<T>>
class Vector3D
{
public:
    using Type = T;
    using SIMD3Def = SIMD3<T, SIMD3<T, Width>::widthImpl>;
    using Data = Vector3DData<T>;
    template<typename T2 = Type>
    using DataPad = Vector3DDataPad<T, T2>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = SIMD3Def::widthImpl;
    using BaseDef = typename SIMD3Def::BaseDef;
    using InBaseDef = typename SIMD3Def::InBaseDef;

    SIMD3Def values;

    /** Default constructor. */
    XS_INLINE Vector3D() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Vector3D(const Vector3D& other) = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE Vector3D(Vector3D&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Vector3D& operator=(const Vector3D& other) = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Vector3D& operator=(Vector3D&& other) noexcept = default;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_INLINE explicit Vector3D(const Vector3D<T, Width2>& other) noexcept
        : values(other.values)
    {}

    /**
     * Construct a 3-D vector from x, y, and z elements.
     * @param x The x value.
     * @param y The y value.
     * @param z The z value.
     */
    XS_INLINE Vector3D(T x, T y, T z) noexcept
        : values(x, y, z)
    {}

    /**
     * Constructor to set all elements to the same scalar value.
     * @param value Value to set all vector elements to.
     */
    XS_INLINE explicit Vector3D(T value) noexcept
        : values(value)
    {}

    /**
     * Construct a 3-D vector from a SIMD3.
     * @param values The SIMD3.
     */
    XS_INLINE explicit Vector3D(const SIMD3Def& values) noexcept
        : values(values)
    {}

    /**
     * Constructor to build a 3-D vector with all elements set to 0.
     * @returns Newly constructed Vector3D with required attributes.
     */
    XS_INLINE static Vector3D Zero() noexcept
    {
        return Vector3D(SIMD3Def::Zero());
    }

    /**
     * Constructor to build a 3-D vector with all elements set to 1.
     * @returns Newly constructed Vector3D with required attributes.
     */
    XS_INLINE static Vector3D One() noexcept
    {
        return Vector3D(SIMD3Def::One());
    }

    /**
     * Get an element of a 3-D vector.
     * @note Optimized for getting value from register.
     * @tparam Index The index of the element to retrieve (must be between 0 and 2).
     * @returns InBaseDef containing the desired value.
     */
    template<uint32_t Index>
    XS_INLINE InBaseDef getValueInBase() const noexcept
    {
        return this->values.template getValueInBase<Index>();
    }

    /**
     * Get an element of a 3-D vector.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (must be between 0 and 2).
     * @returns SIMDBase containing the desired value.
     */
    template<uint32_t Index>
    XS_INLINE BaseDef getValue() const noexcept
    {
        return this->values.template getValue<Index>();
    }

    /**
     * Dynamically get an element of the object.
     * @note Optimised for getting value from register.
     * @param index The index of the element to retrieve (range is 0-2).
     * @returns SIMDInBase containing the desired value.
     */
    XS_INLINE InBaseDef getValueInBase(const uint32 index) const noexcept
    {
        return this->values.getValueInBase(index);
    }

    /**
     * Dynamically get an element of the object.
     * @note Optimised for getting value from register.
     * @param index The index of the element to retrieve (range is 0-2).
     * @returns BaseDef containing the desired value.
     */
    XS_INLINE BaseDef getValue(const uint32 index) const noexcept
    {
        return this->values.getValue(index);
    }

    /**
     * Set an element of a 3-D vector.
     * @tparam Index The index of the element to set (must be between 0 and 2).
     * @param value The new value.
     */
    template<uint32_t Index>
    XS_INLINE void setValue(InBaseDef value) noexcept
    {
        this->values.template setValue<Index>(value);
    }

    /**
     * Set an element of a 3-D vector.
     * @tparam Index The index of the element to set (must be between 0 and 2).
     * @param value The new value.
     */
    template<uint32_t Index>
    XS_INLINE void setValue(BaseDef value) noexcept
    {
        this->values.template setValue<Index>(value);
    }

    /**
     * Multiply a 3-D vector by a scalar and then add another vector.
     * @param value   Value to multiply the vector by.
     * @param vector Second vector to add.
     * @returns The result of the operation.
     */
    XS_INLINE Vector3D mad(BaseDef value, const Vector3D& vector) const noexcept
    {
        return Vector3D(this->values.mad(value, vector.values));
    }

    /**
     * Compute the dot product of two 3-D vectors.
     * @param vector The second vector.
     * @returns The value of the dot product stored in a floatBase.
     */
    XS_INLINE BaseDef dot3(const Vector3D& vector) const noexcept
    {
        return this->values.dot3(vector.values);
    }

    /**
     * Compute the dot product of two 3-D vectors.
     * @param vector The second vector.
     * @returns The value of the dot product stored in a floatInBase.
     */
    XS_INLINE InBaseDef dot3InBase(const Vector3D& vector) const noexcept
    {
        return this->values.dot3InBase(vector.values);
    }

    /**
     * Compute cross product of two 3-D vectors.
     * @note Uses Right-handed coordinate system.
     * @param vector The second vector.
     * @returns A new vector corresponding to the cross product of the inputs.
     */
    XS_INLINE Vector3D cross3(const Vector3D& vector) const noexcept
    {
        return Vector3D(this->values.cross3(vector.values));
    }

    /**
     * Compute component product of two 3-D vectors.
     * @param vector The second vector.
     * @returns A new vector corresponding to the component product of the inputs.
     */
    XS_INLINE Vector3D component3(const Vector3D& vector) const noexcept
    {
        return Vector3D(this->values * vector.values);
    }

    /**
     * Compute the square of the length of a 3-D vector.
     * @returns Squared length of vector stored in BaseDef.
     */
    XS_INLINE BaseDef lengthSqr() const noexcept
    {
        return this->values.lengthSqr();
    }

    /**
     * Compute the length of a 3-D vector.
     * @returns Length of vector stored in BaseDef.
     */
    XS_INLINE BaseDef length() const noexcept
    {
        return this->values.length();
    }

    /**
     * Compute the square of the length of a 3-D vector.
     * @returns Squared length of vector stored in InBaseDef.
     */
    XS_INLINE InBaseDef lengthSqrInBase() const noexcept
    {
        return this->values.lengthSqrInBase();
    }

    /**
     * Compute the length of a 3-D vector.
     * @returns Length of vector stored in InBaseDef.
     */
    XS_INLINE InBaseDef lengthInBase() const noexcept
    {
        return this->values.lengthInBase();
    }

    /**
     * Normalise a vector.
     * @note The result is unpredictable when all elements of vector are at or near zero.
     * @returns A new vector corresponding to the normalised vector.
     */
    XS_INLINE Vector3D normalize() const noexcept
    {
        return Vector3D(this->values.normalize());
    }
};

/**
 * Add two 3-D vectors.
 * @param vector1 The first vector.
 * @param vector2 Vector to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width> operator+(const Vector3D<T, Width>& vector1, const Vector3D<T, Width>& vector2) noexcept
{
    return Vector3D<T, Width>(vector1.values + vector2.values);
}

/**
 * Subtract a 3-D vector from another 3-D vector.
 * @param vector1 The first vector.
 * @param vector2 Vector to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width> operator-(const Vector3D<T, Width>& vector1, const Vector3D<T, Width>& vector2) noexcept
{
    return Vector3D<T, Width>(vector1.values - vector2.values);
}

/**
 * Multiply a 3-D vector by a scalar.
 * @param vector The vector.
 * @param value  Value to multiply the vector by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width> operator*(
    const Vector3D<T, Width>& vector, typename Vector3D<T, Width>::BaseDef value) noexcept
{
    return Vector3D<T, Width>(vector.values * value);
}

/**
 * Divide a 3-D vector by a scalar.
 * @param vector The vector.
 * @param value  Value to divide the vector by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width> operator/(
    const Vector3D<T, Width>& vector, typename Vector3D<T, Width>::BaseDef value) noexcept
{
    return Vector3D<T, Width>(vector.values / value);
}

/**
 * Negate all elements of a 3-D vector.
 * @param vector The vector to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width> operator-(const Vector3D<T, Width>& vector) noexcept
{
    return Vector3D<T, Width>(-vector.values);
}

/**
 * Perform compound assignment and addition with a 3-D vector.
 * @param [in,out] vector1 The first vector.
 * @param          vector2 Vector3D to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width>& operator+=(Vector3D<T, Width>& vector1, const Vector3D<T, Width>& vector2) noexcept
{
    vector1.values += vector2.values;
    return vector1;
}

/**
 * Perform compound assignment and subtraction by a 3-D vector.
 * @param [in,out] vector1 The first vector.
 * @param          vector2 Vector3D to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width>& operator-=(Vector3D<T, Width>& vector1, const Vector3D<T, Width>& vector2) noexcept
{
    vector1.values -= vector2.values;
    return vector1;
}

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] vector The vector.
 * @param          value  Value to multiply the vector by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width>& operator*=(
    Vector3D<T, Width>& vector, typename Vector3D<T, Width>::BaseDef value) noexcept
{
    vector.values *= value;
    return vector;
}

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] vector The vector.
 * @param          value  Value to divide the vector by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Vector3D<T, Width>& operator/=(
    Vector3D<T, Width>& vector, typename Vector3D<T, Width>::BaseDef value) noexcept
{
    vector.values /= value;
    return vector;
}
} // namespace Shift
