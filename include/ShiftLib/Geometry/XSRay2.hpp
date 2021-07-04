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
#include "Geometry/XSVector3D2.hpp"

// Additional includes
#include "Geometry/XSRay.hpp"
#include "SIMD/XSSIMD2.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class SIMD2;

/**
 * A ray2 object. The ray2 is used to store 2 rays at a time. This is mainly used when optimising for SIMD
 * architectures.
 * @tparam T      Generic type parameter.
 * @tparam Width  Type of SIMD being used.
 * @tparam Packed Type of storage used for internal data.
 */
template<typename T, SIMDWidth Width = widthSIMD<T>, bool Packed = false>
class Ray2
{
public:
    using Type = T;
    using Point3D2Def = Point3D2<T, Point3D2<T, Width, Packed>::widthImpl, Packed>;
    using Vector3D2Def = Vector3D2<T, Vector3D2<T, Width, Packed>::widthImpl, Packed>;
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
    XS_FUNCTION Ray2() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION Ray2(const Ray2& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Ray2(Ray2&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Ray2& operator=(const Ray2& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Ray2& operator=(Ray2&& other) noexcept = default;

    /**
     * Construct a ray2 in structure of arrays format.
     * @param origin    The new origin values for the 2 rays.
     * @param direction The new direction values for the 2 rays.
     */
    XS_FUNCTION Ray2(const Point3D2Def& origin, const Vector3D2Def& direction) noexcept;

    /**
     * Construct a ray2 from 4 regular rays.
     * @param ray0 The first ray.
     * @param ray1 The second ray.
     */
    XS_FUNCTION Ray2(const RayDef& ray0, const RayDef& ray1) noexcept;

    /**
     * Construct a ray2 by duplicating a single ray.
     * @param ray The ray.
     */
    XS_FUNCTION explicit Ray2(const RayDef& ray) noexcept;

    /**
     * Determine a point along a ray2.
     * @param dist The number of units to move along each ray.
     * @returns The Point3D2 at the specified distance along each ray.
     */
    XS_FUNCTION Point3D2Def pointAlongRay(const SIMD2Def& dist) const;
};
} // namespace Shift
