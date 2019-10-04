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

#include "SIMD/XSSIMD12.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#endif
#include "XSMath.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD12Data<T>::SIMD12Data(const SIMD12<T, Width>& other) noexcept
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
XS_INLINE void SIMD12Data<T>::setData(const T value0, const T value1, const T value2, const T value3, const T value4,
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
XS_INLINE void SIMD12Data<T>::store(const SIMD12<T, Width>& other) noexcept
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
XS_INLINE SIMD12<T, Width> SIMD12Data<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(m_value0, m_value1, m_value2, m_value3, m_value4, m_value5, m_value6, m_value7,
            m_value8, m_value9, m_value10, m_value11);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>::Shuffle::Shuffle(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>::Shuffle::Shuffle(const uint32 shuffle0, const uint32 shuffle1, const uint32 shuffle2,
    const uint32 shuffle3, const uint32 shuffle4, const uint32 shuffle5, const uint32 shuffle6, const uint32 shuffle7,
    const uint32 shuffle8, const uint32 shuffle9, const uint32 shuffle10, const uint32 shuffle11) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = shuffle0;
        m_data.m_values[1] = shuffle1;
        m_data.m_values[2] = shuffle2;
        m_data.m_values[3] = shuffle3;
        m_data.m_values[4] = shuffle4;
        m_data.m_values[5] = shuffle5;
        m_data.m_values[6] = shuffle6;
        m_data.m_values[7] = shuffle7;
        m_data.m_values[8] = shuffle8;
        m_data.m_values[9] = shuffle9;
        m_data.m_values[10] = shuffle10;
        m_data.m_values[11] = shuffle11;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::Shuffle SIMD12<T, Width>::Shuffle::Swap2() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(
            1_ui32, 0_ui32, 3_ui32, 2_ui32, 5_ui32, 4_ui32, 7_ui32, 6_ui32, 9_ui32, 8_ui32, 11_ui32, 10_ui32);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD12<T, Width>::Shuffle SIMD12<T, Width>::Shuffle::Swap2(
    const typename SIMD6<T, Width2>::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(static_cast<uint32>(mask.m_data.m_values[0]),
            1_ui32 ^ static_cast<uint32>(mask.m_data.m_values[0]),
            2_ui32 ^ static_cast<uint32>(mask.m_data.m_values[1]),
            3_ui32 ^ static_cast<uint32>(mask.m_data.m_values[1]),
            4_ui32 ^ static_cast<uint32>(mask.m_data.m_values[2]),
            5_ui32 ^ static_cast<uint32>(mask.m_data.m_values[2]),
            6_ui32 ^ static_cast<uint32>(mask.m_data.m_values[3]),
            7_ui32 ^ static_cast<uint32>(mask.m_data.m_values[3]),
            8_ui32 ^ static_cast<uint32>(mask.m_data.m_values[4]),
            9_ui32 ^ static_cast<uint32>(mask.m_data.m_values[4]),
            10_ui32 ^ static_cast<uint32>(mask.m_data.m_values[5]),
            11_ui32 ^ static_cast<uint32>(mask.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::Shuffle SIMD12<T, Width>::Shuffle::Swap4() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(
            2_ui32, 3_ui32, 0_ui32, 1_ui32, 6_ui32, 7_ui32, 4_ui32, 5_ui32, 10_ui32, 11_ui32, 8_ui32, 9_ui32);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD12<T, Width>::Shuffle SIMD12<T, Width>::Shuffle::Swap4(
    const typename SIMD3<T, Width2>::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(static_cast<uint32>(mask.m_data.m_values[0]) << 1_ui8,
            1_ui32 ^ (static_cast<uint32>(mask.m_data.m_values[0]) << 1_ui8),
            2_ui32 ^ (static_cast<uint32>(mask.m_data.m_values[0]) << 1_ui8),
            3_ui32 ^ (static_cast<uint32>(mask.m_data.m_values[0]) << 1_ui8),
            4_ui32 ^ (static_cast<uint32>(mask.m_data.m_values[1]) << 1_ui8),
            5_ui32 ^ (static_cast<uint32>(mask.m_data.m_values[1]) << 1_ui8),
            6_ui32 ^ (static_cast<uint32>(mask.m_data.m_values[1]) << 1_ui8),
            7_ui32 ^ (static_cast<uint32>(mask.m_data.m_values[1]) << 1_ui8),
            8_ui32 ^ (static_cast<uint32>(mask.m_data.m_values[2]) << 1_ui8),
            9_ui32 ^ (static_cast<uint32>(mask.m_data.m_values[2]) << 1_ui8),
            10_ui32 ^ (static_cast<uint32>(mask.m_data.m_values[2]) << 1_ui8),
            11_ui32 ^ (static_cast<uint32>(mask.m_data.m_values[2]) << 1_ui8));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>::SIMD12(const T value0, const T value1, const T value2, const T value3, const T value4,
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
XS_INLINE SIMD12<T, Width>::SIMD12(const T value) noexcept
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
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMDBase<T, Width2>& other) noexcept
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
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMDInBase<T, Width2>& other) noexcept
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
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMD2<T, Width2>& other0, const SIMD2<T, Width2>& other1,
    const SIMD2<T, Width2>& other2, const SIMD2<T, Width2>& other3, const SIMD2<T, Width2>& other4,
    const SIMD2<T, Width2>& other5) noexcept
{
    static_assert(sameImpl<BaseDef, SIMD2<T, Width2>>);
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMD3<T, Width2>& other0, const SIMD3<T, Width2>& other1,
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
        m_data.m_values[1] = other1.m_data.m_values[0];
        m_data.m_values[2] = other2.m_data.m_values[0];
        m_data.m_values[3] = other3.m_data.m_values[0];
        m_data.m_values[4] = other0.m_data.m_values[1];
        m_data.m_values[5] = other1.m_data.m_values[1];
        m_data.m_values[6] = other2.m_data.m_values[1];
        m_data.m_values[7] = other3.m_data.m_values[1];
        m_data.m_values[8] = other0.m_data.m_values[2];
        m_data.m_values[9] = other1.m_data.m_values[2];
        m_data.m_values[10] = other2.m_data.m_values[2];
        m_data.m_values[11] = other3.m_data.m_values[2];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMD3<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other.m_data.m_values[0];
        m_data.m_values[1] = other.m_data.m_values[0];
        m_data.m_values[2] = other.m_data.m_values[0];
        m_data.m_values[3] = other.m_data.m_values[0];
        m_data.m_values[4] = other.m_data.m_values[1];
        m_data.m_values[5] = other.m_data.m_values[1];
        m_data.m_values[6] = other.m_data.m_values[1];
        m_data.m_values[7] = other.m_data.m_values[1];
        m_data.m_values[8] = other.m_data.m_values[2];
        m_data.m_values[9] = other.m_data.m_values[2];
        m_data.m_values[10] = other.m_data.m_values[2];
        m_data.m_values[11] = other.m_data.m_values[2];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>::SIMD12(
    const SIMD4<T, Width2>& other0, const SIMD4<T, Width2>& other1, const SIMD4<T, Width2>& other2) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMD4<T, Width2>& other) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMD6<T, Width2>& other0, const SIMD6<T, Width2>& other1) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
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
        m_data.m_values[4] = other0.m_data.m_values[2];
        m_data.m_values[5] = other0.m_data.m_values[3];
        m_data.m_values[6] = other1.m_data.m_values[2];
        m_data.m_values[7] = other1.m_data.m_values[3];
        m_data.m_values[8] = other0.m_data.m_values[4];
        m_data.m_values[9] = other0.m_data.m_values[5];
        m_data.m_values[10] = other1.m_data.m_values[4];
        m_data.m_values[11] = other1.m_data.m_values[5];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMD6<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
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
        m_data.m_values[4] = other.m_data.m_values[2];
        m_data.m_values[5] = other.m_data.m_values[3];
        m_data.m_values[6] = other.m_data.m_values[2];
        m_data.m_values[7] = other.m_data.m_values[3];
        m_data.m_values[8] = other.m_data.m_values[4];
        m_data.m_values[9] = other.m_data.m_values[5];
        m_data.m_values[10] = other.m_data.m_values[4];
        m_data.m_values[11] = other.m_data.m_values[5];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMD8<T, Width2>& other0, const SIMD4<T, Width2>& other1) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>> && sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMD3x3<T, Width2>& other0, const SIMD3<T, Width2>& other1) noexcept
{
    static_assert(sameImpl<SIMD3x3<T, Width>, SIMD3x3<T, Width2>> && sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other0.m_data.m_values[0];
        m_data.m_values[1] = other0.m_data.m_values[3];
        m_data.m_values[2] = other0.m_data.m_values[6];
        m_data.m_values[3] = other1.m_data.m_values[0];
        m_data.m_values[4] = other0.m_data.m_values[1];
        m_data.m_values[5] = other0.m_data.m_values[4];
        m_data.m_values[6] = other0.m_data.m_values[7];
        m_data.m_values[7] = other1.m_data.m_values[1];
        m_data.m_values[8] = other0.m_data.m_values[2];
        m_data.m_values[9] = other0.m_data.m_values[5];
        m_data.m_values[10] = other0.m_data.m_values[8];
        m_data.m_values[11] = other1.m_data.m_values[2];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>::SIMD12(const SIMD3x4<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD3x4<T, Width>, SIMD3x4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other.m_data.m_values[0];
        m_data.m_values[1] = other.m_data.m_values[3];
        m_data.m_values[2] = other.m_data.m_values[6];
        m_data.m_values[3] = other.m_data.m_values[9];
        m_data.m_values[4] = other.m_data.m_values[1];
        m_data.m_values[5] = other.m_data.m_values[4];
        m_data.m_values[6] = other.m_data.m_values[7];
        m_data.m_values[7] = other.m_data.m_values[10];
        m_data.m_values[8] = other.m_data.m_values[2];
        m_data.m_values[9] = other.m_data.m_values[5];
        m_data.m_values[10] = other.m_data.m_values[8];
        m_data.m_values[11] = other.m_data.m_values[11];
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>::SIMD12(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(T{1});
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE SIMDInBase<T, Width2> SIMD12<T, Width>::getValueInBase() const noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 12);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        SIMDInBase<T, Width2>(m_data.m_values[Index]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width2> SIMD12<T, Width>::getValue2() const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
    static_assert(Index < 6);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        SIMD2<T, Width2>(m_data.m_values[Index * 2], m_data.m_values[Index * 2 + 1]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width2> SIMD12<T, Width>::getValue3() const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        SIMD3<T, Width2>(m_data.m_values[Index], m_data.m_values[Index + 4], m_data.m_values[Index + 8]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width2> SIMD12<T, Width>::getValue4() const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        SIMD4<T, Width2>(m_data.m_values[Index * 4], m_data.m_values[Index * 4 + 1], m_data.m_values[Index * 4 + 2],
            m_data.m_values[Index * 4 + 3]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width2> SIMD12<T, Width>::getValue6() const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        SIMD6<T, Width2>(m_data.m_values[Index * 2], m_data.m_values[Index * 2 + 1], m_data.m_values[Index * 2 + 4],
            m_data.m_values[Index * 2 + 5], m_data.m_values[Index * 2 + 8], m_data.m_values[Index * 2 + 9]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width2> SIMD12<T, Width>::getValue4x2() const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
    static_assert(Index0 < 3 && Index1 < 3);
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
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD12<T, Width>::setValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 12);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index] = other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD12<T, Width>::setValue(const SIMDBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
    static_assert(Index < 12);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index] = other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD12<T, Width>::setValue2(const SIMD2<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
    static_assert(Index < 6);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index * 2] = other.m_data.m_values[0];
        m_data.m_values[Index * 2 + 1] = other.m_data.m_values[1];
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD12<T, Width>::setValue3(const SIMD3<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index] = other.m_data.m_values[0];
        m_data.m_values[Index + 4] = other.m_data.m_values[1];
        m_data.m_values[Index + 8] = other.m_data.m_values[2];
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD12<T, Width>::setValue4(const SIMD4<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index * 4] = other.m_data.m_values[0];
        m_data.m_values[Index * 4 + 1] = other.m_data.m_values[1];
        m_data.m_values[Index * 4 + 2] = other.m_data.m_values[2];
        m_data.m_values[Index * 4 + 3] = other.m_data.m_values[3];
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD12<T, Width>::setValue6(const SIMD6<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index * 2] = other.m_data.m_values[0];
        m_data.m_values[Index * 2 + 1] = other.m_data.m_values[1];
        m_data.m_values[Index * 2 + 4] = other.m_data.m_values[2];
        m_data.m_values[Index * 2 + 5] = other.m_data.m_values[3];
        m_data.m_values[Index * 2 + 8] = other.m_data.m_values[4];
        m_data.m_values[Index * 2 + 9] = other.m_data.m_values[5];
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE void SIMD12<T, Width>::setValue4x2(const SIMD8<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
    static_assert(Index0 < 3 && Index1 < 3);
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
    bool Elem9, bool Elem10, bool Elem11>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::negate() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Elem0 ? -m_data.m_values[0] : m_data.m_values[0],
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
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::mad(const SIMD12<T, Width2>& other1, const SIMD12<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
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
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::mad(
    const SIMDBase<T, Width2>& other1, const SIMD12<T, Width>& other2) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_value, other2.m_data.m_values[0]),
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
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::mad(
    const SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_value),
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
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::mad(const SIMD4<T, Width2>& other1, const SIMD12<T, Width>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
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
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_values[3], other2.m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::mad(const SIMD3<T, Width2>& other1, const SIMD3<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[8], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[9], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[10], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_values[2], other2.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::mad(const SIMD3<T, Width2>& other1, const SIMD12<T, Width>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[0], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[0], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[0], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[1], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[1], other2.m_data.m_values[5]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[1], other2.m_data.m_values[6]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[1], other2.m_data.m_values[7]),
            Shift::fma<T>(m_data.m_values[8], other1.m_data.m_values[2], other2.m_data.m_values[8]),
            Shift::fma<T>(m_data.m_values[9], other1.m_data.m_values[2], other2.m_data.m_values[9]),
            Shift::fma<T>(m_data.m_values[10], other1.m_data.m_values[2], other2.m_data.m_values[10]),
            Shift::fma<T>(m_data.m_values[11], other1.m_data.m_values[2], other2.m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::msub(
    const SIMD12<T, Width2>& other1, const SIMD12<T, Width2>& other2) const noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], -other2.m_data.m_values[0]),
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
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::abs<T>(m_data.m_values[0]), Shift::abs<T>(m_data.m_values[1]),
            Shift::abs<T>(m_data.m_values[2]), Shift::abs<T>(m_data.m_values[3]), Shift::abs<T>(m_data.m_values[4]),
            Shift::abs<T>(m_data.m_values[5]), Shift::abs<T>(m_data.m_values[6]), Shift::abs<T>(m_data.m_values[7]),
            Shift::abs<T>(m_data.m_values[8]), Shift::abs<T>(m_data.m_values[9]), Shift::abs<T>(m_data.m_values[10]),
            Shift::abs<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::max(const SIMD12<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::max<T>(m_data.m_values[0], other.m_data.m_values[0]),
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
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::min(const SIMD12<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::min<T>(m_data.m_values[0], other.m_data.m_values[0]),
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
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::max(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::max<T>(m_data.m_values[0], other.m_data.m_value),
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
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::min(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::min<T>(m_data.m_values[0], other.m_data.m_value),
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
XS_INLINE SIMD2<T, Width> SIMD12<T, Width>::max2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other0 = Shift::max<T>(m_data.m_values[0], m_data.m_values[2]);
        T other1 = Shift::max<T>(m_data.m_values[1], m_data.m_values[3]);
        const T other2 = Shift::max<T>(m_data.m_values[4], m_data.m_values[6]);
        const T other3 = Shift::max<T>(m_data.m_values[5], m_data.m_values[7]);
        const T other4 = Shift::max<T>(m_data.m_values[8], m_data.m_values[10]);
        const T other5 = Shift::max<T>(m_data.m_values[9], m_data.m_values[11]);
        other0 = Shift::max<T>(other0, other2);
        other1 = Shift::max<T>(other1, other3);
        other0 = Shift::max<T>(other0, other4);
        other1 = Shift::max<T>(other1, other5);
        return SIMD2<T, Width>(other0, other1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD12<T, Width>::min2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other0 = Shift::min<T>(m_data.m_values[0], m_data.m_values[2]);
        T other1 = Shift::min<T>(m_data.m_values[1], m_data.m_values[3]);
        const T other2 = Shift::min<T>(m_data.m_values[4], m_data.m_values[6]);
        const T other3 = Shift::min<T>(m_data.m_values[5], m_data.m_values[7]);
        const T other4 = Shift::min<T>(m_data.m_values[8], m_data.m_values[10]);
        const T other5 = Shift::min<T>(m_data.m_values[9], m_data.m_values[11]);
        other0 = Shift::min<T>(other0, other2);
        other1 = Shift::min<T>(other1, other3);
        other0 = Shift::min<T>(other0, other4);
        other1 = Shift::min<T>(other1, other5);
        return SIMD2<T, Width>(other0, other1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD12<T, Width>::max3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other0 = Shift::max<T>(m_data.m_values[0], m_data.m_values[1]);
        const T other1 = Shift::max<T>(m_data.m_values[2], m_data.m_values[3]);
        T other2 = Shift::max<T>(m_data.m_values[4], m_data.m_values[5]);
        const T other3 = Shift::max<T>(m_data.m_values[6], m_data.m_values[7]);
        T other4 = Shift::max<T>(m_data.m_values[8], m_data.m_values[9]);
        const T other5 = Shift::max<T>(m_data.m_values[10], m_data.m_values[11]);
        other0 = Shift::max<T>(other0, other1);
        other2 = Shift::max<T>(other2, other3);
        other4 = Shift::max<T>(other4, other5);
        return SIMD3<T, Width>(other0, other2, other4);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD12<T, Width>::min3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other0 = Shift::min<T>(m_data.m_values[0], m_data.m_values[1]);
        const T other1 = Shift::min<T>(m_data.m_values[2], m_data.m_values[3]);
        T other2 = Shift::min<T>(m_data.m_values[4], m_data.m_values[5]);
        const T other3 = Shift::min<T>(m_data.m_values[6], m_data.m_values[7]);
        T other4 = Shift::min<T>(m_data.m_values[8], m_data.m_values[9]);
        const T other5 = Shift::min<T>(m_data.m_values[10], m_data.m_values[11]);
        other0 = Shift::min<T>(other0, other1);
        other2 = Shift::min<T>(other2, other3);
        other4 = Shift::min<T>(other4, other5);
        return SIMD3<T, Width>(other0, other2, other4);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD12<T, Width>::max4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other0 = Shift::max<T>(m_data.m_values[0], m_data.m_values[4]);
        T other1 = Shift::max<T>(m_data.m_values[1], m_data.m_values[5]);
        T other2 = Shift::max<T>(m_data.m_values[2], m_data.m_values[6]);
        T other3 = Shift::max<T>(m_data.m_values[3], m_data.m_values[7]);
        other0 = Shift::max<T>(other0, m_data.m_values[8]);
        other1 = Shift::max<T>(other1, m_data.m_values[9]);
        other2 = Shift::max<T>(other2, m_data.m_values[10]);
        other3 = Shift::max<T>(other3, m_data.m_values[11]);
        return SIMD4<T, Width>(other0, other1, other2, other3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD12<T, Width>::min4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other0 = Shift::min<T>(m_data.m_values[0], m_data.m_values[4]);
        T other1 = Shift::min<T>(m_data.m_values[1], m_data.m_values[5]);
        T other2 = Shift::min<T>(m_data.m_values[2], m_data.m_values[6]);
        T other3 = Shift::min<T>(m_data.m_values[3], m_data.m_values[7]);
        other0 = Shift::min<T>(other0, m_data.m_values[8]);
        other1 = Shift::min<T>(other1, m_data.m_values[9]);
        other2 = Shift::min<T>(other2, m_data.m_values[10]);
        other3 = Shift::min<T>(other3, m_data.m_values[11]);
        return SIMD4<T, Width>(other0, other1, other2, other3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD12<T, Width>::max6() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T other0 = Shift::max<T>(m_data.m_values[0], m_data.m_values[2]);
        const T other1 = Shift::max<T>(m_data.m_values[1], m_data.m_values[3]);
        const T other2 = Shift::max<T>(m_data.m_values[4], m_data.m_values[6]);
        const T other3 = Shift::max<T>(m_data.m_values[5], m_data.m_values[7]);
        const T other4 = Shift::max<T>(m_data.m_values[8], m_data.m_values[10]);
        const T other5 = Shift::max<T>(m_data.m_values[9], m_data.m_values[11]);
        return SIMD6<T, Width>(other0, other1, other2, other3, other4, other5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD12<T, Width>::min6() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T other0 = Shift::min<T>(m_data.m_values[0], m_data.m_values[2]);
        const T other1 = Shift::min<T>(m_data.m_values[1], m_data.m_values[3]);
        const T other2 = Shift::min<T>(m_data.m_values[4], m_data.m_values[6]);
        const T other3 = Shift::min<T>(m_data.m_values[5], m_data.m_values[7]);
        const T other4 = Shift::min<T>(m_data.m_values[8], m_data.m_values[10]);
        const T other5 = Shift::min<T>(m_data.m_values[9], m_data.m_values[11]);
        return SIMD6<T, Width>(other0, other1, other2, other3, other4, other5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::BaseDef SIMD12<T, Width>::hmax() const noexcept
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
        other = Shift::max<T>(m_data.m_values[6], other);
        other2 = Shift::max<T>(m_data.m_values[7], other2);
        other3 = Shift::max<T>(m_data.m_values[8], other3);
        other = Shift::max<T>(m_data.m_values[9], other);
        other2 = Shift::max<T>(m_data.m_values[10], other2);
        other3 = Shift::max<T>(m_data.m_values[11], other3);
        other = Shift::max<T>(other2, other);
        other = Shift::max<T>(other3, other);
        return BaseDef(other);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::BaseDef SIMD12<T, Width>::hmin() const noexcept
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
        other = Shift::min<T>(m_data.m_values[6], other);
        other2 = Shift::min<T>(m_data.m_values[7], other2);
        other3 = Shift::min<T>(m_data.m_values[8], other3);
        other = Shift::min<T>(m_data.m_values[9], other);
        other2 = Shift::min<T>(m_data.m_values[10], other2);
        other3 = Shift::min<T>(m_data.m_values[11], other3);
        other = Shift::min<T>(other2, other);
        other = Shift::min<T>(other3, other);
        return BaseDef(other);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD12<T, Width>::hmaxInBase() const noexcept
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
XS_INLINE SIMDInBase<T, Width> SIMD12<T, Width>::hminInBase() const noexcept
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
XS_INLINE SIMD6<T, Width> SIMD12<T, Width>::hmax2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(Shift::max<T>(m_data.m_values[0], m_data.m_values[1]),
            Shift::max<T>(m_data.m_values[2], m_data.m_values[3]),
            Shift::max<T>(m_data.m_values[4], m_data.m_values[5]),
            Shift::max<T>(m_data.m_values[6], m_data.m_values[7]),
            Shift::max<T>(m_data.m_values[8], m_data.m_values[9]),
            Shift::max<T>(m_data.m_values[10], m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD12<T, Width>::hmin2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(Shift::min<T>(m_data.m_values[0], m_data.m_values[1]),
            Shift::min<T>(m_data.m_values[2], m_data.m_values[3]),
            Shift::min<T>(m_data.m_values[4], m_data.m_values[5]),
            Shift::min<T>(m_data.m_values[6], m_data.m_values[7]),
            Shift::min<T>(m_data.m_values[8], m_data.m_values[9]),
            Shift::min<T>(m_data.m_values[10], m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD12<T, Width>::hmax3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T other1 = Shift::max<T>(m_data.m_values[0], m_data.m_values[4]);
        const T other2 = Shift::max<T>(m_data.m_values[1], m_data.m_values[5]);
        const T other3 = Shift::max<T>(m_data.m_values[2], m_data.m_values[6]);
        const T other4 = Shift::max<T>(m_data.m_values[3], m_data.m_values[7]);
        return SIMD4<T, Width>(Shift::max<T>(m_data.m_values[8], other1), Shift::max<T>(m_data.m_values[9], other2),
            Shift::max<T>(m_data.m_values[10], other3), Shift::max<T>(m_data.m_values[11], other4));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD12<T, Width>::hmin3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T other1 = Shift::min<T>(m_data.m_values[0], m_data.m_values[4]);
        const T other2 = Shift::min<T>(m_data.m_values[1], m_data.m_values[5]);
        const T other3 = Shift::min<T>(m_data.m_values[2], m_data.m_values[6]);
        const T other4 = Shift::min<T>(m_data.m_values[3], m_data.m_values[7]);
        return SIMD4<T, Width>(Shift::min<T>(m_data.m_values[8], other1), Shift::min<T>(m_data.m_values[9], other2),
            Shift::min<T>(m_data.m_values[10], other3), Shift::min<T>(m_data.m_values[11], other4));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD12<T, Width>::hmax4() const noexcept
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
        other1 = Shift::max<T>(m_data.m_values[2], other1);
        other2 = Shift::max<T>(m_data.m_values[6], other2);
        other3 = Shift::max<T>(m_data.m_values[10], other3);
        return SIMD3<T, Width>(Shift::max<T>(m_data.m_values[3], other1), Shift::max<T>(m_data.m_values[7], other2),
            Shift::max<T>(m_data.m_values[11], other3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD12<T, Width>::hmin4() const noexcept
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
        other1 = Shift::min<T>(m_data.m_values[2], other1);
        other2 = Shift::min<T>(m_data.m_values[6], other2);
        other3 = Shift::min<T>(m_data.m_values[10], other3);
        return SIMD3<T, Width>(Shift::min<T>(m_data.m_values[3], other1), Shift::min<T>(m_data.m_values[7], other2),
            Shift::min<T>(m_data.m_values[11], other3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD12<T, Width>::hmax6() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other0 = Shift::max<T>(m_data.m_values[0], m_data.m_values[1]);
        T other1 = Shift::max<T>(m_data.m_values[2], m_data.m_values[3]);
        const T other2 = Shift::max<T>(m_data.m_values[4], m_data.m_values[5]);
        const T other3 = Shift::max<T>(m_data.m_values[6], m_data.m_values[7]);
        const T other4 = Shift::max<T>(m_data.m_values[8], m_data.m_values[9]);
        const T other5 = Shift::max<T>(m_data.m_values[10], m_data.m_values[11]);
        other0 = Shift::max<T>(other0, other2);
        other1 = Shift::max<T>(other1, other3);
        other0 = Shift::max<T>(other0, other4);
        other1 = Shift::max<T>(other1, other5);
        return SIMD2<T, Width>(other0, other1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD12<T, Width>::hmin6() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other0 = Shift::min<T>(m_data.m_values[0], m_data.m_values[1]);
        T other1 = Shift::min<T>(m_data.m_values[2], m_data.m_values[3]);
        const T other2 = Shift::min<T>(m_data.m_values[4], m_data.m_values[5]);
        const T other3 = Shift::min<T>(m_data.m_values[6], m_data.m_values[7]);
        const T other4 = Shift::min<T>(m_data.m_values[8], m_data.m_values[9]);
        const T other5 = Shift::min<T>(m_data.m_values[10], m_data.m_values[11]);
        other0 = Shift::min<T>(other0, other2);
        other1 = Shift::min<T>(other1, other3);
        other0 = Shift::min<T>(other0, other4);
        other1 = Shift::min<T>(other1, other5);
        return SIMD2<T, Width>(other0, other1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::reciprocal() const noexcept
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
XS_INLINE SIMD2<T, Width> SIMD12<T, Width>::add2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other0 = m_data.m_values[0] + m_data.m_values[2];
        T other1 = m_data.m_values[1] + m_data.m_values[3];
        const T other2 = m_data.m_values[4] + m_data.m_values[6];
        const T other3 = m_data.m_values[5] + m_data.m_values[7];
        const T other4 = m_data.m_values[8] + m_data.m_values[10];
        const T other5 = m_data.m_values[9] + m_data.m_values[11];
        other0 += other2;
        other1 += other3;
        other0 += other4;
        other1 += other5;
        return SIMD2<T, Width>(other0, other1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD12<T, Width>::add3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other0 = m_data.m_values[0] + m_data.m_values[1];
        const T other1 = m_data.m_values[2] + m_data.m_values[3];
        T other2 = m_data.m_values[4] + m_data.m_values[5];
        const T other3 = m_data.m_values[6] + m_data.m_values[7];
        T other4 = m_data.m_values[8] + m_data.m_values[9];
        const T other5 = m_data.m_values[10] + m_data.m_values[11];
        other0 += other1;
        other2 += other3;
        other4 += other5;
        return SIMD3<T, Width>(other0, other2, other4);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD12<T, Width>::add4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other0 = m_data.m_values[0] + m_data.m_values[4];
        T other1 = m_data.m_values[1] + m_data.m_values[5];
        T other2 = m_data.m_values[2] + m_data.m_values[6];
        T other3 = m_data.m_values[3] + m_data.m_values[7];
        other0 += m_data.m_values[8];
        other1 += m_data.m_values[9];
        other2 += m_data.m_values[10];
        other3 += m_data.m_values[11];
        return SIMD4<T, Width>(other0, other1, other2, other3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD12<T, Width>::add6() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T other0 = m_data.m_values[0] + m_data.m_values[2];
        const T other1 = m_data.m_values[1] + m_data.m_values[3];
        const T other2 = m_data.m_values[4] + m_data.m_values[6];
        const T other3 = m_data.m_values[5] + m_data.m_values[7];
        const T other4 = m_data.m_values[8] + m_data.m_values[10];
        const T other5 = m_data.m_values[9] + m_data.m_values[11];
        return SIMD6<T, Width>(other0, other1, other2, other3, other4, other5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD12<T, Width>::sub4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other0 = m_data.m_values[0] - m_data.m_values[4];
        T other1 = m_data.m_values[1] - m_data.m_values[5];
        T other2 = m_data.m_values[2] - m_data.m_values[6];
        T other3 = m_data.m_values[3] - m_data.m_values[7];
        other0 -= m_data.m_values[8];
        other1 -= m_data.m_values[9];
        other2 -= m_data.m_values[10];
        other3 -= m_data.m_values[11];
        return SIMD4<T, Width>(other0, other1, other2, other3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD12<T, Width>::sub6() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T other0 = m_data.m_values[0] - m_data.m_values[2];
        const T other1 = m_data.m_values[1] - m_data.m_values[3];
        const T other2 = m_data.m_values[4] - m_data.m_values[6];
        const T other3 = m_data.m_values[5] - m_data.m_values[7];
        const T other4 = m_data.m_values[8] - m_data.m_values[10];
        const T other5 = m_data.m_values[9] - m_data.m_values[11];
        return SIMD6<T, Width>(other0, other1, other2, other3, other4, other5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD12<T, Width>::BaseDef SIMD12<T, Width>::hadd() const noexcept
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
        res += res3;
        res2 += res4;
        res += res2;
        return BaseDef(res);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD12<T, Width>::haddInBase() const noexcept
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
XS_INLINE SIMD6<T, Width> SIMD12<T, Width>::hadd2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(m_data.m_values[0] + m_data.m_values[1], m_data.m_values[2] + m_data.m_values[3],
            m_data.m_values[4] + m_data.m_values[5], m_data.m_values[6] + m_data.m_values[7],
            m_data.m_values[8] + m_data.m_values[9], m_data.m_values[10] + m_data.m_values[11]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD12<T, Width>::hadd3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T res = (m_data.m_values[0] + m_data.m_values[4]);
        T res2 = (m_data.m_values[1] + m_data.m_values[5]);
        T res3 = (m_data.m_values[2] + m_data.m_values[6]);
        T res4 = (m_data.m_values[3] + m_data.m_values[7]);
        res += m_data.m_values[8];
        res2 += m_data.m_values[9];
        res3 += m_data.m_values[10];
        res4 += m_data.m_values[11];
        return SIMD4<T, Width>(res, res2, res3, res4);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD12<T, Width>::hadd4() const noexcept
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
        res += m_data.m_values[2];
        res2 += m_data.m_values[6];
        res3 += m_data.m_values[10];
        res += m_data.m_values[3];
        res2 += m_data.m_values[7];
        res3 += m_data.m_values[11];
        return SIMD3<T, Width>(res, res2, res3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD12<T, Width>::hadd6() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other0 = m_data.m_values[0] + m_data.m_values[1];
        T other1 = m_data.m_values[2] + m_data.m_values[3];
        const T other2 = m_data.m_values[4] + m_data.m_values[5];
        const T other3 = m_data.m_values[6] + m_data.m_values[7];
        const T other4 = m_data.m_values[8] + m_data.m_values[9];
        const T other5 = m_data.m_values[10] + m_data.m_values[11];
        other0 += other2;
        other1 += other3;
        other0 += other4;
        other1 += other5;
        return SIMD2<T, Width>(other0, other1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD12<T, Width>::hsub2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(m_data.m_values[0] - m_data.m_values[1], m_data.m_values[2] - m_data.m_values[3],
            m_data.m_values[4] - m_data.m_values[5], m_data.m_values[6] - m_data.m_values[7],
            m_data.m_values[8] - m_data.m_values[9], m_data.m_values[10] - m_data.m_values[11]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::ceil<T>(m_data.m_values[0]), Shift::ceil<T>(m_data.m_values[1]),
            Shift::ceil<T>(m_data.m_values[2]), Shift::ceil<T>(m_data.m_values[3]), Shift::ceil<T>(m_data.m_values[4]),
            Shift::ceil<T>(m_data.m_values[5]), Shift::ceil<T>(m_data.m_values[6]), Shift::ceil<T>(m_data.m_values[7]),
            Shift::ceil<T>(m_data.m_values[8]), Shift::ceil<T>(m_data.m_values[9]), Shift::ceil<T>(m_data.m_values[10]),
            Shift::ceil<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::floor<T>(m_data.m_values[0]), Shift::floor<T>(m_data.m_values[1]),
            Shift::floor<T>(m_data.m_values[2]), Shift::floor<T>(m_data.m_values[3]),
            Shift::floor<T>(m_data.m_values[4]), Shift::floor<T>(m_data.m_values[5]),
            Shift::floor<T>(m_data.m_values[6]), Shift::floor<T>(m_data.m_values[7]),
            Shift::floor<T>(m_data.m_values[8]), Shift::floor<T>(m_data.m_values[9]),
            Shift::floor<T>(m_data.m_values[10]), Shift::floor<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::trunc<T>(m_data.m_values[0]), Shift::trunc<T>(m_data.m_values[1]),
            Shift::trunc<T>(m_data.m_values[2]), Shift::trunc<T>(m_data.m_values[3]),
            Shift::trunc<T>(m_data.m_values[4]), Shift::trunc<T>(m_data.m_values[5]),
            Shift::trunc<T>(m_data.m_values[6]), Shift::trunc<T>(m_data.m_values[7]),
            Shift::trunc<T>(m_data.m_values[8]), Shift::trunc<T>(m_data.m_values[9]),
            Shift::trunc<T>(m_data.m_values[10]), Shift::trunc<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::sqrt<T>(m_data.m_values[0]), Shift::sqrt<T>(m_data.m_values[1]),
            Shift::sqrt<T>(m_data.m_values[2]), Shift::sqrt<T>(m_data.m_values[3]), Shift::sqrt<T>(m_data.m_values[4]),
            Shift::sqrt<T>(m_data.m_values[5]), Shift::sqrt<T>(m_data.m_values[6]), Shift::sqrt<T>(m_data.m_values[7]),
            Shift::sqrt<T>(m_data.m_values[8]), Shift::sqrt<T>(m_data.m_values[9]), Shift::sqrt<T>(m_data.m_values[10]),
            Shift::sqrt<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::rsqrt<T>(m_data.m_values[0]), Shift::rsqrt<T>(m_data.m_values[1]),
            Shift::rsqrt<T>(m_data.m_values[2]), Shift::rsqrt<T>(m_data.m_values[3]),
            Shift::rsqrt<T>(m_data.m_values[4]), Shift::rsqrt<T>(m_data.m_values[5]),
            Shift::rsqrt<T>(m_data.m_values[6]), Shift::rsqrt<T>(m_data.m_values[7]),
            Shift::rsqrt<T>(m_data.m_values[8]), Shift::rsqrt<T>(m_data.m_values[9]),
            Shift::rsqrt<T>(m_data.m_values[10]), Shift::rsqrt<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD12<T, Width>::dot3(const SIMD12<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T v0Sq = m_data.m_values[0] * other.m_data.m_values[0];
        T v1Sq = m_data.m_values[1] * other.m_data.m_values[1];
        T v2Sq = m_data.m_values[2] * other.m_data.m_values[2];
        T v3Sq = m_data.m_values[3] * other.m_data.m_values[3];
        v0Sq = Shift::fma<T>(m_data.m_values[4], other.m_data.m_values[4], v0Sq);
        v1Sq = Shift::fma<T>(m_data.m_values[5], other.m_data.m_values[5], v1Sq);
        v2Sq = Shift::fma<T>(m_data.m_values[6], other.m_data.m_values[6], v2Sq);
        v3Sq = Shift::fma<T>(m_data.m_values[7], other.m_data.m_values[7], v3Sq);
        return SIMD4<T, Width>(Shift::fma<T>(m_data.m_values[8], other.m_data.m_values[8], v0Sq),
            Shift::fma<T>(m_data.m_values[9], other.m_data.m_values[9], v1Sq),
            Shift::fma<T>(m_data.m_values[10], other.m_data.m_values[10], v2Sq),
            Shift::fma<T>(m_data.m_values[11], other.m_data.m_values[11], v3Sq));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::cross3(const SIMD12<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T v0CrossX = -(m_data.m_values[8] * other.m_data.m_values[4]);
        const T v1CrossX = -(m_data.m_values[9] * other.m_data.m_values[5]);
        const T v2CrossX = -(m_data.m_values[10] * other.m_data.m_values[6]);
        const T v3CrossX = -(m_data.m_values[11] * other.m_data.m_values[7]);
        const T v0CrossY = -(m_data.m_values[0] * other.m_data.m_values[8]);
        const T v1CrossY = -(m_data.m_values[1] * other.m_data.m_values[9]);
        const T v2CrossY = -(m_data.m_values[2] * other.m_data.m_values[10]);
        const T v3CrossY = -(m_data.m_values[3] * other.m_data.m_values[11]);
        const T v0CrossZ = -(m_data.m_values[4] * other.m_data.m_values[0]);
        const T v1CrossZ = -(m_data.m_values[5] * other.m_data.m_values[1]);
        const T v2CrossZ = -(m_data.m_values[6] * other.m_data.m_values[2]);
        const T v3CrossZ = -(m_data.m_values[7] * other.m_data.m_values[3]);

        return SIMD12(Shift::fma<T>(m_data.m_values[4], other.m_data.m_values[8], v0CrossX),
            Shift::fma<T>(m_data.m_values[5], other.m_data.m_values[9], v1CrossX),
            Shift::fma<T>(m_data.m_values[6], other.m_data.m_values[10], v2CrossX),
            Shift::fma<T>(m_data.m_values[7], other.m_data.m_values[11], v3CrossX),
            Shift::fma<T>(m_data.m_values[8], other.m_data.m_values[0], v0CrossY),
            Shift::fma<T>(m_data.m_values[9], other.m_data.m_values[1], v1CrossY),
            Shift::fma<T>(m_data.m_values[10], other.m_data.m_values[2], v2CrossY),
            Shift::fma<T>(m_data.m_values[11], other.m_data.m_values[3], v3CrossY),
            Shift::fma<T>(m_data.m_values[0], other.m_data.m_values[4], v0CrossZ),
            Shift::fma<T>(m_data.m_values[1], other.m_data.m_values[5], v1CrossZ),
            Shift::fma<T>(m_data.m_values[2], other.m_data.m_values[6], v2CrossZ),
            Shift::fma<T>(m_data.m_values[3], other.m_data.m_values[7], v3CrossZ));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD12<T, Width>::lengthSqr3() const noexcept
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
XS_INLINE SIMD4<T, Width> SIMD12<T, Width>::length3() const noexcept
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
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::normalize3() const noexcept
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
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::exp2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::exp2<T>(m_data.m_values[0]), Shift::exp2<T>(m_data.m_values[1]),
            Shift::exp2<T>(m_data.m_values[2]), Shift::exp2<T>(m_data.m_values[3]), Shift::exp2<T>(m_data.m_values[4]),
            Shift::exp2<T>(m_data.m_values[5]), Shift::exp2<T>(m_data.m_values[6]), Shift::exp2<T>(m_data.m_values[7]),
            Shift::exp2<T>(m_data.m_values[8]), Shift::exp2<T>(m_data.m_values[9]), Shift::exp2<T>(m_data.m_values[10]),
            Shift::exp2<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::exp() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::exp<T>(m_data.m_values[0]), Shift::exp<T>(m_data.m_values[1]),
            Shift::exp<T>(m_data.m_values[2]), Shift::exp<T>(m_data.m_values[3]), Shift::exp<T>(m_data.m_values[4]),
            Shift::exp<T>(m_data.m_values[5]), Shift::exp<T>(m_data.m_values[6]), Shift::exp<T>(m_data.m_values[7]),
            Shift::exp<T>(m_data.m_values[8]), Shift::exp<T>(m_data.m_values[9]), Shift::exp<T>(m_data.m_values[10]),
            Shift::exp<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::log2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::log2<T>(m_data.m_values[0]), Shift::log2<T>(m_data.m_values[1]),
            Shift::log2<T>(m_data.m_values[2]), Shift::log2<T>(m_data.m_values[3]), Shift::log2<T>(m_data.m_values[4]),
            Shift::log2<T>(m_data.m_values[5]), Shift::log2<T>(m_data.m_values[6]), Shift::log2<T>(m_data.m_values[7]),
            Shift::log2<T>(m_data.m_values[8]), Shift::log2<T>(m_data.m_values[9]), Shift::log2<T>(m_data.m_values[10]),
            Shift::log2<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::log() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::log<T>(m_data.m_values[0]), Shift::log<T>(m_data.m_values[1]),
            Shift::log<T>(m_data.m_values[2]), Shift::log<T>(m_data.m_values[3]), Shift::log<T>(m_data.m_values[4]),
            Shift::log<T>(m_data.m_values[5]), Shift::log<T>(m_data.m_values[6]), Shift::log<T>(m_data.m_values[7]),
            Shift::log<T>(m_data.m_values[8]), Shift::log<T>(m_data.m_values[9]), Shift::log<T>(m_data.m_values[10]),
            Shift::log<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::pow(const SIMD12<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::pow<T>(m_data.m_values[0], other.m_data.m_values[0]),
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
            Shift::pow<T>(m_data.m_values[11], other.m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::powr(const SIMD12<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::powr<T>(m_data.m_values[0], other.m_data.m_values[0]),
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
            Shift::powr<T>(m_data.m_values[11], other.m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::pow(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::pow<T>(m_data.m_values[0], other.m_data.m_value),
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
            Shift::pow<T>(m_data.m_values[11], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::powr(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::powr<T>(m_data.m_values[0], other.m_data.m_value),
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
            Shift::powr<T>(m_data.m_values[11], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::sin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::sin<T>(m_data.m_values[0]), Shift::sin<T>(m_data.m_values[1]),
            Shift::sin<T>(m_data.m_values[2]), Shift::sin<T>(m_data.m_values[3]), Shift::sin<T>(m_data.m_values[4]),
            Shift::sin<T>(m_data.m_values[5]), Shift::sin<T>(m_data.m_values[6]), Shift::sin<T>(m_data.m_values[7]),
            Shift::sin<T>(m_data.m_values[8]), Shift::sin<T>(m_data.m_values[9]), Shift::sin<T>(m_data.m_values[10]),
            Shift::sin<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::cos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::cos<T>(m_data.m_values[0]), Shift::cos<T>(m_data.m_values[1]),
            Shift::cos<T>(m_data.m_values[2]), Shift::cos<T>(m_data.m_values[3]), Shift::cos<T>(m_data.m_values[4]),
            Shift::cos<T>(m_data.m_values[5]), Shift::cos<T>(m_data.m_values[6]), Shift::cos<T>(m_data.m_values[7]),
            Shift::cos<T>(m_data.m_values[8]), Shift::cos<T>(m_data.m_values[9]), Shift::cos<T>(m_data.m_values[10]),
            Shift::cos<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::tan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::tan<T>(m_data.m_values[0]), Shift::tan<T>(m_data.m_values[1]),
            Shift::tan<T>(m_data.m_values[2]), Shift::tan<T>(m_data.m_values[3]), Shift::tan<T>(m_data.m_values[4]),
            Shift::tan<T>(m_data.m_values[5]), Shift::tan<T>(m_data.m_values[6]), Shift::tan<T>(m_data.m_values[7]),
            Shift::tan<T>(m_data.m_values[8]), Shift::tan<T>(m_data.m_values[9]), Shift::tan<T>(m_data.m_values[10]),
            Shift::tan<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::asin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::asin<T>(m_data.m_values[0]), Shift::asin<T>(m_data.m_values[1]),
            Shift::asin<T>(m_data.m_values[2]), Shift::asin<T>(m_data.m_values[3]), Shift::asin<T>(m_data.m_values[4]),
            Shift::asin<T>(m_data.m_values[5]), Shift::asin<T>(m_data.m_values[6]), Shift::asin<T>(m_data.m_values[7]),
            Shift::asin<T>(m_data.m_values[8]), Shift::asin<T>(m_data.m_values[9]), Shift::asin<T>(m_data.m_values[10]),
            Shift::asin<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::acos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::acos<T>(m_data.m_values[0]), Shift::acos<T>(m_data.m_values[1]),
            Shift::acos<T>(m_data.m_values[2]), Shift::acos<T>(m_data.m_values[3]), Shift::acos<T>(m_data.m_values[4]),
            Shift::acos<T>(m_data.m_values[5]), Shift::acos<T>(m_data.m_values[6]), Shift::acos<T>(m_data.m_values[7]),
            Shift::acos<T>(m_data.m_values[8]), Shift::acos<T>(m_data.m_values[9]), Shift::acos<T>(m_data.m_values[10]),
            Shift::acos<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::atan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::atan<T>(m_data.m_values[0]), Shift::atan<T>(m_data.m_values[1]),
            Shift::atan<T>(m_data.m_values[2]), Shift::atan<T>(m_data.m_values[3]), Shift::atan<T>(m_data.m_values[4]),
            Shift::atan<T>(m_data.m_values[5]), Shift::atan<T>(m_data.m_values[6]), Shift::atan<T>(m_data.m_values[7]),
            Shift::atan<T>(m_data.m_values[8]), Shift::atan<T>(m_data.m_values[9]), Shift::atan<T>(m_data.m_values[10]),
            Shift::atan<T>(m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::atan2(const SIMD12<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Shift::atan2<T>(m_data.m_values[0], other.m_data.m_values[0]),
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
            Shift::atan2<T>(m_data.m_values[11], other.m_data.m_values[11]));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::insert2(const SIMD12<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
    static_assert(Index0 < 2 && Index1 < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(getValue4<0>().insert2<Index0, Index1>(other.template getQuadFloat<0>()),
            getValue4<1>().insert2<Index0, Index1>(other.template getQuadFloat<1>()),
            getValue4<2>().insert2<Index0, Index1>(other.template getQuadFloat<2>()));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::insert3(const SIMD12<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
    static_assert(Index0 < 3 && Index1 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(SIMD3<T, Width>(m_data.m_values[0], m_data.m_values[4], m_data.m_values[8])
                          .insert<Index0, Index1>(SIMD3<T, Width>(
                              other.m_data.m_values[0], other.m_data.m_values[4], other.m_data.m_values[8])),
            SIMD3<T, Width>(m_data.m_values[1], m_data.m_values[5], m_data.m_values[9])
                .insert<Index0, Index1>(
                    SIMD3<T, Width>(other.m_data.m_values[1], other.m_data.m_values[5], other.m_data.m_values[9])),
            SIMD3<T, Width>(m_data.m_values[2], m_data.m_values[6], m_data.m_values[10])
                .insert<Index0, Index1>(
                    SIMD3<T, Width>(other.m_data.m_values[2], other.m_data.m_values[6], other.m_data.m_values[10])),
            SIMD3<T, Width>(m_data.m_values[3], m_data.m_values[7], m_data.m_values[11])
                .insert<Index0, Index1>(
                    SIMD3<T, Width>(other.m_data.m_values[3], other.m_data.m_values[7], other.m_data.m_values[11])));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::insert4(const SIMD12<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
    static_assert(Index0 < 4 && Index1 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(getValue4<0>().insert<Index0, Index1>(other.getValue4<0>()),
            getValue4<1>().insert<Index0, Index1>(other.getValue4<1>()),
            getValue4<2>().insert<Index0, Index1>(other.getValue4<2>()));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::insert6(const SIMD12<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
    static_assert(Index0 < 6 && Index1 < 6);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(SIMD6<T, Width>(m_data.m_values[0], m_data.m_values[1], m_data.m_values[4], m_data.m_values[5],
                          m_data.m_values[8], m_data.m_values[9])
                          .insert<Index0, Index1>(SIMD6<T, Width>(other.m_data.m_values[0], other.m_data.m_values[1],
                              other.m_data.m_values[4], other.m_data.m_values[5], other.m_data.m_values[8],
                              other.m_data.m_values[9])),
            SIMD6<T, Width>(m_data.m_values[2], m_data.m_values[3], m_data.m_values[6], m_data.m_values[7],
                m_data.m_values[10], m_data.m_values[11])
                .insert<Index0, Index1>(
                    SIMD6<T, Width>(other.m_data.m_values[2], other.m_data.m_values[3], other.m_data.m_values[6],
                        other.m_data.m_values[7], other.m_data.m_values[10], other.m_data.m_values[11])));
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::blend2(const SIMD12<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const auto temp0 = (Elem0) ?
            SIMD6<T, Width>(other.m_data.m_values[0], other.m_data.m_values[2], other.m_data.m_values[4],
                other.m_data.m_values[6], other.m_data.m_values[8], other.m_data.m_values[10]) :
            SIMD6<T, Width>(m_data.m_values[0], m_data.m_values[2], m_data.m_values[4], m_data.m_values[6],
                m_data.m_values[8], m_data.m_values[10]);
        const auto temp1 = (Elem1) ?
            SIMD6<T, Width>(other.m_data.m_values[1], other.m_data.m_values[3], other.m_data.m_values[5],
                other.m_data.m_values[7], other.m_data.m_values[9], other.m_data.m_values[11]) :
            SIMD6<T, Width>(m_data.m_values[1], m_data.m_values[3], m_data.m_values[5], m_data.m_values[7],
                m_data.m_values[9], m_data.m_values[11]);

        return SIMD12(temp0.m_data.m_values[0], temp1.m_data.m_values[0], temp0.m_data.m_values[1],
            temp1.m_data.m_values[1], temp0.m_data.m_values[2], temp1.m_data.m_values[2], temp0.m_data.m_values[3],
            temp1.m_data.m_values[3], temp0.m_data.m_values[4], temp1.m_data.m_values[4], temp0.m_data.m_values[5],
            temp1.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::blend3(const SIMD12<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(Elem0 ? other.getValue4<0>() : getValue4<0>(), Elem1 ? other.getValue4<1>() : getValue4<1>(),
            Elem2 ? other.getValue4<2>() : getValue4<2>());
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::blend4(const SIMD12<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const auto temp0 = (Elem0) ?
            SIMD3<T, Width>(other.m_data.m_values[0], other.m_data.m_values[4], other.m_data.m_values[8]) :
            SIMD3<T, Width>(m_data.m_values[0], m_data.m_values[4], m_data.m_values[8]);
        const auto temp1 = (Elem1) ?
            SIMD3<T, Width>(other.m_data.m_values[1], other.m_data.m_values[5], other.m_data.m_values[9]) :
            SIMD3<T, Width>(m_data.m_values[1], m_data.m_values[5], m_data.m_values[9]);
        const auto temp2 = (Elem2) ?
            SIMD3<T, Width>(other.m_data.m_values[2], other.m_data.m_values[6], other.m_data.m_values[10]) :
            SIMD3<T, Width>(m_data.m_values[2], m_data.m_values[6], m_data.m_values[10]);
        const auto temp3 = (Elem3) ?
            SIMD3<T, Width>(other.m_data.m_values[3], other.m_data.m_values[7], other.m_data.m_values[11]) :
            SIMD3<T, Width>(m_data.m_values[3], m_data.m_values[7], m_data.m_values[11]);

        return SIMD12(temp0.m_data.m_values[0], temp1.m_data.m_values[0], temp2.m_data.m_values[0],
            temp3.m_data.m_values[0], temp0.m_data.m_values[1], temp1.m_data.m_values[1], temp2.m_data.m_values[1],
            temp3.m_data.m_values[1], temp0.m_data.m_values[2], temp1.m_data.m_values[2], temp2.m_data.m_values[2],
            temp3.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::blend6(const SIMD12<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const auto temp0 = (Elem0) ? SIMD2<T, Width>(other.m_data.m_values[0], other.m_data.m_values[2]) :
                                     SIMD2<T, Width>(m_data.m_values[0], m_data.m_values[2]);
        const auto temp1 = (Elem1) ? SIMD2<T, Width>(other.m_data.m_values[1], other.m_data.m_values[3]) :
                                     SIMD2<T, Width>(m_data.m_values[1], m_data.m_values[3]);
        const auto temp2 = (Elem2) ? SIMD2<T, Width>(other.m_data.m_values[4], other.m_data.m_values[6]) :
                                     SIMD2<T, Width>(m_data.m_values[4], m_data.m_values[6]);
        const auto temp3 = (Elem3) ? SIMD2<T, Width>(other.m_data.m_values[5], other.m_data.m_values[7]) :
                                     SIMD2<T, Width>(m_data.m_values[5], m_data.m_values[7]);
        const auto temp4 = (Elem4) ? SIMD2<T, Width>(other.m_data.m_values[8], other.m_data.m_values[10]) :
                                     SIMD2<T, Width>(m_data.m_values[8], m_data.m_values[10]);
        const auto temp5 = (Elem5) ? SIMD2<T, Width>(other.m_data.m_values[9], other.m_data.m_values[11]) :
                                     SIMD2<T, Width>(m_data.m_values[9], m_data.m_values[11]);

        return SIMD12(temp0.m_data.m_values[0], temp1.m_data.m_values[0], temp0.m_data.m_values[1],
            temp1.m_data.m_values[1], temp2.m_data.m_values[0], temp3.m_data.m_values[0], temp2.m_data.m_values[1],
            temp3.m_data.m_values[1], temp4.m_data.m_values[0], temp5.m_data.m_values[0], temp4.m_data.m_values[1],
            temp5.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::shuffleVar(const Shuffle& shuffle) const noexcept
{
    XS_ASSERT(shuffle.m_data.m_values[0] < 12 && shuffle.m_data.m_values[1] < 12 && shuffle.m_data.m_values[2] < 12 &&
        shuffle.m_data.m_values[3] < 12 && shuffle.m_data.m_values[4] < 12 && shuffle.m_data.m_values[5] < 12 &&
        shuffle.m_data.m_values[6] < 12 && shuffle.m_data.m_values[7] < 12 && shuffle.m_data.m_values[8] < 12 &&
        shuffle.m_data.m_values[9] < 12 && shuffle.m_data.m_values[10] < 12 && shuffle.m_data.m_values[11] < 12);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(m_data.m_values[shuffle.m_data.m_values[0]], m_data.m_values[shuffle.m_data.m_values[1]],
            m_data.m_values[shuffle.m_data.m_values[2]], m_data.m_values[shuffle.m_data.m_values[3]],
            m_data.m_values[shuffle.m_data.m_values[4]], m_data.m_values[shuffle.m_data.m_values[5]],
            m_data.m_values[shuffle.m_data.m_values[6]], m_data.m_values[shuffle.m_data.m_values[7]],
            m_data.m_values[shuffle.m_data.m_values[8]], m_data.m_values[shuffle.m_data.m_values[9]],
            m_data.m_values[shuffle.m_data.m_values[10]], m_data.m_values[shuffle.m_data.m_values[11]]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::shuffle2() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(getValue4<0>().shuffleDual<Index0, Index1>(), getValue4<1>().shuffleDual<Index0, Index1>(),
            getValue4<2>().shuffleDual<Index0, Index1>());
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::shuffle3() const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3);
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
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD16<T, Width> SIMD12<T, Width>::shuffle3() const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3 && Index3 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD16(getValue4<Index0>(), getValue4<Index1>(), getValue4<Index2>(), getValue4<Index3>()).transpose4();
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::shuffle4() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(getValue4<0>().shuffle<Index0, Index1, Index2, Index3>(),
            getValue4<1>().shuffle<Index0, Index1, Index2, Index3>(),
            getValue4<2>().shuffle<Index0, Index1, Index2, Index3>());
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3, uint32 Index4, uint32 Index5>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::shuffle6() const noexcept
{
    static_assert(Index0 < 6 && Index1 < 6 && Index2 < 6 && Index3 < 6 && Index4 < 6 && Index5 < 6);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12(m_data.m_values[((Index0 / 2) * 2) + Index0], m_data.m_values[((Index1 / 2) * 2) + Index1],
            m_data.m_values[((Index0 / 2) * 2) + Index0 + 2], m_data.m_values[((Index1 / 2) * 2) + Index1 + 2],
            m_data.m_values[((Index2 / 2) * 2) + Index2], m_data.m_values[((Index3 / 2) * 2) + Index3],
            m_data.m_values[((Index2 / 2) * 2) + Index2 + 2], m_data.m_values[((Index3 / 2) * 2) + Index3 + 2],
            m_data.m_values[((Index4 / 2) * 2) + Index4], m_data.m_values[((Index5 / 2) * 2) + Index5],
            m_data.m_values[((Index4 / 2) * 2) + Index4 + 2], m_data.m_values[((Index5 / 2) * 2) + Index5 + 2]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE SIMD12<T, Width> SIMD12<T, Width>::shuffleH4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return shuffle3<Index0, Index1, Index2>();
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator+(const SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
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
XS_INLINE SIMD12<T, Width> operator+(const SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_value,
            other1.m_data.m_values[1] + other2.m_data.m_value, other1.m_data.m_values[2] + other2.m_data.m_value,
            other1.m_data.m_values[3] + other2.m_data.m_value, other1.m_data.m_values[4] + other2.m_data.m_value,
            other1.m_data.m_values[5] + other2.m_data.m_value, other1.m_data.m_values[6] + other2.m_data.m_value,
            other1.m_data.m_values[7] + other2.m_data.m_value, other1.m_data.m_values[8] + other2.m_data.m_value,
            other1.m_data.m_values[9] + other2.m_data.m_value, other1.m_data.m_values[10] + other2.m_data.m_value,
            other1.m_data.m_values[11] + other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator+(const SIMD12<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
            other1.m_data.m_values[1] + other2.m_data.m_values[1],
            other1.m_data.m_values[2] + other2.m_data.m_values[0],
            other1.m_data.m_values[3] + other2.m_data.m_values[1],
            other1.m_data.m_values[4] + other2.m_data.m_values[0],
            other1.m_data.m_values[5] + other2.m_data.m_values[1],
            other1.m_data.m_values[6] + other2.m_data.m_values[0],
            other1.m_data.m_values[7] + other2.m_data.m_values[1],
            other1.m_data.m_values[8] + other2.m_data.m_values[0],
            other1.m_data.m_values[9] + other2.m_data.m_values[1],
            other1.m_data.m_values[10] + other2.m_data.m_values[0],
            other1.m_data.m_values[11] + other2.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator+(const SIMD12<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
            other1.m_data.m_values[1] + other2.m_data.m_values[0],
            other1.m_data.m_values[2] + other2.m_data.m_values[0],
            other1.m_data.m_values[3] + other2.m_data.m_values[0],
            other1.m_data.m_values[4] + other2.m_data.m_values[1],
            other1.m_data.m_values[5] + other2.m_data.m_values[1],
            other1.m_data.m_values[6] + other2.m_data.m_values[1],
            other1.m_data.m_values[7] + other2.m_data.m_values[1],
            other1.m_data.m_values[8] + other2.m_data.m_values[2],
            other1.m_data.m_values[9] + other2.m_data.m_values[2],
            other1.m_data.m_values[10] + other2.m_data.m_values[2],
            other1.m_data.m_values[11] + other2.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator+(const SIMD12<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
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
            other1.m_data.m_values[11] + other2.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator+(const SIMD12<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
            other1.m_data.m_values[1] + other2.m_data.m_values[1],
            other1.m_data.m_values[2] + other2.m_data.m_values[0],
            other1.m_data.m_values[3] + other2.m_data.m_values[1],
            other1.m_data.m_values[4] + other2.m_data.m_values[2],
            other1.m_data.m_values[5] + other2.m_data.m_values[3],
            other1.m_data.m_values[6] + other2.m_data.m_values[2],
            other1.m_data.m_values[7] + other2.m_data.m_values[3],
            other1.m_data.m_values[8] + other2.m_data.m_values[4],
            other1.m_data.m_values[9] + other2.m_data.m_values[5],
            other1.m_data.m_values[10] + other2.m_data.m_values[4],
            other1.m_data.m_values[11] + other2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator-(const SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
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
XS_INLINE SIMD12<T, Width> operator-(const SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_value,
            other1.m_data.m_values[1] - other2.m_data.m_value, other1.m_data.m_values[2] - other2.m_data.m_value,
            other1.m_data.m_values[3] - other2.m_data.m_value, other1.m_data.m_values[4] - other2.m_data.m_value,
            other1.m_data.m_values[5] - other2.m_data.m_value, other1.m_data.m_values[6] - other2.m_data.m_value,
            other1.m_data.m_values[7] - other2.m_data.m_value, other1.m_data.m_values[8] - other2.m_data.m_value,
            other1.m_data.m_values[9] - other2.m_data.m_value, other1.m_data.m_values[10] - other2.m_data.m_value,
            other1.m_data.m_values[11] - other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator-(const SIMDBase<T, Width2>& other1, const SIMD12<T, Width>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_value - other2.m_data.m_values[0],
            other1.m_data.m_value - other2.m_data.m_values[1], other1.m_data.m_value - other2.m_data.m_values[2],
            other1.m_data.m_value - other2.m_data.m_values[3], other1.m_data.m_value - other2.m_data.m_values[4],
            other1.m_data.m_value - other2.m_data.m_values[5], other1.m_data.m_value - other2.m_data.m_values[6],
            other1.m_data.m_value - other2.m_data.m_values[7], other1.m_data.m_value - other2.m_data.m_values[8],
            other1.m_data.m_value - other2.m_data.m_values[9], other1.m_data.m_value - other2.m_data.m_values[10],
            other1.m_data.m_value - other2.m_data.m_values[11]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator-(const SIMD12<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
            other1.m_data.m_values[1] - other2.m_data.m_values[1],
            other1.m_data.m_values[2] - other2.m_data.m_values[0],
            other1.m_data.m_values[3] - other2.m_data.m_values[1],
            other1.m_data.m_values[4] - other2.m_data.m_values[0],
            other1.m_data.m_values[5] - other2.m_data.m_values[1],
            other1.m_data.m_values[6] - other2.m_data.m_values[0],
            other1.m_data.m_values[7] - other2.m_data.m_values[1],
            other1.m_data.m_values[8] - other2.m_data.m_values[0],
            other1.m_data.m_values[9] - other2.m_data.m_values[1],
            other1.m_data.m_values[10] - other2.m_data.m_values[0],
            other1.m_data.m_values[11] - other2.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator-(const SIMD12<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
            other1.m_data.m_values[1] - other2.m_data.m_values[0],
            other1.m_data.m_values[2] - other2.m_data.m_values[0],
            other1.m_data.m_values[3] - other2.m_data.m_values[0],
            other1.m_data.m_values[4] - other2.m_data.m_values[1],
            other1.m_data.m_values[5] - other2.m_data.m_values[1],
            other1.m_data.m_values[6] - other2.m_data.m_values[1],
            other1.m_data.m_values[7] - other2.m_data.m_values[1],
            other1.m_data.m_values[8] - other2.m_data.m_values[2],
            other1.m_data.m_values[9] - other2.m_data.m_values[2],
            other1.m_data.m_values[10] - other2.m_data.m_values[2],
            other1.m_data.m_values[11] - other2.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator-(const SIMD12<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
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
            other1.m_data.m_values[11] - other2.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator-(const SIMD12<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
            other1.m_data.m_values[1] - other2.m_data.m_values[1],
            other1.m_data.m_values[2] - other2.m_data.m_values[0],
            other1.m_data.m_values[3] - other2.m_data.m_values[1],
            other1.m_data.m_values[4] - other2.m_data.m_values[2],
            other1.m_data.m_values[5] - other2.m_data.m_values[3],
            other1.m_data.m_values[6] - other2.m_data.m_values[2],
            other1.m_data.m_values[7] - other2.m_data.m_values[3],
            other1.m_data.m_values[8] - other2.m_data.m_values[4],
            other1.m_data.m_values[9] - other2.m_data.m_values[5],
            other1.m_data.m_values[10] - other2.m_data.m_values[4],
            other1.m_data.m_values[11] - other2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator*(const SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
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
XS_INLINE SIMD12<T, Width> operator*(const SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_value,
            other1.m_data.m_values[1] * other2.m_data.m_value, other1.m_data.m_values[2] * other2.m_data.m_value,
            other1.m_data.m_values[3] * other2.m_data.m_value, other1.m_data.m_values[4] * other2.m_data.m_value,
            other1.m_data.m_values[5] * other2.m_data.m_value, other1.m_data.m_values[6] * other2.m_data.m_value,
            other1.m_data.m_values[7] * other2.m_data.m_value, other1.m_data.m_values[8] * other2.m_data.m_value,
            other1.m_data.m_values[9] * other2.m_data.m_value, other1.m_data.m_values[10] * other2.m_data.m_value,
            other1.m_data.m_values[11] * other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator*(const SIMD12<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
            other1.m_data.m_values[1] * other2.m_data.m_values[1],
            other1.m_data.m_values[2] * other2.m_data.m_values[0],
            other1.m_data.m_values[3] * other2.m_data.m_values[1],
            other1.m_data.m_values[4] * other2.m_data.m_values[0],
            other1.m_data.m_values[5] * other2.m_data.m_values[1],
            other1.m_data.m_values[6] * other2.m_data.m_values[0],
            other1.m_data.m_values[7] * other2.m_data.m_values[1],
            other1.m_data.m_values[8] * other2.m_data.m_values[0],
            other1.m_data.m_values[9] * other2.m_data.m_values[1],
            other1.m_data.m_values[10] * other2.m_data.m_values[0],
            other1.m_data.m_values[11] * other2.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator*(const SIMD12<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
            other1.m_data.m_values[1] * other2.m_data.m_values[0],
            other1.m_data.m_values[2] * other2.m_data.m_values[0],
            other1.m_data.m_values[3] * other2.m_data.m_values[0],
            other1.m_data.m_values[4] * other2.m_data.m_values[1],
            other1.m_data.m_values[5] * other2.m_data.m_values[1],
            other1.m_data.m_values[6] * other2.m_data.m_values[1],
            other1.m_data.m_values[7] * other2.m_data.m_values[1],
            other1.m_data.m_values[8] * other2.m_data.m_values[2],
            other1.m_data.m_values[9] * other2.m_data.m_values[2],
            other1.m_data.m_values[10] * other2.m_data.m_values[2],
            other1.m_data.m_values[11] * other2.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator*(const SIMD12<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
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
            other1.m_data.m_values[11] * other2.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator*(const SIMD12<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
            other1.m_data.m_values[1] * other2.m_data.m_values[1],
            other1.m_data.m_values[2] * other2.m_data.m_values[0],
            other1.m_data.m_values[3] * other2.m_data.m_values[1],
            other1.m_data.m_values[4] * other2.m_data.m_values[2],
            other1.m_data.m_values[5] * other2.m_data.m_values[3],
            other1.m_data.m_values[6] * other2.m_data.m_values[2],
            other1.m_data.m_values[7] * other2.m_data.m_values[3],
            other1.m_data.m_values[8] * other2.m_data.m_values[4],
            other1.m_data.m_values[9] * other2.m_data.m_values[5],
            other1.m_data.m_values[10] * other2.m_data.m_values[4],
            other1.m_data.m_values[11] * other2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator/(const SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
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
XS_INLINE SIMD12<T, Width> operator/(const SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD12<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_value,
                other1.m_data.m_values[1] / other2.m_data.m_value, other1.m_data.m_values[2] / other2.m_data.m_value,
                other1.m_data.m_values[3] / other2.m_data.m_value, other1.m_data.m_values[4] / other2.m_data.m_value,
                other1.m_data.m_values[5] / other2.m_data.m_value, other1.m_data.m_values[6] / other2.m_data.m_value,
                other1.m_data.m_values[7] / other2.m_data.m_value, other1.m_data.m_values[8] / other2.m_data.m_value,
                other1.m_data.m_values[9] / other2.m_data.m_value, other1.m_data.m_values[10] / other2.m_data.m_value,
                other1.m_data.m_values[11] / other2.m_data.m_value);
        } else {
            const T divisor = Shift::recip<T>(other2.m_data.m_value);
            return SIMD12<T, Width>(other1.m_data.m_values[0] * divisor, other1.m_data.m_values[1] * divisor,
                other1.m_data.m_values[2] * divisor, other1.m_data.m_values[3] * divisor,
                other1.m_data.m_values[4] * divisor, other1.m_data.m_values[5] * divisor,
                other1.m_data.m_values[6] * divisor, other1.m_data.m_values[7] * divisor,
                other1.m_data.m_values[8] * divisor, other1.m_data.m_values[9] * divisor,
                other1.m_data.m_values[10] * divisor, other1.m_data.m_values[11] * divisor);
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator/(const SIMDBase<T, Width2>& other1, const SIMD12<T, Width>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(other1.m_data.m_value / other2.m_data.m_values[0],
            other1.m_data.m_value / other2.m_data.m_values[1], other1.m_data.m_value / other2.m_data.m_values[2],
            other1.m_data.m_value / other2.m_data.m_values[3], other1.m_data.m_value / other2.m_data.m_values[4],
            other1.m_data.m_value / other2.m_data.m_values[5], other1.m_data.m_value / other2.m_data.m_values[6],
            other1.m_data.m_value / other2.m_data.m_values[7], other1.m_data.m_value / other2.m_data.m_values[8],
            other1.m_data.m_value / other2.m_data.m_values[9], other1.m_data.m_value / other2.m_data.m_values[10],
            other1.m_data.m_value / other2.m_data.m_values[11]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator/(const SIMD12<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD12<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
                other1.m_data.m_values[1] / other2.m_data.m_values[1],
                other1.m_data.m_values[2] / other2.m_data.m_values[0],
                other1.m_data.m_values[3] / other2.m_data.m_values[1],
                other1.m_data.m_values[4] / other2.m_data.m_values[0],
                other1.m_data.m_values[5] / other2.m_data.m_values[1],
                other1.m_data.m_values[6] / other2.m_data.m_values[0],
                other1.m_data.m_values[7] / other2.m_data.m_values[1],
                other1.m_data.m_values[8] / other2.m_data.m_values[0],
                other1.m_data.m_values[9] / other2.m_data.m_values[1],
                other1.m_data.m_values[10] / other2.m_data.m_values[0],
                other1.m_data.m_values[11] / other2.m_data.m_values[1]);
        } else {
            const T divisor0 = Shift::recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = Shift::recip<T>(other2.m_data.m_values[1]);
            return SIMD12<T, Width>(other1.m_data.m_values[0] * divisor0, other1.m_data.m_values[1] * divisor1,
                other1.m_data.m_values[2] * divisor0, other1.m_data.m_values[3] * divisor1,
                other1.m_data.m_values[4] * divisor0, other1.m_data.m_values[5] * divisor1,
                other1.m_data.m_values[6] * divisor0, other1.m_data.m_values[7] * divisor1,
                other1.m_data.m_values[8] * divisor0, other1.m_data.m_values[9] * divisor1,
                other1.m_data.m_values[10] * divisor0, other1.m_data.m_values[11] * divisor1);
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator/(const SIMD12<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD12<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
                other1.m_data.m_values[1] / other2.m_data.m_values[0],
                other1.m_data.m_values[2] / other2.m_data.m_values[0],
                other1.m_data.m_values[3] / other2.m_data.m_values[0],
                other1.m_data.m_values[4] / other2.m_data.m_values[1],
                other1.m_data.m_values[5] / other2.m_data.m_values[1],
                other1.m_data.m_values[6] / other2.m_data.m_values[1],
                other1.m_data.m_values[7] / other2.m_data.m_values[1],
                other1.m_data.m_values[8] / other2.m_data.m_values[2],
                other1.m_data.m_values[9] / other2.m_data.m_values[2],
                other1.m_data.m_values[10] / other2.m_data.m_values[2],
                other1.m_data.m_values[11] / other2.m_data.m_values[2]);
        } else {
            const T divisor0 = Shift::recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = Shift::recip<T>(other2.m_data.m_values[1]);
            const T divisor2 = Shift::recip<T>(other2.m_data.m_values[2]);
            return SIMD12<T, Width>(other1.m_data.m_values[0] * divisor0, other1.m_data.m_values[1] * divisor0,
                other1.m_data.m_values[2] * divisor0, other1.m_data.m_values[3] * divisor0,
                other1.m_data.m_values[4] * divisor1, other1.m_data.m_values[5] * divisor1,
                other1.m_data.m_values[6] * divisor1, other1.m_data.m_values[7] * divisor1,
                other1.m_data.m_values[8] * divisor2, other1.m_data.m_values[9] * divisor2,
                other1.m_data.m_values[10] * divisor2, other1.m_data.m_values[11] * divisor2);
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator/(const SIMD12<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD12<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
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
                other1.m_data.m_values[11] / other2.m_data.m_values[3]);
        } else {
            const T divisor0 = Shift::recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = Shift::recip<T>(other2.m_data.m_values[1]);
            const T divisor2 = Shift::recip<T>(other2.m_data.m_values[2]);
            const T divisor3 = Shift::recip<T>(other2.m_data.m_values[3]);
            return SIMD12<T, Width>(other1.m_data.m_values[0] * divisor0, other1.m_data.m_values[1] * divisor1,
                other1.m_data.m_values[2] * divisor2, other1.m_data.m_values[3] * divisor3,
                other1.m_data.m_values[4] * divisor0, other1.m_data.m_values[5] * divisor1,
                other1.m_data.m_values[6] * divisor2, other1.m_data.m_values[7] * divisor3,
                other1.m_data.m_values[8] * divisor0, other1.m_data.m_values[9] * divisor1,
                other1.m_data.m_values[10] * divisor2, other1.m_data.m_values[11] * divisor3);
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator/(const SIMD12<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD12<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
                other1.m_data.m_values[1] / other2.m_data.m_values[1],
                other1.m_data.m_values[2] / other2.m_data.m_values[0],
                other1.m_data.m_values[3] / other2.m_data.m_values[1],
                other1.m_data.m_values[4] / other2.m_data.m_values[2],
                other1.m_data.m_values[5] / other2.m_data.m_values[3],
                other1.m_data.m_values[6] / other2.m_data.m_values[2],
                other1.m_data.m_values[7] / other2.m_data.m_values[3],
                other1.m_data.m_values[8] / other2.m_data.m_values[4],
                other1.m_data.m_values[9] / other2.m_data.m_values[5],
                other1.m_data.m_values[10] / other2.m_data.m_values[4],
                other1.m_data.m_values[11] / other2.m_data.m_values[5]);
        } else {
            const T divisor0 = Shift::recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = Shift::recip<T>(other2.m_data.m_values[1]);
            const T divisor2 = Shift::recip<T>(other2.m_data.m_values[2]);
            const T divisor3 = Shift::recip<T>(other2.m_data.m_values[3]);
            const T divisor4 = Shift::recip<T>(other2.m_data.m_values[4]);
            const T divisor5 = Shift::recip<T>(other2.m_data.m_values[5]);
            return SIMD12<T, Width>(other1.m_data.m_values[0] * divisor0, other1.m_data.m_values[1] * divisor1,
                other1.m_data.m_values[2] * divisor0, other1.m_data.m_values[3] * divisor1,
                other1.m_data.m_values[4] * divisor2, other1.m_data.m_values[5] * divisor3,
                other1.m_data.m_values[6] * divisor2, other1.m_data.m_values[7] * divisor3,
                other1.m_data.m_values[8] * divisor4, other1.m_data.m_values[9] * divisor5,
                other1.m_data.m_values[10] * divisor4, other1.m_data.m_values[11] * divisor5);
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator-(const SIMD12<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD12<T, Width>(-other.m_data.m_values[0], -other.m_data.m_values[1], -other.m_data.m_values[2],
            -other.m_data.m_values[3], -other.m_data.m_values[4], -other.m_data.m_values[5], -other.m_data.m_values[6],
            -other.m_data.m_values[7], -other.m_data.m_values[8], -other.m_data.m_values[9], -other.m_data.m_values[10],
            -other.m_data.m_values[11]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>& operator+=(SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
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
XS_INLINE SIMD12<T, Width>& operator+=(SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>);
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
XS_INLINE SIMD12<T, Width>& operator+=(SIMD12<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] += other2.m_data.m_values[0];
        other1.m_data.m_values[1] += other2.m_data.m_values[1];
        other1.m_data.m_values[2] += other2.m_data.m_values[0];
        other1.m_data.m_values[3] += other2.m_data.m_values[1];
        other1.m_data.m_values[4] += other2.m_data.m_values[0];
        other1.m_data.m_values[5] += other2.m_data.m_values[1];
        other1.m_data.m_values[6] += other2.m_data.m_values[0];
        other1.m_data.m_values[7] += other2.m_data.m_values[1];
        other1.m_data.m_values[8] += other2.m_data.m_values[0];
        other1.m_data.m_values[9] += other2.m_data.m_values[1];
        other1.m_data.m_values[10] += other2.m_data.m_values[0];
        other1.m_data.m_values[11] += other2.m_data.m_values[1];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>& operator+=(SIMD12<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] += other2.m_data.m_values[0];
        other1.m_data.m_values[1] += other2.m_data.m_values[0];
        other1.m_data.m_values[2] += other2.m_data.m_values[0];
        other1.m_data.m_values[3] += other2.m_data.m_values[0];
        other1.m_data.m_values[4] += other2.m_data.m_values[1];
        other1.m_data.m_values[5] += other2.m_data.m_values[1];
        other1.m_data.m_values[6] += other2.m_data.m_values[1];
        other1.m_data.m_values[7] += other2.m_data.m_values[1];
        other1.m_data.m_values[8] += other2.m_data.m_values[2];
        other1.m_data.m_values[9] += other2.m_data.m_values[2];
        other1.m_data.m_values[10] += other2.m_data.m_values[2];
        other1.m_data.m_values[11] += other2.m_data.m_values[2];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>& operator+=(SIMD12<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
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
        other1.m_data.m_values[4] += other2.m_data.m_values[0];
        other1.m_data.m_values[5] += other2.m_data.m_values[1];
        other1.m_data.m_values[6] += other2.m_data.m_values[2];
        other1.m_data.m_values[7] += other2.m_data.m_values[3];
        other1.m_data.m_values[8] += other2.m_data.m_values[0];
        other1.m_data.m_values[9] += other2.m_data.m_values[1];
        other1.m_data.m_values[10] += other2.m_data.m_values[2];
        other1.m_data.m_values[11] += other2.m_data.m_values[3];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>& operator+=(SIMD12<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] += other2.m_data.m_values[0];
        other1.m_data.m_values[1] += other2.m_data.m_values[1];
        other1.m_data.m_values[2] += other2.m_data.m_values[0];
        other1.m_data.m_values[3] += other2.m_data.m_values[1];
        other1.m_data.m_values[4] += other2.m_data.m_values[2];
        other1.m_data.m_values[5] += other2.m_data.m_values[3];
        other1.m_data.m_values[6] += other2.m_data.m_values[2];
        other1.m_data.m_values[7] += other2.m_data.m_values[3];
        other1.m_data.m_values[8] += other2.m_data.m_values[4];
        other1.m_data.m_values[9] += other2.m_data.m_values[5];
        other1.m_data.m_values[10] += other2.m_data.m_values[4];
        other1.m_data.m_values[11] += other2.m_data.m_values[5];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>& operator-=(SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
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
XS_INLINE SIMD12<T, Width>& operator-=(SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>);
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
XS_INLINE SIMD12<T, Width>& operator-=(SIMD12<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] -= other2.m_data.m_values[0];
        other1.m_data.m_values[1] -= other2.m_data.m_values[1];
        other1.m_data.m_values[2] -= other2.m_data.m_values[0];
        other1.m_data.m_values[3] -= other2.m_data.m_values[1];
        other1.m_data.m_values[4] -= other2.m_data.m_values[0];
        other1.m_data.m_values[5] -= other2.m_data.m_values[1];
        other1.m_data.m_values[6] -= other2.m_data.m_values[0];
        other1.m_data.m_values[7] -= other2.m_data.m_values[1];
        other1.m_data.m_values[8] -= other2.m_data.m_values[0];
        other1.m_data.m_values[9] -= other2.m_data.m_values[1];
        other1.m_data.m_values[10] -= other2.m_data.m_values[0];
        other1.m_data.m_values[11] -= other2.m_data.m_values[1];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>& operator-=(SIMD12<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] -= other2.m_data.m_values[0];
        other1.m_data.m_values[1] -= other2.m_data.m_values[0];
        other1.m_data.m_values[2] -= other2.m_data.m_values[0];
        other1.m_data.m_values[3] -= other2.m_data.m_values[0];
        other1.m_data.m_values[4] -= other2.m_data.m_values[1];
        other1.m_data.m_values[5] -= other2.m_data.m_values[1];
        other1.m_data.m_values[6] -= other2.m_data.m_values[1];
        other1.m_data.m_values[7] -= other2.m_data.m_values[1];
        other1.m_data.m_values[8] -= other2.m_data.m_values[2];
        other1.m_data.m_values[9] -= other2.m_data.m_values[2];
        other1.m_data.m_values[10] -= other2.m_data.m_values[2];
        other1.m_data.m_values[11] -= other2.m_data.m_values[2];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>& operator-=(SIMD12<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
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
        other1.m_data.m_values[4] -= other2.m_data.m_values[0];
        other1.m_data.m_values[5] -= other2.m_data.m_values[1];
        other1.m_data.m_values[6] -= other2.m_data.m_values[2];
        other1.m_data.m_values[7] -= other2.m_data.m_values[3];
        other1.m_data.m_values[8] -= other2.m_data.m_values[0];
        other1.m_data.m_values[9] -= other2.m_data.m_values[1];
        other1.m_data.m_values[10] -= other2.m_data.m_values[2];
        other1.m_data.m_values[11] -= other2.m_data.m_values[3];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>& operator-=(SIMD12<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] -= other2.m_data.m_values[0];
        other1.m_data.m_values[1] -= other2.m_data.m_values[1];
        other1.m_data.m_values[2] -= other2.m_data.m_values[0];
        other1.m_data.m_values[3] -= other2.m_data.m_values[1];
        other1.m_data.m_values[4] -= other2.m_data.m_values[2];
        other1.m_data.m_values[5] -= other2.m_data.m_values[3];
        other1.m_data.m_values[6] -= other2.m_data.m_values[2];
        other1.m_data.m_values[7] -= other2.m_data.m_values[3];
        other1.m_data.m_values[8] -= other2.m_data.m_values[4];
        other1.m_data.m_values[9] -= other2.m_data.m_values[5];
        other1.m_data.m_values[10] -= other2.m_data.m_values[4];
        other1.m_data.m_values[11] -= other2.m_data.m_values[5];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>& operator*=(SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
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
XS_INLINE SIMD12<T, Width>& operator*=(SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>);
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
XS_INLINE SIMD12<T, Width>& operator*=(SIMD12<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] *= other2.m_data.m_values[0];
        other1.m_data.m_values[1] *= other2.m_data.m_values[1];
        other1.m_data.m_values[2] *= other2.m_data.m_values[0];
        other1.m_data.m_values[3] *= other2.m_data.m_values[1];
        other1.m_data.m_values[4] *= other2.m_data.m_values[0];
        other1.m_data.m_values[5] *= other2.m_data.m_values[1];
        other1.m_data.m_values[6] *= other2.m_data.m_values[0];
        other1.m_data.m_values[7] *= other2.m_data.m_values[1];
        other1.m_data.m_values[8] *= other2.m_data.m_values[0];
        other1.m_data.m_values[9] *= other2.m_data.m_values[1];
        other1.m_data.m_values[10] *= other2.m_data.m_values[0];
        other1.m_data.m_values[11] *= other2.m_data.m_values[1];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>& operator*=(SIMD12<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] *= other2.m_data.m_values[0];
        other1.m_data.m_values[1] *= other2.m_data.m_values[0];
        other1.m_data.m_values[2] *= other2.m_data.m_values[0];
        other1.m_data.m_values[3] *= other2.m_data.m_values[0];
        other1.m_data.m_values[4] *= other2.m_data.m_values[1];
        other1.m_data.m_values[5] *= other2.m_data.m_values[1];
        other1.m_data.m_values[6] *= other2.m_data.m_values[1];
        other1.m_data.m_values[7] *= other2.m_data.m_values[1];
        other1.m_data.m_values[8] *= other2.m_data.m_values[2];
        other1.m_data.m_values[9] *= other2.m_data.m_values[2];
        other1.m_data.m_values[10] *= other2.m_data.m_values[2];
        other1.m_data.m_values[11] *= other2.m_data.m_values[2];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>& operator*=(SIMD12<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
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
        other1.m_data.m_values[4] *= other2.m_data.m_values[0];
        other1.m_data.m_values[5] *= other2.m_data.m_values[1];
        other1.m_data.m_values[6] *= other2.m_data.m_values[2];
        other1.m_data.m_values[7] *= other2.m_data.m_values[3];
        other1.m_data.m_values[8] *= other2.m_data.m_values[0];
        other1.m_data.m_values[9] *= other2.m_data.m_values[1];
        other1.m_data.m_values[10] *= other2.m_data.m_values[2];
        other1.m_data.m_values[11] *= other2.m_data.m_values[3];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>& operator*=(SIMD12<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] *= other2.m_data.m_values[0];
        other1.m_data.m_values[1] *= other2.m_data.m_values[1];
        other1.m_data.m_values[2] *= other2.m_data.m_values[0];
        other1.m_data.m_values[3] *= other2.m_data.m_values[1];
        other1.m_data.m_values[4] *= other2.m_data.m_values[2];
        other1.m_data.m_values[5] *= other2.m_data.m_values[3];
        other1.m_data.m_values[6] *= other2.m_data.m_values[2];
        other1.m_data.m_values[7] *= other2.m_data.m_values[3];
        other1.m_data.m_values[8] *= other2.m_data.m_values[4];
        other1.m_data.m_values[9] *= other2.m_data.m_values[5];
        other1.m_data.m_values[10] *= other2.m_data.m_values[4];
        other1.m_data.m_values[11] *= other2.m_data.m_values[5];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>& operator/=(SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
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
XS_INLINE SIMD12<T, Width>& operator/=(SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>);
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
XS_INLINE SIMD12<T, Width>& operator/=(SIMD12<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            other1.m_data.m_values[0] /= other2.m_data.m_values[0];
            other1.m_data.m_values[1] /= other2.m_data.m_values[1];
            other1.m_data.m_values[2] /= other2.m_data.m_values[0];
            other1.m_data.m_values[3] /= other2.m_data.m_values[1];
            other1.m_data.m_values[4] /= other2.m_data.m_values[0];
            other1.m_data.m_values[5] /= other2.m_data.m_values[1];
            other1.m_data.m_values[6] /= other2.m_data.m_values[0];
            other1.m_data.m_values[7] /= other2.m_data.m_values[1];
            other1.m_data.m_values[8] /= other2.m_data.m_values[0];
            other1.m_data.m_values[9] /= other2.m_data.m_values[1];
            other1.m_data.m_values[10] /= other2.m_data.m_values[0];
            other1.m_data.m_values[11] /= other2.m_data.m_values[1];
        } else {
            const T divisor0 = Shift::recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = Shift::recip<T>(other2.m_data.m_values[1]);
            other1.m_data.m_values[0] *= divisor0;
            other1.m_data.m_values[1] *= divisor1;
            other1.m_data.m_values[2] *= divisor0;
            other1.m_data.m_values[3] *= divisor1;
            other1.m_data.m_values[4] *= divisor0;
            other1.m_data.m_values[5] *= divisor1;
            other1.m_data.m_values[6] *= divisor0;
            other1.m_data.m_values[7] *= divisor1;
            other1.m_data.m_values[8] *= divisor0;
            other1.m_data.m_values[9] *= divisor1;
            other1.m_data.m_values[10] *= divisor0;
            other1.m_data.m_values[11] *= divisor1;
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>& operator/=(SIMD12<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
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
            other1.m_data.m_values[1] /= other2.m_data.m_values[0];
            other1.m_data.m_values[2] /= other2.m_data.m_values[0];
            other1.m_data.m_values[3] /= other2.m_data.m_values[0];
            other1.m_data.m_values[4] /= other2.m_data.m_values[1];
            other1.m_data.m_values[5] /= other2.m_data.m_values[1];
            other1.m_data.m_values[6] /= other2.m_data.m_values[1];
            other1.m_data.m_values[7] /= other2.m_data.m_values[1];
            other1.m_data.m_values[8] /= other2.m_data.m_values[2];
            other1.m_data.m_values[9] /= other2.m_data.m_values[2];
            other1.m_data.m_values[10] /= other2.m_data.m_values[2];
            other1.m_data.m_values[11] /= other2.m_data.m_values[2];
        } else {
            const T divisor0 = Shift::recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = Shift::recip<T>(other2.m_data.m_values[1]);
            const T divisor2 = Shift::recip<T>(other2.m_data.m_values[2]);
            other1.m_data.m_values[0] *= divisor0;
            other1.m_data.m_values[1] *= divisor0;
            other1.m_data.m_values[2] *= divisor0;
            other1.m_data.m_values[3] *= divisor0;
            other1.m_data.m_values[4] *= divisor1;
            other1.m_data.m_values[5] *= divisor1;
            other1.m_data.m_values[6] *= divisor1;
            other1.m_data.m_values[7] *= divisor1;
            other1.m_data.m_values[8] *= divisor2;
            other1.m_data.m_values[9] *= divisor2;
            other1.m_data.m_values[10] *= divisor2;
            other1.m_data.m_values[11] *= divisor2;
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>& operator/=(SIMD12<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
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
            other1.m_data.m_values[1] /= other2.m_data.m_values[1];
            other1.m_data.m_values[2] /= other2.m_data.m_values[2];
            other1.m_data.m_values[3] /= other2.m_data.m_values[3];
            other1.m_data.m_values[4] /= other2.m_data.m_values[0];
            other1.m_data.m_values[5] /= other2.m_data.m_values[1];
            other1.m_data.m_values[6] /= other2.m_data.m_values[2];
            other1.m_data.m_values[7] /= other2.m_data.m_values[3];
            other1.m_data.m_values[8] /= other2.m_data.m_values[0];
            other1.m_data.m_values[9] /= other2.m_data.m_values[1];
            other1.m_data.m_values[10] /= other2.m_data.m_values[2];
            other1.m_data.m_values[11] /= other2.m_data.m_values[3];
        } else {
            const T divisor0 = Shift::recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = Shift::recip<T>(other2.m_data.m_values[1]);
            const T divisor2 = Shift::recip<T>(other2.m_data.m_values[2]);
            const T divisor3 = Shift::recip<T>(other2.m_data.m_values[3]);
            other1.m_data.m_values[0] *= divisor0;
            other1.m_data.m_values[1] *= divisor1;
            other1.m_data.m_values[2] *= divisor2;
            other1.m_data.m_values[3] *= divisor3;
            other1.m_data.m_values[4] *= divisor0;
            other1.m_data.m_values[5] *= divisor1;
            other1.m_data.m_values[6] *= divisor2;
            other1.m_data.m_values[7] *= divisor3;
            other1.m_data.m_values[8] *= divisor0;
            other1.m_data.m_values[9] *= divisor1;
            other1.m_data.m_values[10] *= divisor2;
            other1.m_data.m_values[11] *= divisor3;
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width>& operator/=(SIMD12<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            other1.m_data.m_values[0] /= other2.m_data.m_values[0];
            other1.m_data.m_values[1] /= other2.m_data.m_values[1];
            other1.m_data.m_values[2] /= other2.m_data.m_values[0];
            other1.m_data.m_values[3] /= other2.m_data.m_values[1];
            other1.m_data.m_values[4] /= other2.m_data.m_values[2];
            other1.m_data.m_values[5] /= other2.m_data.m_values[3];
            other1.m_data.m_values[6] /= other2.m_data.m_values[2];
            other1.m_data.m_values[7] /= other2.m_data.m_values[3];
            other1.m_data.m_values[8] /= other2.m_data.m_values[4];
            other1.m_data.m_values[9] /= other2.m_data.m_values[5];
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
            other1.m_data.m_values[2] *= divisor0;
            other1.m_data.m_values[3] *= divisor1;
            other1.m_data.m_values[4] *= divisor2;
            other1.m_data.m_values[5] *= divisor3;
            other1.m_data.m_values[6] *= divisor2;
            other1.m_data.m_values[7] *= divisor3;
            other1.m_data.m_values[8] *= divisor4;
            other1.m_data.m_values[9] *= divisor5;
            other1.m_data.m_values[10] *= divisor4;
            other1.m_data.m_values[11] *= divisor5;
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD12<T, Width> operator&(const SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(other1.m_data.m_values[0] & other2.m_data.m_values[0],
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
            return SIMD12<T, Width>(Shift::bitAnd(other1.m_data.m_values[0], other2.m_data.m_values[0]),
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
XS_INLINE SIMD12<T, Width> operator&(const SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(other1.m_data.m_values[0] & other2.m_data.m_value,
                other1.m_data.m_values[1] & other2.m_data.m_value, other1.m_data.m_values[2] & other2.m_data.m_value,
                other1.m_data.m_values[3] & other2.m_data.m_value, other1.m_data.m_values[4] & other2.m_data.m_value,
                other1.m_data.m_values[5] & other2.m_data.m_value, other1.m_data.m_values[6] & other2.m_data.m_value,
                other1.m_data.m_values[7] & other2.m_data.m_value, other1.m_data.m_values[8] & other2.m_data.m_value,
                other1.m_data.m_values[9] & other2.m_data.m_value, other1.m_data.m_values[10] & other2.m_data.m_value,
                other1.m_data.m_values[11] & other2.m_data.m_value);
        } else {
            return SIMD12<T, Width>(Shift::bitAnd(other1.m_data.m_values[0], other2.m_data.m_value),
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
XS_INLINE SIMD12<T, Width> operator|(const SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(other1.m_data.m_values[0] | other2.m_data.m_values[0],
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
            return SIMD12<T, Width>(Shift::bitOr(other1.m_data.m_values[0], other2.m_data.m_values[0]),
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
XS_INLINE SIMD12<T, Width> operator|(const SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(other1.m_data.m_values[0] | other2.m_data.m_value,
                other1.m_data.m_values[1] | other2.m_data.m_value, other1.m_data.m_values[2] | other2.m_data.m_value,
                other1.m_data.m_values[3] | other2.m_data.m_value, other1.m_data.m_values[4] | other2.m_data.m_value,
                other1.m_data.m_values[5] | other2.m_data.m_value, other1.m_data.m_values[6] | other2.m_data.m_value,
                other1.m_data.m_values[7] | other2.m_data.m_value, other1.m_data.m_values[8] | other2.m_data.m_value,
                other1.m_data.m_values[9] | other2.m_data.m_value, other1.m_data.m_values[10] | other2.m_data.m_value,
                other1.m_data.m_values[11] | other2.m_data.m_value);
        } else {
            return SIMD12<T, Width>(Shift::bitOr(other1.m_data.m_values[0], other2.m_data.m_value),
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
XS_INLINE SIMD12<T, Width> operator^(const SIMD12<T, Width>& other1, const SIMD12<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD12<T, Width>, SIMD12<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(other1.m_data.m_values[0] ^ other2.m_data.m_values[0],
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
            return SIMD12<T, Width>(Shift::bitXor(other1.m_data.m_values[0], other2.m_data.m_values[0]),
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
XS_INLINE SIMD12<T, Width> operator^(const SIMD12<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD12<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(other1.m_data.m_values[0] ^ other2.m_data.m_value,
                other1.m_data.m_values[1] ^ other2.m_data.m_value, other1.m_data.m_values[2] ^ other2.m_data.m_value,
                other1.m_data.m_values[3] ^ other2.m_data.m_value, other1.m_data.m_values[4] ^ other2.m_data.m_value,
                other1.m_data.m_values[5] ^ other2.m_data.m_value, other1.m_data.m_values[6] ^ other2.m_data.m_value,
                other1.m_data.m_values[7] ^ other2.m_data.m_value, other1.m_data.m_values[8] ^ other2.m_data.m_value,
                other1.m_data.m_values[9] ^ other2.m_data.m_value, other1.m_data.m_values[10] ^ other2.m_data.m_value,
                other1.m_data.m_values[11] ^ other2.m_data.m_value);
        } else {
            return SIMD12<T, Width>(Shift::bitXor(other1.m_data.m_values[0], other2.m_data.m_value),
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
XS_INLINE SIMD12<T, Width> operator~(const SIMD12<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(~other.m_data.m_values[0], ~other.m_data.m_values[1], ~other.m_data.m_values[2],
                ~other.m_data.m_values[3], ~other.m_data.m_values[4], ~other.m_data.m_values[5],
                ~other.m_data.m_values[6], ~other.m_data.m_values[7], ~other.m_data.m_values[8],
                ~other.m_data.m_values[9], ~other.m_data.m_values[10], ~other.m_data.m_values[11]);
        } else {
            return SIMD12<T, Width>(Shift::bitNot(other.m_data.m_values[0]), Shift::bitNot(other.m_data.m_values[1]),
                Shift::bitNot(other.m_data.m_values[2]), Shift::bitNot(other.m_data.m_values[3]),
                Shift::bitNot(other.m_data.m_values[4]), Shift::bitNot(other.m_data.m_values[5]),
                Shift::bitNot(other.m_data.m_values[6]), Shift::bitNot(other.m_data.m_values[7]),
                Shift::bitNot(other.m_data.m_values[8]), Shift::bitNot(other.m_data.m_values[9]),
                Shift::bitNot(other.m_data.m_values[10]), Shift::bitNot(other.m_data.m_values[11]));
        }
    }
}
} // namespace Shift
