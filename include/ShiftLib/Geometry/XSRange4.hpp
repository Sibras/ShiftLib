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

#include "Geometry/XSRange.hpp"
#include "SIMD/XSSIMD4.hpp"

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
    XS_FUNCTION explicit Range4Data(const Range4<T, Width>& other) noexcept
        : min(other.mins)
        , max(other.maxs)
    {}

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const Range4<T, Width>& other) noexcept
    {
        this->min.store(other.mins);
        this->max.store(other.maxs);
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION Range4<T, Width> load() const noexcept
    {
        return Range4<T, Width>(this->min.template load<Range4<T, Width>::widthImpl>(),
            this->max.template load<Range4<T, Width>::widthImpl>());
    }
};

/** Range4 type used to store minimum and maximum values for four different specific ranges. */
template<typename T, SIMDWidth Width = widthSIMD<T>>
class Range4
{
public:
    using Type = T;
    using SIMD4Def = SIMD4<T, SIMD4<T, Width>::widthImpl>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = SIMD4Def::widthImpl;
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
    XS_FUNCTION Range4(const SIMD4Def& mins, const SIMD4Def& maxs)
        : mins(mins)
        , maxs(maxs)
    {}

    /**
     * Construct a range4 from a single range.
     * @param range Input range to copy.
     */
    XS_FUNCTION explicit Range4(const RangeDef& range)
        : mins(range.getMin())
        , maxs(range.getMax())
    {}

    /**
     * Returns an internal range element.
     * @tparam Index Index of range element to get (must be between 0 and 3).
     * @returns The specified range.
     */
    template<uint32_t Index>
    XS_FUNCTION RangeDef getRange() const noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
        return RangeDef(XSFloat2(this->mins.template getValueInBase<0>(), this->maxs.template getValueInBase<0>()));
    }

    /**
     * Returns the ranges minimum value.
     * @tparam Index Index of range element to get (must be between 0 and 3).
     * @returns The minimum value return as a InBaseDef.
     */
    template<uint32_t Index>
    XS_FUNCTION InBaseDef getMinInBase() const noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
        return this->mins.template getValueInBase<0>();
    }

    /**
     * Returns the ranges maximum value.
     * @tparam Index Index of range element to get (must be between 0 and 3).
     * @returns The maximum value return as a InBaseDef.
     */
    template<uint32_t Index>
    XS_FUNCTION InBaseDef getMaxInBase() const noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
        return this->maxs.template getValueInBase<0>();
    }

    /**
     * Returns the ranges minimum value.
     * @tparam Index Index of range element to get (must be between 0 and 3).
     * @returns The minimum value return as a SIMDBase.
     */
    template<uint32_t Index>
    XS_FUNCTION BaseDef getMin() const noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
        return this->mins.template getValue<Index>();
    }

    /**
     * Returns the ranges maximum value.
     * @tparam Index Index of range element to get (must be between 0 and 3).
     * @returns The maximum value return as a SIMDBase.
     */
    template<uint32_t Index>
    XS_FUNCTION BaseDef getMax() const noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
        return this->maxs.template getValue<Index>();
    }

    /**
     * Gets the length of each range.
     * @returns The length.
     */
    XS_FUNCTION SIMD4Def getLength() const noexcept
    {
        return (this->maxs - this->mins);
    }

    /**
     * Sets the minimum value.
     * @tparam Index The element to modify.
     * @param min The new minimum.
     */
    template<uint32_t Index>
    XS_FUNCTION void setMin(BaseDef min)
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
        this->mins.template setValue<Index>(min);
    }

    /**
     * Sets the maximum value.
     * @tparam Index The element to modify.
     * @param max The new maximum.
     */
    template<uint32_t Index>
    XS_FUNCTION void setMax(BaseDef max)
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
        this->maxs.template setValue<Index + 2>(max);
    }

    /**
     * Sets the minimum value of all range elements.
     * @param min The new minimum.
     */
    XS_FUNCTION void setMin4(BaseDef min)
    {
        this->mins = SIMD4Def(min);
    }

    /**
     * Sets the maximum value of all range elements.
     * @param max The new maximum.
     */
    XS_FUNCTION void setMaxQuad(BaseDef max)
    {
        this->maxs = SIMD4Def(max);
    }

    /**
     * Check if a value is within the current range.
     * @param value The value to check.
     * @returns If the value is within the range.
     */
    XS_FUNCTION typename SIMD4Def::Mask isWithinRange(BaseDef value) const noexcept
    {
        return (this->mins.lessThanMask(value)) & (SIMD4Def(value).lessThanMask(this->maxs));
    }

    /**
     * Check if a values are within the current range.
     * @param values The values to check.
     * @returns If the values are within the range.
     */
    XS_FUNCTION typename SIMD4Def::Mask isWithinRange(const SIMD4Def& values) const noexcept
    {
        return (this->mins.lessThanMask(values)) & (SIMD4Def(values).lessThanMask(this->maxs));
    }

    /**
     * Clips the minimum values to the largest from 2 ranges.
     * @param range The second range to clip to.
     */
    XS_FUNCTION void clipMin(const Range4& range)
    {
        this->mins = this->mins.max(range.mins);
    }

    /**
     * Clips the maximum values to the smallest from 2 ranges.
     * @param range The second range to clip to.
     */
    XS_FUNCTION void clipMax(const Range4& range)
    {
        this->maxs = this->maxs.min(range.maxs);
    }
};
} // namespace Shift
