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

#include "Geometry/XSPoint3D.hpp"

// Additional includes
#include "SIMD/XSSIMDInBase.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class AABoundingBox;

template<typename T>
class AABoundingBoxData
{
public:
    Point3DData<T> minPoint;
    Point3DData<T> maxPoint;

    /** Default constructor. */
    XS_FUNCTION AABoundingBoxData() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit AABoundingBoxData(const AABoundingBox<T, Width>& other) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const AABoundingBox<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION AABoundingBox<T, Width> load() const noexcept;
};

template<typename T>
class AABoundingBoxDataPad
{
public:
    Point3DDataPad<T> minPoint;
    Point3DDataPad<T> maxPoint;

    /** Default constructor. */
    XS_FUNCTION AABoundingBoxDataPad() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit AABoundingBoxDataPad(const AABoundingBox<T, Width>& other) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const AABoundingBox<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION AABoundingBox<T, Width> load() const noexcept;
};

/**
 * A bounding box object. A bounding box is defined by its 3 min and max values for each x, y and z.
 * @tparam T     Generic type parameter.
 * @tparam Width Type of SIMD being used.
 */
template<typename T, SIMDWidth Width = widthSIMD<T>>
class AABoundingBox
{
public:
    using Type = T;
    using Point3DDef = Point3D<T, Point3D<T, Width>::widthImpl>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Point3DDef::widthImpl;
    using InBaseDef = typename Point3DDef::InBaseDef;

    Point3DDef minPoint;
    Point3DDef maxPoint;

    /** Default constructor. */
    XS_FUNCTION AABoundingBox() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION AABoundingBox(const AABoundingBox& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION AABoundingBox(AABoundingBox&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION AABoundingBox& operator=(const AABoundingBox& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION AABoundingBox& operator=(AABoundingBox&& other) noexcept = default;

    /**
     * Construct a bounding box from its member variables.
     * @param minValues The new min values for the BB.
     * @param maxValues The new max values for the BB.
     */
    XS_FUNCTION AABoundingBox(const Point3DDef& minValues, const Point3DDef& maxValues) noexcept;

    /**
     * Construct a bounding box around a single point.
     * @param point The bounding point.
     */
    XS_FUNCTION explicit AABoundingBox(const Point3DDef& point) noexcept;

    /**
     * Extends the original bounding box to incorporate the new point.
     * @param point The bounding point to extend around.
     */
    XS_FUNCTION void extend(const Point3DDef& point) noexcept;

    /**
     * Extends the original bounding box to incorporate the additional one.
     * @param box The bounding box to add to the original.
     */
    XS_FUNCTION void extend(const AABoundingBox<T, Width>& box) noexcept;

    /**
     * Calculates the Surface Area of a Bounding Box.
     * @returns The surface area of the box in floating point.
     */
    XS_FUNCTION InBaseDef surfaceArea() const noexcept;
};
} // namespace Shift
