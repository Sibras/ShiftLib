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
class Range2;

template<typename T>
class Range2Data
{
public:
    SIMD4Data<T> minMax;

    /** Default constructor. */
    XS_FUNCTION Range2Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit Range2Data(const Range2<T, Width>& other) noexcept;

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const Range2<T, Width>& other) noexcept;

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Range2<T, Width> load() const noexcept;
};

/** Range2 type used to store minimum and maximum values for two different specific ranges. */
template<typename T, SIMDWidth Width = widthSIMD<T>>
class Range2
{
public:
    using Type = T;
    using SIMD4Def = SIMD4<T, SIMD4<T, Width>::widthImpl>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = SIMD4Def::widthImpl;
    using BaseDef = typename SIMD4Def::BaseDef;
    using InBaseDef = typename SIMD4Def::InBaseDef;
    using SIMD2Def = typename SIMD4Def::SIMD2Def;
    using RangeDef = Range<T, Range<T, widthImpl>::widthImpl>;

    SIMD4Def minMax; /**< The 2 min values followed by the 2 max values. */

    /** Default constructor. */
    XS_FUNCTION Range2() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION Range2(const Range2& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION Range2(Range2&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Range2& operator=(const Range2& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION Range2& operator=(Range2&& other) noexcept = default;

    /**
     * Construct a range2 from its member variables.
     * @param minMax The minimum 2 values followed by the maximum 2 values of the range.
     */
    XS_FUNCTION explicit Range2(const SIMD4Def& minMax);

    /**
     * Construct a range2 from a single range.
     * @param range Input range to copy.
     */
    XS_FUNCTION explicit Range2(const RangeDef& range);

    /**
     * Returns an internal range element.
     * @tparam Index Index of range element to get (must be between 0 and 1).
     * @returns The specified range.
     */
    template<uint32_t Index>
    XS_FUNCTION RangeDef getRange() const noexcept;

    /**
     * Returns the ranges minimum value.
     * @tparam Index Index of range element to get (must be between 0 and 1).
     * @returns The minimum value return as a InBaseDef.
     */
    template<uint32_t Index>
    XS_FUNCTION InBaseDef getMinInBase() const noexcept;

    /**
     * Returns the ranges maximum value.
     * @tparam Index Index of range element to get (must be between 0 and 1).
     * @returns The maximum value return as a InBaseDef.
     */
    template<uint32_t Index>
    XS_FUNCTION InBaseDef getMaxInBase() const noexcept;

    /**
     * Returns the ranges minimum value.
     * @tparam Index Index of range element to get (must be between 0 and 1).
     * @returns The minimum value return as a SIMDBase.
     */
    template<uint32_t Index>
    XS_FUNCTION BaseDef getMin() const noexcept;

    /**
     * Returns the ranges maximum value.
     * @tparam Index Index of range element to get (must be between 0 and 1).
     * @returns The maximum value return as a SIMDBase.
     */
    template<uint32_t Index>
    XS_FUNCTION BaseDef getMax() const noexcept;

    /**
     * Gets the length of each range.
     * @returns The length.
     */
    XS_FUNCTION SIMD2Def getLength() const noexcept;

    /**
     * Sets the minimum value.
     * @tparam Index The element to modify.
     * @param min The new minimum.
     */
    template<uint32_t Index>
    XS_FUNCTION void setMin(BaseDef min);

    /**
     * Sets the maximum value.
     * @tparam Index The element to modify.
     * @param max The new maximum.
     */
    template<uint32_t Index>
    XS_FUNCTION void setMax(BaseDef max);

    /**
     * Sets the minimum value of all range elements.
     * @param min The new minimum.
     */
    XS_FUNCTION void setMin2(BaseDef min);

    /**
     * Sets the maximum value of all range elements.
     * @param max The new maximum.
     */
    XS_FUNCTION void setMaxDual(BaseDef max);

    /**
     * Check if a value is within the current range.
     * @param value The value to check.
     * @returns If the value is within the range.
     */
    XS_FUNCTION typename SIMD2Def::Mask isWithinRange(BaseDef value) const noexcept;

    /**
     * Check if a values are within the current range.
     * @param values The values to check.
     * @returns If the values are within the range.
     */
    XS_FUNCTION typename SIMD2Def::Mask isWithinRange(const SIMD2Def& values) const noexcept;

    /**
     * Clips the minimum values to the largest from 2 ranges.
     * @param range The second range to clip to.
     */
    XS_FUNCTION void clipMin(const Range2& range);

    /**
     * Clips the maximum values to the smallest from 2 ranges.
     * @param range The second range to clip to.
     */
    XS_FUNCTION void clipMax(const Range2& range);
};
} // namespace Shift
