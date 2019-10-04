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

#include "SIMD/XSSIMD3x4.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#endif
#include "XSMath.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3x4Data<T>::SIMD3x4Data(const SIMD3x4<T, Width>& other) noexcept
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
    }
}

template<typename T>
XS_INLINE void SIMD3x4Data<T>::setData(const T value0, const T value1, const T value2, const T value3, const T value4,
    const T value5, const T value6, const T value7, const T value8, const T value9, const T value10,
    const T value11) noexcept
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
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD3x4Data<T>::store(const SIMD3x4<T, Width>& other) noexcept
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
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4Data<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(m_value0, m_value1, m_value2, m_value3, m_value4, m_value5, m_value6, m_value7,
            m_value8, m_value9, m_value10, m_value11);
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3x4DataPad<T>::SIMD3x4DataPad(const SIMD3x4<T, Width>& other) noexcept
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
    }
}

template<typename T>
XS_INLINE void SIMD3x4DataPad<T>::setData(const T value0, const T value1, const T value2, const T value3,
    const T value4, const T value5, const T value6, const T value7, const T value8, const T value9, const T value10,
    const T value11) noexcept
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
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD3x4DataPad<T>::store(const SIMD3x4<T, Width>& other) noexcept
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
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4DataPad<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(m_value0, m_value1, m_value2, m_value3, m_value4, m_value5, m_value6, m_value7,
            m_value8, m_value9, m_value10, m_value11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(const T value0, const T value1, const T value2, const T value3, const T value4,
    const T value5, const T value6, const T value7, const T value8, const T value9, const T value10,
    const T value11) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(T value) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(const SIMDBase<T, Width2>& other) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(const SIMDInBase<T, Width2>& other) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(const SIMD3<T, Width2>& other0, const SIMD3<T, Width2>& other1,
    const SIMD3<T, Width2>& other2, const SIMD3<T, Width2>& other3) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
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
        m_data.m_values[4] = other1.m_data.m_values[1];
        m_data.m_values[5] = other1.m_data.m_values[2];
        m_data.m_values[6] = other2.m_data.m_values[0];
        m_data.m_values[7] = other2.m_data.m_values[1];
        m_data.m_values[8] = other2.m_data.m_values[2];
        m_data.m_values[9] = other3.m_data.m_values[0];
        m_data.m_values[10] = other3.m_data.m_values[1];
        m_data.m_values[11] = other3.m_data.m_values[2];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(const SIMD3<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other.m_data.m_values[0];
        m_data.m_values[1] = other.m_data.m_values[1];
        m_data.m_values[2] = other.m_data.m_values[2];
        m_data.m_values[3] = other.m_data.m_values[0];
        m_data.m_values[4] = other.m_data.m_values[1];
        m_data.m_values[5] = other.m_data.m_values[2];
        m_data.m_values[6] = other.m_data.m_values[0];
        m_data.m_values[7] = other.m_data.m_values[1];
        m_data.m_values[8] = other.m_data.m_values[2];
        m_data.m_values[9] = other.m_data.m_values[0];
        m_data.m_values[10] = other.m_data.m_values[1];
        m_data.m_values[11] = other.m_data.m_values[2];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(const SIMD3x2<T, Width2>& other0, const SIMD3x2<T, Width2>& other1) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
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
        m_data.m_values[6] = other1.m_data.m_values[0];
        m_data.m_values[7] = other1.m_data.m_values[1];
        m_data.m_values[8] = other1.m_data.m_values[2];
        m_data.m_values[9] = other1.m_data.m_values[3];
        m_data.m_values[10] = other1.m_data.m_values[4];
        m_data.m_values[11] = other1.m_data.m_values[5];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(const SIMD3x3<T, Width2>& other0, const SIMD3<T, Width2>& other1) noexcept
{
    static_assert(sameImpl<SIMD3x3<T, Width>, SIMD3x3<T, Width2>> && sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
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
        m_data.m_values[8] = other1.m_data.m_values[8];
        m_data.m_values[9] = other1.m_data.m_values[0];
        m_data.m_values[10] = other1.m_data.m_values[1];
        m_data.m_values[11] = other1.m_data.m_values[2];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(const SIMD12<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other.m_data.m_values[0];
        m_data.m_values[1] = other.m_data.m_values[4];
        m_data.m_values[2] = other.m_data.m_values[8];
        m_data.m_values[3] = other.m_data.m_values[1];
        m_data.m_values[4] = other.m_data.m_values[5];
        m_data.m_values[5] = other.m_data.m_values[9];
        m_data.m_values[6] = other.m_data.m_values[2];
        m_data.m_values[7] = other.m_data.m_values[6];
        m_data.m_values[8] = other.m_data.m_values[10];
        m_data.m_values[9] = other.m_data.m_values[3];
        m_data.m_values[10] = other.m_data.m_values[7];
        m_data.m_values[11] = other.m_data.m_values[11];
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width>::SIMD3x4(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(T{1});
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width2> SIMD3x4<T, Width>::getValue3() const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width2>(
            m_data.m_values[Index * 3], m_data.m_values[Index * 3 + 1], m_data.m_values[Index * 3 + 2]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width2> SIMD3x4<T, Width>::getValue3x2() const noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
    static_assert(Index0 < 4 && Index1 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width2>(getValue3<Index0>(), getValue3<Index1>());
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, SIMDWidth Width2>
XS_INLINE SIMD3x3<T, Width2> SIMD3x4<T, Width>::getValue3x3() const noexcept
{
    static_assert(sameImpl<SIMD3x3<T, Width>, SIMD3x3<T, Width2>>);
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x3<T, Width2>(getValue3<Index0>(), getValue3<Index1>(), getValue3<Index2>());
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD3x4<T, Width>::setValue3(const SIMD3<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index * 3] = other.m_data.m_values[0];
        m_data.m_values[Index * 3 + 1] = other.m_data.m_values[1];
        m_data.m_values[Index * 3 + 2] = other.m_data.m_values[2];
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE void SIMD3x4<T, Width>::setValue3x2(const SIMD3x2<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
    static_assert(Index0 < 4 && Index1 < 4);
    static_assert(Index0 != Index1);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        setValue3<Index0>(other.template getValue3<0>());
        setValue3<Index1>(other.template getValue3<1>());
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7, bool Elem8,
    bool Elem9, bool Elem10, bool Elem11>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::negate() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Elem0 ? -m_data.m_values[0] : m_data.m_values[0],
            Elem1 ? -m_data.m_values[1] : m_data.m_values[1], Elem2 ? -m_data.m_values[2] : m_data.m_values[2],
            Elem3 ? -m_data.m_values[3] : m_data.m_values[3], Elem4 ? -m_data.m_values[4] : m_data.m_values[4],
            Elem5 ? -m_data.m_values[5] : m_data.m_values[5], Elem6 ? -m_data.m_values[6] : m_data.m_values[6],
            Elem7 ? -m_data.m_values[7] : m_data.m_values[7], Elem8 ? -m_data.m_values[8] : m_data.m_values[8],
            Elem9 ? -m_data.m_values[9] : m_data.m_values[9], Elem10 ? -m_data.m_values[10] : m_data.m_values[10],
            Elem11 ? -m_data.m_values[11] : m_data.m_values[11]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::mad(
    const SIMD3x4<T, Width2>& other1, const SIMD3x4<T, Width2>& other2) const noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
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
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_values[11], other2.m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::mad(
    const SIMDBase<T, Width2>& other1, const SIMD3x4<T, Width>& other2) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_value, other2.m_data.m_values[0]),
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
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_value, other2.m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::mad(
    const SIMD3x4<T, Width>& other1, const SIMDBase<T, Width2>& other2) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_value),
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
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_values[11], other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::mad(
    const SIMD4<T, Width2>& other1, const SIMD3x4<T, Width>& other2) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[0], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[0], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[1], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[1], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[1], other2.m_data.m_values[5]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[2], other2.m_data.m_values[6]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[2], other2.m_data.m_values[7]),
            Shift::fma<T>(m_data.m_values[8], other1.m_data.m_values[2], other2.m_data.m_values[8]),
            Shift::fma<T>(m_data.m_values[9], other1.m_data.m_values[3], other2.m_data.m_values[9]),
            Shift::fma<T>(m_data.m_values[10], other1.m_data.m_values[3], other2.m_data.m_values[10]),
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_values[3], other2.m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::mad(const SIMD3<T, Width2>& other1, const SIMD3<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[8], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[9], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[10], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_values[2], other2.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::mad(
    const SIMD3<T, Width2>& other1, const SIMD3x4<T, Width>& other2) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[0], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[1], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[2], other2.m_data.m_values[5]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[0], other2.m_data.m_values[6]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[1], other2.m_data.m_values[7]),
            Shift::fma<T>(m_data.m_values[8], other1.m_data.m_values[2], other2.m_data.m_values[8]),
            Shift::fma<T>(m_data.m_values[9], other1.m_data.m_values[0], other2.m_data.m_values[9]),
            Shift::fma<T>(m_data.m_values[10], other1.m_data.m_values[1], other2.m_data.m_values[10]),
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_values[2], other2.m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::mad(
    const SIMD3x2<T, Width2>& other1, const SIMD3x2<T, Width2>& other2) const noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[4], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[5], other2.m_data.m_values[5]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[8], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[9], other1.m_data.m_values[3], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[10], other1.m_data.m_values[4], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_values[5], other2.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::mad(
    const SIMD3x2<T, Width2>& other1, const SIMD3x4<T, Width>& other2) const noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[4], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[5], other2.m_data.m_values[5]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[0], other2.m_data.m_values[6]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[1], other2.m_data.m_values[7]),
            Shift::fma<T>(m_data.m_values[8], other1.m_data.m_values[2], other2.m_data.m_values[8]),
            Shift::fma<T>(m_data.m_values[9], other1.m_data.m_values[3], other2.m_data.m_values[9]),
            Shift::fma<T>(m_data.m_values[10], other1.m_data.m_values[4], other2.m_data.m_values[10]),
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_values[5], other2.m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::msub(
    const SIMD3x4<T, Width2>& other1, const SIMD3x4<T, Width2>& other2) const noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], -other2.m_data.m_values[0]),
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
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_values[11], -other2.m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::abs<T>(m_data.m_values[0]), Shift::abs<T>(m_data.m_values[1]),
            Shift::abs<T>(m_data.m_values[2]), Shift::abs<T>(m_data.m_values[3]), Shift::abs<T>(m_data.m_values[4]),
            Shift::abs<T>(m_data.m_values[5]), Shift::abs<T>(m_data.m_values[6]), Shift::abs<T>(m_data.m_values[7]),
            Shift::abs<T>(m_data.m_values[8]), Shift::abs<T>(m_data.m_values[9]), Shift::abs<T>(m_data.m_values[10]),
            Shift::abs<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::max(const SIMD3x4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::max<T>(m_data.m_values[0], other.m_data.m_values[0]),
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
            Shift::max<T>(m_data.m_values[11], other.m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::min(const SIMD3x4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::min<T>(m_data.m_values[0], other.m_data.m_values[0]),
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
            Shift::min<T>(m_data.m_values[11], other.m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::max(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::max<T>(m_data.m_values[0], other.m_data.m_value),
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
            Shift::max<T>(m_data.m_values[11], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::min(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::min<T>(m_data.m_values[0], other.m_data.m_value),
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
            Shift::min<T>(m_data.m_values[11], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3x4<T, Width>::max3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other0 = Shift::max<T>(m_data.m_values[0], m_data.m_values[6]);
        T other1 = Shift::max<T>(m_data.m_values[1], m_data.m_values[7]);
        T other2 = Shift::max<T>(m_data.m_values[2], m_data.m_values[8]);
        const T other3 = Shift::max<T>(m_data.m_values[3], m_data.m_values[9]);
        const T other4 = Shift::max<T>(m_data.m_values[4], m_data.m_values[10]);
        const T other5 = Shift::max<T>(m_data.m_values[5], m_data.m_values[11]);
        other0 = Shift::max<T>(other0, other3);
        other1 = Shift::max<T>(other1, other4);
        other2 = Shift::max<T>(other2, other5);
        return SIMD3(other0, other1, other2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3x4<T, Width>::min3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other0 = Shift::min<T>(m_data.m_values[0], m_data.m_values[6]);
        T other1 = Shift::min<T>(m_data.m_values[1], m_data.m_values[7]);
        T other2 = Shift::min<T>(m_data.m_values[2], m_data.m_values[8]);
        const T other3 = Shift::min<T>(m_data.m_values[3], m_data.m_values[9]);
        const T other4 = Shift::min<T>(m_data.m_values[4], m_data.m_values[10]);
        const T other5 = Shift::min<T>(m_data.m_values[5], m_data.m_values[11]);
        other0 = Shift::min<T>(other0, other3);
        other1 = Shift::min<T>(other1, other4);
        other2 = Shift::min<T>(other2, other5);
        return SIMD3(other0, other1, other2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x4<T, Width>::max3x2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T other0 = Shift::max<T>(m_data.m_values[0], m_data.m_values[6]);
        const T other1 = Shift::max<T>(m_data.m_values[1], m_data.m_values[7]);
        const T other2 = Shift::max<T>(m_data.m_values[2], m_data.m_values[8]);
        const T other3 = Shift::max<T>(m_data.m_values[3], m_data.m_values[9]);
        const T other4 = Shift::max<T>(m_data.m_values[4], m_data.m_values[10]);
        const T other5 = Shift::max<T>(m_data.m_values[5], m_data.m_values[11]);
        return SIMD3x2(other0, other1, other2, other3, other4, other5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x4<T, Width>::min3x2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T other0 = Shift::min<T>(m_data.m_values[0], m_data.m_values[6]);
        const T other1 = Shift::min<T>(m_data.m_values[1], m_data.m_values[7]);
        const T other2 = Shift::min<T>(m_data.m_values[2], m_data.m_values[8]);
        const T other3 = Shift::min<T>(m_data.m_values[3], m_data.m_values[9]);
        const T other4 = Shift::min<T>(m_data.m_values[4], m_data.m_values[10]);
        const T other5 = Shift::min<T>(m_data.m_values[5], m_data.m_values[11]);
        return SIMD3x2(other0, other1, other2, other3, other4, other5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD3x4<T, Width>::hmax3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T other1 = Shift::max<T>(m_data.m_values[0], m_data.m_values[1]);
        const T other2 = Shift::max<T>(m_data.m_values[3], m_data.m_values[4]);
        const T other3 = Shift::max<T>(m_data.m_values[6], m_data.m_values[7]);
        const T other4 = Shift::max<T>(m_data.m_values[9], m_data.m_values[10]);
        return SIMD4(Shift::max<T>(m_data.m_values[2], other1), Shift::max<T>(m_data.m_values[5], other2),
            Shift::max<T>(m_data.m_values[8], other3), Shift::max<T>(m_data.m_values[11], other4));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD3x4<T, Width>::hmin3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T other1 = Shift::min<T>(m_data.m_values[0], m_data.m_values[1]);
        const T other2 = Shift::min<T>(m_data.m_values[3], m_data.m_values[4]);
        const T other3 = Shift::min<T>(m_data.m_values[6], m_data.m_values[7]);
        const T other4 = Shift::min<T>(m_data.m_values[9], m_data.m_values[10]);
        return SIMD4(Shift::min<T>(m_data.m_values[2], other1), Shift::min<T>(m_data.m_values[5], other2),
            Shift::min<T>(m_data.m_values[8], other3), Shift::min<T>(m_data.m_values[11], other4));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::reciprocal() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::recip<T>(m_data.m_values[0]), Shift::recip<T>(m_data.m_values[1]),
            Shift::recip<T>(m_data.m_values[2]), Shift::recip<T>(m_data.m_values[3]),
            Shift::recip<T>(m_data.m_values[4]), Shift::recip<T>(m_data.m_values[5]),
            Shift::recip<T>(m_data.m_values[6]), Shift::recip<T>(m_data.m_values[7]),
            Shift::recip<T>(m_data.m_values[8]), Shift::recip<T>(m_data.m_values[9]),
            Shift::recip<T>(m_data.m_values[10]), Shift::recip<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3x4<T, Width>::add3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other0 = m_data.m_values[0] + m_data.m_values[6];
        T other1 = m_data.m_values[1] + m_data.m_values[7];
        T other2 = m_data.m_values[2] + m_data.m_values[8];
        const T other3 = m_data.m_values[3] + m_data.m_values[9];
        const T other4 = m_data.m_values[4] + m_data.m_values[10];
        const T other5 = m_data.m_values[5] + m_data.m_values[11];
        other0 += other3;
        other1 += other4;
        other2 += other5;
        return SIMD3(other0, other1, other2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x4<T, Width>::add3x2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T other0 = m_data.m_values[0] + m_data.m_values[6];
        const T other1 = m_data.m_values[1] + m_data.m_values[7];
        const T other2 = m_data.m_values[2] + m_data.m_values[8];
        const T other3 = m_data.m_values[3] + m_data.m_values[9];
        const T other4 = m_data.m_values[4] + m_data.m_values[10];
        const T other5 = m_data.m_values[5] + m_data.m_values[11];
        return SIMD3x2(other0, other1, other2, other3, other4, other5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x4<T, Width>::sub3x2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T other0 = m_data.m_values[0] - m_data.m_values[6];
        const T other1 = m_data.m_values[1] - m_data.m_values[7];
        const T other2 = m_data.m_values[2] - m_data.m_values[8];
        const T other3 = m_data.m_values[3] - m_data.m_values[9];
        const T other4 = m_data.m_values[4] - m_data.m_values[10];
        const T other5 = m_data.m_values[5] - m_data.m_values[11];
        return SIMD3x2(other0, other1, other2, other3, other4, other5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD3x4<T, Width>::hadd3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T res = (m_data.m_values[0] + m_data.m_values[1]);
        T res2 = (m_data.m_values[3] + m_data.m_values[4]);
        T res3 = (m_data.m_values[6] + m_data.m_values[7]);
        T res4 = (m_data.m_values[9] + m_data.m_values[10]);
        res += m_data.m_values[2];
        res2 += m_data.m_values[5];
        res3 += m_data.m_values[8];
        res4 += m_data.m_values[11];
        return SIMD4(res, res2, res3, res4);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::ceil<T>(m_data.m_values[0]), Shift::ceil<T>(m_data.m_values[1]),
            Shift::ceil<T>(m_data.m_values[2]), Shift::ceil<T>(m_data.m_values[3]), Shift::ceil<T>(m_data.m_values[4]),
            Shift::ceil<T>(m_data.m_values[5]), Shift::ceil<T>(m_data.m_values[6]), Shift::ceil<T>(m_data.m_values[7]),
            Shift::ceil<T>(m_data.m_values[8]), Shift::ceil<T>(m_data.m_values[9]), Shift::ceil<T>(m_data.m_values[10]),
            Shift::ceil<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::floor<T>(m_data.m_values[0]), Shift::floor<T>(m_data.m_values[1]),
            Shift::floor<T>(m_data.m_values[2]), Shift::floor<T>(m_data.m_values[3]),
            Shift::floor<T>(m_data.m_values[4]), Shift::floor<T>(m_data.m_values[5]),
            Shift::floor<T>(m_data.m_values[6]), Shift::floor<T>(m_data.m_values[7]),
            Shift::floor<T>(m_data.m_values[8]), Shift::floor<T>(m_data.m_values[9]),
            Shift::floor<T>(m_data.m_values[10]), Shift::floor<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::trunc<T>(m_data.m_values[0]), Shift::trunc<T>(m_data.m_values[1]),
            Shift::trunc<T>(m_data.m_values[2]), Shift::trunc<T>(m_data.m_values[3]),
            Shift::trunc<T>(m_data.m_values[4]), Shift::trunc<T>(m_data.m_values[5]),
            Shift::trunc<T>(m_data.m_values[6]), Shift::trunc<T>(m_data.m_values[7]),
            Shift::trunc<T>(m_data.m_values[8]), Shift::trunc<T>(m_data.m_values[9]),
            Shift::trunc<T>(m_data.m_values[10]), Shift::trunc<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::sqrt<T>(m_data.m_values[0]), Shift::sqrt<T>(m_data.m_values[1]),
            Shift::sqrt<T>(m_data.m_values[2]), Shift::sqrt<T>(m_data.m_values[3]), Shift::sqrt<T>(m_data.m_values[4]),
            Shift::sqrt<T>(m_data.m_values[5]), Shift::sqrt<T>(m_data.m_values[6]), Shift::sqrt<T>(m_data.m_values[7]),
            Shift::sqrt<T>(m_data.m_values[8]), Shift::sqrt<T>(m_data.m_values[9]), Shift::sqrt<T>(m_data.m_values[10]),
            Shift::sqrt<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(Shift::rsqrt<T>(m_data.m_values[0]), Shift::rsqrt<T>(m_data.m_values[1]),
            Shift::rsqrt<T>(m_data.m_values[2]), Shift::rsqrt<T>(m_data.m_values[3]),
            Shift::rsqrt<T>(m_data.m_values[4]), Shift::rsqrt<T>(m_data.m_values[5]),
            Shift::rsqrt<T>(m_data.m_values[6]), Shift::rsqrt<T>(m_data.m_values[7]),
            Shift::rsqrt<T>(m_data.m_values[8]), Shift::rsqrt<T>(m_data.m_values[9]),
            Shift::rsqrt<T>(m_data.m_values[10]), Shift::rsqrt<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD3x4<T, Width>::dot3(const SIMD3x4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T v0Sq = m_data.m_values[0] * other.m_data.m_values[0];
        T v1Sq = m_data.m_values[3] * other.m_data.m_values[3];
        T v2Sq = m_data.m_values[6] * other.m_data.m_values[6];
        T v3Sq = m_data.m_values[9] * other.m_data.m_values[9];
        v0Sq = Shift::fma<T>(m_data.m_values[1], other.m_data.m_values[1], v0Sq);
        v1Sq = Shift::fma<T>(m_data.m_values[4], other.m_data.m_values[4], v1Sq);
        v2Sq = Shift::fma<T>(m_data.m_values[7], other.m_data.m_values[7], v2Sq);
        v3Sq = Shift::fma<T>(m_data.m_values[10], other.m_data.m_values[10], v3Sq);
        return SIMD4(Shift::fma<T>(m_data.m_values[2], other.m_data.m_values[2], v0Sq),
            Shift::fma<T>(m_data.m_values[5], other.m_data.m_values[5], v1Sq),
            Shift::fma<T>(m_data.m_values[8], other.m_data.m_values[8], v2Sq),
            Shift::fma<T>(m_data.m_values[11], other.m_data.m_values[11], v3Sq));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::cross3(const SIMD3x4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T v0CrossX = -(m_data.m_values[2] * other.m_data.m_values[1]);
        const T v1CrossX = -(m_data.m_values[5] * other.m_data.m_values[4]);
        const T v2CrossX = -(m_data.m_values[8] * other.m_data.m_values[7]);
        const T v3CrossX = -(m_data.m_values[11] * other.m_data.m_values[10]);
        const T v0CrossY = -(m_data.m_values[0] * other.m_data.m_values[2]);
        const T v1CrossY = -(m_data.m_values[3] * other.m_data.m_values[5]);
        const T v2CrossY = -(m_data.m_values[6] * other.m_data.m_values[8]);
        const T v3CrossY = -(m_data.m_values[9] * other.m_data.m_values[11]);
        const T v0CrossZ = -(m_data.m_values[1] * other.m_data.m_values[0]);
        const T v1CrossZ = -(m_data.m_values[4] * other.m_data.m_values[3]);
        const T v2CrossZ = -(m_data.m_values[7] * other.m_data.m_values[6]);
        const T v3CrossZ = -(m_data.m_values[10] * other.m_data.m_values[9]);

        return SIMD3x4(Shift::fma<T>(m_data.m_values[1], other.m_data.m_values[2], v0CrossX),
            Shift::fma<T>(m_data.m_values[2], other.m_data.m_values[0], v0CrossY),
            Shift::fma<T>(m_data.m_values[0], other.m_data.m_values[1], v0CrossZ),
            Shift::fma<T>(m_data.m_values[4], other.m_data.m_values[5], v1CrossX),
            Shift::fma<T>(m_data.m_values[5], other.m_data.m_values[3], v1CrossY),
            Shift::fma<T>(m_data.m_values[3], other.m_data.m_values[4], v1CrossZ),
            Shift::fma<T>(m_data.m_values[7], other.m_data.m_values[8], v2CrossX),
            Shift::fma<T>(m_data.m_values[8], other.m_data.m_values[6], v2CrossY),
            Shift::fma<T>(m_data.m_values[6], other.m_data.m_values[7], v2CrossZ),
            Shift::fma<T>(m_data.m_values[10], other.m_data.m_values[11], v3CrossX),
            Shift::fma<T>(m_data.m_values[11], other.m_data.m_values[9], v3CrossY),
            Shift::fma<T>(m_data.m_values[9], other.m_data.m_values[10], v3CrossZ));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD3x4<T, Width>::lengthSqr3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return dot3(*this);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD3x4<T, Width>::length3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return dot3(*this).sqrt();
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::normalize3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return *this / length3();
        } else {
            return *this * lengthSqr3().rsqrt();
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD3x4<T, Width>::transpose() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(*this);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::insert3(const SIMD3x4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
    static_assert(Index0 < 3 && Index1 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(SIMD3x2<T, Width>(m_data.m_values[0], m_data.m_values[1], m_data.m_values[2], m_data.m_values[3],
                           m_data.m_values[4], m_data.m_values[5])
                           .insertTri<Index0, Index1>(SIMD3x2<T, Width>(other.m_data.m_values[0],
                               other.m_data.m_values[1], other.m_data.m_values[2], other.m_data.m_values[3],
                               other.m_data.m_values[4], other.m_data.m_values[5])),
            SIMD3x2<T, Width>(m_data.m_values[6], m_data.m_values[7], m_data.m_values[8], m_data.m_values[9],
                m_data.m_values[10], m_data.m_values[11])
                .insertTri<Index0, Index1>(
                    SIMD3x2<T, Width>(other.m_data.m_values[6], other.m_data.m_values[7], other.m_data.m_values[8],
                        other.m_data.m_values[9], other.m_data.m_values[10], other.m_data.m_values[11])));
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::blend3(const SIMD3x4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(SIMD3x2<T, Width>(m_data.m_values[0], m_data.m_values[1], m_data.m_values[2], m_data.m_values[3],
                           m_data.m_values[4], m_data.m_values[5])
                           .blendTri<Elem0, Elem1, Elem2>(SIMD3x2<T, Width>(other.m_data.m_values[0],
                               other.m_data.m_values[1], other.m_data.m_values[2], other.m_data.m_values[3],
                               other.m_data.m_values[4], other.m_data.m_values[5])),
            SIMD3x2<T, Width>(m_data.m_values[6], m_data.m_values[7], m_data.m_values[8], m_data.m_values[9],
                m_data.m_values[10], m_data.m_values[11])
                .blendTri<Elem0, Elem1, Elem2>(
                    SIMD3x2<T, Width>(other.m_data.m_values[6], other.m_data.m_values[7], other.m_data.m_values[8],
                        other.m_data.m_values[9], other.m_data.m_values[10], other.m_data.m_values[11])));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::shuffle3() const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(SIMD3x2<T, Width>(m_data.m_values[0], m_data.m_values[1], m_data.m_values[2], m_data.m_values[3],
                           m_data.m_values[4], m_data.m_values[5])
                           .template shuffleTri<Index0, Index1, Index2>(),
            SIMD3x2<T, Width>(m_data.m_values[6], m_data.m_values[7], m_data.m_values[8], m_data.m_values[9],
                m_data.m_values[10], m_data.m_values[11])
                .template shuffleTri<Index0, Index1, Index2>());
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD16<T, Width> SIMD3x4<T, Width>::shuffle3() const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3 && Index3 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16<T, Width>(m_data.m_values[Index0], m_data.m_values[Index1], m_data.m_values[Index2],
            m_data.m_values[Index3], m_data.m_values[Index0 + 3], m_data.m_values[Index1 + 3],
            m_data.m_values[Index2 + 3], m_data.m_values[Index3 + 3], m_data.m_values[Index0 + 6],
            m_data.m_values[Index1 + 6], m_data.m_values[Index2 + 6], m_data.m_values[Index3 + 6],
            m_data.m_values[Index0 + 9], m_data.m_values[Index1 + 9], m_data.m_values[Index2 + 9],
            m_data.m_values[Index3 + 9]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD3x4<T, Width> SIMD3x4<T, Width>::shuffleH3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4(getValue3<Index0>(), getValue3<Index1>(), getValue3<Index2>(), getValue3<Index3>());
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator+(const SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
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
            other1.m_data.m_values[11] + other2.m_data.m_values[11]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator+(const SIMD3x4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3x4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_value,
            other1.m_data.m_values[1] + other2.m_data.m_value, other1.m_data.m_values[2] + other2.m_data.m_value,
            other1.m_data.m_values[3] + other2.m_data.m_value, other1.m_data.m_values[4] + other2.m_data.m_value,
            other1.m_data.m_values[5] + other2.m_data.m_value, other1.m_data.m_values[6] + other2.m_data.m_value,
            other1.m_data.m_values[7] + other2.m_data.m_value, other1.m_data.m_values[8] + other2.m_data.m_value,
            other1.m_data.m_values[9] + other2.m_data.m_value, other1.m_data.m_values[10] + other2.m_data.m_value,
            other1.m_data.m_values[11] + other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator+(const SIMD3x4<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
            other1.m_data.m_values[1] + other2.m_data.m_values[1],
            other1.m_data.m_values[2] + other2.m_data.m_values[2],
            other1.m_data.m_values[3] + other2.m_data.m_values[0],
            other1.m_data.m_values[4] + other2.m_data.m_values[1],
            other1.m_data.m_values[5] + other2.m_data.m_values[2],
            other1.m_data.m_values[6] + other2.m_data.m_values[0],
            other1.m_data.m_values[7] + other2.m_data.m_values[1],
            other1.m_data.m_values[8] + other2.m_data.m_values[2],
            other1.m_data.m_values[9] + other2.m_data.m_values[0],
            other1.m_data.m_values[10] + other2.m_data.m_values[1],
            other1.m_data.m_values[11] + other2.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator+(const SIMD3x4<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
            other1.m_data.m_values[1] + other2.m_data.m_values[1],
            other1.m_data.m_values[2] + other2.m_data.m_values[2],
            other1.m_data.m_values[3] + other2.m_data.m_values[3],
            other1.m_data.m_values[4] + other2.m_data.m_values[4],
            other1.m_data.m_values[5] + other2.m_data.m_values[5],
            other1.m_data.m_values[6] + other2.m_data.m_values[0],
            other1.m_data.m_values[7] + other2.m_data.m_values[1],
            other1.m_data.m_values[8] + other2.m_data.m_values[2],
            other1.m_data.m_values[9] + other2.m_data.m_values[3],
            other1.m_data.m_values[10] + other2.m_data.m_values[4],
            other1.m_data.m_values[11] + other2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator-(const SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
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
            other1.m_data.m_values[11] - other2.m_data.m_values[11]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator-(const SIMD3x4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3x4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_value,
            other1.m_data.m_values[1] - other2.m_data.m_value, other1.m_data.m_values[2] - other2.m_data.m_value,
            other1.m_data.m_values[3] - other2.m_data.m_value, other1.m_data.m_values[4] - other2.m_data.m_value,
            other1.m_data.m_values[5] - other2.m_data.m_value, other1.m_data.m_values[6] - other2.m_data.m_value,
            other1.m_data.m_values[7] - other2.m_data.m_value, other1.m_data.m_values[8] - other2.m_data.m_value,
            other1.m_data.m_values[9] - other2.m_data.m_value, other1.m_data.m_values[10] - other2.m_data.m_value,
            other1.m_data.m_values[11] - other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator-(const SIMDBase<T, Width2>& other1, const SIMD3x4<T, Width>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3x4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.m_data.m_value - other2.m_data.m_values[0],
            other1.m_data.m_value - other2.m_data.m_values[1], other1.m_data.m_value - other2.m_data.m_values[2],
            other1.m_data.m_value - other2.m_data.m_values[3], other1.m_data.m_value - other2.m_data.m_values[4],
            other1.m_data.m_value - other2.m_data.m_values[5], other1.m_data.m_value - other2.m_data.m_values[6],
            other1.m_data.m_value - other2.m_data.m_values[7], other1.m_data.m_value - other2.m_data.m_values[8],
            other1.m_data.m_value - other2.m_data.m_values[9], other1.m_data.m_value - other2.m_data.m_values[10],
            other1.m_data.m_value - other2.m_data.m_values[11]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator-(const SIMD3x4<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
            other1.m_data.m_values[1] - other2.m_data.m_values[1],
            other1.m_data.m_values[2] - other2.m_data.m_values[2],
            other1.m_data.m_values[3] - other2.m_data.m_values[0],
            other1.m_data.m_values[4] - other2.m_data.m_values[1],
            other1.m_data.m_values[5] - other2.m_data.m_values[2],
            other1.m_data.m_values[6] - other2.m_data.m_values[0],
            other1.m_data.m_values[7] - other2.m_data.m_values[1],
            other1.m_data.m_values[8] - other2.m_data.m_values[2],
            other1.m_data.m_values[9] - other2.m_data.m_values[0],
            other1.m_data.m_values[10] - other2.m_data.m_values[1],
            other1.m_data.m_values[11] - other2.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator-(const SIMD3x4<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
            other1.m_data.m_values[1] - other2.m_data.m_values[1],
            other1.m_data.m_values[2] - other2.m_data.m_values[2],
            other1.m_data.m_values[3] - other2.m_data.m_values[3],
            other1.m_data.m_values[4] - other2.m_data.m_values[4],
            other1.m_data.m_values[5] - other2.m_data.m_values[5],
            other1.m_data.m_values[6] - other2.m_data.m_values[0],
            other1.m_data.m_values[7] - other2.m_data.m_values[1],
            other1.m_data.m_values[8] - other2.m_data.m_values[2],
            other1.m_data.m_values[9] - other2.m_data.m_values[3],
            other1.m_data.m_values[10] - other2.m_data.m_values[4],
            other1.m_data.m_values[11] - other2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator*(const SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
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
            other1.m_data.m_values[11] * other2.m_data.m_values[11]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator*(const SIMD3x4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3x4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_value,
            other1.m_data.m_values[1] * other2.m_data.m_value, other1.m_data.m_values[2] * other2.m_data.m_value,
            other1.m_data.m_values[3] * other2.m_data.m_value, other1.m_data.m_values[4] * other2.m_data.m_value,
            other1.m_data.m_values[5] * other2.m_data.m_value, other1.m_data.m_values[6] * other2.m_data.m_value,
            other1.m_data.m_values[7] * other2.m_data.m_value, other1.m_data.m_values[8] * other2.m_data.m_value,
            other1.m_data.m_values[9] * other2.m_data.m_value, other1.m_data.m_values[10] * other2.m_data.m_value,
            other1.m_data.m_values[11] * other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator*(const SIMD3x4<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
            other1.m_data.m_values[1] * other2.m_data.m_values[1],
            other1.m_data.m_values[2] * other2.m_data.m_values[2],
            other1.m_data.m_values[3] * other2.m_data.m_values[0],
            other1.m_data.m_values[4] * other2.m_data.m_values[1],
            other1.m_data.m_values[5] * other2.m_data.m_values[2],
            other1.m_data.m_values[6] * other2.m_data.m_values[0],
            other1.m_data.m_values[7] * other2.m_data.m_values[1],
            other1.m_data.m_values[8] * other2.m_data.m_values[2],
            other1.m_data.m_values[9] * other2.m_data.m_values[0],
            other1.m_data.m_values[10] * other2.m_data.m_values[1],
            other1.m_data.m_values[11] * other2.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator*(const SIMD3x4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
            other1.m_data.m_values[1] * other2.m_data.m_values[0],
            other1.m_data.m_values[2] * other2.m_data.m_values[0],
            other1.m_data.m_values[3] * other2.m_data.m_values[1],
            other1.m_data.m_values[4] * other2.m_data.m_values[1],
            other1.m_data.m_values[5] * other2.m_data.m_values[1],
            other1.m_data.m_values[6] * other2.m_data.m_values[2],
            other1.m_data.m_values[7] * other2.m_data.m_values[2],
            other1.m_data.m_values[8] * other2.m_data.m_values[2],
            other1.m_data.m_values[9] * other2.m_data.m_values[3],
            other1.m_data.m_values[10] * other2.m_data.m_values[3],
            other1.m_data.m_values[11] * other2.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator*(const SIMD3x4<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
            other1.m_data.m_values[1] * other2.m_data.m_values[1],
            other1.m_data.m_values[2] * other2.m_data.m_values[2],
            other1.m_data.m_values[3] * other2.m_data.m_values[3],
            other1.m_data.m_values[4] * other2.m_data.m_values[4],
            other1.m_data.m_values[5] * other2.m_data.m_values[5],
            other1.m_data.m_values[6] * other2.m_data.m_values[0],
            other1.m_data.m_values[7] * other2.m_data.m_values[1],
            other1.m_data.m_values[8] * other2.m_data.m_values[2],
            other1.m_data.m_values[9] * other2.m_data.m_values[3],
            other1.m_data.m_values[10] * other2.m_data.m_values[4],
            other1.m_data.m_values[11] * other2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator/(const SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
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
            other1.m_data.m_values[11] / other2.m_data.m_values[11]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator/(const SIMD3x4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3x4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD3x4<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_value,
                other1.m_data.m_values[1] / other2.m_data.m_value, other1.m_data.m_values[2] / other2.m_data.m_value,
                other1.m_data.m_values[3] / other2.m_data.m_value, other1.m_data.m_values[4] / other2.m_data.m_value,
                other1.m_data.m_values[5] / other2.m_data.m_value, other1.m_data.m_values[6] / other2.m_data.m_value,
                other1.m_data.m_values[7] / other2.m_data.m_value, other1.m_data.m_values[8] / other2.m_data.m_value,
                other1.m_data.m_values[9] / other2.m_data.m_value, other1.m_data.m_values[10] / other2.m_data.m_value,
                other1.m_data.m_values[11] / other2.m_data.m_value);
        } else {
            const T divisor = Shift::recip<T>(other2.m_data.m_value);
            return SIMD3x4<T, Width>(other1.m_data.m_values[0] * divisor, other1.m_data.m_values[1] * divisor,
                other1.m_data.m_values[2] * divisor, other1.m_data.m_values[3] * divisor,
                other1.m_data.m_values[4] * divisor, other1.m_data.m_values[5] * divisor,
                other1.m_data.m_values[6] * divisor, other1.m_data.m_values[7] * divisor,
                other1.m_data.m_values[8] * divisor, other1.m_data.m_values[9] * divisor,
                other1.m_data.m_values[10] * divisor, other1.m_data.m_values[11] * divisor);
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator/(const SIMDBase<T, Width2>& other1, const SIMD3x4<T, Width>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3x4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(other1.m_data.m_value / other2.m_data.m_values[0],
            other1.m_data.m_value / other2.m_data.m_values[1], other1.m_data.m_value / other2.m_data.m_values[2],
            other1.m_data.m_value / other2.m_data.m_values[3], other1.m_data.m_value / other2.m_data.m_values[4],
            other1.m_data.m_value / other2.m_data.m_values[5], other1.m_data.m_value / other2.m_data.m_values[6],
            other1.m_data.m_value / other2.m_data.m_values[7], other1.m_data.m_value / other2.m_data.m_values[8],
            other1.m_data.m_value / other2.m_data.m_values[9], other1.m_data.m_value / other2.m_data.m_values[10],
            other1.m_data.m_value / other2.m_data.m_values[11]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator/(const SIMD3x4<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD3x4<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
                other1.m_data.m_values[1] / other2.m_data.m_values[1],
                other1.m_data.m_values[2] / other2.m_data.m_values[2],
                other1.m_data.m_values[3] / other2.m_data.m_values[0],
                other1.m_data.m_values[4] / other2.m_data.m_values[1],
                other1.m_data.m_values[5] / other2.m_data.m_values[2],
                other1.m_data.m_values[6] / other2.m_data.m_values[0],
                other1.m_data.m_values[7] / other2.m_data.m_values[1],
                other1.m_data.m_values[8] / other2.m_data.m_values[2],
                other1.m_data.m_values[9] / other2.m_data.m_values[0],
                other1.m_data.m_values[10] / other2.m_data.m_values[1],
                other1.m_data.m_values[11] / other2.m_data.m_values[2]);
        } else {
            const T divisor0 = Shift::recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = Shift::recip<T>(other2.m_data.m_values[1]);
            const T divisor2 = Shift::recip<T>(other2.m_data.m_values[2]);
            return SIMD3x4<T, Width>(other1.m_data.m_values[0] * divisor0, other1.m_data.m_values[1] * divisor1,
                other1.m_data.m_values[2] * divisor2, other1.m_data.m_values[3] * divisor0,
                other1.m_data.m_values[4] * divisor1, other1.m_data.m_values[5] * divisor2,
                other1.m_data.m_values[6] * divisor0, other1.m_data.m_values[7] * divisor1,
                other1.m_data.m_values[8] * divisor2, other1.m_data.m_values[9] * divisor0,
                other1.m_data.m_values[10] * divisor1, other1.m_data.m_values[11] * divisor2);
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator/(const SIMD3x4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD3x4<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
                other1.m_data.m_values[1] / other2.m_data.m_values[0],
                other1.m_data.m_values[2] / other2.m_data.m_values[0],
                other1.m_data.m_values[3] / other2.m_data.m_values[1],
                other1.m_data.m_values[4] / other2.m_data.m_values[1],
                other1.m_data.m_values[5] / other2.m_data.m_values[1],
                other1.m_data.m_values[6] / other2.m_data.m_values[2],
                other1.m_data.m_values[7] / other2.m_data.m_values[2],
                other1.m_data.m_values[8] / other2.m_data.m_values[2],
                other1.m_data.m_values[9] / other2.m_data.m_values[3],
                other1.m_data.m_values[10] / other2.m_data.m_values[3],
                other1.m_data.m_values[11] / other2.m_data.m_values[3]);
        } else {
            const T divisor0 = Shift::recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = Shift::recip<T>(other2.m_data.m_values[1]);
            const T divisor2 = Shift::recip<T>(other2.m_data.m_values[2]);
            const T divisor3 = Shift::recip<T>(other2.m_data.m_values[3]);
            return SIMD3x4<T, Width>(other1.m_data.m_values[0] * divisor0, other1.m_data.m_values[1] * divisor0,
                other1.m_data.m_values[2] * divisor0, other1.m_data.m_values[3] * divisor1,
                other1.m_data.m_values[4] * divisor1, other1.m_data.m_values[5] * divisor1,
                other1.m_data.m_values[6] * divisor2, other1.m_data.m_values[7] * divisor2,
                other1.m_data.m_values[8] * divisor2, other1.m_data.m_values[9] * divisor3,
                other1.m_data.m_values[10] * divisor3, other1.m_data.m_values[11] * divisor3);
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator/(const SIMD3x4<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD3x4<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
                other1.m_data.m_values[1] / other2.m_data.m_values[1],
                other1.m_data.m_values[2] / other2.m_data.m_values[2],
                other1.m_data.m_values[3] / other2.m_data.m_values[3],
                other1.m_data.m_values[4] / other2.m_data.m_values[4],
                other1.m_data.m_values[5] / other2.m_data.m_values[5],
                other1.m_data.m_values[6] / other2.m_data.m_values[0],
                other1.m_data.m_values[7] / other2.m_data.m_values[1],
                other1.m_data.m_values[8] / other2.m_data.m_values[2],
                other1.m_data.m_values[9] / other2.m_data.m_values[3],
                other1.m_data.m_values[10] / other2.m_data.m_values[4],
                other1.m_data.m_values[11] / other2.m_data.m_values[5]);
        } else {
            const T divisor0 = Shift::recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = Shift::recip<T>(other2.m_data.m_values[1]);
            const T divisor2 = Shift::recip<T>(other2.m_data.m_values[2]);
            const T divisor3 = Shift::recip<T>(other2.m_data.m_values[3]);
            const T divisor4 = Shift::recip<T>(other2.m_data.m_values[4]);
            const T divisor5 = Shift::recip<T>(other2.m_data.m_values[5]);
            return SIMD3x4<T, Width>(other1.m_data.m_values[0] * divisor0, other1.m_data.m_values[1] * divisor1,
                other1.m_data.m_values[2] * divisor2, other1.m_data.m_values[3] * divisor3,
                other1.m_data.m_values[4] * divisor4, other1.m_data.m_values[5] * divisor5,
                other1.m_data.m_values[6] * divisor0, other1.m_data.m_values[7] * divisor1,
                other1.m_data.m_values[8] * divisor2, other1.m_data.m_values[9] * divisor3,
                other1.m_data.m_values[10] * divisor4, other1.m_data.m_values[11] * divisor5);
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator-(const SIMD3x4<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x4<T, Width>(-other.m_data.m_values[0], -other.m_data.m_values[1], -other.m_data.m_values[2],
            -other.m_data.m_values[3], -other.m_data.m_values[4], -other.m_data.m_values[5], -other.m_data.m_values[6],
            -other.m_data.m_values[7], -other.m_data.m_values[8], -other.m_data.m_values[9], -other.m_data.m_values[10],
            -other.m_data.m_values[11]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>& operator+=(SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>& operator+=(SIMD3x4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3x4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>& operator+=(SIMD3x4<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] += other2.m_data.m_values[0];
        other1.m_data.m_values[1] += other2.m_data.m_values[1];
        other1.m_data.m_values[2] += other2.m_data.m_values[2];
        other1.m_data.m_values[3] += other2.m_data.m_values[0];
        other1.m_data.m_values[4] += other2.m_data.m_values[1];
        other1.m_data.m_values[5] += other2.m_data.m_values[2];
        other1.m_data.m_values[6] += other2.m_data.m_values[0];
        other1.m_data.m_values[7] += other2.m_data.m_values[1];
        other1.m_data.m_values[8] += other2.m_data.m_values[2];
        other1.m_data.m_values[9] += other2.m_data.m_values[0];
        other1.m_data.m_values[10] += other2.m_data.m_values[1];
        other1.m_data.m_values[11] += other2.m_data.m_values[2];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>& operator+=(SIMD3x4<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
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
        other1.m_data.m_values[6] += other2.m_data.m_values[0];
        other1.m_data.m_values[7] += other2.m_data.m_values[1];
        other1.m_data.m_values[8] += other2.m_data.m_values[2];
        other1.m_data.m_values[9] += other2.m_data.m_values[3];
        other1.m_data.m_values[10] += other2.m_data.m_values[4];
        other1.m_data.m_values[11] += other2.m_data.m_values[5];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>& operator-=(SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>& operator-=(SIMD3x4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3x4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>& operator-=(SIMD3x4<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] -= other2.m_data.m_values[0];
        other1.m_data.m_values[1] -= other2.m_data.m_values[1];
        other1.m_data.m_values[2] -= other2.m_data.m_values[2];
        other1.m_data.m_values[3] -= other2.m_data.m_values[0];
        other1.m_data.m_values[4] -= other2.m_data.m_values[1];
        other1.m_data.m_values[5] -= other2.m_data.m_values[2];
        other1.m_data.m_values[6] -= other2.m_data.m_values[0];
        other1.m_data.m_values[7] -= other2.m_data.m_values[1];
        other1.m_data.m_values[8] -= other2.m_data.m_values[2];
        other1.m_data.m_values[9] -= other2.m_data.m_values[0];
        other1.m_data.m_values[10] -= other2.m_data.m_values[1];
        other1.m_data.m_values[11] -= other2.m_data.m_values[2];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>& operator-=(SIMD3x4<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
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
        other1.m_data.m_values[6] -= other2.m_data.m_values[0];
        other1.m_data.m_values[7] -= other2.m_data.m_values[1];
        other1.m_data.m_values[8] -= other2.m_data.m_values[2];
        other1.m_data.m_values[9] -= other2.m_data.m_values[3];
        other1.m_data.m_values[10] -= other2.m_data.m_values[4];
        other1.m_data.m_values[11] -= other2.m_data.m_values[5];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>& operator*=(SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>& operator*=(SIMD3x4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3x4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>& operator*=(SIMD3x4<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] *= other2.m_data.m_values[0];
        other1.m_data.m_values[1] *= other2.m_data.m_values[1];
        other1.m_data.m_values[2] *= other2.m_data.m_values[2];
        other1.m_data.m_values[3] *= other2.m_data.m_values[0];
        other1.m_data.m_values[4] *= other2.m_data.m_values[1];
        other1.m_data.m_values[5] *= other2.m_data.m_values[2];
        other1.m_data.m_values[6] *= other2.m_data.m_values[0];
        other1.m_data.m_values[7] *= other2.m_data.m_values[1];
        other1.m_data.m_values[8] *= other2.m_data.m_values[2];
        other1.m_data.m_values[9] *= other2.m_data.m_values[0];
        other1.m_data.m_values[10] *= other2.m_data.m_values[1];
        other1.m_data.m_values[11] *= other2.m_data.m_values[2];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>& operator*=(SIMD3x4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] *= other2.m_data.m_values[0];
        other1.m_data.m_values[1] *= other2.m_data.m_values[0];
        other1.m_data.m_values[2] *= other2.m_data.m_values[0];
        other1.m_data.m_values[3] *= other2.m_data.m_values[1];
        other1.m_data.m_values[4] *= other2.m_data.m_values[1];
        other1.m_data.m_values[5] *= other2.m_data.m_values[1];
        other1.m_data.m_values[6] *= other2.m_data.m_values[2];
        other1.m_data.m_values[7] *= other2.m_data.m_values[2];
        other1.m_data.m_values[8] *= other2.m_data.m_values[2];
        other1.m_data.m_values[9] *= other2.m_data.m_values[3];
        other1.m_data.m_values[10] *= other2.m_data.m_values[3];
        other1.m_data.m_values[11] *= other2.m_data.m_values[3];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>& operator*=(SIMD3x4<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
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
        other1.m_data.m_values[6] *= other2.m_data.m_values[0];
        other1.m_data.m_values[7] *= other2.m_data.m_values[1];
        other1.m_data.m_values[8] *= other2.m_data.m_values[2];
        other1.m_data.m_values[9] *= other2.m_data.m_values[3];
        other1.m_data.m_values[10] *= other2.m_data.m_values[4];
        other1.m_data.m_values[11] *= other2.m_data.m_values[5];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>& operator/=(SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>& operator/=(SIMD3x4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3x4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
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
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>& operator/=(SIMD3x4<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            other1.m_data.m_values[0] /= other2.m_data.m_values[0];
            other1.m_data.m_values[1] /= other2.m_data.m_values[1];
            other1.m_data.m_values[2] /= other2.m_data.m_values[2];
            other1.m_data.m_values[3] /= other2.m_data.m_values[0];
            other1.m_data.m_values[4] /= other2.m_data.m_values[1];
            other1.m_data.m_values[5] /= other2.m_data.m_values[2];
            other1.m_data.m_values[6] /= other2.m_data.m_values[0];
            other1.m_data.m_values[7] /= other2.m_data.m_values[1];
            other1.m_data.m_values[8] /= other2.m_data.m_values[2];
            other1.m_data.m_values[9] /= other2.m_data.m_values[0];
            other1.m_data.m_values[10] /= other2.m_data.m_values[1];
            other1.m_data.m_values[11] /= other2.m_data.m_values[2];
        } else {
            const T divisor0 = Shift::recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = Shift::recip<T>(other2.m_data.m_values[1]);
            const T divisor2 = Shift::recip<T>(other2.m_data.m_values[2]);
            other1.m_data.m_values[0] *= divisor0;
            other1.m_data.m_values[1] *= divisor1;
            other1.m_data.m_values[2] *= divisor2;
            other1.m_data.m_values[3] *= divisor0;
            other1.m_data.m_values[4] *= divisor1;
            other1.m_data.m_values[5] *= divisor2;
            other1.m_data.m_values[6] *= divisor0;
            other1.m_data.m_values[7] *= divisor1;
            other1.m_data.m_values[8] *= divisor2;
            other1.m_data.m_values[9] *= divisor0;
            other1.m_data.m_values[10] *= divisor1;
            other1.m_data.m_values[11] *= divisor2;
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>& operator/=(SIMD3x4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            other1.m_data.m_values[0] /= other2.m_data.m_values[0];
            other1.m_data.m_values[1] /= other2.m_data.m_values[0];
            other1.m_data.m_values[2] /= other2.m_data.m_values[0];
            other1.m_data.m_values[3] /= other2.m_data.m_values[1];
            other1.m_data.m_values[4] /= other2.m_data.m_values[1];
            other1.m_data.m_values[5] /= other2.m_data.m_values[1];
            other1.m_data.m_values[6] /= other2.m_data.m_values[2];
            other1.m_data.m_values[7] /= other2.m_data.m_values[2];
            other1.m_data.m_values[8] /= other2.m_data.m_values[2];
            other1.m_data.m_values[9] /= other2.m_data.m_values[3];
            other1.m_data.m_values[10] /= other2.m_data.m_values[3];
            other1.m_data.m_values[11] /= other2.m_data.m_values[3];
        } else {
            const T divisor0 = Shift::recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = Shift::recip<T>(other2.m_data.m_values[1]);
            const T divisor2 = Shift::recip<T>(other2.m_data.m_values[2]);
            const T divisor3 = Shift::recip<T>(other2.m_data.m_values[3]);
            other1.m_data.m_values[0] *= divisor0;
            other1.m_data.m_values[1] *= divisor0;
            other1.m_data.m_values[2] *= divisor0;
            other1.m_data.m_values[3] *= divisor1;
            other1.m_data.m_values[4] *= divisor1;
            other1.m_data.m_values[5] *= divisor1;
            other1.m_data.m_values[6] *= divisor2;
            other1.m_data.m_values[7] *= divisor2;
            other1.m_data.m_values[8] *= divisor2;
            other1.m_data.m_values[9] *= divisor3;
            other1.m_data.m_values[10] *= divisor3;
            other1.m_data.m_values[11] *= divisor3;
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width>& operator/=(SIMD3x4<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            other1.m_data.m_values[0] /= other2.m_data.m_values[0];
            other1.m_data.m_values[1] /= other2.m_data.m_values[1];
            other1.m_data.m_values[2] /= other2.m_data.m_values[2];
            other1.m_data.m_values[3] /= other2.m_data.m_values[3];
            other1.m_data.m_values[4] /= other2.m_data.m_values[4];
            other1.m_data.m_values[5] /= other2.m_data.m_values[5];
            other1.m_data.m_values[6] /= other2.m_data.m_values[0];
            other1.m_data.m_values[7] /= other2.m_data.m_values[1];
            other1.m_data.m_values[8] /= other2.m_data.m_values[2];
            other1.m_data.m_values[9] /= other2.m_data.m_values[3];
            other1.m_data.m_values[10] /= other2.m_data.m_values[4];
            other1.m_data.m_values[11] /= other2.m_data.m_values[5];
        } else {
            const T divisor0 = Shift::recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = Shift::recip<T>(other2.m_data.m_values[1]);
            const T divisor2 = Shift::recip<T>(other2.m_data.m_values[2]);
            const T divisor3 = Shift::recip<T>(other2.m_data.m_values[3]);
            const T divisor4 = Shift::recip<T>(other2.m_data.m_values[4]);
            const T divisor5 = Shift::recip<T>(other2.m_data.m_values[5]);
            other1.m_data.m_values[0] *= divisor0;
            other1.m_data.m_values[1] *= divisor1;
            other1.m_data.m_values[2] *= divisor2;
            other1.m_data.m_values[3] *= divisor3;
            other1.m_data.m_values[4] *= divisor4;
            other1.m_data.m_values[5] *= divisor5;
            other1.m_data.m_values[6] *= divisor0;
            other1.m_data.m_values[7] *= divisor1;
            other1.m_data.m_values[8] *= divisor2;
            other1.m_data.m_values[9] *= divisor3;
            other1.m_data.m_values[10] *= divisor4;
            other1.m_data.m_values[11] *= divisor5;
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator&(const SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x4<T, Width>(other1.m_data.m_values[0] & other2.m_data.m_values[0],
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
                other1.m_data.m_values[11] & other2.m_data.m_values[11]);
        } else {
            return SIMD3x4<T, Width>(Shift::bitAnd(other1.m_data.m_values[0], other2.m_data.m_values[0]),
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
                Shift::bitAnd(other1.m_data.m_values[11], other2.m_data.m_values[11]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator&(const SIMD3x4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3x4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x4<T, Width>(other1.m_data.m_values[0] & other2.m_data.m_value,
                other1.m_data.m_values[1] & other2.m_data.m_value, other1.m_data.m_values[2] & other2.m_data.m_value,
                other1.m_data.m_values[3] & other2.m_data.m_value, other1.m_data.m_values[4] & other2.m_data.m_value,
                other1.m_data.m_values[5] & other2.m_data.m_value, other1.m_data.m_values[6] & other2.m_data.m_value,
                other1.m_data.m_values[7] & other2.m_data.m_value, other1.m_data.m_values[8] & other2.m_data.m_value,
                other1.m_data.m_values[9] & other2.m_data.m_value, other1.m_data.m_values[10] & other2.m_data.m_value,
                other1.m_data.m_values[11] & other2.m_data.m_value);
        } else {
            return SIMD3x4<T, Width>(Shift::bitAnd(other1.m_data.m_values[0], other2.m_data.m_value),
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
                Shift::bitAnd(other1.m_data.m_values[11], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator|(const SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x4<T, Width>(other1.m_data.m_values[0] | other2.m_data.m_values[0],
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
                other1.m_data.m_values[11] | other2.m_data.m_values[11]);
        } else {
            return SIMD3x4<T, Width>(Shift::bitOr(other1.m_data.m_values[0], other2.m_data.m_values[0]),
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
                Shift::bitOr(other1.m_data.m_values[11], other2.m_data.m_values[11]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator|(const SIMD3x4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3x4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x4<T, Width>(other1.m_data.m_values[0] | other2.m_data.m_value,
                other1.m_data.m_values[1] | other2.m_data.m_value, other1.m_data.m_values[2] | other2.m_data.m_value,
                other1.m_data.m_values[3] | other2.m_data.m_value, other1.m_data.m_values[4] | other2.m_data.m_value,
                other1.m_data.m_values[5] | other2.m_data.m_value, other1.m_data.m_values[6] | other2.m_data.m_value,
                other1.m_data.m_values[7] | other2.m_data.m_value, other1.m_data.m_values[8] | other2.m_data.m_value,
                other1.m_data.m_values[9] | other2.m_data.m_value, other1.m_data.m_values[10] | other2.m_data.m_value,
                other1.m_data.m_values[11] | other2.m_data.m_value);
        } else {
            return SIMD3x4<T, Width>(Shift::bitOr(other1.m_data.m_values[0], other2.m_data.m_value),
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
                Shift::bitOr(other1.m_data.m_values[11], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator^(const SIMD3x4<T, Width>& other1, const SIMD3x4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x4<T, Width>(other1.m_data.m_values[0] ^ other2.m_data.m_values[0],
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
                other1.m_data.m_values[11] ^ other2.m_data.m_values[11]);
        } else {
            return SIMD3x4<T, Width>(Shift::bitXor(other1.m_data.m_values[0], other2.m_data.m_values[0]),
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
                Shift::bitXor(other1.m_data.m_values[11], other2.m_data.m_values[11]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x4<T, Width> operator^(const SIMD3x4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3x4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x4<T, Width>(other1.m_data.m_values[0] ^ other2.m_data.m_value,
                other1.m_data.m_values[1] ^ other2.m_data.m_value, other1.m_data.m_values[2] ^ other2.m_data.m_value,
                other1.m_data.m_values[3] ^ other2.m_data.m_value, other1.m_data.m_values[4] ^ other2.m_data.m_value,
                other1.m_data.m_values[5] ^ other2.m_data.m_value, other1.m_data.m_values[6] ^ other2.m_data.m_value,
                other1.m_data.m_values[7] ^ other2.m_data.m_value, other1.m_data.m_values[8] ^ other2.m_data.m_value,
                other1.m_data.m_values[9] ^ other2.m_data.m_value, other1.m_data.m_values[10] ^ other2.m_data.m_value,
                other1.m_data.m_values[11] ^ other2.m_data.m_value);
        } else {
            return SIMD3x4<T, Width>(Shift::bitXor(other1.m_data.m_values[0], other2.m_data.m_value),
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
                Shift::bitXor(other1.m_data.m_values[11], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> operator~(const SIMD3x4<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x4<T, Width>(~other.m_data.m_values[0], ~other.m_data.m_values[1], ~other.m_data.m_values[2],
                ~other.m_data.m_values[3], ~other.m_data.m_values[4], ~other.m_data.m_values[5],
                ~other.m_data.m_values[6], ~other.m_data.m_values[7], ~other.m_data.m_values[8],
                ~other.m_data.m_values[9], ~other.m_data.m_values[10], ~other.m_data.m_values[11]);
        } else {
            return SIMD3x4<T, Width>(Shift::bitNot(other.m_data.m_values[0]), Shift::bitNot(other.m_data.m_values[1]),
                Shift::bitNot(other.m_data.m_values[2]), Shift::bitNot(other.m_data.m_values[3]),
                Shift::bitNot(other.m_data.m_values[4]), Shift::bitNot(other.m_data.m_values[5]),
                Shift::bitNot(other.m_data.m_values[6]), Shift::bitNot(other.m_data.m_values[7]),
                Shift::bitNot(other.m_data.m_values[8]), Shift::bitNot(other.m_data.m_values[9]),
                Shift::bitNot(other.m_data.m_values[10]), Shift::bitNot(other.m_data.m_values[11]));
        }
    }
}
} // namespace Shift
