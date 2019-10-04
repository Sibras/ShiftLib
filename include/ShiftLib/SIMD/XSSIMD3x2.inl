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

#include "SIMD/XSSIMD3x2.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#endif
#include "XSMath.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3x2Data<T>::SIMD3x2Data(const SIMD3x2<T, Width>& other) noexcept
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
XS_INLINE void SIMD3x2Data<T>::setData(
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
XS_INLINE void SIMD3x2Data<T>::store(const SIMD3x2<T, Width>& other) noexcept
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
XS_INLINE SIMD3x2<T, Width> SIMD3x2Data<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>(m_value0, m_value1, m_value2, m_value3, m_value4, m_value5);
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3x2DataPad<T>::SIMD3x2DataPad(const SIMD3x2<T, Width>& other) noexcept
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
XS_INLINE void SIMD3x2DataPad<T>::setData(
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
XS_INLINE void SIMD3x2DataPad<T>::store(const SIMD3x2<T, Width>& other) noexcept
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
XS_INLINE SIMD3x2<T, Width> SIMD3x2DataPad<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>(m_value0, m_value1, m_value2, m_value3, m_value4, m_value5);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>::Mask::Mask(const uint32 mask0, const uint32 mask1) noexcept
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
        m_data.m_values[3] = mask1 & 0x1;
        m_data.m_values[4] = mask1 & 0x2;
        m_data.m_values[5] = mask1 & 0x4;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>::Mask::Mask(const uint32 mask) noexcept
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
        m_data.m_values[3] = mask & 0x10;
        m_data.m_values[4] = mask & 0x20;
        m_data.m_values[5] = mask & 0x40;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>::Mask::Mask(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>::Mask::Mask(
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
XS_INLINE Bool3x2<hasSIMD<T> && (Width > SIMDWidth::Scalar)> SIMD3x2<T, Width>::Mask::getBool3x2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return Bool3x2(m_data.m_values[0], m_data.m_values[1], m_data.m_values[2], m_data.m_values[3],
            m_data.m_values[4], m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD3x2<T, Width>::Mask::getAny() const noexcept
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
XS_INLINE bool SIMD3x2<T, Width>::Mask::getAll() const noexcept
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
XS_INLINE bool SIMD3x2<T, Width>::Mask::getNone() const noexcept
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

template<typename T, SIMDWidth Width, uint32 Index>
class SIMDMasker3x2
{
public:
    XS_INLINE static SIMDInBase<T, Width>& ToType(SIMD3x2<T, Width>& other)
    {
        return reinterpret_cast<SIMDInBase<T, Width>*>(other.m_data.m_values)[Index];
    }
};

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD3x2<T, Width>::Mask::mask3x2Function(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if (m_data.m_values[0]) {
            SIMDInBase value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 0>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 0>>(value);
        }
        if (m_data.m_values[1]) {
            SIMDInBase value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 1>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 1>>(value);
        }
        if (m_data.m_values[2]) {
            SIMDInBase value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 2>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 2>>(value);
        }
        if (m_data.m_values[3]) {
            SIMDInBase value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 3>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 3>>(value);
        }
        if (m_data.m_values[4]) {
            SIMDInBase value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 4>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 4>>(value);
        }
        if (m_data.m_values[5]) {
            SIMDInBase value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 5>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 5>>(value);
        }
    }
}

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD3x2<T, Width>::Mask::mask3x2ElseFunction(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        auto value = (m_data.m_values[0]) ?
            maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 0>>() :
            maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 0>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 0>>(value);
        value = (m_data.m_values[1]) ?
            maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 1>>() :
            maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 1>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 1>>(value);
        value = (m_data.m_values[2]) ?
            maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 2>>() :
            maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 2>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 2>>(value);
        value = (m_data.m_values[3]) ?
            maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 3>>() :
            maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 3>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 3>>(value);
        value = (m_data.m_values[4]) ?
            maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 4>>() :
            maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 4>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 4>>(value);
        value = (m_data.m_values[5]) ?
            maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 5>>() :
            maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 5>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker3x2<T, Width, 5>>(value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>::SIMD3x2(
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
XS_INLINE SIMD3x2<T, Width>::SIMD3x2(const T value) noexcept
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
XS_INLINE SIMD3x2<T, Width>::SIMD3x2(const SIMDBase<T, Width2>& other) noexcept
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
XS_INLINE SIMD3x2<T, Width>::SIMD3x2(const SIMDInBase<T, Width2>& other) noexcept
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
XS_INLINE SIMD3x2<T, Width>::SIMD3x2(const SIMD2<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other.m_data.m_values[0];
        m_data.m_values[1] = other.m_data.m_values[0];
        m_data.m_values[2] = other.m_data.m_values[0];
        m_data.m_values[3] = other.m_data.m_values[1];
        m_data.m_values[4] = other.m_data.m_values[1];
        m_data.m_values[5] = other.m_data.m_values[1];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width>::SIMD3x2(const SIMD3<T, Width2>& other0, const SIMD3<T, Width2>& other1) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width>::SIMD3x2(const SIMD3<T, Width2>& other) noexcept
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
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width>::SIMD3x2(const SIMD6<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD6<T, Width>, SIMD6<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other.m_data.m_values[0];
        m_data.m_values[1] = other.m_data.m_values[2];
        m_data.m_values[2] = other.m_data.m_values[4];
        m_data.m_values[3] = other.m_data.m_values[1];
        m_data.m_values[4] = other.m_data.m_values[3];
        m_data.m_values[5] = other.m_data.m_values[5];
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>::SIMD3x2(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(T{1});
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width2> SIMD3x2<T, Width>::getValue3() const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>(
            m_data.m_values[Index * 3], m_data.m_values[Index * 3 + 1], m_data.m_values[Index * 3 + 2]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD3x2<T, Width>::setValue3(const SIMD3<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
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
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::negate() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Elem0 ? -m_data.m_values[0] : m_data.m_values[0],
            Elem1 ? -m_data.m_values[1] : m_data.m_values[1], Elem2 ? -m_data.m_values[2] : m_data.m_values[2],
            Elem3 ? -m_data.m_values[3] : m_data.m_values[3], Elem4 ? -m_data.m_values[4] : m_data.m_values[4],
            Elem5 ? -m_data.m_values[5] : m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::mad(
    const SIMD3x2<T, Width2>& other1, const SIMD3x2<T, Width2>& other2) const noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[4], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[5], other2.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::mad(
    const SIMDBase<T, Width2>& other1, const SIMD3x2<T, Width>& other2) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_value, other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_value, other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_value, other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_value, other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_value, other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_value, other2.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::mad(
    const SIMD2<T, Width2>& other1, const SIMD3x2<T, Width>& other2) const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[0], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[0], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[1], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[1], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[1], other2.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::mad(
    const SIMD3<T, Width2>& other1, const SIMD3x2<T, Width>& other2) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[0], other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[1], other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[2], other2.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::mad(
    const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[4], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[5], other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::mad(const SIMD3<T, Width2>& other1, const SIMD3<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[2], other2.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::msub(
    const SIMD3x2<T, Width2>& other1, const SIMD3x2<T, Width2>& other2) const noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], -other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], -other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], -other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], -other2.m_data.m_values[3]),
            Shift::fma<T>(m_data.m_values[4], other1.m_data.m_values[4], -other2.m_data.m_values[4]),
            Shift::fma<T>(m_data.m_values[5], other1.m_data.m_values[5], -other2.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::equalMask(const SIMD3x2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
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
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::lessOrEqualMask(const SIMD3x2<T, Width2>& other) const
    noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
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
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::lessThanMask(const SIMD3x2<T, Width2>& other) const
    noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
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
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::notEqualMask(const SIMD3x2<T, Width2>& other) const
    noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
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
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::equalMask(const SIMDBase<T, Width2>& other) const noexcept
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
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::lessOrEqualMask(const SIMDBase<T, Width2>& other) const
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
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::lessThanMask(const SIMDBase<T, Width2>& other) const
    noexcept
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
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::greaterOrEqualMask(const SIMDBase<T, Width2>& other) const
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
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::greaterThanMask(const SIMDBase<T, Width2>& other) const
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
XS_INLINE typename SIMD3x2<T, Width>::Mask SIMD3x2<T, Width>::notEqualMask(const SIMDBase<T, Width2>& other) const
    noexcept
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
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::sign(const SIMD3x2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::sign<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::sign<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::sign<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::sign<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::sign<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::sign<T>(m_data.m_values[5], other.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::abs<T>(m_data.m_values[0]), Shift::abs<T>(m_data.m_values[1]),
            Shift::abs<T>(m_data.m_values[2]), Shift::abs<T>(m_data.m_values[3]), Shift::abs<T>(m_data.m_values[4]),
            Shift::abs<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::max(const SIMD3x2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::max<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::max<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::max<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::max<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::max<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::max<T>(m_data.m_values[5], other.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::min(const SIMD3x2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::min<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::min<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::min<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::min<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::min<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::min<T>(m_data.m_values[5], other.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::max(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::max<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[3], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[4], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[5], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::min(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::min<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[3], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[4], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[5], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3x2<T, Width>::max3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>(Shift::max<T>(m_data.m_values[0], m_data.m_values[3]),
            Shift::max<T>(m_data.m_values[1], m_data.m_values[4]),
            Shift::max<T>(m_data.m_values[2], m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3x2<T, Width>::min3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>(Shift::min<T>(m_data.m_values[0], m_data.m_values[3]),
            Shift::min<T>(m_data.m_values[1], m_data.m_values[4]),
            Shift::min<T>(m_data.m_values[2], m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD3x2<T, Width>::hmax3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T other1 = Shift::max<T>(m_data.m_values[0], m_data.m_values[1]);
        const T other2 = Shift::max<T>(m_data.m_values[3], m_data.m_values[4]);
        return SIMD2<T, Width>(Shift::max<T>(m_data.m_values[2], other1), Shift::max<T>(m_data.m_values[5], other2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD3x2<T, Width>::hmin3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T other1 = Shift::min<T>(m_data.m_values[0], m_data.m_values[1]);
        const T other2 = Shift::min<T>(m_data.m_values[3], m_data.m_values[4]);
        return SIMD2<T, Width>(Shift::min<T>(m_data.m_values[2], other1), Shift::min<T>(m_data.m_values[5], other2));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::reciprocal() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::recip<T>(m_data.m_values[0]), Shift::recip<T>(m_data.m_values[1]),
            Shift::recip<T>(m_data.m_values[2]), Shift::recip<T>(m_data.m_values[3]),
            Shift::recip<T>(m_data.m_values[4]), Shift::recip<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3x2<T, Width>::add3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T res1 = (m_data.m_values[0] + m_data.m_values[3]);
        const T res2 = (m_data.m_values[1] + m_data.m_values[4]);
        const T res3 = (m_data.m_values[2] + m_data.m_values[5]);
        return SIMD3<T, Width>(res1, res2, res3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3x2<T, Width>::sub3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T res1 = (m_data.m_values[0] - m_data.m_values[3]);
        const T res2 = (m_data.m_values[1] - m_data.m_values[4]);
        const T res3 = (m_data.m_values[2] - m_data.m_values[5]);
        return SIMD3<T, Width>(res1, res2, res3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD3x2<T, Width>::hadd3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T res = (m_data.m_values[0] + m_data.m_values[1]);
        T res2 = (m_data.m_values[3] + m_data.m_values[4]);
        res += m_data.m_values[2];
        res2 += m_data.m_values[5];
        return SIMD2<T, Width>(res, res2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::ceil<T>(m_data.m_values[0]), Shift::ceil<T>(m_data.m_values[1]),
            Shift::ceil<T>(m_data.m_values[2]), Shift::ceil<T>(m_data.m_values[3]), Shift::ceil<T>(m_data.m_values[4]),
            Shift::ceil<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::floor<T>(m_data.m_values[0]), Shift::floor<T>(m_data.m_values[1]),
            Shift::floor<T>(m_data.m_values[2]), Shift::floor<T>(m_data.m_values[3]),
            Shift::floor<T>(m_data.m_values[4]), Shift::floor<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::trunc<T>(m_data.m_values[0]), Shift::trunc<T>(m_data.m_values[1]),
            Shift::trunc<T>(m_data.m_values[2]), Shift::trunc<T>(m_data.m_values[3]),
            Shift::trunc<T>(m_data.m_values[4]), Shift::trunc<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::sqrt<T>(m_data.m_values[0]), Shift::sqrt<T>(m_data.m_values[1]),
            Shift::sqrt<T>(m_data.m_values[2]), Shift::sqrt<T>(m_data.m_values[3]), Shift::sqrt<T>(m_data.m_values[4]),
            Shift::sqrt<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::rsqrt<T>(m_data.m_values[0]), Shift::rsqrt<T>(m_data.m_values[1]),
            Shift::rsqrt<T>(m_data.m_values[2]), Shift::rsqrt<T>(m_data.m_values[3]),
            Shift::rsqrt<T>(m_data.m_values[4]), Shift::rsqrt<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD3x2<T, Width>::dot3(const SIMD3x2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        T sq0 = m_data.m_values[0] * other.m_data.m_values[0];
        T sq1 = m_data.m_values[3] * other.m_data.m_values[3];
        sq0 = Shift::fma<T>(m_data.m_values[1], other.m_data.m_values[1], sq0);
        sq1 = Shift::fma<T>(m_data.m_values[4], other.m_data.m_values[4], sq1);
        return SIMD2<T, Width>(Shift::fma<T>(m_data.m_values[2], other.m_data.m_values[2], sq0),
            Shift::fma<T>(m_data.m_values[5], other.m_data.m_values[5], sq1));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::cross3(const SIMD3x2<T, Width2>& other) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const T v0CrossX = -(m_data.m_values[2] * other.m_data.m_values[1]);
        const T v1CrossX = -(m_data.m_values[5] * other.m_data.m_values[4]);
        const T v0CrossY = -(m_data.m_values[0] * other.m_data.m_values[2]);
        const T v1CrossY = -(m_data.m_values[3] * other.m_data.m_values[5]);
        const T v0CrossZ = -(m_data.m_values[1] * other.m_data.m_values[0]);
        const T v1CrossZ = -(m_data.m_values[4] * other.m_data.m_values[3]);

        return SIMD3x2(Shift::fma<T>(m_data.m_values[1], other.m_data.m_values[2], v0CrossX),
            Shift::fma<T>(m_data.m_values[2], other.m_data.m_values[0], v0CrossY),
            Shift::fma<T>(m_data.m_values[0], other.m_data.m_values[1], v0CrossZ),
            Shift::fma<T>(m_data.m_values[4], other.m_data.m_values[5], v1CrossX),
            Shift::fma<T>(m_data.m_values[5], other.m_data.m_values[3], v1CrossY),
            Shift::fma<T>(m_data.m_values[3], other.m_data.m_values[4], v1CrossZ));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD3x2<T, Width>::lengthSqr3() const noexcept
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
XS_INLINE SIMD2<T, Width> SIMD3x2<T, Width>::length3() const noexcept
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
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::normalize3() const noexcept
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
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::exp2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::exp2<T>(m_data.m_values[0]), Shift::exp2<T>(m_data.m_values[1]),
            Shift::exp2<T>(m_data.m_values[2]), Shift::exp2<T>(m_data.m_values[3]), Shift::exp2<T>(m_data.m_values[4]),
            Shift::exp2<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::exp() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::exp<T>(m_data.m_values[0]), Shift::exp<T>(m_data.m_values[1]),
            Shift::exp<T>(m_data.m_values[2]), Shift::exp<T>(m_data.m_values[3]), Shift::exp<T>(m_data.m_values[4]),
            Shift::exp<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::log2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::log2<T>(m_data.m_values[0]), Shift::log2<T>(m_data.m_values[1]),
            Shift::log2<T>(m_data.m_values[2]), Shift::log2<T>(m_data.m_values[3]), Shift::log2<T>(m_data.m_values[4]),
            Shift::log2<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::log() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::log<T>(m_data.m_values[0]), Shift::log<T>(m_data.m_values[1]),
            Shift::log<T>(m_data.m_values[2]), Shift::log<T>(m_data.m_values[3]), Shift::log<T>(m_data.m_values[4]),
            Shift::log<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::pow(const SIMD3x2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::pow<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::pow<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::pow<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::pow<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::pow<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::pow<T>(m_data.m_values[5], other.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::powr(const SIMD3x2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::powr<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::powr<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::powr<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::powr<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::powr<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::powr<T>(m_data.m_values[5], other.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::pow(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::pow<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[3], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[4], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[5], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::powr(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::powr<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[3], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[4], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[5], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::sin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::sin<T>(m_data.m_values[0]), Shift::sin<T>(m_data.m_values[1]),
            Shift::sin<T>(m_data.m_values[2]), Shift::sin<T>(m_data.m_values[3]), Shift::sin<T>(m_data.m_values[4]),
            Shift::sin<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::cos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::cos<T>(m_data.m_values[0]), Shift::cos<T>(m_data.m_values[1]),
            Shift::cos<T>(m_data.m_values[2]), Shift::cos<T>(m_data.m_values[3]), Shift::cos<T>(m_data.m_values[4]),
            Shift::cos<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::tan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::tan<T>(m_data.m_values[0]), Shift::tan<T>(m_data.m_values[1]),
            Shift::tan<T>(m_data.m_values[2]), Shift::tan<T>(m_data.m_values[3]), Shift::tan<T>(m_data.m_values[4]),
            Shift::tan<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x4<T, Width> SIMD3x2<T, Width>::sincos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        SIMD3x4<T, Width> ret;
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
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::asin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::asin<T>(m_data.m_values[0]), Shift::asin<T>(m_data.m_values[1]),
            Shift::asin<T>(m_data.m_values[2]), Shift::asin<T>(m_data.m_values[3]), Shift::asin<T>(m_data.m_values[4]),
            Shift::asin<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::acos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::acos<T>(m_data.m_values[0]), Shift::acos<T>(m_data.m_values[1]),
            Shift::acos<T>(m_data.m_values[2]), Shift::acos<T>(m_data.m_values[3]), Shift::acos<T>(m_data.m_values[4]),
            Shift::acos<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::atan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::atan<T>(m_data.m_values[0]), Shift::atan<T>(m_data.m_values[1]),
            Shift::atan<T>(m_data.m_values[2]), Shift::atan<T>(m_data.m_values[3]), Shift::atan<T>(m_data.m_values[4]),
            Shift::atan<T>(m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::atan2(const SIMD3x2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(Shift::atan2<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::atan2<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::atan2<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::atan2<T>(m_data.m_values[3], other.m_data.m_values[3]),
            Shift::atan2<T>(m_data.m_values[4], other.m_data.m_values[4]),
            Shift::atan2<T>(m_data.m_values[5], other.m_data.m_values[5]));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::insert3(const SIMD3x2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
    static_assert(Index0 < 3 && Index1 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index0 * 3] = other.m_data.m_values[Index1 * 3];
        m_data.m_values[Index0 * 3 + 1] = other.m_data.m_values[Index1 * 3 + 1];
        m_data.m_values[Index0 * 3 + 2] = other.m_data.m_values[Index1 * 3 + 2];
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::blend3(const SIMD3x2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        const auto temp0 = (Elem0) ? SIMD2<T, Width>(other.m_data.m_values[0], other.m_data.m_values[3]) :
                                     SIMD2<T, Width>(m_data.m_values[0], m_data.m_values[3]);
        const auto temp1 = (Elem1) ? SIMD2<T, Width>(other.m_data.m_values[1], other.m_data.m_values[4]) :
                                     SIMD2<T, Width>(m_data.m_values[1], m_data.m_values[4]);
        const auto temp2 = (Elem2) ? SIMD2<T, Width>(other.m_data.m_values[2], other.m_data.m_values[5]) :
                                     SIMD2<T, Width>(m_data.m_values[2], m_data.m_values[5]);
        return SIMD3x2(temp0.m_data.m_values[0], temp1.m_data.m_values[0], temp2.m_data.m_values[0],
            temp0.m_data.m_values[1], temp1.m_data.m_values[1], temp2.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::shuffle3() const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(m_data.m_values[Index0], m_data.m_values[Index0], m_data.m_values[Index2],
            m_data.m_values[Index0 + 3], m_data.m_values[Index1 + 3], m_data.m_values[Index2 + 3]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD8<T, Width> SIMD3x2<T, Width>::shuffle3() const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3 && Index3 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(m_data.m_values[Index0], m_data.m_values[Index0], m_data.m_values[Index2],
            m_data.m_values[Index3], m_data.m_values[Index0 + 3], m_data.m_values[Index1 + 3],
            m_data.m_values[Index2 + 3], m_data.m_values[Index3 + 3]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD3x2<T, Width> SIMD3x2<T, Width>::shuffleH3() const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2(getValue3<Index0>(), getValue3<Index1>());
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator+(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
            other1.m_data.m_values[1] + other2.m_data.m_values[1],
            other1.m_data.m_values[2] + other2.m_data.m_values[2],
            other1.m_data.m_values[3] + other2.m_data.m_values[3],
            other1.m_data.m_values[4] + other2.m_data.m_values[4],
            other1.m_data.m_values[5] + other2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator+(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_value,
            other1.m_data.m_values[1] + other2.m_data.m_value, other1.m_data.m_values[2] + other2.m_data.m_value,
            other1.m_data.m_values[3] + other2.m_data.m_value, other1.m_data.m_values[4] + other2.m_data.m_value,
            other1.m_data.m_values[5] + other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator+(const SIMD3x2<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.m_data.m_values[0] + other2.m_data.m_values[0],
            other1.m_data.m_values[1] + other2.m_data.m_values[1],
            other1.m_data.m_values[2] + other2.m_data.m_values[2],
            other1.m_data.m_values[3] + other2.m_data.m_values[0],
            other1.m_data.m_values[4] + other2.m_data.m_values[1],
            other1.m_data.m_values[5] + other2.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator-(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
            other1.m_data.m_values[1] - other2.m_data.m_values[1],
            other1.m_data.m_values[2] - other2.m_data.m_values[2],
            other1.m_data.m_values[3] - other2.m_data.m_values[3],
            other1.m_data.m_values[4] - other2.m_data.m_values[4],
            other1.m_data.m_values[5] - other2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator-(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_value,
            other1.m_data.m_values[1] - other2.m_data.m_value, other1.m_data.m_values[2] - other2.m_data.m_value,
            other1.m_data.m_values[3] - other2.m_data.m_value, other1.m_data.m_values[4] - other2.m_data.m_value,
            other1.m_data.m_values[5] - other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator-(const SIMDBase<T, Width2>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.m_data.m_value - other2.m_data.m_values[0],
            other1.m_data.m_value - other2.m_data.m_values[1], other1.m_data.m_value - other2.m_data.m_values[2],
            other1.m_data.m_value - other2.m_data.m_values[3], other1.m_data.m_value - other2.m_data.m_values[4],
            other1.m_data.m_value - other2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator-(const SIMD3x2<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.m_data.m_values[0] - other2.m_data.m_values[0],
            other1.m_data.m_values[1] - other2.m_data.m_values[1],
            other1.m_data.m_values[2] - other2.m_data.m_values[2],
            other1.m_data.m_values[3] - other2.m_data.m_values[0],
            other1.m_data.m_values[4] - other2.m_data.m_values[1],
            other1.m_data.m_values[5] - other2.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator*(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
            other1.m_data.m_values[1] * other2.m_data.m_values[1],
            other1.m_data.m_values[2] * other2.m_data.m_values[2],
            other1.m_data.m_values[3] * other2.m_data.m_values[3],
            other1.m_data.m_values[4] * other2.m_data.m_values[4],
            other1.m_data.m_values[5] * other2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator*(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_value,
            other1.m_data.m_values[1] * other2.m_data.m_value, other1.m_data.m_values[2] * other2.m_data.m_value,
            other1.m_data.m_values[3] * other2.m_data.m_value, other1.m_data.m_values[4] * other2.m_data.m_value,
            other1.m_data.m_values[5] * other2.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator*(const SIMD3x2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
            other1.m_data.m_values[1] * other2.m_data.m_values[0],
            other1.m_data.m_values[2] * other2.m_data.m_values[0],
            other1.m_data.m_values[3] * other2.m_data.m_values[1],
            other1.m_data.m_values[4] * other2.m_data.m_values[1],
            other1.m_data.m_values[5] * other2.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator*(const SIMD3x2<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.m_data.m_values[0] * other2.m_data.m_values[0],
            other1.m_data.m_values[1] * other2.m_data.m_values[1],
            other1.m_data.m_values[2] * other2.m_data.m_values[2],
            other1.m_data.m_values[3] * other2.m_data.m_values[0],
            other1.m_data.m_values[4] * other2.m_data.m_values[1],
            other1.m_data.m_values[5] * other2.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator/(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
            other1.m_data.m_values[1] / other2.m_data.m_values[1],
            other1.m_data.m_values[2] / other2.m_data.m_values[2],
            other1.m_data.m_values[3] / other2.m_data.m_values[3],
            other1.m_data.m_values[4] / other2.m_data.m_values[4],
            other1.m_data.m_values[5] / other2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator/(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD3x2<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_value,
                other1.m_data.m_values[1] / other2.m_data.m_value, other1.m_data.m_values[2] / other2.m_data.m_value,
                other1.m_data.m_values[3] / other2.m_data.m_value, other1.m_data.m_values[4] / other2.m_data.m_value,
                other1.m_data.m_values[5] / other2.m_data.m_value);
        } else {
            const T divisor = recip<T>(other2.m_data.m_value);
            return SIMD3x2<T, Width>(other1.m_data.m_values[0] * divisor, other1.m_data.m_values[1] * divisor,
                other1.m_data.m_values[2] * divisor, other1.m_data.m_values[3] * divisor,
                other1.m_data.m_values[4] * divisor, other1.m_data.m_values[5] * divisor);
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator/(const SIMD3x2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD3x2<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
                other1.m_data.m_values[1] / other2.m_data.m_values[0],
                other1.m_data.m_values[2] / other2.m_data.m_values[0],
                other1.m_data.m_values[3] / other2.m_data.m_values[1],
                other1.m_data.m_values[4] / other2.m_data.m_values[1],
                other1.m_data.m_values[5] / other2.m_data.m_values[1]);
        } else {
            const T divisor0 = recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = recip<T>(other2.m_data.m_values[1]);
            return SIMD3x2<T, Width>(other1.m_data.m_values[0] * divisor0, other1.m_data.m_values[1] * divisor0,
                other1.m_data.m_values[2] * divisor0, other1.m_data.m_values[3] * divisor1,
                other1.m_data.m_values[4] * divisor1, other1.m_data.m_values[5] * divisor1);
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator/(const SIMDBase<T, Width2>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.m_data.m_value / other2.m_data.m_values[0],
            other1.m_data.m_value / other2.m_data.m_values[1], other1.m_data.m_value / other2.m_data.m_values[2],
            other1.m_data.m_value / other2.m_data.m_values[3], other1.m_data.m_value / other2.m_data.m_values[4],
            other1.m_data.m_value / other2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator/(const SIMD3x2<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD3x2<T, Width>(other1.m_data.m_values[0] / other2.m_data.m_values[0],
                other1.m_data.m_values[1] / other2.m_data.m_values[1],
                other1.m_data.m_values[2] / other2.m_data.m_values[2],
                other1.m_data.m_values[3] / other2.m_data.m_values[0],
                other1.m_data.m_values[4] / other2.m_data.m_values[1],
                other1.m_data.m_values[5] / other2.m_data.m_values[2]);
        } else {
            const T divisor0 = recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = recip<T>(other2.m_data.m_values[1]);
            const T divisor2 = recip<T>(other2.m_data.m_values[2]);
            return SIMD3x2<T, Width>(other1.m_data.m_values[0] * divisor0, other1.m_data.m_values[1] * divisor1,
                other1.m_data.m_values[2] * divisor2, other1.m_data.m_values[3] * divisor0,
                other1.m_data.m_values[4] * divisor1, other1.m_data.m_values[5] * divisor2);
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator-(const SIMD3x2<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>(-other.m_data.m_values[0], -other.m_data.m_values[1], -other.m_data.m_values[2],
            -other.m_data.m_values[3], -other.m_data.m_values[4], -other.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width>& operator+=(SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width>& operator+=(SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
XS_INLINE SIMD3x2<T, Width>& operator+=(SIMD3x2<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width>& operator-=(SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width>& operator-=(SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
XS_INLINE SIMD3x2<T, Width>& operator-=(SIMD3x2<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width>& operator*=(SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width>& operator*=(SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
XS_INLINE SIMD3x2<T, Width>& operator*=(SIMD3x2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width>& operator*=(SIMD3x2<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
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
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width>& operator/=(SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
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
XS_INLINE SIMD3x2<T, Width>& operator/=(SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
XS_INLINE SIMD3x2<T, Width>& operator/=(SIMD3x2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
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
            other1.m_data.m_values[1] /= other2.m_data.m_values[0];
            other1.m_data.m_values[2] /= other2.m_data.m_values[0];
            other1.m_data.m_values[3] /= other2.m_data.m_values[1];
            other1.m_data.m_values[4] /= other2.m_data.m_values[1];
            other1.m_data.m_values[5] /= other2.m_data.m_values[1];
        } else {
            const T divisor0 = Shift::recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = Shift::recip<T>(other2.m_data.m_values[1]);
            other1.m_data.m_values[0] *= divisor0;
            other1.m_data.m_values[1] *= divisor0;
            other1.m_data.m_values[2] *= divisor0;
            other1.m_data.m_values[3] *= divisor1;
            other1.m_data.m_values[4] *= divisor1;
            other1.m_data.m_values[5] *= divisor1;
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width>& operator/=(SIMD3x2<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
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
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator==(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
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
XS_INLINE bool operator<=(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
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
XS_INLINE bool operator<(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
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
XS_INLINE bool operator!=(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
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
XS_INLINE bool operator==(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
XS_INLINE bool operator<=(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
XS_INLINE bool operator<(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
XS_INLINE bool operator!=(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
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
XS_INLINE SIMD3x2<T, Width> operator&(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(other1.m_data.m_values[0] & other2.m_data.m_values[0],
                other1.m_data.m_values[1] & other2.m_data.m_values[1],
                other1.m_data.m_values[2] & other2.m_data.m_values[2],
                other1.m_data.m_values[3] & other2.m_data.m_values[3],
                other1.m_data.m_values[4] & other2.m_data.m_values[4],
                other1.m_data.m_values[5] & other2.m_data.m_values[5]);
        } else {
            return SIMD3x2<T, Width>(Shift::bitAnd(other1.m_data.m_values[0], other2.m_data.m_values[0]),
                Shift::bitAnd(other1.m_data.m_values[1], other2.m_data.m_values[1]),
                Shift::bitAnd(other1.m_data.m_values[2], other2.m_data.m_values[2]),
                Shift::bitAnd(other1.m_data.m_values[3], other2.m_data.m_values[3]),
                Shift::bitAnd(other1.m_data.m_values[4], other2.m_data.m_values[4]),
                Shift::bitAnd(other1.m_data.m_values[5], other2.m_data.m_values[5]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator&(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(other1.m_data.m_values[0] & other2.m_data.m_value,
                other1.m_data.m_values[1] & other2.m_data.m_value, other1.m_data.m_values[2] & other2.m_data.m_value,
                other1.m_data.m_values[3] & other2.m_data.m_value, other1.m_data.m_values[4] & other2.m_data.m_value,
                other1.m_data.m_values[5] & other2.m_data.m_value);
        } else {
            return SIMD3x2<T, Width>(Shift::bitAnd(other1.m_data.m_values[0], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[1], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[2], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[3], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[4], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[5], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator|(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(other1.m_data.m_values[0] | other2.m_data.m_values[0],
                other1.m_data.m_values[1] | other2.m_data.m_values[1],
                other1.m_data.m_values[2] | other2.m_data.m_values[2],
                other1.m_data.m_values[3] | other2.m_data.m_values[3],
                other1.m_data.m_values[4] | other2.m_data.m_values[4],
                other1.m_data.m_values[5] | other2.m_data.m_values[5]);
        } else {
            return SIMD3x2<T, Width>(Shift::bitOr(other1.m_data.m_values[0], other2.m_data.m_values[0]),
                Shift::bitOr(other1.m_data.m_values[1], other2.m_data.m_values[1]),
                Shift::bitOr(other1.m_data.m_values[2], other2.m_data.m_values[2]),
                Shift::bitOr(other1.m_data.m_values[3], other2.m_data.m_values[3]),
                Shift::bitOr(other1.m_data.m_values[4], other2.m_data.m_values[4]),
                Shift::bitOr(other1.m_data.m_values[5], other2.m_data.m_values[5]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator|(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(other1.m_data.m_values[0] | other2.m_data.m_value,
                other1.m_data.m_values[1] | other2.m_data.m_value, other1.m_data.m_values[2] | other2.m_data.m_value,
                other1.m_data.m_values[3] | other2.m_data.m_value, other1.m_data.m_values[4] | other2.m_data.m_value,
                other1.m_data.m_values[5] | other2.m_data.m_value);
        } else {
            return SIMD3x2<T, Width>(Shift::bitOr(other1.m_data.m_values[0], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[1], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[2], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[3], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[4], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[5], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator^(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3x2<T, Width>, SIMD3x2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(other1.m_data.m_values[0] ^ other2.m_data.m_values[0],
                other1.m_data.m_values[1] ^ other2.m_data.m_values[1],
                other1.m_data.m_values[2] ^ other2.m_data.m_values[2],
                other1.m_data.m_values[3] ^ other2.m_data.m_values[3],
                other1.m_data.m_values[4] ^ other2.m_data.m_values[4],
                other1.m_data.m_values[5] ^ other2.m_data.m_values[5]);
        } else {
            return SIMD3x2<T, Width>(Shift::bitXor(other1.m_data.m_values[0], other2.m_data.m_values[0]),
                Shift::bitXor(other1.m_data.m_values[1], other2.m_data.m_values[1]),
                Shift::bitXor(other1.m_data.m_values[2], other2.m_data.m_values[2]),
                Shift::bitXor(other1.m_data.m_values[3], other2.m_data.m_values[3]),
                Shift::bitXor(other1.m_data.m_values[4], other2.m_data.m_values[4]),
                Shift::bitXor(other1.m_data.m_values[5], other2.m_data.m_values[5]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3x2<T, Width> operator^(const SIMD3x2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD3x2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(other1.m_data.m_values[0] ^ other2.m_data.m_value,
                other1.m_data.m_values[1] ^ other2.m_data.m_value, other1.m_data.m_values[2] ^ other2.m_data.m_value,
                other1.m_data.m_values[3] ^ other2.m_data.m_value, other1.m_data.m_values[4] ^ other2.m_data.m_value,
                other1.m_data.m_values[5] ^ other2.m_data.m_value);
        } else {
            return SIMD3x2<T, Width>(Shift::bitXor(other1.m_data.m_values[0], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[1], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[2], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[3], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[4], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[5], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator~(const SIMD3x2<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(~other.m_data.m_values[0], ~other.m_data.m_values[1], ~other.m_data.m_values[2],
                ~other.m_data.m_values[3], ~other.m_data.m_values[4], ~other.m_data.m_values[5]);
        } else {
            return SIMD3x2<T, Width>(Shift::bitNot(other.m_data.m_values[0]), Shift::bitNot(other.m_data.m_values[1]),
                Shift::bitNot(other.m_data.m_values[2]), Shift::bitNot(other.m_data.m_values[3]),
                Shift::bitNot(other.m_data.m_values[4]), Shift::bitNot(other.m_data.m_values[5]));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask operator&(
    const typename SIMD3x2<T, Width>::Mask& mask1, const typename SIMD3x2<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>::Mask(mask1.m_data.m_values[0] & mask2.m_data.m_values[0],
            mask1.m_data.m_values[1] & mask2.m_data.m_values[1], mask1.m_data.m_values[2] & mask2.m_data.m_values[2],
            mask1.m_data.m_values[3] & mask2.m_data.m_values[3], mask1.m_data.m_values[4] & mask2.m_data.m_values[4],
            mask1.m_data.m_values[5] & mask2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask operator|(
    const typename SIMD3x2<T, Width>::Mask& mask1, const typename SIMD3x2<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>::Mask(mask1.m_data.m_values[0] | mask2.m_data.m_values[0],
            mask1.m_data.m_values[1] | mask2.m_data.m_values[1], mask1.m_data.m_values[2] | mask2.m_data.m_values[2],
            mask1.m_data.m_values[3] | mask2.m_data.m_values[3], mask1.m_data.m_values[4] | mask2.m_data.m_values[4],
            mask1.m_data.m_values[5] | mask2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask operator^(
    const typename SIMD3x2<T, Width>::Mask& mask1, const typename SIMD3x2<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>::Mask(mask1.m_data.m_values[0] ^ mask2.m_data.m_values[0],
            mask1.m_data.m_values[1] ^ mask2.m_data.m_values[1], mask1.m_data.m_values[2] ^ mask2.m_data.m_values[2],
            mask1.m_data.m_values[3] ^ mask2.m_data.m_values[3], mask1.m_data.m_values[4] ^ mask2.m_data.m_values[4],
            mask1.m_data.m_values[5] ^ mask2.m_data.m_values[5]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask operator~(const typename SIMD3x2<T, Width>::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3x2<T, Width>::Mask(~mask.m_data.m_values[0], ~mask.m_data.m_values[1], ~mask.m_data.m_values[2],
            ~mask.m_data.m_values[3], ~mask.m_data.m_values[4], ~mask.m_data.m_values[5]);
    }
}
} // namespace Shift
