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
class Range2;

template<typename T>
class Range2Data
{
public:
    SIMD4Data<T> minMax;

    /** Default constructor. */
    XS_INLINE Range2Data() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Range2Data(const Range2Data& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE Range2Data(Range2Data&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Range2Data& operator=(const Range2Data& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Range2Data& operator=(Range2Data&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit Range2Data(const Range2<T, Width>& other) noexcept
    {
        store(other);
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const Range2<T, Width>& other) noexcept
    {
        if constexpr (Width > SIMDWidth::Scalar) {
            this->minMax.store(other.minMax.template negate<true, true, false, false>());
        } else {
            this->minMax.store(other.minMax);
        }
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD<T>>
    XS_INLINE Range2<T, Width> load() const noexcept
    {
        return Range2<T, Width>(this->minMax.template load<Range2<T, Width>::widthImpl>());
    }
};

template<typename T>
using Range2DataPad = Range2Data<T>;

/** Range2 type used to store minimum and maximum values for two different specific ranges. */
template<typename T, SIMDWidth Width = defaultWidthSIMD<T>>
class Range2
{
public:
    using Type = T;
    using SIMD4Def = SIMD4<T, SIMD4<T, Width>::widthImpl>;
    using Data = Range2Data<T>;
    using DataPad = Range2DataPad<T>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = SIMD4Def::widthImpl;
    using BaseDef = typename SIMD4Def::BaseDef;
    using InBaseDef = typename SIMD4Def::InBaseDef;
    using SIMD2Def = typename SIMD4Def::SIMD2Def;
    using RangeDef = Range<T, Range<T, widthImpl>::widthImpl>;

    SIMD4Def minMax; /**< The 2 min values followed by the 2 max values. */

    /** Default constructor. */
    XS_INLINE Range2() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Range2(const Range2& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE Range2(Range2&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Range2& operator=(const Range2& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Range2& operator=(Range2&& other) noexcept = default;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_INLINE explicit Range2(const Range2<T, Width2>& other) noexcept
        : minMax(other.minMax)
    {}

    /**
     * Construct a range2 from its member variables.
     * @param minMax The minimum 2 values followed by the maximum 2 values of the range.
     */
    XS_INLINE explicit Range2(const SIMD4Def& minMax)
        : minMax(minMax)
    {
        if constexpr (Width > SIMDWidth::Scalar) {
            this->minMax = this->minMax.template negate<true, true, false, false>();
        }
    }

    /**
     * Construct a range2 from a single range.
     * @param range Input range to copy.
     */
    XS_INLINE explicit Range2(const RangeDef& range)
    {
        if constexpr (Width > SIMDWidth::Scalar) {
            minMax = SIMD4Def(range.minMax.values).template shuffle<0, 0, 1, 1>();
        } else {
            minMax = range.minMax.template shuffle<0, 0>(), range.minMax.template shuffle<1, 1>();
        }
    }

    /**
     * Returns an internal range element.
     * @tparam Index Index of range element to get (must be between 0 and 1).
     * @returns The specified range.
     */
    template<uint32_t Index>
    XS_INLINE RangeDef getRange() const noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
        if constexpr (Width > SIMDWidth::Scalar) {
            RangeDef ret;
            ret.minMax = this->minMax.template shuffle<0 + Index, 2 + Index, 0 + Index,
                2 + Index>(); // Need to avoid re-negating the min values in constructor
            return ret;
        } else {
            return RangeDef(SIMD2(
                this->minMax.template getValueInBase<Index>(), this->minMax.template getValueInBase<2 + Index>()));
        }
    }

    /**
     * Returns the ranges minimum value.
     * @tparam Index Index of range element to get (must be between 0 and 1).
     * @returns The minimum value return as a InBaseDef.
     */
    template<uint32_t Index>
    XS_INLINE InBaseDef getMinInBase() const noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
        if constexpr (Width > SIMDWidth::Scalar) {
            return this->minMax.template negate<true, true, false, false>().template getValueInBase<Index>();
        } else {
            return this->minMax.template getValueInBase<Index>();
        }
    }

    /**
     * Returns the ranges maximum value.
     * @tparam Index Index of range element to get (must be between 0 and 1).
     * @returns The maximum value return as a InBaseDef.
     */
    template<uint32_t Index>
    XS_INLINE InBaseDef getMaxInBase() const noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
        return this->minMax.template getValueInBase<Index + 2>();
    }

    /**
     * Returns the ranges minimum value.
     * @tparam Index Index of range element to get (must be between 0 and 1).
     * @returns The minimum value return as a SIMDBase.
     */
    template<uint32_t Index>
    XS_INLINE BaseDef getMin() const noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
        if constexpr (Width > SIMDWidth::Scalar) {
            return this->minMax.template negate<true, true, false, false>().template getValue<Index>();
        } else {
            return this->minMax.template getValue<Index>();
        }
    }

    /**
     * Returns the ranges maximum value.
     * @tparam Index Index of range element to get (must be between 0 and 1).
     * @returns The maximum value return as a SIMDBase.
     */
    template<uint32_t Index>
    XS_INLINE BaseDef getMax() const noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
        return this->minMax.template getValue<Index + 2>();
    }

    /**
     * Gets the length of each range.
     * @returns The length.
     */
    XS_INLINE SIMD2Def getLength() const noexcept
    {
        if constexpr (Width > SIMDWidth::Scalar) {
            return this->minMax.template getValue2<1>() + this->minMax.template getValue2<0>();
        } else {
            return this->minMax.template getValue2<1>() - this->minMax.template getValue2<0>();
        }
    }

    /**
     * Sets the minimum value.
     * @tparam Index The element to modify.
     * @param min The new minimum.
     */
    template<uint32_t Index>
    XS_INLINE void setMin(BaseDef min)
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
        this->minMax.template setValue<Index>(min);
        if constexpr (Width > SIMDWidth::Scalar) {
            this->minMax = this->minMax.template negate<true, true, false, false>();
        }
    }

    /**
     * Sets the maximum value.
     * @tparam Index The element to modify.
     * @param max The new maximum.
     */
    template<uint32_t Index>
    XS_INLINE void setMax(BaseDef max)
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
        this->minMax.template setValue<Index + 2>(max);
    }

    /**
     * Sets the minimum value of all range elements.
     * @param min The new minimum.
     */
    XS_INLINE void setMin2(BaseDef min)
    {
        this->minMax.template setDualFloat<0>(XSFloat2(min));
        if constexpr (Width > SIMDWidth::Scalar) {
            this->minMax = this->minMax.template negate<true, true, false, false>();
        }
    }

    /**
     * Sets the maximum value of all range elements.
     * @param max The new maximum.
     */
    XS_INLINE void setMaxDual(BaseDef max)
    {
        this->minMax.template setDualFloat<1>(XSFloat2(max));
    }

    /**
     * Check if a value is within the current range.
     * @param value The value to check.
     * @returns If the value is within the range.
     */
    XS_INLINE typename SIMD2Def::Mask isWithinRange(BaseDef value) const noexcept
    {
        if constexpr (Width > SIMDWidth::Scalar) {
            const typename SIMD4Def::Mask mask4(
                SIMD4Def(value).template negate<true, true, false, false>().lessThanMask(this->minMax));
            return mask4.template getMask2<1>() & mask4.template getMask2<0>();
        } else {
            return (this->minMax.template getValue2<0>().lessThanMask(value)) &
                (SIMD2Def(value).lessThanMask(this->minMax.template getValue2<1>()));
        }
    }

    /**
     * Check if a values are within the current range.
     * @param values The values to check.
     * @returns If the values are within the range.
     */
    XS_INLINE typename SIMD2Def::Mask isWithinRange(const SIMD2Def& values) const noexcept
    {
        if constexpr (Width > SIMDWidth::Scalar) {
            const SIMD4Def data4(values); // Avoid constructor duplication v2
            const typename SIMD4Def::Mask mask4(
                data4.template shuffle<0, 0, 1, 1>().template negate<true, true, false, false>().lessThanMask(
                    this->minMax));
            return mask4.template getMask2<1>() & mask4.template getMask2<0>();
        } else {
            return (this->minMax.template getValue2<0>().lessThanMask(values)) &
                (values.lessThanMask(this->minMax.template getValue2<1>()));
        }
    }

    /**
     * Clips the minimum values to the largest from 2 ranges.
     * @param range The second range to clip to.
     */
    XS_INLINE void clipMin(const Range2& range)
    {
        if constexpr (Width > SIMDWidth::Scalar) {
            const SIMD4Def min(this->minMax.min(range.minMax));
            this->minMax.template blend<true, true, false, false>(min);
        } else {
            this->minMax.template setValue2<0>(
                this->minMax.template getValue2<0>().max(range.minMax.template getValue2<0>()));
        }
    }

    /**
     * Clips the maximum values to the smallest from 2 ranges.
     * @param range The second range to clip to.
     */
    XS_INLINE void clipMax(const Range2& range)
    {
        if constexpr (Width > SIMDWidth::Scalar) {
            const SIMD4Def max(this->minMax.min(range.minMax));
            this->minMax.template blend<false, false, true, true>(max);
        } else {
            this->minMax.template setValue2<1>(
                this->minMax.template getValue2<1>().min(range.minMax.template getValue2<1>()));
        }
    }
};
} // namespace Shift
