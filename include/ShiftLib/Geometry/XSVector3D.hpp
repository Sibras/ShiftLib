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
    XS_FUNCTION Vector3DData() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit Vector3DData(const Vector3D<T, Width>& other) noexcept;

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param x Points pre-calculated x value.
     * @param y Points pre-calculated y value.
     * @param z Points pre-calculated z value.
     */
    XS_FUNCTION void setData(T x, T y, T z) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const Vector3D<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Vector3D<T, Width> load() const noexcept;
};

template<typename T>
class Vector3DDataPad
{
public:
    SIMD3DataPad<T> values;

    /** Default constructor. */
    XS_FUNCTION Vector3DDataPad() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit Vector3DDataPad(const Vector3D<T, Width>& other) noexcept;

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param x Points pre-calculated x value.
     * @param y Points pre-calculated y value.
     * @param z Points pre-calculated z value.
     */
    XS_FUNCTION void setData(T x, T y, T z) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const Vector3D<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Vector3D<T, Width> load() const noexcept;
};

/**
 * A base type used to store a 3-D vector.
 * @note Cross product operations on 3-D vectors contained in this library are
 *  performed in a right-hand coordinate system.
 * @tparam T     Generic type parameter.
 * @tparam Width Type of SIMD being used.
 */
template<typename T, SIMDWidth Width = widthSIMD<T>>
class Vector3D
{
public:
    using Type = T;
    using SIMD3Def = SIMD3<T, SIMD3<T, Width>::widthImpl>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = SIMD3Def::widthImpl;
    using BaseDef = typename SIMD3Def::BaseDef;
    using InBaseDef = typename SIMD3Def::InBaseDef;

    SIMD3Def values;

    /** Default constructor. */
    XS_FUNCTION Vector3D() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION Vector3D(const Vector3D& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Vector3D(Vector3D&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Vector3D& operator=(const Vector3D& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Vector3D& operator=(Vector3D&& other) noexcept = default;

    /**
     * Construct a 3-D vector from x, y, and z elements.
     * @param x The x value.
     * @param y The y value.
     * @param z The z value.
     */
    XS_FUNCTION Vector3D(T x, T y, T z) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param value Value to set all vector elements to.
     */
    XS_FUNCTION explicit Vector3D(T value) noexcept;

    /**
     * Construct a 3-D vector from a SIMD3.
     * @param values The SIMD3.
     */
    XS_FUNCTION explicit Vector3D(const SIMD3Def& values) noexcept;

    /**
     * Constructor to build a 3-D vector with all elements set to 0.
     * @returns Newly constructed Vector3D with required attributes.
     */
    XS_FUNCTION static Vector3D Zero() noexcept;

    /**
     * Constructor to build a 3-D vector with all elements set to 1.
     * @returns Newly constructed Vector3D with required attributes.
     */
    XS_FUNCTION static Vector3D One() noexcept;

    /**
     * Get an element of a 3-D vector.
     * @note Optimized for getting value from register.
     * @tparam Index The index of the element to retrieve (must be between 0 and 2).
     * @returns InBaseDef containing the desired value.
     */
    template<uint32_t Index>
    XS_FUNCTION InBaseDef getValueInBase() const;

    /**
     * Get an element of a 3-D vector.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (must be between 0 and 2).
     * @returns SIMDBase containing the desired value.
     */
    template<uint32_t Index>
    XS_FUNCTION BaseDef getValue() const;

    /**
     * Set an element of a 3-D vector.
     * @tparam Index The index of the element to set (must be between 0 and 2).
     * @param value The new value.
     */
    template<uint32_t Index>
    XS_FUNCTION void setValue(InBaseDef value) noexcept;

    /**
     * Set an element of a 3-D vector.
     * @tparam Index The index of the element to set (must be between 0 and 2).
     * @param value The new value.
     */
    template<uint32_t Index>
    XS_FUNCTION void setValue(BaseDef value) noexcept;

    /**
     * Multiply a 3-D vector by a scalar and then add another vector.
     * @param value   Value to multiply the vector by.
     * @param vector Second vector to add.
     * @returns The result of the operation.
     */
    XS_FUNCTION Vector3D mad(BaseDef value, const Vector3D& vector) const;

    /**
     * Compute the dot product of two 3-D vectors.
     * @param vector The second vector.
     * @returns The value of the dot product stored in a floatBase.
     */
    XS_FUNCTION BaseDef dot3(const Vector3D& vector) const;

    /**
     * Compute the dot product of two 3-D vectors.
     * @param vector The second vector.
     * @returns The value of the dot product stored in a floatInBase.
     */
    XS_FUNCTION InBaseDef dot3InBase(const Vector3D& vector) const;

    /**
     * Compute cross product of two 3-D vectors.
     * @note Uses Right-handed coordinate system.
     * @param vector The second vector.
     * @returns A new vector corresponding to the cross product of the inputs.
     */
    XS_FUNCTION Vector3D cross3(const Vector3D& vector) const;

    /**
     * Compute component product of two 3-D vectors.
     * @param vector The second vector.
     * @returns A new vector corresponding to the component product of the inputs.
     */
    XS_FUNCTION Vector3D component3(const Vector3D& vector) const;

    /**
     * Compute the square of the length of a 3-D vector.
     * @returns Squared length of vector stored in BaseDef.
     */
    XS_FUNCTION BaseDef lengthSqr() const;

    /**
     * Compute the length of a 3-D vector.
     * @returns Length of vector stored in BaseDef.
     */
    XS_FUNCTION BaseDef length() const;

    /**
     * Compute the square of the length of a 3-D vector.
     * @returns Squared length of vector stored in InBaseDef.
     */
    XS_FUNCTION InBaseDef lengthSqrInBase() const;

    /**
     * Compute the length of a 3-D vector.
     * @returns Length of vector stored in InBaseDef.
     */
    XS_FUNCTION InBaseDef lengthInBase() const;

    /**
     * Normalise a vector.
     * @note The result is unpredictable when all elements of vector are at or near zero.
     * @returns A new vector corresponding to the normalised vector.
     */
    XS_FUNCTION Vector3D normalize() const;
};

/**
 * Add two 3-D vectors.
 * @param vector1 The first vector.
 * @param vector2 Vector to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Vector3D<T, Width> operator+(const Vector3D<T, Width>& vector1, const Vector3D<T, Width>& vector2) noexcept;

/**
 * Subtract a 3-D vector from another 3-D vector.
 * @param vector1 The first vector.
 * @param vector2 Vector to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Vector3D<T, Width> operator-(const Vector3D<T, Width>& vector1, const Vector3D<T, Width>& vector2) noexcept;

/**
 * Multiply a 3-D vector by a scalar.
 * @param vector The vector.
 * @param value  Value to multiply the vector by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Vector3D<T, Width> operator*(
    const Vector3D<T, Width>& vector, typename Vector3D<T, Width>::BaseDef value) noexcept;

/**
 * Divide a 3-D vector by a scalar.
 * @param vector The vector.
 * @param value  Value to divide the vector by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Vector3D<T, Width> operator/(
    const Vector3D<T, Width>& vector, typename Vector3D<T, Width>::BaseDef value) noexcept;

/**
 * Negate all elements of a 3-D vector.
 * @param vector The vector to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Vector3D<T, Width> operator-(const Vector3D<T, Width>& vector) noexcept;

/**
 * Perform compound assignment and addition with a 3-D vector.
 * @param [in,out] vector1 The first vector.
 * @param          vector2 Vector3D to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Vector3D<T, Width>& operator+=(Vector3D<T, Width>& vector1, const Vector3D<T, Width>& vector2) noexcept;

/**
 * Perform compound assignment and subtraction by a 3-D vector.
 * @param [in,out] vector1 The first vector.
 * @param          vector2 Vector3D to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Vector3D<T, Width>& operator-=(Vector3D<T, Width>& vector1, const Vector3D<T, Width>& vector2) noexcept;

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] vector The vector.
 * @param          value  Value to multiply the vector by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Vector3D<T, Width>& operator*=(
    Vector3D<T, Width>& vector, typename Vector3D<T, Width>::BaseDef value) noexcept;

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] vector The vector.
 * @param          value  Value to divide the vector by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Vector3D<T, Width>& operator/=(
    Vector3D<T, Width>& vector, typename Vector3D<T, Width>::BaseDef value) noexcept;
} // namespace Shift
