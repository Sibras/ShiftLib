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

#include "SIMD/XSSIMDBool.hpp"
#include "XSBit.inl"
#include "XSMath.inl"

namespace Shift {
template<bool Type>
XS_INLINE constexpr Bool2<Type>::Bool2(const uint8 bool2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value = bool2;
    } else
#endif
    {
        m_data.m_values[0] = bool2 & 0x1_ui8;
        m_data.m_values[1] = bool2 & 0x2_ui8;
    }
}

template<bool Type>
XS_INLINE constexpr Bool2<Type>::Bool2(const bool bool0, const bool bool1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value = static_cast<uint8>(bool0) | (static_cast<uint8>(bool1) << 1_ui8);
    } else
#endif
    {
        m_data.m_values[0] = bool0;
        m_data.m_values[1] = bool1;
    }
}

template<bool Type>
XS_INLINE constexpr Bool2<Type> Bool2<Type>::True() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return Bool2(0x3_ui8);
    } else
#endif
    {
        return Bool2(true, true);
    }
}

template<bool Type>
XS_INLINE constexpr Bool2<Type> Bool2<Type>::False() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return Bool2(0x0_ui8);
    } else
#endif
    {
        return Bool2(false, false);
    }
}

template<bool Type>
template<uint32 Index>
XS_INLINE bool Bool2<Type>::getBool() const noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return bitExtract(m_data.m_value, Index);
    } else
#endif
    {
        return m_data.m_values[Index];
    }
}

template<bool Type>
template<uint32 Index>
XS_INLINE constexpr void Bool2<Type>::setBoolTrue() noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value |= (1_ui8 << Index);
    } else
#endif
    {
        m_data.m_values[Index] = true;
    }
}

template<bool Type>
template<uint32 Index>
XS_INLINE constexpr void Bool2<Type>::setBoolFalse() noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value &= ~(1_ui8 << Index);
    } else
#endif
    {
        m_data.m_values[Index] = false;
    }
}

template<bool Type>
XS_INLINE constexpr bool Bool2<Type>::getAny() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->m_data.m_value &= 0x3_ui8;
        return this->m_data.m_value;
    } else
#endif
    {
        return m_data.m_values[0] | m_data.m_values[1];
    }
}

template<bool Type>
XS_INLINE constexpr bool Bool2<Type>::getAll() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->m_data.m_value &= 0x3_ui8;
        return this->m_data.m_value == 0x3_ui8;
    } else
#endif
    {
        return m_data.m_values[0] & m_data.m_values[1];
    }
}

template<bool Type>
XS_INLINE constexpr bool Bool2<Type>::getNone() noexcept
{
    return !getAny();
}

template<bool Type>
XS_INLINE bool Bool2<Type>::getBool(const uint32 index) const noexcept
{
    XS_ASSERT(index < 2);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return bitExtract(this->m_data.m_value, index);
    } else
#endif
    {
        return m_data.m_values[index];
    }
}

template<bool Type>
XS_INLINE void Bool2<Type>::setBoolTrue(const uint32 index) noexcept
{
    XS_ASSERT(index < 2);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        this->m_data.m_value = bitSet(this->m_data.m_value, index);
    } else
#endif
    {
        m_data.m_values[index] = true;
    }
}

template<bool Type>
XS_INLINE void Bool2<Type>::setBoolFalse(const uint32 index) noexcept
{
    XS_ASSERT(index < 2);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        this->m_data.m_value = bitClear(this->m_data.m_value, index);
    } else
#endif
    {
        m_data.m_values[index] = false;
    }
}

template<bool Type>
XS_INLINE uint32 Bool2<Type>::indexOfFirstValid() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->m_data.m_value &= 0x3_ui8;
        return ctz(this->m_data.m_value);
    } else
#endif
    {
        return (m_data.m_values[0]) ? 0_ui32 : 1_ui32;
    }
}

template<bool Type>
XS_INLINE constexpr uint8 Bool2<Type>::getAsInteger() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->m_data.m_value &= 0x3_ui8;
        return this->m_data.m_value;
    } else
#endif
    {
        return static_cast<uint8>(m_data.m_values[0]) | (static_cast<uint8>(m_data.m_values[1]) << 1_ui8);
    }
}

template<bool Type>
XS_INLINE constexpr Bool3<Type>::Bool3(const uint8 bool3) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value = bool3;
    } else
#endif
    {
        m_data.m_values[0] = bool3 & 0x1_ui8;
        m_data.m_values[1] = bool3 & 0x2_ui8;
        m_data.m_values[2] = bool3 & 0x4_ui8;
    }
}

template<bool Type>
XS_INLINE constexpr Bool3<Type>::Bool3(const bool bool0, const bool bool1, const bool bool2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value =
            static_cast<uint8>(bool0) | (static_cast<uint8>(bool1) << 1_ui8) | (static_cast<uint8>(bool2) << 2_ui8);
    } else
#endif
    {
        m_data.m_values[0] = bool0;
        m_data.m_values[1] = bool1;
        m_data.m_values[2] = bool2;
    }
}

template<bool Type>
XS_INLINE constexpr Bool3<Type> Bool3<Type>::True() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return Bool3(0x7);
    } else
#endif
    {
        return Bool3(true, true, true);
    }
}

template<bool Type>
XS_INLINE constexpr Bool3<Type> Bool3<Type>::False() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return Bool3(0x0_ui8);
    } else
#endif
    {
        return Bool3(false, false, false);
    }
}

template<bool Type>
template<uint32 Index>
XS_INLINE bool Bool3<Type>::getBool() const noexcept
{
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return bitExtract(m_data.m_value, Index);
    } else
#endif
    {
        return m_data.m_values[Index];
    }
}

template<bool Type>
template<uint32 Index>
XS_INLINE constexpr void Bool3<Type>::setBoolTrue() noexcept
{
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value |= (1_ui8 << Index);
    } else
#endif
    {
        m_data.m_values[Index] = true;
    }
}

template<bool Type>
template<uint32 Index>
XS_INLINE constexpr void Bool3<Type>::setBoolFalse() noexcept
{
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value &= ~(1_ui8 << Index);
    } else
#endif
    {
        m_data.m_values[Index] = false;
    }
}

template<bool Type>
XS_INLINE constexpr bool Bool3<Type>::getAny() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->m_data.m_value &= 0x7_ui8;
        return this->m_data.m_value;
    } else
#endif
    {
        return (m_data.m_values[0] | m_data.m_values[1] | m_data.m_values[2]);
    }
}

template<bool Type>
XS_INLINE constexpr bool Bool3<Type>::getAll() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->m_data.m_value &= 0x7_ui8;
        return this->m_data.m_value == 0x7_ui8;
    } else
#endif
    {
        return (m_data.m_values[0] & m_data.m_values[1] & m_data.m_values[2]);
    }
}

template<bool Type>
XS_INLINE constexpr bool Bool3<Type>::getNone() noexcept
{
    return !getAny();
}

template<bool Type>
XS_INLINE bool Bool3<Type>::getBool(const uint32 index) const noexcept
{
    XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return bitExtract(this->m_data.m_value, index);
    } else
#endif
    {
        return m_data.m_values[index];
    }
}

template<bool Type>
XS_INLINE void Bool3<Type>::setBoolTrue(const uint32 index) noexcept
{
    XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        this->m_data.m_value = bitSet(this->m_data.m_value, index);
    } else
#endif
    {
        m_data.m_values[index] = true;
    }
}

template<bool Type>
XS_INLINE void Bool3<Type>::setBoolFalse(const uint32 index) noexcept
{
    XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        this->m_data.m_value = bitClear(this->m_data.m_value, index);
    } else
#endif
    {
        m_data.m_values[index] = false;
    }
}

template<bool Type>
XS_INLINE uint32 Bool3<Type>::indexOfFirstValid() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->m_data.m_value &= 0x7_ui8;
        return ctz(this->m_data.m_value);
    } else
#endif
    {
        return (m_data.m_values[0]) ? 0_ui32 : ((m_data.m_values[1]) ? 1_ui32 : 2_ui32);
    }
}

template<bool Type>
XS_INLINE constexpr uint8 Bool3<Type>::getAsInteger() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->m_data.m_value &= 0x7_ui8;
        return this->m_data.m_value;
    } else
#endif
    {
        return static_cast<uint8>(m_data.m_values[0]) | (static_cast<uint8>(m_data.m_values[1]) << 1_ui8) |
            (static_cast<uint8>(m_data.m_values[2]) << 2_ui8);
    }
}

template<bool Type>
XS_INLINE constexpr Bool3x2<Type>::Bool3x2(const uint8 bool3x2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value = bool3x2;
    } else
#endif
    {
        m_data.m_values[0] = bool3x2 & 0x1_ui8;
        m_data.m_values[1] = bool3x2 & 0x2_ui8;
        m_data.m_values[2] = bool3x2 & 0x4_ui8;
        m_data.m_values[3] = bool3x2 & 0x8_ui8;
        m_data.m_values[4] = bool3x2 & 0x10_ui8;
        m_data.m_values[5] = bool3x2 & 0x20_ui8;
    }
}

template<bool Type>
XS_INLINE constexpr Bool3x2<Type>::Bool3x2(const uint8 bool30, const uint8 bool31) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value = bool30 | (bool31 << 4_ui8);
    } else
#endif
    {
        m_data.m_values[0] = bool30 & 0x1_ui8;
        m_data.m_values[1] = bool30 & 0x2_ui8;
        m_data.m_values[2] = bool30 & 0x4_ui8;
        m_data.m_values[3] = bool31 & 0x1_ui8;
        m_data.m_values[4] = bool31 & 0x2_ui8;
        m_data.m_values[5] = bool31 & 0x4_ui8;
    }
}

template<bool Type>
XS_INLINE constexpr Bool3x2<Type>::Bool3x2(
    const bool bool0, const bool bool1, const bool bool2, const bool bool3, const bool bool4, const bool bool5) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value = static_cast<uint8>(bool0) | (static_cast<uint8>(bool1) << 1_ui8) |
            (static_cast<uint8>(bool2) << 2_ui8) | (static_cast<uint8>(bool3) << 4_ui8) |
            (static_cast<uint8>(bool4) << 5_ui8) | (static_cast<uint8>(bool5) << 6_ui8);
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

template<bool Type>
XS_INLINE constexpr Bool3x2<Type> Bool3x2<Type>::True() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return Bool3x2(0x77_ui8);
    } else
#endif
    {
        return Bool3x2(true, true, true, true, true, true);
    }
}

template<bool Type>
XS_INLINE constexpr Bool3x2<Type> Bool3x2<Type>::False() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return Bool3x2(0x0_ui8);
    } else
#endif
    {
        return Bool3x2(false, false, false, false, false, false);
    }
}

template<bool Type>
template<uint32 Index>
XS_INLINE bool Bool3x2<Type>::getBool() const noexcept
{
    static_assert(Index < 6);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        constexpr uint32 index2 = (Index >= 3_ui32) ? Index + 1_ui32 : Index;
        return bitExtract(m_data.m_value, index2);
    } else
#endif
    {
        return m_data.m_values[Index];
    }
}

template<bool Type>
template<uint32 Index>
XS_INLINE constexpr void Bool3x2<Type>::setBoolTrue() noexcept
{
    static_assert(Index < 6);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        constexpr uint32 index2 = (Index >= 3_ui32) ? Index + 1_ui32 : Index;
        m_data.m_value |= (1_ui8 << index2);
    } else
#endif
    {
        m_data.m_values[Index] = true;
    }
}

template<bool Type>
template<uint32 Index>
XS_INLINE constexpr void Bool3x2<Type>::setBoolFalse() noexcept
{
    static_assert(Index < 6);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        constexpr uint32 index2 = (Index >= 3_ui32) ? Index + 1_ui32 : Index;
        m_data.m_value &= ~(1_ui8 << index2);
    } else
#endif
    {
        m_data.m_values[Index] = false;
    }
}

template<bool Type>
XS_INLINE constexpr bool Bool3x2<Type>::getAny() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->m_data.m_value &= 0x77_ui8;
        return (this->m_data.m_value);
    } else
#endif
    {
        return (m_data.m_values[0] | m_data.m_values[1] | m_data.m_values[2] | m_data.m_values[3] | m_data.m_values[4] |
            m_data.m_values[5]);
    }
}

template<bool Type>
XS_INLINE constexpr bool Bool3x2<Type>::getAll() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->m_data.m_value &= 0x77_ui8;
        return (this->m_data.m_value == 0x77_ui8);
    } else
#endif
    {
        return (m_data.m_values[0] & m_data.m_values[1] & m_data.m_values[2] & m_data.m_values[3] & m_data.m_values[4] &
            m_data.m_values[5]);
    }
}

template<bool Type>
XS_INLINE constexpr bool Bool3x2<Type>::getNone() noexcept
{
    return !getAny();
}

template<bool Type>
XS_INLINE bool Bool3x2<Type>::getBool(const uint32 index) const noexcept
{
    XS_ASSERT(index < 6);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        const uint32 index2 = (index >= 3_ui32) ? index + 1_ui32 : index;
        return bitExtract(this->m_data.m_value, index2);
    } else
#endif
    {
        return m_data.m_values[index];
    }
}

template<bool Type>
XS_INLINE void Bool3x2<Type>::setBoolTrue(const uint32 index) noexcept
{
    XS_ASSERT(index < 6);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        const uint32 index2 = (index >= 3_ui32) ? index + 1_ui32 : index;
        this->m_data.m_value = bitSet(this->m_data.m_value, index2);
    } else
#endif
    {
        m_data.m_values[index] = true;
    }
}

template<bool Type>
XS_INLINE void Bool3x2<Type>::setBoolFalse(const uint32 index) noexcept
{
    XS_ASSERT(index < 6);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        const uint32 index2 = (index >= 3_ui32) ? index + 1_ui32 : index;
        this->m_data.m_value = bitClear(this->m_data.m_value, index2);
    } else
#endif
    {
        m_data.m_values[index] = false;
    }
}

template<bool Type>
XS_INLINE uint32 Bool3x2<Type>::indexOfFirstValid() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->m_data.m_value &= 0x77_ui8;
        uint32 index = ctz(this->m_data.m_value);
        index = (index >= 3_ui32) ? index - 1_ui32 : index;
        return index;
    } else
#endif
    {
        return (m_data.m_values[0]) ?
            0_ui32 :
            ((m_data.m_values[1]) ?
                    1_ui32 :
                    ((m_data.m_values[2]) ?
                            2_ui32 :
                            ((m_data.m_values[3]) ? 3_ui32 : ((m_data.m_values[4]) ? 4_ui32 : 5_ui32))));
    }
}

template<bool Type>
XS_INLINE constexpr uint8 Bool3x2<Type>::getAsInteger() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->m_data.m_value &= 0x77_ui8;
        return this->m_data.m_value;
    } else
#endif
    {
        return static_cast<uint8>(m_data.m_values[0]) | (static_cast<uint8>(m_data.m_values[1]) << 1_ui8) |
            (static_cast<uint8>(m_data.m_values[2]) << 2_ui8) | (static_cast<uint8>(m_data.m_values[3]) << 4_ui8) |
            (static_cast<uint8>(m_data.m_values[4]) << 5_ui8) | (static_cast<uint8>(m_data.m_values[5]) << 6_ui8);
    }
}

template<bool Type>
XS_INLINE constexpr Bool4<Type>::Bool4(const uint8 bool4) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value = bool4;
    } else
#endif
    {
        m_data.m_values[0] = bool4 & 0x1_ui8;
        m_data.m_values[1] = bool4 & 0x2_ui8;
        m_data.m_values[2] = bool4 & 0x4_ui8;
        m_data.m_values[3] = bool4 & 0x8_ui8;
    }
}

template<bool Type>
XS_INLINE constexpr Bool4<Type>::Bool4(const bool bool0, const bool bool1, const bool bool2, const bool bool3) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value = static_cast<uint8>(bool0) | (static_cast<uint8>(bool1) << 1_ui8) |
            (static_cast<uint8>(bool2) << 2_ui8) | (static_cast<uint8>(bool3) << 3_ui8);
    } else
#endif
    {
        m_data.m_values[0] = bool0;
        m_data.m_values[1] = bool1;
        m_data.m_values[2] = bool2;
        m_data.m_values[3] = bool3;
    }
}

template<bool Type>
XS_INLINE constexpr Bool4<Type> Bool4<Type>::True() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return Bool4(0xF_ui8);
    } else
#endif
    {
        return Bool4(true, true, true, true);
    }
}

template<bool Type>
XS_INLINE constexpr Bool4<Type> Bool4<Type>::False() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return Bool4(0x0_ui8);
    } else
#endif
    {
        return Bool4(false, false, false, false);
    }
}

template<bool Type>
template<uint32 Index>
XS_INLINE bool Bool4<Type>::getBool() const noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return bitExtract(m_data.m_value, Index);
    } else
#endif
    {
        return m_data.m_values[Index];
    }
}

template<bool Type>
template<uint32 Index>
XS_INLINE constexpr void Bool4<Type>::setBoolTrue() noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value |= (1_ui8 << Index);
    } else
#endif
    {
        m_data.m_values[Index] = true;
    }
}

template<bool Type>
template<uint32 Index>
XS_INLINE constexpr void Bool4<Type>::setBoolFalse() noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value &= ~(1_ui8 << Index);
    } else
#endif
    {
        m_data.m_values[Index] = false;
    }
}

template<bool Type>
XS_INLINE constexpr bool Bool4<Type>::getAny() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return (this->m_data.m_value);
    } else
#endif
    {
        return (m_data.m_values[0] | m_data.m_values[1] | m_data.m_values[2] | m_data.m_values[3]);
    }
}

template<bool Type>
XS_INLINE constexpr bool Bool4<Type>::getAll() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return (this->m_data.m_value == 0xF_ui8);
    } else
#endif
    {
        return (m_data.m_values[0] & m_data.m_values[1] & m_data.m_values[2] & m_data.m_values[3]);
    }
}

template<bool Type>
XS_INLINE constexpr bool Bool4<Type>::getNone() const noexcept
{
    return !getAny();
}

template<bool Type>
XS_INLINE bool Bool4<Type>::getBool(const uint32 index) const noexcept
{
    XS_ASSERT(index < 4);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return bitExtract(this->m_data.m_value, index);
    } else
#endif
    {
        return m_data.m_values[index];
    }
}

template<bool Type>
XS_INLINE void Bool4<Type>::setBoolTrue(const uint32 index) noexcept
{
    XS_ASSERT(index < 4);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        this->m_data.m_value = bitSet(this->m_data.m_value, index);
    } else
#endif
    {
        m_data.m_values[index] = true;
    }
}

template<bool Type>
XS_INLINE void Bool4<Type>::setBoolFalse(const uint32 index) noexcept
{
    XS_ASSERT(index < 4);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        this->m_data.m_value = bitClear(this->m_data.m_value, index);
    } else
#endif
    {
        m_data.m_values[index] = false;
    }
}

template<bool Type>
XS_INLINE uint32 Bool4<Type>::indexOfFirstValid() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return ctz(this->m_data.m_value);
    } else
#endif
    {
        return (m_data.m_values[0]) ? 0_ui32 :
                                      ((m_data.m_values[1]) ? 1_ui32 : ((m_data.m_values[2]) ? 2_ui32 : 3_ui32));
    }
}

template<bool Type>
XS_INLINE constexpr uint8 Bool4<Type>::getAsInteger() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return this->m_data.m_value;
    } else
#endif
    {
        return static_cast<uint8>(m_data.m_values[0]) | (static_cast<uint8>(m_data.m_values[1]) << 1_ui8) |
            (static_cast<uint8>(m_data.m_values[2]) << 2_ui8) | (static_cast<uint8>(m_data.m_values[3]) << 3_ui8);
    }
}

template<bool Type>
XS_INLINE constexpr Bool6<Type>::Bool6(const uint8 bool6) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value = bool6;
    } else
#endif
    {
        m_data.m_values[0] = bool6 & 0x1_ui8;
        m_data.m_values[1] = bool6 & 0x2_ui8;
        m_data.m_values[2] = bool6 & 0x4_ui8;
        m_data.m_values[3] = bool6 & 0x8_ui8;
        m_data.m_values[4] = bool6 & 0x10_ui8;
        m_data.m_values[5] = bool6 & 0x20_ui8;
    }
}

template<bool Type>
XS_INLINE constexpr Bool6<Type>::Bool6(
    const bool bool0, const bool bool1, const bool bool2, const bool bool3, const bool bool4, const bool bool5) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value = static_cast<uint8>(bool0) | (static_cast<uint8>(bool1) << 1_ui8) |
            (static_cast<uint8>(bool2) << 2_ui8) | (static_cast<uint8>(bool3) << 3_ui8) |
            (static_cast<uint8>(bool4) << 4_ui8) | (static_cast<uint8>(bool5) << 5_ui8);
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

template<bool Type>
XS_INLINE constexpr Bool6<Type> Bool6<Type>::True() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return Bool6(0x3F_ui8);
    } else
#endif
    {
        return Bool6(true, true, true, true, true, true);
    }
}

template<bool Type>
XS_INLINE constexpr Bool6<Type> Bool6<Type>::False() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return Bool6(0x0_ui8);
    } else
#endif
    {
        return Bool6(false, false, false, false, false, false);
    }
}

template<bool Type>
template<uint32 Index>
XS_INLINE bool Bool6<Type>::getBool() const noexcept
{
    static_assert(Index < 6);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return bitExtract(m_data.m_value, Index);
    } else
#endif
    {
        return m_data.m_values[Index];
    }
}

template<bool Type>
template<uint32 Index>
XS_INLINE constexpr void Bool6<Type>::setBoolTrue() noexcept
{
    static_assert(Index < 6);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value |= (1_ui8 << Index);
    } else
#endif
    {
        m_data.m_values[Index] = true;
    }
}

template<bool Type>
template<uint32 Index>
XS_INLINE constexpr void Bool6<Type>::setBoolFalse() noexcept
{
    static_assert(Index < 6);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value &= ~(1_ui8 << Index);
    } else
#endif
    {
        m_data.m_values[Index] = false;
    }
}

template<bool Type>
XS_INLINE constexpr bool Bool6<Type>::getAny() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->m_data.m_value &= 0x3F_ui8;
        return (this->m_data.m_value);
    } else
#endif
    {
        return (m_data.m_values[0] | m_data.m_values[1] | m_data.m_values[2] | m_data.m_values[3] | m_data.m_values[4] |
            m_data.m_values[5]);
    }
}

template<bool Type>
XS_INLINE constexpr bool Bool6<Type>::getAll() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->m_data.m_value &= 0x3F_ui8;
        return (this->m_data.m_value == 0x3F_ui8);
    } else
#endif
    {
        return (m_data.m_values[0] & m_data.m_values[1] & m_data.m_values[2] & m_data.m_values[3] & m_data.m_values[4] &
            m_data.m_values[5]);
    }
}

template<bool Type>
XS_INLINE constexpr bool Bool6<Type>::getNone() noexcept
{
    return !getAny();
}

template<bool Type>
XS_INLINE bool Bool6<Type>::getBool(const uint32 index) const noexcept
{
    XS_ASSERT(index < 6);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return bitExtract(this->m_data.m_value, index);
    } else
#endif
    {
        return m_data.m_values[index];
    }
}

template<bool Type>
XS_INLINE void Bool6<Type>::setBoolTrue(const uint32 index) noexcept
{
    XS_ASSERT(index < 6);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        this->m_data.m_value = bitSet(this->m_data.m_value, index);
    } else
#endif
    {
        m_data.m_values[index] = true;
    }
}

template<bool Type>
XS_INLINE void Bool6<Type>::setBoolFalse(const uint32 index) noexcept
{
    XS_ASSERT(index < 6);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        this->m_data.m_value = bitClear(this->m_data.m_value, index);
    } else
#endif
    {
        m_data.m_values[index] = false;
    }
}

template<bool Type>
XS_INLINE uint32 Bool6<Type>::indexOfFirstValid() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->m_data.m_value &= 0x3F_ui8;
        return ctz(this->m_data.m_value);
    } else
#endif
    {
        return (m_data.m_values[0]) ?
            0_ui32 :
            ((m_data.m_values[1]) ?
                    1_ui32 :
                    ((m_data.m_values[2]) ?
                            2_ui32 :
                            ((m_data.m_values[3]) ? 3_ui32 : ((m_data.m_values[4]) ? 4_ui32 : 5_ui32))));
    }
}

template<bool Type>
XS_INLINE constexpr uint8 Bool6<Type>::getAsInteger() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->m_data.m_value &= 0x3F_ui8;
        return this->m_data.m_value;
    } else
#endif
    {
        return static_cast<uint8>(m_data.m_values[0]) | (static_cast<uint8>(m_data.m_values[1]) << 1_ui8) |
            (static_cast<uint8>(m_data.m_values[2]) << 2_ui8) | (static_cast<uint8>(m_data.m_values[3]) << 3_ui8) |
            (static_cast<uint8>(m_data.m_values[4]) << 4_ui8) | (static_cast<uint8>(m_data.m_values[5]) << 5_ui8);
    }
}

template<bool Type>
XS_INLINE constexpr Bool8<Type>::Bool8(const uint8 bool8) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value = bool8;
    } else
#endif
    {
        m_data.m_values[0] = bool8 & 0x1_ui8;
        m_data.m_values[1] = bool8 & 0x2_ui8;
        m_data.m_values[2] = bool8 & 0x4_ui8;
        m_data.m_values[3] = bool8 & 0x8_ui8;
        m_data.m_values[4] = bool8 & 0x10_ui8;
        m_data.m_values[5] = bool8 & 0x20_ui8;
        m_data.m_values[6] = bool8 & 0x40_ui8;
        m_data.m_values[7] = bool8 & 0x80_ui8;
    }
}

template<bool Type>
XS_INLINE constexpr Bool8<Type>::Bool8(const bool bool0, const bool bool1, const bool bool2, const bool bool3,
    const bool bool4, const bool bool5, const bool bool6, const bool bool7) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value = static_cast<uint8>(bool0) | (static_cast<uint8>(bool1) << 1_ui8) |
            (static_cast<uint8>(bool2) << 2_ui8) | (static_cast<uint8>(bool3) << 3_ui8) |
            (static_cast<uint8>(bool4) << 4_ui8) | (static_cast<uint8>(bool5) << 5_ui8) |
            (static_cast<uint8>(bool6) << 6_ui8) | (static_cast<uint8>(bool7) << 7_ui8);
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

template<bool Type>
XS_INLINE constexpr Bool8<Type> Bool8<Type>::True() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return Bool8(0xFF_ui8);
    } else
#endif
    {
        return Bool8(true, true, true, true, true, true, true, true);
    }
}

template<bool Type>
XS_INLINE constexpr Bool8<Type> Bool8<Type>::False() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return Bool8(0x0_ui8);
    } else
#endif
    {
        return Bool8(false, false, false, false, false, false, false, false);
    }
}

template<bool Type>
template<uint32 Index>
XS_INLINE bool Bool8<Type>::getBool() const noexcept
{
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return bitExtract(m_data.m_value, Index);
    } else
#endif
    {
        return m_data.m_values[Index];
    }
}

template<bool Type>
template<uint32 Index>
XS_INLINE constexpr void Bool8<Type>::setBoolTrue() noexcept
{
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value |= (1_ui8 << Index);
    } else
#endif
    {
        m_data.m_values[Index] = true;
    }
}

template<bool Type>
template<uint32 Index>
XS_INLINE constexpr void Bool8<Type>::setBoolFalse() noexcept
{
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        m_data.m_value &= ~(1_ui8 << Index);
    } else
#endif
    {
        m_data.m_values[Index] = false;
    }
}

template<bool Type>
XS_INLINE constexpr bool Bool8<Type>::getAny() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return (this->m_data.m_value);
    } else
#endif
    {
        return (m_data.m_values[0] | m_data.m_values[1] | m_data.m_values[2] | m_data.m_values[3] | m_data.m_values[4] |
            m_data.m_values[5] | m_data.m_values[6] | m_data.m_values[7]);
    }
}

template<bool Type>
XS_INLINE constexpr bool Bool8<Type>::getAll() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return (this->m_data.m_value == 0xFF_ui8);
    } else
#endif
    {
        return (m_data.m_values[0] & m_data.m_values[1] & m_data.m_values[2] & m_data.m_values[3] & m_data.m_values[4] &
            m_data.m_values[5] & m_data.m_values[6] & m_data.m_values[7]);
    }
}

template<bool Type>
XS_INLINE constexpr bool Bool8<Type>::getNone() const noexcept
{
    return !getAny();
}

template<bool Type>
XS_INLINE bool Bool8<Type>::getBool(const uint32 index) const noexcept
{
    XS_ASSERT(index < 8);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return bitExtract(this->m_data.m_value, index);
    } else
#endif
    {
        return m_data.m_values[index];
    }
}

template<bool Type>
XS_INLINE void Bool8<Type>::setBoolTrue(const uint32 index) noexcept
{
    XS_ASSERT(index < 8);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        this->m_data.m_value = bitSet(this->m_data.m_value, index);
    } else
#endif
    {
        m_data.m_values[index] = true;
    }
}

template<bool Type>
XS_INLINE void Bool8<Type>::setBoolFalse(const uint32 index) noexcept
{
    XS_ASSERT(index < 8);
#if XS_ISA == XS_X86
    if constexpr (Type) {
        this->m_data.m_value = bitClear(this->m_data.m_value, index);
    } else
#endif
    {
        m_data.m_values[index] = false;
    }
}

template<bool Type>
XS_INLINE uint32 Bool8<Type>::indexOfFirstValid() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return ctz(this->m_data.m_value);
    } else
#endif
    {
        return (m_data.m_values[0]) ?
            0_ui32 :
            ((m_data.m_values[1]) ?
                    1_ui32 :
                    ((m_data.m_values[2]) ?
                            2_ui32 :
                            ((m_data.m_values[3]) ?
                                    3_ui32 :
                                    ((m_data.m_values[4]) ?
                                            4_ui32 :
                                            ((m_data.m_values[5]) ? 5_ui32 :
                                                                    ((m_data.m_values[6]) ? 6_ui32 : 7_ui32))))));
    }
}

template<bool Type>
XS_INLINE constexpr uint8 Bool8<Type>::getAsInteger() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (Type) {
        return this->m_data.m_value;
    } else
#endif
    {
        return static_cast<uint8>(m_data.m_values[0]) | (static_cast<uint8>(m_data.m_values[1]) << 1_ui8) |
            (static_cast<uint8>(m_data.m_values[2]) << 2_ui8) | (static_cast<uint8>(m_data.m_values[3]) << 3_ui8) |
            (static_cast<uint8>(m_data.m_values[4]) << 4_ui8) | (static_cast<uint8>(m_data.m_values[5]) << 5_ui8) |
            (static_cast<uint8>(m_data.m_values[6]) << 6_ui8) | (static_cast<uint8>(m_data.m_values[7]) << 7_ui8);
    }
}
} // namespace Shift
