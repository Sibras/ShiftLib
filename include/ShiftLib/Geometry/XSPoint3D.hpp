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

namespace Shift {
template<typename T, SIMDWidth Width>
class Point3D;

template<typename T>
class Point3DData
{
public:
    SIMD3Data<T> values;

    /** Default constructor. */
    XS_INLINE Point3DData() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Point3DData(const Point3DData& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE Point3DData(Point3DData&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Point3DData& operator=(const Point3DData& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Point3DData& operator=(Point3DData&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit Point3DData(const Point3D<T, Width>& other) noexcept
        : values(other.values)
    {}

    /**
     * Construct from member variables.
     * @param value0In The first pre-calculated value.
     * @param value1In The second pre-calculated value.
     * @param value2In The third pre-calculated value.
     */
    XS_INLINE Point3DData(T&& value0In, T&& value1In, T&& value2In)
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
    XS_INLINE void store(const Point3D<T, Width>& other) noexcept
    {
        this->values.store(other.values);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD128<T>>
    XS_INLINE Point3D<T, Width> load() const noexcept
    {
        return Point3D<T, Width>(this->values.template load<Point3D<T, Width>::widthImpl>());
    }
};

template<typename T>
class Point3DDataPad
{
public:
    SIMD3DataPad<T> values;

    /** Default constructor. */
    XS_INLINE Point3DDataPad() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Point3DDataPad(const Point3DDataPad& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE Point3DDataPad(Point3DDataPad&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Point3DDataPad& operator=(const Point3DDataPad& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Point3DDataPad& operator=(Point3DDataPad&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit Point3DDataPad(const Point3D<T, Width>& other) noexcept
        : values(other.values)
    {}

    /**
     * Construct from member variables.
     * @param value0In The first pre-calculated value.
     * @param value1In The second pre-calculated value.
     * @param value2In The third pre-calculated value.
     */
    XS_INLINE Point3DDataPad(T&& value0In, T&& value1In, T&& value2In)
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
    XS_INLINE void store(const Point3D<T, Width>& other) noexcept
    {
        this->values.store(other.values);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD128<T>>
    XS_INLINE Point3D<T, Width> load() const noexcept
    {
        return Point3D<T, Width>(this->values.template load<Point3D<T, Width>::widthImpl>());
    }
};

/**
 * A a base type used to store a 3-D point.
 * @tparam T     Generic type parameter.
 * @tparam Width Type of SIMD being used.
 */
template<typename T, SIMDWidth Width = defaultWidthSIMD128<T>>
class Point3D
{
public:
    using Type = T;
    using SIMD3Def = SIMD3<T, SIMD3<T, Width>::widthImpl>;
    using Data = Point3DData<T>;
    using DataPad = Point3DDataPad<T>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = SIMD3Def::widthImpl;
    using BaseDef = typename SIMD3Def::BaseDef;
    using InBaseDef = typename SIMD3Def::InBaseDef;
    using Vector3DDef = Vector3D<T, Vector3D<T, widthImpl>::widthImpl>;

    SIMD3Def values;

    /** Default constructor. */
    XS_INLINE Point3D() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Point3D(const Point3D& other) = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE Point3D(Point3D&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Point3D& operator=(const Point3D& other) = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Point3D& operator=(Point3D&& other) noexcept = default;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_INLINE explicit Point3D(const Point3D<T, Width2>& other) noexcept
        : values(other.values)
    {}

    /**
     * Construct a 3-D point from x, y, and z elements.
     * @param x The x value.
     * @param y The y value.
     * @param z The z value.
     */
    XS_INLINE Point3D(T x, T y, T z) noexcept
        : values(x, y, z)
    {}

    /**
     * Constructor to set all elements to the same scalar value.
     * @param value Value to set all point elements to.
     */
    XS_INLINE explicit Point3D(T value) noexcept
        : values(value)
    {}

    /**
     * Construct a 3-D point from a SIMD3.
     * @param values The 3 values.
     */
    XS_INLINE explicit Point3D(const SIMD3Def& values) noexcept
        : values(values)
    {}

    /**
     * Function to build a 3-D point with all elements set to 0.
     * @returns Newly constructed Point3D with required attributes.
     */
    XS_INLINE static Point3D Zero() noexcept
    {
        return Point3D(SIMD3Def::Zero());
    }

    /**
     * Function to build a 3-D point with all elements set to 1.
     * @returns Newly constructed Point3D with required attributes.
     */
    XS_INLINE static Point3D One() noexcept
    {
        return Point3D(SIMD3Def::One());
    }

    /**
     * Get the x element of a 3-D point.
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
     * Get an element of a 3-D point.
     * @note Optimized for getting value from register.
     * @tparam Index The index of the element to retrieve (must be between 0 and 2).
     * @returns SIMDBase containing the desired value.
     */
    template<uint32_t Index>
    XS_INLINE BaseDef getValue() const noexcept
    {
        return this->values.template getValue<Index>();
    }

    /**
     * Set an element of a 3-D point.
     * @tparam Index The index of the element to retrieve (must be between 0 and 2).
     * @param value The new value.
     */
    template<uint32_t Index>
    XS_INLINE void setValue(InBaseDef value) noexcept
    {
        this->values.template setValue<Index>(value);
    }

    /**
     * Set an element of a 3-D point.
     * @tparam Index The index of the element to retrieve (must be between 0 and 2).
     * @param value The new value.
     */
    template<uint32_t Index>
    XS_INLINE void setValue(BaseDef value) noexcept
    {
        this->values.template setValue<Index>(value);
    }
};

/**
 * Add a 3-D vector to a 3-D point.
 * @param point  The point.
 * @param vector Vector to add to the point.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Point3D<T, Width> operator+(
    const Point3D<T, Width>& point, const typename Point3D<T, Width>::Vector3DDef& vector) noexcept
{
    return Point3D<T, Width>(point.values + vector.values);
}

/**
 * Get the vector between two 3-D points.
 * @param point1 The first point.
 * @param point2 Point to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE typename Point3D<T, Width>::Vector3DDef operator-(
    const Point3D<T, Width>& point1, const Point3D<T, Width>& point2) noexcept
{
    return typename Point3D<T, Width>::Vector3DDef(point1.values - point2.values);
}

/**
 * Subtract a vector from a 3-D point.
 * @param point  The point.
 * @param vector Vector3D to subtract from the point.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Point3D<T, Width> operator-(
    const Point3D<T, Width>& point, const typename Point3D<T, Width>::Vector3DDef& vector) noexcept
{
    return Point3D<T, Width>(point.values - vector.values);
}

/**
 * Multiply a 3-D point by a scalar.
 * @param point The point.
 * @param value Value to scale the point by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Point3D<T, Width> operator*(
    const Point3D<T, Width>& point, typename Point3D<T, Width>::BaseDef value) noexcept
{
    return Point3D<T, Width>(point.values * value);
}

/**
 * Perform compound assignment and addition with a 3-D point.
 * @param [in,out] point  The first point.
 * @param          vector Vector3D to add to point.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Point3D<T, Width>& operator+=(
    Point3D<T, Width>& point, const typename Point3D<T, Width>::Vector3DDef& vector) noexcept
{
    point.values += vector.values;
    return point;
}

/**
 * Perform compound assignment and subtraction by a 3-D point.
 * @param [in,out] point  The first point.
 * @param          vector Vector3D to subtract from point.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Point3D<T, Width>& operator-=(
    Point3D<T, Width>& point, const typename Point3D<T, Width>::Vector3DDef& vector) noexcept
{
    point.values -= vector.values;
    return point;
}

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] point The vector.
 * @param          value Value to multiply the vector by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE Point3D<T, Width>& operator*=(Point3D<T, Width>& point, typename Point3D<T, Width>::BaseDef value) noexcept
{
    point.values *= value;
    return point;
}
} // namespace Shift
