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

#include "SIMD/XSSIMD4.hpp"
#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#endif
#include "XSMath.inl"

namespace Shift {
template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD4Data<T>::SIMD4Data(const SIMD4<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_value0 = other.m_data.m_values[0];
        m_value1 = other.m_data.m_values[1];
        m_value2 = other.m_data.m_values[2];
        m_value3 = other.m_data.m_values[3];
    }
}

template<typename T>
XS_INLINE void SIMD4Data<T>::setData(const T value0, const T value1, const T value2, const T value3) noexcept
{
    m_value0 = value0;
    m_value1 = value1;
    m_value2 = value2;
    m_value3 = value3;
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE void SIMD4Data<T>::store(const SIMD4<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_value0 = other.m_data.m_values[0];
        m_value1 = other.m_data.m_values[1];
        m_value2 = other.m_data.m_values[2];
        m_value3 = other.m_data.m_values[3];
    }
}

template<typename T>
template<SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4Data<T>::load() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>(m_value0, m_value1, m_value2, m_value3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::Mask::Mask(const uint32 mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = mask & 0x1;
        m_data.m_values[1] = mask & 0x2;
        m_data.m_values[2] = mask & 0x4;
        m_data.m_values[3] = mask & 0x8;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::Mask::Mask(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::Mask::Mask(const bool bool0, const bool bool1, const bool bool2, const bool bool3) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = bool0;
        m_data.m_values[1] = bool1;
        m_data.m_values[2] = bool2;
        m_data.m_values[3] = bool3;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE Bool4<hasSIMD<T> && (Width > SIMDWidth::Scalar)> SIMD4<T, Width>::Mask::getBool4() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Bool4 < hasSIMD<T> &&
            (Width > SIMDWidth::Scalar) >
            (m_data.m_values[0], m_data.m_values[1], m_data.m_values[2], m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD4<T, Width>::Mask::getAny() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return (m_data.m_values[0] | m_data.m_values[1]) | (m_data.m_values[2] | m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD4<T, Width>::Mask::getAll() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return (m_data.m_values[0] & m_data.m_values[1]) & (m_data.m_values[2] & m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE bool SIMD4<T, Width>::Mask::getNone() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return !getAny();
    }
}

template<typename T, SIMDWidth Width, uint32 Index>
class SIMDMasker4
{
public:
    XS_INLINE static SIMDInBase<T, Width>& ToType(SIMD4<T, Width>& other)
    {
        return reinterpret_cast<SIMDInBase<T, Width>*>(other.m_data.m_values)[Index];
    }
};

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD4<T, Width>::Mask::mask4Function(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if (m_data.m_values[0]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 0>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 0>>(value);
        }
        if (m_data.m_values[1]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 1>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 1>>(value);
        }
        if (m_data.m_values[2]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 2>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 2>>(value);
        }
        if (m_data.m_values[3]) {
            auto value = maskFunc.template expression<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 3>>();
            maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 3>>(value);
        }
    }
}

template<typename T, SIMDWidth Width>
template<typename MaskOperator>
XS_INLINE void SIMD4<T, Width>::Mask::mask4ElseFunction(MaskOperator& maskFunc) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        auto value = (m_data.m_values[0]) ?
            maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 0>>() :
            maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 0>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 0>>(value);
        value = (m_data.m_values[1]) ? maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 1>>() :
                                       maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 1>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 1>>(value);
        value = (m_data.m_values[2]) ? maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 2>>() :
                                       maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 2>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 2>>(value);
        value = (m_data.m_values[3]) ? maskFunc.template expression1<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 3>>() :
                                       maskFunc.template expression2<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 3>>();
        maskFunc.template finalExpression<SIMDInBase<T, Width>, SIMDMasker4<T, Width, 3>>(value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::Shuffle::Shuffle(
    const uint32 shuff0, const uint32 shuff1, const uint32 shuff2, const uint32 shuff3) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = shuff0;
        m_data.m_values[1] = shuff1;
        m_data.m_values[2] = shuff2;
        m_data.m_values[3] = shuff3;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::Shuffle::Shuffle(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::Shuffle::Shuffle(const uint32 shuffle) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = shuffle;
        m_data.m_values[1] = shuffle;
        m_data.m_values[2] = shuffle;
        m_data.m_values[3] = shuffle;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::Default() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(0, 1, 2, 3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::Reverse() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(3, 2, 1, 0);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::Reverse(const Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(0 ^ ((mask.m_data.m_values[0]) ? 3 : 0), 1 ^ ((mask.m_data.m_values[1]) ? 3 : 0),
            2 ^ ((mask.m_data.m_values[2]) ? 3 : 0), 3 ^ ((mask.m_data.m_values[3]) ? 3 : 0));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::Swap() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(2, 3, 0, 1);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::Swap(const Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(0 ^ ((mask.m_data.m_values[0]) ? 2 : 0), 1 ^ ((mask.m_data.m_values[1]) ? 2 : 0),
            2 ^ ((mask.m_data.m_values[2]) ? 2 : 0), 3 ^ ((mask.m_data.m_values[3]) ? 2 : 0));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::Swap2() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(1, 0, 3, 2);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::Swap2(const Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(0 ^ ((mask.m_data.m_values[0]) ? 1 : 0), 1 ^ ((mask.m_data.m_values[1]) ? 1 : 0),
            2 ^ ((mask.m_data.m_values[2]) ? 1 : 0), 3 ^ ((mask.m_data.m_values[3]) ? 1 : 0));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::RotateLeft(const uint32 rotate) noexcept
{
    XS_ASSERT(rotate < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle((rotate)&0x3, (1 + rotate) & 0x3, (2 + rotate) & 0x3, (3 + rotate) & 0x3);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::RotateRight(const uint32 rotate) noexcept
{
    XS_ASSERT(rotate < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle((0 - rotate) & 0x3, (1 - rotate) & 0x3, (2 - rotate) & 0x3, (3 - rotate) & 0x3);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE typename SIMD4<T, Width>::Shuffle SIMD4<T, Width>::Shuffle::shuffle() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Shuffle(
            m_data.m_values[Index0], m_data.m_values[Index1], m_data.m_values[Index2], m_data.m_values[Index3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::SIMD4(const T value0, const T value1, const T value2, const T value3) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = value0;
        m_data.m_values[1] = value1;
        m_data.m_values[2] = value2;
        m_data.m_values[3] = value3;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::SIMD4(const T value) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = value;
        m_data.m_values[1] = value;
        m_data.m_values[2] = value;
        m_data.m_values[3] = value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width>::SIMD4(const SIMDBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other.m_data.m_value;
        m_data.m_values[1] = other.m_data.m_value;
        m_data.m_values[2] = other.m_data.m_value;
        m_data.m_values[3] = other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width>::SIMD4(const SIMDInBase<T, Width2>& other) noexcept
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
        m_data.m_values[3] = other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width>::SIMD4(const SIMD3<T, Width2>& other0, const SIMDInBase<T, Width2>& other1) noexcept
{
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>> && sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other0.m_data.m_values[0];
        m_data.m_values[1] = other0.m_data.m_values[1];
        m_data.m_values[2] = other0.m_data.m_values[2];
        m_data.m_values[3] = other1.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width>::SIMD4(const SIMD3<T, Width2>& other0, const SIMDBase<T, Width2>& other1) noexcept
{
    XS_REQUIRES((sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>> && sameImpl<BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other0.m_data.m_values[0];
        m_data.m_values[1] = other0.m_data.m_values[1];
        m_data.m_values[2] = other0.m_data.m_values[2];
        m_data.m_values[3] = other1.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width>::SIMD4(const SIMD2<T, Width2>& other0, const SIMD2<T, Width2>& other1) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other0.m_data.m_values[0];
        m_data.m_values[1] = other0.m_data.m_values[1];
        m_data.m_values[2] = other1.m_data.m_values[0];
        m_data.m_values[3] = other1.m_data.m_values[1];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width>::SIMD4(const SIMD2<T, Width2>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[0] = other.m_data.m_values[0];
        m_data.m_values[1] = other.m_data.m_values[1];
        m_data.m_values[2] = other.m_data.m_values[0];
        m_data.m_values[3] = other.m_data.m_values[1];
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>::SIMD4(const Data& data) noexcept
    : m_data(data)
{}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::Zero() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(T{0});
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::One() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(T{1});
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE void SIMD4<T, Width>::Transpose(const SIMD3<T, Width>& other0, const SIMD3<T, Width>& other1,
    const SIMD3<T, Width>& other2, const SIMD3<T, Width>& other3, SIMD4<T, Width2>& otherT0, SIMD4<T, Width2>& otherT1,
    SIMD4<T, Width2>& otherT2) noexcept
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
        otherT0.m_data.m_values[3] = other3.m_data.m_values[0];
        otherT1.m_data.m_values[0] = other0.m_data.m_values[1];
        otherT1.m_data.m_values[1] = other1.m_data.m_values[1];
        otherT1.m_data.m_values[2] = other2.m_data.m_values[1];
        otherT1.m_data.m_values[3] = other3.m_data.m_values[1];
        otherT2.m_data.m_values[0] = other0.m_data.m_values[2];
        otherT2.m_data.m_values[1] = other1.m_data.m_values[2];
        otherT2.m_data.m_values[2] = other2.m_data.m_values[2];
        otherT2.m_data.m_values[3] = other3.m_data.m_values[2];
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE void SIMD4<T, Width>::Transpose(const SIMD4<T, Width>& other0, const SIMD4<T, Width>& other1,
    const SIMD4<T, Width>& other2, const SIMD4<T, Width>& other3, SIMD4<T, Width2>& otherT0, SIMD4<T, Width2>& otherT1,
    SIMD4<T, Width2>& otherT2, SIMD4<T, Width2>& otherT3) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        otherT0.m_data.m_values[0] = other0.m_data.m_values[0];
        otherT0.m_data.m_values[1] = other1.m_data.m_values[0];
        otherT0.m_data.m_values[2] = other2.m_data.m_values[0];
        otherT0.m_data.m_values[3] = other3.m_data.m_values[0];
        otherT1.m_data.m_values[0] = other0.m_data.m_values[1];
        otherT1.m_data.m_values[1] = other1.m_data.m_values[1];
        otherT1.m_data.m_values[2] = other2.m_data.m_values[1];
        otherT1.m_data.m_values[3] = other3.m_data.m_values[1];
        otherT2.m_data.m_values[0] = other0.m_data.m_values[2];
        otherT2.m_data.m_values[1] = other1.m_data.m_values[2];
        otherT2.m_data.m_values[2] = other2.m_data.m_values[2];
        otherT2.m_data.m_values[3] = other3.m_data.m_values[2];
        otherT3.m_data.m_values[0] = other0.m_data.m_values[3];
        otherT3.m_data.m_values[1] = other1.m_data.m_values[3];
        otherT3.m_data.m_values[2] = other2.m_data.m_values[3];
        otherT3.m_data.m_values[3] = other3.m_data.m_values[3];
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE SIMDInBase<T, Width2> SIMD4<T, Width>::getValueInBase() const noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 4);
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
XS_INLINE SIMDBase<T, Width2> SIMD4<T, Width>::getValue() const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
    static_assert(Index < 4);
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
XS_INLINE SIMDInBase<T, Width2> SIMD4<T, Width>::getValueInBase(const uint32 index) const noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    XS_ASSERT(index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDBase<T, Width2>(m_data.m_values[index]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMDBase<T, Width2> SIMD4<T, Width>::getValue(const uint32 index) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
    XS_ASSERT(index < 4);
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
XS_INLINE SIMD2<T, Width2> SIMD4<T, Width>::getValue2() const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width2>(m_data.m_values[Index * 2], m_data.m_values[Index * 2 + 1]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD2<T, Width2> SIMD4<T, Width>::getValue2() const noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
    static_assert(Index0 < 4 && Index1 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width2>(m_data.m_values[Index0], m_data.m_values[Index1]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, SIMDWidth Width2>
XS_INLINE SIMD3<T, Width2> SIMD4<T, Width>::getValue3() const noexcept
{
    static_assert(sameImpl<SIMD3<T, Width>, SIMD3<T, Width2>>);
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD3<T, Width2>(m_data.m_values[Index0], m_data.m_values[Index1], m_data.m_values[Index2]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD4<T, Width>::setValue(const SIMDBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
    static_assert(Index < 4);
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
XS_INLINE void SIMD4<T, Width>::setValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 4);
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
XS_INLINE void SIMD4<T, Width>::setValue(const uint32 index, const SIMDBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
    XS_ASSERT(index < 4);
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
XS_INLINE void SIMD4<T, Width>::setValue2(const SIMD2<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index * 2] = other.m_data.m_values[0];
        m_data.m_values[Index * 2 + 1] = other.m_data.m_values[1];
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index, SIMDWidth Width2>
XS_INLINE void SIMD4<T, Width>::addValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 4);
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
XS_INLINE void SIMD4<T, Width>::subValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 4);
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
XS_INLINE void SIMD4<T, Width>::mulValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 4);
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
XS_INLINE void SIMD4<T, Width>::divValue(const SIMDInBase<T, Width2>& other) noexcept
{
    static_assert(sameImpl<SIMDInBase<T, Width>, SIMDInBase<T, Width2>>);
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index] /= other.m_data.m_value;
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::negate() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Elem0 ? -m_data.m_values[0] : m_data.m_values[0], Elem1 ? -m_data.m_values[1] : m_data.m_values[1],
            Elem2 ? -m_data.m_values[2] : m_data.m_values[2], Elem3 ? -m_data.m_values[3] : m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::mad(const SIMD4<T, Width2>& other1, const SIMD4<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], other2.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::mad(const SIMDBase<T, Width2>& other1, const SIMD4<T, Width>& other2) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_value, other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_value, other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_value, other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_value, other2.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::mad(const SIMD4<T, Width>& other1, const SIMDBase<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[1], other2.m_data.m_values[1], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[2], other2.m_data.m_values[2], other2.m_data.m_value),
            Shift::fma<T>(m_data.m_values[3], other2.m_data.m_values[3], other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::msub(const SIMD4<T, Width2>& other1, const SIMD4<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_values[0], -other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_values[1], -other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_values[2], -other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_values[3], -other2.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::msub(const SIMDBase<T, Width2>& other1, const SIMD4<T, Width>& other2) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::fma<T>(m_data.m_values[0], other1.m_data.m_value, -other2.m_data.m_values[0]),
            Shift::fma<T>(m_data.m_values[1], other1.m_data.m_value, -other2.m_data.m_values[1]),
            Shift::fma<T>(m_data.m_values[2], other1.m_data.m_value, -other2.m_data.m_values[2]),
            Shift::fma<T>(m_data.m_values[3], other1.m_data.m_value, -other2.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::nmad(const SIMD4<T, Width2>& other1, const SIMD4<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::fma<T>(-m_data.m_values[0], other1.m_data.m_values[0], other2.m_data.m_values[0]),
            Shift::fma<T>(-m_data.m_values[1], other1.m_data.m_values[1], other2.m_data.m_values[1]),
            Shift::fma<T>(-m_data.m_values[2], other1.m_data.m_values[2], other2.m_data.m_values[2]),
            Shift::fma<T>(-m_data.m_values[3], other1.m_data.m_values[3], other2.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::nmsub(const SIMD4<T, Width2>& other1, const SIMD4<T, Width2>& other2) const
    noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::fma<T>(-m_data.m_values[0], other1.m_data.m_values[0], -other2.m_data.m_values[0]),
            Shift::fma<T>(-m_data.m_values[1], other1.m_data.m_values[1], -other2.m_data.m_values[1]),
            Shift::fma<T>(-m_data.m_values[2], other1.m_data.m_values[2], -other2.m_data.m_values[2]),
            Shift::fma<T>(-m_data.m_values[3], other1.m_data.m_values[3], -other2.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::subAdd(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(m_data.m_values[0] - other.m_data.m_values[0], m_data.m_values[1] + other.m_data.m_values[1],
            m_data.m_values[2] - other.m_data.m_values[2], m_data.m_values[3] + other.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::equalMask(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] == other.m_data.m_values[0], m_data.m_values[1] == other.m_data.m_values[1],
            m_data.m_values[2] == other.m_data.m_values[2], m_data.m_values[3] == other.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::lessOrEqualMask(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] <= other.m_data.m_values[0], m_data.m_values[1] <= other.m_data.m_values[1],
            m_data.m_values[2] <= other.m_data.m_values[2], m_data.m_values[3] <= other.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::lessThanMask(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] < other.m_data.m_values[0], m_data.m_values[1] < other.m_data.m_values[1],
            m_data.m_values[2] < other.m_data.m_values[2], m_data.m_values[3] < other.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::notEqualMask(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] != other.m_data.m_values[0], m_data.m_values[1] != other.m_data.m_values[1],
            m_data.m_values[2] != other.m_data.m_values[2], m_data.m_values[3] != other.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::equalMask(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] == other.m_data.m_value, m_data.m_values[1] == other.m_data.m_value,
            m_data.m_values[2] == other.m_data.m_value, m_data.m_values[3] == other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::lessOrEqualMask(const SIMDBase<T, Width2>& other) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] <= other.m_data.m_value, m_data.m_values[1] <= other.m_data.m_value,
            m_data.m_values[2] <= other.m_data.m_value, m_data.m_values[3] <= other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::lessThanMask(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] < other.m_data.m_value, m_data.m_values[1] < other.m_data.m_value,
            m_data.m_values[2] < other.m_data.m_value, m_data.m_values[3] < other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::greaterOrEqualMask(const SIMDBase<T, Width2>& other) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] >= other.m_data.m_value, m_data.m_values[1] >= other.m_data.m_value,
            m_data.m_values[2] >= other.m_data.m_value, m_data.m_values[3] >= other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::greaterThanMask(const SIMDBase<T, Width2>& other) const
    noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] > other.m_data.m_value, m_data.m_values[1] > other.m_data.m_value,
            m_data.m_values[2] > other.m_data.m_value, m_data.m_values[3] > other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD4<T, Width>::Mask SIMD4<T, Width>::notEqualMask(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return Mask(m_data.m_values[0] != other.m_data.m_value, m_data.m_values[1] != other.m_data.m_value,
            m_data.m_values[2] != other.m_data.m_value, m_data.m_values[3] != other.m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::sign(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::sign<T>(m_data.m_values[0], m_data.m_values[0]),
            Shift::sign<T>(m_data.m_values[1], m_data.m_values[1]),
            Shift::sign<T>(m_data.m_values[2], m_data.m_values[2]),
            Shift::sign<T>(m_data.m_values[3], m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::sign(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Elem0 ? Shift::sign<T>(m_data.m_values[0], other.m_data.m_values[0]) : m_data.m_values[0],
            Elem1 ? Shift::sign<T>(m_data.m_values[1], other.m_data.m_values[1]) : m_data.m_values[1],
            Elem2 ? Shift::sign<T>(m_data.m_values[2], other.m_data.m_values[2]) : m_data.m_values[2],
            Elem3 ? Shift::sign<T>(m_data.m_values[3], other.m_data.m_values[3]) : m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::sign(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::sign<T>(m_data.m_values[0], m_data.m_value),
            Shift::sign<T>(m_data.m_values[1], m_data.m_value), Shift::sign<T>(m_data.m_values[2], m_data.m_value),
            Shift::sign<T>(m_data.m_values[3], m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::abs() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::abs<T>(m_data.m_values[0]), Shift::abs<T>(m_data.m_values[1]),
            Shift::abs<T>(m_data.m_values[2]), Shift::abs<T>(m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::max(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::max<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::max<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::max<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::max<T>(m_data.m_values[3], other.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::min(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::min<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::min<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::min<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::min<T>(m_data.m_values[3], other.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::max(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::max<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::max<T>(m_data.m_values[3], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::min(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::min<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::min<T>(m_data.m_values[3], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD4<T, Width>::max2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::max<T>(m_data.m_values[0], m_data.m_values[2]),
            Shift::max<T>(m_data.m_values[1], m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD4<T, Width>::min2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2(Shift::min<T>(m_data.m_values[0], m_data.m_values[2]),
            Shift::min<T>(m_data.m_values[1], m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::BaseDef SIMD4<T, Width>::hmax() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        const T other = Shift::max<T>(m_data.m_values[0], m_data.m_values[1]);
        const T other2 = Shift::max<T>(m_data.m_values[2], m_data.m_values[3]);
        return BaseDef(Shift::max<T>(other2, other));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::BaseDef SIMD4<T, Width>::hmin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        const T other = Shift::min<T>(m_data.m_values[0], m_data.m_values[1]);
        const T other2 = Shift::min<T>(m_data.m_values[2], m_data.m_values[3]);
        return BaseDef(Shift::min<T>(other2, other));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD4<T, Width>::hmaxInBase() const noexcept
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
XS_INLINE SIMDInBase<T, Width> SIMD4<T, Width>::hminInBase() const noexcept
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
XS_INLINE typename SIMD4<T, Width>::BaseDef SIMD4<T, Width>::hmaxIndex(uint32& index) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        T other = Shift::max<T>(m_data.m_values[0], m_data.m_values[1]);
        index = (other == m_data.m_values[0]) ? 0 : 1;
        const T other2 = Shift::max<T>(m_data.m_values[2], m_data.m_values[3]);
        const uint32 index2 = (other2 == m_data.m_values[2]) ? 2 : 3;
        other = Shift::max<T>(other, other2);
        index = (other2 == other) ? index2 : index;
        return BaseDef(other);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::BaseDef SIMD4<T, Width>::hminIndex(uint32& index) const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        T other = Shift::min<T>(m_data.m_values[0], m_data.m_values[1]);
        index = (other == m_data.m_values[0]) ? 0 : 1;
        const T other2 = Shift::min<T>(m_data.m_values[2], m_data.m_values[3]);
        const uint32 index2 = (other2 == m_data.m_values[2]) ? 2 : 3;
        other = Shift::min<T>(other, other2);
        index = (other2 == other) ? index2 : index;
        return BaseDef(other);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD4<T, Width>::hmax2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(Shift::max<T>(m_data.m_values[0], m_data.m_values[1]),
            Shift::max<T>(m_data.m_values[2], m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD4<T, Width>::hmin2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(Shift::min<T>(m_data.m_values[0], m_data.m_values[1]),
            Shift::min<T>(m_data.m_values[2], m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::reciprocal() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(recip<T>(m_data.m_values[0]), recip<T>(m_data.m_values[1]), recip<T>(m_data.m_values[2]),
            recip<T>(m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD4<T, Width>::add2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(m_data.m_values[0] + m_data.m_values[2], m_data.m_values[1] + m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD4<T, Width>::sub2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(m_data.m_values[0] - m_data.m_values[2], m_data.m_values[1] - m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::BaseDef SIMD4<T, Width>::hadd() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        T res = (m_data.m_values[0] + m_data.m_values[1]);
        const T res2 = (m_data.m_values[2] + m_data.m_values[3]);
        res += res2;
        return BaseDef(res);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD4<T, Width>::haddInBase() const noexcept
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
XS_INLINE SIMD2<T, Width> SIMD4<T, Width>::hadd2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(m_data.m_values[0] + m_data.m_values[1], m_data.m_values[2] + m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD2<T, Width> SIMD4<T, Width>::hsub2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD2<T, Width>(m_data.m_values[0] - m_data.m_values[1], m_data.m_values[2] - m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::ceil() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::ceil<T>(m_data.m_values[0]), Shift::ceil<T>(m_data.m_values[1]),
            Shift::ceil<T>(m_data.m_values[2]), Shift::ceil<T>(m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::floor() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::floor<T>(m_data.m_values[0]), Shift::floor<T>(m_data.m_values[1]),
            Shift::floor<T>(m_data.m_values[2]), Shift::floor<T>(m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::trunc() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::trunc<T>(m_data.m_values[0]), Shift::trunc<T>(m_data.m_values[1]),
            Shift::trunc<T>(m_data.m_values[2]), Shift::trunc<T>(m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::sqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::sqrt<T>(m_data.m_values[0]), Shift::sqrt<T>(m_data.m_values[1]),
            Shift::sqrt<T>(m_data.m_values[2]), Shift::sqrt<T>(m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::rsqrt() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::rsqrt<T>(m_data.m_values[0]), Shift::rsqrt<T>(m_data.m_values[1]),
            Shift::rsqrt<T>(m_data.m_values[2]), Shift::rsqrt<T>(m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE typename SIMD4<T, Width>::BaseDef SIMD4<T, Width>::dot4(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        T res = (m_data.m_values[0] * other.m_data.m_values[0]);
        const T res2 = (m_data.m_values[1] * other.m_data.m_values[1]);
        T res3 = (m_data.m_values[2] * other.m_data.m_values[2]);
        const T res4 = (m_data.m_values[3] * other.m_data.m_values[3]);
        res += res2;
        res3 += res4;
        return BaseDef(res + res3);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMDInBase<T, Width> SIMD4<T, Width>::dot4InBase(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width>(dot4(other));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::BaseDef SIMD4<T, Width>::lengthSqr() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return dot4(*this);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::BaseDef SIMD4<T, Width>::length() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return BaseDef(Shift::sqrt<T>(lengthSqr().m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD4<T, Width>::lengthSqrInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width>(lengthSqr().m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> SIMD4<T, Width>::lengthInBase() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMDInBase<T, Width>(length().m_data.m_value);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::normalize() const noexcept
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
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::exp2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::exp2<T>(m_data.m_values[0]), Shift::exp2<T>(m_data.m_values[1]),
            Shift::exp2<T>(m_data.m_values[2]), Shift::exp2<T>(m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::exp() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::exp<T>(m_data.m_values[0]), Shift::exp<T>(m_data.m_values[1]),
            Shift::exp<T>(m_data.m_values[2]), Shift::exp<T>(m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::log2() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::log2<T>(m_data.m_values[0]), Shift::log2<T>(m_data.m_values[1]),
            Shift::log2<T>(m_data.m_values[2]), Shift::log2<T>(m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::log() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::log<T>(m_data.m_values[0]), Shift::log<T>(m_data.m_values[1]),
            Shift::log<T>(m_data.m_values[2]), Shift::log<T>(m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::pow(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::pow<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::pow<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::pow<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::pow<T>(m_data.m_values[3], other.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::powr(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::powr<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::powr<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::powr<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::powr<T>(m_data.m_values[3], other.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::pow(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::pow<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::pow<T>(m_data.m_values[3], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::powr(const SIMDBase<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::powr<T>(m_data.m_values[0], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[1], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[2], other.m_data.m_value),
            Shift::powr<T>(m_data.m_values[3], other.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::sin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::sin<T>(m_data.m_values[0]), Shift::sin<T>(m_data.m_values[1]),
            Shift::sin<T>(m_data.m_values[2]), Shift::sin<T>(m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::cos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::cos<T>(m_data.m_values[0]), Shift::cos<T>(m_data.m_values[1]),
            Shift::cos<T>(m_data.m_values[2]), Shift::cos<T>(m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::tan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::tan<T>(m_data.m_values[0]), Shift::tan<T>(m_data.m_values[1]),
            Shift::tan<T>(m_data.m_values[2]), Shift::tan<T>(m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD8<T, Width> SIMD4<T, Width>::sincos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        SIMD8 ret;
        ret.m_data.m_values[0] = Shift::sincos<T>(m_data.m_values[0], ret.m_data.m_values[4]);
        ret.m_data.m_values[1] = Shift::sincos<T>(m_data.m_values[1], ret.m_data.m_values[5]);
        ret.m_data.m_values[2] = Shift::sincos<T>(m_data.m_values[2], ret.m_data.m_values[6]);
        ret.m_data.m_values[3] = Shift::sincos<T>(m_data.m_values[3], ret.m_data.m_values[7]);
        return ret;
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::asin() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::asin<T>(m_data.m_values[0]), Shift::asin<T>(m_data.m_values[1]),
            Shift::asin<T>(m_data.m_values[2]), Shift::asin<T>(m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::acos() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::acos<T>(m_data.m_values[0]), Shift::acos<T>(m_data.m_values[1]),
            Shift::acos<T>(m_data.m_values[2]), Shift::acos<T>(m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::atan() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::atan<T>(m_data.m_values[0]), Shift::atan<T>(m_data.m_values[1]),
            Shift::atan<T>(m_data.m_values[2]), Shift::atan<T>(m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::atan2(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Shift::atan2<T>(m_data.m_values[0], other.m_data.m_values[0]),
            Shift::atan2<T>(m_data.m_values[1], other.m_data.m_values[1]),
            Shift::atan2<T>(m_data.m_values[2], other.m_data.m_values[2]),
            Shift::atan2<T>(m_data.m_values[3], other.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::insert(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
    static_assert(Index0 < 4 && Index1 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index0] = other.m_data.m_values[Index1];
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::insert2(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
    static_assert(Index0 < 2 && Index1 < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        m_data.m_values[Index0 * 2] = other.m_data.m_values[Index1 * 2];
        m_data.m_values[Index0 * 2 + 1] = other.m_data.m_values[Index1 * 2 + 1];
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::blend(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Elem0 ? other.m_data.m_values[0] : m_data.m_values[0],
            Elem1 ? other.m_data.m_values[1] : m_data.m_values[1],
            Elem2 ? other.m_data.m_values[2] : m_data.m_values[2],
            Elem3 ? other.m_data.m_values[3] : m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::blendVar(const SIMD4<T, Width2>& other, const Mask& mask) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4((mask.m_data.m_values[0]) ? other.m_data.m_values[0] : m_data.m_values[0],
            (mask.m_data.m_values[1]) ? other.m_data.m_values[1] : m_data.m_values[1],
            (mask.m_data.m_values[2]) ? other.m_data.m_values[2] : m_data.m_values[2],
            (mask.m_data.m_values[3]) ? other.m_data.m_values[3] : m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::blendSwap(SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        const SIMD4 f4Backup = other;
        other.m_data.m_values[0] = Elem0 ? m_data.m_values[0] : other.m_data.m_values[0];
        other.m_data.m_values[1] = Elem1 ? m_data.m_values[1] : other.m_data.m_values[1];
        other.m_data.m_values[2] = Elem2 ? m_data.m_values[2] : other.m_data.m_values[2];
        other.m_data.m_values[3] = Elem3 ? m_data.m_values[3] : other.m_data.m_values[3];
        return SIMD4(Elem0 ? f4Backup.m_data.m_values[0] : m_data.m_values[0],
            Elem1 ? f4Backup.m_data.m_values[1] : m_data.m_values[1],
            Elem2 ? f4Backup.m_data.m_values[2] : m_data.m_values[2],
            Elem3 ? f4Backup.m_data.m_values[3] : m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
template<SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::blendSwapVar(SIMD4<T, Width2>& other, const Mask& mask) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        const SIMD4 backup = other;
        other.m_data.m_values[0] = (mask.m_data.m_values[0]) ? m_data.m_values[0] : other.m_data.m_values[0];
        other.m_data.m_values[1] = (mask.m_data.m_values[1]) ? m_data.m_values[1] : other.m_data.m_values[1];
        other.m_data.m_values[2] = (mask.m_data.m_values[2]) ? m_data.m_values[2] : other.m_data.m_values[2];
        other.m_data.m_values[3] = (mask.m_data.m_values[3]) ? m_data.m_values[3] : other.m_data.m_values[3];
        return SIMD4((mask.m_data.m_values[0]) ? backup.m_data.m_values[0] : m_data.m_values[0],
            (mask.m_data.m_values[1]) ? backup.m_data.m_values[1] : m_data.m_values[1],
            (mask.m_data.m_values[2]) ? backup.m_data.m_values[2] : m_data.m_values[2],
            (mask.m_data.m_values[3]) ? backup.m_data.m_values[3] : m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
template<bool Elem0, bool Elem1, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::blend2(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(Elem0 ? other.m_data.m_values[0] : m_data.m_values[0],
            Elem0 ? other.m_data.m_values[1] : m_data.m_values[1],
            Elem1 ? other.m_data.m_values[2] : m_data.m_values[2],
            Elem1 ? other.m_data.m_values[3] : m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::shuffle() const noexcept
{
    static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(
            m_data.m_values[Index0], m_data.m_values[Index1], m_data.m_values[Index2], m_data.m_values[Index3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::shuffleVar(const Shuffle& shuffle) const noexcept
{
    XS_ASSERT(shuffle.m_data.m_values[0] < 4 && shuffle.m_data.m_values[1] < 4 && shuffle.m_data.m_values[2] < 4 &&
        shuffle.m_data.m_values[3] < 4);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(m_data.m_values[shuffle.m_data.m_values[0]], m_data.m_values[shuffle.m_data.m_values[1]],
            m_data.m_values[shuffle.m_data.m_values[2]], m_data.m_values[shuffle.m_data.m_values[3]]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::shuffle2() const noexcept
{
    static_assert(Index0 < 2 && Index1 < 2);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(m_data.m_values[Index0 * 2], m_data.m_values[Index0 * 2 + 1], m_data.m_values[Index1 * 2],
            m_data.m_values[Index1 * 2 + 1]);
    }
}

template<typename T, SIMDWidth Width>
template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> SIMD4<T, Width>::combine(const SIMD4<T, Width2>& other) const noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
    static_assert(Index0 < 8 && Index1 < 8 && Index2 < 8 && Index3 < 8);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4(((Index0 < 4) ? *this : other).m_data.m_values[Index0 % 4],
            ((Index0 < 4) ? *this : other).m_data.m_values[Index1 % 4],
            ((Index0 < 4) ? *this : other).m_data.m_values[Index2 % 4],
            ((Index0 < 4) ? *this : other).m_data.m_values[Index3 % 4]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator+(const SIMD4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>((other1.m_data.m_values[0] + other2.m_data.m_values[0]),
            (other1.m_data.m_values[1] + other2.m_data.m_values[1]),
            (other1.m_data.m_values[2] + other2.m_data.m_values[2]),
            (other1.m_data.m_values[3] + other2.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator+(const SIMD4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>((other1.m_data.m_values[0] + other2.m_data.m_value),
            (other1.m_data.m_values[1] + other2.m_data.m_value), (other1.m_data.m_values[2] + other2.m_data.m_value),
            (other1.m_data.m_values[3] + other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator+(const SIMD4<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>((other1.m_data.m_values[0] + other2.m_data.m_values[0]),
            (other1.m_data.m_values[1] + other2.m_data.m_values[1]),
            (other1.m_data.m_values[2] + other2.m_data.m_values[0]),
            (other1.m_data.m_values[3] + other2.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator-(const SIMD4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>((other1.m_data.m_values[0] - other2.m_data.m_values[0]),
            (other1.m_data.m_values[1] - other2.m_data.m_values[1]),
            (other1.m_data.m_values[2] - other2.m_data.m_values[2]),
            (other1.m_data.m_values[3] - other2.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator-(const SIMD4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD4<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>((other1.m_data.m_values[0] - other2.m_data.m_value),
            (other1.m_data.m_values[1] - other2.m_data.m_value), (other1.m_data.m_values[2] - other2.m_data.m_value),
            (other1.m_data.m_values[3] - other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator-(const SIMDBase<T, Width2>& other1, const SIMD4<T, Width>& other2) noexcept
{
    XS_REQUIRES((sameImpl<typename SIMD4<T, Width>::BaseDef, SIMDBase<T, Width2>>))
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4((other1.m_data.m_value - other2.m_data.m_values[0]),
            (other1.m_data.m_value - other2.m_data.m_values[1]), (other1.m_data.m_value - other2.m_data.m_values[2]),
            (other1.m_data.m_value - other2.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator-(const SIMD4<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>((other1.m_data.m_values[0] - other2.m_data.m_values[0]),
            (other1.m_data.m_values[1] - other2.m_data.m_values[1]),
            (other1.m_data.m_values[2] - other2.m_data.m_values[0]),
            (other1.m_data.m_values[3] - other2.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator*(const SIMD4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>((other1.m_data.m_values[0] * other2.m_data.m_values[0]),
            (other1.m_data.m_values[1] * other2.m_data.m_values[1]),
            (other1.m_data.m_values[2] * other2.m_data.m_values[2]),
            (other1.m_data.m_values[3] * other2.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator*(const SIMD4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>((other1.m_data.m_values[0] * other2.m_data.m_value),
            (other1.m_data.m_values[1] * other2.m_data.m_value), (other1.m_data.m_values[2] * other2.m_data.m_value),
            (other1.m_data.m_values[3] * other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator*(const SIMD4<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>((other1.m_data.m_values[0] * other2.m_data.m_values[0]),
            (other1.m_data.m_values[1] * other2.m_data.m_values[1]),
            (other1.m_data.m_values[2] * other2.m_data.m_values[0]),
            (other1.m_data.m_values[3] * other2.m_data.m_values[1]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator/(const SIMD4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>((other1.m_data.m_values[0] / other2.m_data.m_values[0]),
            (other1.m_data.m_values[1] / other2.m_data.m_values[1]),
            (other1.m_data.m_values[2] / other2.m_data.m_values[2]),
            (other1.m_data.m_values[3] / other2.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator/(const SIMD4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD4<T, Width>((other1.m_data.m_values[0] / other2.m_data.m_value),
                (other1.m_data.m_values[1] / other2.m_data.m_value),
                (other1.m_data.m_values[2] / other2.m_data.m_value),
                (other1.m_data.m_values[3] / other2.m_data.m_value));
        } else {
            const T divisor = recip<T>(other2.m_data.m_value);
            return SIMD4<T, Width>((other1.m_data.m_values[0] * divisor), (other1.m_data.m_values[1] * divisor),
                (other1.m_data.m_values[2] * divisor), (other1.m_data.m_values[3] * divisor));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator/(const SIMDBase<T, Width2>& other1, const SIMD4<T, Width>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>((other1.m_data.m_value / other2.m_data.m_values[0]),
            (other1.m_data.m_value / other2.m_data.m_values[1]), (other1.m_data.m_value / other2.m_data.m_values[2]),
            (other1.m_data.m_value / other2.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator/(const SIMD4<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            return SIMD4<T, Width>((other1.m_data.m_values[0] / other2.m_data.m_values[0]),
                (other1.m_data.m_values[1] / other2.m_data.m_values[1]),
                (other1.m_data.m_values[2] / other2.m_data.m_values[0]),
                (other1.m_data.m_values[3] / other2.m_data.m_values[1]));
        } else {
            const T divisor0 = recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = recip<T>(other2.m_data.m_values[1]);
            return SIMD4<T, Width>((other1.m_data.m_values[0] * divisor0), (other1.m_data.m_values[1] * divisor0),
                (other1.m_data.m_values[2] * divisor1), (other1.m_data.m_values[3] * divisor1));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator-(const SIMD4<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>(
            -other.m_data.m_values[0], -other.m_data.m_values[1], -other.m_data.m_values[2], -other.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width>& operator+=(SIMD4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] += other2.m_data.m_values[0];
        other1.m_data.m_values[1] += other2.m_data.m_values[1];
        other1.m_data.m_values[2] += other2.m_data.m_values[2];
        other1.m_data.m_values[3] += other2.m_data.m_values[3];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width>& operator+=(SIMD4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] += other2.m_data.m_value;
        other1.m_data.m_values[1] += other2.m_data.m_value;
        other1.m_data.m_values[2] += other2.m_data.m_value;
        other1.m_data.m_values[3] += other2.m_data.m_value;
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width>& operator+=(SIMD4<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] += other2.m_data.m_values[0];
        other1.m_data.m_values[1] += other2.m_data.m_values[1];
        other1.m_data.m_values[2] += other2.m_data.m_values[0];
        other1.m_data.m_values[3] += other2.m_data.m_values[1];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width>& operator-=(SIMD4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] -= other2.m_data.m_values[0];
        other1.m_data.m_values[1] -= other2.m_data.m_values[1];
        other1.m_data.m_values[2] -= other2.m_data.m_values[2];
        other1.m_data.m_values[3] -= other2.m_data.m_values[3];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width>& operator-=(SIMD4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] -= other2.m_data.m_value;
        other1.m_data.m_values[1] -= other2.m_data.m_value;
        other1.m_data.m_values[2] -= other2.m_data.m_value;
        other1.m_data.m_values[3] -= other2.m_data.m_value;
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width>& operator-=(SIMD4<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] -= other2.m_data.m_values[0];
        other1.m_data.m_values[1] -= other2.m_data.m_values[1];
        other1.m_data.m_values[2] -= other2.m_data.m_values[0];
        other1.m_data.m_values[3] -= other2.m_data.m_values[1];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width>& operator*=(SIMD4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] *= other2.m_data.m_values[0];
        other1.m_data.m_values[1] *= other2.m_data.m_values[1];
        other1.m_data.m_values[2] *= other2.m_data.m_values[2];
        other1.m_data.m_values[3] *= other2.m_data.m_values[3];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width>& operator*=(SIMD4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] *= other2.m_data.m_value;
        other1.m_data.m_values[1] *= other2.m_data.m_value;
        other1.m_data.m_values[2] *= other2.m_data.m_value;
        other1.m_data.m_values[3] *= other2.m_data.m_value;
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width>& operator*=(SIMD4<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] *= other2.m_data.m_values[0];
        other1.m_data.m_values[1] *= other2.m_data.m_values[1];
        other1.m_data.m_values[2] *= other2.m_data.m_values[0];
        other1.m_data.m_values[3] *= other2.m_data.m_values[1];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width>& operator/=(SIMD4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        other1.m_data.m_values[0] /= other2.m_data.m_values[0];
        other1.m_data.m_values[1] /= other2.m_data.m_values[1];
        other1.m_data.m_values[2] /= other2.m_data.m_values[2];
        other1.m_data.m_values[3] /= other2.m_data.m_values[3];
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width>& operator/=(SIMD4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            other1.m_data.m_values[0] /= other2.m_data.m_value;
            other1.m_data.m_values[1] /= other2.m_data.m_value;
            other1.m_data.m_values[2] /= other2.m_data.m_value;
            other1.m_data.m_values[3] /= other2.m_data.m_value;
        } else {
            const T divisor = recip<T>(other2.m_data.m_value);
            other1.m_data.m_values[0] *= divisor;
            other1.m_data.m_values[1] *= divisor;
            other1.m_data.m_values[2] *= divisor;
            other1.m_data.m_values[3] *= divisor;
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width>& operator/=(SIMD4<T, Width>& other1, const SIMD2<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD2<T, Width>, SIMD2<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (defaultPrecision == Precision::Precise) {
            other1.m_data.m_values[0] /= other2.m_data.m_values[0];
            other1.m_data.m_values[1] /= other2.m_data.m_values[1];
            other1.m_data.m_values[2] /= other2.m_data.m_values[0];
            other1.m_data.m_values[3] /= other2.m_data.m_values[1];
        } else {
            const T divisor0 = recip<T>(other2.m_data.m_values[0]);
            const T divisor1 = recip<T>(other2.m_data.m_values[1]);
            other1.m_data.m_values[0] *= divisor0;
            other1.m_data.m_values[1] *= divisor1;
            other1.m_data.m_values[2] *= divisor0;
            other1.m_data.m_values[3] *= divisor1;
        }
    }
    return other1;
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator==(const SIMD4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] == other2.m_data.m_values[0]) &
            (other1.m_data.m_values[1] == other2.m_data.m_values[1]) &
            (other1.m_data.m_values[2] == other2.m_data.m_values[2]) &
            (other1.m_data.m_values[3] == other2.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<=(const SIMD4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] <= other2.m_data.m_values[0]) &
            (other1.m_data.m_values[1] <= other2.m_data.m_values[1]) &
            (other1.m_data.m_values[2] <= other2.m_data.m_values[2]) &
            (other1.m_data.m_values[3] <= other2.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<(const SIMD4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] < other2.m_data.m_values[0]) &
            (other1.m_data.m_values[1] < other2.m_data.m_values[1]) &
            (other1.m_data.m_values[2] < other2.m_data.m_values[2]) &
            (other1.m_data.m_values[3] < other2.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator!=(const SIMD4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] != other2.m_data.m_values[0]) &
            (other1.m_data.m_values[1] != other2.m_data.m_values[1]) &
            (other1.m_data.m_values[2] != other2.m_data.m_values[2]) &
            (other1.m_data.m_values[3] != other2.m_data.m_values[3]));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator==(const SIMD4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] == other2.m_data.m_value) &
            (other1.m_data.m_values[1] == other2.m_data.m_value) &
            (other1.m_data.m_values[2] == other2.m_data.m_value) &
            (other1.m_data.m_values[3] == other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<=(const SIMD4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] <= other2.m_data.m_value) &
            (other1.m_data.m_values[1] <= other2.m_data.m_value) &
            (other1.m_data.m_values[2] <= other2.m_data.m_value) &
            (other1.m_data.m_values[3] <= other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator<(const SIMD4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] < other2.m_data.m_value) &
            (other1.m_data.m_values[1] < other2.m_data.m_value) & (other1.m_data.m_values[2] < other2.m_data.m_value) &
            (other1.m_data.m_values[3] < other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE bool operator!=(const SIMD4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return ((other1.m_data.m_values[0] != other2.m_data.m_value) &
            (other1.m_data.m_values[1] != other2.m_data.m_value) &
            (other1.m_data.m_values[2] != other2.m_data.m_value) &
            (other1.m_data.m_values[3] != other2.m_data.m_value));
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator&(const SIMD4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(other1.m_data.m_values[0] & other2.m_data.m_values[0],
                other1.m_data.m_values[1] & other2.m_data.m_values[1],
                other1.m_data.m_values[2] & other2.m_data.m_values[2],
                other1.m_data.m_values[3] & other2.m_data.m_values[3]);
        } else {
            return SIMD4<T, Width>(Shift::bitAnd(other1.m_data.m_values[0], other2.m_data.m_values[0]),
                Shift::bitAnd(other1.m_data.m_values[1], other2.m_data.m_values[1]),
                Shift::bitAnd(other1.m_data.m_values[2], other2.m_data.m_values[2]),
                Shift::bitAnd(other1.m_data.m_values[3], other2.m_data.m_values[3]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator&(const SIMD4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(other1.m_data.m_values[0] & other2.m_data.m_value,
                other1.m_data.m_values[1] & other2.m_data.m_value, other1.m_data.m_values[2] & other2.m_data.m_value,
                other1.m_data.m_values[3] & other2.m_data.m_value);
        } else {
            return SIMD4<T, Width>(Shift::bitAnd(other1.m_data.m_values[0], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[1], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[2], other2.m_data.m_value),
                Shift::bitAnd(other1.m_data.m_values[3], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator|(const SIMD4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(other1.m_data.m_values[0] | other2.m_data.m_values[0],
                other1.m_data.m_values[1] | other2.m_data.m_values[1],
                other1.m_data.m_values[2] | other2.m_data.m_values[2],
                other1.m_data.m_values[3] | other2.m_data.m_values[3]);
        } else {
            return SIMD4<T, Width>(Shift::bitOr(other1.m_data.m_values[0], other2.m_data.m_values[0]),
                Shift::bitOr(other1.m_data.m_values[1], other2.m_data.m_values[1]),
                Shift::bitOr(other1.m_data.m_values[2], other2.m_data.m_values[2]),
                Shift::bitOr(other1.m_data.m_values[3], other2.m_data.m_values[3]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator|(const SIMD4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(other1.m_data.m_values[0] | other2.m_data.m_value,
                other1.m_data.m_values[1] | other2.m_data.m_value, other1.m_data.m_values[2] | other2.m_data.m_value,
                other1.m_data.m_values[3] | other2.m_data.m_value);
        } else {
            return SIMD4<T, Width>(Shift::bitOr(other1.m_data.m_values[0], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[1], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[2], other2.m_data.m_value),
                Shift::bitOr(other1.m_data.m_values[3], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator^(const SIMD4<T, Width>& other1, const SIMD4<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<SIMD4<T, Width>, SIMD4<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(other1.m_data.m_values[0] ^ other2.m_data.m_values[0],
                other1.m_data.m_values[1] ^ other2.m_data.m_values[1],
                other1.m_data.m_values[2] ^ other2.m_data.m_values[2],
                other1.m_data.m_values[3] ^ other2.m_data.m_values[3]);
        } else {
            return SIMD4<T, Width>(Shift::bitXor(other1.m_data.m_values[0], other2.m_data.m_values[0]),
                Shift::bitXor(other1.m_data.m_values[1], other2.m_data.m_values[1]),
                Shift::bitXor(other1.m_data.m_values[2], other2.m_data.m_values[2]),
                Shift::bitXor(other1.m_data.m_values[3], other2.m_data.m_values[3]));
        }
    }
}

template<typename T, SIMDWidth Width, SIMDWidth Width2>
XS_INLINE SIMD4<T, Width> operator^(const SIMD4<T, Width>& other1, const SIMDBase<T, Width2>& other2) noexcept
{
    static_assert(sameImpl<typename SIMD4<T, Width>::BaseDef, SIMDBase<T, Width2>>);
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(other1.m_data.m_values[0] ^ other2.m_data.m_value,
                other1.m_data.m_values[1] ^ other2.m_data.m_value, other1.m_data.m_values[2] ^ other2.m_data.m_value,
                other1.m_data.m_values[3] ^ other2.m_data.m_value);
        } else {
            return SIMD4<T, Width>(Shift::bitXor(other1.m_data.m_values[0], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[1], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[2], other2.m_data.m_value),
                Shift::bitXor(other1.m_data.m_values[3], other2.m_data.m_value));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator~(const SIMD4<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(~other.m_data.m_values[0], ~other.m_data.m_values[1], ~other.m_data.m_values[2],
                ~other.m_data.m_values[3]);
        } else {
            return SIMD4<T, Width>(Shift::bitNot(other.m_data.m_values[0]), Shift::bitNot(other.m_data.m_values[1]),
                Shift::bitNot(other.m_data.m_values[2]), Shift::bitNot(other.m_data.m_values[3]));
        }
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask operator&(
    const typename SIMD4<T, Width>::Mask& mask1, const typename SIMD4<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>::Mask(mask1.m_data.m_values[0] & mask2.m_data.m_values[0],
            mask1.m_data.m_values[1] & mask2.m_data.m_values[1], mask1.m_data.m_values[2] & mask2.m_data.m_values[2],
            mask1.m_data.m_values[3] & mask2.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask operator|(
    const typename SIMD4<T, Width>::Mask& mask1, const typename SIMD4<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>::Mask(mask1.m_data.m_values[0] | mask2.m_data.m_values[0],
            mask1.m_data.m_values[1] | mask2.m_data.m_values[1], mask1.m_data.m_values[2] | mask2.m_data.m_values[2],
            mask1.m_data.m_values[3] | mask2.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask operator^(
    const typename SIMD4<T, Width>::Mask& mask1, const typename SIMD4<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>::Mask(mask1.m_data.m_values[0] ^ mask2.m_data.m_values[0],
            mask1.m_data.m_values[1] ^ mask2.m_data.m_values[1], mask1.m_data.m_values[2] ^ mask2.m_data.m_values[2],
            mask1.m_data.m_values[3] ^ mask2.m_data.m_values[3]);
    }
}

template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD4<T, Width>::Mask operator~(const typename SIMD4<T, Width>::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
    } else
#endif
    {
        return SIMD4<T, Width>::Mask(
            ~mask.m_data.m_values[0], ~mask.m_data.m_values[1], ~mask.m_data.m_values[2], ~mask.m_data.m_values[3]);
    }
}
} // namespace Shift
