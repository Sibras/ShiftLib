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
    XS_INLINE RangeData() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit RangeData(const Range<T, Width>& other) noexcept
    {
        store(other);
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const Range<T, Width>& other) noexcept
    {
        if constexpr (Width > SIMDWidth::Scalar) {
            this->minMax.store(other.minMax.template negate<true, false>());
        } else {
            this->minMax.store(other.minMax);
        }
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_INLINE Range<T, Width> load() const noexcept
    {
        return Range<T, Width>(this->minMax.template load<Range<T, Width>::widthImpl>());
    }
};

template<typename T>
using RangeDataPad = RangeData<T>;

/** Range type used to store minimum and maximum values for a specific range. */
template<typename T, SIMDWidth Width = widthSIMD<T>>
class Range
{
public:
    using Type = T;
    using SIMD2Def = SIMD2<T, SIMD2<T, Width>::widthImpl>;
    using Data = RangeData<T>;
    using DataPad = RangeDataPad<T>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = SIMD2Def::widthImpl;
    using BaseDef = typename SIMD2Def::BaseDef;
    using InBaseDef = typename SIMD2Def::InBaseDef;

    SIMD2Def minMax;

    /** Default constructor. */
    XS_INLINE Range() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE Range(const Range& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE Range(Range&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Range& operator=(const Range& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE Range& operator=(Range&& other) noexcept = default;

    /**
     * Construct a range from its member variable.
     * @param f2MinMax The minimum and maximum value of the range.
     */
    XS_INLINE explicit Range(const SIMD2Def& f2MinMax)
        : minMax(f2MinMax)
    {
        if constexpr (Width > SIMDWidth::Scalar) {
            minMax = minMax.template negate<true, false>();
        }
    }

    /**
     * Construct from a minimum and maximum value.
     * @param min The minimum value.
     * @param max The maximum value.
     */
    XS_INLINE Range(T min, T max)
    {
        if constexpr (Width > SIMDWidth::Scalar) {
            this->minMax = SIMD2<T>(-min, max);
        } else {
            this->minMax = SIMD2<T>(min, max);
        }
    }

    /**
     * Returns the ranges minimum value.
     * @returns The minimum value return as a InBaseDef.
     */
    XS_INLINE InBaseDef getMinInBase() const noexcept
    {
        if constexpr (Width > SIMDWidth::Scalar) {
            return this->minMax.template negate<true, false>().template getValueInBase<0>();
        } else {
            return this->minMax.template getValueInBase<0>();
        }
    }

    /**
     * Returns the ranges maximum value.
     * @returns The maximum value return as a InBaseDef.
     */
    XS_INLINE InBaseDef getMaxInBase() const noexcept
    {
        return minMax.template getValueInBase<1>();
    }

    /**
     * Returns the ranges minimum value.
     * @returns The minimum value return as a SIMDBase.
     */
    XS_INLINE BaseDef getMin() const noexcept
    {
        if constexpr (Width > SIMDWidth::Scalar) {
            return this->minMax.template negate<true, false>().template getValue<0>();
        } else {
            return this->minMax.template getValue<0>();
        }
    }

    /**
     * Returns the ranges maximum value.
     * @returns The maximum value return as a SIMDBase.
     */
    XS_INLINE BaseDef getMax() const noexcept
    {
        return minMax.template getValue<1>();
    }

    /**
     * Gets the length of the range.
     * @returns The length.
     */
    XS_INLINE InBaseDef getLength() const noexcept
    {
        if constexpr (Width > SIMDWidth::Scalar) {
            return this->minMax.haddInBase();
        } else {
            return this->minMax.hsubInBase();
        }
    }

    /**
     * Sets the minimum value.
     * @param min The new minimum.
     */
    XS_INLINE void setMin(InBaseDef min)
    {
        this->minMax.template setValue<0>(min);
        if constexpr (Width > SIMDWidth::Scalar) {
            this->minMax = this->minMax.template negate<true, false>();
        }
    }

    /**
     * Sets the minimum value.
     * @param min The new minimum.
     */
    XS_INLINE void setMin(BaseDef min)
    {
        this->minMax.template setValue<0>(min);
        if constexpr (Width > SIMDWidth::Scalar) {
            this->minMax = this->minMax.template negate<true, false>();
        }
    }

    /**
     * Sets the maximum value.
     * @param max The new maximum.
     */
    XS_INLINE void setMax(InBaseDef max)
    {
        this->minMax.template setValue<1>(max);
    }

    /**
     * Sets the maximum value.
     * @param max The new maximum.
     */
    XS_INLINE void setMax(BaseDef max)
    {
        this->minMax.template setValue<1>(max);
    }

    /**
     * Check if a value is within the current range.
     * @param value The value to check.
     * @returns If the value is within the range.
     */
    XS_INLINE bool isWithinRange(BaseDef value) const noexcept
    {
        if constexpr (Width > SIMDWidth::Scalar) {
            return this->minMax.lessThanMask(SIMD2Def(value).template negate<true, false>()).getNone();
        } else {
            return (this->minMax.template getValue<0>() < value) & (value < this->minMax.template getValue<1>());
        }
    }

    /**
     * Clips the minimum value to the largest from 2 ranges.
     * @param other The second range to clip to.
     */
    XS_INLINE void clipMin(const Range& other)
    {
        if constexpr (Width > SIMDWidth::Scalar) {
            this->minMax.template blend<true, false>(this->minMax.min(other.minMax));
        } else {
            this->minMax.template setValue<0>(
                this->minMax.template getValue<0>().max(other.minMax.template getValue<0>()));
        }
    }

    /**
     * Clips the maximum value to the smallest from 2 ranges.
     * @param other The second range to clip to.
     */
    XS_INLINE void clipMax(const Range& other)
    {
        if constexpr (Width > SIMDWidth::Scalar) {
            this->minMax.template blend<false, true>(this->minMax.min(other.minMax));
        } else {
            this->minMax.template setValue<1>(
                this->minMax.template getValue<1>().min(other.minMax.template getValue<1>()));
        }
    }
};
} // namespace Shift
