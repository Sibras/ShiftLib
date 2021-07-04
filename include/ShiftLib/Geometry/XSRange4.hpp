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

#include "SIMD/XSSIMD4.hpp"

// Additional includes
#include "Geometry/XSRange.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class Range4;

template<typename T>
class Range4Data
{
public:
    SIMD4Data<T> min;
    SIMD4Data<T> max;

    /** Default constructor. */
    XS_FUNCTION Range4Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit Range4Data(const Range4<T, Width>& other) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const Range4<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Range4<T, Width> load() const noexcept;
};

/** Range4 type used to store minimum and maximum values for four different specific ranges. */
template<typename T, SIMDWidth Width = widthSIMD<T>>
class Range4
{
public:
    using Type = T;
    using SIMD4Def = SIMD4<T, SIMD4<T, Width>::widthImpl>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = typename SIMD4Def::widthImpl;
    using BaseDef = typename SIMD4Def::BaseDef;
    using InBaseDef = typename SIMD4Def::InBaseDef;
    using RangeDef = Range<T, Range<T, widthImpl>::widthImpl>;

    SIMD4Def mins;
    SIMD4Def maxs;

    /** Default constructor. */
    XS_FUNCTION Range4() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION Range4(const Range4& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Range4(Range4&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Range4& operator=(const Range4& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Range4& operator=(Range4&& other) noexcept = default;

    /**
     * Construct a range4 from its member variables.
     * @param mins The minimum 4 values of the range.
     * @param maxs The maximum 4 values of the range.
     */
    XS_FUNCTION Range4(const SIMD4Def& mins, const SIMD4Def& maxs);

    /**
     * Construct a range4 from a single range.
     * @param range Input range to copy.
     */
    XS_FUNCTION explicit Range4(const RangeDef& range);

    /**
     * Returns an internal range element.
     * @tparam Index Index of range element to get (must be between 0 and 3).
     * @returns The specified range.
     */
    template<uint32_t Index>
    XS_FUNCTION RangeDef getRange() const;

    /**
     * Returns the ranges minimum value.
     * @tparam Index Index of range element to get (must be between 0 and 3).
     * @returns The minimum value return as a InBaseDef.
     */
    template<uint32_t Index>
    XS_FUNCTION InBaseDef getMinInBase() const;

    /**
     * Returns the ranges maximum value.
     * @tparam Index Index of range element to get (must be between 0 and 3).
     * @returns The maximum value return as a InBaseDef.
     */
    template<uint32_t Index>
    XS_FUNCTION InBaseDef getMaxInBase() const;

    /**
     * Returns the ranges minimum value.
     * @tparam Index Index of range element to get (must be between 0 and 3).
     * @returns The minimum value return as a SIMDBase.
     */
    template<uint32_t Index>
    XS_FUNCTION BaseDef getMin() const;

    /**
     * Returns the ranges maximum value.
     * @tparam Index Index of range element to get (must be between 0 and 3).
     * @returns The maximum value return as a SIMDBase.
     */
    template<uint32_t Index>
    XS_FUNCTION BaseDef getMax() const;

    /**
     * Gets the length of each range.
     * @returns The length.
     */
    XS_FUNCTION SIMD4Def getLength() const;

    /**
     * Sets the minimum value.
     * @tparam Index The element to modify.
     * @param min The new minimum.
     */
    template<uint32_t Index>
    XS_FUNCTION void setMin(const BaseDef& min);

    /**
     * Sets the maximum value.
     * @tparam Index The element to modify.
     * @param max The new maximum.
     */
    template<uint32_t Index>
    XS_FUNCTION void setMax(const BaseDef& max);

    /**
     * Sets the minimum value of all range elements.
     * @param min The new minimum.
     */
    XS_FUNCTION void setMin4(const BaseDef& min);

    /**
     * Sets the maximum value of all range elements.
     * @param max The new maximum.
     */
    XS_FUNCTION void setMaxQuad(const BaseDef& max);

    /**
     * Check if a value is within the current range.
     * @param value The value to check.
     * @returns If the value is within the range.
     */
    XS_FUNCTION typename SIMD4Def::Mask isWithinRange(const BaseDef& value) const;

    /**
     * Check if a values are within the current range.
     * @param values The values to check.
     * @returns If the values are within the range.
     */
    XS_FUNCTION typename SIMD4Def::Mask isWithinRange(const SIMD4Def& values) const;

    /**
     * Clips the minimum values to the largest from 2 ranges.
     * @param range The second range to clip to.
     */
    XS_FUNCTION void clipMin(const Range4& range);

    /**
     * Clips the maximum values to the smallest from 2 ranges.
     * @param range The second range to clip to.
     */
    XS_FUNCTION void clipMax(const Range4& range);
};
} // namespace Shift
