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

// Additional includes
#include "Geometry/XSAABoundingBox.hpp"
#include "SIMD/XSSIMD2.hpp"

namespace Shift {
template<typename T, SIMDWidth Width, bool Packed>
class AABoundingBox2;

template<typename T, bool Packed = false>
class AABoundingBox2Data
{
public:
    Point3D2Data<T, Packed> minPoints;
    Point3D2Data<T, Packed> maxPoints;

    /** Default constructor. */
    XS_FUNCTION AABoundingBox2Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit AABoundingBox2Data(const AABoundingBox2<T, Width, Packed>& other) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const AABoundingBox2<T, Width, Packed>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION AABoundingBox2<T, Width, Packed> load() const noexcept;
};

template<typename T, bool Packed = false>
class AABoundingBox2DataPad
{
public:
    Point3D2DataPad<T, Packed> minPoints;
    Point3D2DataPad<T, Packed> maxPoints;

    /** Default constructor. */
    XS_FUNCTION AABoundingBox2DataPad() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit AABoundingBox2DataPad(const AABoundingBox2<T, Width, Packed>& other) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const AABoundingBox2<T, Width, Packed>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION AABoundingBox2<T, Width, Packed> load() const noexcept;
};

/**
 * A boundingBox2 object. The boundingBox2 is used to store 2 bounding boxes at a time.
 * @tparam T     Generic type parameter.
 * @tparam Width Type of SIMD being used.
 * @tparam Packed Type of storage used for internal data.
 */
template<typename T, SIMDWidth Width = widthSIMD<defaultSIMD>, bool Packed = false>
class AABoundingBox2
{
public:
    using Type = T;
    using Point3D2Def = Point3D2<T, Point3D2<T, Width, Packed>::widthImpl, Packed>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Point3D2Def::widthImpl;
    static constexpr bool packed = Packed;
    using SIMD2Def = SIMD2<T, SIMD2<T, widthImpl>::widthImpl>;
    using AABBDef = AABoundingBox<T, AABoundingBox<T, widthImpl>::widthImpl>;

    Point3D2Def minPoints; /**< The min values of each of the 2 bounding boxes */
    Point3D2Def maxPoints; /**< The max values of each of the 2 bounding boxes */

    /** Default constructor. */
    XS_FUNCTION AABoundingBox2() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION AABoundingBox2(const AABoundingBox2& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION AABoundingBox2(AABoundingBox2&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION AABoundingBox2& operator=(const AABoundingBox2& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION AABoundingBox2& operator=(AABoundingBox2&& other) noexcept = default;

    /**
     * Construct a boundingBox2 in structure of arrays format.
     * @param minValues The new min values for the 2 BBs.
     * @param maxValues The new max values for the 2 BBs.
     */
    XS_FUNCTION AABoundingBox2(const Point3D2Def& minValues, const Point3D2Def& maxValues) noexcept;

    /**
     * Construct a boundingBox2 from 2 regular bounding boxes.
     * @param box0 The first bounding box.
     * @param box1 The second bounding box.
     */
    XS_FUNCTION AABoundingBox2(const AABBDef& box0, const AABBDef& box1) noexcept;

    /**
     * Construct a boundingBox2 from a single regular bounding boxes.
     * @param box The first bounding box.
     */
    XS_FUNCTION explicit AABoundingBox2(const AABBDef& box) noexcept;

    /**
     * Extends the original boundingBox2 to incorporate the new points.
     * @param points The bounding points to extend around.
     */
    XS_FUNCTION void extend(const Point3D2Def& points) noexcept;

    /**
     * Extends the original boundingBox2 to incorporate the additional one.
     * @param box The bounding boxes to add to the originals.
     */
    XS_FUNCTION void extend(const AABoundingBox2& box) noexcept;

    /**
     * Calculates the Surface Area of a boundingBox2.
     * @returns The surface area of the boxes in floating point.
     */
    XS_FUNCTION SIMD2Def surfaceArea() const;
};
} // namespace Shift
