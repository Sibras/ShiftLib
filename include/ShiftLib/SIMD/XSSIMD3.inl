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

#include "SIMD/XSSIMD3.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#endif
#include "XSMath.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3Data<T>::SIMD3Data(const SIMD3<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_value0 = other.m_data.m_values[0];
        m_value1 = other.m_data.m_values[1];
        m_value2 = other.m_data.m_values[2];
    }
}

template<typename T>
XS_INLINE void SIMD3Data<T>::setData(const T value0, const T value1, const T value2) noexcept
{
    m_value0 = value0;
    m_value1 = value1;
    m_value2 = value2;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD3Data<T>::store(const SIMD3<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_value0 = other.m_data.m_values[0];
        m_value1 = other.m_data.m_values[1];
        m_value2 = other.m_data.m_values[2];
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3Data<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>(m_value0, m_value1, m_value2);
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3DataPad<T>::SIMD3DataPad(const SIMD3<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_value0 = other.m_data.m_values[0];
        m_value1 = other.m_data.m_values[1];
        m_value2 = other.m_data.m_values[2];
    }
}

template<typename T>
XS_INLINE void SIMD3DataPad<T>::setData(const T value0, const T value1, const T value2) noexcept
{
    m_value0 = value0;
    m_value1 = value1;
    m_value2 = value2;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD3DataPad<T>::store(const SIMD3<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>(m_value0, m_value1, m_value2);
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3DataPad<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>(m_value0, m_value1, m_value2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::Mask::Mask(const uint32 mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = mask & 0x1;
        m_data.m_values[1] = mask & 0x2;
        m_data.m_values[2] = mask & 0x4;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::Mask::Mask(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::Mask::Mask(const bool bool0, const bool bool1, const bool bool2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE Bool3<hasSIMD<T> && (Width > SIMDWidth::Scalar)> SIMD3<T, Width>::Mask::getBool3() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Bool3(m_data.m_values[0], m_data.m_values[1], m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD3<T, Width>::Mask::getAny() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return m_data.m_values[0] | m_data.m_values[1] | m_data.m_values[2];
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD3<T, Width>::Mask::getAll() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return (m_data.m_values[0] & m_data.m_values[1] & m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD3<T, Width>::Mask::getNone() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return !((m_data.m_values[0] & m_data.m_values[1] & m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width, uint32 Index>
class Masker3
{
public:
    XS_INLINE static SIMDInBase<T, Width>& ToType(SIMD2<T, Width>& other)
    {
        return reinterpret_cast<SIMDInBase<T, Width>*>(other.m_data.m_values)[Index];
    }
};

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD3<T, Width>::Mask::mask3Function(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if (m_data.m_values[0]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, Masker3<T, Width, 0>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, Masker3<T, Width, 0>>(value);
        }
        if (m_data.m_values[1]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, Masker3<T, Width, 1>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, Masker3<T, Width, 1>>(value);
        }
        if (m_data.m_values[2]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, Masker3<T, Width, 2>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, Masker3<T, Width, 2>>(value);
        }
    }
}

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD3<T, Width>::Mask::mask3ElseFunction(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        auto value = (m_data.m_values[0]) ? maskFunc.template expression1<SIMDInBase<T, Width>, Masker3<T, Width, 0>>() :
                                            maskFunc.template expression2<SIMDInBase<T, Width>, Masker3<T, Width, 0>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, Masker3<T, Width, 0>>(value);
        value = (m_data.m_values[1]) ? maskFunc.template expression1<SIMDInBase<T, Width>, Masker3<T, Width, 1>>() :
                                       maskFunc.template expression2<SIMDInBase<T, Width>, Masker3<T, Width, 1>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, Masker3<T, Width, 1>>(value);
        value = (m_data.m_values[1]) ? maskFunc.template expression1<SIMDInBase<T, Width>, Masker3<T, Width, 2>>() :
                                       maskFunc.template expression2<SIMDInBase<T, Width>, Masker3<T, Width, 2>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, Masker3<T, Width, 2>>(value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::Shuffle::Shuffle(const uint32 shuff0, const uint32 shuff1, const uint32 shuff2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = shuff0;
        m_data.m_values[1] = shuff1;
        m_data.m_values[2] = shuff2;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::Shuffle::Shuffle(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::Shuffle::Shuffle(const uint32 shuffle) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = shuffle;
        m_data.m_values[1] = shuffle;
        m_data.m_values[2] = shuffle;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Shuffle SIMD3<T, Width>::Shuffle::Default() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(0, 1, 2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Shuffle SIMD3<T, Width>::Shuffle::Reverse() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(2, 1, 0);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Shuffle SIMD3<T, Width>::Shuffle::RotateLeft(const uint32 rotate) noexcept
{
    XS_ASSERT(rotate < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        switch (rotate) {
            case 0:
                return Shuffle(0, 1, 2);
            case 1:
                return Shuffle(1, 2, 0);
            case 2:
                return Shuffle(2, 0, 1);
            default:
                XS_UNREACHABLE;
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Shuffle SIMD3<T, Width>::Shuffle::RotateRight(const uint32 rotate) noexcept
{
    XS_ASSERT(rotate < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        const uint32 u = (4 >> rotate) & 3;
        const uint32 w = 2 - rotate;
        const uint32 v = 3 - u - w;
        return Shuffle(u, v, w);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Shuffle SIMD3<T, Width>::Shuffle::Extract(const uint32 extract) noexcept
{
    XS_ASSERT(extract < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        const uint32 u = (2 - extract) >> 1;
        const uint32 v = 3 - u - extract;
        return Shuffle(u, v, extract);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE typename SIMD3<T, Width>::Shuffle SIMD3<T, Width>::Shuffle::shuffle() const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(m_data.m_values[Index0], m_data.m_values[Index1], m_data.m_values[Index2]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::SIMD3(const T value0, const T value1, const T value2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = value0;
        m_data.m_values[1] = value1;
        m_data.m_values[2] = value2;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::SIMD3(const T value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = value;
        m_data.m_values[1] = value;
        m_data.m_values[2] = value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width>::SIMD3(const SIMDBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other.m_data.m_values[0];
        m_data.m_values[1] = other.m_data.m_values[1];
        m_data.m_values[2] = other.m_data.m_values[2];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width>::SIMD3(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other.m_data.m_value;
        m_data.m_values[1] = other.m_data.m_value;
        m_data.m_values[2] = other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width>::SIMD3(const SIMD2<T, Width2>& other0, const SIMDInBase<T, Width2>& other1) noexcept
{
    static_assert(
        (sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>> && sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>));
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other0.m_data.m_values[0];
        m_data.m_values[1] = other0.m_data.m_values[1];
        m_data.m_values[2] = other1.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width>::SIMD3(
    const SIMD2<T, Width2>& other0, const SIMDInBase<T, Width2>& other1, const uint32 index) noexcept
{
    static_assert(
        (sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>> && sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>));
    XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if (index == 0) {
            m_data.m_values[0] = other1.m_data.m_value;
            m_data.m_values[1] = other0.m_data.m_values[0];
            m_data.m_values[2] = other0.m_data.m_values[1];
        } else if (index == 1) {
            m_data.m_values[0] = other0.m_data.m_values[0];
            m_data.m_values[1] = other1.m_data.m_value;
            m_data.m_values[2] = other0.m_data.m_values[1];
        } else if (index == 2) {
            m_data.m_values[0] = other0.m_data.m_values[0];
            m_data.m_values[1] = other0.m_data.m_values[1];
            m_data.m_values[2] = other1.m_data.m_value;
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width>::SIMD3(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(T{1});
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE void SIMD3<T, Width>::Transpose(const SIMD3<T, Width>& other0, const SIMD3<T, Width>& other1,
    const SIMD3<T, Width>& other2, SIMD3<T, Width2>& otherT0, SIMD3<T, Width2>& otherT1,
    SIMD3<T, Width2>& otherT2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        otherT0.m_data.m_values[0] = other0.m_data.m_values[0];
        otherT0.m_data.m_values[1] = other1.m_data.m_values[0];
        otherT0.m_data.m_values[2] = other2.m_data.m_values[0];
        otherT1.m_data.m_values[0] = other0.m_data.m_values[1];
        otherT1.m_data.m_values[1] = other1.m_data.m_values[1];
        otherT1.m_data.m_values[2] = other2.m_data.m_values[1];
        otherT2.m_data.m_values[0] = other0.m_data.m_values[2];
        otherT2.m_data.m_values[1] = other1.m_data.m_values[2];
        otherT2.m_data.m_values[2] = other2.m_data.m_values[2];
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE SIMDInBase<T, Width2> SIMD3<T, Width>::getValueInBase() const noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 3);
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
XS_INLINE SIMDBase<T, Width2> SIMD3<T, Width>::getValue() const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
    static_assert(Index < 3);
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
XS_INLINE SIMDInBase<T, Width2> SIMD3<T, Width>::getValueInBase(const uint32 index) const noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    XS_ASSERT(index < 3);
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
XS_INLINE SIMDBase<T, Width2> SIMD3<T, Width>::getValue(const uint32 index) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
    XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDBase<T, Width2>(m_data.m_values[index]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width2> SIMD3<T, Width>::getValue2() const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
    static_assert(Index0 < 3 && Index1 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width2>(m_data.m_values[Index0], m_data.m_values[Index1]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD3<T, Width>::setValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 3);
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
XS_INLINE void SIMD3<T, Width>::setValue(const SIMDBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
    static_assert(Index < 3);
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
XS_INLINE void SIMD3<T, Width>::setValue(const uint32 index, const SIMDBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
    XS_ASSERT(index < 3);
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
XS_INLINE void SIMD3<T, Width>::addValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 3);
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
XS_INLINE void SIMD3<T, Width>::subValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 3);
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
XS_INLINE void SIMD3<T, Width>::mulValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 3);
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
XS_INLINE void SIMD3<T, Width>::divValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index] /= other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::negate() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Elem0 ? -m_data.m_values[0] : m_data.m_values[0], Elem1 ? -m_data.m_values[1] : m_data.m_values[1],
            Elem2 ? -m_data.m_values[2] : m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::mad(const SIMD3<T, Width2>& other1, const SIMD3<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::mad(const SIMDBase<T, Width2>& other1, const SIMD3<T, Width>& other2) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_value, other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_value, other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_value, other2.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::mad(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::msub(const SIMD3<T, Width2>& other1, const SIMD3<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], -other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], -other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], -other2.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::subAdd(const SIMD3<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(m_data.m_values[0] - other.m_data.m_values[0], m_data.m_values[1] + other.m_data.m_values[1],
            m_data.m_values[2] - other.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::equalMask(const SIMD3<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] == other.m_data.m_values[0], m_data.m_values[1] == other.m_data.m_values[1],
            m_data.m_values[2] == other.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::lessOrEqualMask(const SIMD3<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] <= other.m_data.m_values[0], m_data.m_values[1] <= other.m_data.m_values[1],
            m_data.m_values[2] <= other.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::lessThanMask(const SIMD3<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] < other.m_data.m_values[0], m_data.m_values[1] < other.m_data.m_values[1],
            m_data.m_values[2] < other.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::notEqualMask(const SIMD3<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] != other.m_data.m_values[0], m_data.m_values[1] != other.m_data.m_values[1],
            m_data.m_values[2] != other.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::equalMask(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] == other.m_data.m_value, m_data.m_values[1] == other.m_data.m_value,
            m_data.m_values[2] == other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::lessOrEqualMask(const SIMDBase<T, Width2>& other) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] <= other.m_data.m_value, m_data.m_values[1] <= other.m_data.m_value,
            m_data.m_values[2] <= other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::lessThanMask(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] < other.m_data.m_value, m_data.m_values[1] < other.m_data.m_value,
            m_data.m_values[2] < other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::greaterOrEqualMask(const SIMDBase<T, Width2>& other) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] >= other.m_data.m_value, m_data.m_values[1] >= other.m_data.m_value,
            m_data.m_values[2] >= other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::greaterThanMask(const SIMDBase<T, Width2>& other) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] > other.m_data.m_value, m_data.m_values[1] > other.m_data.m_value,
            m_data.m_values[2] > other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD3<T, Width>::Mask SIMD3<T, Width>::notEqualMask(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] != other.m_data.m_value, m_data.m_values[1] != other.m_data.m_value,
            m_data.m_values[2] != other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::sign(const SIMD3<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::sign<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::sign<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::sign<T>(m_data.m_values[2], other.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::sign(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::sign<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::sign<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::sign<T>(m_data.m_values[2], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(
            Shift::abs<T>(m_data.m_values[0]), Shift::abs<T>(m_data.m_values[1]), Shift::abs<T>(m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::max(const SIMD3<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::max<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::max<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::max<T>(m_data.m_values[2], other.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::min(const SIMD3<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::min<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::min<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::min<T>(m_data.m_values[2], other.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::max(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::max<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[2], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::min(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::min<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[2], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::BaseDef SIMD3<T, Width>::hmax() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        const T other = Shift::max<T>(m_data.m_values[0], m_data.m_values[1]);
        return BaseDef(Shift::max<T>(m_data.m_values[2], other));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::BaseDef SIMD3<T, Width>::hmin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        const T other = Shift::min<T>(m_data.m_values[0], m_data.m_values[1]);
        return BaseDef(Shift::min<T>(m_data.m_values[2], other));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD3<T, Width>::hmaxInBase() const noexcept
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
XS_INLINE SIMDInBase<T, Width> SIMD3<T, Width>::hminInBase() const noexcept
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
XS_INLINE typename SIMD3<T, Width>::BaseDef SIMD3<T, Width>::hmaxIndex(uint32& index) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        const T other = Shift::max<T>(m_data.m_values[0], m_data.m_values[1]);
        index = (other == m_data.m_values[0]) ? 0 : 1;
        const T other2 = Shift::max<T>(m_data.m_values[2], other);
        index = (other2 == other) ? index : 2;
        return BaseDef(other2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::BaseDef SIMD3<T, Width>::hminIndex(uint32& index) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        const T other = Shift::min<T>(m_data.m_values[0], m_data.m_values[1]);
        index = (other == m_data.m_values[0]) ? 0 : 1;
        const T other2 = Shift::min<T>(m_data.m_values[2], other);
        index = (other2 == other) ? index : 2;
        return BaseDef(other2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::reciprocal() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(recip<T>(m_data.m_values[0]), recip<T>(m_data.m_values[1]), recip<T>(m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::BaseDef SIMD3<T, Width>::hadd() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        T res = (m_data.m_values[0] + m_data.m_values[1]);
        res += m_data.m_values[2];
        return BaseDef(res);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD3<T, Width>::haddInBase() const noexcept
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
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3((Shift::ceil<T>(m_data.m_values[0])), (Shift::ceil<T>(m_data.m_values[1])),
            (Shift::ceil<T>(m_data.m_values[2])));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3((Shift::floor<T>(m_data.m_values[0])), (Shift::floor<T>(m_data.m_values[1])),
            (Shift::floor<T>(m_data.m_values[2])));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3((Shift::trunc<T>(m_data.m_values[0])), (Shift::trunc<T>(m_data.m_values[1])),
            (Shift::trunc<T>(m_data.m_values[2])));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(
            Shift::sqrt<T>(m_data.m_values[0]), Shift::sqrt<T>(m_data.m_values[1]), Shift::sqrt<T>(m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::rsqrt<T>(m_data.m_values[0]), Shift::rsqrt<T>(m_data.m_values[1]),
            Shift::rsqrt<T>(m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD3<T, Width>::BaseDef SIMD3<T, Width>::dot3(const SIMD3<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        T res = (m_data.m_values[0] * other.m_data.m_values[0]);
        const T res2 = (m_data.m_values[1] * other.m_data.m_values[1]);
        const T res3 = (m_data.m_values[2] * other.m_data.m_values[2]);
        res += res2;
        res += res3;
        return BaseDef(res);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMDInBase<T, Width> SIMD3<T, Width>::dot3InBase(const SIMD3<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width>(dot3(other));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::cross3(const SIMD3<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        const T crossX = -(m_data.m_values[2] * other.m_data.m_values[1]);
        const T crossY = -(m_data.m_values[0] * other.m_data.m_values[2]);
        const T crossZ = -(m_data.m_values[1] * other.m_data.m_values[0]);
        return SIMD3(Shift::fma<T>(m_data.m_values[1], other.m_data.m_values[2], crossX),
            Shift::fma<T>(m_data.m_values[2], other.m_data.m_values[0], crossY),
            Shift::fma<T>(m_data.m_values[0], other.m_data.m_values[1], crossZ));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::BaseDef SIMD3<T, Width>::lengthSqr() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        T res = (m_data.m_values[0] * m_data.m_values[0]);
        const T res2 = (m_data.m_values[1] * m_data.m_values[1]);
        const T res3 = (m_data.m_values[2] * m_data.m_values[2]);
        res += res2;
        res += res3;
        return BaseDef(res);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::BaseDef SIMD3<T, Width>::length() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return BaseDef(Shift::sqrt<T>(length()));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD3<T, Width>::lengthSqrInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width>(lengthSqr());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD3<T, Width>::lengthInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width>(length());
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::normalize() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return *this / length();
        } else {
            const auto invLen = Shift::rsqrt<T>(lengthSqr().m_data.m_value);
            return *this * invLen;
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::exp2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(
            Shift::exp2<T>(m_data.m_values[0]), Shift::exp2<T>(m_data.m_values[1]), Shift::exp2<T>(m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::exp() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(
            Shift::exp<T>(m_data.m_values[0]), Shift::exp<T>(m_data.m_values[1]), Shift::exp<T>(m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::log2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(
            Shift::log2<T>(m_data.m_values[0]), Shift::log2<T>(m_data.m_values[1]), Shift::log2<T>(m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::log() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(
            Shift::log<T>(m_data.m_values[0]), Shift::log<T>(m_data.m_values[1]), Shift::log<T>(m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::pow(const SIMD3<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::pow<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::pow<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::pow<T>(m_data.m_values[2], other.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::powr(const SIMD3<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::powr<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::powr<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::powr<T>(m_data.m_values[2], other.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::pow(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::pow<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[2], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::powr(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::powr<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[2], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::sin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(
            Shift::sin<T>(m_data.m_values[0]), Shift::sin<T>(m_data.m_values[1]), Shift::sin<T>(m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::cos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(
            Shift::cos<T>(m_data.m_values[0]), Shift::cos<T>(m_data.m_values[1]), Shift::cos<T>(m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::tan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(
            Shift::tan<T>(m_data.m_values[0]), Shift::tan<T>(m_data.m_values[1]), Shift::tan<T>(m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> SIMD3<T, Width>::sincos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        SIMD3 ret1, ret2;
        ret1.m_data.m_values[0] = Shift::sincos<T>(m_data.m_values[0], ret2.m_data.m_values[0]);
        ret1.m_data.m_values[1] = Shift::sincos<T>(m_data.m_values[1], ret2.m_data.m_values[1]);
        ret1.m_data.m_values[2] = Shift::sincos<T>(m_data.m_values[2], ret2.m_data.m_values[2]);
        return SIMD3x2(ret1, ret2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::asin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(
            Shift::asin<T>(m_data.m_values[0]), Shift::asin<T>(m_data.m_values[1]), Shift::asin<T>(m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::acos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(
            Shift::acos<T>(m_data.m_values[0]), Shift::acos<T>(m_data.m_values[1]), Shift::acos<T>(m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::atan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(
            Shift::atan<T>(m_data.m_values[0]), Shift::atan<T>(m_data.m_values[1]), Shift::atan<T>(m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::atan2(const SIMD3<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(Shift::atan2<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::atan2<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::atan2<T>(m_data.m_values[2], other.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::insert(const SIMD3<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
    static_assert(Index0 < 3 && Index1 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index0] = other.m_data.m_values[Index1];
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::blend(const SIMD3<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3((Elem0) ? other.m_data.m_values[0] : m_data.m_values[0],
            (Elem1) ? other.m_data.m_values[1] : m_data.m_values[1],
            (Elem2) ? other.m_data.m_values[2] : m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::blendVar(const SIMD3<T, Width2>& other, const Mask& mask) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3((mask.m_data.m_values[0]) ? other.m_data.m_values[0] : m_data.m_values[0],
            (mask.m_data.m_values[1]) ? other.m_data.m_values[1] : m_data.m_values[1],
            (mask.m_data.m_values[2]) ? other.m_data.m_values[2] : m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::blendSwap(SIMD3<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        const SIMD3 backup = other;
        other.m_data.m_values[0] = (Elem0) ? m_data.m_values[0] : other.m_data.m_values[0];
        other.m_data.m_values[1] = (Elem1) ? m_data.m_values[1] : other.m_data.m_values[1];
        other.m_data.m_values[2] = (Elem2) ? m_data.m_values[2] : other.m_data.m_values[2];
        return SIMD3((Elem0) ? backup.m_data.m_values[0] : m_data.m_values[0],
            (Elem1) ? backup.m_data.m_values[1] : m_data.m_values[1],
            (Elem2) ? backup.m_data.m_values[2] : m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::blendSwapVar(SIMD3<T, Width2>& other, const Mask& mask) const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        const SIMD3 backup = other;
        other.m_data.m_values[0] = (mask.m_data.m_values[0]) ? m_data.m_values[0] : other.m_data.m_values[0];
        other.m_data.m_values[1] = (mask.m_data.m_values[1]) ? m_data.m_values[1] : other.m_data.m_values[1];
        other.m_data.m_values[2] = (mask.m_data.m_values[2]) ? m_data.m_values[2] : other.m_data.m_values[2];
        return SIMD3((mask.m_data.m_values[0]) ? backup.m_data.m_values[0] : m_data.m_values[0],
            (mask.m_data.m_values[1]) ? backup.m_data.m_values[1] : m_data.m_values[1],
            (mask.m_data.m_values[2]) ? backup.m_data.m_values[2] : m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::shuffle() const noexcept
{
    static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(m_data.m_values[Index0], m_data.m_values[Index1], m_data.m_values[Index2]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::shuffleVar(const Shuffle& shuffle) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(m_data.m_values[shuffle.m_data.m_values[0]], m_data.m_values[shuffle.m_data.m_values[1]],
            m_data.m_values[shuffle.m_data.m_values[2]]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> SIMD3<T, Width>::combine(const SIMD3<T, Width2>& other) const noexcept
{
    static_assert(Index0 < 6 && Index1 < 6 && Index2 < 6);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3(((Index0 < 3) ? this->m_data.m_values : other.m_data.m_values)[Index0 % 3],
            ((Index1 < 3) ? this->m_data.m_values : other.m_data.m_values)[Index1 % 3],
            ((Index2 < 3) ? this->m_data.m_values : other.m_data.m_values)[Index2 % 3]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD3<T, Width>::combine4(const SIMD3<T, Width2>& other) const noexcept
{
    static_assert(Index0 < 6 && Index1 < 6 && Index2 < 6);
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(((Index0 < 3) ? this->m_data.m_values : other.m_data.m_values)[Index0 % 3],
            ((Index1 < 3) ? this->m_data.m_values : other.m_data.m_values)[Index1 % 3],
            ((Index2 < 3) ? this->m_data.m_values : other.m_data.m_values)[Index2 % 3],
            ((Index3 < 3) ? this->m_data.m_values : other.m_data.m_values)[Index3 % 3]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> operator+(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>((other1.m_data.m_values[0] + other2.m_data.m_values[0]),
            (other1.m_data.m_values[1] + other2.m_data.m_values[1]),
            (other1.m_data.m_values[2] + other2.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> operator+(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>((other1.m_data.m_values[0] + other2.m_data.m_value),
            (other1.m_data.m_values[1] + other2.m_data.m_value), (other1.m_data.m_values[2] + other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> operator-(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>((other1.m_data.m_values[0] - other2.m_data.m_values[0]),
            (other1.m_data.m_values[1] - other2.m_data.m_values[1]),
            (other1.m_data.m_values[2] - other2.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> operator-(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>((other1.m_data.m_values[0] - other2.m_data.m_value),
            (other1.m_data.m_values[1] - other2.m_data.m_value), (other1.m_data.m_values[2] - other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> operator-(const SIMDBase<T, Width2>& other1, const SIMD3<T, Width>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>((other1.m_data.m_value - other2.m_data.m_values[0]),
            (other1.m_data.m_value - other2.m_data.m_values[1]), (other1.m_data.m_value - other2.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> operator*(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>((other1.m_data.m_values[0] * other2.m_data.m_values[0]),
            (other1.m_data.m_values[1] * other2.m_data.m_values[1]),
            (other1.m_data.m_values[2] * other2.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> operator*(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>((other1.m_data.m_values[0] * other2.m_data.m_value),
            (other1.m_data.m_values[1] * other2.m_data.m_value), (other1.m_data.m_values[2] * other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> operator/(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>((other1.m_data.m_values[0] / other2.m_data.m_values[0]),
            (other1.m_data.m_values[1] / other2.m_data.m_values[1]),
            (other1.m_data.m_values[2] / other2.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> operator/(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD3<T, Width>((other1.m_data.m_values[0] / other2.m_data.m_value),
                (other1.m_data.m_values[1] / other2.m_data.m_value),
                (other1.m_data.m_values[2] / other2.m_data.m_value));
        } else {
            const T divisor = recip<T>(other2.m_data.m_value);
            return SIMD3<T, Width>((other1.m_data.m_values[0] * divisor), (other1.m_data.m_values[1] * divisor),
                (other1.m_data.m_values[2] * divisor));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> operator/(const SIMDBase<T, Width2>& other1, const SIMD3<T, Width>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>((other1.m_data.m_value / other2.m_data.m_values[0]),
            (other1.m_data.m_value / other2.m_data.m_values[1]), (other1.m_data.m_value / other2.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator-(const SIMD3<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>(-other.m_data.m_values[0], -other.m_data.m_values[1], -other.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width>& operator+=(SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] += other2.m_data.m_values[0];
        other1.m_data.m_values[1] += other2.m_data.m_values[1];
        other1.m_data.m_values[2] += other2.m_data.m_values[2];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width>& operator+=(SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] += other2.m_data.m_value;
        other1.m_data.m_values[1] += other2.m_data.m_value;
        other1.m_data.m_values[2] += other2.m_data.m_value;
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width>& operator-=(SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] -= other2.m_data.m_values[0];
        other1.m_data.m_values[1] -= other2.m_data.m_values[1];
        other1.m_data.m_values[2] -= other2.m_data.m_values[2];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width>& operator-=(SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] -= other2.m_data.m_value;
        other1.m_data.m_values[1] -= other2.m_data.m_value;
        other1.m_data.m_values[2] -= other2.m_data.m_value;
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width>& operator*=(SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] *= other2.m_data.m_values[0];
        other1.m_data.m_values[1] *= other2.m_data.m_values[1];
        other1.m_data.m_values[2] *= other2.m_data.m_values[2];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width>& operator*=(SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] *= other2.m_data.m_value;
        other1.m_data.m_values[1] *= other2.m_data.m_value;
        other1.m_data.m_values[2] *= other2.m_data.m_value;
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width>& operator/=(SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] /= other2.m_data.m_values[0];
        other1.m_data.m_values[1] /= other2.m_data.m_values[1];
        other1.m_data.m_values[2] /= other2.m_data.m_values[2];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width>& operator/=(SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            other1.m_data.m_values[0] /= other2.m_data.m_value;
            other1.m_data.m_values[1] /= other2.m_data.m_value;
            other1.m_data.m_values[2] /= other2.m_data.m_value;
        } else {
            const T divisor = recip<T>(other2.m_data.m_value);
            other1.m_data.m_values[0] *= divisor;
            other1.m_data.m_values[1] *= divisor;
            other1.m_data.m_values[2] *= divisor;
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator==(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] == other2.m_data.m_values[0]) &
            (other1.m_data.m_values[1] == other2.m_data.m_values[1]) &
            (other1.m_data.m_values[2] == other2.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<=(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] <= other2.m_data.m_values[0]) &
            (other1.m_data.m_values[1] <= other2.m_data.m_values[1]) &
            (other1.m_data.m_values[2] <= other2.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] < other2.m_data.m_values[0]) &
            (other1.m_data.m_values[1] < other2.m_data.m_values[1]) &
            (other1.m_data.m_values[2] < other2.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator!=(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] != other2.m_data.m_values[0]) &
            (other1.m_data.m_values[1] != other2.m_data.m_values[1]) &
            (other1.m_data.m_values[2] != other2.m_data.m_values[2]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator==(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] == other2.m_data.m_value) &
            (other1.m_data.m_values[1] == other2.m_data.m_value) &
            (other1.m_data.m_values[2] == other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<=(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] <= other2.m_data.m_value) &
            (other1.m_data.m_values[1] <= other2.m_data.m_value) &
            (other1.m_data.m_values[2] <= other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] < other2.m_data.m_value) &
            (other1.m_data.m_values[1] < other2.m_data.m_value) & (other1.m_data.m_values[2] < other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator!=(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] != other2.m_data.m_value) &
            (other1.m_data.m_values[1] != other2.m_data.m_value) &
            (other1.m_data.m_values[2] != other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> operator&(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(other1.m_data.m_values[0] & other2.m_data.m_values[0],
                other1.m_data.m_values[1] & other2.m_data.m_values[1],
                other1.m_data.m_values[2] & other2.m_data.m_values[2]);
        } else {
            return SIMD3<T, Width>(Shift::bitAnd(other1.m_data.m_values[0], other2.m_data.m_values[0]),
                Shift::bitAnd(other1.m_data.m_values[1], other2.m_data.m_values[1]),
                Shift::bitAnd(other1.m_data.m_values[2], other2.m_data.m_values[2]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> operator&(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(other1.m_data.m_values[0] & other2.m_data.m_value,
                other1.m_data.m_values[1] & other2.m_data.m_value, other1.m_data.m_values[2] & other2.m_data.m_value);
        } else {
            return SIMD3<T, Width>(Shift::bitAnd(other1.m_data.m_values[0], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[1], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[2], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> operator|(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(other1.m_data.m_values[0] | other2.m_data.m_values[0],
                other1.m_data.m_values[1] | other2.m_data.m_values[1],
                other1.m_data.m_values[2] | other2.m_data.m_values[2]);
        } else {
            return SIMD3<T, Width>(Shift::bitOr(other1.m_data.m_values[0], other2.m_data.m_values[0]),
                Shift::bitOr(other1.m_data.m_values[1], other2.m_data.m_values[1]),
                Shift::bitOr(other1.m_data.m_values[2], other2.m_data.m_values[2]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> operator|(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(other1.m_data.m_values[0] | other2.m_data.m_value,
                other1.m_data.m_values[1] | other2.m_data.m_value, other1.m_data.m_values[2] | other2.m_data.m_value);
        } else {
            return SIMD3<T, Width>(Shift::bitOr(other1.m_data.m_values[0], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[1], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[2], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> operator^(const SIMD3<T, Width>& other1, const SIMD3<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(other1.m_data.m_values[0] ^ other2.m_data.m_values[0],
                other1.m_data.m_values[1] ^ other2.m_data.m_values[1],
                other1.m_data.m_values[2] ^ other2.m_data.m_values[2]);
        } else {
            return SIMD3<T, Width>(Shift::bitXor(other1.m_data.m_values[0], other2.m_data.m_values[0]),
                Shift::bitXor(other1.m_data.m_values[1], other2.m_data.m_values[1]),
                Shift::bitXor(other1.m_data.m_values[2], other2.m_data.m_values[2]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width> operator^(const SIMD3<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD3<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(other1.m_data.m_values[0] ^ other2.m_data.m_value,
                other1.m_data.m_values[1] ^ other2.m_data.m_value, other1.m_data.m_values[2] ^ other2.m_data.m_value);
        } else {
            return SIMD3<T, Width>(Shift::bitXor(other1.m_data.m_values[0], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[1], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[2], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD3<T, Width> operator~(const SIMD3<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(~other.m_data.m_values[0], ~other.m_data.m_values[1], ~other.m_data.m_values[2]);
        } else {
            return SIMD3<T, Width>(Shift::bitNot(other.m_data.m_values[0]), Shift::bitNot(other.m_data.m_values[1]),
                Shift::bitNot(other.m_data.m_values[2]));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask operator&(
    const typename SIMD3<T, Width>::Mask& mask1, const typename SIMD3<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>::Mask(mask1.m_data.m_values[0] & mask2.m_data.m_values[0],
            mask1.m_data.m_values[1] & mask2.m_data.m_values[1], mask1.m_data.m_values[2] & mask2.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask operator|(
    const typename SIMD3<T, Width>::Mask& mask1, const typename SIMD3<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>::Mask(mask1.m_data.m_values[0] | mask2.m_data.m_values[0],
            mask1.m_data.m_values[1] | mask2.m_data.m_values[1], mask1.m_data.m_values[2] | mask2.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask operator^(
    const typename SIMD3<T, Width>::Mask& mask1, const typename SIMD3<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>::Mask(mask1.m_data.m_values[0] ^ mask2.m_data.m_values[0],
            mask1.m_data.m_values[1] ^ mask2.m_data.m_values[1], mask1.m_data.m_values[2] ^ mask2.m_data.m_values[2]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3<T, Width>::Mask operator~(const typename SIMD3<T, Width>::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width>::Mask(~mask.m_data.m_values[0], ~mask.m_data.m_values[1], ~mask.m_data.m_values[2]);
    }
}
} // namespace Shift
