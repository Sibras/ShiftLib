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

#include "Geometry/XSAABoundingBox.hpp"
#include "Geometry/XSPoint3D4.hpp"
#include "XSLimits.hpp"

namespace Shift {
template<typename T, bool Packed, SIMDWidth Width>
class AABoundingBox4;

template<typename T, bool Packed = false>
class AABoundingBox4Data
{
public:
    Point3D4Data<T, Packed> minPoints;
    Point3D4Data<T, Packed> maxPoints;

    /** Default constructor. */
    XS_INLINE AABoundingBox4Data() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE AABoundingBox4Data(const AABoundingBox4Data& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE AABoundingBox4Data(AABoundingBox4Data&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE AABoundingBox4Data& operator=(const AABoundingBox4Data& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE AABoundingBox4Data& operator=(AABoundingBox4Data&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit AABoundingBox4Data(const AABoundingBox4<T, Packed, Width>& other) noexcept
        : minPoints(other.minPoints)
        , maxPoints(other.maxPoints)
    {}

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const AABoundingBox4<T, Packed, Width>& other) noexcept
    {
        this->minPoints.store(other.minPoints);
        this->maxPoints.store(other.maxPoints);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD512<T>>
    XS_INLINE AABoundingBox4<T, Packed, Width> load() const noexcept
    {
        return AABoundingBox4<T, Packed, Width>(
            this->minPoints.template load<AABoundingBox4<T, Packed, Width>::widthImpl>(),
            this->maxPoints.template load<AABoundingBox4<T, Packed, Width>::widthImpl>());
    }
};

template<typename T, bool Packed = false>
using AABoundingBox4DataPad = AABoundingBox4Data<T, Packed>;

/**
 * A boundingBox4 object. The boundingBox4 is used to store 4 bounding boxes at a time.
 * @tparam T      Generic type parameter.
 * @tparam Width  Type of SIMD being used.
 * @tparam Packed Type of storage used for internal data.
 */
template<typename T, bool Packed = false, SIMDWidth Width = defaultWidthSIMD512<T>>
class AABoundingBox4
{
public:
    using Type = T;
    using Point3D4Def = Point3D4<T, Packed, Point3D4<T, Packed, Width>::widthImpl>;
    using Data = AABoundingBox4Data<T, Packed>;
    using DataPad = AABoundingBox4DataPad<T, Packed>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Point3D4Def::widthImpl;
    static constexpr bool packed = Packed;
    using SIMD4Def = SIMD4<T, SIMD4<T, widthImpl>::widthImpl>;
    using AABBDef = AABoundingBox<T, AABoundingBox<T, widthImpl>::widthImpl>;

    Point3D4Def minPoints; /**< The min values of each of the 4 bounding boxes */
    Point3D4Def maxPoints; /**< The max values of each of the 4 bounding boxes */

    /** Default constructor. */
    XS_INLINE AABoundingBox4() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE AABoundingBox4(const AABoundingBox4& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE AABoundingBox4(AABoundingBox4&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE AABoundingBox4& operator=(const AABoundingBox4& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE AABoundingBox4& operator=(AABoundingBox4&& other) noexcept = default;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_INLINE explicit AABoundingBox4(const AABoundingBox4<T, Packed, Width2>& other) noexcept
        : minPoints(other.minPoints)
        , maxPoints(other.maxPoints)
    {}

    /**
     * Construct a boundingBox4 in structure of arrays format.
     * @param minValues The new min values for the 4 BBs.
     * @param maxValues The new max values for the 4 BBs.
     */
    XS_INLINE AABoundingBox4(const Point3D4Def& minValues, const Point3D4Def& maxValues) noexcept
        : minPoints(minValues)
        , maxPoints(maxValues)
    {}

    /**
     * Construct a boundingBox4 from 4 regular bounding boxes.
     * @param box0 The first bounding box.
     * @param box1 The second bounding box.
     * @param box2 The third bounding box.
     * @param box3 The fourth bounding box.
     */
    XS_INLINE AABoundingBox4(
        const AABBDef& box0, const AABBDef& box1, const AABBDef& box2, const AABBDef& box3) noexcept
        : minPoints(box0.minPoint, box1.minPoint, box2.minPoint, box3.minPoint)
        , maxPoints(box0.maxPoint, box1.maxPoint, box2.maxPoint, box3.maxPoint)
    {}

    /**
     * Construct a boundingBox4 from a single regular bounding boxes.
     * @param box The first bounding box.
     */
    XS_INLINE explicit AABoundingBox4(const AABBDef& box) noexcept
        : minPoints(box.minPoint)
        , maxPoints(box.maxPoint)
    {}

    /**
     * Function to build a boundingbox2 with invalid extents.
     * @returns Newly constructed bounding box with required attributes.
     */
    XS_INLINE static AABoundingBox4 Invalid() noexcept
    {
        return AABoundingBox4(Point3D4Def(Limits<Type>::Max()), Point3D4Def(Limits<Type>::Lowest()));
    }

    /**
     * Extends the original boundingBox4 to incorporate the new points.
     * @param points The bounding points to extend around.
     */
    XS_INLINE void extend(const Point3D4Def& points) noexcept
    {
        this->minPoints = Point3D4Def(this->minPoints.points.min(points.points));
        this->maxPoints = Point3D4Def(this->maxPoints.points.max(points.points));
    }

    /**
     * Extends the original boundingBox4 to incorporate the additional ones.
     * @param box The bounding boxes to add to the originals.
     */
    XS_INLINE void extend(const AABoundingBox4& box) noexcept
    {
        this->minPoints = Point3D4Def(this->minPoints.points.min(box.minPoints.points));
        this->maxPoints = Point3D4Def(this->maxPoints.points.max(box.maxPoints.points));
    }

    /**
     * Calculates the Surface Area of a boundingBox4.
     * @returns The surface area of the boxes in floating point.
     */
    XS_INLINE SIMD4Def surfaceArea() const noexcept
    {
        // calculate size of bounding box
        auto size(this->maxPoints.points - this->minPoints.points);
        // calculate Surface Area
        const auto sizeYZX(size.template shuffle3<1, 2, 0>());
        return ((size *= sizeYZX).hadd3() /** SIMDBase( 2.0f )*/);
    }
};
} // namespace Shift
