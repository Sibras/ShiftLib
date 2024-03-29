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
#include "Geometry/XSRay.hpp"
#include "Geometry/XSVector3D4.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class Ray;

/**
 * A ray4 object. The ray4 is used to store 4 rays at a time. This is mainly used when optimising for SIMD
 * architectures.
 * @tparam T      Generic type parameter.
 * @tparam Width  Type of SIMD being used.
 * @tparam Packed Type of storage used for internal data.
 */
template<typename T, bool Packed = false, SIMDWidth Width = defaultWidthSIMD512<T>>
class Ray4
{
public:
    using Type = T;
    using Point3D4Def = Point3D4<T, Packed, Point3D4<T, Packed, Width>::widthImpl>;
    using Vector3D4Def = Vector3D4<T, Packed, Vector3D4<T, Packed, Width>::widthImpl>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Point3D4Def::widthImpl;
    static constexpr bool packed = Packed;
    using BaseDef = typename Point3D4Def::BaseDef;
    using InBaseDef = typename Point3D4Def::InBaseDef;
    using SIMD4Def = SIMD4<T, SIMD4<T, widthImpl>::widthImpl>;
    using RayDef = Ray<T, Ray<T, widthImpl>::widthImpl>;

    Point3D4Def origins;     /**< The values for the origins of each of the 4 rays. */
    Vector3D4Def directions; /**< The values for the directions of each of the 4 rays. */

    /** Default constructor. */
    XS_INLINE Ray4() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Ray4(const Ray4& other) noexcept = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE Ray4(Ray4&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Ray4& operator=(const Ray4& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Ray4& operator=(Ray4&& other) noexcept = default;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_INLINE explicit Ray4(const Ray4<T, Packed, Width2>& other) noexcept
        : origins(other.origins)
        , directions(other.directions)
    {}

    /**
     * Construct a ray4 in structure of arrays format.
     * @param origins    The new origin values for the 4 rays.
     * @param directions The new direction values for the 4 rays.
     */
    XS_INLINE Ray4(const Point3D4Def& origins, const Vector3D4Def& directions) noexcept
        : origins(origins)
        , directions(directions)
    {}

    /**
     * Construct a ray4 from 4 regular rays.
     * @param ray0 The first ray.
     * @param ray1 The second ray.
     * @param ray2 The third ray.
     * @param ray3 The fourth ray.
     */
    XS_INLINE Ray4(const RayDef& ray0, const RayDef& ray1, const RayDef& ray2, const RayDef& ray3) noexcept
        : origins(ray0.origin, ray1.origin, ray2.origin, ray3.origin)
        , directions(ray0.direction, ray1.direction, ray2.direction, ray3.direction)
    {}

    /**
     * Construct a ray4 by duplicating a single ray.
     * @param ray The ray.
     */
    XS_INLINE Ray4(const RayDef& ray) noexcept
        : origins(ray.origin)
        , directions(ray.direction)
    {}

    /**
     * Determine a point along a ray4.
     * @param dist The number of units to move along each ray.
     * @returns The Point3D4 at the specified distance along each ray.
     */
    XS_INLINE Point3D4Def pointAlongRay(const SIMD4Def& dist) const noexcept
    {
        return this->directions.mad(dist, this->origins);
    }
};
} // namespace Shift
