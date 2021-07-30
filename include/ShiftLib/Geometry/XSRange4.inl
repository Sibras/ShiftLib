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

#include "Geometry/XSRange4.hpp"

#include "SIMD/XSSIMD4.inl"
#include "SIMD/XSSIMDBase.inl"
#include "SIMD/XSSIMDInBase.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE Range4Data<T>::Range4Data(const Range4<T, Width>& other) noexcept
    : min(other.mins)
    , max(other.maxs)
{}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void Range4Data<T>::store(const Range4<T, Width>& other) noexcept
{
    this->min.store(other.mins);
    this->max.store(other.maxs);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE Range4<T, Width> Range4Data<T>::load() const noexcept
{
    return Range4<T, Width>(
        this->min.template load<Range4<T, Width>::widthImpl>(), this->max.template load<Range4<T, Width>::widthImpl>());
}

template<typename T, SIMDWidth Width>
XS_INLINE Range4<T, Width>::Range4(const SIMD4Def& mins, const SIMD4Def& maxs)
    : mins(mins)
    , maxs(maxs)
{}

template<typename T, SIMDWidth Width>
XS_INLINE Range4<T, Width>::Range4(const RangeDef& range)
    : mins(range.getMin())
    , maxs(range.getMax())
{}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE typename Range4<T, Width>::RangeDef Range4<T, Width>::getRange() const
{
    static_assert(Index < 4);
    return RangeDef(XSFloat2(this->mins.template getValueInBase<0>(), this->maxs.template getValueInBase<0>()));
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE typename Range4<T, Width>::InBaseDef Range4<T, Width>::getMinInBase() const
{
    static_assert(Index < 4);
    return this->mins.template getValueInBase<0>();
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE typename Range4<T, Width>::InBaseDef Range4<T, Width>::getMaxInBase() const
{
    static_assert(Index < 4);
    return this->maxs.template getValueInBase<0>();
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE typename Range4<T, Width>::BaseDef Range4<T, Width>::getMin() const
{
    static_assert(Index < 4);
    return this->mins.template getValue<Index>();
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE typename Range4<T, Width>::BaseDef Range4<T, Width>::getMax() const
{
    static_assert(Index < 4);
    return this->maxs.template getValue<Index>();
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Range4<T, Width>::SIMD4Def Range4<T, Width>::getLength() const
{
    return (this->maxs - this->mins);
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE void Range4<T, Width>::setMin(BaseDef min)
{
    static_assert(Index < 4);
    this->mins.template setValue<Index>(min);
}

template<typename T, SIMDWidth Width>
template<uint32_t Index>
XS_INLINE void Range4<T, Width>::setMax(BaseDef max)
{
    static_assert(Index < 4);
    this->maxs.template setValue<Index + 2>(max);
}

template<typename T, SIMDWidth Width>
XS_INLINE void Range4<T, Width>::setMin4(BaseDef min)
{
    this->mins = SIMD4Def(min);
}

template<typename T, SIMDWidth Width>
XS_INLINE void Range4<T, Width>::setMaxQuad(BaseDef max)
{
    this->maxs = SIMD4Def(max);
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Range4<T, Width>::SIMD4Def::Mask Range4<T, Width>::isWithinRange(BaseDef value) const
{
    return (this->mins.lessThanMask(value)) & (SIMD4Def(value).lessThanMask(this->maxs));
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Range4<T, Width>::SIMD4Def::Mask Range4<T, Width>::isWithinRange(const SIMD4Def& values) const
{
    return (this->mins.lessThanMask(values)) & (SIMD4Def(values).lessThanMask(this->maxs));
}

template<typename T, SIMDWidth Width>
XS_INLINE void Range4<T, Width>::clipMin(const Range4& range)
{
    this->mins = this->mins.max(range.mins);
}

template<typename T, SIMDWidth Width>
XS_INLINE void Range4<T, Width>::clipMax(const Range4& range)
{
    this->maxs = this->maxs.min(range.maxs);
}
} // namespace Shift
