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

// Additional includes
#include "Geometry/XSVector3D.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class Point3D;

template<typename T>
class Point3DData
{
public:
    SIMD3Data<T> values;

    /** Default constructor. */
    XS_FUNCTION Point3DData() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit Point3DData(const Point3D<T, Width>& other) noexcept;

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
    XS_FUNCTION void store(const Point3D<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Point3D<T, Width> load() const noexcept;
};

template<typename T>
class Point3DDataPad
{
public:
    SIMD3DataPad<T> values;

    /** Default constructor. */
    XS_FUNCTION Point3DDataPad() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit Point3DDataPad(const Point3D<T, Width>& other) noexcept;

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
    XS_FUNCTION void store(const Point3D<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Point3D<T, Width> load() const noexcept;
};

/**
 * A a base type used to store a 3-D point.
 * @tparam T     Generic type parameter.
 * @tparam Width Type of SIMD being used.
 */
template<typename T, SIMDWidth Width = widthSIMD<defaultSIMD>>
class Point3D
{
public:
    using Type = T;
    using SIMD3Def = SIMD3<T, SIMD3<T, Width>::widthImpl>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = SIMD3Def::widthImpl;
    using BaseDef = typename SIMD3Def::BaseDef;
    using InBaseDef = typename SIMD3Def::InBaseDef;
    using Vector3DDef = Vector3D<T, Vector3D<T, widthImpl>::widthImpl>;

    SIMD3Def values;

    /** Default constructor. */
    XS_FUNCTION Point3D() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION Point3D(const Point3D& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Point3D(Point3D&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Point3D& operator=(const Point3D& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Point3D& operator=(Point3D&& other) noexcept = default;

    /**
     * Construct a 3-D point from x, y, and z elements.
     * @param x The x value.
     * @param y The y value.
     * @param z The z value.
     */
    XS_FUNCTION Point3D(T x, T y, T z) noexcept;

    /**
     * Constructor to set all elements to the same scalar value.
     * @param value Value to set all point elements to.
     */
    XS_FUNCTION explicit Point3D(T value) noexcept;

    /**
     * Construct a 3-D point from a SIMD3.
     * @param values The 3 values.
     */
    XS_FUNCTION explicit Point3D(const SIMD3Def& values) noexcept;

    /**
     * Function to build a 3-D point with all elements set to 0.
     * @returns Newly constructed Point3D with required attributes.
     */
    XS_FUNCTION static Point3D Zero() noexcept;

    /**
     * Function to build a 3-D point with all elements set to 1.
     * @returns Newly constructed Point3D with required attributes.
     */
    XS_FUNCTION static Point3D One() noexcept;

    /**
     * Get the x element of a 3-D point.
     * @note Optimized for getting value from register.
     * @tparam Index The index of the element to retrieve (must be between 0 and 2).
     * @returns InBaseDef containing the desired value.
     */
    template<uint32_t Index>
    XS_FUNCTION InBaseDef getValueInBase() const;

    /**
     * Get an element of a 3-D point.
     * @note Optimized for getting value from register.
     * @tparam Index The index of the element to retrieve (must be between 0 and 2).
     * @returns SIMDBase containing the desired value.
     */
    template<uint32_t Index>
    XS_FUNCTION BaseDef getValue() const;

    /**
     * Set an element of a 3-D point.
     * @tparam Index The index of the element to retrieve (must be between 0 and 2).
     * @param value The new value.
     */
    template<uint32_t Index>
    XS_FUNCTION void setValue(const InBaseDef& value) noexcept;

    /**
     * Set an element of a 3-D point.
     * @tparam Index The index of the element to retrieve (must be between 0 and 2).
     * @param value The new value.
     */
    template<uint32_t Index>
    XS_FUNCTION void setValue(const BaseDef& value) noexcept;
};

/**
 * Add a 3-D vector to a 3-D point.
 * @param point  The point.
 * @param vector Vector to add to the point.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Point3D<T, Width> operator+(
    const Point3D<T, Width>& point, const typename Point3D<T, Width>::Vector3DDef& vector) noexcept;

/**
 * Get the vector between two 3-D points.
 * @param point1 The first point.
 * @param point2 Point to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename Point3D<T, Width>::Vector3DDef operator-(
    const Point3D<T, Width>& point1, const Point3D<T, Width>& point2) noexcept;

/**
 * Subtract a vector from a 3-D point.
 * @param point  The point.
 * @param vector Vector3D to subtract from the point.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Point3D<T, Width> operator-(
    const Point3D<T, Width>& point, const typename Point3D<T, Width>::Vector3DDef& vector) noexcept;

/**
 * Multiply a 3-D point by a scalar.
 * @param point The point.
 * @param value Value to scale the point by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Point3D<T, Width> operator*(
    const Point3D<T, Width>& point, const typename Point3D<T, Width>::BaseDef& value) noexcept;

/**
 * Perform compound assignment and addition with a 3-D point.
 * @param [in,out] point  The first point.
 * @param          vector Vector3D to add to point.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Point3D<T, Width>& operator+=(
    Point3D<T, Width>& point, const typename Point3D<T, Width>::Vector3DDef& vector) noexcept;

/**
 * Perform compound assignment and subtraction by a 3-D point.
 * @param [in,out] point  The first point.
 * @param          vector Vector3D to subtract from point.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Point3D<T, Width>& operator-=(
    Point3D<T, Width>& point, const typename Point3D<T, Width>::Vector3DDef& vector) noexcept;

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] point The vector.
 * @param          value Value to multiply the vector by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION Point3D<T, Width>& operator*=(
    Point3D<T, Width>& point, const typename Point3D<T, Width>::BaseDeff& value) noexcept;
} // namespace Shift
