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
#include "XSLimits.hpp"

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
    XS_INLINE AABoundingBoxData() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE AABoundingBoxData(const AABoundingBoxData& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE AABoundingBoxData(AABoundingBoxData&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE AABoundingBoxData& operator=(const AABoundingBoxData& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE AABoundingBoxData& operator=(AABoundingBoxData&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit AABoundingBoxData(const AABoundingBox<T, Width>& other) noexcept
        : minPoint(other.minPoint)
        , maxPoint(other.maxPoint)
    {}

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const AABoundingBox<T, Width>& other) noexcept
    {
        this->minPoint.store(other.minPoint);
        this->maxPoint.store(other.maxPoint);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD128<T>>
    XS_INLINE AABoundingBox<T, Width> load() const noexcept
    {
        return AABoundingBox<T, Width>(this->minPoint.template load<AABoundingBox<T, Width>::widthImpl>(),
            this->maxPoint.template load<AABoundingBox<T, Width>::widthImpl>());
    }
};

template<typename T, typename T2 = T>
class AABoundingBoxDataPad
{
public:
    Point3DDataPad<T, T2> minPoint;
    Point3DDataPad<T, T2> maxPoint;

    /** Default constructor. */
    XS_INLINE AABoundingBoxDataPad() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE AABoundingBoxDataPad(const AABoundingBoxDataPad& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE AABoundingBoxDataPad(AABoundingBoxDataPad&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE AABoundingBoxDataPad& operator=(const AABoundingBoxDataPad& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE AABoundingBoxDataPad& operator=(AABoundingBoxDataPad&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit AABoundingBoxDataPad(const AABoundingBox<T, Width>& other) noexcept
        : minPoint(other.minPoint)
        , maxPoint(other.maxPoint)
    {}

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const AABoundingBox<T, Width>& other) noexcept
    {
        this->minPoint.store(other.minPoint);
        this->maxPoint.store(other.maxPoint);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD128<T>>
    XS_INLINE AABoundingBox<T, Width> load() const noexcept
    {
        return AABoundingBox<T, Width>(this->minPoint.template load<AABoundingBox<T, Width>::widthImpl>(),
            this->maxPoint.template load<AABoundingBox<T, Width>::widthImpl>());
    }
};

/**
 * A bounding box object. A bounding box is defined by its 3 min and max values for each x, y and z.
 * @tparam T     Generic type parameter.
 * @tparam Width Type of SIMD being used.
 */
template<typename T, SIMDWidth Width = defaultWidthSIMD128<T>>
class AABoundingBox
{
public:
    using Type = T;
    using Point3DDef = Point3D<T, Point3D<T, Width>::widthImpl>;
    using Data = AABoundingBoxData<T>;
    template<typename T2 = Type>
    using DataPad = AABoundingBoxDataPad<T, T2>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Point3DDef::widthImpl;
    using InBaseDef = typename Point3DDef::InBaseDef;

    Point3DDef minPoint;
    Point3DDef maxPoint;

    /** Default constructor. */
    XS_INLINE AABoundingBox() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE AABoundingBox(const AABoundingBox& other) noexcept = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE AABoundingBox(AABoundingBox&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE AABoundingBox& operator=(const AABoundingBox& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE AABoundingBox& operator=(AABoundingBox&& other) noexcept = default;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_INLINE explicit AABoundingBox(const AABoundingBox<T, Width2>& other) noexcept
        : minPoint(other.minPoint)
        , maxPoint(other.maxPoint)
    {}

    /**
     * Construct a bounding box from its member variables.
     * @param minValues The new min values for the BB.
     * @param maxValues The new max values for the BB.
     */
    XS_INLINE AABoundingBox(const Point3DDef& minValues, const Point3DDef& maxValues) noexcept
        : minPoint(minValues)
        , maxPoint(maxValues)
    {}

    /**
     * Construct a bounding box around a single point.
     * @param point The bounding point.
     */
    XS_INLINE explicit AABoundingBox(const Point3DDef& point) noexcept
        : minPoint(point)
        , maxPoint(point)
    {}

    /**
     * Function to build a bounding box with invalid extents.
     * @returns Newly constructed bounding box with required attributes.
     */
    XS_INLINE static AABoundingBox Invalid() noexcept
    {
        return AABoundingBox(Point3DDef(Limits<Type>::Max()), Point3DDef(Limits<Type>::Lowest()));
    }

    /**
     * Extends the original bounding box to incorporate the new point.
     * @param point The bounding point to extend around.
     */
    XS_INLINE void extend(const Point3DDef& point) noexcept
    {
        this->minPoint = Point3DDef(this->minPoint.values.min(point.values));
        this->maxPoint = Point3DDef(this->maxPoint.values.max(point.values));
    }

    /**
     * Extends the original bounding box to incorporate the additional one.
     * @param box The bounding box to add to the original.
     */
    XS_INLINE void extend(const AABoundingBox<T, Width>& box) noexcept
    {
        this->minPoint = Point3DDef(this->minPoint.values.min(box.minPoint.values));
        this->maxPoint = Point3DDef(this->maxPoint.values.max(box.maxPoint.values));
    }

    /**
     * Calculates the Surface Area of a Bounding Box.
     * @returns The surface area of the box in floating point.
     */
    XS_INLINE InBaseDef surfaceArea() const noexcept
    {
        // calculate size of bounding box
        SIMD3<T, Width> size(this->maxPoint.values - this->minPoint.values);
        // calculate Surface Area
        const SIMD3<T, Width> sizeYZX(size.template shuffle<1, 2, 0>());
        return ((size *= sizeYZX).haddInBase() /** SIMDInBase( 2.0f )*/);
    }
};
} // namespace Shift
