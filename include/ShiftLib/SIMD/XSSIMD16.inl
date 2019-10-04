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
#endif
#include "XSMath.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD16Data<T>::SIMD16Data(const SIMD16<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_value0 = other.m_data.m_values[0];
        m_value1 = other.m_data.m_values[1];
        m_value2 = other.m_data.m_values[2];
        m_value3 = other.m_data.m_values[3];
        m_value4 = other.m_data.m_values[4];
        m_value5 = other.m_data.m_values[5];
        m_value6 = other.m_data.m_values[6];
        m_value7 = other.m_data.m_values[7];
        m_value8 = other.m_data.m_values[8];
        m_value9 = other.m_data.m_values[9];
        m_value10 = other.m_data.m_values[10];
        m_value11 = other.m_data.m_values[11];
        m_value12 = other.m_data.m_values[12];
        m_value13 = other.m_data.m_values[13];
        m_value14 = other.m_data.m_values[14];
        m_value15 = other.m_data.m_values[15];
    }
}

template<typename T>
XS_INLINE void SIMD16Data<T>::setData(const T value0, const T value1, const T value2, const T value3, const T value4,
    const T value5, const T value6, const T value7, const T value8, const T value9, const T value10, const T value11,
    const T value12, const T value13, const T value14, const T value15) noexcept
{
    m_value0 = value0;
    m_value1 = value1;
    m_value2 = value2;
    m_value3 = value3;
    m_value4 = value4;
    m_value5 = value5;
    m_value6 = value6;
    m_value7 = value7;
    m_value8 = value8;
    m_value9 = value9;
    m_value10 = value10;
    m_value11 = value11;
    m_value12 = value12;
    m_value13 = value13;
    m_value14 = value14;
    m_value15 = value15;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD16Data<T>::store(const SIMD16<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_value0 = other.m_data.m_values[0];
        m_value1 = other.m_data.m_values[1];
        m_value2 = other.m_data.m_values[2];
        m_value3 = other.m_data.m_values[3];
        m_value4 = other.m_data.m_values[4];
        m_value5 = other.m_data.m_values[5];
        m_value6 = other.m_data.m_values[6];
        m_value7 = other.m_data.m_values[7];
        m_value8 = other.m_data.m_values[8];
        m_value9 = other.m_data.m_values[9];
        m_value10 = other.m_data.m_values[10];
        m_value11 = other.m_data.m_values[11];
        m_value12 = other.m_data.m_values[12];
        m_value13 = other.m_data.m_values[13];
        m_value14 = other.m_data.m_values[14];
        m_value15 = other.m_data.m_values[15];
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16Data<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16<T, Width>(m_value0, m_value1, m_value2, m_value3, m_value4, m_value5, m_value6, m_value7,
            m_value8, m_value9, m_value10, m_value11, m_value12, m_value13, m_value14, m_value15);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>::SIMD16(const T value0, const T value1, const T value2, const T value3, const T value4,
    const T value5, const T value6, const T value7, const T value8, const T value9, const T value10, const T value11,
    const T value12, const T value13, const T value14, const T value15) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = value0;
        m_data.m_values[1] = value1;
        m_data.m_values[2] = value2;
        m_data.m_values[3] = value3;
        m_data.m_values[4] = value4;
        m_data.m_values[5] = value5;
        m_data.m_values[6] = value6;
        m_data.m_values[7] = value7;
        m_data.m_values[8] = value8;
        m_data.m_values[9] = value9;
        m_data.m_values[10] = value10;
        m_data.m_values[11] = value11;
        m_data.m_values[12] = value12;
        m_data.m_values[13] = value13;
        m_data.m_values[14] = value14;
        m_data.m_values[15] = value15;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>::SIMD16(const T value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = value;
        m_data.m_values[1] = value;
        m_data.m_values[2] = value;
        m_data.m_values[3] = value;
        m_data.m_values[4] = value;
        m_data.m_values[5] = value;
        m_data.m_values[6] = value;
        m_data.m_values[7] = value;
        m_data.m_values[8] = value;
        m_data.m_values[9] = value;
        m_data.m_values[10] = value;
        m_data.m_values[11] = value;
        m_data.m_values[12] = value;
        m_data.m_values[13] = value;
        m_data.m_values[14] = value;
        m_data.m_values[15] = value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width>::SIMD16(const SIMDBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other.m_data.m_value;
        m_data.m_values[1] = other.m_data.m_value;
        m_data.m_values[2] = other.m_data.m_value;
        m_data.m_values[3] = other.m_data.m_value;
        m_data.m_values[4] = other.m_data.m_value;
        m_data.m_values[5] = other.m_data.m_value;
        m_data.m_values[6] = other.m_data.m_value;
        m_data.m_values[7] = other.m_data.m_value;
        m_data.m_values[8] = other.m_data.m_value;
        m_data.m_values[9] = other.m_data.m_value;
        m_data.m_values[10] = other.m_data.m_value;
        m_data.m_values[11] = other.m_data.m_value;
        m_data.m_values[12] = other.m_data.m_value;
        m_data.m_values[13] = other.m_data.m_value;
        m_data.m_values[14] = other.m_data.m_value;
        m_data.m_values[15] = other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width>::SIMD16(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other.m_data.m_value;
        m_data.m_values[1] = other.m_data.m_value;
        m_data.m_values[2] = other.m_data.m_value;
        m_data.m_values[3] = other.m_data.m_value;
        m_data.m_values[4] = other.m_data.m_value;
        m_data.m_values[5] = other.m_data.m_value;
        m_data.m_values[6] = other.m_data.m_value;
        m_data.m_values[7] = other.m_data.m_value;
        m_data.m_values[8] = other.m_data.m_value;
        m_data.m_values[9] = other.m_data.m_value;
        m_data.m_values[10] = other.m_data.m_value;
        m_data.m_values[11] = other.m_data.m_value;
        m_data.m_values[12] = other.m_data.m_value;
        m_data.m_values[13] = other.m_data.m_value;
        m_data.m_values[14] = other.m_data.m_value;
        m_data.m_values[15] = other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width>::SIMD16(const SIMD2<T, Width>& other0, const SIMD2<T, Width>& other1,
    const SIMD2<T, Width>& other2, const SIMD2<T, Width>& other3, const SIMD2<T, Width>& other4,
    const SIMD2<T, Width>& other5, const SIMD2<T, Width>& other6, const SIMD2<T, Width>& other7) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other0.m_data.m_values[0];
        m_data.m_values[1] = other0.m_data.m_values[1];
        m_data.m_values[2] = other1.m_data.m_values[0];
        m_data.m_values[3] = other1.m_data.m_values[1];
        m_data.m_values[4] = other2.m_data.m_values[0];
        m_data.m_values[5] = other2.m_data.m_values[1];
        m_data.m_values[6] = other3.m_data.m_values[0];
        m_data.m_values[7] = other3.m_data.m_values[1];
        m_data.m_values[8] = other4.m_data.m_values[0];
        m_data.m_values[9] = other4.m_data.m_values[1];
        m_data.m_values[10] = other5.m_data.m_values[0];
        m_data.m_values[11] = other5.m_data.m_values[1];
        m_data.m_values[12] = other6.m_data.m_values[0];
        m_data.m_values[13] = other6.m_data.m_values[1];
        m_data.m_values[14] = other7.m_data.m_values[0];
        m_data.m_values[15] = other7.m_data.m_values[1];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width>::SIMD16(const SIMD2<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other.m_data.m_values[0];
        m_data.m_values[1] = other.m_data.m_values[1];
        m_data.m_values[2] = other.m_data.m_values[0];
        m_data.m_values[3] = other.m_data.m_values[1];
        m_data.m_values[4] = other.m_data.m_values[0];
        m_data.m_values[5] = other.m_data.m_values[1];
        m_data.m_values[6] = other.m_data.m_values[0];
        m_data.m_values[7] = other.m_data.m_values[1];
        m_data.m_values[8] = other.m_data.m_values[0];
        m_data.m_values[9] = other.m_data.m_values[1];
        m_data.m_values[10] = other.m_data.m_values[0];
        m_data.m_values[11] = other.m_data.m_values[1];
        m_data.m_values[12] = other.m_data.m_values[0];
        m_data.m_values[13] = other.m_data.m_values[1];
        m_data.m_values[14] = other.m_data.m_values[0];
        m_data.m_values[15] = other.m_data.m_values[1];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width>::SIMD16(const SIMD4<T, Width2>& other0, const SIMD4<T, Width2>& other1,
    const SIMD4<T, Width2>& other2, const SIMD4<T, Width2>& other3) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other0.m_data.m_values[0];
        m_data.m_values[1] = other0.m_data.m_values[1];
        m_data.m_values[2] = other0.m_data.m_values[2];
        m_data.m_values[3] = other0.m_data.m_values[3];
        m_data.m_values[4] = other1.m_data.m_values[0];
        m_data.m_values[5] = other1.m_data.m_values[1];
        m_data.m_values[6] = other1.m_data.m_values[2];
        m_data.m_values[7] = other1.m_data.m_values[3];
        m_data.m_values[8] = other2.m_data.m_values[0];
        m_data.m_values[9] = other2.m_data.m_values[1];
        m_data.m_values[10] = other2.m_data.m_values[2];
        m_data.m_values[11] = other2.m_data.m_values[3];
        m_data.m_values[12] = other3.m_data.m_values[0];
        m_data.m_values[13] = other3.m_data.m_values[1];
        m_data.m_values[14] = other3.m_data.m_values[2];
        m_data.m_values[15] = other3.m_data.m_values[3];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width>::SIMD16(const SIMD4<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other.m_data.m_values[0];
        m_data.m_values[1] = other.m_data.m_values[1];
        m_data.m_values[2] = other.m_data.m_values[2];
        m_data.m_values[3] = other.m_data.m_values[3];
        m_data.m_values[4] = other.m_data.m_values[0];
        m_data.m_values[5] = other.m_data.m_values[1];
        m_data.m_values[6] = other.m_data.m_values[2];
        m_data.m_values[7] = other.m_data.m_values[3];
        m_data.m_values[8] = other.m_data.m_values[0];
        m_data.m_values[9] = other.m_data.m_values[1];
        m_data.m_values[10] = other.m_data.m_values[2];
        m_data.m_values[11] = other.m_data.m_values[3];
        m_data.m_values[12] = other.m_data.m_values[0];
        m_data.m_values[13] = other.m_data.m_values[1];
        m_data.m_values[14] = other.m_data.m_values[2];
        m_data.m_values[15] = other.m_data.m_values[3];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width>::SIMD16(const SIMD8<T, Width2>& other0, const SIMD8<T, Width2>& other1) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other0.m_data.m_values[0];
        m_data.m_values[1] = other0.m_data.m_values[1];
        m_data.m_values[2] = other0.m_data.m_values[2];
        m_data.m_values[3] = other0.m_data.m_values[3];
        m_data.m_values[4] = other0.m_data.m_values[4];
        m_data.m_values[5] = other0.m_data.m_values[5];
        m_data.m_values[6] = other0.m_data.m_values[6];
        m_data.m_values[7] = other0.m_data.m_values[7];
        m_data.m_values[8] = other1.m_data.m_values[0];
        m_data.m_values[9] = other1.m_data.m_values[1];
        m_data.m_values[10] = other1.m_data.m_values[2];
        m_data.m_values[11] = other1.m_data.m_values[3];
        m_data.m_values[12] = other1.m_data.m_values[4];
        m_data.m_values[13] = other1.m_data.m_values[5];
        m_data.m_values[14] = other1.m_data.m_values[6];
        m_data.m_values[15] = other1.m_data.m_values[7];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width>::SIMD16(const SIMD8<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other.m_data.m_values[0];
        m_data.m_values[1] = other.m_data.m_values[1];
        m_data.m_values[2] = other.m_data.m_values[2];
        m_data.m_values[3] = other.m_data.m_values[3];
        m_data.m_values[4] = other.m_data.m_values[4];
        m_data.m_values[5] = other.m_data.m_values[5];
        m_data.m_values[6] = other.m_data.m_values[6];
        m_data.m_values[7] = other.m_data.m_values[7];
        m_data.m_values[8] = other.m_data.m_values[0];
        m_data.m_values[9] = other.m_data.m_values[1];
        m_data.m_values[10] = other.m_data.m_values[2];
        m_data.m_values[11] = other.m_data.m_values[3];
        m_data.m_values[12] = other.m_data.m_values[4];
        m_data.m_values[13] = other.m_data.m_values[5];
        m_data.m_values[14] = other.m_data.m_values[6];
        m_data.m_values[15] = other.m_data.m_values[7];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width>::SIMD16(const SIMD12<T, Width2>& other0, const SIMD4<T, Width2>& other1) noexcept
{
    XS_REQUIRES((sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>> && sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other0.m_data.m_values[0];
        m_data.m_values[1] = other0.m_data.m_values[1];
        m_data.m_values[2] = other0.m_data.m_values[2];
        m_data.m_values[3] = other0.m_data.m_values[3];
        m_data.m_values[4] = other0.m_data.m_values[4];
        m_data.m_values[5] = other0.m_data.m_values[5];
        m_data.m_values[6] = other0.m_data.m_values[6];
        m_data.m_values[7] = other0.m_data.m_values[7];
        m_data.m_values[8] = other0.m_data.m_values[8];
        m_data.m_values[9] = other0.m_data.m_values[9];
        m_data.m_values[10] = other0.m_data.m_values[10];
        m_data.m_values[11] = other0.m_data.m_values[11];
        m_data.m_values[12] = other1.m_data.m_values[0];
        m_data.m_values[13] = other1.m_data.m_values[1];
        m_data.m_values[14] = other1.m_data.m_values[2];
        m_data.m_values[15] = other1.m_data.m_values[3];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width>::SIMD16(const SIMD3x4<T, Width2>& other0, const SIMD4<T, Width2>& other1) noexcept
{
    XS_REQUIRES((sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>> && sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other0.m_data.m_values[0];
        m_data.m_values[1] = other0.m_data.m_values[1];
        m_data.m_values[2] = other0.m_data.m_values[2];
        m_data.m_values[3] = other1.m_data.m_values[0];
        m_data.m_values[4] = other0.m_data.m_values[3];
        m_data.m_values[5] = other0.m_data.m_values[4];
        m_data.m_values[6] = other0.m_data.m_values[5];
        m_data.m_values[7] = other1.m_data.m_values[1];
        m_data.m_values[8] = other0.m_data.m_values[6];
        m_data.m_values[9] = other0.m_data.m_values[7];
        m_data.m_values[10] = other0.m_data.m_values[8];
        m_data.m_values[11] = other1.m_data.m_values[2];
        m_data.m_values[12] = other0.m_data.m_values[9];
        m_data.m_values[13] = other0.m_data.m_values[10];
        m_data.m_values[14] = other0.m_data.m_values[11];
        m_data.m_values[15] = other1.m_data.m_values[3];
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width>::SIMD16(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(T{1});
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE SIMDInBase<T, Width2> SIMD16<T, Width>::getValueInBase() const noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 16);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width2>(m_data.m_values[Index]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width2> SIMD16<T, Width>::getValue() const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
    static_assert(Index < 16);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDBase<T, Width2>(m_data.m_values[Index]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width2> SIMD16<T, Width>::getValue2() const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width2>(m_data.m_values[Index * 2], m_data.m_values[Index * 2 + 1]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width2> SIMD16<T, Width>::getValue4() const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width2>(m_data.m_values[Index * 4], m_data.m_values[Index * 4 + 1],
            m_data.m_values[Index * 4 + 2], m_data.m_values[Index * 4 + 3]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width2> SIMD16<T, Width>::getValue8() const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width2>(m_data.m_values[Index * 8], m_data.m_values[Index * 8 + 1],
            m_data.m_values[Index * 8 + 2], m_data.m_values[Index * 8 + 3], m_data.m_values[Index * 8 + 4],
            m_data.m_values[Index * 8 + 5], m_data.m_values[Index * 8 + 6], m_data.m_values[Index * 8 + 7]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width2> SIMD16<T, Width>::getValue4x2() const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
    static_assert(Index0 < 4 && Index1 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width2>(getValue4<Index0>(), getValue4<Index1>());
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width2> SIMD16<T, Width>::getValue4x3() const noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(getValue4<Index0>(), getValue4<Index1>(), getValue4<Index2>());
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width2> SIMD16<T, Width>::getValue3x4() const noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width2>(m_data.m_values[Index0], m_data.m_values[Index1], m_data.m_values[Index2],
            m_data.m_values[Index0 + 4], m_data.m_values[Index1 + 4], m_data.m_values[Index2 + 4],
            m_data.m_values[Index0 + 8], m_data.m_values[Index1 + 8], m_data.m_values[Index2 + 8],
            m_data.m_values[Index0 + 12], m_data.m_values[Index1 + 12], m_data.m_values[Index2 + 12]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD16<T, Width>::setValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 16);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index] = m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD16<T, Width>::setValue(const SIMDBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
    static_assert(Index < 16);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index] = m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD16<T, Width>::setValue2(const SIMD2<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index * 2] = m_data.m_values[0];
        m_data.m_values[Index * 2 + 1] = m_data.m_values[1];
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD16<T, Width>::setValue4(const SIMD4<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index * 4] = m_data.m_values[0];
        m_data.m_values[Index * 4 + 1] = m_data.m_values[1];
        m_data.m_values[Index * 4 + 2] = m_data.m_values[2];
        m_data.m_values[Index * 4 + 3] = m_data.m_values[3];
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD16<T, Width>::setValue8(const SIMD8<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index * 8] = m_data.m_values[0];
        m_data.m_values[Index * 8 + 1] = m_data.m_values[1];
        m_data.m_values[Index * 8 + 2] = m_data.m_values[2];
        m_data.m_values[Index * 8 + 3] = m_data.m_values[3];
        m_data.m_values[Index * 8 + 4] = m_data.m_values[4];
        m_data.m_values[Index * 8 + 5] = m_data.m_values[5];
        m_data.m_values[Index * 8 + 6] = m_data.m_values[6];
        m_data.m_values[Index * 8 + 7] = m_data.m_values[7];
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE void SIMD16<T, Width>::setValue4x2(const SIMD8<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
    static_assert(Index0 < 4 && Index1 < 4);
    static_assert(Index0 != Index1);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        setValue4<Index0>(other.template getValue4<0>());
        setValue4<Index1>(other.template getValue4<1>());
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7, bool Elem8,
    bool Elem9, bool Elem10, bool Elem11, bool Elem12, bool Elem13, bool Elem14, bool Elem15>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::negate() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Elem0 ? -m_data.m_values[0] : m_data.m_values[0],
            Elem1 ? -m_data.m_values[1] : m_data.m_values[1], Elem2 ? -m_data.m_values[2] : m_data.m_values[2],
            Elem3 ? -m_data.m_values[3] : m_data.m_values[3], Elem4 ? -m_data.m_values[4] : m_data.m_values[4],
            Elem5 ? -m_data.m_values[5] : m_data.m_values[5], Elem6 ? -m_data.m_values[6] : m_data.m_values[6],
            Elem7 ? -m_data.m_values[7] : m_data.m_values[7], Elem8 ? -m_data.m_values[8] : m_data.m_values[8],
            Elem9 ? -m_data.m_values[9] : m_data.m_values[9], Elem10 ? -m_data.m_values[10] : m_data.m_values[10],
            Elem11 ? -m_data.m_values[11] : m_data.m_values[11], Elem12 ? -m_data.m_values[12] : m_data.m_values[12],
            Elem13 ? -m_data.m_values[13] : m_data.m_values[13], Elem14 ? -m_data.m_values[14] : m_data.m_values[14],
            Elem15 ? -m_data.m_values[15] : m_data.m_values[15]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::mad(const SIMD16<T, Width2>& other1, const SIMD16<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[4], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[5], other2.m_data.m_values[5]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[6], other2.m_data.m_values[6]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[7], other2.m_data.m_values[7]),
            Shift::fma<T>(m_data.m_values[8], other1.m_data.m_values[8], other2.m_data.m_values[8]),
            Shift::fma<T>(m_data.m_values[9], other1.m_data.m_values[9], other2.m_data.m_values[9]),
            Shift::fma<T>(m_data.m_values[10], other1.m_data.m_values[10], other2.m_data.m_values[10]),
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_values[11], other2.m_data.m_values[11]),
            Shift::fma<T>(m_data.m_values[12], other1.m_data.m_values[12], other2.m_data.m_values[12]),
            Shift::fma<T>(m_data.m_values[13], other1.m_data.m_values[13], other2.m_data.m_values[13]),
            Shift::fma<T>(m_data.m_values[14], other1.m_data.m_values[14], other2.m_data.m_values[14]),
            Shift::fma<T>(m_data.m_values[15], other1.m_data.m_values[15], other2.m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::mad(
    const SIMDBase<T, Width2>& other1, const SIMD16<T, Width>& other2) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_value, other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_value, other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_value, other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_value, other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_value, other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_value, other2.m_data.m_values[5]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_value, other2.m_data.m_values[6]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_value, other2.m_data.m_values[7]),
            Shift::fma<T>(m_data.m_values[8], other1.m_data.m_value, other2.m_data.m_values[8]),
            Shift::fma<T>(m_data.m_values[9], other1.m_data.m_value, other2.m_data.m_values[9]),
            Shift::fma<T>(m_data.m_values[10], other1.m_data.m_value, other2.m_data.m_values[10]),
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_value, other2.m_data.m_values[11]),
            Shift::fma<T>(m_data.m_values[12], other1.m_data.m_value, other2.m_data.m_values[12]),
            Shift::fma<T>(m_data.m_values[13], other1.m_data.m_value, other2.m_data.m_values[13]),
            Shift::fma<T>(m_data.m_values[14], other1.m_data.m_value, other2.m_data.m_values[14]),
            Shift::fma<T>(m_data.m_values[15], other1.m_data.m_value, other2.m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::mad(
    const SIMD16<T, Width>& other1, const SIMDBase<T, Width2>& other2) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[4], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[5], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[6], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[7], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[8], other1.m_data.m_values[8], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[9], other1.m_data.m_values[9], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[10], other1.m_data.m_values[10], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_values[11], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[12], other1.m_data.m_values[12], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[13], other1.m_data.m_values[13], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[14], other1.m_data.m_values[14], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[15], other1.m_data.m_values[15], other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::mad(const SIMD4<T, Width2>& other1, const SIMD4<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[3], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[8], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[9], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[10], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_values[3], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[12], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[13], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[14], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[15], other1.m_data.m_values[3], other2.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::mad(const SIMD4<T, Width2>& other1, const SIMD16<T, Width>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[0], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[1], other2.m_data.m_values[5]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[2], other2.m_data.m_values[6]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[3], other2.m_data.m_values[7]),
            Shift::fma<T>(m_data.m_values[8], other1.m_data.m_values[0], other2.m_data.m_values[8]),
            Shift::fma<T>(m_data.m_values[9], other1.m_data.m_values[1], other2.m_data.m_values[9]),
            Shift::fma<T>(m_data.m_values[10], other1.m_data.m_values[2], other2.m_data.m_values[10]),
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_values[3], other2.m_data.m_values[11]),
            Shift::fma<T>(m_data.m_values[12], other1.m_data.m_values[0], other2.m_data.m_values[12]),
            Shift::fma<T>(m_data.m_values[13], other1.m_data.m_values[1], other2.m_data.m_values[13]),
            Shift::fma<T>(m_data.m_values[14], other1.m_data.m_values[2], other2.m_data.m_values[14]),
            Shift::fma<T>(m_data.m_values[15], other1.m_data.m_values[3], other2.m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::mad(const SIMD8<T, Width2>& other1, const SIMD8<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[4], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[5], other2.m_data.m_values[5]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[6], other2.m_data.m_values[6]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[7], other2.m_data.m_values[7]),
            Shift::fma<T>(m_data.m_values[8], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[9], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[10], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_values[3], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[12], other1.m_data.m_values[4], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[13], other1.m_data.m_values[5], other2.m_data.m_values[5]),
            Shift::fma<T>(m_data.m_values[14], other1.m_data.m_values[6], other2.m_data.m_values[6]),
            Shift::fma<T>(m_data.m_values[15], other1.m_data.m_values[7], other2.m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::mad(const SIMD8<T, Width2>& other1, const SIMD16<T, Width>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[4], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[5], other2.m_data.m_values[5]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[6], other2.m_data.m_values[6]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[7], other2.m_data.m_values[7]),
            Shift::fma<T>(m_data.m_values[8], other1.m_data.m_values[0], other2.m_data.m_values[8]),
            Shift::fma<T>(m_data.m_values[9], other1.m_data.m_values[1], other2.m_data.m_values[9]),
            Shift::fma<T>(m_data.m_values[10], other1.m_data.m_values[2], other2.m_data.m_values[10]),
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_values[3], other2.m_data.m_values[11]),
            Shift::fma<T>(m_data.m_values[12], other1.m_data.m_values[4], other2.m_data.m_values[12]),
            Shift::fma<T>(m_data.m_values[13], other1.m_data.m_values[5], other2.m_data.m_values[13]),
            Shift::fma<T>(m_data.m_values[14], other1.m_data.m_values[6], other2.m_data.m_values[14]),
            Shift::fma<T>(m_data.m_values[15], other1.m_data.m_values[7], other2.m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::msub(const SIMD16& other1, const SIMD16& other2) const noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], -other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], -other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], -other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], -other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[4], -other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[5], -other2.m_data.m_values[5]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[6], -other2.m_data.m_values[6]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[7], -other2.m_data.m_values[7]),
            Shift::fma<T>(m_data.m_values[8], other1.m_data.m_values[8], -other2.m_data.m_values[8]),
            Shift::fma<T>(m_data.m_values[9], other1.m_data.m_values[9], -other2.m_data.m_values[9]),
            Shift::fma<T>(m_data.m_values[10], other1.m_data.m_values[10], -other2.m_data.m_values[10]),
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_values[11], -other2.m_data.m_values[11]),
            Shift::fma<T>(m_data.m_values[12], other1.m_data.m_values[12], -other2.m_data.m_values[12]),
            Shift::fma<T>(m_data.m_values[13], other1.m_data.m_values[13], -other2.m_data.m_values[13]),
            Shift::fma<T>(m_data.m_values[14], other1.m_data.m_values[14], -other2.m_data.m_values[14]),
            Shift::fma<T>(m_data.m_values[15], other1.m_data.m_values[15], -other2.m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::sign(const SIMD16<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::sign<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::sign<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::sign<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::sign<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::sign<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::sign<T>(m_data.m_values[5], other.m_data.m_values[5]),
            Shift::sign<T>(m_data.m_values[6], other.m_data.m_values[6]),
            Shift::sign<T>(m_data.m_values[7], other.m_data.m_values[7]),
            Shift::sign<T>(m_data.m_values[8], other.m_data.m_values[8]),
            Shift::sign<T>(m_data.m_values[9], other.m_data.m_values[9]),
            Shift::sign<T>(m_data.m_values[10], other.m_data.m_values[10]),
            Shift::sign<T>(m_data.m_values[11], other.m_data.m_values[11]),
            Shift::sign<T>(m_data.m_values[12], other.m_data.m_values[12]),
            Shift::sign<T>(m_data.m_values[13], other.m_data.m_values[13]),
            Shift::sign<T>(m_data.m_values[14], other.m_data.m_values[14]),
            Shift::sign<T>(m_data.m_values[15], other.m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::abs<T>(m_data.m_values[0]), Shift::abs<T>(m_data.m_values[1]),
            Shift::abs<T>(m_data.m_values[2]), Shift::abs<T>(m_data.m_values[3]), Shift::abs<T>(m_data.m_values[4]),
            Shift::abs<T>(m_data.m_values[5]), Shift::abs<T>(m_data.m_values[6]), Shift::abs<T>(m_data.m_values[7]),
            Shift::abs<T>(m_data.m_values[8]), Shift::abs<T>(m_data.m_values[9]), Shift::abs<T>(m_data.m_values[10]),
            Shift::abs<T>(m_data.m_values[11]), Shift::abs<T>(m_data.m_values[12]), Shift::abs<T>(m_data.m_values[13]),
            Shift::abs<T>(m_data.m_values[14]), Shift::abs<T>(m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::max(const SIMD16<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::max<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::max<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::max<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::max<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::max<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::max<T>(m_data.m_values[5], other.m_data.m_values[5]),
            Shift::max<T>(m_data.m_values[6], other.m_data.m_values[6]),
            Shift::max<T>(m_data.m_values[7], other.m_data.m_values[7]),
            Shift::max<T>(m_data.m_values[8], other.m_data.m_values[8]),
            Shift::max<T>(m_data.m_values[9], other.m_data.m_values[9]),
            Shift::max<T>(m_data.m_values[10], other.m_data.m_values[10]),
            Shift::max<T>(m_data.m_values[11], other.m_data.m_values[11]),
            Shift::max<T>(m_data.m_values[12], other.m_data.m_values[12]),
            Shift::max<T>(m_data.m_values[13], other.m_data.m_values[13]),
            Shift::max<T>(m_data.m_values[14], other.m_data.m_values[14]),
            Shift::max<T>(m_data.m_values[15], other.m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::min(const SIMD16<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::min<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::min<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::min<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::min<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::min<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::min<T>(m_data.m_values[5], other.m_data.m_values[5]),
            Shift::min<T>(m_data.m_values[6], other.m_data.m_values[6]),
            Shift::min<T>(m_data.m_values[7], other.m_data.m_values[7]),
            Shift::min<T>(m_data.m_values[8], other.m_data.m_values[8]),
            Shift::min<T>(m_data.m_values[9], other.m_data.m_values[9]),
            Shift::min<T>(m_data.m_values[10], other.m_data.m_values[10]),
            Shift::min<T>(m_data.m_values[11], other.m_data.m_values[11]),
            Shift::min<T>(m_data.m_values[12], other.m_data.m_values[12]),
            Shift::min<T>(m_data.m_values[13], other.m_data.m_values[13]),
            Shift::min<T>(m_data.m_values[14], other.m_data.m_values[14]),
            Shift::min<T>(m_data.m_values[15], other.m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::max(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::max<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[3], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[4], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[5], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[6], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[7], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[8], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[9], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[10], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[11], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[12], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[13], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[14], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[15], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::min(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::min<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[3], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[4], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[5], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[6], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[7], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[8], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[9], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[10], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[11], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[12], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[13], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[14], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[15], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD16<T, Width>::max4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other1 = Shift::max<T>(m_data.m_values[0], m_data.m_values[4]);
        T other2 = Shift::max<T>(m_data.m_values[1], m_data.m_values[5]);
        T other3 = Shift::max<T>(m_data.m_values[2], m_data.m_values[6]);
        T other4 = Shift::max<T>(m_data.m_values[3], m_data.m_values[7]);
        other1 = Shift::max<T>(other1, m_data.m_values[8]);
        other2 = Shift::max<T>(other2, m_data.m_values[9]);
        other3 = Shift::max<T>(other3, m_data.m_values[10]);
        other4 = Shift::max<T>(other4, m_data.m_values[11]);
        return SIMD4<T, Width>(Shift::max<T>(other1, m_data.m_values[12]), Shift::max<T>(other2, m_data.m_values[13]),
            Shift::max<T>(other3, m_data.m_values[14]), Shift::max<T>(other4, m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD16<T, Width>::min4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other1 = Shift::min<T>(m_data.m_values[0], m_data.m_values[4]);
        T other2 = Shift::min<T>(m_data.m_values[1], m_data.m_values[5]);
        T other3 = Shift::min<T>(m_data.m_values[2], m_data.m_values[6]);
        T other4 = Shift::min<T>(m_data.m_values[3], m_data.m_values[7]);
        other1 = Shift::min<T>(other1, m_data.m_values[8]);
        other2 = Shift::min<T>(other2, m_data.m_values[9]);
        other3 = Shift::min<T>(other3, m_data.m_values[10]);
        other4 = Shift::min<T>(other4, m_data.m_values[11]);
        return SIMD4<T, Width>(Shift::min<T>(other1, m_data.m_values[12]), Shift::min<T>(other2, m_data.m_values[13]),
            Shift::min<T>(other3, m_data.m_values[14]), Shift::min<T>(other4, m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD16<T, Width>::max8() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(Shift::max<T>(m_data.m_values[0], m_data.m_values[8]),
            Shift::max<T>(m_data.m_values[1], m_data.m_values[9]),
            Shift::max<T>(m_data.m_values[2], m_data.m_values[10]),
            Shift::max<T>(m_data.m_values[3], m_data.m_values[11]),
            Shift::max<T>(m_data.m_values[4], m_data.m_values[12]),
            Shift::max<T>(m_data.m_values[5], m_data.m_values[13]),
            Shift::max<T>(m_data.m_values[6], m_data.m_values[14]),
            Shift::max<T>(m_data.m_values[7], m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD16<T, Width>::min8() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(Shift::min<T>(m_data.m_values[0], m_data.m_values[8]),
            Shift::min<T>(m_data.m_values[1], m_data.m_values[9]),
            Shift::min<T>(m_data.m_values[2], m_data.m_values[10]),
            Shift::min<T>(m_data.m_values[3], m_data.m_values[11]),
            Shift::min<T>(m_data.m_values[4], m_data.m_values[12]),
            Shift::min<T>(m_data.m_values[5], m_data.m_values[13]),
            Shift::min<T>(m_data.m_values[6], m_data.m_values[14]),
            Shift::min<T>(m_data.m_values[7], m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::BaseDef SIMD16<T, Width>::hmax() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other = Shift::max<T>(m_data.m_values[0], m_data.m_values[1]);
        T other2 = Shift::max<T>(m_data.m_values[2], m_data.m_values[3]);
        T other3 = Shift::max<T>(m_data.m_values[4], m_data.m_values[5]);
        T other4 = Shift::max<T>(m_data.m_values[6], m_data.m_values[7]);
        other = Shift::max<T>(m_data.m_values[8], other);
        other2 = Shift::max<T>(m_data.m_values[9], other2);
        other3 = Shift::max<T>(m_data.m_values[10], other3);
        other4 = Shift::max<T>(m_data.m_values[11], other4);
        other = Shift::max<T>(m_data.m_values[12], other);
        other2 = Shift::max<T>(m_data.m_values[13], other2);
        other3 = Shift::max<T>(m_data.m_values[14], other3);
        other4 = Shift::max<T>(m_data.m_values[15], other4);
        other = Shift::max<T>(other2, other);
        other3 = Shift::max<T>(other4, other3);
        return BaseDef(Shift::max<T>(other3, other));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::BaseDef SIMD16<T, Width>::hmin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other = Shift::min<T>(m_data.m_values[0], m_data.m_values[1]);
        T other2 = Shift::min<T>(m_data.m_values[2], m_data.m_values[3]);
        T other3 = Shift::min<T>(m_data.m_values[4], m_data.m_values[5]);
        T other4 = Shift::min<T>(m_data.m_values[6], m_data.m_values[7]);
        other = Shift::min<T>(m_data.m_values[8], other);
        other2 = Shift::min<T>(m_data.m_values[9], other);
        other3 = Shift::min<T>(m_data.m_values[10], other);
        other4 = Shift::min<T>(m_data.m_values[11], other);
        other = Shift::min<T>(m_data.m_values[12], other);
        other2 = Shift::min<T>(m_data.m_values[13], other);
        other3 = Shift::min<T>(m_data.m_values[14], other3);
        other4 = Shift::min<T>(m_data.m_values[15], other4);
        other = Shift::min<T>(other2, other);
        other3 = Shift::min<T>(other4, other3);
        return BaseDef(Shift::min<T>(other3, other));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD16<T, Width>::hmaxInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width>(hmax().m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD16<T, Width>::hminInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width>(hmin().m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD16<T, Width>::hmax2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(Shift::max<T>(m_data.m_values[0], m_data.m_values[1]),
            Shift::max<T>(m_data.m_values[2], m_data.m_values[3]),
            Shift::max<T>(m_data.m_values[4], m_data.m_values[5]),
            Shift::max<T>(m_data.m_values[6], m_data.m_values[7]),
            Shift::max<T>(m_data.m_values[8], m_data.m_values[9]),
            Shift::max<T>(m_data.m_values[10], m_data.m_values[11]),
            Shift::max<T>(m_data.m_values[12], m_data.m_values[13]),
            Shift::max<T>(m_data.m_values[14], m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD16<T, Width>::hmin2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(Shift::min<T>(m_data.m_values[0], m_data.m_values[1]),
            Shift::min<T>(m_data.m_values[2], m_data.m_values[3]),
            Shift::min<T>(m_data.m_values[4], m_data.m_values[5]),
            Shift::min<T>(m_data.m_values[6], m_data.m_values[7]),
            Shift::min<T>(m_data.m_values[8], m_data.m_values[9]),
            Shift::min<T>(m_data.m_values[10], m_data.m_values[11]),
            Shift::min<T>(m_data.m_values[12], m_data.m_values[13]),
            Shift::min<T>(m_data.m_values[14], m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD16<T, Width>::hmax4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other1 = Shift::max<T>(m_data.m_values[0], m_data.m_values[1]);
        T other2 = Shift::max<T>(m_data.m_values[4], m_data.m_values[5]);
        T other3 = Shift::max<T>(m_data.m_values[8], m_data.m_values[9]);
        T other4 = Shift::max<T>(m_data.m_values[12], m_data.m_values[13]);
        other1 = Shift::max<T>(m_data.m_values[2], other1);
        other2 = Shift::max<T>(m_data.m_values[6], other2);
        other3 = Shift::max<T>(m_data.m_values[10], other3);
        other4 = Shift::max<T>(m_data.m_values[14], other3);
        return SIMD4<T, Width>(Shift::max<T>(m_data.m_values[3], other1), Shift::max<T>(m_data.m_values[7], other2),
            Shift::max<T>(m_data.m_values[11], other3), Shift::max<T>(m_data.m_values[15], other4));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD16<T, Width>::hmin4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other1 = Shift::min<T>(m_data.m_values[0], m_data.m_values[1]);
        T other2 = Shift::min<T>(m_data.m_values[4], m_data.m_values[5]);
        T other3 = Shift::min<T>(m_data.m_values[8], m_data.m_values[9]);
        T other4 = Shift::min<T>(m_data.m_values[12], m_data.m_values[13]);
        other1 = Shift::min<T>(m_data.m_values[2], other1);
        other2 = Shift::min<T>(m_data.m_values[6], other2);
        other3 = Shift::min<T>(m_data.m_values[10], other3);
        other4 = Shift::min<T>(m_data.m_values[14], other3);
        return SIMD4<T, Width>(Shift::min<T>(m_data.m_values[3], other1), Shift::min<T>(m_data.m_values[7], other2),
            Shift::min<T>(m_data.m_values[11], other3), Shift::min<T>(m_data.m_values[15], other4));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD16<T, Width>::hmax8() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other1 = Shift::max<T>(m_data.m_values[0], m_data.m_values[1]);
        T other2 = Shift::max<T>(m_data.m_values[8], m_data.m_values[9]);
        T other3 = Shift::max<T>(m_data.m_values[2], m_data.m_values[3]);
        T other4 = Shift::max<T>(m_data.m_values[10], m_data.m_values[11]);
        other1 = Shift::max<T>(m_data.m_values[4], other1);
        other2 = Shift::max<T>(m_data.m_values[12], other2);
        other3 = Shift::max<T>(m_data.m_values[5], other3);
        other4 = Shift::max<T>(m_data.m_values[13], other4);
        other1 = Shift::max<T>(m_data.m_values[6], other1);
        other2 = Shift::max<T>(m_data.m_values[14], other2);
        other3 = Shift::max<T>(m_data.m_values[7], other3);
        other4 = Shift::max<T>(m_data.m_values[15], other4);
        return SIMD2<T, Width>(Shift::max<T>(other3, other1), Shift::max<T>(other4, other2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD16<T, Width>::hmin8() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other1 = Shift::min<T>(m_data.m_values[0], m_data.m_values[1]);
        T other2 = Shift::min<T>(m_data.m_values[8], m_data.m_values[9]);
        T other3 = Shift::min<T>(m_data.m_values[2], m_data.m_values[3]);
        T other4 = Shift::min<T>(m_data.m_values[10], m_data.m_values[11]);
        other1 = Shift::min<T>(m_data.m_values[4], other1);
        other2 = Shift::min<T>(m_data.m_values[12], other2);
        other3 = Shift::min<T>(m_data.m_values[5], other3);
        other4 = Shift::min<T>(m_data.m_values[13], other4);
        other1 = Shift::min<T>(m_data.m_values[6], other1);
        other2 = Shift::min<T>(m_data.m_values[14], other2);
        other3 = Shift::min<T>(m_data.m_values[7], other3);
        other4 = Shift::min<T>(m_data.m_values[15], other4);
        return SIMD2<T, Width>(Shift::min<T>(other3, other1), Shift::min<T>(other4, other2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::reciprocal() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::recip<T>(m_data.m_values[0]), Shift::recip<T>(m_data.m_values[1]),
            Shift::recip<T>(m_data.m_values[2]), Shift::recip<T>(m_data.m_values[3]),
            Shift::recip<T>(m_data.m_values[4]), Shift::recip<T>(m_data.m_values[5]),
            Shift::recip<T>(m_data.m_values[6]), Shift::recip<T>(m_data.m_values[7]),
            Shift::recip<T>(m_data.m_values[8]), Shift::recip<T>(m_data.m_values[9]),
            Shift::recip<T>(m_data.m_values[10]), Shift::recip<T>(m_data.m_values[11]),
            Shift::recip<T>(m_data.m_values[12]), Shift::recip<T>(m_data.m_values[13]),
            Shift::recip<T>(m_data.m_values[14]), Shift::recip<T>(m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD16<T, Width>::add4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other1 = m_data.m_values[0] + m_data.m_values[4];
        T other2 = m_data.m_values[1] + m_data.m_values[5];
        T other3 = m_data.m_values[2] + m_data.m_values[6];
        T other4 = m_data.m_values[3] + m_data.m_values[7];
        other1 = other1 + m_data.m_values[8];
        other2 = other2 + m_data.m_values[9];
        other3 = other3 + m_data.m_values[10];
        other4 = other4 + m_data.m_values[11];
        return SIMD4<T, Width>(other1 + m_data.m_values[12], other2 + m_data.m_values[13], other3 + m_data.m_values[14],
            other4 + m_data.m_values[15]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD16<T, Width>::add8() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(m_data.m_values[0] + m_data.m_values[8], m_data.m_values[1] + m_data.m_values[9],
            m_data.m_values[2] + m_data.m_values[10], m_data.m_values[3] + m_data.m_values[11],
            m_data.m_values[4] + m_data.m_values[12], m_data.m_values[5] + m_data.m_values[13],
            m_data.m_values[6] + m_data.m_values[14], m_data.m_values[7] + m_data.m_values[15]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD16<T, Width>::sub8() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(m_data.m_values[0] - m_data.m_values[8], m_data.m_values[1] - m_data.m_values[9],
            m_data.m_values[2] - m_data.m_values[10], m_data.m_values[3] - m_data.m_values[11],
            m_data.m_values[4] - m_data.m_values[12], m_data.m_values[5] - m_data.m_values[13],
            m_data.m_values[6] - m_data.m_values[14], m_data.m_values[7] - m_data.m_values[15]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD16<T, Width>::BaseDef SIMD16<T, Width>::hadd() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T res = (m_data.m_values[0] + m_data.m_values[1]);
        T res2 = (m_data.m_values[2] + m_data.m_values[3]);
        T res3 = (m_data.m_values[4] + m_data.m_values[5]);
        T res4 = (m_data.m_values[6] + m_data.m_values[7]);
        res += m_data.m_values[8];
        res2 += m_data.m_values[9];
        res3 += m_data.m_values[10];
        res4 += m_data.m_values[11];
        res += m_data.m_values[12];
        res2 += m_data.m_values[13];
        res3 += m_data.m_values[14];
        res4 += m_data.m_values[15];
        res += res3;
        res2 += res4;
        res += res2;
        return BaseDef(res);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD16<T, Width>::haddInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        SIMDInBase<T, Width>(hadd().m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD16<T, Width>::hadd2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(m_data.m_values[0] + m_data.m_values[1], m_data.m_values[2] + m_data.m_values[3],
            m_data.m_values[4] + m_data.m_values[5], m_data.m_values[6] + m_data.m_values[7],
            m_data.m_values[8] + m_data.m_values[9], m_data.m_values[10] + m_data.m_values[11],
            m_data.m_values[12] + m_data.m_values[13], m_data.m_values[14] + m_data.m_values[15]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD16<T, Width>::hadd4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T res = (m_data.m_values[0] + m_data.m_values[1]);
        T res2 = (m_data.m_values[4] + m_data.m_values[5]);
        T res3 = (m_data.m_values[8] + m_data.m_values[9]);
        T res4 = (m_data.m_values[12] + m_data.m_values[13]);
        res += m_data.m_values[2];
        res2 += m_data.m_values[6];
        res3 += m_data.m_values[10];
        res4 += m_data.m_values[14];
        res += m_data.m_values[3];
        res2 += m_data.m_values[7];
        res3 += m_data.m_values[11];
        res4 += m_data.m_values[15];
        return SIMD4<T, Width>(res, res2, res3, res4);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD16<T, Width>::hadd8() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T res = (m_data.m_values[0] + m_data.m_values[1]);
        T res2 = (m_data.m_values[4] + m_data.m_values[5]);
        T res3 = (m_data.m_values[8] + m_data.m_values[9]);
        T res4 = (m_data.m_values[12] + m_data.m_values[13]);
        res += m_data.m_values[2];
        res2 += m_data.m_values[6];
        res3 += m_data.m_values[10];
        res4 += m_data.m_values[14];
        res += m_data.m_values[3];
        res2 += m_data.m_values[7];
        res3 += m_data.m_values[11];
        res4 += m_data.m_values[15];
        res += res2;
        res3 += res4;
        return SIMD2<T, Width>(res, res3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD16<T, Width>::hsub2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(m_data.m_values[0] - m_data.m_values[1], m_data.m_values[2] - m_data.m_values[3],
            m_data.m_values[4] - m_data.m_values[5], m_data.m_values[6] - m_data.m_values[7],
            m_data.m_values[8] - m_data.m_values[9], m_data.m_values[10] - m_data.m_values[11],
            m_data.m_values[12] - m_data.m_values[13], m_data.m_values[14] - m_data.m_values[15]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::ceil<T>(m_data.m_values[0]), Shift::ceil<T>(m_data.m_values[1]),
            Shift::ceil<T>(m_data.m_values[2]), Shift::ceil<T>(m_data.m_values[3]), Shift::ceil<T>(m_data.m_values[4]),
            Shift::ceil<T>(m_data.m_values[5]), Shift::ceil<T>(m_data.m_values[6]), Shift::ceil<T>(m_data.m_values[7]),
            Shift::ceil<T>(m_data.m_values[8]), Shift::ceil<T>(m_data.m_values[9]), Shift::ceil<T>(m_data.m_values[10]),
            Shift::ceil<T>(m_data.m_values[11]), Shift::ceil<T>(m_data.m_values[12]),
            Shift::ceil<T>(m_data.m_values[13]), Shift::ceil<T>(m_data.m_values[14]),
            Shift::ceil<T>(m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::floor<T>(m_data.m_values[0]), Shift::floor<T>(m_data.m_values[1]),
            Shift::floor<T>(m_data.m_values[2]), Shift::floor<T>(m_data.m_values[3]),
            Shift::floor<T>(m_data.m_values[4]), Shift::floor<T>(m_data.m_values[5]),
            Shift::floor<T>(m_data.m_values[6]), Shift::floor<T>(m_data.m_values[7]),
            Shift::floor<T>(m_data.m_values[8]), Shift::floor<T>(m_data.m_values[9]),
            Shift::floor<T>(m_data.m_values[10]), Shift::floor<T>(m_data.m_values[11]),
            Shift::floor<T>(m_data.m_values[12]), Shift::floor<T>(m_data.m_values[13]),
            Shift::floor<T>(m_data.m_values[14]), Shift::floor<T>(m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::trunc<T>(m_data.m_values[0]), Shift::trunc<T>(m_data.m_values[1]),
            Shift::trunc<T>(m_data.m_values[2]), Shift::trunc<T>(m_data.m_values[3]),
            Shift::trunc<T>(m_data.m_values[4]), Shift::trunc<T>(m_data.m_values[5]),
            Shift::trunc<T>(m_data.m_values[6]), Shift::trunc<T>(m_data.m_values[7]),
            Shift::trunc<T>(m_data.m_values[8]), Shift::trunc<T>(m_data.m_values[9]),
            Shift::trunc<T>(m_data.m_values[10]), Shift::trunc<T>(m_data.m_values[11]),
            Shift::trunc<T>(m_data.m_values[12]), Shift::trunc<T>(m_data.m_values[13]),
            Shift::trunc<T>(m_data.m_values[14]), Shift::trunc<T>(m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::sqrt<T>(m_data.m_values[0]), Shift::sqrt<T>(m_data.m_values[1]),
            Shift::sqrt<T>(m_data.m_values[2]), Shift::sqrt<T>(m_data.m_values[3]), Shift::sqrt<T>(m_data.m_values[4]),
            Shift::sqrt<T>(m_data.m_values[5]), Shift::sqrt<T>(m_data.m_values[6]), Shift::sqrt<T>(m_data.m_values[7]),
            Shift::sqrt<T>(m_data.m_values[8]), Shift::sqrt<T>(m_data.m_values[9]), Shift::sqrt<T>(m_data.m_values[10]),
            Shift::sqrt<T>(m_data.m_values[11]), Shift::sqrt<T>(m_data.m_values[12]),
            Shift::sqrt<T>(m_data.m_values[13]), Shift::sqrt<T>(m_data.m_values[14]),
            Shift::sqrt<T>(m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::rsqrt<T>(m_data.m_values[0]), Shift::rsqrt<T>(m_data.m_values[1]),
            Shift::rsqrt<T>(m_data.m_values[2]), Shift::rsqrt<T>(m_data.m_values[3]),
            Shift::rsqrt<T>(m_data.m_values[4]), Shift::rsqrt<T>(m_data.m_values[5]),
            Shift::rsqrt<T>(m_data.m_values[6]), Shift::rsqrt<T>(m_data.m_values[7]),
            Shift::rsqrt<T>(m_data.m_values[8]), Shift::rsqrt<T>(m_data.m_values[9]),
            Shift::rsqrt<T>(m_data.m_values[10]), Shift::rsqrt<T>(m_data.m_values[11]),
            Shift::rsqrt<T>(m_data.m_values[12]), Shift::rsqrt<T>(m_data.m_values[13]),
            Shift::rsqrt<T>(m_data.m_values[14]), Shift::rsqrt<T>(m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD16<T, Width>::dot4(const SIMD16<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T temp0 = m_data.m_values[0] * other.m_data.m_values[0];
        T temp1 = m_data.m_values[1] * other.m_data.m_values[1];
        T temp2 = m_data.m_values[2] * other.m_data.m_values[2];
        T temp3 = m_data.m_values[3] * other.m_data.m_values[3];
        const T temp4 = m_data.m_values[4] * other.m_data.m_values[4];
        const T temp5 = m_data.m_values[5] * other.m_data.m_values[5];
        const T temp6 = m_data.m_values[6] * other.m_data.m_values[6];
        const T temp7 = m_data.m_values[7] * other.m_data.m_values[7];
        T temp8 = m_data.m_values[8] * other.m_data.m_values[8];
        T temp9 = m_data.m_values[9] * other.m_data.m_values[9];
        T temp10 = m_data.m_values[10] * other.m_data.m_values[10];
        T temp11 = m_data.m_values[11] * other.m_data.m_values[11];
        const T temp12 = m_data.m_values[12] * other.m_data.m_values[12];
        const T temp13 = m_data.m_values[13] * other.m_data.m_values[13];
        const T temp14 = m_data.m_values[14] * other.m_data.m_values[14];
        const T temp15 = m_data.m_values[15] * other.m_data.m_values[15];
        temp0 += temp4;
        temp1 += temp5;
        temp2 += temp6;
        temp3 += temp7;
        temp8 += temp12;
        temp9 += temp13;
        temp10 += temp14;
        temp11 += temp15;
        temp0 += temp8;
        temp1 += temp9;
        temp2 += temp10;
        temp3 += temp11;
        return SIMD4<T, Width>(temp0, temp1, temp2, temp3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::transpose4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        SIMD4<T, Width> return0, return1, return2, return3;
        SIMD4<T, Width>::transpose(
            getValue4<0>(), getValue4<1>(), getValue4<2>(), getValue4<3>(), return0, return1, return2, return3);
        return SIMD16(return0, return1, return2, return3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::exp2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::exp2<T>(m_data.m_values[0]), Shift::exp2<T>(m_data.m_values[1]),
            Shift::exp2<T>(m_data.m_values[2]), Shift::exp2<T>(m_data.m_values[3]), Shift::exp2<T>(m_data.m_values[4]),
            Shift::exp2<T>(m_data.m_values[5]), Shift::exp2<T>(m_data.m_values[6]), Shift::exp2<T>(m_data.m_values[7]),
            Shift::exp2<T>(m_data.m_values[8]), Shift::exp2<T>(m_data.m_values[9]), Shift::exp2<T>(m_data.m_values[10]),
            Shift::exp2<T>(m_data.m_values[11]), Shift::exp2<T>(m_data.m_values[12]),
            Shift::exp2<T>(m_data.m_values[13]), Shift::exp2<T>(m_data.m_values[14]),
            Shift::exp2<T>(m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::exp() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::exp<T>(m_data.m_values[0]), Shift::exp<T>(m_data.m_values[1]),
            Shift::exp<T>(m_data.m_values[2]), Shift::exp<T>(m_data.m_values[3]), Shift::exp<T>(m_data.m_values[4]),
            Shift::exp<T>(m_data.m_values[5]), Shift::exp<T>(m_data.m_values[6]), Shift::exp<T>(m_data.m_values[7]),
            Shift::exp<T>(m_data.m_values[8]), Shift::exp<T>(m_data.m_values[9]), Shift::exp<T>(m_data.m_values[10]),
            Shift::exp<T>(m_data.m_values[11]), Shift::exp<T>(m_data.m_values[12]), Shift::exp<T>(m_data.m_values[13]),
            Shift::exp<T>(m_data.m_values[14]), Shift::exp<T>(m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::log2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::log2<T>(m_data.m_values[0]), Shift::log2<T>(m_data.m_values[1]),
            Shift::log2<T>(m_data.m_values[2]), Shift::log2<T>(m_data.m_values[3]), Shift::log2<T>(m_data.m_values[4]),
            Shift::log2<T>(m_data.m_values[5]), Shift::log2<T>(m_data.m_values[6]), Shift::log2<T>(m_data.m_values[7]),
            Shift::log2<T>(m_data.m_values[8]), Shift::log2<T>(m_data.m_values[9]), Shift::log2<T>(m_data.m_values[10]),
            Shift::log2<T>(m_data.m_values[11]), Shift::log2<T>(m_data.m_values[12]),
            Shift::log2<T>(m_data.m_values[13]), Shift::log2<T>(m_data.m_values[14]),
            Shift::log2<T>(m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::log() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::log<T>(m_data.m_values[0]), Shift::log<T>(m_data.m_values[1]),
            Shift::log<T>(m_data.m_values[2]), Shift::log<T>(m_data.m_values[3]), Shift::log<T>(m_data.m_values[4]),
            Shift::log<T>(m_data.m_values[5]), Shift::log<T>(m_data.m_values[6]), Shift::log<T>(m_data.m_values[7]),
            Shift::log<T>(m_data.m_values[8]), Shift::log<T>(m_data.m_values[9]), Shift::log<T>(m_data.m_values[10]),
            Shift::log<T>(m_data.m_values[11]), Shift::log<T>(m_data.m_values[12]), Shift::log<T>(m_data.m_values[13]),
            Shift::log<T>(m_data.m_values[14]), Shift::log<T>(m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::pow(const SIMD16<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::pow<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::pow<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::pow<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::pow<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::pow<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::pow<T>(m_data.m_values[5], other.m_data.m_values[5]),
            Shift::pow<T>(m_data.m_values[6], other.m_data.m_values[6]),
            Shift::pow<T>(m_data.m_values[7], other.m_data.m_values[7]),
            Shift::pow<T>(m_data.m_values[8], other.m_data.m_values[8]),
            Shift::pow<T>(m_data.m_values[9], other.m_data.m_values[9]),
            Shift::pow<T>(m_data.m_values[10], other.m_data.m_values[10]),
            Shift::pow<T>(m_data.m_values[11], other.m_data.m_values[11]),
            Shift::pow<T>(m_data.m_values[12], other.m_data.m_values[12]),
            Shift::pow<T>(m_data.m_values[13], other.m_data.m_values[13]),
            Shift::pow<T>(m_data.m_values[14], other.m_data.m_values[14]),
            Shift::pow<T>(m_data.m_values[15], other.m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::powr(const SIMD16<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::powr<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::powr<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::powr<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::powr<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::powr<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::powr<T>(m_data.m_values[5], other.m_data.m_values[5]),
            Shift::powr<T>(m_data.m_values[6], other.m_data.m_values[6]),
            Shift::powr<T>(m_data.m_values[7], other.m_data.m_values[7]),
            Shift::powr<T>(m_data.m_values[8], other.m_data.m_values[8]),
            Shift::powr<T>(m_data.m_values[9], other.m_data.m_values[9]),
            Shift::powr<T>(m_data.m_values[10], other.m_data.m_values[10]),
            Shift::powr<T>(m_data.m_values[11], other.m_data.m_values[11]),
            Shift::powr<T>(m_data.m_values[12], other.m_data.m_values[12]),
            Shift::powr<T>(m_data.m_values[13], other.m_data.m_values[13]),
            Shift::powr<T>(m_data.m_values[14], other.m_data.m_values[14]),
            Shift::powr<T>(m_data.m_values[15], other.m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::pow(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::pow<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[3], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[4], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[5], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[6], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[7], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[8], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[9], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[10], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[11], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[12], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[13], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[14], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[15], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::powr(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::powr<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[3], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[4], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[5], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[6], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[7], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[8], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[9], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[10], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[11], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[12], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[13], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[14], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[15], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::sin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::sin<T>(m_data.m_values[0]), Shift::sin<T>(m_data.m_values[1]),
            Shift::sin<T>(m_data.m_values[2]), Shift::sin<T>(m_data.m_values[3]), Shift::sin<T>(m_data.m_values[4]),
            Shift::sin<T>(m_data.m_values[5]), Shift::sin<T>(m_data.m_values[6]), Shift::sin<T>(m_data.m_values[7]),
            Shift::sin<T>(m_data.m_values[8]), Shift::sin<T>(m_data.m_values[9]), Shift::sin<T>(m_data.m_values[10]),
            Shift::sin<T>(m_data.m_values[11]), Shift::sin<T>(m_data.m_values[12]), Shift::sin<T>(m_data.m_values[13]),
            Shift::sin<T>(m_data.m_values[14]), Shift::sin<T>(m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::cos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::cos<T>(m_data.m_values[0]), Shift::cos<T>(m_data.m_values[1]),
            Shift::cos<T>(m_data.m_values[2]), Shift::cos<T>(m_data.m_values[3]), Shift::cos<T>(m_data.m_values[4]),
            Shift::cos<T>(m_data.m_values[5]), Shift::cos<T>(m_data.m_values[6]), Shift::cos<T>(m_data.m_values[7]),
            Shift::cos<T>(m_data.m_values[8]), Shift::cos<T>(m_data.m_values[9]), Shift::cos<T>(m_data.m_values[10]),
            Shift::cos<T>(m_data.m_values[11]), Shift::cos<T>(m_data.m_values[12]), Shift::cos<T>(m_data.m_values[13]),
            Shift::cos<T>(m_data.m_values[14]), Shift::cos<T>(m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::tan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::tan<T>(m_data.m_values[0]), Shift::tan<T>(m_data.m_values[1]),
            Shift::tan<T>(m_data.m_values[2]), Shift::tan<T>(m_data.m_values[3]), Shift::tan<T>(m_data.m_values[4]),
            Shift::tan<T>(m_data.m_values[5]), Shift::tan<T>(m_data.m_values[6]), Shift::tan<T>(m_data.m_values[7]),
            Shift::tan<T>(m_data.m_values[8]), Shift::tan<T>(m_data.m_values[9]), Shift::tan<T>(m_data.m_values[10]),
            Shift::tan<T>(m_data.m_values[11]), Shift::tan<T>(m_data.m_values[12]), Shift::tan<T>(m_data.m_values[13]),
            Shift::tan<T>(m_data.m_values[14]), Shift::tan<T>(m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::sincos(SIMD16& cosReturn) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        SIMD16 ret;
        ret.m_data.m_values[0] = Shift::sincos<T>(m_data.m_values[0], cosReturn->m_data.m_values[0]);
        ret.m_data.m_values[1] = Shift::sincos<T>(m_data.m_values[1], cosReturn->m_data.m_values[1]);
        ret.m_data.m_values[2] = Shift::sincos<T>(m_data.m_values[2], cosReturn->m_data.m_values[2]);
        ret.m_data.m_values[3] = Shift::sincos<T>(m_data.m_values[3], cosReturn->m_data.m_values[3]);
        ret.m_data.m_values[4] = Shift::sincos<T>(m_data.m_values[4], cosReturn->m_data.m_values[4]);
        ret.m_data.m_values[5] = Shift::sincos<T>(m_data.m_values[5], cosReturn->m_data.m_values[5]);
        ret.m_data.m_values[6] = Shift::sincos<T>(m_data.m_values[6], cosReturn->m_data.m_values[6]);
        ret.m_data.m_values[7] = Shift::sincos<T>(m_data.m_values[7], cosReturn->m_data.m_values[7]);
        ret.m_data.m_values[8] = Shift::sincos<T>(m_data.m_values[8], cosReturn->m_data.m_values[8]);
        ret.m_data.m_values[9] = Shift::sincos<T>(m_data.m_values[9], cosReturn->m_data.m_values[9]);
        ret.m_data.m_values[10] = Shift::sincos<T>(m_data.m_values[10], cosReturn->m_data.m_values[10]);
        ret.m_data.m_values[11] = Shift::sincos<T>(m_data.m_values[11], cosReturn->m_data.m_values[11]);
        ret.m_data.m_values[12] = Shift::sincos<T>(m_data.m_values[12], cosReturn->m_data.m_values[12]);
        ret.m_data.m_values[13] = Shift::sincos<T>(m_data.m_values[13], cosReturn->m_data.m_values[13]);
        ret.m_data.m_values[14] = Shift::sincos<T>(m_data.m_values[14], cosReturn->m_data.m_values[14]);
        ret.m_data.m_values[15] = Shift::sincos<T>(m_data.m_values[15], cosReturn->m_data.m_values[15]);
        return ret;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::asin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::asin<T>(m_data.m_values[0]), Shift::asin<T>(m_data.m_values[1]),
            Shift::asin<T>(m_data.m_values[2]), Shift::asin<T>(m_data.m_values[3]), Shift::asin<T>(m_data.m_values[4]),
            Shift::asin<T>(m_data.m_values[5]), Shift::asin<T>(m_data.m_values[6]), Shift::asin<T>(m_data.m_values[7]),
            Shift::asin<T>(m_data.m_values[8]), Shift::asin<T>(m_data.m_values[9]), Shift::asin<T>(m_data.m_values[10]),
            Shift::asin<T>(m_data.m_values[11]), Shift::asin<T>(m_data.m_values[12]),
            Shift::asin<T>(m_data.m_values[13]), Shift::asin<T>(m_data.m_values[14]),
            Shift::asin<T>(m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::acos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::acos<T>(m_data.m_values[0]), Shift::acos<T>(m_data.m_values[1]),
            Shift::acos<T>(m_data.m_values[2]), Shift::acos<T>(m_data.m_values[3]), Shift::acos<T>(m_data.m_values[4]),
            Shift::acos<T>(m_data.m_values[5]), Shift::acos<T>(m_data.m_values[6]), Shift::acos<T>(m_data.m_values[7]),
            Shift::acos<T>(m_data.m_values[8]), Shift::acos<T>(m_data.m_values[9]), Shift::acos<T>(m_data.m_values[10]),
            Shift::acos<T>(m_data.m_values[11]), Shift::acos<T>(m_data.m_values[12]),
            Shift::acos<T>(m_data.m_values[13]), Shift::acos<T>(m_data.m_values[14]),
            Shift::acos<T>(m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::atan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::atan<T>(m_data.m_values[0]), Shift::atan<T>(m_data.m_values[1]),
            Shift::atan<T>(m_data.m_values[2]), Shift::atan<T>(m_data.m_values[3]), Shift::atan<T>(m_data.m_values[4]),
            Shift::atan<T>(m_data.m_values[5]), Shift::atan<T>(m_data.m_values[6]), Shift::atan<T>(m_data.m_values[7]),
            Shift::atan<T>(m_data.m_values[8]), Shift::atan<T>(m_data.m_values[9]), Shift::atan<T>(m_data.m_values[10]),
            Shift::atan<T>(m_data.m_values[11]), Shift::atan<T>(m_data.m_values[12]),
            Shift::atan<T>(m_data.m_values[13]), Shift::atan<T>(m_data.m_values[14]),
            Shift::atan<T>(m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::atan2(const SIMD16<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(Shift::atan2<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::atan2<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::atan2<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::atan2<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::atan2<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::atan2<T>(m_data.m_values[5], other.m_data.m_values[5]),
            Shift::atan2<T>(m_data.m_values[6], other.m_data.m_values[6]),
            Shift::atan2<T>(m_data.m_values[7], other.m_data.m_values[7]),
            Shift::atan2<T>(m_data.m_values[8], other.m_data.m_values[8]),
            Shift::atan2<T>(m_data.m_values[9], other.m_data.m_values[9]),
            Shift::atan2<T>(m_data.m_values[10], other.m_data.m_values[10]),
            Shift::atan2<T>(m_data.m_values[11], other.m_data.m_values[11]),
            Shift::atan2<T>(m_data.m_values[12], other.m_data.m_values[12]),
            Shift::atan2<T>(m_data.m_values[13], other.m_data.m_values[13]),
            Shift::atan2<T>(m_data.m_values[14], other.m_data.m_values[14]),
            Shift::atan2<T>(m_data.m_values[15], other.m_data.m_values[15]));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::insert2(const SIMD16<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
    static_assert(Index0 < 2 && Index1 < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(getValue4<0>().insert2<Index0, Index1>(other.template getQuadFloat<0>()),
            getValue4<1>().insert2<Index0, Index1>(other.template getQuadFloat<1>()),
            getValue4<2>().insert2<Index0, Index1>(other.template getQuadFloat<2>()),
            getValue4<3>().insert2<Index0, Index1>(other.template getQuadFloat<3>()));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::insert4(const SIMD16<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
    static_assert(Index0 < 4 && Index1 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(getValue4<0>().insert<Index0, Index1>(other.template getValue4<0>()),
            getValue4<1>().insert<Index0, Index1>(other.template getValue4<1>()),
            getValue4<2>().insert<Index0, Index1>(other.template getValue4<2>()),
            getValue4<3>().insert<Index0, Index1>(other.template getValue4<3>()));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::insert8(const SIMD16<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
    static_assert(Index0 < 8 && Index1 < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(getValue8<0>().insert<Index0, Index1>(other.template getValue8<0>()),
            getValue8<1>().insert<Index0, Index1>(other.template getValue8<1>()));
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7,
    SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::blend8(const SIMD16<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const auto temp0 = (Elem0) ? SIMD2<T, Width>(other.m_data.m_values[0], other.m_data.m_values[8]) :
                                     SIMD2<T, Width>(m_data.m_values[0], m_data.m_values[8]);
        const auto temp1 = (Elem1) ? SIMD2<T, Width>(other.m_data.m_values[1], other.m_data.m_values[9]) :
                                     SIMD2<T, Width>(m_data.m_values[1], m_data.m_values[9]);
        const auto temp2 = (Elem2) ? SIMD2<T, Width>(other.m_data.m_values[2], other.m_data.m_values[10]) :
                                     SIMD2<T, Width>(m_data.m_values[2], m_data.m_values[10]);
        const auto temp3 = (Elem3) ? SIMD2<T, Width>(other.m_data.m_values[3], other.m_data.m_values[11]) :
                                     SIMD2<T, Width>(m_data.m_values[3], m_data.m_values[11]);
        const auto temp4 = (Elem4) ? SIMD2<T, Width>(other.m_data.m_values[4], other.m_data.m_values[12]) :
                                     SIMD2<T, Width>(m_data.m_values[4], m_data.m_values[12]);
        const auto temp5 = (Elem5) ? SIMD2<T, Width>(other.m_data.m_values[5], other.m_data.m_values[13]) :
                                     SIMD2<T, Width>(m_data.m_values[5], m_data.m_values[13]);
        const auto temp6 = (Elem6) ? SIMD2<T, Width>(other.m_data.m_values[6], other.m_data.m_values[14]) :
                                     SIMD2<T, Width>(m_data.m_values[6], m_data.m_values[14]);
        const auto temp7 = (Elem7) ? SIMD2<T, Width>(other.m_data.m_values[7], other.m_data.m_values[15]) :
                                     SIMD2<T, Width>(m_data.m_values[7], m_data.m_values[15]);
        return SIMD16(temp0.m_data.m_values[0], temp1.m_data.m_values[0], temp2.m_data.m_values[0],
            temp3.m_data.m_values[0], temp4.m_data.m_values[0], temp5.m_data.m_values[0], temp6.m_data.m_values[0],
            temp7.m_data.m_values[0], temp0.m_data.m_values[1], temp1.m_data.m_values[1], temp2.m_data.m_values[1],
            temp3.m_data.m_values[1], temp4.m_data.m_values[1], temp5.m_data.m_values[1], temp6.m_data.m_values[1],
            temp7.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::blend4(const SIMD16<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const auto temp0 = (Elem0) ?
            SIMD4<T, Width>(other.m_data.m_values[0], other.m_data.m_values[4], other.m_data.m_values[8],
                other.m_data.m_values[12]) :
            SIMD4<T, Width>(m_data.m_values[0], m_data.m_values[4], m_data.m_values[8], m_data.m_values[12]);
        const auto temp1 = (Elem1) ?
            SIMD4<T, Width>(other.m_data.m_values[1], other.m_data.m_values[5], other.m_data.m_values[9],
                other.m_data.m_values[13]) :
            SIMD4<T, Width>(m_data.m_values[1], m_data.m_values[5], m_data.m_values[9], m_data.m_values[13]);
        const auto temp2 = (Elem2) ?
            SIMD4<T, Width>(other.m_data.m_values[2], other.m_data.m_values[6], other.m_data.m_values[10],
                other.m_data.m_values[14]) :
            SIMD4<T, Width>(m_data.m_values[2], m_data.m_values[6], m_data.m_values[10], m_data.m_values[14]);
        const auto temp3 = (Elem3) ?
            SIMD4<T, Width>(other.m_data.m_values[3], other.m_data.m_values[7], other.m_data.m_values[11],
                other.m_data.m_values[15]) :
            SIMD4<T, Width>(m_data.m_values[3], m_data.m_values[7], m_data.m_values[11], m_data.m_values[15]);
        return SIMD16(temp0.m_data.m_values[0], temp1.m_data.m_values[0], temp2.m_data.m_values[0],
            temp3.m_data.m_values[0], temp0.m_data.m_values[1], temp1.m_data.m_values[1], temp2.m_data.m_values[1],
            temp3.m_data.m_values[1], temp0.m_data.m_values[2], temp1.m_data.m_values[2], temp2.m_data.m_values[2],
            temp3.m_data.m_values[2], temp0.m_data.m_values[3], temp1.m_data.m_values[3], temp2.m_data.m_values[3],
            temp3.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::blend2(const SIMD16<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const auto temp0 = (Elem0) ?
            SIMD8<T, Width>(other.m_data.m_values[0], other.m_data.m_values[2], other.m_data.m_values[4],
                other.m_data.m_values[6], other.m_data.m_values[8], other.m_data.m_values[10],
                other.m_data.m_values[12], other.m_data.m_values[14]) :
            SIMD8<T, Width>(m_data.m_values[0], m_data.m_values[2], m_data.m_values[4], m_data.m_values[6],
                m_data.m_values[8], m_data.m_values[10], m_data.m_values[12], m_data.m_values[14]);
        const auto temp1 = (Elem1) ?
            SIMD8<T, Width>(other.m_data.m_values[1], other.m_data.m_values[3], other.m_data.m_values[5],
                other.m_data.m_values[7], other.m_data.m_values[9], other.m_data.m_values[11],
                other.m_data.m_values[13], other.m_data.m_values[15]) :
            SIMD8<T, Width>(m_data.m_values[1], m_data.m_values[3], m_data.m_values[5], m_data.m_values[7],
                m_data.m_values[9], m_data.m_values[11], m_data.m_values[13], m_data.m_values[15]);
        return SIMD16(temp0.m_data.m_values[0], temp1.m_data.m_values[0], temp0.m_data.m_values[1],
            temp1.m_data.m_values[1], temp0.m_data.m_values[2], temp1.m_data.m_values[2], temp0.m_data.m_values[3],
            temp1.m_data.m_values[3], temp0.m_data.m_values[4], temp1.m_data.m_values[4], temp0.m_data.m_values[5],
            temp1.m_data.m_values[5], temp0.m_data.m_values[6], temp1.m_data.m_values[6], temp0.m_data.m_values[7],
            temp1.m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3, uint32 Index4, uint32 Index5, uint32 Index6,
    uint32 Index7>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::shuffle8() const noexcept
{
    static_assert(
        Index0 < 8 && Index1 < 8 && Index2 < 8 && Index3 < 8 && Index4 < 8 && Index5 < 8 && Index6 < 8 && Index7 < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(getValue8<0>().template shuffle<Index0, Index1, Index2, Index3, Index4, Index5, Index6, Index7>(),
            getValue8<1>().template shuffle<Index0, Index1, Index2, Index3, Index4, Index5, Index6, Index7>());
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::shuffle4() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(getValue4<0>().template shuffle<Index0, Index1, Index2, Index3>(),
            getValue4<1>().template shuffle<Index0, Index1, Index2, Index3>(),
            getValue4<2>().template shuffle<Index0, Index1, Index2, Index3>(),
            getValue4<3>().template shuffle<Index0, Index1, Index2, Index3>());
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::shuffle2() const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(getValue4<0>().shuffleDual<Index0, Index1>(), getValue4<1>().shuffleDual<Index0, Index1>(),
            getValue4<2>().shuffleDual<Index0, Index1>(), getValue4<3>().shuffleDual<Index0, Index1>());
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD16<T, Width> SIMD16<T, Width>::shuffleH4() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(getValue4<Index0>(), getValue4<Index1>(), getValue4<Index2>(), getValue4<Index3>());
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator+(const SIMD16<T, Width>& other1, const SIMD16<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
            other1.m_data.m_values[1] + other2.m_data.m_values[1],
            other1.m_data.m_values[2] + other2.m_data.m_values[2],
            other1.m_data.m_values[3] + other2.m_data.m_values[3],
            other1.m_data.m_values[4] + other2.m_data.m_values[4],
            other1.m_data.m_values[5] + other2.m_data.m_values[5],
            other1.m_data.m_values[6] + other2.m_data.m_values[6],
            other1.m_data.m_values[7] + other2.m_data.m_values[7],
            other1.m_data.m_values[8] + other2.m_data.m_values[8],
            other1.m_data.m_values[9] + other2.m_data.m_values[9],
            other1.m_data.m_values[10] + other2.m_data.m_values[10],
            other1.m_data.m_values[11] + other2.m_data.m_values[11],
            other1.m_data.m_values[12] + other2.m_data.m_values[12],
            other1.m_data.m_values[13] + other2.m_data.m_values[13],
            other1.m_data.m_values[14] + other2.m_data.m_values[14],
            other1.m_data.m_values[15] + other2.m_data.m_values[15]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator+(const SIMD16<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD16<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_value,
            other1.m_data.m_values[1] + other2.m_data.m_value, other1.m_data.m_values[2] + other2.m_data.m_value,
            other1.m_data.m_values[3] + other2.m_data.m_value, other1.m_data.m_values[4] + other2.m_data.m_value,
            other1.m_data.m_values[5] + other2.m_data.m_value, other1.m_data.m_values[6] + other2.m_data.m_value,
            other1.m_data.m_values[7] + other2.m_data.m_value, other1.m_data.m_values[8] + other2.m_data.m_value,
            other1.m_data.m_values[9] + other2.m_data.m_value, other1.m_data.m_values[10] + other2.m_data.m_value,
            other1.m_data.m_values[11] + other2.m_data.m_value, other1.m_data.m_values[12] + other2.m_data.m_value,
            other1.m_data.m_values[13] + other2.m_data.m_value, other1.m_data.m_values[14] + other2.m_data.m_value,
            other1.m_data.m_values[15] + other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator+(const SIMD16<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
            other1.m_data.m_values[1] + other2.m_data.m_values[1],
            other1.m_data.m_values[2] + other2.m_data.m_values[2],
            other1.m_data.m_values[3] + other2.m_data.m_values[3],
            other1.m_data.m_values[4] + other2.m_data.m_values[0],
            other1.m_data.m_values[5] + other2.m_data.m_values[1],
            other1.m_data.m_values[6] + other2.m_data.m_values[2],
            other1.m_data.m_values[7] + other2.m_data.m_values[3],
            other1.m_data.m_values[8] + other2.m_data.m_values[0],
            other1.m_data.m_values[9] + other2.m_data.m_values[1],
            other1.m_data.m_values[10] + other2.m_data.m_values[2],
            other1.m_data.m_values[11] + other2.m_data.m_values[3],
            other1.m_data.m_values[12] + other2.m_data.m_values[0],
            other1.m_data.m_values[13] + other2.m_data.m_values[1],
            other1.m_data.m_values[14] + other2.m_data.m_values[2],
            other1.m_data.m_values[15] + other2.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator+(const SIMD16<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
            other1.m_data.m_values[1] + other2.m_data.m_values[1],
            other1.m_data.m_values[2] + other2.m_data.m_values[2],
            other1.m_data.m_values[3] + other2.m_data.m_values[3],
            other1.m_data.m_values[4] + other2.m_data.m_values[4],
            other1.m_data.m_values[5] + other2.m_data.m_values[5],
            other1.m_data.m_values[6] + other2.m_data.m_values[6],
            other1.m_data.m_values[7] + other2.m_data.m_values[7],
            other1.m_data.m_values[8] + other2.m_data.m_values[0],
            other1.m_data.m_values[9] + other2.m_data.m_values[1],
            other1.m_data.m_values[10] + other2.m_data.m_values[2],
            other1.m_data.m_values[11] + other2.m_data.m_values[3],
            other1.m_data.m_values[12] + other2.m_data.m_values[4],
            other1.m_data.m_values[13] + other2.m_data.m_values[5],
            other1.m_data.m_values[14] + other2.m_data.m_values[6],
            other1.m_data.m_values[15] + other2.m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator-(const SIMD16<T, Width>& other1, const SIMD16<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
            other1.m_data.m_values[1] - other2.m_data.m_values[1],
            other1.m_data.m_values[2] - other2.m_data.m_values[2],
            other1.m_data.m_values[3] - other2.m_data.m_values[3],
            other1.m_data.m_values[4] - other2.m_data.m_values[4],
            other1.m_data.m_values[5] - other2.m_data.m_values[5],
            other1.m_data.m_values[6] - other2.m_data.m_values[6],
            other1.m_data.m_values[7] - other2.m_data.m_values[7],
            other1.m_data.m_values[8] - other2.m_data.m_values[8],
            other1.m_data.m_values[9] - other2.m_data.m_values[9],
            other1.m_data.m_values[10] - other2.m_data.m_values[10],
            other1.m_data.m_values[11] - other2.m_data.m_values[11],
            other1.m_data.m_values[12] - other2.m_data.m_values[12],
            other1.m_data.m_values[13] - other2.m_data.m_values[13],
            other1.m_data.m_values[14] - other2.m_data.m_values[14],
            other1.m_data.m_values[15] - other2.m_data.m_values[15]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator-(const SIMD16<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD16<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_value,
            other1.m_data.m_values[1] - other2.m_data.m_value, other1.m_data.m_values[2] - other2.m_data.m_value,
            other1.m_data.m_values[3] - other2.m_data.m_value, other1.m_data.m_values[4] - other2.m_data.m_value,
            other1.m_data.m_values[5] - other2.m_data.m_value, other1.m_data.m_values[6] - other2.m_data.m_value,
            other1.m_data.m_values[7] - other2.m_data.m_value, other1.m_data.m_values[8] - other2.m_data.m_value,
            other1.m_data.m_values[9] - other2.m_data.m_value, other1.m_data.m_values[10] - other2.m_data.m_value,
            other1.m_data.m_values[11] - other2.m_data.m_value, other1.m_data.m_values[12] - other2.m_data.m_value,
            other1.m_data.m_values[13] - other2.m_data.m_value, other1.m_data.m_values[14] - other2.m_data.m_value,
            other1.m_data.m_values[15] - other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator-(const SIMDBase<T, Width2>& other1, const SIMD16<T, Width>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD16<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16<T, Width>(other1.m_data.m_value - other2.m_data.m_values[0],
            other1.m_data.m_value - other2.m_data.m_values[1], other1.m_data.m_value - other2.m_data.m_values[2],
            other1.m_data.m_value - other2.m_data.m_values[3], other1.m_data.m_value - other2.m_data.m_values[4],
            other1.m_data.m_value - other2.m_data.m_values[5], other1.m_data.m_value - other2.m_data.m_values[6],
            other1.m_data.m_value - other2.m_data.m_values[7], other1.m_data.m_value - other2.m_data.m_values[8],
            other1.m_data.m_value - other2.m_data.m_values[9], other1.m_data.m_value - other2.m_data.m_values[10],
            other1.m_data.m_value - other2.m_data.m_values[11], other1.m_data.m_value - other2.m_data.m_values[12],
            other1.m_data.m_value - other2.m_data.m_values[13], other1.m_data.m_value - other2.m_data.m_values[14],
            other1.m_data.m_value - other2.m_data.m_values[15]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator-(const SIMD16<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
            other1.m_data.m_values[1] - other2.m_data.m_values[1],
            other1.m_data.m_values[2] - other2.m_data.m_values[2],
            other1.m_data.m_values[3] - other2.m_data.m_values[3],
            other1.m_data.m_values[4] - other2.m_data.m_values[0],
            other1.m_data.m_values[5] - other2.m_data.m_values[1],
            other1.m_data.m_values[6] - other2.m_data.m_values[2],
            other1.m_data.m_values[7] - other2.m_data.m_values[3],
            other1.m_data.m_values[8] - other2.m_data.m_values[0],
            other1.m_data.m_values[9] - other2.m_data.m_values[1],
            other1.m_data.m_values[10] - other2.m_data.m_values[2],
            other1.m_data.m_values[11] - other2.m_data.m_values[3],
            other1.m_data.m_values[12] - other2.m_data.m_values[0],
            other1.m_data.m_values[13] - other2.m_data.m_values[1],
            other1.m_data.m_values[14] - other2.m_data.m_values[2],
            other1.m_data.m_values[15] - other2.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator-(const SIMD16<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
            other1.m_data.m_values[1] - other2.m_data.m_values[1],
            other1.m_data.m_values[2] - other2.m_data.m_values[2],
            other1.m_data.m_values[3] - other2.m_data.m_values[3],
            other1.m_data.m_values[4] - other2.m_data.m_values[4],
            other1.m_data.m_values[5] - other2.m_data.m_values[5],
            other1.m_data.m_values[6] - other2.m_data.m_values[6],
            other1.m_data.m_values[7] - other2.m_data.m_values[7],
            other1.m_data.m_values[8] - other2.m_data.m_values[0],
            other1.m_data.m_values[9] - other2.m_data.m_values[1],
            other1.m_data.m_values[10] - other2.m_data.m_values[2],
            other1.m_data.m_values[11] - other2.m_data.m_values[3],
            other1.m_data.m_values[12] - other2.m_data.m_values[4],
            other1.m_data.m_values[13] - other2.m_data.m_values[5],
            other1.m_data.m_values[14] - other2.m_data.m_values[6],
            other1.m_data.m_values[15] - other2.m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator*(const SIMD16<T, Width>& other1, const SIMD16<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
            other1.m_data.m_values[1] * other2.m_data.m_values[1],
            other1.m_data.m_values[2] * other2.m_data.m_values[2],
            other1.m_data.m_values[3] * other2.m_data.m_values[3],
            other1.m_data.m_values[4] * other2.m_data.m_values[4],
            other1.m_data.m_values[5] * other2.m_data.m_values[5],
            other1.m_data.m_values[6] * other2.m_data.m_values[6],
            other1.m_data.m_values[7] * other2.m_data.m_values[7],
            other1.m_data.m_values[8] * other2.m_data.m_values[8],
            other1.m_data.m_values[9] * other2.m_data.m_values[9],
            other1.m_data.m_values[10] * other2.m_data.m_values[10],
            other1.m_data.m_values[11] * other2.m_data.m_values[11],
            other1.m_data.m_values[12] * other2.m_data.m_values[12],
            other1.m_data.m_values[13] * other2.m_data.m_values[13],
            other1.m_data.m_values[14] * other2.m_data.m_values[14],
            other1.m_data.m_values[15] * other2.m_data.m_values[15]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator*(const SIMD16<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD16<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_value,
            other1.m_data.m_values[1] * other2.m_data.m_value, other1.m_data.m_values[2] * other2.m_data.m_value,
            other1.m_data.m_values[3] * other2.m_data.m_value, other1.m_data.m_values[4] * other2.m_data.m_value,
            other1.m_data.m_values[5] * other2.m_data.m_value, other1.m_data.m_values[6] * other2.m_data.m_value,
            other1.m_data.m_values[7] * other2.m_data.m_value, other1.m_data.m_values[8] * other2.m_data.m_value,
            other1.m_data.m_values[9] * other2.m_data.m_value, other1.m_data.m_values[10] * other2.m_data.m_value,
            other1.m_data.m_values[11] * other2.m_data.m_value, other1.m_data.m_values[12] * other2.m_data.m_value,
            other1.m_data.m_values[13] * other2.m_data.m_value, other1.m_data.m_values[14] * other2.m_data.m_value,
            other1.m_data.m_values[15] * other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator*(const SIMD16<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
            other1.m_data.m_values[1] * other2.m_data.m_values[1],
            other1.m_data.m_values[2] * other2.m_data.m_values[2],
            other1.m_data.m_values[3] * other2.m_data.m_values[3],
            other1.m_data.m_values[4] * other2.m_data.m_values[0],
            other1.m_data.m_values[5] * other2.m_data.m_values[1],
            other1.m_data.m_values[6] * other2.m_data.m_values[2],
            other1.m_data.m_values[7] * other2.m_data.m_values[3],
            other1.m_data.m_values[8] * other2.m_data.m_values[0],
            other1.m_data.m_values[9] * other2.m_data.m_values[1],
            other1.m_data.m_values[10] * other2.m_data.m_values[2],
            other1.m_data.m_values[11] * other2.m_data.m_values[3],
            other1.m_data.m_values[12] * other2.m_data.m_values[0],
            other1.m_data.m_values[13] * other2.m_data.m_values[1],
            other1.m_data.m_values[14] * other2.m_data.m_values[2],
            other1.m_data.m_values[15] * other2.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator*(const SIMD16<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
            other1.m_data.m_values[1] * other2.m_data.m_values[1],
            other1.m_data.m_values[2] * other2.m_data.m_values[2],
            other1.m_data.m_values[3] * other2.m_data.m_values[3],
            other1.m_data.m_values[4] * other2.m_data.m_values[4],
            other1.m_data.m_values[5] * other2.m_data.m_values[5],
            other1.m_data.m_values[6] * other2.m_data.m_values[6],
            other1.m_data.m_values[7] * other2.m_data.m_values[7],
            other1.m_data.m_values[8] * other2.m_data.m_values[0],
            other1.m_data.m_values[9] * other2.m_data.m_values[1],
            other1.m_data.m_values[10] * other2.m_data.m_values[2],
            other1.m_data.m_values[11] * other2.m_data.m_values[3],
            other1.m_data.m_values[12] * other2.m_data.m_values[4],
            other1.m_data.m_values[13] * other2.m_data.m_values[5],
            other1.m_data.m_values[14] * other2.m_data.m_values[6],
            other1.m_data.m_values[15] * other2.m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator/(const SIMD16<T, Width>& other1, const SIMD16<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
            other1.m_data.m_values[1] / other2.m_data.m_values[1],
            other1.m_data.m_values[2] / other2.m_data.m_values[2],
            other1.m_data.m_values[3] / other2.m_data.m_values[3],
            other1.m_data.m_values[4] / other2.m_data.m_values[4],
            other1.m_data.m_values[5] / other2.m_data.m_values[5],
            other1.m_data.m_values[6] / other2.m_data.m_values[6],
            other1.m_data.m_values[7] / other2.m_data.m_values[7],
            other1.m_data.m_values[8] / other2.m_data.m_values[8],
            other1.m_data.m_values[9] / other2.m_data.m_values[9],
            other1.m_data.m_values[10] / other2.m_data.m_values[10],
            other1.m_data.m_values[11] / other2.m_data.m_values[11],
            other1.m_data.m_values[12] / other2.m_data.m_values[12],
            other1.m_data.m_values[13] / other2.m_data.m_values[13],
            other1.m_data.m_values[14] / other2.m_data.m_values[14],
            other1.m_data.m_values[15] / other2.m_data.m_values[15]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator/(const SIMD16<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD16<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD16<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_value,
                other1.m_data.m_values[1] / other2.m_data.m_value, other1.m_data.m_values[2] / other2.m_data.m_value,
                other1.m_data.m_values[3] / other2.m_data.m_value, other1.m_data.m_values[4] / other2.m_data.m_value,
                other1.m_data.m_values[5] / other2.m_data.m_value, other1.m_data.m_values[6] / other2.m_data.m_value,
                other1.m_data.m_values[7] / other2.m_data.m_value, other1.m_data.m_values[8] / other2.m_data.m_value,
                other1.m_data.m_values[9] / other2.m_data.m_value, other1.m_data.m_values[10] / other2.m_data.m_value,
                other1.m_data.m_values[11] / other2.m_data.m_value, other1.m_data.m_values[12] / other2.m_data.m_value,
                other1.m_data.m_values[13] / other2.m_data.m_value, other1.m_data.m_values[14] / other2.m_data.m_value,
                other1.m_data.m_values[15] / other2.m_data.m_value);
        } else {
            const T divisor = Shift::recip<T>(other2.m_data.m_value);
            return SIMD16<T, Width>(other1.m_data.m_values[0] * divisor, other1.m_data.m_values[1] * divisor,
                other1.m_data.m_values[2] * divisor, other1.m_data.m_values[3] * divisor,
                other1.m_data.m_values[4] * divisor, other1.m_data.m_values[5] * divisor,
                other1.m_data.m_values[6] * divisor, other1.m_data.m_values[7] * divisor,
                other1.m_data.m_values[8] * divisor, other1.m_data.m_values[9] * divisor,
                other1.m_data.m_values[10] * divisor, other1.m_data.m_values[11] * divisor,
                other1.m_data.m_values[12] * divisor, other1.m_data.m_values[13] * divisor,
                other1.m_data.m_values[14] * divisor, other1.m_data.m_values[15] * divisor);
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator/(const SIMDBase<T, Width2>& other1, const SIMD16<T, Width>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD16<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16<T, Width>(other1.m_data.m_value / other2.m_data.m_values[0],
            other1.m_data.m_value / other2.m_data.m_values[1], other1.m_data.m_value / other2.m_data.m_values[2],
            other1.m_data.m_value / other2.m_data.m_values[3], other1.m_data.m_value / other2.m_data.m_values[4],
            other1.m_data.m_value / other2.m_data.m_values[5], other1.m_data.m_value / other2.m_data.m_values[6],
            other1.m_data.m_value / other2.m_data.m_values[7], other1.m_data.m_value / other2.m_data.m_values[8],
            other1.m_data.m_value / other2.m_data.m_values[9], other1.m_data.m_value / other2.m_data.m_values[10],
            other1.m_data.m_value / other2.m_data.m_values[11], other1.m_data.m_value / other2.m_data.m_values[12],
            other1.m_data.m_value / other2.m_data.m_values[13], other1.m_data.m_value / other2.m_data.m_values[14],
            other1.m_data.m_value / other2.m_data.m_values[15]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator/(const SIMD16<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD16<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
                other1.m_data.m_values[1] / other2.m_data.m_values[1],
                other1.m_data.m_values[2] / other2.m_data.m_values[2],
                other1.m_data.m_values[3] / other2.m_data.m_values[3],
                other1.m_data.m_values[4] / other2.m_data.m_values[0],
                other1.m_data.m_values[5] / other2.m_data.m_values[1],
                other1.m_data.m_values[6] / other2.m_data.m_values[2],
                other1.m_data.m_values[7] / other2.m_data.m_values[3],
                other1.m_data.m_values[8] / other2.m_data.m_values[0],
                other1.m_data.m_values[9] / other2.m_data.m_values[1],
                other1.m_data.m_values[10] / other2.m_data.m_values[2],
                other1.m_data.m_values[11] / other2.m_data.m_values[3],
                other1.m_data.m_values[12] / other2.m_data.m_values[0],
                other1.m_data.m_values[13] / other2.m_data.m_values[1],
                other1.m_data.m_values[14] / other2.m_data.m_values[2],
                other1.m_data.m_values[15] / other2.m_data.m_values[3]);
        } else {
            const auto recip = other2.reciprocal();
            return SIMD16<T, Width>(other1.m_data.m_values[0] * recip.m_data.m_values[0],
                other1.m_data.m_values[1] * recip.m_data.m_values[1],
                other1.m_data.m_values[2] * recip.m_data.m_values[2],
                other1.m_data.m_values[3] * recip.m_data.m_values[3],
                other1.m_data.m_values[4] * recip.m_data.m_values[0],
                other1.m_data.m_values[5] * recip.m_data.m_values[1],
                other1.m_data.m_values[6] * recip.m_data.m_values[2],
                other1.m_data.m_values[7] * recip.m_data.m_values[3],
                other1.m_data.m_values[8] * recip.m_data.m_values[0],
                other1.m_data.m_values[9] * recip.m_data.m_values[1],
                other1.m_data.m_values[10] * recip.m_data.m_values[2],
                other1.m_data.m_values[11] * recip.m_data.m_values[3],
                other1.m_data.m_values[12] * recip.m_data.m_values[0],
                other1.m_data.m_values[13] * recip.m_data.m_values[1],
                other1.m_data.m_values[14] * recip.m_data.m_values[2],
                other1.m_data.m_values[15] * recip.m_data.m_values[3]);
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator/(const SIMD16<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD16<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
                other1.m_data.m_values[1] / other2.m_data.m_values[1],
                other1.m_data.m_values[2] / other2.m_data.m_values[2],
                other1.m_data.m_values[3] / other2.m_data.m_values[3],
                other1.m_data.m_values[4] / other2.m_data.m_values[4],
                other1.m_data.m_values[5] / other2.m_data.m_values[5],
                other1.m_data.m_values[6] / other2.m_data.m_values[6],
                other1.m_data.m_values[7] / other2.m_data.m_values[7],
                other1.m_data.m_values[8] / other2.m_data.m_values[0],
                other1.m_data.m_values[9] / other2.m_data.m_values[1],
                other1.m_data.m_values[10] / other2.m_data.m_values[2],
                other1.m_data.m_values[11] / other2.m_data.m_values[3],
                other1.m_data.m_values[12] / other2.m_data.m_values[4],
                other1.m_data.m_values[13] / other2.m_data.m_values[5],
                other1.m_data.m_values[14] / other2.m_data.m_values[6],
                other1.m_data.m_values[15] / other2.m_data.m_values[7]);
        } else {
            const auto recip = other2.reciprocal();
            return SIMD16<T, Width>(other1.m_data.m_values[0] * recip.m_data.m_values[0],
                other1.m_data.m_values[1] * recip.m_data.m_values[1],
                other1.m_data.m_values[2] * recip.m_data.m_values[2],
                other1.m_data.m_values[3] * recip.m_data.m_values[3],
                other1.m_data.m_values[4] * recip.m_data.m_values[4],
                other1.m_data.m_values[5] * recip.m_data.m_values[5],
                other1.m_data.m_values[6] * recip.m_data.m_values[6],
                other1.m_data.m_values[7] * recip.m_data.m_values[7],
                other1.m_data.m_values[8] * recip.m_data.m_values[0],
                other1.m_data.m_values[9] * recip.m_data.m_values[1],
                other1.m_data.m_values[10] * recip.m_data.m_values[2],
                other1.m_data.m_values[11] * recip.m_data.m_values[3],
                other1.m_data.m_values[12] * recip.m_data.m_values[4],
                other1.m_data.m_values[13] * recip.m_data.m_values[5],
                other1.m_data.m_values[14] * recip.m_data.m_values[6],
                other1.m_data.m_values[15] * recip.m_data.m_values[7]);
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator-(const SIMD16<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16<T, Width>(-other.m_data.m_values[0], -other.m_data.m_values[1], -other.m_data.m_values[2],
            -other.m_data.m_values[3], -other.m_data.m_values[4], -other.m_data.m_values[5], -other.m_data.m_values[6],
            -other.m_data.m_values[7], -other.m_data.m_values[8], -other.m_data.m_values[9], -other.m_data.m_values[10],
            -other.m_data.m_values[11], -other.m_data.m_values[12], -other.m_data.m_values[13],
            -other.m_data.m_values[14], -other.m_data.m_values[15]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width>& operator+=(SIMD16<T, Width>& other1, const SIMD16<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] += other2.m_data.m_values[0];
        other1.m_data.m_values[1] += other2.m_data.m_values[1];
        other1.m_data.m_values[2] += other2.m_data.m_values[2];
        other1.m_data.m_values[3] += other2.m_data.m_values[3];
        other1.m_data.m_values[4] += other2.m_data.m_values[4];
        other1.m_data.m_values[5] += other2.m_data.m_values[5];
        other1.m_data.m_values[6] += other2.m_data.m_values[6];
        other1.m_data.m_values[7] += other2.m_data.m_values[7];
        other1.m_data.m_values[8] += other2.m_data.m_values[8];
        other1.m_data.m_values[9] += other2.m_data.m_values[9];
        other1.m_data.m_values[10] += other2.m_data.m_values[10];
        other1.m_data.m_values[11] += other2.m_data.m_values[11];
        other1.m_data.m_values[12] += other2.m_data.m_values[12];
        other1.m_data.m_values[13] += other2.m_data.m_values[13];
        other1.m_data.m_values[14] += other2.m_data.m_values[14];
        other1.m_data.m_values[15] += other2.m_data.m_values[15];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width>& operator+=(SIMD16<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD16<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] += other2.m_data.m_value;
        other1.m_data.m_values[1] += other2.m_data.m_value;
        other1.m_data.m_values[2] += other2.m_data.m_value;
        other1.m_data.m_values[3] += other2.m_data.m_value;
        other1.m_data.m_values[4] += other2.m_data.m_value;
        other1.m_data.m_values[5] += other2.m_data.m_value;
        other1.m_data.m_values[6] += other2.m_data.m_value;
        other1.m_data.m_values[7] += other2.m_data.m_value;
        other1.m_data.m_values[8] += other2.m_data.m_value;
        other1.m_data.m_values[9] += other2.m_data.m_value;
        other1.m_data.m_values[10] += other2.m_data.m_value;
        other1.m_data.m_values[11] += other2.m_data.m_value;
        other1.m_data.m_values[12] += other2.m_data.m_value;
        other1.m_data.m_values[13] += other2.m_data.m_value;
        other1.m_data.m_values[14] += other2.m_data.m_value;
        other1.m_data.m_values[15] += other2.m_data.m_value;
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width>& operator-=(SIMD16<T, Width>& other1, const SIMD16<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] -= other2.m_data.m_values[0];
        other1.m_data.m_values[1] -= other2.m_data.m_values[1];
        other1.m_data.m_values[2] -= other2.m_data.m_values[2];
        other1.m_data.m_values[3] -= other2.m_data.m_values[3];
        other1.m_data.m_values[4] -= other2.m_data.m_values[4];
        other1.m_data.m_values[5] -= other2.m_data.m_values[5];
        other1.m_data.m_values[6] -= other2.m_data.m_values[6];
        other1.m_data.m_values[7] -= other2.m_data.m_values[7];
        other1.m_data.m_values[8] -= other2.m_data.m_values[8];
        other1.m_data.m_values[9] -= other2.m_data.m_values[9];
        other1.m_data.m_values[10] -= other2.m_data.m_values[10];
        other1.m_data.m_values[11] -= other2.m_data.m_values[11];
        other1.m_data.m_values[12] -= other2.m_data.m_values[12];
        other1.m_data.m_values[13] -= other2.m_data.m_values[13];
        other1.m_data.m_values[14] -= other2.m_data.m_values[14];
        other1.m_data.m_values[15] -= other2.m_data.m_values[15];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width>& operator-=(SIMD16<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD16<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] -= other2.m_data.m_value;
        other1.m_data.m_values[1] -= other2.m_data.m_value;
        other1.m_data.m_values[2] -= other2.m_data.m_value;
        other1.m_data.m_values[3] -= other2.m_data.m_value;
        other1.m_data.m_values[4] -= other2.m_data.m_value;
        other1.m_data.m_values[5] -= other2.m_data.m_value;
        other1.m_data.m_values[6] -= other2.m_data.m_value;
        other1.m_data.m_values[7] -= other2.m_data.m_value;
        other1.m_data.m_values[8] -= other2.m_data.m_value;
        other1.m_data.m_values[9] -= other2.m_data.m_value;
        other1.m_data.m_values[10] -= other2.m_data.m_value;
        other1.m_data.m_values[11] -= other2.m_data.m_value;
        other1.m_data.m_values[12] -= other2.m_data.m_value;
        other1.m_data.m_values[13] -= other2.m_data.m_value;
        other1.m_data.m_values[14] -= other2.m_data.m_value;
        other1.m_data.m_values[15] -= other2.m_data.m_value;
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width>& operator*=(SIMD16<T, Width>& other1, const SIMD16<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] *= other2.m_data.m_values[0];
        other1.m_data.m_values[1] *= other2.m_data.m_values[1];
        other1.m_data.m_values[2] *= other2.m_data.m_values[2];
        other1.m_data.m_values[3] *= other2.m_data.m_values[3];
        other1.m_data.m_values[4] *= other2.m_data.m_values[4];
        other1.m_data.m_values[5] *= other2.m_data.m_values[5];
        other1.m_data.m_values[6] *= other2.m_data.m_values[6];
        other1.m_data.m_values[7] *= other2.m_data.m_values[7];
        other1.m_data.m_values[8] *= other2.m_data.m_values[8];
        other1.m_data.m_values[9] *= other2.m_data.m_values[9];
        other1.m_data.m_values[10] *= other2.m_data.m_values[10];
        other1.m_data.m_values[11] *= other2.m_data.m_values[11];
        other1.m_data.m_values[12] *= other2.m_data.m_values[12];
        other1.m_data.m_values[13] *= other2.m_data.m_values[13];
        other1.m_data.m_values[14] *= other2.m_data.m_values[14];
        other1.m_data.m_values[15] *= other2.m_data.m_values[15];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width>& operator*=(SIMD16<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD16<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] *= other2.m_data.m_value;
        other1.m_data.m_values[1] *= other2.m_data.m_value;
        other1.m_data.m_values[2] *= other2.m_data.m_value;
        other1.m_data.m_values[3] *= other2.m_data.m_value;
        other1.m_data.m_values[4] *= other2.m_data.m_value;
        other1.m_data.m_values[5] *= other2.m_data.m_value;
        other1.m_data.m_values[6] *= other2.m_data.m_value;
        other1.m_data.m_values[7] *= other2.m_data.m_value;
        other1.m_data.m_values[8] *= other2.m_data.m_value;
        other1.m_data.m_values[9] *= other2.m_data.m_value;
        other1.m_data.m_values[10] *= other2.m_data.m_value;
        other1.m_data.m_values[11] *= other2.m_data.m_value;
        other1.m_data.m_values[12] *= other2.m_data.m_value;
        other1.m_data.m_values[13] *= other2.m_data.m_value;
        other1.m_data.m_values[14] *= other2.m_data.m_value;
        other1.m_data.m_values[15] *= other2.m_data.m_value;
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width>& operator/=(SIMD16<T, Width>& other1, const SIMD16<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] /= other2.m_data.m_values[0];
        other1.m_data.m_values[1] /= other2.m_data.m_values[1];
        other1.m_data.m_values[2] /= other2.m_data.m_values[2];
        other1.m_data.m_values[3] /= other2.m_data.m_values[3];
        other1.m_data.m_values[4] /= other2.m_data.m_values[4];
        other1.m_data.m_values[5] /= other2.m_data.m_values[5];
        other1.m_data.m_values[6] /= other2.m_data.m_values[6];
        other1.m_data.m_values[7] /= other2.m_data.m_values[7];
        other1.m_data.m_values[8] /= other2.m_data.m_values[8];
        other1.m_data.m_values[9] /= other2.m_data.m_values[9];
        other1.m_data.m_values[10] /= other2.m_data.m_values[10];
        other1.m_data.m_values[11] /= other2.m_data.m_values[11];
        other1.m_data.m_values[12] /= other2.m_data.m_values[12];
        other1.m_data.m_values[13] /= other2.m_data.m_values[13];
        other1.m_data.m_values[14] /= other2.m_data.m_values[14];
        other1.m_data.m_values[15] /= other2.m_data.m_values[15];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width>& operator/=(SIMD16<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD16<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            other1.m_data.m_values[0] /= other2.m_data.m_value;
            other1.m_data.m_values[1] /= other2.m_data.m_value;
            other1.m_data.m_values[2] /= other2.m_data.m_value;
            other1.m_data.m_values[3] /= other2.m_data.m_value;
            other1.m_data.m_values[4] /= other2.m_data.m_value;
            other1.m_data.m_values[5] /= other2.m_data.m_value;
            other1.m_data.m_values[6] /= other2.m_data.m_value;
            other1.m_data.m_values[7] /= other2.m_data.m_value;
            other1.m_data.m_values[8] /= other2.m_data.m_value;
            other1.m_data.m_values[9] /= other2.m_data.m_value;
            other1.m_data.m_values[10] /= other2.m_data.m_value;
            other1.m_data.m_values[11] /= other2.m_data.m_value;
            other1.m_data.m_values[12] /= other2.m_data.m_value;
            other1.m_data.m_values[13] /= other2.m_data.m_value;
            other1.m_data.m_values[14] /= other2.m_data.m_value;
            other1.m_data.m_values[15] /= other2.m_data.m_value;
        } else {
            const T divisor = Shift::recip<T>(other2.m_data.m_value);
            other1.m_data.m_values[0] *= divisor;
            other1.m_data.m_values[1] *= divisor;
            other1.m_data.m_values[2] *= divisor;
            other1.m_data.m_values[3] *= divisor;
            other1.m_data.m_values[4] *= divisor;
            other1.m_data.m_values[5] *= divisor;
            other1.m_data.m_values[6] *= divisor;
            other1.m_data.m_values[7] *= divisor;
            other1.m_data.m_values[8] *= divisor;
            other1.m_data.m_values[9] *= divisor;
            other1.m_data.m_values[10] *= divisor;
            other1.m_data.m_values[11] *= divisor;
            other1.m_data.m_values[12] *= divisor;
            other1.m_data.m_values[13] *= divisor;
            other1.m_data.m_values[14] *= divisor;
            other1.m_data.m_values[15] *= divisor;
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator&(const SIMD16<T, Width>& other1, const SIMD16<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(other1.m_data.m_values[0] & other2.m_data.m_values[0],
                other1.m_data.m_values[1] & other2.m_data.m_values[1],
                other1.m_data.m_values[2] & other2.m_data.m_values[2],
                other1.m_data.m_values[3] & other2.m_data.m_values[3],
                other1.m_data.m_values[4] & other2.m_data.m_values[4],
                other1.m_data.m_values[5] & other2.m_data.m_values[5],
                other1.m_data.m_values[6] & other2.m_data.m_values[6],
                other1.m_data.m_values[7] & other2.m_data.m_values[7],
                other1.m_data.m_values[8] & other2.m_data.m_values[8],
                other1.m_data.m_values[9] & other2.m_data.m_values[9],
                other1.m_data.m_values[10] & other2.m_data.m_values[10],
                other1.m_data.m_values[11] & other2.m_data.m_values[11],
                other1.m_data.m_values[12] & other2.m_data.m_values[12],
                other1.m_data.m_values[13] & other2.m_data.m_values[13],
                other1.m_data.m_values[14] & other2.m_data.m_values[14],
                other1.m_data.m_values[15] & other2.m_data.m_values[15]);
        } else {
            return SIMD16<T, Width>(Shift::bitAnd(other1.m_data.m_values[0], other2.m_data.m_values[0]),
                Shift::bitAnd(other1.m_data.m_values[1], other2.m_data.m_values[1]),
                Shift::bitAnd(other1.m_data.m_values[2], other2.m_data.m_values[2]),
                Shift::bitAnd(other1.m_data.m_values[3], other2.m_data.m_values[3]),
                Shift::bitAnd(other1.m_data.m_values[4], other2.m_data.m_values[4]),
                Shift::bitAnd(other1.m_data.m_values[5], other2.m_data.m_values[5]),
                Shift::bitAnd(other1.m_data.m_values[6], other2.m_data.m_values[6]),
                Shift::bitAnd(other1.m_data.m_values[7], other2.m_data.m_values[7]),
                Shift::bitAnd(other1.m_data.m_values[8], other2.m_data.m_values[8]),
                Shift::bitAnd(other1.m_data.m_values[9], other2.m_data.m_values[9]),
                Shift::bitAnd(other1.m_data.m_values[10], other2.m_data.m_values[10]),
                Shift::bitAnd(other1.m_data.m_values[11], other2.m_data.m_values[11]),
                Shift::bitAnd(other1.m_data.m_values[12], other2.m_data.m_values[12]),
                Shift::bitAnd(other1.m_data.m_values[13], other2.m_data.m_values[13]),
                Shift::bitAnd(other1.m_data.m_values[14], other2.m_data.m_values[14]),
                Shift::bitAnd(other1.m_data.m_values[15], other2.m_data.m_values[15]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator&(const SIMD16<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD16<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(other1.m_data.m_values[0] & other2.m_data.m_value,
                other1.m_data.m_values[1] & other2.m_data.m_value, other1.m_data.m_values[2] & other2.m_data.m_value,
                other1.m_data.m_values[3] & other2.m_data.m_value, other1.m_data.m_values[4] & other2.m_data.m_value,
                other1.m_data.m_values[5] & other2.m_data.m_value, other1.m_data.m_values[6] & other2.m_data.m_value,
                other1.m_data.m_values[7] & other2.m_data.m_value, other1.m_data.m_values[8] & other2.m_data.m_value,
                other1.m_data.m_values[9] & other2.m_data.m_value, other1.m_data.m_values[10] & other2.m_data.m_value,
                other1.m_data.m_values[11] & other2.m_data.m_value, other1.m_data.m_values[12] & other2.m_data.m_value,
                other1.m_data.m_values[13] & other2.m_data.m_value, other1.m_data.m_values[14] & other2.m_data.m_value,
                other1.m_data.m_values[15] & other2.m_data.m_value);
        } else {
            return SIMD16<T, Width>(Shift::bitAnd(other1.m_data.m_values[0], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[1], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[2], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[3], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[4], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[5], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[6], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[7], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[8], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[9], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[10], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[11], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[12], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[13], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[14], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[15], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator|(const SIMD16<T, Width>& other1, const SIMD16<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(other1.m_data.m_values[0] | other2.m_data.m_values[0],
                other1.m_data.m_values[1] | other2.m_data.m_values[1],
                other1.m_data.m_values[2] | other2.m_data.m_values[2],
                other1.m_data.m_values[3] | other2.m_data.m_values[3],
                other1.m_data.m_values[4] | other2.m_data.m_values[4],
                other1.m_data.m_values[5] | other2.m_data.m_values[5],
                other1.m_data.m_values[6] | other2.m_data.m_values[6],
                other1.m_data.m_values[7] | other2.m_data.m_values[7],
                other1.m_data.m_values[8] | other2.m_data.m_values[8],
                other1.m_data.m_values[9] | other2.m_data.m_values[9],
                other1.m_data.m_values[10] | other2.m_data.m_values[10],
                other1.m_data.m_values[11] | other2.m_data.m_values[11],
                other1.m_data.m_values[12] | other2.m_data.m_values[12],
                other1.m_data.m_values[13] | other2.m_data.m_values[13],
                other1.m_data.m_values[14] | other2.m_data.m_values[14],
                other1.m_data.m_values[15] | other2.m_data.m_values[15]);
        } else {
            return SIMD16<T, Width>(Shift::bitOr(other1.m_data.m_values[0], other2.m_data.m_values[0]),
                Shift::bitOr(other1.m_data.m_values[1], other2.m_data.m_values[1]),
                Shift::bitOr(other1.m_data.m_values[2], other2.m_data.m_values[2]),
                Shift::bitOr(other1.m_data.m_values[3], other2.m_data.m_values[3]),
                Shift::bitOr(other1.m_data.m_values[4], other2.m_data.m_values[4]),
                Shift::bitOr(other1.m_data.m_values[5], other2.m_data.m_values[5]),
                Shift::bitOr(other1.m_data.m_values[6], other2.m_data.m_values[6]),
                Shift::bitOr(other1.m_data.m_values[7], other2.m_data.m_values[7]),
                Shift::bitOr(other1.m_data.m_values[8], other2.m_data.m_values[8]),
                Shift::bitOr(other1.m_data.m_values[9], other2.m_data.m_values[9]),
                Shift::bitOr(other1.m_data.m_values[10], other2.m_data.m_values[10]),
                Shift::bitOr(other1.m_data.m_values[11], other2.m_data.m_values[11]),
                Shift::bitOr(other1.m_data.m_values[12], other2.m_data.m_values[12]),
                Shift::bitOr(other1.m_data.m_values[13], other2.m_data.m_values[13]),
                Shift::bitOr(other1.m_data.m_values[14], other2.m_data.m_values[14]),
                Shift::bitOr(other1.m_data.m_values[15], other2.m_data.m_values[15]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator|(const SIMD16<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD16<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(other1.m_data.m_values[0] | other2.m_data.m_value,
                other1.m_data.m_values[1] | other2.m_data.m_value, other1.m_data.m_values[2] | other2.m_data.m_value,
                other1.m_data.m_values[3] | other2.m_data.m_value, other1.m_data.m_values[4] | other2.m_data.m_value,
                other1.m_data.m_values[5] | other2.m_data.m_value, other1.m_data.m_values[6] | other2.m_data.m_value,
                other1.m_data.m_values[7] | other2.m_data.m_value, other1.m_data.m_values[8] | other2.m_data.m_value,
                other1.m_data.m_values[9] | other2.m_data.m_value, other1.m_data.m_values[10] | other2.m_data.m_value,
                other1.m_data.m_values[11] | other2.m_data.m_value, other1.m_data.m_values[12] | other2.m_data.m_value,
                other1.m_data.m_values[13] | other2.m_data.m_value, other1.m_data.m_values[14] | other2.m_data.m_value,
                other1.m_data.m_values[15] | other2.m_data.m_value);
        } else {
            return SIMD16<T, Width>(Shift::bitOr(other1.m_data.m_values[0], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[1], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[2], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[3], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[4], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[5], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[6], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[7], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[8], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[9], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[10], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[11], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[12], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[13], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[14], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[15], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator^(const SIMD16<T, Width>& other1, const SIMD16<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD16<T, Width>, SIMD16<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(other1.m_data.m_values[0] ^ other2.m_data.m_values[0],
                other1.m_data.m_values[1] ^ other2.m_data.m_values[1],
                other1.m_data.m_values[2] ^ other2.m_data.m_values[2],
                other1.m_data.m_values[3] ^ other2.m_data.m_values[3],
                other1.m_data.m_values[4] ^ other2.m_data.m_values[4],
                other1.m_data.m_values[5] ^ other2.m_data.m_values[5],
                other1.m_data.m_values[6] ^ other2.m_data.m_values[6],
                other1.m_data.m_values[7] ^ other2.m_data.m_values[7],
                other1.m_data.m_values[8] ^ other2.m_data.m_values[8],
                other1.m_data.m_values[9] ^ other2.m_data.m_values[9],
                other1.m_data.m_values[10] ^ other2.m_data.m_values[10],
                other1.m_data.m_values[11] ^ other2.m_data.m_values[11],
                other1.m_data.m_values[12] ^ other2.m_data.m_values[12],
                other1.m_data.m_values[13] ^ other2.m_data.m_values[13],
                other1.m_data.m_values[14] ^ other2.m_data.m_values[14],
                other1.m_data.m_values[15] ^ other2.m_data.m_values[15]);
        } else {
            return SIMD16<T, Width>(Shift::bitXor(other1.m_data.m_values[0], other2.m_data.m_values[0]),
                Shift::bitXor(other1.m_data.m_values[1], other2.m_data.m_values[1]),
                Shift::bitXor(other1.m_data.m_values[2], other2.m_data.m_values[2]),
                Shift::bitXor(other1.m_data.m_values[3], other2.m_data.m_values[3]),
                Shift::bitXor(other1.m_data.m_values[4], other2.m_data.m_values[4]),
                Shift::bitXor(other1.m_data.m_values[5], other2.m_data.m_values[5]),
                Shift::bitXor(other1.m_data.m_values[6], other2.m_data.m_values[6]),
                Shift::bitXor(other1.m_data.m_values[7], other2.m_data.m_values[7]),
                Shift::bitXor(other1.m_data.m_values[8], other2.m_data.m_values[8]),
                Shift::bitXor(other1.m_data.m_values[9], other2.m_data.m_values[9]),
                Shift::bitXor(other1.m_data.m_values[10], other2.m_data.m_values[10]),
                Shift::bitXor(other1.m_data.m_values[11], other2.m_data.m_values[11]),
                Shift::bitXor(other1.m_data.m_values[12], other2.m_data.m_values[12]),
                Shift::bitXor(other1.m_data.m_values[13], other2.m_data.m_values[13]),
                Shift::bitXor(other1.m_data.m_values[14], other2.m_data.m_values[14]),
                Shift::bitXor(other1.m_data.m_values[15], other2.m_data.m_values[15]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD16<T, Width> operator^(const SIMD16<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD16<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(other1.m_data.m_values[0] ^ other2.m_data.m_value,
                other1.m_data.m_values[1] ^ other2.m_data.m_value, other1.m_data.m_values[2] ^ other2.m_data.m_value,
                other1.m_data.m_values[3] ^ other2.m_data.m_value, other1.m_data.m_values[4] ^ other2.m_data.m_value,
                other1.m_data.m_values[5] ^ other2.m_data.m_value, other1.m_data.m_values[6] ^ other2.m_data.m_value,
                other1.m_data.m_values[7] ^ other2.m_data.m_value, other1.m_data.m_values[8] ^ other2.m_data.m_value,
                other1.m_data.m_values[9] ^ other2.m_data.m_value, other1.m_data.m_values[10] ^ other2.m_data.m_value,
                other1.m_data.m_values[11] ^ other2.m_data.m_value, other1.m_data.m_values[12] ^ other2.m_data.m_value,
                other1.m_data.m_values[13] ^ other2.m_data.m_value, other1.m_data.m_values[14] ^ other2.m_data.m_value,
                other1.m_data.m_values[15] ^ other2.m_data.m_value);
        } else {
            return SIMD16<T, Width>(Shift::bitXor(other1.m_data.m_values[0], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[1], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[2], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[3], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[4], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[5], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[6], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[7], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[8], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[9], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[10], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[11], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[12], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[13], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[14], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[15], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> operator~(const SIMD16<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(~other.m_data.m_values[0], ~other.m_data.m_values[1], ~other.m_data.m_values[2],
                ~other.m_data.m_values[3], ~other.m_data.m_values[4], ~other.m_data.m_values[5],
                ~other.m_data.m_values[6], ~other.m_data.m_values[7], ~other.m_data.m_values[8],
                ~other.m_data.m_values[9], ~other.m_data.m_values[10], ~other.m_data.m_values[11],
                ~other.m_data.m_values[12], ~other.m_data.m_values[13], ~other.m_data.m_values[14],
                ~other.m_data.m_values[15]);
        } else {
            return SIMD16<T, Width>(Shift::bitNot(other.m_data.m_values[0]), Shift::bitNot(other.m_data.m_values[1]),
                Shift::bitNot(other.m_data.m_values[2]), Shift::bitNot(other.m_data.m_values[3]),
                Shift::bitNot(other.m_data.m_values[4]), Shift::bitNot(other.m_data.m_values[5]),
                Shift::bitNot(other.m_data.m_values[6]), Shift::bitNot(other.m_data.m_values[7]),
                Shift::bitNot(other.m_data.m_values[8]), Shift::bitNot(other.m_data.m_values[9]),
                Shift::bitNot(other.m_data.m_values[10]), Shift::bitNot(other.m_data.m_values[11]),
                Shift::bitNot(other.m_data.m_values[12]), Shift::bitNot(other.m_data.m_values[13]),
                Shift::bitNot(other.m_data.m_values[14]), Shift::bitNot(other.m_data.m_values[15]));
        }
    }
}
} // namespace Shift
