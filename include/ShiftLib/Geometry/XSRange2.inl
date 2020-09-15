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

#include "Geometry/XSRange2.hpp"

#include "SIMD/XSSIMD4.inl"
#include "SIMD/XSSIMDBase.inl"
#include "SIMD/XSSIMDInBase.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE Range2Data<T>::Range2Data(const Range2<T, Width>& other) noexcept
{
    store(other);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void Range2Data<T>::store(const Range2<T, Width>& other) noexcept
{
    if constexpr (Width > SIMDWidth::Scalar) {
        this->minMax.store(other.minMax.template negate<true, true, false, false>());
    } else {
        this->minMax.store(other.minMax);
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE Range2<T, Width> Range2Data<T>::load() const noexcept
{
    return Range2<T, Width>(this->minMax.template load<Range2<T, Width>::widthImpl>());
}

template<typename T, SIMDWidth Width>
XS_INLINE Range2<T, Width>::Range2(const SIMD4Def& minMax)
    : minMax(minMax)
{
    if constexpr (Width > SIMDWidth::Scalar) {
        minMax = minMax.template negate<true, true, false, false>();
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE Range2<T, Width>::Range2(const RangeDef& range)
{
    if constexpr (Width > SIMDWidth::Scalar) {
        minMax = SIMD4Def(range.minMax.values).template shuffle<0, 0, 1, 1>();
    } else {
        minMax = range.minMax.template shuffle<0, 0>(), range.minMax.template shuffle<1, 1>();
    }
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE typename Range2<T, Width>::RangeDef Range2<T, Width>::getRange() const
{
    static_assert(Index < 2);
    if constexpr (Width > SIMDWidth::Scalar) {
        RangeDef ret;
        ret.minMax = this->minMax.template shuffle<0 + Index, 2 + Index, 0 + Index,
            2 + Index>(); // Need to avoid re-negating the min values in constructor
        return ret;
    } else {
        return RangeDef(
            SIMD2(this->minMax.template getValueInBase<Index>(), this->minMax.template getValueInBase<2 + Index>()));
    }
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE typename Range2<T, Width>::InBaseDef Range2<T, Width>::getMinInBase() const
{
    static_assert(Index < 2);
    if constexpr (Width > SIMDWidth::Scalar) {
        return this->minMax.template negate<true, true, false, false>().template getValueInBase<Index>();
    } else {
        return this->minMax.template getValueInBase<Index>();
    }
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE typename Range2<T, Width>::InBaseDef Range2<T, Width>::getMaxInBase() const
{
    static_assert(Index < 2);
    return this->minMax.template getValueInBase<Index + 2>();
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE typename Range2<T, Width>::BaseDef Range2<T, Width>::getMin() const
{
    static_assert(Index < 2);
    if constexpr (Width > SIMDWidth::Scalar) {
        return this->minMax.template negate<true, true, false, false>().template getValue<Index>();
    } else {
        return this->minMax.template getValue<Index>();
    }
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE typename Range2<T, Width>::BaseDef Range2<T, Width>::getMax() const
{
    static_assert(Index < 2);
    return this->minMax.template getValue<Index + 2>();
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Range2<T, Width>::SIMD2Def Range2<T, Width>::getLength() const
{
    if constexpr (Width > SIMDWidth::Scalar) {
        return this->minMax.template getValue2<1>() + this->minMax.template getValue2<0>();
    } else {
        return this->minMax.template getValue2<1>() - this->minMax.template getValue2<0>();
    }
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE void Range2<T, Width>::setMin(const BaseDef& min)
{
    static_assert(Index < 2);
    this->minMax.template setValue<Index>(min);
    if constexpr (Width > SIMDWidth::Scalar) {
        this->minMax = this->minMax.template negate<true, true, false, false>();
    }
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE void Range2<T, Width>::setMax(const BaseDef& max)
{
    static_assert(Index < 2);
    this->minMax.template setValue<Index + 2>(max);
}

template<typename T, SIMDWidth Width>
XS_INLINE void Range2<T, Width>::setMin2(const BaseDef& min)
{
    this->minMax.template setDualFloat<0>(XSFloat2(min));
    if constexpr (Width > SIMDWidth::Scalar) {
        this->minMax = this->minMax.template negate<true, true, false, false>();
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE void Range2<T, Width>::setMaxDual(const BaseDef& max)
{
    this->minMax.template setDualFloat<1>(XSFloat2(max));
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Range2<T, Width>::SIMD2Def::Mask Range2<T, Width>::isWithinRange(const BaseDef& value) const
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

template<typename T, SIMDWidth Width>
XS_INLINE typename Range2<T, Width>::SIMD2Def::Mask Range2<T, Width>::isWithinRange(const SIMD2Def& values) const
{
    if constexpr (Width > SIMDWidth::Scalar) {
        const SIMD4Def data4(values.values); // Avoid constructor duplication v2
        const typename SIMD4Def::Mask mask4(
            data4.template shuffle<0, 0, 1, 1>().template negate<true, true, false, false>().lessThanMask(
                this->minMax));
        return mask4.template getMask2<1>() & mask4.template getMask2<0>();
    } else {
        return (this->minMax.template getValue2<0>().lessThanMask(values)) &
            (SIMD2Def(values).lessThanMask(this->minMax.template getValue2<1>()));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE void Range2<T, Width>::clipMin(const Range2& range)
{
    if constexpr (Width > SIMDWidth::Scalar) {
        const SIMD4Def min(this->minMax.min(range.minMax));
        this->minMax.template blend<true, true, false, false>(min);
    } else {
        this->minMax.template setValue2<0>(
            this->minMax.template getValue2<0>().max(range.minMax.template getValue2<0>()));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE void Range2<T, Width>::clipMax(const Range2& range)
{
    if constexpr (Width > SIMDWidth::Scalar) {
        const SIMD4Def max(this->minMax.min(range.minMax));
        this->minMax.template blend<false, false, true, true>(max);
    } else {
        this->minMax.template setValue2<1>(
            this->minMax.template getValue2<1>().min(range.minMax.template getValue2<1>()));
    }
}
} // namespace Shift
