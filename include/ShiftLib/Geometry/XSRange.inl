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

#include "SIMD/XSSIMD2.hpp"
#include "SIMD/XSSIMDBase.hpp"
#include "SIMD/XSSIMDInBase.hpp"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE RangeData<T>::RangeData(const Range<T, Width>& other) noexcept
{
    store(other);
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void RangeData<T>::store(const Range<T, Width>& other) noexcept
{
    if constexpr (Width > SIMDWidth::Scalar) {
        this->minMax.store(other.minMax.template negate<true, false>());
    } else {
        this->minMax.store(other.minMax);
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE Range<T, Width> RangeData<T>::load() const noexcept
{
    return Range<T, Width>(this->minMax.template load<Range<T, Width>::widthImpl>());
}

template<typename T, SIMDWidth Width>
XS_INLINE Range<T, Width>::Range(const SIMD2Def& f2MinMax)
    : minMax(f2MinMax)
{
    if constexpr (Width > SIMDWidth::Scalar) {
        minMax = minMax.template negate<true, false>();
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE Range<T, Width>::Range(T min, T max)
{
    if constexpr (Width > SIMDWidth::Scalar) {
        this->minMax = SIMD2<T>(-min, max);
    } else {
        this->minMax = SIMD2<T>(min, max);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Range<T, Width>::InBaseDef Range<T, Width>::getMinInBase() const noexcept
{
    if constexpr (Width > SIMDWidth::Scalar) {
        return this->minMax.template negate<true, false>().template getValueInBase<0>();
    } else {
        return this->minMax.template getValueInBase<0>();
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Range<T, Width>::InBaseDef Range<T, Width>::getMaxInBase() const noexcept
{
    return minMax.template getValueInBase<1>();
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Range<T, Width>::BaseDef Range<T, Width>::getMin() const noexcept
{
    if constexpr (Width > SIMDWidth::Scalar) {
        return this->minMax.template negate<true, false>().template getValue<0>();
    } else {
        return this->minMax.template getValue<0>();
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Range<T, Width>::BaseDef Range<T, Width>::getMax() const noexcept
{
    return minMax.template getValue<1>();
}

template<typename T, SIMDWidth Width>
XS_INLINE typename Range<T, Width>::InBaseDef Range<T, Width>::getLength() const noexcept
{
    if constexpr (Width > SIMDWidth::Scalar) {
        return this->minMax.haddInBase();
    } else {
        return this->minMax.hsubInBase();
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE void Range<T, Width>::setMin(const InBaseDef min)
{
    this->minMax.template setValue<0>(min);
    if constexpr (Width > SIMDWidth::Scalar) {
        this->minMax = this->minMax.template negate<true, false>();
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE void Range<T, Width>::setMax(const InBaseDef max)
{
    this->minMax.template setValue<1>(max);
}

template<typename T, SIMDWidth Width>
XS_INLINE bool Range<T, Width>::isWithinRange(const BaseDef value) const noexcept
{
    if constexpr (Width > SIMDWidth::Scalar) {
        return this->minMax.lessThanMask(SIMD2Def(value).template negate<true, false>()).getNone();
    } else {
        return (this->minMax.template getValue<0>() < value) & (value < this->minMax.template getValue<1>());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE void Range<T, Width>::clipMin(const Range& other)
{
    if constexpr (Width > SIMDWidth::Scalar) {
        this->minMax.template blend<true, false>(this->minMax.min(other.minMax));
    } else {
        this->minMax.template setValue<0>(this->minMax.template getValue<0>().max(other.minMax.template getValue<0>()));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE void Range<T, Width>::clipMax(const Range& other)
{
    if constexpr (Width > SIMDWidth::Scalar) {
        this->minMax.template blend<false, true>(this->minMax.min(other.minMax));
    } else {
        this->minMax.template setValue<1>(this->minMax.template getValue<1>().min(other.minMax.template getValue<1>()));
    }
}
} // namespace Shift
