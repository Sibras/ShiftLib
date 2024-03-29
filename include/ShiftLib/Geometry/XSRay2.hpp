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
#include "Geometry/XSRay.hpp"
#include "Geometry/XSVector3D2.hpp"

namespace Shift {
/**
 * A ray2 object. The ray2 is used to store 2 rays at a time. This is mainly used when optimising for SIMD
 * architectures.
 * @tparam T      Generic type parameter.
 * @tparam Width  Type of SIMD being used.
 * @tparam Packed Type of storage used for internal data.
 */
template<typename T, bool Packed = false, SIMDWidth Width = defaultWidthSIMD256<T>>
class Ray2
{
public:
    using Type = T;
    using Point3D2Def = Point3D2<T, Packed, Point3D2<T, Packed, Width>::widthImpl>;
    using Vector3D2Def = Vector3D2<T, Packed, Vector3D2<T, Packed, Width>::widthImpl>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Point3D2Def::widthImpl;
    static constexpr bool packed = Packed;
    using BaseDef = typename Point3D2Def::BaseDef;
    using InBaseDef = typename Point3D2Def::InBaseDef;
    using SIMD2Def = SIMD2<T, SIMD2<T, widthImpl>::widthImpl>;
    using RayDef = Ray<T, Ray<T, widthImpl>::widthImpl>;

    Point3D2Def origins;     /**< The values for the origins of each of the 2 rays. */
    Vector3D2Def directions; /**< The values for the directions of each of the 2 rays. */

    /** Default constructor. */
    XS_INLINE Ray2() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Ray2(const Ray2& other) noexcept = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE Ray2(Ray2&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Ray2& operator=(const Ray2& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Ray2& operator=(Ray2&& other) noexcept = default;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_INLINE explicit Ray2(const Ray2<T, Packed, Width2>& other) noexcept
        : origins(other.origins)
        , directions(other.directions)
    {}

    /**
     * Construct a ray2 in structure of arrays format.
     * @param origin    The new origin values for the 2 rays.
     * @param direction The new direction values for the 2 rays.
     */
    XS_INLINE Ray2(const Point3D2Def& origin, const Vector3D2Def& direction) noexcept
        : origins(origin)
        , directions(direction)
    {}

    /**
     * Construct a ray2 from 4 regular rays.
     * @param ray0 The first ray.
     * @param ray1 The second ray.
     */
    XS_INLINE Ray2(const RayDef& ray0, const RayDef& ray1) noexcept
        : origins(ray0.origin, ray1.origin)
        , directions(ray0.direction, ray1.direction)
    {}

    /**
     * Construct a ray2 by duplicating a single ray.
     * @param ray The ray.
     */
    XS_INLINE explicit Ray2(const RayDef& ray) noexcept
        : origins(ray.origin)
        , directions(ray.direction)
    {}

    /**
     * Determine a point along a ray2.
     * @param dist The number of units to move along each ray.
     * @returns The Point3D2 at the specified distance along each ray.
     */
    XS_INLINE Point3D2Def pointAlongRay(const SIMD2Def& dist) const noexcept
    {
        return this->directions.mad(dist, this->origins);
    }
};
} // namespace Shift
