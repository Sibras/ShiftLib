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

#include "SIMD/XSSIMD8.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#endif
#include "XSMath.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD8Data<T>::SIMD8Data(const SIMD8<T, Width>& other) noexcept
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
    }
}

template<typename T>
XS_INLINE void SIMD8Data<T>::setData(const T value0, const T value1, const T value2, const T value3, const T value4,
    const T value5, const T value6, const T value7) noexcept
{
    m_value0 = value0;
    m_value1 = value1;
    m_value2 = value2;
    m_value3 = value3;
    m_value4 = value4;
    m_value5 = value5;
    m_value6 = value6;
    m_value7 = value7;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD8Data<T>::store(const SIMD8<T, Width>& other) noexcept
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
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8Data<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(m_value0, m_value1, m_value2, m_value3, m_value4, m_value5, m_value6, m_value7);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::Mask::Mask(const uint32 mask0, const uint32 mask1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = mask0 & 0x1;
        m_data.m_values[1] = mask0 & 0x2;
        m_data.m_values[2] = mask0 & 0x4;
        m_data.m_values[3] = mask0 & 0x8;
        m_data.m_values[4] = mask1 & 0x1;
        m_data.m_values[5] = mask1 & 0x2;
        m_data.m_values[6] = mask1 & 0x4;
        m_data.m_values[7] = mask1 & 0x8;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::Mask::Mask(const uint32 mask) noexcept
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
        m_data.m_values[6] = mask & 0x40;
        m_data.m_values[7] = mask & 0x80;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::Mask::Mask(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::Mask::Mask(const bool bool0, const bool bool1, const bool bool2, const bool bool3,
    const bool bool4, const bool bool5, const bool bool6, const bool bool7) noexcept
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
        m_data.m_values[6] = bool6;
        m_data.m_values[7] = bool7;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD8<T, Width>::Mask::getMask4() const noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>(m_data.m_values[Index * 2], m_data.m_values[Index * 2 + 1],
            m_data.m_values[Index * 2 + 2], m_data.m_values[Index * 2 + 3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE Bool8<hasSIMD<T> && (Width > SIMDWidth::Scalar)> SIMD8<T, Width>::Mask::getBool8() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Bool8(m_data.m_values[0], m_data.m_values[1], m_data.m_values[2], m_data.m_values[3], m_data.m_values[4],
            m_data.m_values[5], m_data.m_values[6], m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD8<T, Width>::Mask::getAny() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return ((m_data.m_values[0] | m_data.m_values[1]) | (m_data.m_values[2] | m_data.m_values[3])) |
            ((m_data.m_values[4] | m_data.m_values[5]) | (m_data.m_values[6] | m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD8<T, Width>::Mask::getAll() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return ((m_data.m_values[0] & m_data.m_values[1]) & (m_data.m_values[2] & m_data.m_values[3])) &
            ((m_data.m_values[4] & m_data.m_values[5]) & (m_data.m_values[6] & m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD8<T, Width>::Mask::getNone() const noexcept
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
class SIMDMasker8
{
public:
    XS_INLINE static SIMDInBase<T, Width>& ToType(SIMD8<T, Width>& other)
    {
        return reinterpret_cast<SIMDInBase<T, Width>*>(other.m_data.m_values)[Index];
    }
};
template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD8<T, Width>::Mask::mask8Function(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if (m_data.m_values[0]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 0>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 0>>(value);
        }
        if (m_data.m_values[1]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 1>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 1>>(value);
        }
        if (m_data.m_values[2]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 2>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 2>>(value);
        }
        if (m_data.m_values[3]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 3>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 3>>(value);
        }
        if (m_data.m_values[4]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 4>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 4>>(value);
        }
        if (m_data.m_values[5]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 5>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 5>>(value);
        }
        if (m_data.m_values[6]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 6>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 6>>(value);
        }
        if (m_data.m_values[7]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 7>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 7>>(value);
        }
    }
}

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD8<T, Width>::Mask::mask8ElseFunction(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        auto value = (m_data.m_values[0]) ?
            maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 0>>() :
            maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 0>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 0>>(value);
        value = (m_data.m_values[1]) ? maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 1>>() :
                                       maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 1>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 1>>(value);
        value = (m_data.m_values[2]) ? maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 2>>() :
                                       maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 2>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 2>>(value);
        value = (m_data.m_values[3]) ? maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 3>>() :
                                       maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 3>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 3>>(value);
        value = (m_data.m_values[4]) ? maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 4>>() :
                                       maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 4>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 4>>(value);
        value = (m_data.m_values[5]) ? maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 5>>() :
                                       maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 5>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 5>>(value);
        value = (m_data.m_values[6]) ? maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 6>>() :
                                       maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 6>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 6>>(value);
        value = (m_data.m_values[7]) ? maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 7>>() :
                                       maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 7>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker8<T, Width, 7>>(value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::Shuffle::Shuffle(const uint32 shuff0, const uint32 shuff1, const uint32 shuff2,
    const uint32 shuff3, const uint32 shuff4, const uint32 shuff5, const uint32 shuff6, const uint32 shuff7) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = shuff0;
        m_data.m_values[1] = shuff1;
        m_data.m_values[2] = shuff2;
        m_data.m_values[3] = shuff3;
        m_data.m_values[4] = shuff4;
        m_data.m_values[5] = shuff5;
        m_data.m_values[6] = shuff6;
        m_data.m_values[7] = shuff7;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::Shuffle::Shuffle(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::Shuffle::Shuffle(const uint32 shuffle) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = shuffle;
        m_data.m_values[1] = shuffle;
        m_data.m_values[2] = shuffle;
        m_data.m_values[3] = shuffle;
        m_data.m_values[4] = shuffle;
        m_data.m_values[5] = shuffle;
        m_data.m_values[6] = shuffle;
        m_data.m_values[7] = shuffle;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Shuffle SIMD8<T, Width>::Shuffle::Default() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(0_ui32, 1_ui32, 2_ui32, 3_ui32, 4_ui32, 5_ui32, 6_ui32, 7_ui32);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Shuffle SIMD8<T, Width>::Shuffle::Swap2() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(1_ui32, 0_ui32, 3_ui32, 2_ui32, 5_ui32, 4_ui32, 7_ui32, 6_ui32);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Shuffle SIMD8<T, Width>::Shuffle::Swap4() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(2_ui32, 3_ui32, 0_ui32, 1_ui32, 6_ui32, 7_ui32, 4_ui32, 5_ui32);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::SIMD8(const T value0, const T value1, const T value2, const T value3, const T value4,
    const T value5, const T value6, const T value7) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::SIMD8(const T value) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>::SIMD8(const SIMDBase<T, Width2>& other) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>::SIMD8(const SIMDInBase<T, Width2>& other) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>::SIMD8(const SIMD2<T, Width2>& other0, const SIMD2<T, Width2>& other1,
    const SIMD2<T, Width2>& other2, const SIMD2<T, Width2>& other3) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>::SIMD8(const SIMD4<T, Width2>& other0, const SIMD4<T, Width2>& other1) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>::SIMD8(const SIMD4<T, Width2>& other) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>::SIMD8(const SIMD6<T, Width2>& other0, const SIMD2<T, Width2>& other1) noexcept
{
    XS_REQUIRES((sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>> && sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>))
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
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width>::SIMD8(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(T{1});
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE SIMDInBase<T, Width2> SIMD8<T, Width>::getValueInBase() const noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 8);
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
XS_INLINE SIMDBase<T, Width2> SIMD8<T, Width>::getValue() const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
    static_assert(Index < 8);
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
XS_INLINE SIMD2<T, Width2> SIMD8<T, Width>::getValue2() const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
    static_assert(Index < 4);
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
XS_INLINE SIMD4<T, Width2> SIMD8<T, Width>::getValue4() const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
    static_assert(Index < 2);
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
template<uint32 Index0, uint32 Index1, uint32 Index2, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width2> SIMD8<T, Width>::getValue3x2() const noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width2>(m_data.m_values[Index0], m_data.m_values[Index1], m_data.m_values[Index2],
            m_data.m_values[Index0 + 4], m_data.m_values[Index1 + 4], m_data.m_values[Index2 + 4]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD8<T, Width>::setValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 8);
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
XS_INLINE void SIMD8<T, Width>::setValue(const SIMDBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
    static_assert(Index < 8);
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
XS_INLINE void SIMD8<T, Width>::setValue2(const SIMD2<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
    static_assert(Index < 4);
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
XS_INLINE void SIMD8<T, Width>::setValue4(const SIMD4<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index * 4] = other.m_data.m_values[0];
        m_data.m_values[Index * 4 + 1] = other.m_data.m_value[1];
        m_data.m_values[Index * 4 + 2] = other.m_data.m_values[2];
        m_data.m_values[Index * 4 + 3] = other.m_data.m_value[3];
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD8<T, Width>::addValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 8);
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
XS_INLINE void SIMD8<T, Width>::subValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 8);
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
XS_INLINE void SIMD8<T, Width>::mulValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 8);
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
XS_INLINE void SIMD8<T, Width>::divValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 8);
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
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::negate() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Elem0 ? -m_data.m_values[0] : m_data.m_values[0], Elem1 ? -m_data.m_values[1] : m_data.m_values[1],
            Elem2 ? -m_data.m_values[2] : m_data.m_values[2], Elem3 ? -m_data.m_values[3] : m_data.m_values[3],
            Elem4 ? -m_data.m_values[4] : m_data.m_values[4], Elem5 ? -m_data.m_values[5] : m_data.m_values[5],
            Elem6 ? -m_data.m_values[6] : m_data.m_values[6], Elem7 ? -m_data.m_values[7] : m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::mad(const SIMD8<T, Width2>& other1, const SIMD8<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[4], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[5], other2.m_data.m_values[5]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[6], other2.m_data.m_values[6]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[7], other2.m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::mad(const SIMDBase<T, Width2>& other1, const SIMD8<T, Width>& other2) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_value, other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_value, other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_value, other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_value, other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_value, other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_value, other2.m_data.m_values[5]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_value, other2.m_data.m_values[6]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_value, other2.m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::mad(const SIMD8<T, Width>& other1, const SIMDBase<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[4], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[5], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[6], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[7], other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::mad(const SIMD4<T, Width2>& other1, const SIMD4<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[3], other2.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::mad(const SIMD4<T, Width2>& other1, const SIMD8<T, Width>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[0], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[1], other2.m_data.m_values[5]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[2], other2.m_data.m_values[6]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[3], other2.m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::msub(const SIMD8<T, Width2>& other1, const SIMD8<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], -other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], -other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], -other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], -other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[4], -other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[5], -other2.m_data.m_values[5]),
            Shift::fma<T>(m_data.m_values[6], other1.m_data.m_values[6], -other2.m_data.m_values[6]),
            Shift::fma<T>(m_data.m_values[7], other1.m_data.m_values[7], -other2.m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::mulH4(const SIMD2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(m_data.m_values[0] * other.m_data.m_values[0], m_data.m_values[1] * other.m_data.m_values[0],
            m_data.m_values[2] * other.m_data.m_values[0], m_data.m_values[3] * other.m_data.m_values[0],
            m_data.m_values[4] * other.m_data.m_values[1], m_data.m_values[5] * other.m_data.m_values[1],
            m_data.m_values[6] * other.m_data.m_values[1], m_data.m_values[7] * other.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::equalMask(const SIMD8<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] == other.m_data.m_values[0], m_data.m_values[1] == other.m_data.m_values[1],
            m_data.m_values[2] == other.m_data.m_values[2], m_data.m_values[3] == other.m_data.m_values[3],
            m_data.m_values[4] == other.m_data.m_values[4], m_data.m_values[5] == other.m_data.m_values[5],
            m_data.m_values[6] == other.m_data.m_values[6], m_data.m_values[7] == other.m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::lessOrEqualMask(const SIMD8<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] <= other.m_data.m_values[0], m_data.m_values[1] <= other.m_data.m_values[1],
            m_data.m_values[2] <= other.m_data.m_values[2], m_data.m_values[3] <= other.m_data.m_values[3],
            m_data.m_values[4] <= other.m_data.m_values[4], m_data.m_values[5] <= other.m_data.m_values[5],
            m_data.m_values[6] <= other.m_data.m_values[6], m_data.m_values[7] <= other.m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::lessThanMask(const SIMD8<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] < other.m_data.m_values[0], m_data.m_values[1] < other.m_data.m_values[1],
            m_data.m_values[2] < other.m_data.m_values[2], m_data.m_values[3] < other.m_data.m_values[3],
            m_data.m_values[4] < other.m_data.m_values[4], m_data.m_values[5] < other.m_data.m_values[5],
            m_data.m_values[6] < other.m_data.m_values[6], m_data.m_values[7] < other.m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::notEqualMask(const SIMD8<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] != other.m_data.m_values[0], m_data.m_values[1] != other.m_data.m_values[1],
            m_data.m_values[2] != other.m_data.m_values[2], m_data.m_values[3] != other.m_data.m_values[3],
            m_data.m_values[4] != other.m_data.m_values[4], m_data.m_values[5] != other.m_data.m_values[5],
            m_data.m_values[6] != other.m_data.m_values[6], m_data.m_values[7] != other.m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::equalMask(const SIMDBase<T, Width2>& other) const noexcept
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
            m_data.m_values[4] == other.m_data.m_value, m_data.m_values[5] == other.m_data.m_value,
            m_data.m_values[6] == other.m_data.m_value, m_data.m_values[7] == other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::lessOrEqualMask(const SIMDBase<T, Width2>& other) const
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
            m_data.m_values[4] <= other.m_data.m_value, m_data.m_values[5] <= other.m_data.m_value,
            m_data.m_values[6] <= other.m_data.m_value, m_data.m_values[7] <= other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::lessThanMask(const SIMDBase<T, Width2>& other) const noexcept
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
            m_data.m_values[4] < other.m_data.m_value, m_data.m_values[5] < other.m_data.m_value,
            m_data.m_values[6] < other.m_data.m_value, m_data.m_values[7] < other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::greaterOrEqualMask(const SIMDBase<T, Width2>& other) const
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
            m_data.m_values[4] >= other.m_data.m_value, m_data.m_values[5] >= other.m_data.m_value,
            m_data.m_values[6] >= other.m_data.m_value, m_data.m_values[7] >= other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::greaterThanMask(const SIMDBase<T, Width2>& other) const
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
            m_data.m_values[4] > other.m_data.m_value, m_data.m_values[5] > other.m_data.m_value,
            m_data.m_values[6] > other.m_data.m_value, m_data.m_values[7] > other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD8<T, Width>::Mask SIMD8<T, Width>::notEqualMask(const SIMDBase<T, Width2>& other) const noexcept
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
            m_data.m_values[4] != other.m_data.m_value, m_data.m_values[5] != other.m_data.m_value,
            m_data.m_values[6] != other.m_data.m_value, m_data.m_values[7] != other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::sign(const SIMD8<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::sign<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::sign<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::sign<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::sign<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::sign<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::sign<T>(m_data.m_values[5], other.m_data.m_values[5]),
            Shift::sign<T>(m_data.m_values[6], other.m_data.m_values[6]),
            Shift::sign<T>(m_data.m_values[7], other.m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::abs<T>(m_data.m_values[0]), Shift::abs<T>(m_data.m_values[1]),
            Shift::abs<T>(m_data.m_values[2]), Shift::abs<T>(m_data.m_values[3]), Shift::abs<T>(m_data.m_values[4]),
            Shift::abs<T>(m_data.m_values[5]), Shift::abs<T>(m_data.m_values[6]), Shift::abs<T>(m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::max(const SIMD8<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::max<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::max<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::max<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::max<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::max<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::max<T>(m_data.m_values[5], other.m_data.m_values[5]),
            Shift::max<T>(m_data.m_values[6], other.m_data.m_values[6]),
            Shift::max<T>(m_data.m_values[7], other.m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::min(const SIMD8<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::min<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::min<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::min<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::min<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::min<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::min<T>(m_data.m_values[5], other.m_data.m_values[5]),
            Shift::min<T>(m_data.m_values[6], other.m_data.m_values[6]),
            Shift::min<T>(m_data.m_values[7], other.m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::max(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::max<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[3], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[4], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[5], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[6], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[7], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::min(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
        return SIMD8(Shift::min<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[3], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[4], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[5], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[6], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[7], other.m_data.m_value));
    {}
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD8<T, Width>::max2() const noexcept
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
        return SIMD2<T, Width>(Shift::max<T>(other1, other3), Shift::max<T>(other2, other4));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD8<T, Width>::min2() const noexcept
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
        return SIMD2<T, Width>(Shift::min<T>(other1, other3), Shift::min<T>(other2, other4));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD8<T, Width>::max4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>(Shift::max<T>(m_data.m_values[0], m_data.m_values[4]),
            Shift::max<T>(m_data.m_values[1], m_data.m_values[5]),
            Shift::max<T>(m_data.m_values[2], m_data.m_values[6]),
            Shift::max<T>(m_data.m_values[3], m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD8<T, Width>::min4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>(Shift::min<T>(m_data.m_values[0], m_data.m_values[4]),
            Shift::min<T>(m_data.m_values[1], m_data.m_values[5]),
            Shift::min<T>(m_data.m_values[2], m_data.m_values[6]),
            Shift::min<T>(m_data.m_values[3], m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::BaseDef SIMD8<T, Width>::hmax() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other = Shift::max<T>(m_data.m_values[0], m_data.m_values[1]);
        const T other2 = Shift::max<T>(m_data.m_values[2], m_data.m_values[3]);
        T other3 = Shift::max<T>(m_data.m_values[4], m_data.m_values[5]);
        const T other4 = Shift::max<T>(m_data.m_values[6], m_data.m_values[7]);
        other = Shift::max<T>(other, other2);
        other3 = Shift::max<T>(other3, other4);
        return BaseDef(Shift::max<T>(other, other3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::BaseDef SIMD8<T, Width>::hmin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other = Shift::min<T>(m_data.m_values[0], m_data.m_values[1]);
        const T other2 = Shift::min<T>(m_data.m_values[2], m_data.m_values[3]);
        T other3 = Shift::min<T>(m_data.m_values[4], m_data.m_values[5]);
        const T other4 = Shift::min<T>(m_data.m_values[6], m_data.m_values[7]);
        other = Shift::min<T>(other, other2);
        other3 = Shift::min<T>(other3, other4);
        return BaseDef(Shift::min<T>(other, other3));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD8<T, Width>::hmaxInBase() const noexcept
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
XS_INLINE SIMDInBase<T, Width> SIMD8<T, Width>::hminInBase() const noexcept
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
XS_INLINE typename SIMD8<T, Width>::BaseDef SIMD8<T, Width>::hmaxIndex(uint32& index) const noexcept
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
        T other4 = Shift::max<T>(m_data.m_values[6], m_data.m_values[7]);
        uint32 index4 = (other4 == m_data.m_values[6]) ? 6 : 7;
        other2 = Shift::max<T>(other2, other);
        index2 = (other2 == other) ? index : index2;
        other4 = Shift::max<T>(other4, other3);
        index4 = (other4 == other3) ? index3 : index4;
        other = Shift::max<T>(other4, other2);
        index = (other == other2) ? index2 : index4;
        return BaseDef(other);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::BaseDef SIMD8<T, Width>::hminIndex(uint32& index) const noexcept
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
        T other4 = Shift::min<T>(m_data.m_values[6], m_data.m_values[7]);
        uint32 index4 = (other4 == m_data.m_values[6]) ? 6 : 7;
        other2 = Shift::min<T>(other2, other);
        index2 = (other2 == other) ? index : index2;
        other4 = Shift::min<T>(other4, other3);
        index4 = (other4 == other3) ? index3 : index4;
        other = Shift::min<T>(other4, other2);
        index = (other == other2) ? index2 : index4;
        return BaseDef(other);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD8<T, Width>::hmax2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>(Shift::max<T>(m_data.m_values[0], m_data.m_values[1]),
            Shift::max<T>(m_data.m_values[2], m_data.m_values[3]),
            Shift::max<T>(m_data.m_values[4], m_data.m_values[5]),
            Shift::max<T>(m_data.m_values[6], m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD8<T, Width>::hmin2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>(Shift::min<T>(m_data.m_values[0], m_data.m_values[1]),
            Shift::min<T>(m_data.m_values[2], m_data.m_values[3]),
            Shift::min<T>(m_data.m_values[4], m_data.m_values[5]),
            Shift::min<T>(m_data.m_values[6], m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD8<T, Width>::hmax4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other1 = Shift::max<T>(m_data.m_values[0], m_data.m_values[1]);
        T other2 = Shift::max<T>(m_data.m_values[4], m_data.m_values[5]);
        other1 = Shift::max<T>(m_data.m_values[2], other1);
        other2 = Shift::max<T>(m_data.m_values[6], other2);
        return SIMD2<T, Width>(Shift::max<T>(m_data.m_values[3], other1), Shift::max<T>(m_data.m_values[7], other2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD8<T, Width>::hmin4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T other1 = Shift::min<T>(m_data.m_values[0], m_data.m_values[1]);
        T other2 = Shift::min<T>(m_data.m_values[4], m_data.m_values[5]);
        other1 = Shift::min<T>(m_data.m_values[2], other1);
        other2 = Shift::min<T>(m_data.m_values[6], other2);
        return SIMD2<T, Width>(Shift::min<T>(m_data.m_values[3], other1), Shift::min<T>(m_data.m_values[7], other2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::reciprocal() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::recip<T>(m_data.m_values[0]), Shift::recip<T>(m_data.m_values[1]),
            Shift::recip<T>(m_data.m_values[2]), Shift::recip<T>(m_data.m_values[3]),
            Shift::recip<T>(m_data.m_values[4]), Shift::recip<T>(m_data.m_values[5]),
            Shift::recip<T>(m_data.m_values[6]), Shift::recip<T>(m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD8<T, Width>::add2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T res1 = (m_data.m_values[0] + m_data.m_values[4]);
        T res2 = (m_data.m_values[1] + m_data.m_values[5]);
        const T res3 = (m_data.m_values[2] + m_data.m_values[6]);
        const T res4 = (m_data.m_values[3] + m_data.m_values[7]);
        res1 += res3;
        res2 += res4;
        return SIMD2<T, Width>(res1, res2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD8<T, Width>::add4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T res1 = (m_data.m_values[0] + m_data.m_values[4]);
        const T res2 = (m_data.m_values[1] + m_data.m_values[5]);
        const T res3 = (m_data.m_values[2] + m_data.m_values[6]);
        const T res4 = (m_data.m_values[3] + m_data.m_values[7]);
        return SIMD4<T, Width>(res1, res2, res3, res4);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD8<T, Width>::sub4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T res1 = (m_data.m_values[0] - m_data.m_values[4]);
        const T res2 = (m_data.m_values[1] - m_data.m_values[5]);
        const T res3 = (m_data.m_values[2] - m_data.m_values[6]);
        const T res4 = (m_data.m_values[3] - m_data.m_values[7]);
        return SIMD4<T, Width>(res1, res2, res3, res4);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::BaseDef SIMD8<T, Width>::hadd() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T res = (m_data.m_values[0] + m_data.m_values[1]);
        const T res2 = (m_data.m_values[2] + m_data.m_values[3]);
        T res3 = (m_data.m_values[4] + m_data.m_values[5]);
        const T res4 = (m_data.m_values[6] + m_data.m_values[7]);
        res += res2;
        res3 += res4;
        res += res3;
        return BaseDef(res);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD8<T, Width>::haddInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width>(hadd().m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD8<T, Width>::hadd2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>(m_data.m_values[0] + m_data.m_values[1], m_data.m_values[2] + m_data.m_values[3],
            m_data.m_values[4] + m_data.m_values[5], m_data.m_values[6] + m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD8<T, Width>::hadd4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T res = (m_data.m_values[0] + m_data.m_values[1]);
        T res2 = (m_data.m_values[4] + m_data.m_values[5]);
        const T res3 = (m_data.m_values[2] + m_data.m_values[3]);
        const T res4 = (m_data.m_values[6] + m_data.m_values[7]);
        res += res3;
        res2 += res4;
        return SIMD2<T, Width>(res, res2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD8<T, Width>::hsub2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>(m_data.m_values[0] - m_data.m_values[1], m_data.m_values[2] - m_data.m_values[3],
            m_data.m_values[4] - m_data.m_values[5], m_data.m_values[6] - m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::ceil<T>(m_data.m_values[0]), Shift::ceil<T>(m_data.m_values[1]),
            Shift::ceil<T>(m_data.m_values[2]), Shift::ceil<T>(m_data.m_values[3]), Shift::ceil<T>(m_data.m_values[4]),
            Shift::ceil<T>(m_data.m_values[5]), Shift::ceil<T>(m_data.m_values[6]), Shift::ceil<T>(m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::floor<T>(m_data.m_values[0]), Shift::floor<T>(m_data.m_values[1]),
            Shift::floor<T>(m_data.m_values[2]), Shift::floor<T>(m_data.m_values[3]),
            Shift::floor<T>(m_data.m_values[4]), Shift::floor<T>(m_data.m_values[5]),
            Shift::floor<T>(m_data.m_values[6]), Shift::floor<T>(m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::trunc<T>(m_data.m_values[0]), Shift::trunc<T>(m_data.m_values[1]),
            Shift::trunc<T>(m_data.m_values[2]), Shift::trunc<T>(m_data.m_values[3]),
            Shift::trunc<T>(m_data.m_values[4]), Shift::trunc<T>(m_data.m_values[5]),
            Shift::trunc<T>(m_data.m_values[6]), Shift::trunc<T>(m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::sqrt<T>(m_data.m_values[0]), Shift::sqrt<T>(m_data.m_values[1]),
            Shift::sqrt<T>(m_data.m_values[2]), Shift::sqrt<T>(m_data.m_values[3]), Shift::sqrt<T>(m_data.m_values[4]),
            Shift::sqrt<T>(m_data.m_values[5]), Shift::sqrt<T>(m_data.m_values[6]), Shift::sqrt<T>(m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::rsqrt<T>(m_data.m_values[0]), Shift::rsqrt<T>(m_data.m_values[1]),
            Shift::rsqrt<T>(m_data.m_values[2]), Shift::rsqrt<T>(m_data.m_values[3]),
            Shift::rsqrt<T>(m_data.m_values[4]), Shift::rsqrt<T>(m_data.m_values[5]),
            Shift::rsqrt<T>(m_data.m_values[6]), Shift::rsqrt<T>(m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::exp2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::exp2<T>(m_data.m_values[0]), Shift::exp2<T>(m_data.m_values[1]),
            Shift::exp2<T>(m_data.m_values[2]), Shift::exp2<T>(m_data.m_values[3]), Shift::exp2<T>(m_data.m_values[4]),
            Shift::exp2<T>(m_data.m_values[5]), Shift::exp2<T>(m_data.m_values[6]), Shift::exp2<T>(m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::exp() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::exp<T>(m_data.m_values[0]), Shift::exp<T>(m_data.m_values[1]),
            Shift::exp<T>(m_data.m_values[2]), Shift::exp<T>(m_data.m_values[3]), Shift::exp<T>(m_data.m_values[4]),
            Shift::exp<T>(m_data.m_values[5]), Shift::exp<T>(m_data.m_values[6]), Shift::exp<T>(m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::log2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::log2<T>(m_data.m_values[0]), Shift::log2<T>(m_data.m_values[1]),
            Shift::log2<T>(m_data.m_values[2]), Shift::log2<T>(m_data.m_values[3]), Shift::log2<T>(m_data.m_values[4]),
            Shift::log2<T>(m_data.m_values[5]), Shift::log2<T>(m_data.m_values[6]), Shift::log2<T>(m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::log() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::log<T>(m_data.m_values[0]), Shift::log<T>(m_data.m_values[1]),
            Shift::log<T>(m_data.m_values[2]), Shift::log<T>(m_data.m_values[3]), Shift::log<T>(m_data.m_values[4]),
            Shift::log<T>(m_data.m_values[5]), Shift::log<T>(m_data.m_values[6]), Shift::log<T>(m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::pow(const SIMD8<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::pow<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::pow<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::pow<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::pow<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::pow<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::pow<T>(m_data.m_values[5], other.m_data.m_values[5]),
            Shift::pow<T>(m_data.m_values[6], other.m_data.m_values[6]),
            Shift::pow<T>(m_data.m_values[7], other.m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::powr(const SIMD8<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::powr<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::powr<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::powr<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::powr<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::powr<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::powr<T>(m_data.m_values[5], other.m_data.m_values[5]),
            Shift::powr<T>(m_data.m_values[6], other.m_data.m_values[6]),
            Shift::powr<T>(m_data.m_values[7], other.m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::pow(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::pow<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[3], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[4], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[5], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[6], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[7], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::powr(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::powr<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[3], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[4], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[5], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[6], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[7], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::sin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::sin<T>(m_data.m_values[0]), Shift::sin<T>(m_data.m_values[1]),
            Shift::sin<T>(m_data.m_values[2]), Shift::sin<T>(m_data.m_values[3]), Shift::sin<T>(m_data.m_values[4]),
            Shift::sin<T>(m_data.m_values[5]), Shift::sin<T>(m_data.m_values[6]), Shift::sin<T>(m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::cos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::cos<T>(m_data.m_values[0]), Shift::cos<T>(m_data.m_values[1]),
            Shift::cos<T>(m_data.m_values[2]), Shift::cos<T>(m_data.m_values[3]), Shift::cos<T>(m_data.m_values[4]),
            Shift::cos<T>(m_data.m_values[5]), Shift::cos<T>(m_data.m_values[6]), Shift::cos<T>(m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::tan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::tan<T>(m_data.m_values[0]), Shift::tan<T>(m_data.m_values[1]),
            Shift::tan<T>(m_data.m_values[2]), Shift::tan<T>(m_data.m_values[3]), Shift::tan<T>(m_data.m_values[4]),
            Shift::tan<T>(m_data.m_values[5]), Shift::tan<T>(m_data.m_values[6]), Shift::tan<T>(m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD16<T, Width> SIMD8<T, Width>::sincos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        SIMD16 ret;
        ret.m_data.m_values[0] = Shift::sincos<T>(m_data.m_values[0], ret.m_data.m_values[8]);
        ret.m_data.m_values[1] = Shift::sincos<T>(m_data.m_values[1], ret.m_data.m_values[9]);
        ret.m_data.m_values[2] = Shift::sincos<T>(m_data.m_values[2], ret.m_data.m_values[10]);
        ret.m_data.m_values[3] = Shift::sincos<T>(m_data.m_values[3], ret.m_data.m_values[11]);
        ret.m_data.m_values[4] = Shift::sincos<T>(m_data.m_values[4], ret.m_data.m_values[12]);
        ret.m_data.m_values[5] = Shift::sincos<T>(m_data.m_values[5], ret.m_data.m_values[13]);
        ret.m_data.m_values[6] = Shift::sincos<T>(m_data.m_values[6], ret.m_data.m_values[14]);
        ret.m_data.m_values[7] = Shift::sincos<T>(m_data.m_values[7], ret.m_data.m_values[15]);
        return ret;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::asin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::asin<T>(m_data.m_values[0]), Shift::asin<T>(m_data.m_values[1]),
            Shift::asin<T>(m_data.m_values[2]), Shift::asin<T>(m_data.m_values[3]), Shift::asin<T>(m_data.m_values[4]),
            Shift::asin<T>(m_data.m_values[5]), Shift::asin<T>(m_data.m_values[6]), Shift::asin<T>(m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::acos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::acos<T>(m_data.m_values[0]), Shift::acos<T>(m_data.m_values[1]),
            Shift::acos<T>(m_data.m_values[2]), Shift::acos<T>(m_data.m_values[3]), Shift::acos<T>(m_data.m_values[4]),
            Shift::acos<T>(m_data.m_values[5]), Shift::acos<T>(m_data.m_values[6]), Shift::acos<T>(m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::atan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::atan<T>(m_data.m_values[0]), Shift::atan<T>(m_data.m_values[1]),
            Shift::atan<T>(m_data.m_values[2]), Shift::atan<T>(m_data.m_values[3]), Shift::atan<T>(m_data.m_values[4]),
            Shift::atan<T>(m_data.m_values[5]), Shift::atan<T>(m_data.m_values[6]), Shift::atan<T>(m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::atan2(const SIMD8<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(Shift::atan2<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::atan2<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::atan2<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::atan2<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::atan2<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::atan2<T>(m_data.m_values[5], other.m_data.m_values[5]),
            Shift::atan2<T>(m_data.m_values[6], other.m_data.m_values[6]),
            Shift::atan2<T>(m_data.m_values[7], other.m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::insert(const SIMD8<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
    static_assert(Index0 < 8 && Index1 < 8);
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
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::insert2(const SIMD8<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
    static_assert(Index0 < 4 && Index1 < 4);
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
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::insert4(const SIMD8<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
    static_assert(Index0 < 2 && Index1 < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index0 * 4] == other.m_data.m_values[Index1 * 4];
        m_data.m_values[Index0 * 4 + 1] == other.m_data.m_values[Index1 * 4 + 1];
        m_data.m_values[Index0 * 4 + 2] == other.m_data.m_values[Index1 * 4 + 2];
        m_data.m_values[Index0 * 4 + 3] == other.m_data.m_values[Index1 * 4 + 3];
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7,
    SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::blend(const SIMD8<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8((Elem0) ? other.m_data.m_values[0] : m_data.m_values[0],
            (Elem1) ? other.m_data.m_values[1] : m_data.m_values[1],
            (Elem2) ? other.m_data.m_values[2] : m_data.m_values[2],
            (Elem3) ? other.m_data.m_values[3] : m_data.m_values[3],
            (Elem4) ? other.m_data.m_values[4] : m_data.m_values[4],
            (Elem5) ? other.m_data.m_values[5] : m_data.m_values[5],
            (Elem6) ? other.m_data.m_values[6] : m_data.m_values[6],
            (Elem7) ? other.m_data.m_values[7] : m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::blendVar(const SIMD8<T, Width2>& other, const Mask& mask) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const SIMD8 backup = other;
        other.m_data.m_values[0] = (mask.m_data.m_values[0]) ? m_data.m_values[0] : other.m_data.m_values[0];
        other.m_data.m_values[1] = (mask.m_data.m_values[1]) ? m_data.m_values[1] : other.m_data.m_values[1];
        other.m_data.m_values[2] = (mask.m_data.m_values[2]) ? m_data.m_values[2] : other.m_data.m_values[2];
        other.m_data.m_values[3] = (mask.m_data.m_values[3]) ? m_data.m_values[3] : other.m_data.m_values[3];
        other.m_data.m_values[4] = (mask.m_data.m_values[4]) ? m_data.m_values[4] : other.m_data.m_values[4];
        other.m_data.m_values[5] = (mask.m_data.m_values[5]) ? m_data.m_values[5] : other.m_data.m_values[5];
        other.m_data.m_values[6] = (mask.m_data.m_values[6]) ? m_data.m_values[6] : other.m_data.m_values[6];
        other.m_data.m_values[7] = (mask.m_data.m_values[7]) ? m_data.m_values[7] : other.m_data.m_values[7];
        return SIMD8((mask.m_data.m_values[0]) ? backup.m_data.m_values[0] : m_data.m_values[0],
            (mask.m_data.m_values[1]) ? backup.m_data.m_values[1] : m_data.m_values[1],
            (mask.m_data.m_values[2]) ? backup.m_data.m_values[2] : m_data.m_values[2],
            (mask.m_data.m_values[3]) ? backup.m_data.m_values[3] : m_data.m_values[3],
            (mask.m_data.m_values[4]) ? backup.m_data.m_values[4] : m_data.m_values[4],
            (mask.m_data.m_values[5]) ? backup.m_data.m_values[5] : m_data.m_values[5],
            (mask.m_data.m_values[6]) ? backup.m_data.m_values[6] : m_data.m_values[6],
            (mask.m_data.m_values[7]) ? backup.m_data.m_values[7] : m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7,
    SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::blendSwap(SIMD8<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const SIMD8 backup = other;
        other.m_data.m_values[0] = (Elem0) ? m_data.m_values[0] : other.m_data.m_values[0];
        other.m_data.m_values[1] = (Elem1) ? m_data.m_values[1] : other.m_data.m_values[1];
        other.m_data.m_values[2] = (Elem2) ? m_data.m_values[2] : other.m_data.m_values[2];
        other.m_data.m_values[3] = (Elem3) ? m_data.m_values[3] : other.m_data.m_values[3];
        other.m_data.m_values[4] = (Elem4) ? m_data.m_values[4] : other.m_data.m_values[4];
        other.m_data.m_values[5] = (Elem5) ? m_data.m_values[5] : other.m_data.m_values[5];
        other.m_data.m_values[6] = (Elem6) ? m_data.m_values[6] : other.m_data.m_values[6];
        other.m_data.m_values[7] = (Elem7) ? m_data.m_values[7] : other.m_data.m_values[7];
        return SIMD8((Elem0) ? backup.m_data.m_values[0] : m_data.m_values[0],
            (Elem1) ? backup.m_data.m_values[1] : m_data.m_values[1],
            (Elem2) ? backup.m_data.m_values[2] : m_data.m_values[2],
            (Elem3) ? backup.m_data.m_values[3] : m_data.m_values[3],
            (Elem4) ? backup.m_data.m_values[4] : m_data.m_values[4],
            (Elem5) ? backup.m_data.m_values[5] : m_data.m_values[5],
            (Elem6) ? backup.m_data.m_values[6] : m_data.m_values[6],
            (Elem7) ? backup.m_data.m_values[7] : m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::blendSwapVar(SIMD8<T, Width2>& other, const Mask& mask) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const SIMD8 backup = other;
        other.m_data.m_values[0] = (mask.m_data.m_values[0]) ? m_data.m_values[0] : other.m_data.m_values[0];
        other.m_data.m_values[1] = (mask.m_data.m_values[1]) ? m_data.m_values[1] : other.m_data.m_values[1];
        other.m_data.m_values[2] = (mask.m_data.m_values[2]) ? m_data.m_values[2] : other.m_data.m_values[2];
        other.m_data.m_values[3] = (mask.m_data.m_values[3]) ? m_data.m_values[3] : other.m_data.m_values[3];
        other.m_data.m_values[4] = (mask.m_data.m_values[4]) ? m_data.m_values[4] : other.m_data.m_values[4];
        other.m_data.m_values[5] = (mask.m_data.m_values[5]) ? m_data.m_values[5] : other.m_data.m_values[5];
        other.m_data.m_values[6] = (mask.m_data.m_values[6]) ? m_data.m_values[6] : other.m_data.m_values[6];
        other.m_data.m_values[7] = (mask.m_data.m_values[7]) ? m_data.m_values[7] : other.m_data.m_values[7];
        return SIMD8((mask.m_data.m_values[0]) ? backup.m_data.m_values[0] : m_data.m_values[0],
            (mask.m_data.m_values[1]) ? backup.m_data.m_values[1] : m_data.m_values[1],
            (mask.m_data.m_values[2]) ? backup.m_data.m_values[2] : m_data.m_values[2],
            (mask.m_data.m_values[3]) ? backup.m_data.m_values[3] : m_data.m_values[3],
            (mask.m_data.m_values[4]) ? backup.m_data.m_values[4] : m_data.m_values[4],
            (mask.m_data.m_values[5]) ? backup.m_data.m_values[5] : m_data.m_values[5],
            (mask.m_data.m_values[6]) ? backup.m_data.m_values[6] : m_data.m_values[6],
            (mask.m_data.m_values[7]) ? backup.m_data.m_values[7] : m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::blend2(const SIMD8<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const auto temp0 = (Elem0) ?
            SIMD4<T, Width>(other.m_data.m_values[0], other.m_data.m_values[2], other.m_data.m_values[4],
                other.m_data.m_values[6]) :
            SIMD4<T, Width>(m_data.m_values[0], m_data.m_values[2], m_data.m_values[4], m_data.m_values[6]);
        const auto temp1 = (Elem1) ?
            SIMD4<T, Width>(other.m_data.m_values[1], other.m_data.m_values[3], other.m_data.m_values[5],
                other.m_data.m_values[7]) :
            SIMD4<T, Width>(m_data.m_values[1], m_data.m_values[3], m_data.m_values[5], m_data.m_values[7]);
        return SIMD8(temp0.m_data.m_values[0], temp1.m_data.m_values[0], temp0.m_data.m_values[1],
            temp1.m_data.m_values[1], temp0.m_data.m_values[2], temp1.m_data.m_values[2], temp0.m_data.m_values[3],
            temp1.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::blend4(const SIMD8<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const auto temp0 = (Elem0) ? SIMD2<T, Width>(other.m_data.m_values[0], other.m_data.m_values[4]) :
                                     SIMD2<T, Width>(m_data.m_values[0], m_data.m_values[4]);
        const auto temp1 = (Elem1) ? SIMD2<T, Width>(other.m_data.m_values[1], other.m_data.m_values[5]) :
                                     SIMD2<T, Width>(m_data.m_values[1], m_data.m_values[5]);
        const auto temp2 = (Elem2) ? SIMD2<T, Width>(other.m_data.m_values[2], other.m_data.m_values[6]) :
                                     SIMD2<T, Width>(m_data.m_values[2], m_data.m_values[6]);
        const auto temp3 = (Elem3) ? SIMD2<T, Width>(other.m_data.m_values[3], other.m_data.m_values[7]) :
                                     SIMD2<T, Width>(m_data.m_values[3], m_data.m_values[7]);
        return SIMD8(temp0.m_data.m_values[0], temp1.m_data.m_values[0], temp2.m_data.m_values[0],
            temp3.m_data.m_values[0], temp0.m_data.m_values[1], temp1.m_data.m_values[1], temp2.m_data.m_values[1],
            temp3.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3, uint32 Index4, uint32 Index5, uint32 Index6,
    uint32 Index7>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::shuffle() const noexcept
{
    static_assert(
        Index0 < 8 && Index1 < 8 && Index2 < 8 && Index3 < 8 && Index4 < 8 && Index5 < 8 && Index6 < 8 && Index7 < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(m_data.m_values[Index0], m_data.m_values[Index1], m_data.m_values[Index2], m_data.m_values[Index3],
            m_data.m_values[Index4], m_data.m_values[Index5], m_data.m_values[Index6], m_data.m_values[Index7]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::shuffleVar(const Shuffle& shuffle) const noexcept
{
    XS_ASSERT(shuffle.m_data.m_values[0] < 8 && shuffle.m_data.m_values[1] < 8 && shuffle.m_data.m_values[2] < 8 &&
        shuffle.m_data.m_values[3] < 8 && shuffle.m_data.m_values[4] < 8 && shuffle.m_data.m_values[5] < 8 &&
        shuffle.m_data.m_values[6] < 8 && shuffle.m_data.m_values[7] < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(m_data.m_values[shuffle.m_data.m_values[0]], m_data.m_values[shuffle.m_data.m_values[1]],
            m_data.m_values[shuffle.m_data.m_values[2]], m_data.m_values[shuffle.m_data.m_values[3]],
            m_data.m_values[shuffle.m_data.m_values[4]], m_data.m_values[shuffle.m_data.m_values[5]],
            m_data.m_values[shuffle.m_data.m_values[6]], m_data.m_values[shuffle.m_data.m_values[7]]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::shuffle2() const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(m_data.m_values[Index0], m_data.m_values[Index1], m_data.m_values[Index0 + 2],
            m_data.m_values[Index1 + 2], m_data.m_values[Index0 + 4], m_data.m_values[Index1 + 4],
            m_data.m_values[Index0 + 6], m_data.m_values[Index1 + 6]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::shuffle4() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(m_data.m_values[Index0], m_data.m_values[Index1], m_data.m_values[Index2], m_data.m_values[Index3],
            m_data.m_values[Index0 + 4], m_data.m_values[Index1 + 4], m_data.m_values[Index2 + 4],
            m_data.m_values[Index3 + 4]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::shuffleH2() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(getValue2<Index0>(), getValue2<Index1>(), getValue2<Index2>(), getValue2<Index3>());
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD8<T, Width> SIMD8<T, Width>::shuffleH4() const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8(getValue4<Index0>(), getValue4<Index1>());
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator+(const SIMD8<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
            other1.m_data.m_values[1] + other2.m_data.m_values[1],
            other1.m_data.m_values[2] + other2.m_data.m_values[2],
            other1.m_data.m_values[3] + other2.m_data.m_values[3],
            other1.m_data.m_values[4] + other2.m_data.m_values[4],
            other1.m_data.m_values[5] + other2.m_data.m_values[5],
            other1.m_data.m_values[6] + other2.m_data.m_values[6],
            other1.m_data.m_values[7] + other2.m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator+(const SIMD8<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD8<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_value,
            other1.m_data.m_values[1] + other2.m_data.m_value, other1.m_data.m_values[2] + other2.m_data.m_value,
            other1.m_data.m_values[3] + other2.m_data.m_value, other1.m_data.m_values[4] + other2.m_data.m_value,
            other1.m_data.m_values[5] + other2.m_data.m_value, other1.m_data.m_values[6] + other2.m_data.m_value,
            other1.m_data.m_values[7] + other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator+(const SIMD8<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
            other1.m_data.m_values[1] + other2.m_data.m_values[1],
            other1.m_data.m_values[2] + other2.m_data.m_values[0],
            other1.m_data.m_values[3] + other2.m_data.m_values[1],
            other1.m_data.m_values[4] + other2.m_data.m_values[0],
            other1.m_data.m_values[5] + other2.m_data.m_values[1],
            other1.m_data.m_values[6] + other2.m_data.m_values[0],
            other1.m_data.m_values[7] + other2.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator+(const SIMD8<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
            other1.m_data.m_values[1] + other2.m_data.m_values[1],
            other1.m_data.m_values[2] + other2.m_data.m_values[2],
            other1.m_data.m_values[3] + other2.m_data.m_values[3],
            other1.m_data.m_values[4] + other2.m_data.m_values[0],
            other1.m_data.m_values[5] + other2.m_data.m_values[1],
            other1.m_data.m_values[6] + other2.m_data.m_values[2],
            other1.m_data.m_values[7] + other2.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator-(const SIMD8<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
            other1.m_data.m_values[1] - other2.m_data.m_values[1],
            other1.m_data.m_values[2] - other2.m_data.m_values[2],
            other1.m_data.m_values[3] - other2.m_data.m_values[3],
            other1.m_data.m_values[4] - other2.m_data.m_values[4],
            other1.m_data.m_values[5] - other2.m_data.m_values[5],
            other1.m_data.m_values[6] - other2.m_data.m_values[6],
            other1.m_data.m_values[7] - other2.m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator-(const SIMD8<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD8<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_value,
            other1.m_data.m_values[1] - other2.m_data.m_value, other1.m_data.m_values[2] - other2.m_data.m_value,
            other1.m_data.m_values[3] - other2.m_data.m_value, other1.m_data.m_values[4] - other2.m_data.m_value,
            other1.m_data.m_values[5] - other2.m_data.m_value, other1.m_data.m_values[6] - other2.m_data.m_value,
            other1.m_data.m_values[7] - other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator-(const SIMDBase<T, Width2>& other1, const SIMD8<T, Width>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD8<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(other1.m_data.m_value - other2.m_data.m_values[0],
            other1.m_data.m_value - other2.m_data.m_values[1], other1.m_data.m_value - other2.m_data.m_values[2],
            other1.m_data.m_value - other2.m_data.m_values[3], other1.m_data.m_value - other2.m_data.m_values[4],
            other1.m_data.m_value - other2.m_data.m_values[5], other1.m_data.m_value - other2.m_data.m_values[6],
            other1.m_data.m_value - other2.m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator-(const SIMD8<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
            other1.m_data.m_values[1] - other2.m_data.m_values[1],
            other1.m_data.m_values[2] - other2.m_data.m_values[0],
            other1.m_data.m_values[3] - other2.m_data.m_values[1],
            other1.m_data.m_values[4] - other2.m_data.m_values[0],
            other1.m_data.m_values[5] - other2.m_data.m_values[1],
            other1.m_data.m_values[6] - other2.m_data.m_values[0],
            other1.m_data.m_values[7] - other2.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator-(const SIMD8<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
            other1.m_data.m_values[1] - other2.m_data.m_values[1],
            other1.m_data.m_values[2] - other2.m_data.m_values[2],
            other1.m_data.m_values[3] - other2.m_data.m_values[3],
            other1.m_data.m_values[4] - other2.m_data.m_values[0],
            other1.m_data.m_values[5] - other2.m_data.m_values[1],
            other1.m_data.m_values[6] - other2.m_data.m_values[2],
            other1.m_data.m_values[7] - other2.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator*(const SIMD8<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
            other1.m_data.m_values[1] * other2.m_data.m_values[1],
            other1.m_data.m_values[2] * other2.m_data.m_values[2],
            other1.m_data.m_values[3] * other2.m_data.m_values[3],
            other1.m_data.m_values[4] * other2.m_data.m_values[4],
            other1.m_data.m_values[5] * other2.m_data.m_values[5],
            other1.m_data.m_values[6] * other2.m_data.m_values[6],
            other1.m_data.m_values[7] * other2.m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator*(const SIMD8<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD8<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_value,
            other1.m_data.m_values[1] * other2.m_data.m_value, other1.m_data.m_values[2] * other2.m_data.m_value,
            other1.m_data.m_values[3] * other2.m_data.m_value, other1.m_data.m_values[4] * other2.m_data.m_value,
            other1.m_data.m_values[5] * other2.m_data.m_value, other1.m_data.m_values[6] * other2.m_data.m_value,
            other1.m_data.m_values[7] * other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator*(const SIMD8<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
            other1.m_data.m_values[1] * other2.m_data.m_values[1],
            other1.m_data.m_values[2] * other2.m_data.m_values[0],
            other1.m_data.m_values[3] * other2.m_data.m_values[1],
            other1.m_data.m_values[4] * other2.m_data.m_values[0],
            other1.m_data.m_values[5] * other2.m_data.m_values[1],
            other1.m_data.m_values[6] * other2.m_data.m_values[0],
            other1.m_data.m_values[7] * other2.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator*(const SIMD8<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
            other1.m_data.m_values[1] * other2.m_data.m_values[1],
            other1.m_data.m_values[2] * other2.m_data.m_values[2],
            other1.m_data.m_values[3] * other2.m_data.m_values[3],
            other1.m_data.m_values[4] * other2.m_data.m_values[0],
            other1.m_data.m_values[5] * other2.m_data.m_values[1],
            other1.m_data.m_values[6] * other2.m_data.m_values[2],
            other1.m_data.m_values[7] * other2.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator/(const SIMD8<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
            other1.m_data.m_values[1] / other2.m_data.m_values[1],
            other1.m_data.m_values[2] / other2.m_data.m_values[2],
            other1.m_data.m_values[3] / other2.m_data.m_values[3],
            other1.m_data.m_values[4] / other2.m_data.m_values[4],
            other1.m_data.m_values[5] / other2.m_data.m_values[5],
            other1.m_data.m_values[6] / other2.m_data.m_values[6],
            other1.m_data.m_values[7] / other2.m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator/(const SIMD8<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD8<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD8<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_value,
                other1.m_data.m_values[1] / other2.m_data.m_value, other1.m_data.m_values[2] / other2.m_data.m_value,
                other1.m_data.m_values[3] / other2.m_data.m_value, other1.m_data.m_values[4] / other2.m_data.m_value,
                other1.m_data.m_values[5] / other2.m_data.m_value, other1.m_data.m_values[6] / other2.m_data.m_value,
                other1.m_data.m_values[7] / other2.m_data.m_value);
        } else {
            const T divisor = recip<T>(other2.m_data.m_value);
            return SIMD8<T, Width>(other1.m_data.m_values[0] * divisor, other1.m_data.m_values[1] * divisor,
                other1.m_data.m_values[2] * divisor, other1.m_data.m_values[3] * divisor,
                other1.m_data.m_values[4] * divisor, other1.m_data.m_values[5] * divisor,
                other1.m_data.m_values[5] * divisor, other1.m_data.m_values[6] * divisor);
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator/(const SIMDBase<T, Width2>& other1, const SIMD8<T, Width>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD8<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(other1.m_data.m_value / other2.m_data.m_values[0],
            other1.m_data.m_value / other2.m_data.m_values[1], other1.m_data.m_value / other2.m_data.m_values[2],
            other1.m_data.m_value / other2.m_data.m_values[3], other1.m_data.m_value / other2.m_data.m_values[4],
            other1.m_data.m_value / other2.m_data.m_values[5], other1.m_data.m_value / other2.m_data.m_values[6],
            other1.m_data.m_value / other2.m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator/(const SIMD8<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD8<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
                other1.m_data.m_values[1] / other2.m_data.m_values[1],
                other1.m_data.m_values[2] / other2.m_data.m_values[0],
                other1.m_data.m_values[3] / other2.m_data.m_values[1],
                other1.m_data.m_values[4] / other2.m_data.m_values[0],
                other1.m_data.m_values[5] / other2.m_data.m_values[1],
                other1.m_data.m_values[6] / other2.m_data.m_values[0],
                other1.m_data.m_values[7] / other2.m_data.m_values[1]);
        } else {
            const T divisor0 = recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = recip<T>(other2.m_data.m_values[1]);
            return SIMD8<T, Width>(other1.m_data.m_values[0] * divisor0, other1.m_data.m_values[1] * divisor1,
                other1.m_data.m_values[2] * divisor0, other1.m_data.m_values[3] * divisor1,
                other1.m_data.m_values[4] * divisor0, other1.m_data.m_values[5] * divisor1,
                other1.m_data.m_values[6] * divisor0, other1.m_data.m_values[7] * divisor1);
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator/(const SIMD8<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD8<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
                other1.m_data.m_values[1] / other2.m_data.m_values[1],
                other1.m_data.m_values[2] / other2.m_data.m_values[2],
                other1.m_data.m_values[3] / other2.m_data.m_values[3],
                other1.m_data.m_values[4] / other2.m_data.m_values[0],
                other1.m_data.m_values[5] / other2.m_data.m_values[1],
                other1.m_data.m_values[6] / other2.m_data.m_values[2],
                other1.m_data.m_values[7] / other2.m_data.m_values[3]);
        } else {
            const T divisor0 = recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = recip<T>(other2.m_data.m_values[1]);
            const T divisor2 = recip<T>(other2.m_data.m_values[2]);
            const T divisor3 = recip<T>(other2.m_data.m_values[3]);
            return SIMD8<T, Width>(other1.m_data.m_values[0] * divisor0, other1.m_data.m_values[1] * divisor1,
                other1.m_data.m_values[2] * divisor2, other1.m_data.m_values[3] * divisor3,
                other1.m_data.m_values[4] * divisor0, other1.m_data.m_values[5] * divisor1,
                other1.m_data.m_values[5] * divisor2, other1.m_data.m_values[6] * divisor3);
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator-(const SIMD8<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>(-other.m_data.m_values[0], -other.m_data.m_values[1], -other.m_data.m_values[2],
            -other.m_data.m_values[3], -other.m_data.m_values[4], -other.m_data.m_values[5], -other.m_data.m_values[6],
            -other.m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>& operator+=(SIMD8<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>& operator+=(SIMD8<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD8<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>& operator+=(SIMD8<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>& operator+=(SIMD8<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>& operator-=(SIMD8<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>& operator-=(SIMD8<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD8<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>& operator-=(SIMD8<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>& operator-=(SIMD8<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>& operator*=(SIMD8<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>& operator*=(SIMD8<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD8<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>& operator*=(SIMD8<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>& operator*=(SIMD8<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>& operator/=(SIMD8<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>& operator/=(SIMD8<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD8<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>& operator/=(SIMD8<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
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
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width>& operator/=(SIMD8<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
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
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator==(const SIMD8<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
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
            (other1.m_data.m_values[5] == other2.m_data.m_values[5]) &
            (other1.m_data.m_values[6] == other2.m_data.m_values[6]) &
            (other1.m_data.m_values[7] == other2.m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<=(const SIMD8<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
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
            (other1.m_data.m_values[5] <= other2.m_data.m_values[5]) &
            (other1.m_data.m_values[6] <= other2.m_data.m_values[6]) &
            (other1.m_data.m_values[7] <= other2.m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<(const SIMD8<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
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
            (other1.m_data.m_values[5] < other2.m_data.m_values[5]) &
            (other1.m_data.m_values[6] < other2.m_data.m_values[6]) &
            (other1.m_data.m_values[7] < other2.m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator!=(const SIMD8<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
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
            (other1.m_data.m_values[5] != other2.m_data.m_values[5]) &
            (other1.m_data.m_values[6] != other2.m_data.m_values[6]) &
            (other1.m_data.m_values[7] != other2.m_data.m_values[7]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator==(const SIMD8<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD8<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
            (other1.m_data.m_values[5] == other2.m_data.m_value) &
            (other1.m_data.m_values[6] == other2.m_data.m_value) &
            (other1.m_data.m_values[7] == other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<=(const SIMD8<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD8<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
            (other1.m_data.m_values[5] <= other2.m_data.m_value) &
            (other1.m_data.m_values[6] <= other2.m_data.m_value) &
            (other1.m_data.m_values[7] <= other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<(const SIMD8<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD8<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] < other2.m_data.m_value) &
            (other1.m_data.m_values[1] < other2.m_data.m_value) & (other1.m_data.m_values[2] < other2.m_data.m_value) &
            (other1.m_data.m_values[3] < other2.m_data.m_value) & (other1.m_data.m_values[4] < other2.m_data.m_value) &
            (other1.m_data.m_values[5] < other2.m_data.m_value) & (other1.m_data.m_values[6] < other2.m_data.m_value) &
            (other1.m_data.m_values[7] < other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator!=(const SIMD8<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD8<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
            (other1.m_data.m_values[5] != other2.m_data.m_value) &
            (other1.m_data.m_values[6] != other2.m_data.m_value) &
            (other1.m_data.m_values[7] != other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator&(const SIMD8<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD8<T, Width>(other1.m_data.m_values[0] & other2.m_data.m_values[0],
                other1.m_data.m_values[1] & other2.m_data.m_values[1],
                other1.m_data.m_values[2] & other2.m_data.m_values[2],
                other1.m_data.m_values[3] & other2.m_data.m_values[3],
                other1.m_data.m_values[4] & other2.m_data.m_values[4],
                other1.m_data.m_values[5] & other2.m_data.m_values[5],
                other1.m_data.m_values[6] & other2.m_data.m_values[6],
                other1.m_data.m_values[7] & other2.m_data.m_values[7]);
        } else {
            return SIMD8<T, Width>(Shift::bitAnd(other1.m_data.m_values[0], other2.m_data.m_values[0]),
                Shift::bitAnd(other1.m_data.m_values[1], other2.m_data.m_values[1]),
                Shift::bitAnd(other1.m_data.m_values[2], other2.m_data.m_values[2]),
                Shift::bitAnd(other1.m_data.m_values[3], other2.m_data.m_values[3]),
                Shift::bitAnd(other1.m_data.m_values[4], other2.m_data.m_values[4]),
                Shift::bitAnd(other1.m_data.m_values[5], other2.m_data.m_values[5]),
                Shift::bitAnd(other1.m_data.m_values[6], other2.m_data.m_values[6]),
                Shift::bitAnd(other1.m_data.m_values[7], other2.m_data.m_values[7]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator&(const SIMD8<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD8<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD8<T, Width>(other1.m_data.m_values[0] & other2.m_data.m_value,
                other1.m_data.m_values[1] & other2.m_data.m_value, other1.m_data.m_values[2] & other2.m_data.m_value,
                other1.m_data.m_values[3] & other2.m_data.m_value, other1.m_data.m_values[4] & other2.m_data.m_value,
                other1.m_data.m_values[5] & other2.m_data.m_value, other1.m_data.m_values[6] & other2.m_data.m_value,
                other1.m_data.m_values[7] & other2.m_data.m_value);
        } else {
            return SIMD8<T, Width>(Shift::bitAnd(other1.m_data.m_values[0], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[1], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[2], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[3], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[4], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[5], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[6], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[7], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator|(const SIMD8<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD8<T, Width>(other1.m_data.m_values[0] | other2.m_data.m_values[0],
                other1.m_data.m_values[1] | other2.m_data.m_values[1],
                other1.m_data.m_values[2] | other2.m_data.m_values[2],
                other1.m_data.m_values[3] | other2.m_data.m_values[3],
                other1.m_data.m_values[4] | other2.m_data.m_values[4],
                other1.m_data.m_values[5] | other2.m_data.m_values[5],
                other1.m_data.m_values[6] | other2.m_data.m_values[6],
                other1.m_data.m_values[7] | other2.m_data.m_values[7]);
        } else {
            return SIMD8<T, Width>(Shift::bitOr(other1.m_data.m_values[0], other2.m_data.m_values[0]),
                Shift::bitOr(other1.m_data.m_values[1], other2.m_data.m_values[1]),
                Shift::bitOr(other1.m_data.m_values[2], other2.m_data.m_values[2]),
                Shift::bitOr(other1.m_data.m_values[3], other2.m_data.m_values[3]),
                Shift::bitOr(other1.m_data.m_values[4], other2.m_data.m_values[4]),
                Shift::bitOr(other1.m_data.m_values[5], other2.m_data.m_values[5]),
                Shift::bitOr(other1.m_data.m_values[6], other2.m_data.m_values[6]),
                Shift::bitOr(other1.m_data.m_values[7], other2.m_data.m_values[7]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator|(const SIMD8<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD8<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD8<T, Width>(other1.m_data.m_values[0] | other2.m_data.m_value,
                other1.m_data.m_values[1] | other2.m_data.m_value, other1.m_data.m_values[2] | other2.m_data.m_value,
                other1.m_data.m_values[3] | other2.m_data.m_value, other1.m_data.m_values[4] | other2.m_data.m_value,
                other1.m_data.m_values[5] | other2.m_data.m_value, other1.m_data.m_values[6] | other2.m_data.m_value,
                other1.m_data.m_values[7] | other2.m_data.m_value);
        } else {
            return SIMD8<T, Width>(Shift::bitOr(other1.m_data.m_values[0], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[1], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[2], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[3], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[4], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[5], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[6], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[7], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator^(const SIMD8<T, Width>& other1, const SIMD8<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD8<T, Width>, SIMD8<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD8<T, Width>(other1.m_data.m_values[0] ^ other2.m_data.m_values[0],
                other1.m_data.m_values[1] ^ other2.m_data.m_values[1],
                other1.m_data.m_values[2] ^ other2.m_data.m_values[2],
                other1.m_data.m_values[3] ^ other2.m_data.m_values[3],
                other1.m_data.m_values[4] ^ other2.m_data.m_values[4],
                other1.m_data.m_values[5] ^ other2.m_data.m_values[5],
                other1.m_data.m_values[6] ^ other2.m_data.m_values[6],
                other1.m_data.m_values[7] ^ other2.m_data.m_values[7]);
        } else {
            return SIMD8<T, Width>(Shift::bitXor(other1.m_data.m_values[0], other2.m_data.m_values[0]),
                Shift::bitXor(other1.m_data.m_values[1], other2.m_data.m_values[1]),
                Shift::bitXor(other1.m_data.m_values[2], other2.m_data.m_values[2]),
                Shift::bitXor(other1.m_data.m_values[3], other2.m_data.m_values[3]),
                Shift::bitXor(other1.m_data.m_values[4], other2.m_data.m_values[4]),
                Shift::bitXor(other1.m_data.m_values[5], other2.m_data.m_values[5]),
                Shift::bitXor(other1.m_data.m_values[6], other2.m_data.m_values[6]),
                Shift::bitXor(other1.m_data.m_values[7], other2.m_data.m_values[7]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD8<T, Width> operator^(const SIMD8<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD8<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD8<T, Width>(other1.m_data.m_values[0] ^ other2.m_data.m_value,
                other1.m_data.m_values[1] ^ other2.m_data.m_value, other1.m_data.m_values[2] ^ other2.m_data.m_value,
                other1.m_data.m_values[3] ^ other2.m_data.m_value, other1.m_data.m_values[4] ^ other2.m_data.m_value,
                other1.m_data.m_values[5] ^ other2.m_data.m_value, other1.m_data.m_values[6] ^ other2.m_data.m_value,
                other1.m_data.m_values[7] ^ other2.m_data.m_value);
        } else {
            return SIMD8<T, Width>(Shift::bitXor(other1.m_data.m_values[0], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[1], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[2], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[3], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[4], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[5], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[6], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[7], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> operator~(const SIMD8<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD8<T, Width>(~other.m_data.m_values[0], ~other.m_data.m_values[1], ~other.m_data.m_values[2],
                ~other.m_data.m_values[3], ~other.m_data.m_values[4], ~other.m_data.m_values[5],
                ~other.m_data.m_values[6], ~other.m_data.m_values[7]);
        } else {
            return SIMD8<T, Width>(Shift::bitNot(other.m_data.m_values[0]), Shift::bitNot(other.m_data.m_values[1]),
                Shift::bitNot(other.m_data.m_values[2]), Shift::bitNot(other.m_data.m_values[3]),
                Shift::bitNot(other.m_data.m_values[4]), Shift::bitNot(other.m_data.m_values[5]),
                Shift::bitNot(other.m_data.m_values[6]), Shift::bitNot(other.m_data.m_values[7]));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask operator&(
    const typename SIMD8<T, Width>::Mask& mask1, const typename SIMD8<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>::Mask(mask1.m_data.m_values[0] & mask2.m_data.m_values[0],
            mask1.m_data.m_values[1] & mask2.m_data.m_values[1], mask1.m_data.m_values[2] & mask2.m_data.m_values[2],
            mask1.m_data.m_values[3] & mask2.m_data.m_values[3], mask1.m_data.m_values[4] & mask2.m_data.m_values[4],
            mask1.m_data.m_values[5] & mask2.m_data.m_values[5], mask1.m_data.m_values[6] & mask2.m_data.m_values[6],
            mask1.m_data.m_values[7] & mask2.m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask operator|(
    const typename SIMD8<T, Width>::Mask& mask1, const typename SIMD8<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>::Mask(mask1.m_data.m_values[0] | mask2.m_data.m_values[0],
            mask1.m_data.m_values[1] | mask2.m_data.m_values[1], mask1.m_data.m_values[2] | mask2.m_data.m_values[2],
            mask1.m_data.m_values[3] | mask2.m_data.m_values[3], mask1.m_data.m_values[4] | mask2.m_data.m_values[4],
            mask1.m_data.m_values[5] | mask2.m_data.m_values[5], mask1.m_data.m_values[6] | mask2.m_data.m_values[6],
            mask1.m_data.m_values[7] | mask2.m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask operator^(
    const typename SIMD8<T, Width>::Mask& mask1, const typename SIMD8<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>::Mask(mask1.m_data.m_values[0] ^ mask2.m_data.m_values[0],
            mask1.m_data.m_values[1] ^ mask2.m_data.m_values[1], mask1.m_data.m_values[2] ^ mask2.m_data.m_values[2],
            mask1.m_data.m_values[3] ^ mask2.m_data.m_values[3], mask1.m_data.m_values[4] ^ mask2.m_data.m_values[4],
            mask1.m_data.m_values[5] ^ mask2.m_data.m_values[5], mask1.m_data.m_values[6] ^ mask2.m_data.m_values[6],
            mask1.m_data.m_values[7] ^ mask2.m_data.m_values[7]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD8<T, Width>::Mask operator~(const typename SIMD8<T, Width>::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD8<T, Width>::Mask(~mask.m_data.m_values[0], ~mask.m_data.m_values[1], ~mask.m_data.m_values[2],
            ~mask.m_data.m_values[3], ~mask.m_data.m_values[4], ~mask.m_data.m_values[5], ~mask.m_data.m_values[6],
            ~mask.m_data.m_values[7]);
    }
}
} // namespace Shift
