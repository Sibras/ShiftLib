#pragma once
/**
 * Copyright Matthew Oliver
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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

#include "SIMD/XSSIMD2.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class Range;

template<typename T>
class RangeData
{
public:
    SIMD2Data<T> minMax;

    /** Default constructor. */
    XS_FUNCTION RangeData() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit RangeData(const Range<T, Width>& other) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const Range<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Range<T, Width> load() const noexcept;
};

/** Range type used to store minimum and maximum values for a specific range. */
template<typename T, SIMDWidth Width = widthSIMD<T>>
class Range
{
public:
    using Type = T;
    using SIMD2Def = SIMD2<T, SIMD2<T, Width>::widthImpl>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = SIMD2Def::widthImpl;
    using BaseDef = typename SIMD2Def::BaseDef;
    using InBaseDef = typename SIMD2Def::InBaseDef;

    SIMD2Def minMax;

    /** Default constructor. */
    XS_FUNCTION Range() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION Range(const Range& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Range(Range&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Range& operator=(const Range& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Range& operator=(Range&& other) noexcept = default;

    /**
     * Construct a range from its member variable.
     * @param f2MinMax The minimum and maximum value of the range.
     */
    XS_FUNCTION explicit Range(const SIMD2Def& f2MinMax);

    /**
     * Construct from a minimum and maximum value.
     * @param min The minimum value.
     * @param max The maximum value.
     */
    XS_FUNCTION Range(T min, T max);

    /**
     * Returns the ranges minimum value.
     * @returns The minimum value return as a InBaseDef.
     */
    XS_FUNCTION InBaseDef getMinInBase() const;

    /**
     * Returns the ranges maximum value.
     * @returns The maximum value return as a InBaseDef.
     */
    XS_FUNCTION InBaseDef getMaxInBase() const;

    /**
     * Returns the ranges minimum value.
     * @returns The minimum value return as a SIMDBase.
     */
    XS_FUNCTION BaseDef getMin() const;

    /**
     * Returns the ranges maximum value.
     * @returns The maximum value return as a SIMDBase.
     */
    XS_FUNCTION BaseDef getMax() const;

    /**
     * Gets the length of the range.
     * @returns The length.
     */
    XS_FUNCTION InBaseDef getLength() const;

    /**
     * Sets the minimum value.
     * @param min The new minimum.
     */
    XS_FUNCTION void setMin(const InBaseDef& min);

    /**
     * Sets the maximum value.
     * @param max The new maximum.
     */
    XS_FUNCTION void setMax(const InBaseDef& max);

    /**
     * Check if a value is within the current range.
     * @param value The value to check.
     * @returns If the value is within the range.
     */
    XS_FUNCTION bool isWithinRange(const BaseDef& value) const;

    /**
     * Clips the minimum value to the largest from 2 ranges.
     * @param other The second range to clip to.
     */
    XS_FUNCTION void clipMin(const Range& other);

    /**
     * Clips the maximum value to the smallest from 2 ranges.
     * @param other The second range to clip to.
     */
    XS_FUNCTION void clipMax(const Range& other);
};
} // namespace Shift
