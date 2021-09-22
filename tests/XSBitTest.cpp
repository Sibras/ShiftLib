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

#include "XSBit.hpp"

#include "XSCompilerOptions.h"
#include "XSGTest.hpp"

using namespace Shift;

#ifdef XSTESTMAIN
TEST_NS(Bit, Bit, Bswap)
{
    // Has separate implementations for 32b and 64b
    ASSERT_EQ(bswap<int16>(0x965A_i16), 0x5A96_i16);
    ASSERT_EQ(bswap<uint16>(0x965A_ui16), 0x5A96_ui16);

    ASSERT_EQ(bswap<int32>(0x965AF04B_i32), 0x4BF05A96_i32);
    ASSERT_EQ(bswap<uint32>(0x965AF04B_ui32), 0x4BF05A96_ui32);

    ASSERT_EQ(bswap<int64>(0x965AF04B12CDE387_i64), 0x87E3CD124BF05A96_i64);
    ASSERT_EQ(bswap<uint64>(0x965AF04B12CDE387_ui64), 0x87E3CD124BF05A96_ui64);
}

TEST_NS(Bit, Bit, Bsr)
{
    // Has separate implementations for 32b and 64b
    ASSERT_EQ(bsr<int32>(0x00000004_i32), 2_ui32);
    ASSERT_EQ(bsr<int32>(0x0FFF0F00_i32), 27_ui32);

    ASSERT_EQ(bsr<uint32>(0x00000004_ui32), 2_ui32);
    ASSERT_EQ(bsr<uint32>(0x0FFF0F00_ui32), 27_ui32);

    ASSERT_EQ(bsr<int64>(0x00000004_i64), 2_ui32);
    ASSERT_EQ(bsr<int64>(0x0FFF0F00_i64), 27_ui32);
    ASSERT_EQ(bsr<int64>(0x0FF000FF0FFF0F00_i64), 59_ui32);
    ASSERT_EQ(bsr<int64>(0x0FF0000000000000_i64), 59_ui32);

    ASSERT_EQ(bsr<uint64>(0x00000004_ui64), 2_ui32);
    ASSERT_EQ(bsr<uint64>(0x0FFF0F00_ui64), 27_ui32);
    ASSERT_EQ(bsr<uint64>(0x0FF000FF0FFF0F00_ui64), 59_ui32);
    ASSERT_EQ(bsr<uint64>(0x0FF0000000000000_i64), 59_ui32);
}
#endif

#if !defined(XSTESTMAIN) || (XS_ISA != XS_X86)
TEST_NS(Bit, Bit, TESTISA(Popcnt))
{
    // Has separate implementations for 32b, 64b and SSE4.2+
    ASSERT_EQ(popcnt<int8>(0x04_i8), 1_ui8);
    ASSERT_EQ(popcnt<int8>(0xF0_i8), 4_ui8);

    ASSERT_EQ(popcnt<uint8>(0x04_ui8), 1_ui8);
    ASSERT_EQ(popcnt<uint8>(0xF0_ui8), 4_ui8);

    ASSERT_EQ(popcnt<int16>(0x0004_i16), 1_ui16);
    ASSERT_EQ(popcnt<int16>(0x0F00_i16), 4_ui16);

    ASSERT_EQ(popcnt<uint16>(0x0004_ui16), 1_ui16);
    ASSERT_EQ(popcnt<uint16>(0x0F00_ui16), 4_ui16);

    ASSERT_EQ(popcnt<int32>(0x00000004_i32), 1_ui32);
    ASSERT_EQ(popcnt<int32>(0x0FFF0F00_i32), 16_ui32);

    ASSERT_EQ(popcnt<uint32>(0x00000004_ui32), 1_ui32);
    ASSERT_EQ(popcnt<uint32>(0x0FFF0F00_ui32), 16_ui32);

    ASSERT_EQ(popcnt<int64>(0x00000004_i64), 1_ui32);
    ASSERT_EQ(popcnt<int64>(0x0FFF0F00_i64), 16_ui32);
    ASSERT_EQ(popcnt<int64>(0x0FF000FF0FFF0F00_i64), 32_ui32);
    ASSERT_EQ(popcnt<int64>(0x0FF0000000000000_i64), 8_ui32);

    ASSERT_EQ(popcnt<uint64>(0x00000004_ui64), 1_ui32);
    ASSERT_EQ(popcnt<uint64>(0x0FFF0F04_ui64), 17_ui32);
    ASSERT_EQ(popcnt<uint64>(0x0FF000FF0FFF0F00_ui64), 32_ui32);
    ASSERT_EQ(popcnt<uint64>(0x0FF0000000000000_ui64), 8_ui32);
}

TEST_NS(Bit, Bit, TESTISA(Ctz))
{
    // Has separate implementations for 32b, 64b and AVX2
    ASSERT_EQ(ctz<int8>(0x04_i8), 2_ui8);
    ASSERT_EQ(ctz<int8>(0xF0_i8), 4_ui8);

    ASSERT_EQ(ctz<uint8>(0x04_ui8), 2_ui8);
    ASSERT_EQ(ctz<uint8>(0xF0_ui8), 4_ui8);

    ASSERT_EQ(ctz<int16>(0x0004_i16), 2_ui16);
    ASSERT_EQ(ctz<int16>(0x0F00_i16), 8_ui16);

    ASSERT_EQ(ctz<uint16>(0x0004_ui16), 2_ui16);
    ASSERT_EQ(ctz<uint16>(0x0F00_ui16), 8_ui16);

    ASSERT_EQ(ctz<int32>(0x00000004_i32), 2_ui32);
    ASSERT_EQ(ctz<int32>(0x0FFF0F00_i32), 8_ui32);

    ASSERT_EQ(ctz<uint32>(0x00000004_ui32), 2_ui32);
    ASSERT_EQ(ctz<uint32>(0x0FFF0F00_ui32), 8_ui32);

    ASSERT_EQ(ctz<int64>(0x00000004_i64), 2_ui32);
    ASSERT_EQ(ctz<int64>(0x0FFF0F00_i64), 8_ui32);
    ASSERT_EQ(ctz<int64>(0x0FF000FF0FFF0F00_i64), 8_ui32);
    ASSERT_EQ(ctz<int64>(0x0FF0000000000000_i64), 52_ui32);

    ASSERT_EQ(ctz<uint64>(0x00000004_ui64), 2_ui32);
    ASSERT_EQ(ctz<uint64>(0x0FFF0F00_ui64), 8_ui32);
    ASSERT_EQ(ctz<uint64>(0x0FF000FF0FFF0F00_ui64), 8_ui32);
    ASSERT_EQ(ctz<uint64>(0x0FF0000000000000_ui64), 52_ui32);
}

TEST_NS(Bit, Bit, TESTISA(Clz))
{
    // Has separate implementations for 32b, 64b and AVX2
    ASSERT_EQ(clz<int32>(0x00000004_i32), 29_ui32);
    ASSERT_EQ(clz<int32>(0x0FFF0F00_i32), 4_ui32);

    ASSERT_EQ(clz<uint32>(0x00000004_ui32), 29_ui32);
    ASSERT_EQ(clz<uint32>(0x0FFF0F00_ui32), 4_ui32);

    ASSERT_EQ(clz<int64>(0x00000004_i64), 61_ui32);
    ASSERT_EQ(clz<int64>(0x0FFF0F00_i64), 36_ui32);
    ASSERT_EQ(clz<int64>(0x0FF000FF0FFF0F00_i64), 4_ui32);
    ASSERT_EQ(clz<int64>(0x0FF0000000000000_i64), 4_ui32);

    ASSERT_EQ(clz<uint64>(0x00000004_ui64), 61_ui32);
    ASSERT_EQ(clz<uint64>(0x0FFF0F04_ui64), 36_ui32);
    ASSERT_EQ(clz<uint64>(0x0FF000FF0FFF0F00_ui64), 4_ui32);
    ASSERT_EQ(clz<uint64>(0x0FF0000000000000_ui64), 4_ui32);
}
#endif

#ifdef XSTESTMAIN
TEST_NS(Bit, Bit, BitExtract)
{
    // Has separate implementations for 32b and 64b
    ASSERT_EQ(bitExtract<int8>(0x04_i8, 0_ui32), 0_ui8);
    ASSERT_EQ(bitExtract<int8>(0x04_i8, 2_ui32), 1_ui8);
    ASSERT_EQ(bitExtract<int8>(0xF0_i8, 3_ui32), 0_ui8);
    ASSERT_EQ(bitExtract<int8>(0xF0_i8, 7_ui32), 1_ui8);

    ASSERT_EQ(bitExtract<uint8>(0x04_ui8, 0_ui32), 0_ui8);
    ASSERT_EQ(bitExtract<uint8>(0x04_ui8, 2_ui32), 1_ui8);
    ASSERT_EQ(bitExtract<uint8>(0xF0_ui8, 3_ui32), 0_ui8);
    ASSERT_EQ(bitExtract<uint8>(0xF0_ui8, 7_ui32), 1_ui8);

    ASSERT_EQ(bitExtract<int16>(0x0004_i16, 0_ui32), 0_ui8);
    ASSERT_EQ(bitExtract<int16>(0x0004_i16, 2_ui32), 1_ui8);
    ASSERT_EQ(bitExtract<int16>(0x0F00_i16, 14_ui32), 0_ui8);
    ASSERT_EQ(bitExtract<int16>(0x0F00_i16, 10_ui32), 1_ui8);

    ASSERT_EQ(bitExtract<uint16>(0x0004_ui16, 0_ui32), 0_ui8);
    ASSERT_EQ(bitExtract<uint16>(0x0004_ui16, 2_ui32), 1_ui8);
    ASSERT_EQ(bitExtract<uint16>(0x0F00_ui16, 14_ui32), 0_ui8);
    ASSERT_EQ(bitExtract<uint16>(0x0F00_ui16, 10_ui32), 1_ui8);

    ASSERT_EQ(bitExtract<int32>(0x00000004_i32, 0_ui32), 0_ui8);
    ASSERT_EQ(bitExtract<int32>(0x00000004_i32, 2_ui32), 1_ui8);
    ASSERT_EQ(bitExtract<int32>(0x0FFF0F00_i32, 31_ui32), 0_ui8);
    ASSERT_EQ(bitExtract<int32>(0x0FFF0F00_i32, 27_ui32), 1_ui8);

    ASSERT_EQ(bitExtract<uint32>(0x00000004_ui32, 0_ui32), 0_ui8);
    ASSERT_EQ(bitExtract<uint32>(0x00000004_ui32, 2_ui32), 1_ui8);
    ASSERT_EQ(bitExtract<uint32>(0x0FFF0F00_ui32, 31_ui32), 0_ui8);
    ASSERT_EQ(bitExtract<uint32>(0x0FFF0F00_ui32, 27_ui32), 1_ui8);

    ASSERT_EQ(bitExtract<int64>(0x00000004_i64, 0_ui32), 0_ui8);
    ASSERT_EQ(bitExtract<int64>(0x00000004_i64, 2_ui32), 1_ui8);
    ASSERT_EQ(bitExtract<int64>(0x0FFF0F00_i64, 31_ui32), 0_ui8);
    ASSERT_EQ(bitExtract<int64>(0x0FFF0F00_i64, 27_ui32), 1_ui8);
    ASSERT_EQ(bitExtract<int64>(0x0FF000FF0FFF0F00_i64, 62_ui32), 0_ui32);
    ASSERT_EQ(bitExtract<int64>(0x0FF000FF0FFF0F00_i64, 58_ui32), 1_ui32);
    ASSERT_EQ(bitExtract<int64>(0x0FF0000000000000_i64, 62_ui32), 0_ui32);
    ASSERT_EQ(bitExtract<int64>(0x0FF0000000000000_i64, 58_ui32), 1_ui32);

    ASSERT_EQ(bitExtract<uint64>(0x00000004_ui64, 0_ui32), 0_ui8);
    ASSERT_EQ(bitExtract<uint64>(0x00000004_ui64, 2_ui32), 1_ui8);
    ASSERT_EQ(bitExtract<uint64>(0x0FFF0F00_ui64, 31_ui32), 0_ui8);
    ASSERT_EQ(bitExtract<uint64>(0x0FFF0F00_ui64, 27_ui32), 1_ui8);
    ASSERT_EQ(bitExtract<uint64>(0x0FF000FF0FFF0F00_ui64, 62_ui32), 0_ui32);
    ASSERT_EQ(bitExtract<uint64>(0x0FF000FF0FFF0F00_ui64, 58_ui32), 1_ui32);
    ASSERT_EQ(bitExtract<uint64>(0x0FF0000000000000_ui64, 62_ui32), 0_ui32);
    ASSERT_EQ(bitExtract<uint64>(0x0FF0000000000000_ui64, 58_ui32), 1_ui32);
}

TEST_NS(Bit, Bit, BitSet)
{
    // Has separate implementations for 32b and 64b
    ASSERT_EQ(bitSet<int8>(0x04_i8, 0_ui32), 0x05_i8);
    ASSERT_EQ(bitSet<int8>(0x04_i8, 2_ui32), 0x04_i8);
    ASSERT_EQ(bitSet<int8>(0xF0_i8, 3_ui32), 0xF8_i8);
    ASSERT_EQ(bitSet<int8>(0xF0_i8, 7_ui32), 0xF0_i8);

    ASSERT_EQ(bitSet<uint8>(0x04_ui8, 0_ui32), 0x05_ui8);
    ASSERT_EQ(bitSet<uint8>(0x04_ui8, 2_ui32), 0x04_ui8);
    ASSERT_EQ(bitSet<uint8>(0xF0_ui8, 3_ui32), 0xF8_ui8);
    ASSERT_EQ(bitSet<uint8>(0xF0_ui8, 7_ui32), 0xF0_ui8);

    ASSERT_EQ(bitSet<int16>(0x0004_i16, 0_ui32), 0x0005_i16);
    ASSERT_EQ(bitSet<int16>(0x0004_i16, 2_ui32), 0x0004_i16);
    ASSERT_EQ(bitSet<int16>(0x0F00_i16, 14_ui32), 0x4F00_i16);
    ASSERT_EQ(bitSet<int16>(0x0F00_i16, 10_ui32), 0x0F00_i16);

    ASSERT_EQ(bitSet<uint16>(0x0004_ui16, 0_ui32), 0x0005_ui16);
    ASSERT_EQ(bitSet<uint16>(0x0004_ui16, 2_ui32), 0x0004_ui16);
    ASSERT_EQ(bitSet<uint16>(0x0F00_ui16, 14_ui32), 0x4F00_ui16);
    ASSERT_EQ(bitSet<uint16>(0x0F00_ui16, 10_ui32), 0x0F00_ui16);

    ASSERT_EQ(bitSet<int32>(0x00000004_i32, 0_ui32), 0x00000005_i32);
    ASSERT_EQ(bitSet<int32>(0x00000004_i32, 2_ui32), 0x00000004_i32);
    ASSERT_EQ(bitSet<int32>(0x0FFF0F00_i32, 31_ui32), 0x8FFF0F00_i32);
    ASSERT_EQ(bitSet<int32>(0x0FFF0F00_i32, 27_ui32), 0x0FFF0F00_i32);

    ASSERT_EQ(bitSet<uint32>(0x00000004_ui32, 0_ui32), 0x00000005_ui32);
    ASSERT_EQ(bitSet<uint32>(0x00000004_ui32, 2_ui32), 0x00000004_ui32);
    ASSERT_EQ(bitSet<uint32>(0x0FFF0F00_ui32, 31_ui32), 0x8FFF0F00_ui32);
    ASSERT_EQ(bitSet<uint32>(0x0FFF0F00_ui32, 27_ui32), 0x0FFF0F00_ui32);

    ASSERT_EQ(bitSet<int64>(0x00000004_i64, 0_ui32), 0x00000005_i64);
    ASSERT_EQ(bitSet<int64>(0x00000004_i64, 2_ui32), 0x00000004_i64);
    ASSERT_EQ(bitSet<int64>(0x0FFF0F00_i64, 31_ui32), 0x8FFF0F00_i64);
    ASSERT_EQ(bitSet<int64>(0x0FFF0F00_i64, 27_ui32), 0x0FFF0F00_i64);
    ASSERT_EQ(bitSet<int64>(0x0FF000FF0FFF0F00_i64, 62_ui32), 0x4FF000FF0FFF0F00_i64);
    ASSERT_EQ(bitSet<int64>(0x0FF000FF0FFF0F00_i64, 58_ui32), 0x0FF000FF0FFF0F00_i64);
    ASSERT_EQ(bitSet<int64>(0x0FF0000000000000_i64, 62_ui32), 0x4FF0000000000000_i64);
    ASSERT_EQ(bitSet<int64>(0x0FF0000000000000_i64, 58_ui32), 0x0FF0000000000000_i64);

    ASSERT_EQ(bitSet<uint64>(0x00000004_ui64, 0_ui32), 0x00000005_ui64);
    ASSERT_EQ(bitSet<uint64>(0x00000004_ui64, 2_ui32), 0x00000004_ui64);
    ASSERT_EQ(bitSet<uint64>(0x0FFF0F00_ui64, 31_ui32), 0x8FFF0F00_ui64);
    ASSERT_EQ(bitSet<uint64>(0x0FFF0F00_ui64, 27_ui32), 0x0FFF0F00_ui64);
    ASSERT_EQ(bitSet<uint64>(0x0FF000FF0FFF0F00_ui64, 62_ui32), 0x4FF000FF0FFF0F00_ui64);
    ASSERT_EQ(bitSet<uint64>(0x0FF000FF0FFF0F00_ui64, 58_ui32), 0x0FF000FF0FFF0F00_ui64);
    ASSERT_EQ(bitSet<uint64>(0x0FF0000000000000_ui64, 62_ui32), 0x4FF0000000000000_ui64);
    ASSERT_EQ(bitSet<uint64>(0x0FF0000000000000_ui64, 58_ui32), 0x0FF0000000000000_ui64);
}

TEST_NS(Bit, Bit, BitClear)
{
    // Has separate implementations for 32b and 64b
    ASSERT_EQ(bitClear<int8>(0x04_i8, 0_ui32), 0x04_i8);
    ASSERT_EQ(bitClear<int8>(0x04_i8, 2_ui32), 0x00_i8);
    ASSERT_EQ(bitClear<int8>(0xF0_i8, 3_ui32), 0xF0_i8);
    ASSERT_EQ(bitClear<int8>(0xF0_i8, 7_ui32), 0x70_i8);

    ASSERT_EQ(bitClear<uint8>(0x04_ui8, 0_ui32), 0x04_ui8);
    ASSERT_EQ(bitClear<uint8>(0x04_ui8, 2_ui32), 0x00_ui8);
    ASSERT_EQ(bitClear<uint8>(0xF0_ui8, 3_ui32), 0xF0_ui8);
    ASSERT_EQ(bitClear<uint8>(0xF0_ui8, 7_ui32), 0x70_ui8);

    ASSERT_EQ(bitClear<int16>(0x0004_i16, 0_ui32), 0x0004_i16);
    ASSERT_EQ(bitClear<int16>(0x0004_i16, 2_ui32), 0x0000_i16);
    ASSERT_EQ(bitClear<int16>(0x0F00_i16, 14_ui32), 0x0F00_i16);
    ASSERT_EQ(bitClear<int16>(0x0F00_i16, 10_ui32), 0x0B00_i16);

    ASSERT_EQ(bitClear<uint16>(0x0004_ui16, 0_ui32), 0x0004_ui16);
    ASSERT_EQ(bitClear<uint16>(0x0004_ui16, 2_ui32), 0x0000_ui16);
    ASSERT_EQ(bitClear<uint16>(0x0F00_ui16, 14_ui32), 0x0F00_ui16);
    ASSERT_EQ(bitClear<uint16>(0x0F00_ui16, 10_ui32), 0x0B00_ui16);

    ASSERT_EQ(bitClear<int32>(0x00000004_i32, 0_ui32), 0x00000004_i32);
    ASSERT_EQ(bitClear<int32>(0x00000004_i32, 2_ui32), 0x00000000_i32);
    ASSERT_EQ(bitClear<int32>(0x0FFF0F00_i32, 31_ui32), 0x0FFF0F00_i32);
    ASSERT_EQ(bitClear<int32>(0x0FFF0F00_i32, 27_ui32), 0x07FF0F00_i32);

    ASSERT_EQ(bitClear<uint32>(0x00000004_ui32, 0_ui32), 0x00000004_ui32);
    ASSERT_EQ(bitClear<uint32>(0x00000004_ui32, 2_ui32), 0x00000000_ui32);
    ASSERT_EQ(bitClear<uint32>(0x0FFF0F00_ui32, 31_ui32), 0x0FFF0F00_ui32);
    ASSERT_EQ(bitClear<uint32>(0x0FFF0F00_ui32, 27_ui32), 0x07FF0F00_ui32);

    ASSERT_EQ(bitClear<int64>(0x00000004_i64, 0_ui32), 0x00000004_i64);
    ASSERT_EQ(bitClear<int64>(0x00000004_i64, 2_ui32), 0x00000000_i64);
    ASSERT_EQ(bitClear<int64>(0x0FFF0F00_i64, 31_ui32), 0x0FFF0F00_i64);
    ASSERT_EQ(bitClear<int64>(0x0FFF0F00_i64, 27_ui32), 0x07FF0F00_i64);
    ASSERT_EQ(bitClear<int64>(0x0FF000FF0FFF0F00_i64, 62_ui32), 0x0FF000FF0FFF0F00_i64);
    ASSERT_EQ(bitClear<int64>(0x0FF000FF0FFF0F00_i64, 58_ui32), 0x0BF000FF0FFF0F00_i64);
    ASSERT_EQ(bitClear<int64>(0x0FF0000000000000_i64, 62_ui32), 0x0FF0000000000000_i64);
    ASSERT_EQ(bitClear<int64>(0x0FF0000000000000_i64, 58_ui32), 0x0BF0000000000000_i64);

    ASSERT_EQ(bitClear<uint64>(0x00000004_ui64, 0_ui32), 0x00000004_ui64);
    ASSERT_EQ(bitClear<uint64>(0x00000004_ui64, 2_ui32), 0x00000000_ui64);
    ASSERT_EQ(bitClear<uint64>(0x0FFF0F00_ui64, 31_ui32), 0x0FFF0F00_ui64);
    ASSERT_EQ(bitClear<uint64>(0x0FFF0F00_ui64, 27_ui32), 0x07FF0F00_ui64);
    ASSERT_EQ(bitClear<uint64>(0x0FF000FF0FFF0F00_ui64, 62_ui32), 0x0FF000FF0FFF0F00_ui64);
    ASSERT_EQ(bitClear<uint64>(0x0FF000FF0FFF0F00_ui64, 58_ui32), 0x0BF000FF0FFF0F00_ui64);
    ASSERT_EQ(bitClear<uint64>(0x0FF0000000000000_ui64, 62_ui32), 0x0FF0000000000000_ui64);
    ASSERT_EQ(bitClear<uint64>(0x0FF0000000000000_ui64, 58_ui32), 0x0BF0000000000000_ui64);
}

TEST_NS(Bit, Bit, BitAnd)
{
    ASSERT_EQ(bitAnd(0x7_ui8, 0x4_ui8), 4_ui8);
    ASSERT_EQ(bitAnd(0x7_ui8, 0x4_i8), 4_ui8);
    ASSERT_EQ(bitAnd(0x7_ui16, 0x4_ui16), 4_ui16);
    ASSERT_EQ(bitAnd(0x7_ui16, 0x4_i16), 4_ui16);
    ASSERT_EQ(bitAnd(0x7_ui32, 0x4_ui32), 4_ui32);
    ASSERT_EQ(bitAnd(0x7_ui32, 0x4_i32), 4_ui32);
    ASSERT_EQ(bitAnd(0x7_ui64, 0x4_ui64), 4_ui64);
    ASSERT_EQ(bitAnd(0x7_ui64, 0x4_i64), 4_ui64);

    ASSERT_EQ(bitAnd(0x7_ui32, bitCast<float32>(0x4_i32)), 4_ui32);
    ASSERT_EQ(bitAnd(0x7_ui64, bitCast<float64>(0x4_i64)), 4_ui64);
}

TEST_NS(Bit, Bit, BitOr)
{
    ASSERT_EQ(bitOr(0x7_ui8, 0x8_ui8), 0xF_ui8);
    ASSERT_EQ(bitOr(0x7_ui8, 0x8_i8), 0xF_ui8);
    ASSERT_EQ(bitOr(0x7_ui16, 0x8_ui16), 0xF_ui16);
    ASSERT_EQ(bitOr(0x7_ui16, 0x8_i16), 0xF_ui16);
    ASSERT_EQ(bitOr(0x7_ui32, 0x8_ui32), 0xF_ui32);
    ASSERT_EQ(bitOr(0x7_ui32, 0x8_i32), 0xF_ui32);
    ASSERT_EQ(bitOr(0x7_ui64, 0x8_ui64), 0xF_ui64);
    ASSERT_EQ(bitOr(0x7_ui64, 0x8_i64), 0xF_ui64);

    ASSERT_EQ(bitOr(0x7_ui32, bitCast<float32>(0x8_i32)), 0xF_ui32);
    ASSERT_EQ(bitOr(0x7_ui64, bitCast<float64>(0x8_i64)), 0xF_ui64);
}

TEST_NS(Bit, Bit, BitXor)
{
    ASSERT_EQ(bitXor(0x7_ui8, 0x4_ui8), 3_ui8);
    ASSERT_EQ(bitXor(0x7_ui8, 0x4_i8), 3_ui8);
    ASSERT_EQ(bitXor(0x7_ui16, 0x4_ui16), 3_ui16);
    ASSERT_EQ(bitXor(0x7_ui16, 0x4_i16), 3_ui16);
    ASSERT_EQ(bitXor(0x7_ui32, 0x4_ui32), 3_ui32);
    ASSERT_EQ(bitXor(0x7_ui32, 0x4_i32), 3_ui32);
    ASSERT_EQ(bitXor(0x7_ui64, 0x4_ui64), 3_ui64);
    ASSERT_EQ(bitXor(0x7_ui64, 0x4_i64), 3_ui64);

    ASSERT_EQ(bitXor(0x7_ui32, bitCast<float32>(0x4_i32)), 3_ui32);
    ASSERT_EQ(bitXor(0x7_ui64, bitCast<float64>(0x4_i64)), 3_ui64);
}

TEST_NS(Bit, Bit, BitNot)
{
    ASSERT_EQ(bitNot(0x7_ui8), 0xF8_ui8);
    ASSERT_EQ(bitNot(0x7_i8), 0xF8_i8);
    ASSERT_EQ(bitNot(0x7_ui16), 0xFFF8_ui16);
    ASSERT_EQ(bitNot(0x7_i16), 0xFFF8_i16);
    ASSERT_EQ(bitNot(0x7_ui32), 0xFFFFFFF8_ui32);
    ASSERT_EQ(bitNot(0x7_i32), 0xFFFFFFF8_i32);
    ASSERT_EQ(bitNot(0x7_ui64), 0xFFFFFFFFFFFFFFF8_ui64);
    ASSERT_EQ(bitNot(0x7_i64), 0xFFFFFFFFFFFFFFF8_i64);

    ASSERT_EQ(bitCast<uint32>(bitNot(bitCast<float32>(0x7_i32))), 0xFFFFFFF8_ui32);
    ASSERT_EQ(bitCast<uint64>(bitNot(bitCast<float64>(0x7_i64))), 0xFFFFFFFFFFFFFFF8_ui64);
}
#endif
