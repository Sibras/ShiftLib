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
#include "Geometry/XSPoint3D2.hpp"
#include "XSLimits.hpp"

namespace Shift {
template<typename T, bool Packed, SIMDWidth Width>
class AABoundingBox2;

template<typename T, bool Packed = false>
class AABoundingBox2Data
{
public:
    Point3D2Data<T, Packed> minPoints;
    Point3D2Data<T, Packed> maxPoints;

    /** Default constructor. */
    XS_INLINE AABoundingBox2Data() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE AABoundingBox2Data(const AABoundingBox2Data& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE AABoundingBox2Data(AABoundingBox2Data&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE AABoundingBox2Data& operator=(const AABoundingBox2Data& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE AABoundingBox2Data& operator=(AABoundingBox2Data&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit AABoundingBox2Data(const AABoundingBox2<T, Packed, Width>& other) noexcept
        : minPoints(other.minPoints)
        , maxPoints(other.maxPoints)
    {}

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const AABoundingBox2<T, Packed, Width>& other) noexcept
    {
        this->minPoints.store(other.minPoints);
        this->maxPoints.store(other.maxPoints);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD256<T>>
    XS_INLINE AABoundingBox2<T, Packed, Width> load() const noexcept
    {
        return AABoundingBox2<T, Packed, Width>(
            this->minPoints.template load<AABoundingBox2<T, Packed, Width>::widthImpl>(),
            this->maxPoints.template load<AABoundingBox2<T, Packed, Width>::widthImpl>());
    }
};

template<typename T, bool Packed = false, typename T2 = T>
class AABoundingBox2DataPad
{
public:
    Point3D2DataPad<T, Packed, T2> minPoints;
    Point3D2DataPad<T, Packed, T2> maxPoints;

    /** Default constructor. */
    XS_INLINE AABoundingBox2DataPad() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE AABoundingBox2DataPad(const AABoundingBox2DataPad& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE AABoundingBox2DataPad(AABoundingBox2DataPad&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE AABoundingBox2DataPad& operator=(const AABoundingBox2DataPad& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE AABoundingBox2DataPad& operator=(AABoundingBox2DataPad&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit AABoundingBox2DataPad(const AABoundingBox2<T, Packed, Width>& other) noexcept
        : minPoints(other.minPoints)
        , maxPoints(other.maxPoints)
    {}

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const AABoundingBox2<T, Packed, Width>& other) noexcept
    {
        this->minPoints.store(other.minPoints);
        this->maxPoints.store(other.maxPoints);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD256<T>>
    XS_INLINE AABoundingBox2<T, Packed, Width> load() const noexcept
    {
        return AABoundingBox2<T, Packed, Width>(
            this->minPoints.template load<AABoundingBox2<T, Packed, Width>::widthImpl>(),
            this->maxPoints.template load<AABoundingBox2<T, Packed, Width>::widthImpl>());
    }
};

/**
 * A boundingBox2 object. The boundingBox2 is used to store 2 bounding boxes at a time.
 * @tparam T     Generic type parameter.
 * @tparam Width Type of SIMD being used.
 * @tparam Packed Type of storage used for internal data.
 */
template<typename T, bool Packed = false, SIMDWidth Width = defaultWidthSIMD256<T>>
class AABoundingBox2
{
public:
    using Type = T;
    using Point3D2Def = Point3D2<T, Packed, Point3D2<T, Packed, Width>::widthImpl>;
    using Data = AABoundingBox2Data<T, Packed>;
    template<typename T2 = Type>
    using DataPad = AABoundingBox2DataPad<T, Packed, T2>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Point3D2Def::widthImpl;
    static constexpr bool packed = Packed;
    using SIMD2Def = SIMD2<T, SIMD2<T, widthImpl>::widthImpl>;
    using AABBDef = AABoundingBox<T, AABoundingBox<T, widthImpl>::widthImpl>;

    Point3D2Def minPoints; /**< The min values of each of the 2 bounding boxes */
    Point3D2Def maxPoints; /**< The max values of each of the 2 bounding boxes */

    /** Default constructor. */
    XS_INLINE AABoundingBox2() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE AABoundingBox2(const AABoundingBox2& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE AABoundingBox2(AABoundingBox2&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE AABoundingBox2& operator=(const AABoundingBox2& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE AABoundingBox2& operator=(AABoundingBox2&& other) noexcept = default;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_INLINE explicit AABoundingBox2(const AABoundingBox2<T, Packed, Width2>& other) noexcept
        : minPoints(other.minPoints)
        , maxPoints(other.maxPoints)
    {}

    /**
     * Construct a boundingBox2 in structure of arrays format.
     * @param minValues The new min values for the 2 BBs.
     * @param maxValues The new max values for the 2 BBs.
     */
    XS_INLINE AABoundingBox2(const Point3D2Def& minValues, const Point3D2Def& maxValues) noexcept
        : minPoints(minValues)
        , maxPoints(maxValues)
    {
        XS_ASSERT(minPoints.template getPoint<0>().values <= maxPoints.template getPoint<0>().values);
        XS_ASSERT(minPoints.template getPoint<1>().values <= maxPoints.template getPoint<1>().values);
    }

    /**
     * Construct a boundingBox2 from 2 regular bounding boxes.
     * @param box0 The first bounding box.
     * @param box1 The second bounding box.
     */
    XS_INLINE AABoundingBox2(const AABBDef& box0, const AABBDef& box1) noexcept
        : minPoints(box0.minPoint, box1.minPoint)
        , maxPoints(box0.maxPoint, box1.maxPoint)
    {}

    /**
     * Construct a boundingBox2 from a single regular bounding boxes.
     * @param box The first bounding box.
     */
    XS_INLINE explicit AABoundingBox2(const AABBDef& box) noexcept
        : minPoints(box.minPoint, box.minPoint)
        , maxPoints(box.maxPoint, box.maxPoint)
    {}

    /**
     * Function to build a boundingbox2 with invalid extents.
     * @returns Newly constructed bounding box with required attributes.
     */
    XS_INLINE static AABoundingBox2 Invalid() noexcept
    {
        return AABoundingBox2(Point3D2Def(Limits<Type>::Max), Point3D2Def(Limits<Type>::Lowest));
    }

    /**
     * Extends the original boundingBox2 to incorporate the new points.
     * @param points The bounding points to extend around.
     */
    XS_INLINE void extend(const Point3D2Def& points) noexcept
    {
        this->minPoints = Point3D2Def(this->minPoints.points.min(points.points));
        this->maxPoints = Point3D2Def(this->maxPoints.points.max(points.points));
    }

    /**
     * Extends the original boundingBox2 to incorporate the additional one.
     * @param box The bounding boxes to add to the originals.
     */
    XS_INLINE void extend(const AABoundingBox2& box) noexcept
    {
        this->minPoints = Point3D2Def(this->minPoints.points.min(box.minPoints.points));
        this->maxPoints = Point3D2Def(this->maxPoints.points.max(box.maxPoints.points));
    }

    /**
     * Calculates the Surface Area of a boundingBox2.
     * @returns The surface area of the boxes in floating point.
     */
    XS_INLINE SIMD2Def surfaceArea() const noexcept
    {
        // calculate size of bounding box
        auto size(this->maxPoints.points - this->minPoints.points);
        // calculate Surface Area
        const auto sizeYZX(size.template shuffle3<1, 2, 0>());
        return ((size *= sizeYZX).hadd3() /** SIMDBase( 2.0f )*/);
    }
};
} // namespace Shift
