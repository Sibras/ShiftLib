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

#include "SIMD/XSSIMD3x2.hpp"
#include "SIMD/XSSIMD6.hpp"

// Additional includes
#include "Geometry/XSPoint3D.hpp"
#include "Geometry/XSVector3D.hpp"
#include "Geometry/XSVector3D2.hpp"

namespace Shift {
template<typename T, SIMDWidth Width, bool Packed>
class Point3D2;

template<typename T, bool Packed = false>
class Point3D2Data
{
    template<bool IsPacked>
    struct InternData
    {
        using Point3D2InternData = SIMD6Data<T>;
    };

    template<>
    struct InternData<false>
    {
        using Point3D2InternData = SIMD3x2Data<T>;
    };

public:
    typename InternData<Packed>::Point3D2InternData values;

    /** Default constructor. */
    XS_FUNCTION Point3D2Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit Point3D2Data(const Point3D2<T, Width, Packed>& other) noexcept;

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param p0X The X value of the first point.
     * @param p1X The X value of the second point.
     * @param p0Y The Y value of the first point.
     * @param p1Y The Y value of the second point.
     * @param p0Z The Z value of the first point.
     * @param p1Z The Z value of the second point.
     */
    XS_FUNCTION void setData(T p0X, T p1X, T p0Y, T p1Y, T p0Z, T p1Z) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const Point3D2<T, Width, Packed>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Point3D2<T, Width, Packed> load() const noexcept;
};

template<typename T, bool Packed = false>
class Point3D2DataPad
{
    template<bool IsPacked>
    struct InternData
    {
        using Point3D2InternDataPad = SIMD6DataPad<T>;
    };

    template<>
    struct InternData<false>
    {
        using Point3D2InternDataPad = SIMD3x2DataPad<T>;
    };

public:
    typename InternData<Packed>::Point3D2InternDataPad values;

    /** Default constructor. */
    XS_FUNCTION Point3D2DataPad() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit Point3D2DataPad(const Point3D2<T, Width, Packed>& other) noexcept;

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param p0X The X value of the first point.
     * @param p1X The X value of the second point.
     * @param p0Y The Y value of the first point.
     * @param p1Y The Y value of the second point.
     * @param p0Z The Z value of the first point.
     * @param p1Z The Z value of the second point.
     */
    XS_FUNCTION void setData(T p0X, T p1X, T p0Y, T p1Y, T p0Z, T p1Z) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const Point3D2<T, Width, Packed>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Point3D2<T, Width, Packed> load() const noexcept;
};

/**
 * A base type used to store 2 3-D points.
 * @tparam T      Generic type parameter.
 * @tparam Width  Type of SIMD being used.
 * @tparam Packed Type of storage used for internal data.
 */
template<typename T, SIMDWidth Width = widthSIMD<T>, bool Packed = false>
class Point3D2
{
    template<bool IsPacked>
    struct InternData
    {
        using Point3D2Intern = SIMD6<T, SIMD6<T, Width>::widthImpl>;
        using SIMD6Def = Point3D2Intern;
        using SIMD3x2Def = typename Point3D2Intern::SIMD3x2Def;
    };

    template<>
    struct InternData<false>
    {
        using Point3D2Intern = SIMD3x2<T, SIMD6<T, Width>::widthImpl>;
        using SIMD6Def = typename Point3D2Intern::SIMD6Def;
        using SIMD3x2Def = Point3D2Intern;
    };

public:
    using Type = T;
    using Point3D2Def = typename InternData<Packed>::Point3D2Intern;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Point3D2Def::widthImpl;
    static constexpr bool packed = Packed;
    using BaseDef = typename Point3D2Def::BaseDef;
    using InBaseDef = typename Point3D2Def::InBaseDef;
    using SIMD6Def = typename InternData<Packed>::SIMD6Def;
    using SIMD3x2Def = typename InternData<Packed>::SIMD3x2Def;
    using Point3DDef = Point3D<T, Point3D<T, widthImpl>::widthImpl>;
    using Vector3DDef = Vector3D<T, Vector3D<T, widthImpl>::widthImpl>;
    using Vector3D2Def = Vector3D2<T, Width, packed>; // Note: must be Width as operators don't support mixing widths

    Point3D2Def points;

    /** Default constructor. */
    XS_FUNCTION Point3D2() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION Point3D2(const Point3D2& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Point3D2(Point3D2&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Point3D2& operator=(const Point3D2& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Point3D2& operator=(Point3D2&& other) noexcept = default;

    /**
     * Construct a point3d2 from a SIMD6.
     * @param values The SIMD6.
     */
    XS_FUNCTION explicit Point3D2(const SIMD6Def& values) noexcept;

    /**
     * Construct a point3d2 from a SIMD3x2.
     * @param values The SIMD3x2.
     */
    XS_FUNCTION explicit Point3D2(const SIMD3x2Def& values) noexcept;

    /**
     * Construct a point3d2 from 2 point3ds.
     * @param point0 The first point.
     * @param point1 The second point.
     */
    XS_FUNCTION Point3D2(const Point3DDef& point0, const Point3DDef& point1) noexcept;

    /**
     * Construct a point3d2 from 6 values representing 2 different point3ds.
     * @param p0X The X value of the first point.
     * @param p1X The X value of the second point.
     * @param p0Y The Y value of the first point.
     * @param p1Y The Y value of the second point.
     * @param p0Z The Z value of the first point.
     * @param p1Z The Z value of the second point.
     */
    XS_FUNCTION Point3D2(T p0X, T p1X, T p0Y, T p1Y, T p0Z, T p1Z) noexcept;

    /**
     * Construct a point3d2 from a single point.
     * @param point The point.
     */
    XS_FUNCTION explicit Point3D2(const Point3DDef& point) noexcept;

    /**
     * Get a contained 3-D point.
     * @tparam Index The index of the point to retrieve (must be between 0 and 1).
     * @returns Point3D containing the desired point.
     */
    template<uint32_t Index>
    XS_FUNCTION Point3DDef getPoint() const noexcept;

    /**
     * Set a contained 3-D point.
     * @tparam Index The index of the point to set (must be between 0 and 1).
     * @param point The new point.
     */
    template<uint32_t Index>
    XS_FUNCTION void setPoint(const Point3DDef& point) noexcept;
};

/**
 * Add a vector to each component point in a point4.
 * @param point  The point3d4.
 * @param vector Vector3d to add to the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Point3D2<T, Width, Packed> operator+(
    const Point3D2<T, Width, Packed>& point, const typename Point3D2<T, Width, Packed>::Vector3DDef& vector) noexcept;

/**
 * Add a vector2 to each component point2 in a point4.
 * @param point  The point3d4.
 * @param vector Vector3d2 to add to the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Point3D2<T, Width, Packed> operator+(
    const Point3D2<T, Width, Packed>& point, const typename Point3D2<T, Width, Packed>::Vector3D2Def& vector) noexcept;

/**
 * Subtract a point from each component point in a point4.
 * @param point1 The point3d4.
 * @param point2 Point3d to subtract from the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION typename Point3D2<T, Width, Packed>::Vector3D2Def operator-(
    const Point3D2<T, Width, Packed>& point1, const typename Point3D2<T, Width, Packed>::Point3DDef& point2) noexcept;

/**
 * Subtract a point2 from each component point2 in a point4.
 * @param point1 The point3d4.
 * @param point2 Point3d2 to subtract from the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION typename Point3D2<T, Width, Packed>::Vector3D2Def operator-(
    const Point3D2<T, Width, Packed>& point1, const Point3D2<T, Width, Packed>& point2) noexcept;

/**
 * Subtract a vector from each component point in a point4.
 * @param point  The point3d4.
 * @param vector Vector3d to subtract from the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Point3D2<T, Width, Packed> operator-(
    const Point3D2<T, Width, Packed>& point, const typename Point3D2<T, Width, Packed>::Vector3DDef& vector) noexcept;

/**
 * Subtract a vector2 from each component point2 in a point4.
 * @param point  The point3d4.
 * @param vector Vector3d2 to subtract from the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Point3D2<T, Width, Packed> operator-(
    const Point3D2<T, Width, Packed>& point, const typename Point3D2<T, Width, Packed>::Vector3D2Def& vector) noexcept;

/**
 * Perform compound assignment and addition of a vector to each component point in a point4.
 * @param [in,out] point  The point3d4.
 * @param          vector Vector3d to add to the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Point3D2<T, Width, Packed>& operator+=(
    Point3D2<T, Width, Packed>& point, const typename Point3D2<T, Width, Packed>::Vector3DDef& vector) noexcept;

/**
 * Perform compound assignment and addition of a vector2 to each component point2 in a point4.
 * @param [in,out] point  The point3d4.
 * @param          vector Vector3d2 to add to the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Point3D2<T, Width, Packed>& operator+=(
    Point3D2<T, Width, Packed>& point, const typename Point3D2<T, Width, Packed>::Vector3D2Def& vector) noexcept;

/**
 * Perform compound assignment and subtraction of a vector from each component point in a point4.
 * @param [in,out] point  The point3d4.
 * @param          vector Vector3d to subtract from the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Point3D2<T, Width, Packed>& operator-=(
    Point3D2<T, Width, Packed>& point, const typename Point3D2<T, Width, Packed>::Vector3DDef& vector) noexcept;

/**
 * Perform compound assignment and subtraction of a vector2 from each component point2 in a point4.
 * @param [in,out] point  The point3d4.
 * @param          vector Vector3d2 to subtract from the point3d4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width, bool Packed>
XS_FUNCTION Point3D2<T, Width, Packed>& operator-=(
    Point3D2<T, Width, Packed>& point, const typename Point3D2<T, Width, Packed>::Vector3D2Def& vector) noexcept;
} // namespace Shift
