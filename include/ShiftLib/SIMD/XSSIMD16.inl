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

#include "SIMD/XSSIMD16.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#    include "SIMD/XSSIMDx86Functions.hpp"
#endif
#include "XSBit.hpp"
#include "XSMath.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator+(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator+(
        const SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator+(
        const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD4Def& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator+(
        const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD8Def& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator-(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator-(
        const SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator-(
        typename SIMD16<T, Width>::BaseDef other1, const SIMD16<T, Width>& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator-(
        const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD4Def& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator-(
        const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD8Def& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator*(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator*(
        const SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator*(
        const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD4Def& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator*(
        const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD8Def& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator/(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator/(
        const SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator/(
        typename SIMD16<T, Width>::BaseDef other1, const SIMD16<T, Width>& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator/(
        const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD4Def& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator/(
        const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD8Def& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator-(const SIMD16<T, Width>& other) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width>& operator+=(SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width>& operator+=(SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width>& operator-=(SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width>& operator-=(SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width>& operator*=(SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width>& operator*=(SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width>& operator/=(SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width>& operator/=(SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator&(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator&(
        const SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator|(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator|(
        const SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator^(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator^(
        const SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept

    template<typename T, SIMDWidth Width>
    XS_INLINE SIMD16<T, Width> operator~(const SIMD16<T, Width>& other) noexcept
} // namespace Shift
