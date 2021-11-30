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

#include "Geometry/XSPoint3D2.hpp"
#include "Geometry/XSVector3D4.hpp"
#include "SIMD/XSSIMD12.hpp"
#include "SIMD/XSSIMD3x4.hpp"

namespace Shift {
template<typename T, SIMDWidth Width, bool Packed>
class Point3D4;

template<typename T, bool Packed = false>
class Point3D4Data
{
    template<bool IsPacked>
    struct InternData
    {
        using Point3D4InternData = SIMD12Data<T>;
    };

    template<>
    struct InternData<false>
    {
        using Point3D4InternData = SIMD3x4Data<T>;
    };

public:
    typename InternData<Packed>::Point3D4InternData values;

    /** Default constructor. */
    XS_INLINE Point3D4Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit Point3D4Data(const Point3D4<T, Width, Packed>& other) noexcept
        : values(other.points)
    {}

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param p0X The X value of the first point.
     * @param p1X The X value of the second point.
     * @param p2X The X value of the third point.
     * @param p3X The X value of the fourth point.
     * @param p0Y The Y value of the first point.
     * @param p1Y The Y value of the second point.
     * @param p2Y The Y value of the third point.
     * @param p3Y The Y value of the fourth point.
     * @param p0Z The Z value of the first point.
     * @param p1Z The Z value of the second point.
     * @param p2Z The Z value of the third point.
     * @param p3Z The Z value of the fourth point.
     */
    XS_INLINE void setData(T p0X, T p1X, T p2X, T p3X, T p0Y, T p1Y, T p2Y, T p3Y, T p0Z, T p1Z, T p2Z, T p3Z) noexcept
    {
        if constexpr (!Packed) {
            this->values.setData(p0X, p0Y, p0Z, p1X, p1Y, p1Z, p2X, p2Y, p2Z, p3X, p3Y, p3Z);
        } else {
            this->values.setData(p0X, p1X, p2X, p3X, p0Y, p1Y, p2Y, p3Y, p0Z, p1Z, p2Z, p3Z);
        }
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const Point3D4<T, Width, Packed>& other) noexcept
    {
        this->values.store(other.points);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_INLINE Point3D4<T, Width, Packed> load() const noexcept
    {
        return Point3D4<T, Width, Packed>(this->values.template load<Point3D4<T, Width, Packed>::widthImpl>());
    }
};

template<typename T, bool Packed = false>
class Point3D4DataPad
{
    template<bool IsPacked>
    struct InternData
    {
        using Point3D4InternData = SIMD12DataPad<T>;
    };

    template<>
    struct InternData<false>
    {
        using Point3D4InternData = SIMD3x4DataPad<T>;
    };

public:
    typename InternData<Packed>::Point3D4InternData values;

    /** Default constructor. */
    XS_INLINE Point3D4DataPad() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit Point3D4DataPad(const Point3D4<T, Width, Packed>& other) noexcept
        : values(other.points)
    {}

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param p0X The X value of the first point.
     * @param p1X The X value of the second point.
     * @param p2X The X value of the third point.
     * @param p3X The X value of the fourth point.
     * @param p0Y The Y value of the first point.
     * @param p1Y The Y value of the second point.
     * @param p2Y The Y value of the third point.
     * @param p3Y The Y value of the fourth point.
     * @param p0Z The Z value of the first point.
     * @param p1Z The Z value of the second point.
     * @param p2Z The Z value of the third point.
     * @param p3Z The Z value of the fourth point.
     */
    XS_INLINE void setData(T p0X, T p1X, T p2X, T p3X, T p0Y, T p1Y, T p2Y, T p3Y, T p0Z, T p1Z, T p2Z, T p3Z) noexcept
    {
        if constexpr (!Packed) {
            this->values.setData(p0X, p0Y, p0Z, p1X, p1Y, p1Z, p2X, p2Y, p2Z, p3X, p3Y, p3Z);
        } else {
            this->values.setData(p0X, p1X, p2X, p3X, p0Y, p1Y, p2Y, p3Y, p0Z, p1Z, p2Z, p3Z);
        }
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const Point3D4<T, Width, Packed>& other) noexcept
    {
        this->values.store(other.points);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_INLINE Point3D4<T, Width, Packed> load() const noexcept
    {
        return Point3D4<T, Width, Packed>(this->values.template load<Point3D4<T, Width, Packed>::widthImpl>());
    }
};

/**
 * A  base type used to store 4 3-D points.
 * @tparam T      Generic type parameter.
 * @tparam Width  Type of SIMD being used.
 * @tparam Packed Type of storage used for internal data.
 */
template<typename T, SIMDWidth Width = widthSIMD<T>, bool Packed = false>
class Point3D4
{
    template<bool IsPacked>
    struct InternData
    {
        using Point3D4Intern = SIMD12<T, SIMD12<T, Width>::widthImpl>;
        using SIMD12Def = Point3D4Intern;
        using SIMD3x4Def = SIMD3x4<T, SIMD3x4<T, Point3D4Intern::widthImpl>::widthImpl>;
    };

    template<>
    struct InternData<false>
    {
        using Point3D4Intern = SIMD3x4<T, SIMD3x4<T, Width>::widthImpl>;
        using SIMD12Def = typename Point3D4Intern::SIMD12Def;
        using SIMD3x4Def = Point3D4Intern;
    };

public:
    using Type = T;
    using Point3D4Def = typename InternData<Packed>::Point3D4Intern;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Point3D4Def::widthImpl;
    static constexpr bool packed = Packed;
    using BaseDef = typename Point3D4Def::BaseDef;
    using InBaseDef = typename Point3D4Def::InBaseDef;
    using SIMD12Def = typename InternData<Packed>::SIMD12Def;
    using SIMD3x4Def = typename InternData<Packed>::SIMD3x4Def;
    using Point3DDef = Point3D<T, Point3D<T, widthImpl>::widthImpl>;
    using Point3D2Def = Point3D2<T, Point3D2<T, widthImpl, Packed>::widthImpl, Packed>;
    using Vector3DDef = Vector3D<T, Vector3D<T, widthImpl>::widthImpl>;
    using Vector3D2Def = Vector3D2<T, Vector3D2<T, widthImpl, Packed>::widthImpl, Packed>;
    using Vector3D4Def = Vector3D4<T, Width, packed>; // Note: must be Width as operators don't support mixing widths

    Point3D4Def points;

    /** Default constructor. */
    XS_INLINE Point3D4() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Point3D4(const Point3D4& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE Point3D4(Point3D4&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Point3D4& operator=(const Point3D4& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Point3D4& operator=(Point3D4&& other) noexcept = default;

    /**
     * Construct a point3d4 from 2 point3d2s.
     * @param point0 The first point2.
     * @param point1 The second point2.
     */
    XS_INLINE Point3D4(const Point3D2Def& point0, const Point3D2Def& point1) noexcept
        : points(point0.points, point1.points)
    {}

    /**
     * Construct a point3d4 from 4 point3ds.
     * @param point0 The first point.
     * @param point1 The second point.
     * @param point2 The third point.
     * @param point3 The fourth point.
     */
    XS_INLINE Point3D4(
        const Point3DDef& point0, const Point3DDef& point1, const Point3DDef& point2, const Point3DDef& point3) noexcept
        : points(point0.values, point1.values, point2.values, point3.values)
    {}

    /**
     * Construct a point3d4 from 12 values representing 4 different point3ds.
     * @param fP0X The X value of the first point.
     * @param fP1X The X value of the second point.
     * @param fP2X The X value of the third point.
     * @param fP3X The X value of the fourth point.
     * @param fP0Y The Y value of the first point.
     * @param fP1Y The Y value of the second point.
     * @param fP2Y The Y value of the third point.
     * @param fP3Y The Y value of the fourth point.
     * @param fP0Z The Z value of the first point.
     * @param fP1Z The Z value of the second point.
     * @param fP2Z The Z value of the third point.
     * @param fP3Z The Z value of the fourth point.
     */
    XS_INLINE Point3D4(
        T fP0X, T fP1X, T fP2X, T fP3X, T fP0Y, T fP1Y, T fP2Y, T fP3Y, T fP0Z, T fP1Z, T fP2Z, T fP3Z) noexcept
    {
        if constexpr (!Packed) {
            points = Point3D4Def(fP0X, fP0Y, fP0Z, fP1X, fP1Y, fP1Z, fP2X, fP2Y, fP2Z, fP3X, fP3Y, fP3Z);
        } else {
            points = Point3D4Def(fP0X, fP1X, fP2X, fP3X, fP0Y, fP1Y, fP2Y, fP3Y, fP0Z, fP1Z, fP2Z, fP3Z);
        }
    }

    /**
     * Construct a point3d4 from a single point.
     * @param point The point.
     */
    XS_INLINE explicit Point3D4(const Point3DDef& point) noexcept
        : points(point.values)
    {}

    /**
     * Construct a point3d4 from a SIMD12.
     * @param values The SIMD12.
     */
    XS_INLINE explicit Point3D4(const SIMD12Def& values) noexcept
        : points(values)
    {}

    /**
     * Construct a point3d4 from a SIMD3x4.
     * @param values The SIMD3x4.
     */
    XS_INLINE explicit Point3D4(const SIMD3x4Def& values) noexcept
        : points(values)
    {}

    /**
     * Get a contained 3-D point.
     * @tparam Index The index of the point to retrieve (must be between 0 and 3).
     * @returns Point3D containing the desired point.
     */
    template<uint32_t Index>
    XS_INLINE Point3DDef getPoint() const noexcept
    {
        return Point3DDef(this->points.template getValue3<Index>());
    }

    /**
     * Set a contained 3-D point.
     * @tparam Index The index of the point to set (must be between 0 and 3).
     * @param point The new point.
     */
    template<uint32_t Index>
    XS_INLINE void setPoint(const Point3DDef& point) noexcept
    {
        this->points.template setValue3<Index>(point.values);
    }
};

/**
 * Add a vector to each component point in a point4.
 * @param point  The point3d4.
 * @param vector Vector3d to add to the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed> operator+(
    const Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3DDef& vector) noexcept
{
    return Point3D4<T, Width, Packed>(point.points + vector.values);
}

/**
 * Add a vector2 to each component point2 in a point4.
 * @param point  The point3d4.
 * @param vector Vector3d2 to add to the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed> operator+(
    const Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3D2Def& vector) noexcept
{
    return Point3D4<T, Width, Packed>(point.points + vector.vectors);
}

/**
 * Add a 3-D vector to a 3-D point4.
 * @param point  The point3d4.
 * @param vector Vector to add to the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed> operator+(
    const Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3D4Def& vector) noexcept
{
    return Point3D4<T, Width, Packed>(point.points + vector.vectors);
}

/**
 * Subtract a point from each component point in a point4.
 * @param point1 The point3d4.
 * @param point2 Point3d to subtract from the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE typename Point3D4<T, Width, Packed>::Vector3D4Def operator-(
    const Point3D4<T, Width, Packed>& point1, const typename Point3D4<T, Width, Packed>::Point3DDef& point2) noexcept
{
    return typename Point3D4<T, Width, Packed>::Vector3D4Def(point1.points - point2.values);
}

/**
 * Subtract a point2 from each component point2 in a point4.
 * @param point1 The point3d4.
 * @param point2 Point3d2 to subtract from the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE typename Point3D4<T, Width, Packed>::Vector3D4Def operator-(
    const Point3D4<T, Width, Packed>& point1, const typename Point3D4<T, Width, Packed>::Point3D2Def& point2) noexcept
{
    return typename Point3D4<T, Width, Packed>::Vector3D4Def(point1.points - point2.points);
}

/**
 * Get the vector between two 3-D point4s.
 * @param point1 The first point3d4.
 * @param point2 Point3d4 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE typename Point3D4<T, Width, Packed>::Vector3D4Def operator-(
    const Point3D4<T, Width, Packed>& point1, const Point3D4<T, Width, Packed>& point2) noexcept
{
    return typename Point3D4<T, Width, Packed>::Vector3D4Def(point1.points - point2.points);
}

/**
 * Subtract a vector from each component point in a point4.
 * @param point  The point3d4.
 * @param vector Vector3d to subtract from the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed> operator-(
    const Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3DDef& vector) noexcept
{
    return Point3D4<T, Width, Packed>(point.points - vector.values);
}

/**
 * Subtract a vector2 from each component point2 in a point4.
 * @param point  The point3d4.
 * @param vector Vector3d2 to subtract from the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed> operator-(
    const Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3D2Def& vector) noexcept
{
    return Point3D4<T, Width, Packed>(point.points - vector.vectors);
}

/**
 * Subtract a vector from a 3-D point4.
 * @param point  The point3d4.
 * @param vector Vector3D4 to subtract from the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed> operator-(
    const Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3D4Def& vector) noexcept
{
    return Point3D4<T, Width, Packed>(point.points - vector.vectors);
}

/**
 * Perform compound assignment and addition of a vector to each component point in a point4.
 * @param point  The point3d4.
 * @param vector Vector3d to add to the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed>& operator+=(
    Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3DDef& vector) noexcept
{
    point.points += vector.values;
    return point;
}

/**
 * Perform compound assignment and addition of a vector2 to each component point2 in a point4.
 * @param point  The point3d4.
 * @param vector Vector3d2 to add to the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed>& operator+=(
    Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3D2Def& vector) noexcept
{
    point.points += vector.vectors;
    return point;
}

/**
 * Perform compound assignment and addition with a 3-D point4.
 * @param [in,out] point  The first point3d4.
 * @param          vector Vector3D4 to add to point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed>& operator+=(
    Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3D4Def& vector) noexcept
{
    point.points += vector.vectors;
    return point;
}

/**
 * Perform compound assignment and subtraction of a vector from each component point in a point4.
 * @param [in,out] point  The point3d4.
 * @param          vector Vector3d to subtract from the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed>& operator-=(
    Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3DDef& vector) noexcept
{
    point.points -= vector.values;
    return point;
}

/**
 * Perform compound assignment and subtraction of a vector2 from each component point2 in a point4.
 * @param [in,out] point  The point3d4.
 * @param          vector Vector3d2 to subtract from the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed>& operator-=(
    Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3D2Def& vector) noexcept
{
    point.points -= vector.vectors;
    return point;
}

/**
 * Perform compound assignment and subtraction by a 3-D point4.
 * @param [in,out] point  The first point3d4.
 * @param          vector Vector3D4 to subtract from point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_INLINE Point3D4<T, Width, Packed>& operator-=(
    Point3D4<T, Width, Packed>& point, const typename Point3D4<T, Width, Packed>::Vector3D4Def& vector) noexcept
{
    point.points -= vector.vectors;
    return point;
}
} // namespace Shift
