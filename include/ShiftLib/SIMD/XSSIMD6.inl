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

#include "SIMD/XSSIMD6.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#endif
#include "XSMath.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD6Data<T>::SIMD6Data(const SIMD6<T, Width>& other) noexcept
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
    }
}

template<typename T>
XS_INLINE void SIMD6Data<T>::setData(
    const T value0, const T value1, const T value2, const T value3, const T value4, const T value5) noexcept
{
    m_value0 = value0;
    m_value1 = value1;
    m_value2 = value2;
    m_value3 = value3;
    m_value4 = value4;
    m_value5 = value5;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD6Data<T>::store(const SIMD6<T, Width>& other) noexcept
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
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6Data<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(m_value0, m_value1, m_value2, m_value3, m_value4, m_value5);
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD6DataPad<T>::SIMD6DataPad(const SIMD6<T, Width>& other) noexcept
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
    }
}

template<typename T>
XS_INLINE void SIMD6DataPad<T>::setData(
    const T value0, const T value1, const T value2, const T value3, const T value4, const T value5) noexcept
{
    m_value0 = value0;
    m_value1 = value1;
    m_value2 = value2;
    m_value3 = value3;
    m_value4 = value4;
    m_value5 = value5;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD6DataPad<T>::store(const SIMD6<T, Width>& other) noexcept
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
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6DataPad<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(m_value0, m_value1, m_value2, m_value3, m_value4, m_value5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width>::Mask::Mask(const uint32 mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = mask & 0x1;
        m_data.m_values[1] = mask & 0x2;
        m_data.m_values[2] = mask & 0x4;
        m_data.m_values[3] = mask & 0x8;
        m_data.m_values[4] = mask & 0x10;
        m_data.m_values[5] = mask & 0x20;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width>::Mask::Mask(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width>::Mask::Mask(
    const typename SIMD3<T, Width>::Mask& mask0, const typename SIMD3<T, Width>::Mask& mask1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = mask0.m_data.m_values[0];
        m_data.m_values[1] = mask1.m_data.m_values[0];
        m_data.m_values[2] = mask0.m_data.m_values[1];
        m_data.m_values[3] = mask1.m_data.m_values[1];
        m_data.m_values[4] = mask0.m_data.m_values[2];
        m_data.m_values[5] = mask1.m_data.m_values[2];
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width>::Mask::Mask(
    const bool bool0, const bool bool1, const bool bool2, const bool bool3, const bool bool4, const bool bool5) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = bool0;
        m_data.m_values[1] = bool1;
        m_data.m_values[2] = bool2;
        m_data.m_values[3] = bool3;
        m_data.m_values[4] = bool4;
        m_data.m_values[5] = bool5;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE Bool6<hasSIMD<T> && (Width > SIMDWidth::Scalar)> SIMD6<T, Width>::Mask::getBool6() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Bool6(m_data.m_values[0], m_data.m_values[1], m_data.m_values[2], m_data.m_values[3], m_data.m_values[4],
            m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD6<T, Width>::Mask::getAny() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return (m_data.m_values[0] | m_data.m_values[1]) | (m_data.m_values[2] | m_data.m_values[3]) |
            (m_data.m_values[4] | m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD6<T, Width>::Mask::getAll() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return (m_data.m_values[0] & m_data.m_values[1]) & (m_data.m_values[2] & m_data.m_values[3]) &
            (m_data.m_values[4] & m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD6<T, Width>::Mask::getNone() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return !getAny();
    }
}

template<typename T, SIMDWidth Width, uint8 Index>
class SIMDMasker6
{
public:
    XS_INLINE static SIMDInBase<T, Width>& ToType(SIMD6<T, Width>& other)
    {
        return reinterpret_cast<SIMDInBase<T, Width>>(other.m_data.m_values)[Index];
    }
};
template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD6<T, Width>::Mask::mask6Function(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if (m_data.m_values[0]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 0>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 0>>(value);
        }
        if (m_data.m_values[1]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 1>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 1>>(value);
        }
        if (m_data.m_values[2]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 2>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 2>>(value);
        }
        if (m_data.m_values[3]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 3>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 3>>(value);
        }
        if (m_data.m_values[4]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 4>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 4>>(value);
        }
        if (m_data.m_values[5]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 5>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 5>>(value);
        }
    }
}

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD6<T, Width>::Mask::mask6ElseFunction(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        auto value = (m_data.m_values[0]) ?
            maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 0>>() :
            maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 0>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 0>>(value);
        value = (m_data.m_values[1]) ? maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 1>>() :
                                       maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 1>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 1>>(value);
        value = (m_data.m_values[2]) ? maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 2>>() :
                                       maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 2>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 2>>(value);
        value = (m_data.m_values[3]) ? maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 3>>() :
                                       maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 3>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 3>>(value);
        value = (m_data.m_values[4]) ? maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 4>>() :
                                       maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 4>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 4>>(value);
        value = (m_data.m_values[5]) ? maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 5>>() :
                                       maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 5>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker6<T, Width, 5>>(value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width>::Shuffle::Shuffle(const uint32 shuffle0, const uint32 shuffle1, const uint32 shuffle2,
    const uint32 shuffle3, const uint32 shuffle4, const uint32 shuffle5) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width>::Shuffle::Shuffle(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD6<T, Width>::Shuffle SIMD6<T, Width>::Shuffle::Swap2() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(1_ui32, 0_ui32, 3_ui32, 2_ui32, 5_ui32, 4_ui32);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD6<T, Width>::Shuffle SIMD6<T, Width>::Shuffle::Swap2(
    const typename SIMD3<T, Width2>::Mask& mask) noexcept
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
            5_ui32 ^ static_cast<uint32>(mask.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width>::SIMD6(
    const T value0, const T value1, const T value2, const T value3, const T value4, const T value5) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width>::SIMD6(const T value) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>::SIMD6(const SIMDBase<T, Width2>& other) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>::SIMD6(const SIMDInBase<T, Width2>& other) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>::SIMD6(
    const SIMD2<T, Width2>& other0, const SIMD2<T, Width2>& other1, const SIMD2<T, Width2>& other2) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>::SIMD6(const SIMD2<T, Width2>& other) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>::SIMD6(const SIMD3<T, Width2>& other0, const SIMD3<T, Width2>& other1) noexcept
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
        m_data.m_values[2] = other0.m_data.m_values[1];
        m_data.m_values[3] = other1.m_data.m_values[1];
        m_data.m_values[4] = other0.m_data.m_values[2];
        m_data.m_values[5] = other1.m_data.m_values[2];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>::SIMD6(const SIMD3<T, Width2>& other) noexcept
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
        m_data.m_values[2] = other.m_data.m_values[1];
        m_data.m_values[3] = other.m_data.m_values[1];
        m_data.m_values[4] = other.m_data.m_values[2];
        m_data.m_values[5] = other.m_data.m_values[2];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>::SIMD6(const SIMD3x2<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>::SIMD6(const SIMD4<T, Width2>& other0, const SIMD2<T, Width2>& other1) noexcept
{
    XS_REQUIRES((sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>> && sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width>::SIMD6(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(T{1});
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE SIMDInBase<T, Width2> SIMD6<T, Width>::getValueInBase() const noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 6);
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
XS_INLINE SIMD2<T, Width2> SIMD6<T, Width>::getValue2() const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
    static_assert(Index < 3);
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
XS_INLINE SIMD3<T, Width2> SIMD6<T, Width>::getValue3() const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width2>(m_data.m_values[Index], m_data.m_values[Index + 2], m_data.m_values[Index + 4]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD6<T, Width>::setValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 6);
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
XS_INLINE void SIMD6<T, Width>::setValue(const SIMDBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
    static_assert(Index < 6);
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
XS_INLINE void SIMD6<T, Width>::setValue2(const SIMD2<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index * 2] = other.m_data.m_values[0];
        m_data.m_values[Index * 2 + 1] = other.m_data.m_value[1];
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD6<T, Width>::setValue3(const SIMD3<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index] = other.m_data.m_values[0];
        m_data.m_values[Index + 2] = other.m_data.m_values[1];
        m_data.m_values[Index + 4] = other.m_data.m_values[2];
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD6<T, Width>::addValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 6);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index] += other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD6<T, Width>::subValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 6);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index] -= other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD6<T, Width>::mulValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 6);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index] *= other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD6<T, Width>::divValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 6);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index] /= other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::negate() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Elem0 ? -m_data.m_values[0] : m_data.m_values[0], Elem1 ? -m_data.m_values[1] : m_data.m_values[1],
            Elem2 ? -m_data.m_values[2] : m_data.m_values[2], Elem3 ? -m_data.m_values[3] : m_data.m_values[3],
            Elem4 ? -m_data.m_values[4] : m_data.m_values[4], Elem5 ? -m_data.m_values[5] : m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::mad(const SIMD6<T, Width2>& other1, const SIMD6<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[4], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[5], other2.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::mad(const SIMDBase<T, Width2>& other1, const SIMD6<T, Width>& other2) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_value, other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_value, other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_value, other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_value, other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_value, other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_value, other2.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::mad(const SIMD6<T, Width>& other1, const SIMDBase<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[4], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[5], other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::mad(const SIMD2<T, Width2>& other1, const SIMD6<T, Width>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[0], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[1], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[0], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[1], other2.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::mad(const SIMD3<T, Width2>& other1, const SIMD6<T, Width>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[0], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[1], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[1], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[2], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[2], other2.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::mad(const SIMD3<T, Width2>& other1, const SIMD3<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[2], other2.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::msub(const SIMD6<T, Width2>& other1, const SIMD6<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], -other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], -other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], -other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], -other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[4], -other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[5], -other2.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD6<T, Width>::Mask SIMD6<T, Width>::equalMask(const SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] == other.m_data.m_values[0], m_data.m_values[1] == other.m_data.m_values[1],
            m_data.m_values[2] == other.m_data.m_values[2], m_data.m_values[3] == other.m_data.m_values[3],
            m_data.m_values[4] == other.m_data.m_values[4], m_data.m_values[5] == other.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD6<T, Width>::Mask SIMD6<T, Width>::lessOrEqualMask(const SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] <= other.m_data.m_values[0], m_data.m_values[1] <= other.m_data.m_values[1],
            m_data.m_values[2] <= other.m_data.m_values[2], m_data.m_values[3] <= other.m_data.m_values[3],
            m_data.m_values[4] <= other.m_data.m_values[4], m_data.m_values[5] <= other.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD6<T, Width>::Mask SIMD6<T, Width>::lessThanMask(const SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] < other.m_data.m_values[0], m_data.m_values[1] < other.m_data.m_values[1],
            m_data.m_values[2] < other.m_data.m_values[2], m_data.m_values[3] < other.m_data.m_values[3],
            m_data.m_values[4] < other.m_data.m_values[4], m_data.m_values[5] < other.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD6<T, Width>::Mask SIMD6<T, Width>::notEqualMask(const SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] != other.m_data.m_values[0], m_data.m_values[1] != other.m_data.m_values[1],
            m_data.m_values[2] != other.m_data.m_values[2], m_data.m_values[3] != other.m_data.m_values[3],
            m_data.m_values[4] != other.m_data.m_values[4], m_data.m_values[5] != other.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD6<T, Width>::Mask SIMD6<T, Width>::equalMask(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] == other.m_data.m_value, m_data.m_values[1] == other.m_data.m_value,
            m_data.m_values[2] == other.m_data.m_value, m_data.m_values[3] == other.m_data.m_value,
            m_data.m_values[4] == other.m_data.m_value, m_data.m_values[5] == other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD6<T, Width>::Mask SIMD6<T, Width>::lessOrEqualMask(const SIMDBase<T, Width2>& other) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] <= other.m_data.m_value, m_data.m_values[1] <= other.m_data.m_value,
            m_data.m_values[2] <= other.m_data.m_value, m_data.m_values[3] <= other.m_data.m_value,
            m_data.m_values[4] <= other.m_data.m_value, m_data.m_values[5] <= other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD6<T, Width>::Mask SIMD6<T, Width>::lessThanMask(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] < other.m_data.m_value, m_data.m_values[1] < other.m_data.m_value,
            m_data.m_values[2] < other.m_data.m_value, m_data.m_values[3] < other.m_data.m_value,
            m_data.m_values[4] < other.m_data.m_value, m_data.m_values[5] < other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD6<T, Width>::Mask SIMD6<T, Width>::greaterOrEqualMask(const SIMDBase<T, Width2>& other) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] >= other.m_data.m_value, m_data.m_values[1] >= other.m_data.m_value,
            m_data.m_values[2] >= other.m_data.m_value, m_data.m_values[3] >= other.m_data.m_value,
            m_data.m_values[4] >= other.m_data.m_value, m_data.m_values[5] >= other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD6<T, Width>::Mask SIMD6<T, Width>::greaterThanMask(const SIMDBase<T, Width2>& other) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] > other.m_data.m_value, m_data.m_values[1] > other.m_data.m_value,
            m_data.m_values[2] > other.m_data.m_value, m_data.m_values[3] > other.m_data.m_value,
            m_data.m_values[4] > other.m_data.m_value, m_data.m_values[5] > other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD6<T, Width>::Mask SIMD6<T, Width>::notEqualMask(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] != other.m_data.m_value, m_data.m_values[1] != other.m_data.m_value,
            m_data.m_values[2] != other.m_data.m_value, m_data.m_values[3] != other.m_data.m_value,
            m_data.m_values[4] != other.m_data.m_value, m_data.m_values[5] != other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::sign(const SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::sign<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::sign<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::sign<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::sign<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::sign<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::sign<T>(m_data.m_values[5], other.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::abs<T>(m_data.m_values[0]), Shift::abs<T>(m_data.m_values[1]),
            Shift::abs<T>(m_data.m_values[2]), Shift::abs<T>(m_data.m_values[3]), Shift::abs<T>(m_data.m_values[4]),
            Shift::abs<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::max(const SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::max<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::max<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::max<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::max<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::max<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::max<T>(m_data.m_values[5], other.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::min(const SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::min<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::min<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::min<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::min<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::min<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::min<T>(m_data.m_values[5], other.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::max(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::max<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[3], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[4], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[5], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::min(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::min<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[3], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[4], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[5], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD6<T, Width>::max2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T other1 = Shift::max<T>(m_data.m_values[0], m_data.m_values[2]);
        const T other2 = Shift::max<T>(m_data.m_values[1], m_data.m_values[3]);
        return SIMD2(Shift::max<T>(other1, m_data.m_values[4]), Shift::max<T>(other2, m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD6<T, Width>::min2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T other1 = Shift::min<T>(m_data.m_values[0], m_data.m_values[2]);
        const T other2 = Shift::min<T>(m_data.m_values[1], m_data.m_values[3]);
        return SIMD2(Shift::min<T>(other1, m_data.m_values[4]), Shift::min<T>(other2, m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD6<T, Width>::max3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::max<T>(m_data.m_values[0], m_data.m_values[1]),
            Shift::max<T>(m_data.m_values[2], m_data.m_values[3]),
            Shift::max<T>(m_data.m_values[4], m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD6<T, Width>::min3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::min<T>(m_data.m_values[0], m_data.m_values[1]),
            Shift::min<T>(m_data.m_values[2], m_data.m_values[3]),
            Shift::min<T>(m_data.m_values[4], m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD6<T, Width>::BaseDef SIMD6<T, Width>::hmax() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other = Shift::max<T>(m_data.m_values[0], m_data.m_values[1]);
        const T other2 = Shift::max<T>(m_data.m_values[2], m_data.m_values[3]);
        const T other3 = Shift::max<T>(m_data.m_values[4], m_data.m_values[5]);
        other = Shift::max<T>(other, other2);
        return BaseDef(Shift::max<T>(other, other3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD6<T, Width>::BaseDef SIMD6<T, Width>::hmin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other = Shift::min<T>(m_data.m_values[0], m_data.m_values[1]);
        const T other2 = Shift::min<T>(m_data.m_values[2], m_data.m_values[3]);
        const T other3 = Shift::min<T>(m_data.m_values[4], m_data.m_values[5]);
        other = Shift::min<T>(other, other2);
        return BaseDef(Shift::min<T>(other, other3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD6<T, Width>::hmaxInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width>(hmax());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD6<T, Width>::hminInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width>(hmin());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD6<T, Width>::BaseDef SIMD6<T, Width>::hmaxIndex(uint32& index) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other = Shift::max<T>(m_data.m_values[0], m_data.m_values[1]);
        index = (other == m_data.m_values[0]) ? 0 : 1;
        T other2 = Shift::max<T>(m_data.m_values[2], m_data.m_values[3]);
        uint32 index2 = (other2 == m_data.m_values[2]) ? 2 : 3;
        const T other3 = Shift::max<T>(m_data.m_values[4], m_data.m_values[5]);
        const uint32 index3 = (other3 == m_data.m_values[4]) ? 4 : 5;
        other2 = Shift::max<T>(other2, other);
        index2 = (other2 == other) ? index : index2;
        other = Shift::max<T>(other2, other3);
        index = (other == other2) ? index2 : index3;
        return BaseDef(other);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD6<T, Width>::BaseDef SIMD6<T, Width>::hminIndex(uint32& index) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other = Shift::min<T>(m_data.m_values[0], m_data.m_values[1]);
        index = (other == m_data.m_values[0]) ? 0 : 1;
        T other2 = Shift::min<T>(m_data.m_values[2], m_data.m_values[3]);
        uint32 index2 = (other2 == m_data.m_values[2]) ? 2 : 3;
        const T other3 = Shift::min<T>(m_data.m_values[4], m_data.m_values[5]);
        const uint32 index3 = (other3 == m_data.m_values[4]) ? 4 : 5;
        other2 = Shift::min<T>(other2, other);
        index2 = (other2 == other) ? index : index2;
        other = Shift::min<T>(other2, other3);
        index = (other == other2) ? index2 : index3;
        return BaseDef(other);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD6<T, Width>::hmax2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::max<T>(m_data.m_values[0], m_data.m_values[1]),
            Shift::max<T>(m_data.m_values[2], m_data.m_values[3]),
            Shift::max<T>(m_data.m_values[4], m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD6<T, Width>::hmin2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::min<T>(m_data.m_values[0], m_data.m_values[1]),
            Shift::min<T>(m_data.m_values[2], m_data.m_values[3]),
            Shift::min<T>(m_data.m_values[4], m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD6<T, Width>::hmax3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T other1 = Shift::max<T>(m_data.m_values[0], m_data.m_values[2]);
        const T other2 = Shift::max<T>(m_data.m_values[1], m_data.m_values[3]);
        return SIMD2(Shift::max<T>(m_data.m_values[4], other1), Shift::max<T>(m_data.m_values[5], other2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD6<T, Width>::hmin3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T other1 = Shift::min<T>(m_data.m_values[0], m_data.m_values[2]);
        const T other2 = Shift::min<T>(m_data.m_values[1], m_data.m_values[3]);
        return SIMD2(Shift::min<T>(m_data.m_values[4], other1), Shift::min<T>(m_data.m_values[5], other2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::reciprocal() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::recip<T>(m_data.m_values[0]), Shift::recip<T>(m_data.m_values[1]),
            Shift::recip<T>(m_data.m_values[2]), Shift::recip<T>(m_data.m_values[3]),
            Shift::recip<T>(m_data.m_values[4]), Shift::recip<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD6<T, Width>::add2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T res1 = (m_data.m_values[0] + m_data.m_values[2]);
        T res2 = (m_data.m_values[1] + m_data.m_values[3]);
        res1 += m_data.m_values[4];
        res2 += m_data.m_values[5];
        return SIMD2<T, Width>(res1, res2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD6<T, Width>::add3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T res1 = (m_data.m_values[0] + m_data.m_values[1]);
        const T res2 = (m_data.m_values[2] + m_data.m_values[3]);
        const T res3 = (m_data.m_values[4] + m_data.m_values[5]);
        return SIMD3<T, Width>(res1, res2, res3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD6<T, Width>::sub3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T res1 = (m_data.m_values[0] - m_data.m_values[1]);
        const T res2 = (m_data.m_values[2] - m_data.m_values[3]);
        const T res3 = (m_data.m_values[4] - m_data.m_values[5]);
        return SIMD3<T, Width>(res1, res2, res3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD6<T, Width>::BaseDef SIMD6<T, Width>::hadd() const noexcept
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
        res += res2;
        res += res3;
        return BaseDef(res);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD6<T, Width>::haddInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width>(hadd());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD6<T, Width>::hadd2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>(m_data.m_values[0] + m_data.m_values[1], m_data.m_values[2] + m_data.m_values[3],
            m_data.m_values[4] + m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD6<T, Width>::hadd3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T res = (m_data.m_values[0] + m_data.m_values[2]);
        T res2 = (m_data.m_values[1] + m_data.m_values[3]);
        res += m_data.m_values[4];
        res2 += m_data.m_values[5];
        return SIMD2<T, Width>(res, res2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD6<T, Width>::hsub2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>(m_data.m_values[0] - m_data.m_values[1], m_data.m_values[2] - m_data.m_values[3],
            m_data.m_values[4] - m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::ceil<T>(m_data.m_values[0]), Shift::ceil<T>(m_data.m_values[1]),
            Shift::ceil<T>(m_data.m_values[2]), Shift::ceil<T>(m_data.m_values[3]), Shift::ceil<T>(m_data.m_values[4]),
            Shift::ceil<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::floor<T>(m_data.m_values[0]), Shift::floor<T>(m_data.m_values[1]),
            Shift::floor<T>(m_data.m_values[2]), Shift::floor<T>(m_data.m_values[3]),
            Shift::floor<T>(m_data.m_values[4]), Shift::floor<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::trunc<T>(m_data.m_values[0]), Shift::trunc<T>(m_data.m_values[1]),
            Shift::trunc<T>(m_data.m_values[2]), Shift::trunc<T>(m_data.m_values[3]),
            Shift::trunc<T>(m_data.m_values[4]), Shift::trunc<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::sqrt<T>(m_data.m_values[0]), Shift::sqrt<T>(m_data.m_values[1]),
            Shift::sqrt<T>(m_data.m_values[2]), Shift::sqrt<T>(m_data.m_values[3]), Shift::sqrt<T>(m_data.m_values[4]),
            Shift::sqrt<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::rsqrt<T>(m_data.m_values[0]), Shift::rsqrt<T>(m_data.m_values[1]),
            Shift::rsqrt<T>(m_data.m_values[2]), Shift::rsqrt<T>(m_data.m_values[3]),
            Shift::rsqrt<T>(m_data.m_values[4]), Shift::rsqrt<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD6<T, Width>::dot3(const SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T sq0 = m_data.m_values[0] * other.m_data.m_values[0];
        const T sq1 = m_data.m_values[1] * other.m_data.m_values[1];
        const T sq2 = m_data.m_values[2] * other.m_data.m_values[2];
        const T sq3 = m_data.m_values[3] * other.m_data.m_values[3];
        const T sq4 = m_data.m_values[4] * other.m_data.m_values[4];
        const T sq5 = m_data.m_values[5] * other.m_data.m_values[5];
        T res = (sq0 + sq2);
        T res2 = (sq1 + sq3);
        res += sq4;
        res2 += sq5;
        return SIMD2(res, res2);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::cross3(const SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T crossX0 = -(m_data.m_values[4] * other.m_data.m_values[2]);
        const T crossX1 = -(m_data.m_values[5] * other.m_data.m_values[3]);
        const T crossY0 = -(m_data.m_values[0] * other.m_data.m_values[4]);
        const T crossY1 = -(m_data.m_values[1] * other.m_data.m_values[5]);
        const T crossZ0 = -(m_data.m_values[2] * other.m_data.m_values[0]);
        const T crossZ1 = -(m_data.m_values[3] * other.m_data.m_values[1]);
        return SIMD6(Shift::fma<T>(m_data.m_values[2], other.m_data.m_values[4], crossX0),
            Shift::fma<T>(m_data.m_values[3], other.m_data.m_values[5], crossX1),
            Shift::fma<T>(m_data.m_values[4], other.m_data.m_values[0], crossY0),
            Shift::fma<T>(m_data.m_values[5], other.m_data.m_values[1], crossY1),
            Shift::fma<T>(m_data.m_values[0], other.m_data.m_values[2], crossZ0),
            Shift::fma<T>(m_data.m_values[1], other.m_data.m_values[3], crossZ1));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD6<T, Width>::lengthSqr3() const noexcept
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
XS_INLINE SIMD2<T, Width> SIMD6<T, Width>::length3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const auto res = lengthSqr3().sqrt();
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::normalize3() const noexcept
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
            return *this * length3().rsqrt();
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::exp2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::exp2<T>(m_data.m_values[0]), Shift::exp2<T>(m_data.m_values[1]),
            Shift::exp2<T>(m_data.m_values[2]), Shift::exp2<T>(m_data.m_values[3]), Shift::exp2<T>(m_data.m_values[4]),
            Shift::exp2<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::exp() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::exp<T>(m_data.m_values[0]), Shift::exp<T>(m_data.m_values[1]),
            Shift::exp<T>(m_data.m_values[2]), Shift::exp<T>(m_data.m_values[3]), Shift::exp<T>(m_data.m_values[4]),
            Shift::exp<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::log2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::log2<T>(m_data.m_values[0]), Shift::log2<T>(m_data.m_values[1]),
            Shift::log2<T>(m_data.m_values[2]), Shift::log2<T>(m_data.m_values[3]), Shift::log2<T>(m_data.m_values[4]),
            Shift::log2<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::log() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::log<T>(m_data.m_values[0]), Shift::log<T>(m_data.m_values[1]),
            Shift::log<T>(m_data.m_values[2]), Shift::log<T>(m_data.m_values[3]), Shift::log<T>(m_data.m_values[4]),
            Shift::log<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::pow(const SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::pow<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::pow<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::pow<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::pow<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::pow<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::pow<T>(m_data.m_values[5], other.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::powr(const SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::powr<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::powr<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::powr<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::powr<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::powr<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::powr<T>(m_data.m_values[5], other.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::pow(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::pow<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[3], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[4], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[5], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::powr(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::powr<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[3], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[4], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[5], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::sin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::sin<T>(m_data.m_values[0]), Shift::sin<T>(m_data.m_values[1]),
            Shift::sin<T>(m_data.m_values[2]), Shift::sin<T>(m_data.m_values[3]), Shift::sin<T>(m_data.m_values[4]),
            Shift::sin<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::cos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::cos<T>(m_data.m_values[0]), Shift::cos<T>(m_data.m_values[1]),
            Shift::cos<T>(m_data.m_values[2]), Shift::cos<T>(m_data.m_values[3]), Shift::cos<T>(m_data.m_values[4]),
            Shift::cos<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::tan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::tan<T>(m_data.m_values[0]), Shift::tan<T>(m_data.m_values[1]),
            Shift::tan<T>(m_data.m_values[2]), Shift::tan<T>(m_data.m_values[3]), Shift::tan<T>(m_data.m_values[4]),
            Shift::tan<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> SIMD6<T, Width>::sincos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        SIMD12<T, Width> ret;
        ret.m_data.m_values[0] = Shift::sincos<T>(m_data.m_values[0], ret.m_data.m_values[2]);
        ret.m_data.m_values[1] = Shift::sincos<T>(m_data.m_values[1], ret.m_data.m_values[3]);
        ret.m_data.m_values[4] = Shift::sincos<T>(m_data.m_values[2], ret.m_data.m_values[6]);
        ret.m_data.m_values[5] = Shift::sincos<T>(m_data.m_values[3], ret.m_data.m_values[7]);
        ret.m_data.m_values[8] = Shift::sincos<T>(m_data.m_values[4], ret.m_data.m_values[10]);
        ret.m_data.m_values[9] = Shift::sincos<T>(m_data.m_values[5], ret.m_data.m_values[11]);
        return ret;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::asin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::asin<T>(m_data.m_values[0]), Shift::asin<T>(m_data.m_values[1]),
            Shift::asin<T>(m_data.m_values[2]), Shift::asin<T>(m_data.m_values[3]), Shift::asin<T>(m_data.m_values[4]),
            Shift::asin<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::acos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::acos<T>(m_data.m_values[0]), Shift::acos<T>(m_data.m_values[1]),
            Shift::acos<T>(m_data.m_values[2]), Shift::acos<T>(m_data.m_values[3]), Shift::acos<T>(m_data.m_values[4]),
            Shift::acos<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::atan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::atan<T>(m_data.m_values[0]), Shift::atan<T>(m_data.m_values[1]),
            Shift::atan<T>(m_data.m_values[2]), Shift::atan<T>(m_data.m_values[3]), Shift::atan<T>(m_data.m_values[4]),
            Shift::atan<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::atan2(const SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Shift::atan2<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::atan2<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::atan2<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::atan2<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::atan2<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::atan2<T>(m_data.m_values[5], other.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::insert(const SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
    static_assert(Index0 < 6 && Index1 < 6);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index0] == other.m_data.m_values[Index1];
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::insert2(const SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
    static_assert(Index0 < 3 && Index1 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index0 * 2] == other.m_data.m_values[Index1 * 2];
        m_data.m_values[Index0 * 2 + 1] == other.m_data.m_values[Index1 * 2 + 1];
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::insert3(const SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
    static_assert(Index0 < 2 && Index1 < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index0] == other.m_data.m_values[Index1];
        m_data.m_values[Index0 + 2] == other.m_data.m_values[Index1 + 2];
        m_data.m_values[Index0 + 4] == other.m_data.m_values[Index1 + 4];
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::blend(const SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(Elem0 ? other.m_data.m_values[0] : m_data.m_values[0],
            Elem1 ? other.m_data.m_values[1] : m_data.m_values[1],
            Elem2 ? other.m_data.m_values[2] : m_data.m_values[2],
            Elem3 ? other.m_data.m_values[3] : m_data.m_values[3],
            Elem4 ? other.m_data.m_values[4] : m_data.m_values[4],
            Elem5 ? other.m_data.m_values[5] : m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::blendVar(const SIMD6<T, Width2>& other, const Mask& mask) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6((mask.m_data.m_values[0]) ? other.m_data.m_values[0] : m_data.m_values[0],
            (mask.m_data.m_values[1]) ? other.m_data.m_values[1] : m_data.m_values[1],
            (mask.m_data.m_values[2]) ? other.m_data.m_values[2] : m_data.m_values[2],
            (mask.m_data.m_values[3]) ? other.m_data.m_values[3] : m_data.m_values[3],
            (mask.m_data.m_values[4]) ? other.m_data.m_values[4] : m_data.m_values[4],
            (mask.m_data.m_values[5]) ? other.m_data.m_values[5] : m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::blendSwap(SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const SIMD6 backup = other;
        other.m_data.m_values[0] = (Elem0) ? m_data.m_values[0] : other.m_data.m_values[0];
        other.m_data.m_values[1] = (Elem1) ? m_data.m_values[1] : other.m_data.m_values[1];
        other.m_data.m_values[2] = (Elem2) ? m_data.m_values[2] : other.m_data.m_values[2];
        other.m_data.m_values[3] = (Elem3) ? m_data.m_values[3] : other.m_data.m_values[3];
        other.m_data.m_values[4] = (Elem4) ? m_data.m_values[4] : other.m_data.m_values[4];
        other.m_data.m_values[5] = (Elem5) ? m_data.m_values[5] : other.m_data.m_values[5];
        return SIMD6((Elem0) ? backup.m_data.m_values[0] : m_data.m_values[0],
            (Elem1) ? backup.m_data.m_values[1] : m_data.m_values[1],
            (Elem2) ? backup.m_data.m_values[2] : m_data.m_values[2],
            (Elem3) ? backup.m_data.m_values[3] : m_data.m_values[3],
            (Elem4) ? backup.m_data.m_values[4] : m_data.m_values[4],
            (Elem5) ? backup.m_data.m_values[5] : m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::blendSwapVar(SIMD6<T, Width2>& other, const Mask& mask) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const SIMD6 backup = other;
        other.m_data.m_values[0] = (mask.m_data.m_values[0]) ? m_data.m_values[0] : other.m_data.m_values[0];
        other.m_data.m_values[1] = (mask.m_data.m_values[1]) ? m_data.m_values[1] : other.m_data.m_values[1];
        other.m_data.m_values[2] = (mask.m_data.m_values[2]) ? m_data.m_values[2] : other.m_data.m_values[2];
        other.m_data.m_values[3] = (mask.m_data.m_values[3]) ? m_data.m_values[3] : other.m_data.m_values[3];
        other.m_data.m_values[4] = (mask.m_data.m_values[4]) ? m_data.m_values[4] : other.m_data.m_values[4];
        other.m_data.m_values[5] = (mask.m_data.m_values[5]) ? m_data.m_values[5] : other.m_data.m_values[5];
        return SIMD6((mask.m_data.m_values[0]) ? backup.m_data.m_values[0] : m_data.m_values[0],
            (mask.m_data.m_values[1]) ? backup.m_data.m_values[1] : m_data.m_values[1],
            (mask.m_data.m_values[2]) ? backup.m_data.m_values[2] : m_data.m_values[2],
            (mask.m_data.m_values[3]) ? backup.m_data.m_values[3] : m_data.m_values[3],
            (mask.m_data.m_values[4]) ? backup.m_data.m_values[4] : m_data.m_values[4],
            (mask.m_data.m_values[5]) ? backup.m_data.m_values[5] : m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::blend2(const SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const SIMD3 temp0 = (Elem0) ?
            SIMD3(other.m_data.m_values[0], other.m_data.m_values[2], other.m_data.m_values[4]) :
            SIMD3(m_data.m_values[0], m_data.m_values[2], m_data.m_values[4]);
        const SIMD3 temp1 = (Elem1) ?
            SIMD3(other.m_data.m_values[1], other.m_data.m_values[3], other.m_data.m_values[5]) :
            SIMD3(m_data.m_values[1], m_data.m_values[3], m_data.m_values[5]);
        return SIMD6(temp0.m_data.m_values[0], temp1.m_data.m_values[0], temp0.m_data.m_values[1],
            temp1.m_data.m_values[1], temp0.m_data.m_values[2], temp1.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::blend3(const SIMD6<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const SIMD2 temp0 = (Elem0) ? SIMD2(other.m_data.m_values[0], other.m_data.m_values[1]) :
                                      SIMD2(m_data.m_values[0], m_data.m_values[1]);
        const SIMD2 temp1 = (Elem1) ? SIMD2(other.m_data.m_values[2], other.m_data.m_values[3]) :
                                      SIMD2(m_data.m_values[2], m_data.m_values[3]);
        const SIMD2 temp2 = (Elem2) ? SIMD2(other.m_data.m_values[4], other.m_data.m_values[5]) :
                                      SIMD2(m_data.m_values[4], m_data.m_values[5]);
        return SIMD6(temp0.m_data.m_values[0], temp0.m_data.m_values[1], temp1.m_data.m_values[0],
            temp1.m_data.m_values[1], temp2.m_data.m_values[0], temp2.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3, uint32 Index4, uint32 Index5>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::shuffle() const noexcept
{
    static_assert(Index0 < 6 && Index1 < 6 && Index2 < 6 && Index3 < 6 && Index4 < 6 && Index5 < 6);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(m_data.m_values[Index0], m_data.m_values[Index1], m_data.m_values[Index2], m_data.m_values[Index3],
            m_data.m_values[Index4], m_data.m_values[Index5]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::shuffleVar(const Shuffle& shuffle) const noexcept
{
    XS_ASSERT(shuffle.m_data.m_values[0] < 6 && shuffle.m_data.m_values[1] < 6 && shuffle.m_data.m_values[2] < 6 &&
        shuffle.m_data.m_values[3] < 6 && shuffle.m_data.m_values[4] < 6 && shuffle.m_data.m_values[5] < 6);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(m_data.m_values[shuffle.m_data.m_values[0]], m_data.m_values[shuffle.m_data.m_values[1]],
            m_data.m_values[shuffle.m_data.m_values[2]], m_data.m_values[shuffle.m_data.m_values[3]],
            m_data.m_values[shuffle.m_data.m_values[4]], m_data.m_values[shuffle.m_data.m_values[5]]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::shuffle2() const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(m_data.m_values[Index0], m_data.m_values[Index1], m_data.m_values[Index0 + 2],
            m_data.m_values[Index1 + 2], m_data.m_values[Index0 + 4], m_data.m_values[Index1 + 4]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE SIMD6<T, Width> SIMD6<T, Width>::shuffle3() const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6(m_data.m_values[Index0 * 2], m_data.m_values[Index0 * 2 + 1], m_data.m_values[Index1 * 2],
            m_data.m_values[Index1 * 2 + 1], m_data.m_values[Index2 * 2], m_data.m_values[Index2 * 2 + 1]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD8<T, Width> SIMD6<T, Width>::shuffle3() const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3 && Index3 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(m_data.m_values[Index0 * 2], m_data.m_values[Index0 * 2 + 1],
            m_data.m_values[Index1 * 2], m_data.m_values[Index1 * 2 + 1], m_data.m_values[Index2 * 2],
            m_data.m_values[Index2 * 2 + 1], m_data.m_values[Index3 * 2], m_data.m_values[Index3 * 2 + 1]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator+(const SIMD6<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
            other1.m_data.m_values[1] + other2.m_data.m_values[1],
            other1.m_data.m_values[2] + other2.m_data.m_values[2],
            other1.m_data.m_values[3] + other2.m_data.m_values[3],
            other1.m_data.m_values[4] + other2.m_data.m_values[4],
            other1.m_data.m_values[5] + other2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator+(const SIMD6<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD6<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_value,
            other1.m_data.m_values[1] + other2.m_data.m_value, other1.m_data.m_values[2] + other2.m_data.m_value,
            other1.m_data.m_values[3] + other2.m_data.m_value, other1.m_data.m_values[4] + other2.m_data.m_value,
            other1.m_data.m_values[5] + other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator+(const SIMD6<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
            other1.m_data.m_values[1] + other2.m_data.m_values[1],
            other1.m_data.m_values[2] + other2.m_data.m_values[0],
            other1.m_data.m_values[3] + other2.m_data.m_values[1],
            other1.m_data.m_values[4] + other2.m_data.m_values[0],
            other1.m_data.m_values[5] + other2.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator+(const SIMD6<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
            other1.m_data.m_values[1] + other2.m_data.m_values[0],
            other1.m_data.m_values[2] + other2.m_data.m_values[1],
            other1.m_data.m_values[3] + other2.m_data.m_values[1],
            other1.m_data.m_values[4] + other2.m_data.m_values[2],
            other1.m_data.m_values[5] + other2.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator-(const SIMD6<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
            other1.m_data.m_values[1] - other2.m_data.m_values[1],
            other1.m_data.m_values[2] - other2.m_data.m_values[2],
            other1.m_data.m_values[3] - other2.m_data.m_values[3],
            other1.m_data.m_values[4] - other2.m_data.m_values[4],
            other1.m_data.m_values[5] - other2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator-(const SIMD6<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD6<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_value,
            other1.m_data.m_values[1] - other2.m_data.m_value, other1.m_data.m_values[2] - other2.m_data.m_value,
            other1.m_data.m_values[3] - other2.m_data.m_value, other1.m_data.m_values[4] - other2.m_data.m_value,
            other1.m_data.m_values[5] - other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator-(const SIMDBase<T, Width2>& other1, const SIMD6<T, Width>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD6<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(other1.m_data.m_value - other2.m_data.m_values[0],
            other1.m_data.m_value - other2.m_data.m_values[1], other1.m_data.m_value - other2.m_data.m_values[2],
            other1.m_data.m_value - other2.m_data.m_values[3], other1.m_data.m_value - other2.m_data.m_values[4],
            other1.m_data.m_value - other2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator-(const SIMD6<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
            other1.m_data.m_values[1] - other2.m_data.m_values[1],
            other1.m_data.m_values[2] - other2.m_data.m_values[0],
            other1.m_data.m_values[3] - other2.m_data.m_values[1],
            other1.m_data.m_values[4] - other2.m_data.m_values[0],
            other1.m_data.m_values[5] - other2.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator-(const SIMD6<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
            other1.m_data.m_values[1] - other2.m_data.m_values[0],
            other1.m_data.m_values[2] - other2.m_data.m_values[1],
            other1.m_data.m_values[3] - other2.m_data.m_values[1],
            other1.m_data.m_values[4] - other2.m_data.m_values[2],
            other1.m_data.m_values[5] - other2.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator*(const SIMD6<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
            other1.m_data.m_values[1] * other2.m_data.m_values[1],
            other1.m_data.m_values[2] * other2.m_data.m_values[2],
            other1.m_data.m_values[3] * other2.m_data.m_values[3],
            other1.m_data.m_values[4] * other2.m_data.m_values[4],
            other1.m_data.m_values[5] * other2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator*(const SIMD6<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD6<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_value,
            other1.m_data.m_values[1] * other2.m_data.m_value, other1.m_data.m_values[2] * other2.m_data.m_value,
            other1.m_data.m_values[3] * other2.m_data.m_value, other1.m_data.m_values[4] * other2.m_data.m_value,
            other1.m_data.m_values[5] * other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator*(const SIMD6<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
            other1.m_data.m_values[1] * other2.m_data.m_values[1],
            other1.m_data.m_values[2] * other2.m_data.m_values[0],
            other1.m_data.m_values[3] * other2.m_data.m_values[1],
            other1.m_data.m_values[4] * other2.m_data.m_values[0],
            other1.m_data.m_values[5] * other2.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator*(const SIMD6<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
            other1.m_data.m_values[1] * other2.m_data.m_values[0],
            other1.m_data.m_values[2] * other2.m_data.m_values[1],
            other1.m_data.m_values[3] * other2.m_data.m_values[1],
            other1.m_data.m_values[4] * other2.m_data.m_values[2],
            other1.m_data.m_values[5] * other2.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator/(const SIMD6<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
            other1.m_data.m_values[1] / other2.m_data.m_values[1],
            other1.m_data.m_values[2] / other2.m_data.m_values[2],
            other1.m_data.m_values[3] / other2.m_data.m_values[3],
            other1.m_data.m_values[4] / other2.m_data.m_values[4],
            other1.m_data.m_values[5] / other2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator/(const SIMD6<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD6<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD6<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_value,
                other1.m_data.m_values[1] / other2.m_data.m_value, other1.m_data.m_values[2] / other2.m_data.m_value,
                other1.m_data.m_values[3] / other2.m_data.m_value, other1.m_data.m_values[4] / other2.m_data.m_value,
                other1.m_data.m_values[5] / other2.m_data.m_value);
        } else {
            const T divisor = recip<T>(other2.m_data.m_value);
            return SIMD6<T, Width>(other1.m_data.m_values[0] * divisor, other1.m_data.m_values[1] * divisor,
                other1.m_data.m_values[2] * divisor, other1.m_data.m_values[3] * divisor,
                other1.m_data.m_values[4] * divisor, other1.m_data.m_values[5] * divisor);
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator/(const SIMDBase<T, Width2>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD6<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(other1.m_data.m_value / other2.m_data.m_values[0],
            other1.m_data.m_value / other2.m_data.m_values[1], other1.m_data.m_value / other2.m_data.m_values[2],
            other1.m_data.m_value / other2.m_data.m_values[3], other1.m_data.m_value / other2.m_data.m_values[4],
            other1.m_data.m_value / other2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator/(const SIMD6<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD6<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
                other1.m_data.m_values[1] / other2.m_data.m_values[1],
                other1.m_data.m_values[2] / other2.m_data.m_values[0],
                other1.m_data.m_values[3] / other2.m_data.m_values[1],
                other1.m_data.m_values[4] / other2.m_data.m_values[0],
                other1.m_data.m_values[5] / other2.m_data.m_values[1]);
        } else {
            const T divisor0 = recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = recip<T>(other2.m_data.m_values[1]);
            return SIMD6<T, Width>(other1.m_data.m_values[0] * divisor0, other1.m_data.m_values[1] * divisor1,
                other1.m_data.m_values[2] * divisor0, other1.m_data.m_values[3] * divisor1,
                other1.m_data.m_values[4] * divisor0, other1.m_data.m_values[5] * divisor1);
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator/(const SIMD6<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD6<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
                other1.m_data.m_values[1] / other2.m_data.m_values[0],
                other1.m_data.m_values[2] / other2.m_data.m_values[1],
                other1.m_data.m_values[3] / other2.m_data.m_values[1],
                other1.m_data.m_values[4] / other2.m_data.m_values[2],
                other1.m_data.m_values[5] / other2.m_data.m_values[2]);
        } else {
            const T divisor0 = recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = recip<T>(other2.m_data.m_values[1]);
            const T divisor2 = recip<T>(other2.m_data.m_values[2]);
            return SIMD6<T, Width>(other1.m_data.m_values[0] * divisor0, other1.m_data.m_values[1] * divisor0,
                other1.m_data.m_values[2] * divisor1, other1.m_data.m_values[3] * divisor1,
                other1.m_data.m_values[4] * divisor2, other1.m_data.m_values[5] * divisor2);
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> operator-(const SIMD6<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>(-other.m_data.m_values[0], -other.m_data.m_values[1], -other.m_data.m_values[2],
            -other.m_data.m_values[3], -other.m_data.m_values[4], -other.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>& operator+=(SIMD6<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
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
        other1.m_data.m_values[2] += other2.m_data.m_values[2];
        other1.m_data.m_values[3] += other2.m_data.m_values[3];
        other1.m_data.m_values[4] += other2.m_data.m_values[4];
        other1.m_data.m_values[5] += other2.m_data.m_values[5];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>& operator+=(SIMD6<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD6<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>& operator+=(SIMD6<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>& operator+=(SIMD6<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
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
        other1.m_data.m_values[2] += other2.m_data.m_values[1];
        other1.m_data.m_values[3] += other2.m_data.m_values[1];
        other1.m_data.m_values[4] += other2.m_data.m_values[2];
        other1.m_data.m_values[5] += other2.m_data.m_values[2];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>& operator-=(SIMD6<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
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
        other1.m_data.m_values[2] -= other2.m_data.m_values[2];
        other1.m_data.m_values[3] -= other2.m_data.m_values[3];
        other1.m_data.m_values[4] -= other2.m_data.m_values[4];
        other1.m_data.m_values[5] -= other2.m_data.m_values[5];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>& operator-=(SIMD6<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD6<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>& operator-=(SIMD6<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>& operator-=(SIMD6<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
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
        other1.m_data.m_values[2] -= other2.m_data.m_values[1];
        other1.m_data.m_values[3] -= other2.m_data.m_values[1];
        other1.m_data.m_values[4] -= other2.m_data.m_values[2];
        other1.m_data.m_values[5] -= other2.m_data.m_values[2];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>& operator*=(SIMD6<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
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
        other1.m_data.m_values[2] *= other2.m_data.m_values[2];
        other1.m_data.m_values[3] *= other2.m_data.m_values[3];
        other1.m_data.m_values[4] *= other2.m_data.m_values[4];
        other1.m_data.m_values[5] *= other2.m_data.m_values[5];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>& operator*=(SIMD6<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD6<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>& operator*=(SIMD6<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>& operator*=(SIMD6<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
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
        other1.m_data.m_values[2] *= other2.m_data.m_values[1];
        other1.m_data.m_values[3] *= other2.m_data.m_values[1];
        other1.m_data.m_values[4] *= other2.m_data.m_values[2];
        other1.m_data.m_values[5] *= other2.m_data.m_values[2];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>& operator/=(SIMD6<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>& operator/=(SIMD6<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD6<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
        } else {
            const T divisor = Shift::recip<T>(other2.m_data.m_value);
            other1.m_data.m_values[0] *= divisor;
            other1.m_data.m_values[1] *= divisor;
            other1.m_data.m_values[2] *= divisor;
            other1.m_data.m_values[3] *= divisor;
            other1.m_data.m_values[4] *= divisor;
            other1.m_data.m_values[5] *= divisor;
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>& operator/=(SIMD6<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
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
        } else {
            const T divisor0 = Shift::recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = Shift::recip<T>(other2.m_data.m_values[1]);
            other1.m_data.m_values[0] *= divisor0;
            other1.m_data.m_values[1] *= divisor1;
            other1.m_data.m_values[2] *= divisor0;
            other1.m_data.m_values[3] *= divisor1;
            other1.m_data.m_values[4] *= divisor0;
            other1.m_data.m_values[5] *= divisor1;
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width>& operator/=(SIMD6<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
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
            other1.m_data.m_values[2] /= other2.m_data.m_values[1];
            other1.m_data.m_values[3] /= other2.m_data.m_values[1];
            other1.m_data.m_values[4] /= other2.m_data.m_values[2];
            other1.m_data.m_values[5] /= other2.m_data.m_values[2];
        } else {
            const T divisor0 = Shift::recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = Shift::recip<T>(other2.m_data.m_values[1]);
            const T divisor2 = Shift::recip<T>(other2.m_data.m_values[2]);
            other1.m_data.m_values[0] *= divisor0;
            other1.m_data.m_values[1] *= divisor0;
            other1.m_data.m_values[2] *= divisor1;
            other1.m_data.m_values[3] *= divisor1;
            other1.m_data.m_values[4] *= divisor2;
            other1.m_data.m_values[5] *= divisor2;
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator==(const SIMD6<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] == other2.m_data.m_values[0]) &
            (other1.m_data.m_values[1] == other2.m_data.m_values[1]) &
            (other1.m_data.m_values[2] == other2.m_data.m_values[2]) &
            (other1.m_data.m_values[3] == other2.m_data.m_values[3]) &
            (other1.m_data.m_values[4] == other2.m_data.m_values[4]) &
            (other1.m_data.m_values[5] == other2.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<=(const SIMD6<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] <= other2.m_data.m_values[0]) &
            (other1.m_data.m_values[1] <= other2.m_data.m_values[1]) &
            (other1.m_data.m_values[2] <= other2.m_data.m_values[2]) &
            (other1.m_data.m_values[3] <= other2.m_data.m_values[3]) &
            (other1.m_data.m_values[4] <= other2.m_data.m_values[4]) &
            (other1.m_data.m_values[5] <= other2.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<(const SIMD6<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] < other2.m_data.m_values[0]) &
            (other1.m_data.m_values[1] < other2.m_data.m_values[1]) &
            (other1.m_data.m_values[2] < other2.m_data.m_values[2]) &
            (other1.m_data.m_values[3] < other2.m_data.m_values[3]) &
            (other1.m_data.m_values[4] < other2.m_data.m_values[4]) &
            (other1.m_data.m_values[5] < other2.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator!=(const SIMD6<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] != other2.m_data.m_values[0]) &
            (other1.m_data.m_values[1] != other2.m_data.m_values[1]) &
            (other1.m_data.m_values[2] != other2.m_data.m_values[2]) &
            (other1.m_data.m_values[3] != other2.m_data.m_values[3]) &
            (other1.m_data.m_values[4] != other2.m_data.m_values[4]) &
            (other1.m_data.m_values[5] != other2.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator==(const SIMD6<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD6<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] == other2.m_data.m_value) &
            (other1.m_data.m_values[1] == other2.m_data.m_value) &
            (other1.m_data.m_values[2] == other2.m_data.m_value) &
            (other1.m_data.m_values[3] == other2.m_data.m_value) &
            (other1.m_data.m_values[4] == other2.m_data.m_value) &
            (other1.m_data.m_values[5] == other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<=(const SIMD6<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD6<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] <= other2.m_data.m_value) &
            (other1.m_data.m_values[1] <= other2.m_data.m_value) &
            (other1.m_data.m_values[2] <= other2.m_data.m_value) &
            (other1.m_data.m_values[3] <= other2.m_data.m_value) &
            (other1.m_data.m_values[4] <= other2.m_data.m_value) &
            (other1.m_data.m_values[5] <= other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<(const SIMD6<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD6<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] < other2.m_data.m_value) &
            (other1.m_data.m_values[1] < other2.m_data.m_value) & (other1.m_data.m_values[2] < other2.m_data.m_value) &
            (other1.m_data.m_values[3] < other2.m_data.m_value) & (other1.m_data.m_values[4] < other2.m_data.m_value) &
            (other1.m_data.m_values[5] < other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator!=(const SIMD6<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD6<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] != other2.m_data.m_value) &
            (other1.m_data.m_values[1] != other2.m_data.m_value) &
            (other1.m_data.m_values[2] != other2.m_data.m_value) &
            (other1.m_data.m_values[3] != other2.m_data.m_value) &
            (other1.m_data.m_values[4] != other2.m_data.m_value) &
            (other1.m_data.m_values[5] != other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator&(const SIMD6<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD6<T, Width>(other1.m_data.m_values[0] & other2.m_data.m_values[0],
                other1.m_data.m_values[1] & other2.m_data.m_values[1],
                other1.m_data.m_values[2] & other2.m_data.m_values[2],
                other1.m_data.m_values[3] & other2.m_data.m_values[3],
                other1.m_data.m_values[4] & other2.m_data.m_values[4],
                other1.m_data.m_values[5] & other2.m_data.m_values[5]);
        } else {
            return SIMD6<T, Width>(Shift::bitAnd(other1.m_data.m_values[0], other2.m_data.m_values[0]),
                Shift::bitAnd(other1.m_data.m_values[1], other2.m_data.m_values[1]),
                Shift::bitAnd(other1.m_data.m_values[2], other2.m_data.m_values[2]),
                Shift::bitAnd(other1.m_data.m_values[3], other2.m_data.m_values[3]),
                Shift::bitAnd(other1.m_data.m_values[4], other2.m_data.m_values[4]),
                Shift::bitAnd(other1.m_data.m_values[5], other2.m_data.m_values[5]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator&(const SIMD6<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD6<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD6<T, Width>(other1.m_data.m_values[0] & other2.m_data.m_value,
                other1.m_data.m_values[1] & other2.m_data.m_value, other1.m_data.m_values[2] & other2.m_data.m_value,
                other1.m_data.m_values[3] & other2.m_data.m_value, other1.m_data.m_values[4] & other2.m_data.m_value,
                other1.m_data.m_values[5] & other2.m_data.m_value);
        } else {
            return SIMD6<T, Width>(Shift::bitAnd(other1.m_data.m_values[0], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[1], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[2], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[3], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[4], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[5], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator|(const SIMD6<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD6<T, Width>(other1.m_data.m_values[0] | other2.m_data.m_values[0],
                other1.m_data.m_values[1] | other2.m_data.m_values[1],
                other1.m_data.m_values[2] | other2.m_data.m_values[2],
                other1.m_data.m_values[3] | other2.m_data.m_values[3],
                other1.m_data.m_values[4] | other2.m_data.m_values[4],
                other1.m_data.m_values[5] | other2.m_data.m_values[5]);
        } else {
            return SIMD6<T, Width>(Shift::bitOr(other1.m_data.m_values[0], other2.m_data.m_values[0]),
                Shift::bitOr(other1.m_data.m_values[1], other2.m_data.m_values[1]),
                Shift::bitOr(other1.m_data.m_values[2], other2.m_data.m_values[2]),
                Shift::bitOr(other1.m_data.m_values[3], other2.m_data.m_values[3]),
                Shift::bitOr(other1.m_data.m_values[4], other2.m_data.m_values[4]),
                Shift::bitOr(other1.m_data.m_values[5], other2.m_data.m_values[5]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator|(const SIMD6<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD6<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD6<T, Width>(other1.m_data.m_values[0] | other2.m_data.m_value,
                other1.m_data.m_values[1] | other2.m_data.m_value, other1.m_data.m_values[2] | other2.m_data.m_value,
                other1.m_data.m_values[3] | other2.m_data.m_value, other1.m_data.m_values[4] | other2.m_data.m_value,
                other1.m_data.m_values[5] | other2.m_data.m_value);
        } else {
            return SIMD6<T, Width>(Shift::bitOr(other1.m_data.m_values[0], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[1], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[2], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[3], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[4], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[5], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator^(const SIMD6<T, Width>& other1, const SIMD6<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD6<T, Width>(other1.m_data.m_values[0] ^ other2.m_data.m_values[0],
                other1.m_data.m_values[1] ^ other2.m_data.m_values[1],
                other1.m_data.m_values[2] ^ other2.m_data.m_values[2],
                other1.m_data.m_values[3] ^ other2.m_data.m_values[3],
                other1.m_data.m_values[4] ^ other2.m_data.m_values[4],
                other1.m_data.m_values[5] ^ other2.m_data.m_values[5]);
        } else {
            return SIMD6<T, Width>(Shift::bitXor(other1.m_data.m_values[0], other2.m_data.m_values[0]),
                Shift::bitXor(other1.m_data.m_values[1], other2.m_data.m_values[1]),
                Shift::bitXor(other1.m_data.m_values[2], other2.m_data.m_values[2]),
                Shift::bitXor(other1.m_data.m_values[3], other2.m_data.m_values[3]),
                Shift::bitXor(other1.m_data.m_values[4], other2.m_data.m_values[4]),
                Shift::bitXor(other1.m_data.m_values[5], other2.m_data.m_values[5]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD6<T, Width> operator^(const SIMD6<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD6<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD6<T, Width>(other1.m_data.m_values[0] ^ other2.m_data.m_value,
                other1.m_data.m_values[1] ^ other2.m_data.m_value, other1.m_data.m_values[2] ^ other2.m_data.m_value,
                other1.m_data.m_values[3] ^ other2.m_data.m_value, other1.m_data.m_values[4] ^ other2.m_data.m_value,
                other1.m_data.m_values[5] ^ other2.m_data.m_value);
        } else {
            return SIMD6<T, Width>(Shift::bitXor(other1.m_data.m_values[0], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[1], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[2], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[3], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[4], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[5], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD6<T, Width> operator~(const SIMD6<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD6<T, Width>(~other.m_data.m_values[0], ~other.m_data.m_values[1], ~other.m_data.m_values[2],
                ~other.m_data.m_values[3], ~other.m_data.m_values[4], ~other.m_data.m_values[5]);
        } else {
            return SIMD6<T, Width>(Shift::bitNot(other.m_data.m_values[0]), Shift::bitNot(other.m_data.m_values[1]),
                Shift::bitNot(other.m_data.m_values[2]), Shift::bitNot(other.m_data.m_values[3]),
                Shift::bitNot(other.m_data.m_values[4]), Shift::bitNot(other.m_data.m_values[5]));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD6<T, Width>::Mask operator&(
    const typename SIMD6<T, Width>::Mask& mask1, const typename SIMD6<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>::Mask(mask1.m_data.m_values[0] & mask2.m_data.m_values[0],
            mask1.m_data.m_values[1] & mask2.m_data.m_values[1], mask1.m_data.m_values[2] & mask2.m_data.m_values[2],
            mask1.m_data.m_values[3] & mask2.m_data.m_values[3], mask1.m_data.m_values[4] & mask2.m_data.m_values[4],
            mask1.m_data.m_values[5] & mask2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD6<T, Width>::Mask operator|(
    const typename SIMD6<T, Width>::Mask& mask1, const typename SIMD6<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>::Mask(mask1.m_data.m_values[0] | mask2.m_data.m_values[0],
            mask1.m_data.m_values[1] | mask2.m_data.m_values[1], mask1.m_data.m_values[2] | mask2.m_data.m_values[2],
            mask1.m_data.m_values[3] | mask2.m_data.m_values[3], mask1.m_data.m_values[4] | mask2.m_data.m_values[4],
            mask1.m_data.m_values[5] | mask2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD6<T, Width>::Mask operator^(
    const typename SIMD6<T, Width>::Mask& mask1, const typename SIMD6<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>::Mask(mask1.m_data.m_values[0] ^ mask2.m_data.m_values[0],
            mask1.m_data.m_values[1] ^ mask2.m_data.m_values[1], mask1.m_data.m_values[2] ^ mask2.m_data.m_values[2],
            mask1.m_data.m_values[3] ^ mask2.m_data.m_values[3], mask1.m_data.m_values[4] ^ mask2.m_data.m_values[4],
            mask1.m_data.m_values[5] ^ mask2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD6<T, Width>::Mask operator~(const typename SIMD6<T, Width>::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD6<T, Width>::Mask(~mask.m_data.m_values[0], ~mask.m_data.m_values[1], ~mask.m_data.m_values[2],
            ~mask.m_data.m_values[3], ~mask.m_data.m_values[4], ~mask.m_data.m_values[5]);
    }
}
} // namespace Shift
