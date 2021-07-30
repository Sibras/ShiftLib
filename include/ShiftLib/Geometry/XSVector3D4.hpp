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

#include "SIMD/XSSIMD12.hpp"
#include "SIMD/XSSIMD3x4.hpp"

// Additional includes
#include "Geometry/XSVector3D.hpp"
#include "Geometry/XSVector3D2.hpp"

namespace Shift {
template<typename T, SIMDWidth Width, bool Packed>
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
    XS_FUNCTION Vector3D4Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit Vector3D4Data(const Vector3D4<T, Width, Packed>& other) noexcept;

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
    XS_FUNCTION void setData(
        T v0X, T v1X, T v2X, T v3X, T v0Y, T v1Y, T v2Y, T v3Y, T v0Z, T v1Z, T v2Z, T v3Z) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const Vector3D4<T, Width, Packed>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Vector3D4<T, Width, Packed> load() const noexcept;
};

template<typename T, bool Packed = false>
class Vector3D4DataPad
{
    template<bool IsPacked>
    struct InternData
    {
        using Vector3D4InternData = SIMD12DataPad<T>;
    };

    template<>
    struct InternData<false>
    {
        using Vector3D4InternData = SIMD3x4DataPad<T>;
    };

public:
    typename InternData<Packed>::Vector3D4InternData values;

    /** Default constructor. */
    XS_FUNCTION Vector3D4DataPad() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit Vector3D4DataPad(const Vector3D4<T, Width, Packed>& other) noexcept;

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
    XS_FUNCTION void setData(
        T v0X, T v1X, T v2X, T v3X, T v0Y, T v1Y, T v2Y, T v3Y, T v0Z, T v1Z, T v2Z, T v3Z) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const Vector3D4<T, Width, Packed>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Vector3D4<T, Width, Packed> load() const noexcept;
};

/**
 * A base type used to store 4 3-D vectors.
 * @note Cross product operations on 3-D vectors contained in this library are
 *   performed in a right-hand coordinate system.
 * @tparam T      Generic type parameter.
 * @tparam Width  Type of SIMD being used.
 * @tparam Packed Type of storage used for internal data.
 */
template<typename T, SIMDWidth Width = widthSIMD<T>, bool Packed = false>
class Vector3D4
{
    template<bool IsPacked>
    struct InternData
    {
        using Vector3D4Intern = SIMD12<T, SIMD12<T, Width>::widthImpl>;
        using SIMD12Def = Vector3D4Intern;
        using SIMD3x4Def = typename Vector3D4Intern::SIMD3x4Def;
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
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Vector3D4Def::widthImpl;
    static constexpr bool packed = Packed;
    using BaseDef = typename Vector3D4Def::BaseDef;
    using InBaseDef = typename Vector3D4Def::InBaseDef;
    using SIMD12Def = typename InternData<Packed>::SIMD12Def;
    using SIMD3x4Def = typename InternData<Packed>::SIMD3x4Def;
    using Vector3DDef = Vector3D<T, Vector3D<T, widthImpl>::widthImpl>;
    using Vector3D2Def = Vector3D2<T, Vector3D2<T, widthImpl, Packed>::widthImpl, Packed>;
    using SIMD4Def = typename Vector3D4Def::SIMD4Def;

    Vector3D4Def vectors;

    /** Default constructor. */
    XS_FUNCTION Vector3D4() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION Vector3D4(const Vector3D4& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Vector3D4(Vector3D4&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Vector3D4& operator=(const Vector3D4& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Vector3D4& operator=(Vector3D4&& other) noexcept = default;

    /**
     * Construct a vector3d4 from 2 vector2s.
     * @param vector0 The first vector2.
     * @param vector1 The second vector2.
     */
    XS_FUNCTION Vector3D4(const Vector3D2Def& vector0, const Vector3D2Def& vector1) noexcept;

    /**
     * Construct a vector3d4 from 4 vectors.
     * @param vector0 The first vector.
     * @param vector1 The second vector.
     * @param vector2 The third vector.
     * @param vector3 The fourth vector.
     */
    XS_FUNCTION Vector3D4(const Vector3DDef& vector0, const Vector3DDef& vector1, const Vector3DDef& vector2,
        const Vector3DDef& vector3) noexcept;

    /**
     * Construct a vector3d4 from a single vector.
     * @param vector The vector.
     */
    XS_FUNCTION explicit Vector3D4(const Vector3DDef& vector) noexcept;

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
    XS_FUNCTION Vector3D4(T v0X, T v1X, T v2X, T v3X, T v0Y, T v1Y, T v2Y, T v3Y, T v0Z, T v1Z, T v2Z, T v3Z) noexcept;

    /**
     * Construct a vector3d4 from a SIMD12.
     * @param values The SIMD12.
     */
    XS_FUNCTION explicit Vector3D4(const SIMD12Def& values) noexcept;

    /**
     * Construct a vector3d4 from a SIMD3x4.
     * @param values The SIMD3x4.
     */
    XS_FUNCTION explicit Vector3D4(const SIMD3x4Def& values) noexcept;

    /**
     * Get a contained 3-D vector.
     * @tparam Index The index of the vector to retrieve (must be between 0 and 3).
     * @returns Vector3D containing the desired value.
     */
    template<uint32_t Index>
    XS_FUNCTION Vector3DDef getVector() const;

    /**
     * Set the first 3-D vector.
     * @tparam Index The index of the vector to set (must be between 0 and 3).
     * @param vector The new vector.
     */
    template<uint32_t Index>
    XS_FUNCTION void setVector(const Vector3DDef& vector) noexcept;

    /**
     * Multiply a 3-D vector4 by a scalar and then add another vector3d4.
     * @param value    Value to multiply the vector by.
     * @param vector Second vector to add.
     * @returns The result of the operation.
     */
    XS_FUNCTION Vector3D4 mad(const SIMD4Def& value, const Vector3D4& vector) const;

    /**
     * Compute the square of the length of a 3-D vector4.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD4Def lengthSqr() const;

    /**
     * Compute the length of a 3-D vector4.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD4Def length() const;

    /**
     * Normalise a vector4.
     * @note The result is unpredictable when all elements of vector are at or near zero.
     * @returns A new vector corresponding to the normalised vector.
     */
    XS_FUNCTION Vector3D4 normalize() const;

    /**
     * Compute the dot product of two 3-D vector4s.
     * @param vector The second vector3d4.
     * @returns SIMD4Def containing the dot product of each stored vector.
     */
    XS_FUNCTION SIMD4Def dot3(const Vector3D4& vector) const;

    /**
     * Compute cross product of two 3-D vector4s.
     * @note Uses Right-handed coordinate system.
     * @param vector The second vector3d4.
     * @returns Vector3D4 containing the cross product of each stored vector.
     */
    XS_FUNCTION Vector3D4 cross3(const Vector3D4& vector) const;
};

/**
 * Add a vector to each component vector in a vector4.
 * @param vector1 The vector3d4.
 * @param vector2 Vector3d to add to the vector3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed> operator+(const Vector3D4<T, Width, Packed>& vector1,
    const typename Vector3D4<T, Width, Packed>::Vector3DDef& vector2) noexcept;

/**
 * Add a vector2 to each component vector2 in a vector4.
 * @param vector1 The vector3d4.
 * @param vector2 Vector3d2 to add to the vector3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed> operator+(const Vector3D4<T, Width, Packed>& vector1,
    const typename Vector3D4<T, Width, Packed>::Vector3D2Def& vector2) noexcept;

/**
 * Add two 3-D vector4s.
 * @param vector1 The first vector3d4.
 * @param vector2 Vector3d4 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed> operator+(
    const Vector3D4<T, Width, Packed>& vector1, const Vector3D4<T, Width, Packed>& vector2) noexcept;

/**
 * Subtract a vector from each component vector in a vector4.
 * @param vector1 The vector3d4.
 * @param vector2 Vector3d to add to the vector3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed> operator-(const Vector3D4<T, Width, Packed>& vector1,
    const typename Vector3D4<T, Width, Packed>::Vector3DDef& vector2) noexcept;

/**
 * Subtract a vector2 from each component vector2 in a vector4.
 * @param vector1 The vector3d4.
 * @param vector2 Vector3d2 to add to the vector3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed> operator-(const Vector3D4<T, Width, Packed>& vector1,
    const typename Vector3D4<T, Width, Packed>::Vector3D2Def& vector2) noexcept;

/**
 * Subtract a 3-D vector4 from another 3-D vector4.
 * @param vector1 The first vector3d4.
 * @param vector2 Vector3d4 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed> operator-(
    const Vector3D4<T, Width, Packed>& vector1, const Vector3D4<T, Width, Packed>& vector2) noexcept;

/**
 * Multiply a 3-D vector4 by a scalar.
 * @param vector The vector3d4.
 * @param value  Value to multiply the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed> operator*(
    const Vector3D4<T, Width, Packed>& vector, typename Vector3D4<T, Width, Packed>::BaseDef value) noexcept;

/**
 * Multiply a 3-D vector4 by a scalar.
 * @param vector The vector3d4.
 * @param value  Value to multiply the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed> operator*(
    const Vector3D4<T, Width, Packed>& vector, const typename Vector3D4<T, Width, Packed>::SIMD4Def& value) noexcept;

/**
 * Multiply a 3-D vector4 by a vector3d.
 * @param vector1 The vector3d4.
 * @param vector2 Value to multiply the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed> operator*(const Vector3D4<T, Width, Packed>& vector1,
    const typename Vector3D4<T, Width, Packed>::Vector3DDef& vector2) noexcept;

/**
 * Multiply a vector2 by each component vector2 in a vector4.
 * @param vector1 The vector3d4.
 * @param vector2 Vector3d2 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed> operator*(const Vector3D4<T, Width, Packed>& vector1,
    const typename Vector3D4<T, Width, Packed>::Vector3D2Def& vector2) noexcept;

/**
 * Multiply a 3-D vector4 by another vector.
 * @param vector1 The vector3d4.
 * @param vector2 Value to multiply the fist vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed> operator*(
    const Vector3D4<T, Width, Packed>& vector1, const Vector3D4<T, Width, Packed>& vector2) noexcept;

/**
 * Divide a 3-D vector4 by a scalar.
 * @param vector The vector3d4.
 * @param value  Value to divide the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed> operator/(
    const Vector3D4<T, Width, Packed>& vector, typename Vector3D4<T, Width, Packed>::BaseDef value) noexcept;

/**
 * Divide a 3-D vector4 by a scalar.
 * @param vector The vector3d4.
 * @param value  Value to divide the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed> operator/(
    const Vector3D4<T, Width, Packed>& vector, const typename Vector3D4<T, Width, Packed>::SIMD4Def& value) noexcept;

/**
 * Negate all elements of a 3-D vector4.
 * @param vector The vector3d4 to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed> operator-(const Vector3D4<T, Width, Packed>& vector) noexcept;

/**
 * Perform compound assignment and addition with a vector and a vector4.
 * @param [in,out] vector1 The vector3d4.
 * @param          vector2 Vector3d to add to the vector3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed>& operator+=(
    Vector3D4<T, Width, Packed>& vector1, const typename Vector3D4<T, Width, Packed>::Vector3DDef& vector2) noexcept;

/**
 * Perform compound assignment and addition with a vector2 and a vector4.
 * @param [in,out] vector1 The vector3d4.
 * @param          vector2 Vector3d2 to add to the vector3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed>& operator+=(
    Vector3D4<T, Width, Packed>& vector1, const typename Vector3D4<T, Width, Packed>::Vector3D2Def& vector2) noexcept;

/**
 * Perform compound assignment and addition with a 3-D vector4.
 * @param [in,out] vector1 The first vector3d4.
 * @param          vector2 Vector3D4 to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed>& operator+=(
    Vector3D4<T, Width, Packed>& vector1, const Vector3D4<T, Width, Packed>& vector2) noexcept;

/**
 * Perform compound assignment and subtraction with a vector and a vector4.
 * @param [in,out] vector1 The vector3d4.
 * @param          vector2 Vector3d to add to the vector3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed>& operator-=(
    Vector3D4<T, Width, Packed>& vector1, const typename Vector3D4<T, Width, Packed>::Vector3DDef& vector2) noexcept;

/**
 * Perform compound assignment and subtraction with a vector2 and a vector4.
 * @param [in,out] vector1 The vector3d4.
 * @param          vector2 Vector3d2 to add to the vector3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed>& operator-=(
    Vector3D4<T, Width, Packed>& vector1, const typename Vector3D4<T, Width, Packed>::Vector3D2Def& vector2) noexcept;

/**
 * Perform compound assignment and subtraction by a 3-D vector4.
 * @param [in,out] vector1 The first vector3d4.
 * @param          vector2 Vector3D4 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed>& operator-=(
    Vector3D4<T, Width, Packed>& vector1, const Vector3D4<T, Width, Packed>& vector2) noexcept;

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] vector The vector3d4.
 * @param          value  Value to multiply the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed>& operator*=(
    Vector3D4<T, Width, Packed>& vector, typename Vector3D4<T, Width, Packed>::BaseDef value) noexcept;

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] vector The vector3d4.
 * @param          value  Value to multiply the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed>& operator*=(
    Vector3D4<T, Width, Packed>& vector, const typename Vector3D4<T, Width, Packed>::SIMD4Def& value) noexcept;

/**
 * Perform compound assignment and multiplication with a vector2 and a vector4.
 * @param [in,out] vector1 The vector3d4.
 * @param          vector2 Vector3d to multiply the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed>& operator*=(
    Vector3D4<T, Width, Packed>& vector1, const typename Vector3D4<T, Width, Packed>::Vector3DDef& vector2) noexcept;

/**
 * Perform compound assignment and multiplication with a vector2 and a vector4.
 * @param [in,out] vector1 The vector3d4.
 * @param          vector2 Vector3d2 to multiply the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed>& operator*=(
    Vector3D4<T, Width, Packed>& vector1, const typename Vector3D4<T, Width, Packed>::Vector3D2Def& vector2) noexcept;

/**
 * Perform compound assignment and multiplication by another vector.
 * @param [in,out] vector1 The vector3d4.
 * @param          vector2 Value to multiply the fist vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed>& operator*=(
    Vector3D4<T, Width, Packed>& vector1, const Vector3D4<T, Width, Packed>& vector2) noexcept;

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] vector The vector3d4.
 * @param          value  Value to divide the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed>& operator/=(
    Vector3D4<T, Width, Packed>& vector, typename Vector3D4<T, Width, Packed>::BaseDef value) noexcept;

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] vector The vector3d4.
 * @param          value  Value to divide the vector3d4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Vector3D4<T, Width, Packed>& operator/=(
    Vector3D4<T, Width, Packed>& vector, const typename Vector3D4<T, Width, Packed>::SIMD4Def& value) noexcept;
} // namespace Shift
