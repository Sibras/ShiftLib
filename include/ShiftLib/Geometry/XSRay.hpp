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
#include "Geometry/XSVector3D.hpp"

namespace Shift {
/**
 * A ray object. The ray object defines a rays origin and its direction.
 * @tparam T     Generic type parameter.
 * @tparam Width Type of SIMD being used.
 */
template<typename T, SIMDWidth Width = defaultWidthSIMD128<T>>
class Ray
{
public:
    using Type = T;
    using Point3DDef = Point3D<T, Point3D<T, Width>::widthImpl>;
    using Vector3DDef = Vector3D<T, Vector3D<T, Width>::widthImpl>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Point3DDef::widthImpl;
    using InBaseDef = typename Point3DDef::InBaseDef;

    Point3DDef origin;     /**< The rays starting point or origin. */
    Vector3DDef direction; /**< The direction the ray is travelling. */

    /** Default constructor. */
    XS_INLINE Ray() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Ray(const Ray& other) noexcept = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE Ray(Ray&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Ray& operator=(const Ray& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Ray& operator=(Ray&& other) noexcept = default;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_INLINE explicit Ray(const Ray<T, Width2>& other) noexcept
        : origin(other.origin)
        , direction(other.direction)
    {}

    /**
     * Construct a ray from its member variables.
     * @param origin    The point to use as the rays origin.
     * @param direction The vector to use as the rays direction.
     */
    XS_INLINE Ray(const Point3DDef& origin, const Vector3DDef& direction) noexcept
        : origin(origin)
        , direction(direction)
    {}

    /**
     * Determine a point along a ray.
     * @param dist The number of units to move along the ray.
     * @returns The 3-D point at the specified distance along the ray.
     */
    XS_INLINE Point3DDef pointAlongRay(InBaseDef dist) const noexcept
    {
        return this->direction.mad(dist, this->origin);
    }
};
} // namespace Shift
