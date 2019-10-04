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

#include "SIMD/XSSIMD2.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#endif
#include "XSMath.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD2Data<T>::SIMD2Data(const SIMD2<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_value0 = other.m_data.m_values[0];
        m_value1 = other.m_data.m_values[1];
    }
}

template<typename T>
XS_INLINE void SIMD2Data<T>::setData(T other0, T other1) noexcept
{
    m_value0 = other0;
    m_value1 = other1;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD2Data<T>::store(const SIMD2<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_value0 = other.m_data.m_values[0];
        m_value1 = other.m_data.m_values[1];
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2Data<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(m_value0, m_value1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>::Mask::Mask(const uint32 mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = mask & 0x1;
        m_data.m_values[1] = mask & 0x2;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>::Mask::Mask(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>::Mask::Mask(bool bool0, bool bool1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = bool0;
        m_data.m_values[1] = bool1;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE Bool2<hasSIMD<T> && (Width > SIMDWidth::Scalar)> SIMD2<T, Width>::Mask::getBool2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Bool2<Width>(m_data.m_values[0], m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD2<T, Width>::Mask::getAny() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return m_data.m_values[0] | m_data.m_values[1];
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD2<T, Width>::Mask::getAll() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return m_data.m_values[0] & m_data.m_values[1];
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD2<T, Width>::Mask::getNone() const noexcept
{
    return !getAny();
}

template<typename T, SIMDWidth Width, uint32 Index>
class Masker2
{
public:
    XS_INLINE static SIMDInBase<T, Width>& ToType(SIMD2<T, Width>& other)
    {
        return reinterpret_cast<SIMDInBase<T, Width>*>(other.m_data.m_values)[Index];
    }
};

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD2<T, Width>::Mask::mask2Function(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if (m_data.m_values[0]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, Masker2<T, Width, 0>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, Masker2<T, Width, 0>>(value);
        }
        if (m_data.m_values[1]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, Masker2<T, Width, 1>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, Masker2<T, Width, 1>>(value);
        }
    }
}

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD2<T, Width>::Mask::mask2ElseFunction(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        auto value = (m_data.m_values[0]) ? maskFunc.template expression1<SIMDInBase<T, Width>, Masker2<T, Width, 0>>() :
                                            maskFunc.template expression2<SIMDInBase<T, Width>, Masker2<T, Width, 0>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, Masker2<T, Width, 0>>(value);
        value = (m_data.m_values[1]) ? maskFunc.template expression1<SIMDInBase<T, Width>, Masker2<T, Width, 1>>() :
                                       maskFunc.template expression2<SIMDInBase<T, Width>, Masker2<T, Width, 1>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, Masker2<T, Width, 1>>(value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>::SIMD2(T other0, T other1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other0;
        m_data.m_values[1] = other1;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>::SIMD2(T value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = value;
        m_data.m_values[1] = value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width>::SIMD2(const SIMD2<T, Width2>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other.m_data.m_values[0];
        m_data.m_values[1] = other.m_data.m_values[1];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width>::SIMD2(const SIMDBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other.m_data.m_value;
        m_data.m_values[1] = other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width>::SIMD2(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other.m_data.m_value;
        m_data.m_values[1] = other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width>::SIMD2(const SIMDInBase<T, Width2>& other0, const SIMDInBase<T, Width2>& other1) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other0.m_data.m_value;
        m_data.m_values[1] = other1.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width>::SIMD2(const SIMDBase<T, Width2>& other0, const SIMDBase<T, Width2>& other1) noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other0.m_data.m_value;
        m_data.m_values[1] = other1.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width>::SIMD2(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(T{1});
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE SIMDInBase<T, Width2> SIMD2<T, Width>::getValueInBase() const noexcept
{
    static_assert(Index < 2);
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width2>(m_data.m_values[Index]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width2> SIMD2<T, Width>::getValue() const noexcept
{
    static_assert(Index < 2);
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDBase<T, Width2>(m_data.m_values[Index]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMDInBase<T, Width2> SIMD2<T, Width>::getValueInBase(uint32 index) const noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width2>(m_data.m_values[index]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width2> SIMD2<T, Width>::getValue(uint32 index) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDBase<T, Width2>(m_data.m_values[index]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD2<T, Width>::setValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(Index < 2);
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index] = other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD2<T, Width>::setValue(const SIMDBase<T, Width2>& other) noexcept
{
    static_assert(Index < 2);
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index] = other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE void SIMD2<T, Width>::setValue(uint32 index, const SIMDBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[index] = other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD2<T, Width>::addValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(Index < 2);
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index] += other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD2<T, Width>::subValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(Index < 2);
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index] -= other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD2<T, Width>::mulValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(Index < 2);
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index] *= other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD2<T, Width>::divValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(Index < 2);
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index] /= other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::negate() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(
            Elem0 ? -m_data.m_values[0] : m_data.m_values[0], Elem1 ? -m_data.m_values[1] : m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::mad(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::mad(const SIMDBase<T, Width2>& other1, const SIMD2<T, Width>& other2) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(fma<T>(m_data.m_values[0], other1.m_data.m_value, other2.m_data.m_values[0]),
            fma<T>(m_data.m_values[1], other1.m_data.m_value, other2.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::mad(const SIMD2<T, Width>& other1, const SIMDBase<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_value),
            fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::msub(const SIMD2<T, Width2>& other1, const SIMD2<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (hasFMA<Width>) {
            return SIMD2(fma<T>(m_data.m_values[0], other1.m_data.m_values[0], -other2.m_data.m_values[0]),
                fma<T>(m_data.m_values[1], other1.m_data.m_values[1], -other2.m_data.m_values[1]));
        } else {
            return SIMD2((m_data.m_values[0] * other1.m_data.m_values[0]) - other2.m_data.m_values[0],
                (m_data.m_values[1] * other1.m_data.m_values[1]) - other2.m_data.m_values[1]);
        }
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::equalMask(const SIMD2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] == other.m_data.m_values[0], m_data.m_values[1] == other.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::lessOrEqualMask(const SIMD2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] <= other.m_data.m_values[0], m_data.m_values[1] <= other.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::lessThanMask(const SIMD2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] < other.m_data.m_values[0], m_data.m_values[1] < other.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::notEqualMask(const SIMD2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] != other.m_data.m_values[0], m_data.m_values[1] != other.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::equalMask(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] == other.m_data.m_value, m_data.m_values[1] == other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::lessOrEqualMask(const SIMDBase<T, Width2>& other) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] <= other.m_data.m_value, m_data.m_values[1] <= other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::lessThanMask(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] < other.m_data.m_value, m_data.m_values[1] < other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::notEqualMask(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] != other.m_data.m_value, m_data.m_values[1] != other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::greaterOrEqualMask(const SIMDBase<T, Width2>& other) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] >= other.m_data.m_value, m_data.m_values[1] >= other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD2<T, Width>::Mask SIMD2<T, Width>::greaterThanMask(const SIMDBase<T, Width2>& other) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] > other.m_data.m_value, m_data.m_values[1] > other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::sign(const SIMD2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::sign<T>(m_data.m_values[0], m_data.m_values[0]),
            Shift::sign<T>(-m_data.m_values[1], m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::sign(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::sign<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::sign<T>(m_data.m_values[1], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(abs<T>(m_data.m_values[0]), abs<T>(m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::max(const SIMD2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(
            max<T>(m_data.m_values[0], other.m_data.m_values[0]), max<T>(m_data.m_values[1], other.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::min(const SIMD2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(
            min<T>(m_data.m_values[0], other.m_data.m_values[0]), min<T>(m_data.m_values[1], other.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::max(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(
            max<T>(m_data.m_values[0], other.m_data.m_value), max<T>(m_data.m_values[1], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::min(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(
            min<T>(m_data.m_values[0], other.m_data.m_value), min<T>(m_data.m_values[1], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::BaseDef SIMD2<T, Width>::hmax() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return BaseDef(max<T>(m_data.m_values[0], m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::BaseDef SIMD2<T, Width>::hmin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return BaseDef(min<T>(m_data.m_values[0], m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD2<T, Width>::hmaxInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width>(hmax());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD2<T, Width>::hminInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width>(hmin());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::BaseDef SIMD2<T, Width>::hmaxIndex(uint32& index) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        const T other = max<T>(m_data.m_values[0], m_data.m_values[1]);
        index = (other == m_data.m_values[0]) ? 0 : 1;
        return BaseDef(other);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::BaseDef SIMD2<T, Width>::hminIndex(uint32& index) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        const T other = min<T>(m_data.m_values[0], m_data.m_values[1]);
        index = (other == m_data.m_values[0]) ? 0 : 1;
        return BaseDef(other);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::reciprocal() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(recip<T>(m_data.m_values[0]), recip<T>(m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::BaseDef SIMD2<T, Width>::hadd() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return BaseDef(m_data.m_values[0] + m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD2<T, Width>::haddInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width>(hadd());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::BaseDef SIMD2<T, Width>::hsub() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return BaseDef(m_data.m_values[0] - m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD2<T, Width>::hsubInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width>(hsub());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::ceil<T>(m_data.m_values[0]), Shift::ceil<T>(m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::floor<T>(m_data.m_values[0]), Shift::floor<T>(m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::trunc<T>(m_data.m_values[0]), Shift::trunc<T>(m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::sqrt<T>(m_data.m_values[0]), Shift::sqrt<T>(m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::rsqrt<T>(m_data.m_values[0]), Shift::rsqrt<T>(m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::exp2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::exp2<T>(m_data.m_values[0]), Shift::exp2<T>(m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::exp() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::exp<T>(m_data.m_values[0]), Shift::exp<T>(m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::log2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::log2<T>(m_data.m_values[0]), Shift::log2<T>(m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::log() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::log<T>(m_data.m_values[0]), Shift::log<T>(m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::pow(const SIMD2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::pow<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::pow<T>(m_data.m_values[1], other.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::powr(const SIMD2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::powr<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::powr<T>(m_data.m_values[1], other.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::pow(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::pow<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[1], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::powr(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::powr<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[1], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::sin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::sin<T>(m_data.m_values[0]), Shift::sin<T>(m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::cos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::cos<T>(m_data.m_values[0]), Shift::cos<T>(m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::tan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::tan<T>(m_data.m_values[0]), Shift::tan<T>(m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD2<T, Width>::sincos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        SIMD4<T, Width> ret;
        ret.m_data.m_values[0] = Shift::sincos<T>(m_data.m_values[0], ret.m_data.m_values[2]);
        ret.m_data.m_values[1] = Shift::sincos<T>(m_data.m_values[1], ret.m_data.m_values[3]);
        return ret;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::asin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::asin<T>(m_data.m_values[0]), Shift::asin<T>(m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::acos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::acos<T>(m_data.m_values[0]), Shift::acos<T>(m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::atan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::atan<T>(m_data.m_values[0]), Shift::atan<T>(m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::atan2(const SIMD2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::atan2<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::atan2<T>(m_data.m_values[1], other.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::insert(const SIMD2<T, Width2>& other) const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index0] = other.m_data.m_values[Index1];
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::blend(const SIMD2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(Elem0 ? other.m_data.m_values[0] : m_data.m_values[0],
            Elem1 ? other.m_data.m_values[1] : m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::blendVar(const SIMD2<T, Width2>& other, const Mask& mask) const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>((mask.m_data.m_values[0]) ? other.m_data.m_values[0] : m_data.m_values[0],
            (mask.m_data.m_values[1]) ? other.m_data.m_values[1] : m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::blendSwap(SIMD2<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        const SIMD2<T, Width> back = other;
        other.m_data.m_values[0] = Elem0 ? m_data.m_values[0] : other.m_data.m_values[0];
        other.m_data.m_values[1] = Elem1 ? m_data.m_values[1] : other.m_data.m_values[1];
        return SIMD2<T, Width>(
            Elem0 ? back.m_data.m_values[0] : m_data.m_values[0], Elem1 ? back.m_data.m_values[1] : m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::blendSwapVar(SIMD2<T, Width2>& other, const Mask& mask) const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        const SIMD2<T, Width> back = other;
        other.m_data.m_values[0] = (mask.m_data.m_values[0]) ? m_data.m_values[0] : other.m_data.m_values[0];
        other.m_data.m_values[1] = (mask.m_data.m_values[1]) ? m_data.m_values[1] : other.m_data.m_values[1];
        return SIMD2<T, Width>((mask.m_data.m_values[0]) ? back.m_data.m_values[0] : m_data.m_values[0],
            (mask.m_data.m_values[1]) ? back.m_data.m_values[1] : m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD2<T, Width> SIMD2<T, Width>::shuffle() const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(m_data.m_values[Index0], m_data.m_values[Index1]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> operator+(const SIMD2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>((other1.m_data.m_values[0] + other2.m_data.m_values[0]),
            (other1.m_data.m_values[1] + other2.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> operator+(const SIMD2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD2<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(
            (other1.m_data.m_values[0] + other2.m_data.m_value), (other1.m_data.m_values[1] + other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> operator-(const SIMD2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>((other1.m_data.m_values[0] - other2.m_data.m_values[0]),
            (other1.m_data.m_values[1] - other2.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> operator-(const SIMD2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD2<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(
            (other1.m_data.m_values[0] - other2.m_data.m_value), (other1.m_data.m_values[1] - other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> operator-(const SIMDBase<T, Width2>& other1, const SIMD2<T, Width>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD2<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(
            (other1.m_data.m_value - other2.m_data.m_values[0]), (other1.m_data.m_value - other2.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> operator*(const SIMD2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>((other1.m_data.m_values[0] * other2.m_data.m_values[0]),
            (other1.m_data.m_values[1] * other2.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> operator*(const SIMD2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD2<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(
            (other1.m_data.m_values[0] * other2.m_data.m_value), (other1.m_data.m_values[1] * other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> operator/(const SIMD2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>((other1.m_data.m_values[0] / other2.m_data.m_values[0]),
            (other1.m_data.m_values[1] / other2.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> operator/(const SIMD2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD2<T, Width>::BaseDef, SIMDBase<T, Width2>>);
    if constexpr (defaultPrecision == Precision::Precise) {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        } else
#endif
        {
            return SIMD2<T, Width>((other1.m_data.m_values[0] / other2.m_data.m_value),
                (other1.m_data.m_values[1] / other2.m_data.m_value));
        }
    } else {
        return other1 * other2.reciprocal();
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> operator/(const SIMDBase<T, Width2>& other1, const SIMD2<T, Width>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD2<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(
            (other1.m_data.m_value / other2.m_data.m_values[0]), (other1.m_data.m_value / other2.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator-(const SIMD2<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(-other.m_data.m_values[0], -other.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width>& operator+=(SIMD2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] += other2.m_data.m_values[0];
        other1.m_data.m_values[1] += other2.m_data.m_values[1];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width>& operator+=(SIMD2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] += other2.m_data.m_value;
        other1.m_data.m_values[1] += other2.m_data.m_value;
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width>& operator-=(SIMD2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] -= other2.m_data.m_values[0];
        other1.m_data.m_values[1] -= other2.m_data.m_values[1];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width>& operator-=(SIMD2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD2<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] -= other2.m_data.m_value;
        other1.m_data.m_values[1] -= other2.m_data.m_value;
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width>& operator*=(SIMD2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] *= other2.m_data.m_values[0];
        other1.m_data.m_values[1] *= other2.m_data.m_values[1];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width>& operator*=(SIMD2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD2<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] *= other2.m_data.m_value;
        other1.m_data.m_values[1] *= other2.m_data.m_value;
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width>& operator/=(SIMD2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] /= other2.m_data.m_values[0];
        other1.m_data.m_values[1] /= other2.m_data.m_values[1];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width>& operator/=(SIMD2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD2<T, Width>::BaseDef, SIMDBase<T, Width2>>);
    if constexpr (defaultPrecision == Precision::Precise) {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        } else
#endif
        {
            other1.m_data.m_values[0] /= other2.m_data.m_value;
            other1.m_data.m_values[1] /= other2.m_data.m_value;
        }
    } else {
        other1 *= other2.reciprocal();
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator==(const SIMD2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] == other2.m_data.m_values[0]) &
            (other1.m_data.m_values[1] == other2.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator==(const SIMD2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD2<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] == other2.m_data.m_value) &
            (other1.m_data.m_values[1] == other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<=(const SIMD2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] <= other2.m_data.m_values[0]) &
            (other1.m_data.m_values[1] <= other2.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<=(const SIMD2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD2<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] <= other2.m_data.m_value) &
            (other1.m_data.m_values[1] <= other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<(const SIMD2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] < other2.m_data.m_values[0]) &
            (other1.m_data.m_values[1] < other2.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<(const SIMD2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD2<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return (
            (other1.m_data.m_values[0] < other2.m_data.m_value) & (other1.m_data.m_values[1] < other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator!=(const SIMD2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] != other2.m_data.m_values[0]) &
            (other1.m_data.m_values[1] != other2.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator!=(const SIMD2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD2<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] != other2.m_data.m_value) &
            (other1.m_data.m_values[1] != other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> operator&(const SIMD2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(Shift::bitAnd<T>(other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::bitAnd<T>(other1.m_data.m_values[1], other2.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> operator&(const SIMD2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD2<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(Shift::bitAnd<T>(other1.m_data.m_values[0], other2.m_data.m_value),
            Shift::bitAnd<T>(other1.m_data.m_values[1], other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> operator|(const SIMD2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(Shift::bitOr<T>(other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::bitOr<T>(other1.m_data.m_values[1], other2.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> operator|(const SIMD2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD2<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(Shift::bitOr<T>(other1.m_data.m_values[0], other2.m_data.m_value),
            Shift::bitOr<T>(other1.m_data.m_values[1], other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> operator^(const SIMD2<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(Shift::bitXor<T>(other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::bitXor<T>(other1.m_data.m_values[1], other2.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width> operator^(const SIMD2<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD2<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(Shift::bitXor<T>(other1.m_data.m_values[0], other2.m_data.m_value),
            Shift::bitXor<T>(other1.m_data.m_values[1], other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> operator~(const SIMD2<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(Shift::bitNot<T>(other.m_data.m_values[0]), Shift::bitNot<T>(other.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask operator&(
    const typename SIMD2<T, Width>::Mask& mask1, const typename SIMD2<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(
            mask1.m_data.m_values[0] & mask2.m_data.m_values[0], mask1.m_data.m_values[1] & mask2.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask operator|(
    const typename SIMD2<T, Width>::Mask& mask1, const typename SIMD2<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(
            mask1.m_data.m_values[0] | mask2.m_data.m_values[0], mask1.m_data.m_values[1] | mask2.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask operator^(
    const typename SIMD2<T, Width>::Mask& mask1, const typename SIMD2<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(
            mask1.m_data.m_values[0] ^ mask2.m_data.m_values[0], mask1.m_data.m_values[1] ^ mask2.m_data.m_values[1]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD2<T, Width>::Mask operator~(const typename SIMD2<T, Width>::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(!mask.m_data.m_values[0], !mask.m_data.m_values[1]);
    }
}
} // namespace Shift
