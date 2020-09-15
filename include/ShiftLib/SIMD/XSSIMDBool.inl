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
template<bool IsSIMD>
XS_INLINE constexpr Bool2<IsSIMD>::Bool2(const uint8 bool2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = bool2;
    } else
#endif
    {
        this->values0 = bool2 & 0x1_ui8;
        this->values1 = bool2 & 0x2_ui8;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool2<IsSIMD>::Bool2(const bool bool0, const bool bool1) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = static_cast<uint8>(bool0) | (static_cast<uint8>(bool1) << 1_ui8);
    } else
#endif
    {
        this->values0 = bool0;
        this->values1 = bool1;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool2<IsSIMD> Bool2<IsSIMD>::True() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return Bool2(0x3_ui8);
    } else
#endif
    {
        return Bool2(true, true);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool2<IsSIMD> Bool2<IsSIMD>::False() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return Bool2(0x0_ui8);
    } else
#endif
    {
        return Bool2(false, false);
    }
}

template<bool IsSIMD>
template<uint32 Index>
XS_REQUIRES(Index < 2)
XS_INLINE bool Bool2<IsSIMD>::getBool() const noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return bitExtract(this->values, Index);
    } else
#endif
    {
        return (&this->values0)[Index];
    }
}

template<bool IsSIMD>
template<uint32 Index>
XS_REQUIRES(Index < 2)
XS_INLINE constexpr void Bool2<IsSIMD>::setBoolTrue() noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values |= (1_ui8 << Index);
    } else
#endif
    {
        (&this->values0)[Index] = true;
    }
}

template<bool IsSIMD>
template<uint32 Index>
XS_REQUIRES(Index < 2)
XS_INLINE constexpr void Bool2<IsSIMD>::setBoolFalse() noexcept
{
    static_assert(Index < 2);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values &= ~(1_ui8 << Index);
    } else
#endif
    {
        (&this->values0)[Index] = false;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr bool Bool2<IsSIMD>::getAny() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->values &= 0x3_ui8;
        return this->values;
    } else
#endif
    {
        return this->values0 | this->values1;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr bool Bool2<IsSIMD>::getAll() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->values &= 0x3_ui8;
        return this->values == 0x3_ui8;
    } else
#endif
    {
        return this->values0 & this->values1;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr bool Bool2<IsSIMD>::getNone() noexcept
{
    return !getAny();
}

template<bool IsSIMD>
XS_INLINE bool Bool2<IsSIMD>::getBool(const uint32 index) const noexcept
{
    XS_ASSERT(index < 2);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return bitExtract(this->values, index);
    } else
#endif
    {
        return (&this->values0)[index];
    }
}

template<bool IsSIMD>
XS_INLINE void Bool2<IsSIMD>::setBoolTrue(const uint32 index) noexcept
{
    XS_ASSERT(index < 2);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = bitSet(this->values, index);
    } else
#endif
    {
        (&this->values0)[index] = true;
    }
}

template<bool IsSIMD>
XS_INLINE void Bool2<IsSIMD>::setBoolFalse(const uint32 index) noexcept
{
    XS_ASSERT(index < 2);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = bitClear(this->values, index);
    } else
#endif
    {
        (&this->values0)[index] = false;
    }
}

template<bool IsSIMD>
XS_INLINE uint32 Bool2<IsSIMD>::indexOfFirstValid() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->values &= 0x3_ui8;
        return ctz(this->values);
    } else
#endif
    {
        return (this->values0) ? 0_ui32 : 1_ui32;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr uint8 Bool2<IsSIMD>::getAsInteger() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->values &= 0x3_ui8;
        return this->values;
    } else
#endif
    {
        return static_cast<uint8>(this->values0) | (static_cast<uint8>(this->values1) << 1_ui8);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool3<IsSIMD>::Bool3(const uint8 bool3) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = bool3;
    } else
#endif
    {
        this->values0 = bool3 & 0x1_ui8;
        this->values1 = bool3 & 0x2_ui8;
        this->values2 = bool3 & 0x4_ui8;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool3<IsSIMD>::Bool3(const bool bool0, const bool bool1, const bool bool2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values =
            static_cast<uint8>(bool0) | (static_cast<uint8>(bool1) << 1_ui8) | (static_cast<uint8>(bool2) << 2_ui8);
    } else
#endif
    {
        this->values0 = bool0;
        this->values1 = bool1;
        this->values2 = bool2;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool3<IsSIMD> Bool3<IsSIMD>::True() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return Bool3(0x7);
    } else
#endif
    {
        return Bool3(true, true, true);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool3<IsSIMD> Bool3<IsSIMD>::False() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return Bool3(0x0_ui8);
    } else
#endif
    {
        return Bool3(false, false, false);
    }
}

template<bool IsSIMD>
template<uint32 Index>
XS_REQUIRES(Index < 3)
XS_INLINE bool Bool3<IsSIMD>::getBool() const noexcept
{
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return bitExtract(this->values, Index);
    } else
#endif
    {
        return (&this->values0)[Index];
    }
}

template<bool IsSIMD>
template<uint32 Index>
XS_REQUIRES(Index < 3)
XS_INLINE constexpr void Bool3<IsSIMD>::setBoolTrue() noexcept
{
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values |= (1_ui8 << Index);
    } else
#endif
    {
        (&this->values0)[Index] = true;
    }
}

template<bool IsSIMD>
template<uint32 Index>
XS_REQUIRES(Index < 3)
XS_INLINE constexpr void Bool3<IsSIMD>::setBoolFalse() noexcept
{
    static_assert(Index < 3);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values &= ~(1_ui8 << Index);
    } else
#endif
    {
        (&this->values0)[Index] = false;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr bool Bool3<IsSIMD>::getAny() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->values &= 0x7_ui8;
        return this->values;
    } else
#endif
    {
        return (this->values0 | this->values1 | this->values2);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr bool Bool3<IsSIMD>::getAll() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->values &= 0x7_ui8;
        return this->values == 0x7_ui8;
    } else
#endif
    {
        return (this->values0 & this->values1 & this->values2);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr bool Bool3<IsSIMD>::getNone() noexcept
{
    return !getAny();
}

template<bool IsSIMD>
XS_INLINE bool Bool3<IsSIMD>::getBool(const uint32 index) const noexcept
{
    XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return bitExtract(this->values, index);
    } else
#endif
    {
        return (&this->values0)[index];
    }
}

template<bool IsSIMD>
XS_INLINE void Bool3<IsSIMD>::setBoolTrue(const uint32 index) noexcept
{
    XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = bitSet(this->values, index);
    } else
#endif
    {
        (&this->values0)[index] = true;
    }
}

template<bool IsSIMD>
XS_INLINE void Bool3<IsSIMD>::setBoolFalse(const uint32 index) noexcept
{
    XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = bitClear(this->values, index);
    } else
#endif
    {
        (&this->values0)[index] = false;
    }
}

template<bool IsSIMD>
XS_INLINE uint32 Bool3<IsSIMD>::indexOfFirstValid() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->values &= 0x7_ui8;
        return ctz(this->values);
    } else
#endif
    {
        return (this->values0) ? 0_ui32 : ((this->values1) ? 1_ui32 : 2_ui32);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr uint8 Bool3<IsSIMD>::getAsInteger() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->values &= 0x7_ui8;
        return this->values;
    } else
#endif
    {
        return static_cast<uint8>(this->values0) | (static_cast<uint8>(this->values1) << 1_ui8) |
            (static_cast<uint8>(this->values2) << 2_ui8);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool3x2<IsSIMD>::Bool3x2(const uint8 bool3x2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = bool3x2;
    } else
#endif
    {
        this->values0 = bool3x2 & 0x1_ui8;
        this->values1 = bool3x2 & 0x2_ui8;
        this->values2 = bool3x2 & 0x4_ui8;
        this->values3 = bool3x2 & 0x8_ui8;
        this->values4 = bool3x2 & 0x10_ui8;
        this->values5 = bool3x2 & 0x20_ui8;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool3x2<IsSIMD>::Bool3x2(const uint8 bool30, const uint8 bool31) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = bool30 | (bool31 << 4_ui8);
    } else
#endif
    {
        this->values0 = bool30 & 0x1_ui8;
        this->values1 = bool30 & 0x2_ui8;
        this->values2 = bool30 & 0x4_ui8;
        this->values3 = bool31 & 0x1_ui8;
        this->values4 = bool31 & 0x2_ui8;
        this->values5 = bool31 & 0x4_ui8;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool3x2<IsSIMD>::Bool3x2(
    const bool bool0, const bool bool1, const bool bool2, const bool bool3, const bool bool4, const bool bool5) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = static_cast<uint8>(bool0) | (static_cast<uint8>(bool1) << 1_ui8) |
            (static_cast<uint8>(bool2) << 2_ui8) | (static_cast<uint8>(bool3) << 4_ui8) |
            (static_cast<uint8>(bool4) << 5_ui8) | (static_cast<uint8>(bool5) << 6_ui8);
    } else
#endif
    {
        this->values0 = bool0;
        this->values1 = bool1;
        this->values2 = bool2;
        this->values3 = bool3;
        this->values4 = bool4;
        this->values5 = bool5;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool3x2<IsSIMD> Bool3x2<IsSIMD>::True() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return Bool3x2(0x77_ui8);
    } else
#endif
    {
        return Bool3x2(true, true, true, true, true, true);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool3x2<IsSIMD> Bool3x2<IsSIMD>::False() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return Bool3x2(0x0_ui8);
    } else
#endif
    {
        return Bool3x2(false, false, false, false, false, false);
    }
}

template<bool IsSIMD>
template<uint32 Index>
XS_REQUIRES(Index < 6)
XS_INLINE bool Bool3x2<IsSIMD>::getBool() const noexcept
{
    static_assert(Index < 6);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        constexpr uint32 index2 = (Index >= 3_ui32) ? Index + 1_ui32 : Index;
        return bitExtract(this->values, index2);
    } else
#endif
    {
        return (&this->values0)[Index];
    }
}

template<bool IsSIMD>
template<uint32 Index>
XS_REQUIRES(Index < 6)
XS_INLINE constexpr void Bool3x2<IsSIMD>::setBoolTrue() noexcept
{
    static_assert(Index < 6);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        constexpr uint32 index2 = (Index >= 3_ui32) ? Index + 1_ui32 : Index;
        this->values |= (1_ui8 << index2);
    } else
#endif
    {
        (&this->values0)[Index] = true;
    }
}

template<bool IsSIMD>
template<uint32 Index>
XS_REQUIRES(Index < 6)
XS_INLINE constexpr void Bool3x2<IsSIMD>::setBoolFalse() noexcept
{
    static_assert(Index < 6);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        constexpr uint32 index2 = (Index >= 3_ui32) ? Index + 1_ui32 : Index;
        this->values &= ~(1_ui8 << index2);
    } else
#endif
    {
        (&this->values0)[Index] = false;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr bool Bool3x2<IsSIMD>::getAny() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->values &= 0x77_ui8;
        return (this->values);
    } else
#endif
    {
        return (this->values0 | this->values1 | this->values2 | this->values3 | this->values4 | this->values5);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr bool Bool3x2<IsSIMD>::getAll() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->values &= 0x77_ui8;
        return (this->values == 0x77_ui8);
    } else
#endif
    {
        return (this->values0 & this->values1 & this->values2 & this->values3 & this->values4 & this->values5);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr bool Bool3x2<IsSIMD>::getNone() noexcept
{
    return !getAny();
}

template<bool IsSIMD>
XS_INLINE bool Bool3x2<IsSIMD>::getBool(const uint32 index) const noexcept
{
    XS_ASSERT(index < 6);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        const uint32 index2 = (index >= 3_ui32) ? index + 1_ui32 : index;
        return bitExtract(this->values, index2);
    } else
#endif
    {
        return (&this->values0)[index];
    }
}

template<bool IsSIMD>
XS_INLINE void Bool3x2<IsSIMD>::setBoolTrue(const uint32 index) noexcept
{
    XS_ASSERT(index < 6);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        const uint32 index2 = (index >= 3_ui32) ? index + 1_ui32 : index;
        this->values = bitSet(this->values, index2);
    } else
#endif
    {
        (&this->values0)[index] = true;
    }
}

template<bool IsSIMD>
XS_INLINE void Bool3x2<IsSIMD>::setBoolFalse(const uint32 index) noexcept
{
    XS_ASSERT(index < 6);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        const uint32 index2 = (index >= 3_ui32) ? index + 1_ui32 : index;
        this->values = bitClear(this->values, index2);
    } else
#endif
    {
        (&this->values0)[index] = false;
    }
}

template<bool IsSIMD>
XS_INLINE uint32 Bool3x2<IsSIMD>::indexOfFirstValid() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->values &= 0x77_ui8;
        uint32 index = ctz(this->values);
        index = (index >= 3_ui32) ? index - 1_ui32 : index;
        return index;
    } else
#endif
    {
        return (this->values0) ?
            0_ui32 :
            ((this->values1) ?
                    1_ui32 :
                    ((this->values2) ? 2_ui32 : ((this->values3) ? 3_ui32 : ((this->values4) ? 4_ui32 : 5_ui32))));
    }
}

template<bool IsSIMD>
XS_INLINE constexpr uint8 Bool3x2<IsSIMD>::getAsInteger() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->values &= 0x77_ui8;
        return this->values;
    } else
#endif
    {
        return static_cast<uint8>(this->values0) | (static_cast<uint8>(this->values1) << 1_ui8) |
            (static_cast<uint8>(this->values2) << 2_ui8) | (static_cast<uint8>(this->values3) << 4_ui8) |
            (static_cast<uint8>(this->values4) << 5_ui8) | (static_cast<uint8>(this->values5) << 6_ui8);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool4<IsSIMD>::Bool4(const uint8 bool4) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = bool4;
    } else
#endif
    {
        this->values0 = bool4 & 0x1_ui8;
        this->values1 = bool4 & 0x2_ui8;
        this->values2 = bool4 & 0x4_ui8;
        this->values3 = bool4 & 0x8_ui8;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool4<IsSIMD>::Bool4(
    const bool bool0, const bool bool1, const bool bool2, const bool bool3) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = static_cast<uint8>(bool0) | (static_cast<uint8>(bool1) << 1_ui8) |
            (static_cast<uint8>(bool2) << 2_ui8) | (static_cast<uint8>(bool3) << 3_ui8);
    } else
#endif
    {
        this->values0 = bool0;
        this->values1 = bool1;
        this->values2 = bool2;
        this->values3 = bool3;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool4<IsSIMD> Bool4<IsSIMD>::True() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return Bool4(0xF_ui8);
    } else
#endif
    {
        return Bool4(true, true, true, true);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool4<IsSIMD> Bool4<IsSIMD>::False() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return Bool4(0x0_ui8);
    } else
#endif
    {
        return Bool4(false, false, false, false);
    }
}

template<bool IsSIMD>
template<uint32 Index>
XS_REQUIRES(Index < 4)
XS_INLINE bool Bool4<IsSIMD>::getBool() const noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return bitExtract(this->values, Index);
    } else
#endif
    {
        return (&this->values0)[Index];
    }
}

template<bool IsSIMD>
template<uint32 Index>
XS_REQUIRES(Index < 4)
XS_INLINE constexpr void Bool4<IsSIMD>::setBoolTrue() noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values |= (1_ui8 << Index);
    } else
#endif
    {
        (&this->values0)[Index] = true;
    }
}

template<bool IsSIMD>
template<uint32 Index>
XS_REQUIRES(Index < 4)
XS_INLINE constexpr void Bool4<IsSIMD>::setBoolFalse() noexcept
{
    static_assert(Index < 4);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values &= ~(1_ui8 << Index);
    } else
#endif
    {
        (&this->values0)[Index] = false;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr bool Bool4<IsSIMD>::getAny() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return (this->values);
    } else
#endif
    {
        return (this->values0 | this->values1 | this->values2 | this->values3);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr bool Bool4<IsSIMD>::getAll() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return (this->values == 0xF_ui8);
    } else
#endif
    {
        return (this->values0 & this->values1 & this->values2 & this->values3);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr bool Bool4<IsSIMD>::getNone() const noexcept
{
    return !getAny();
}

template<bool IsSIMD>
XS_INLINE bool Bool4<IsSIMD>::getBool(const uint32 index) const noexcept
{
    XS_ASSERT(index < 4);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return bitExtract(this->values, index);
    } else
#endif
    {
        return (&this->values0)[index];
    }
}

template<bool IsSIMD>
XS_INLINE void Bool4<IsSIMD>::setBoolTrue(const uint32 index) noexcept
{
    XS_ASSERT(index < 4);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = bitSet(this->values, index);
    } else
#endif
    {
        (&this->values0)[index] = true;
    }
}

template<bool IsSIMD>
XS_INLINE void Bool4<IsSIMD>::setBoolFalse(const uint32 index) noexcept
{
    XS_ASSERT(index < 4);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = bitClear(this->values, index);
    } else
#endif
    {
        (&this->values0)[index] = false;
    }
}

template<bool IsSIMD>
XS_INLINE uint32 Bool4<IsSIMD>::indexOfFirstValid() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return ctz(this->values);
    } else
#endif
    {
        return (this->values0) ? 0_ui32 : ((this->values1) ? 1_ui32 : ((this->values2) ? 2_ui32 : 3_ui32));
    }
}

template<bool IsSIMD>
XS_INLINE constexpr uint8 Bool4<IsSIMD>::getAsInteger() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return this->values;
    } else
#endif
    {
        return static_cast<uint8>(this->values0) | (static_cast<uint8>(this->values1) << 1_ui8) |
            (static_cast<uint8>(this->values2) << 2_ui8) | (static_cast<uint8>(this->values3) << 3_ui8);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool6<IsSIMD>::Bool6(const uint8 bool6) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = bool6;
    } else
#endif
    {
        this->values0 = bool6 & 0x1_ui8;
        this->values1 = bool6 & 0x2_ui8;
        this->values2 = bool6 & 0x4_ui8;
        this->values3 = bool6 & 0x8_ui8;
        this->values4 = bool6 & 0x10_ui8;
        this->values5 = bool6 & 0x20_ui8;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool6<IsSIMD>::Bool6(
    const bool bool0, const bool bool1, const bool bool2, const bool bool3, const bool bool4, const bool bool5) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = static_cast<uint8>(bool0) | (static_cast<uint8>(bool1) << 1_ui8) |
            (static_cast<uint8>(bool2) << 2_ui8) | (static_cast<uint8>(bool3) << 3_ui8) |
            (static_cast<uint8>(bool4) << 4_ui8) | (static_cast<uint8>(bool5) << 5_ui8);
    } else
#endif
    {
        this->values0 = bool0;
        this->values1 = bool1;
        this->values2 = bool2;
        this->values3 = bool3;
        this->values4 = bool4;
        this->values5 = bool5;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool6<IsSIMD> Bool6<IsSIMD>::True() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return Bool6(0x3F_ui8);
    } else
#endif
    {
        return Bool6(true, true, true, true, true, true);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool6<IsSIMD> Bool6<IsSIMD>::False() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return Bool6(0x0_ui8);
    } else
#endif
    {
        return Bool6(false, false, false, false, false, false);
    }
}

template<bool IsSIMD>
template<uint32 Index>
XS_REQUIRES(Index < 6)
XS_INLINE bool Bool6<IsSIMD>::getBool() const noexcept
{
    static_assert(Index < 6);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return bitExtract(this->values, Index);
    } else
#endif
    {
        return (&this->values0)[Index];
    }
}

template<bool IsSIMD>
template<uint32 Index>
XS_REQUIRES(Index < 6)
XS_INLINE constexpr void Bool6<IsSIMD>::setBoolTrue() noexcept
{
    static_assert(Index < 6);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values |= (1_ui8 << Index);
    } else
#endif
    {
        (&this->values0)[Index] = true;
    }
}

template<bool IsSIMD>
template<uint32 Index>
XS_REQUIRES(Index < 6)
XS_INLINE constexpr void Bool6<IsSIMD>::setBoolFalse() noexcept
{
    static_assert(Index < 6);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values &= ~(1_ui8 << Index);
    } else
#endif
    {
        (&this->values0)[Index] = false;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr bool Bool6<IsSIMD>::getAny() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->values &= 0x3F_ui8;
        return (this->values);
    } else
#endif
    {
        return (this->values0 | this->values1 | this->values2 | this->values3 | this->values4 | this->values5);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr bool Bool6<IsSIMD>::getAll() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->values &= 0x3F_ui8;
        return (this->values == 0x3F_ui8);
    } else
#endif
    {
        return (this->values0 & this->values1 & this->values2 & this->values3 & this->values4 & this->values5);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr bool Bool6<IsSIMD>::getNone() noexcept
{
    return !getAny();
}

template<bool IsSIMD>
XS_INLINE bool Bool6<IsSIMD>::getBool(const uint32 index) const noexcept
{
    XS_ASSERT(index < 6);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return bitExtract(this->values, index);
    } else
#endif
    {
        return (&this->values0)[index];
    }
}

template<bool IsSIMD>
XS_INLINE void Bool6<IsSIMD>::setBoolTrue(const uint32 index) noexcept
{
    XS_ASSERT(index < 6);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = bitSet(this->values, index);
    } else
#endif
    {
        (&this->values0)[index] = true;
    }
}

template<bool IsSIMD>
XS_INLINE void Bool6<IsSIMD>::setBoolFalse(const uint32 index) noexcept
{
    XS_ASSERT(index < 6);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = bitClear(this->values, index);
    } else
#endif
    {
        (&this->values0)[index] = false;
    }
}

template<bool IsSIMD>
XS_INLINE uint32 Bool6<IsSIMD>::indexOfFirstValid() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->values &= 0x3F_ui8;
        return ctz(this->values);
    } else
#endif
    {
        return (this->values0) ?
            0_ui32 :
            ((this->values1) ?
                    1_ui32 :
                    ((this->values2) ? 2_ui32 : ((this->values3) ? 3_ui32 : ((this->values4) ? 4_ui32 : 5_ui32))));
    }
}

template<bool IsSIMD>
XS_INLINE constexpr uint8 Bool6<IsSIMD>::getAsInteger() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        // Need to avoid top bits affecting result. Done here in hopes that function
        // inlining will prevent duplication (Note loss of const) noexcept
        this->values &= 0x3F_ui8;
        return this->values;
    } else
#endif
    {
        return static_cast<uint8>(this->values0) | (static_cast<uint8>(this->values1) << 1_ui8) |
            (static_cast<uint8>(this->values2) << 2_ui8) | (static_cast<uint8>(this->values3) << 3_ui8) |
            (static_cast<uint8>(this->values4) << 4_ui8) | (static_cast<uint8>(this->values5) << 5_ui8);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool8<IsSIMD>::Bool8(const uint8 bool8) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = bool8;
    } else
#endif
    {
        this->values0 = bool8 & 0x1_ui8;
        this->values1 = bool8 & 0x2_ui8;
        this->values2 = bool8 & 0x4_ui8;
        this->values3 = bool8 & 0x8_ui8;
        this->values4 = bool8 & 0x10_ui8;
        this->values5 = bool8 & 0x20_ui8;
        this->values6 = bool8 & 0x40_ui8;
        this->values7 = bool8 & 0x80_ui8;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool8<IsSIMD>::Bool8(const bool bool0, const bool bool1, const bool bool2, const bool bool3,
    const bool bool4, const bool bool5, const bool bool6, const bool bool7) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = static_cast<uint8>(bool0) | (static_cast<uint8>(bool1) << 1_ui8) |
            (static_cast<uint8>(bool2) << 2_ui8) | (static_cast<uint8>(bool3) << 3_ui8) |
            (static_cast<uint8>(bool4) << 4_ui8) | (static_cast<uint8>(bool5) << 5_ui8) |
            (static_cast<uint8>(bool6) << 6_ui8) | (static_cast<uint8>(bool7) << 7_ui8);
    } else
#endif
    {
        this->values0 = bool0;
        this->values1 = bool1;
        this->values2 = bool2;
        this->values3 = bool3;
        this->values4 = bool4;
        this->values5 = bool5;
        this->values6 = bool6;
        this->values7 = bool7;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool8<IsSIMD> Bool8<IsSIMD>::True() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return Bool8(0xFF_ui8);
    } else
#endif
    {
        return Bool8(true, true, true, true, true, true, true, true);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr Bool8<IsSIMD> Bool8<IsSIMD>::False() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return Bool8(0x0_ui8);
    } else
#endif
    {
        return Bool8(false, false, false, false, false, false, false, false);
    }
}

template<bool IsSIMD>
template<uint32 Index>
XS_REQUIRES(Index < 8)
XS_INLINE bool Bool8<IsSIMD>::getBool() const noexcept
{
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return bitExtract(this->values, Index);
    } else
#endif
    {
        return (&this->values0)[Index];
    }
}

template<bool IsSIMD>
template<uint32 Index>
XS_REQUIRES(Index < 8)
XS_INLINE constexpr void Bool8<IsSIMD>::setBoolTrue() noexcept
{
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values |= (1_ui8 << Index);
    } else
#endif
    {
        (&this->values0)[Index] = true;
    }
}

template<bool IsSIMD>
template<uint32 Index>
XS_REQUIRES(Index < 8)
XS_INLINE constexpr void Bool8<IsSIMD>::setBoolFalse() noexcept
{
    static_assert(Index < 8);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values &= ~(1_ui8 << Index);
    } else
#endif
    {
        (&this->values0)[Index] = false;
    }
}

template<bool IsSIMD>
XS_INLINE constexpr bool Bool8<IsSIMD>::getAny() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return (this->values);
    } else
#endif
    {
        return (this->values0 | this->values1 | this->values2 | this->values3 | this->values4 | this->values5 |
            this->values6 | this->values7);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr bool Bool8<IsSIMD>::getAll() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return (this->values == 0xFF_ui8);
    } else
#endif
    {
        return (this->values0 & this->values1 & this->values2 & this->values3 & this->values4 & this->values5 &
            this->values6 & this->values7);
    }
}

template<bool IsSIMD>
XS_INLINE constexpr bool Bool8<IsSIMD>::getNone() const noexcept
{
    return !getAny();
}

template<bool IsSIMD>
XS_INLINE bool Bool8<IsSIMD>::getBool(const uint32 index) const noexcept
{
    XS_ASSERT(index < 8);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return bitExtract(this->values, index);
    } else
#endif
    {
        return (&this->values0)[index];
    }
}

template<bool IsSIMD>
XS_INLINE void Bool8<IsSIMD>::setBoolTrue(const uint32 index) noexcept
{
    XS_ASSERT(index < 8);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = bitSet(this->values, index);
    } else
#endif
    {
        (&this->values0)[index] = true;
    }
}

template<bool IsSIMD>
XS_INLINE void Bool8<IsSIMD>::setBoolFalse(const uint32 index) noexcept
{
    XS_ASSERT(index < 8);
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        this->values = bitClear(this->values, index);
    } else
#endif
    {
        (&this->values0)[index] = false;
    }
}

template<bool IsSIMD>
XS_INLINE uint32 Bool8<IsSIMD>::indexOfFirstValid() const noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return ctz(this->values);
    } else
#endif
    {
        return (this->values0) ?
            0_ui32 :
            ((this->values1) ?
                    1_ui32 :
                    ((this->values2) ?
                            2_ui32 :
                            ((this->values3) ?
                                    3_ui32 :
                                    ((this->values4) ?
                                            4_ui32 :
                                            ((this->values5) ? 5_ui32 : ((this->values6) ? 6_ui32 : 7_ui32))))));
    }
}

template<bool IsSIMD>
XS_INLINE constexpr uint8 Bool8<IsSIMD>::getAsInteger() noexcept
{
#if XS_ISA == XS_X86
    if constexpr (IsSIMD) {
        return this->values;
    } else
#endif
    {
        return static_cast<uint8>(this->values0) | (static_cast<uint8>(this->values1) << 1_ui8) |
            (static_cast<uint8>(this->values2) << 2_ui8) | (static_cast<uint8>(this->values3) << 3_ui8) |
            (static_cast<uint8>(this->values4) << 4_ui8) | (static_cast<uint8>(this->values5) << 5_ui8) |
            (static_cast<uint8>(this->values6) << 6_ui8) | (static_cast<uint8>(this->values7) << 7_ui8);
    }
}
} // namespace Shift
