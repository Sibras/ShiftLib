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

#include "Geometry/XSPoint3D4.hpp"

// Additional includes
#include "Geometry/XSAABoundingBox.hpp"
#include "SIMD/XSSIMD4.hpp"

namespace Shift {
template<typename T, SIMDWidth Width, bool Packed>
class AABoundingBox4;

template<typename T, bool Packed = false>
class AABoundingBox4Data
{
public:
    Point3D4Data<T, Packed> minPoints;
    Point3D4Data<T, Packed> maxPoints;

    /** Default constructor. */
    XS_FUNCTION AABoundingBox4Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit AABoundingBox4Data(const AABoundingBox4<T, Width, Packed>& other) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const AABoundingBox4<T, Width, Packed>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION AABoundingBox4<T, Width, Packed> load() const noexcept;
};

template<typename T, bool Packed = false>
using AABoundingBox4DataPad = AABoundingBox4Data<T, Packed>;

/**
 * A boundingBox4 object. The boundingBox4 is used to store 4 bounding boxes at a time.
 * @tparam T      Generic type parameter.
 * @tparam Width  Type of SIMD being used.
 * @tparam Packed Type of storage used for internal data.
 */
template<typename T, SIMDWidth Width = widthSIMD<T>, bool Packed = false>
class AABoundingBox4
{
public:
    using Type = T;
    using Point3D4Def = Point3D4<T, Point3D4<T, Width, Packed>::widthImpl, Packed>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Point3D4Def::widthImpl;
    static constexpr bool packed = Packed;
    using SIMD4Def = SIMD4<T, SIMD4<T, widthImpl>::widthImpl>;
    using AABBDef = AABoundingBox<T, AABoundingBox<T, widthImpl>::widthImpl>;

    Point3D4Def minPoints; /**< The min values of each of the 4 bounding boxes */
    Point3D4Def maxPoints; /**< The max values of each of the 4 bounding boxes */

    /** Default constructor. */
    XS_FUNCTION AABoundingBox4() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION AABoundingBox4(const AABoundingBox4& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION AABoundingBox4(AABoundingBox4&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION AABoundingBox4& operator=(const AABoundingBox4& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION AABoundingBox4& operator=(AABoundingBox4&& other) noexcept = default;

    /**
     * Construct a boundingBox4 in structure of arrays format.
     * @param minValues The new min values for the 4 BBs.
     * @param maxValues The new max values for the 4 BBs.
     */
    XS_FUNCTION AABoundingBox4(const Point3D4Def& minValues, const Point3D4Def& maxValues) noexcept;

    /**
     * Construct a boundingBox4 from 4 regular bounding boxes.
     * @param box0 The first bounding box.
     * @param box1 The second bounding box.
     * @param box2 The third bounding box.
     * @param box3 The fourth bounding box.
     */
    XS_FUNCTION AABoundingBox4(
        const AABBDef& box0, const AABBDef& box1, const AABBDef& box2, const AABBDef& box3) noexcept;

    /**
     * Construct a boundingBox4 from a single regular bounding boxes.
     * @param box The first bounding box.
     */
    XS_FUNCTION explicit AABoundingBox4(const AABBDef& box) noexcept;

    /**
     * Extends the original boundingBox4 to incorporate the new points.
     * @param points The bounding points to extend around.
     */
    XS_FUNCTION void extend(const Point3D4Def& points) noexcept;

    /**
     * Extends the original boundingBox4 to incorporate the additional ones.
     * @param box The bounding boxes to add to the originals.
     */
    XS_FUNCTION void extend(const AABoundingBox4& box) noexcept;

    /**
     * Calculates the Surface Area of a boundingBox4.
     * @returns The surface area of the boxes in floating point.
     */
    XS_FUNCTION SIMD4Def surfaceArea() const noexcept;
};
} // namespace Shift
