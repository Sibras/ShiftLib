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

#ifdef XSTESTMAIN
#    include "XSUInt128.inl"

#    include "XSInt128.inl"

#    include <gtest/gtest.h>

using namespace Shift;

TEST(UInt128, Constructor)
{
    ASSERT_EQ(uint128(56_ui8), uint128(0_ui64, 56_ui64));
    ASSERT_EQ(uint128(56_ui16), uint128(0_ui64, 56_ui64));
    ASSERT_EQ(uint128(56_ui32), uint128(0_ui64, 56_ui64));
    ASSERT_EQ(uint128(56_ui64), uint128(0_ui64, 56_ui64));
    ASSERT_EQ(uint128(56_i8), uint128(0_ui64, 56_ui64));
    ASSERT_EQ(uint128(56_i16), uint128(0_ui64, 56_ui64));
    ASSERT_EQ(uint128(56_i32), uint128(0_ui64, 56_ui64));
    ASSERT_EQ(uint128(56_i64), uint128(0_ui64, 56_ui64));

    uint128 test1 = uint128(256_ui64, 65483245_ui64);
    ASSERT_EQ(test1, uint128(256_ui64, 65483245_ui64));

    uint128 test2 = uint128(1_ui64, UINT64_MAX);
    ASSERT_EQ(test2, uint128(1_ui64, UINT64_MAX));

    ASSERT_EQ(uint128(352.0_f32), uint128(0_ui64, 352_ui64));
    ASSERT_EQ(uint128(352.0_f64), uint128(0_ui64, 352_ui64));
    ASSERT_EQ(uint128(352.01111111_f32), uint128(0_ui64, 352_ui64));
    ASSERT_EQ(uint128(352.01111111_f64), uint128(0_ui64, 352_ui64));
    ASSERT_EQ(uint128(2.0e20_f32), uint128(10_ui64, 15532563271079952384_ui64));
    ASSERT_EQ(uint128(2.0e20_f64), uint128(10_ui64, 15532559262904483840_ui64));

    uint128 test4(test1);
    ASSERT_EQ(test4, test1);

    test4 = test2;
    ASSERT_EQ(test4, test2);
}

TEST(UInt128, Return)
{
    const uint128 test1(37);
    const uint128 test2(10_ui64, 15532563271079952384_ui64);
    const uint128 test3(10_ui64, 15532559262904483840_ui64);

    ASSERT_TRUE(static_cast<bool>(test1));
    ASSERT_FALSE(static_cast<bool>(uint128(0)));

    ASSERT_FALSE(!test1);
    ASSERT_TRUE(!uint128(0));

    ASSERT_EQ(static_cast<uint8>(test1), 37_ui8);
    ASSERT_EQ(static_cast<uint16>(test1), 37_ui16);
    ASSERT_EQ(static_cast<uint32>(test1), 37_ui32);
    ASSERT_EQ(static_cast<uint64>(test1), 37_ui64);
    ASSERT_EQ(static_cast<int8>(test1), 37_i8);
    ASSERT_EQ(static_cast<int16>(test1), 37_i16);
    ASSERT_EQ(static_cast<int32>(test1), 37_i32);
    ASSERT_EQ(static_cast<int64>(test1), 37_i64);
    ASSERT_FLOAT_EQ(static_cast<float32>(test1), 37._f32);
    ASSERT_DOUBLE_EQ(static_cast<float64>(test1), 37._f64);
    ASSERT_FLOAT_EQ(static_cast<float32>(test2), 2.0e20_f32);
    ASSERT_DOUBLE_EQ(static_cast<float64>(test3), 2.0e20_f64);
}

TEST(UInt128, Assign)
{
    uint128 test1(352_ui32);
    ASSERT_EQ(test1, uint128(0_ui64, 352_ui64));
    test1 = 37_ui8;
    ASSERT_EQ(test1, uint128(0_ui64, 37_ui64));
    test1 = 37_ui16;
    ASSERT_EQ(test1, uint128(0_ui64, 37_ui64));
    test1 = 37_ui32;
    ASSERT_EQ(test1, uint128(0_ui64, 37_ui64));
    test1 = 37_ui64;
    ASSERT_EQ(test1, uint128(0_ui64, 37_ui64));
    test1 = 37_i8;
    ASSERT_EQ(test1, uint128(0_ui64, 37_ui64));
    test1 = 37_i16;
    ASSERT_EQ(test1, uint128(0_ui64, 37_ui64));
    test1 = 37_i32;
    ASSERT_EQ(test1, uint128(0_ui64, 37_ui64));
    test1 = 37_i64;
    ASSERT_EQ(test1, uint128(0_ui64, 37_ui64));

    test1 = 352.0_f32;
    ASSERT_EQ(test1, uint128(0_ui64, 352_ui64));
    test1 = 352.0_f64;
    ASSERT_EQ(test1, uint128(0_ui64, 352_ui64));
    test1 = 2.0e20_f32;
    ASSERT_EQ(test1, uint128(10_ui64, 15532563271079952384_ui64));
    test1 = 2.0e20_f64;
    ASSERT_EQ(test1, uint128(10_ui64, 15532559262904483840_ui64));
}

TEST(UInt128, Bitwise)
{
    // TODO
}

TEST(UInt128, Shift)
{
    uint128 test2 = uint128(256_ui64, 65483245_ui64);
    ASSERT_EQ(test2, uint128(256_ui64, 65483245_ui64));

    const uint128 test3 = test2 << 32_ui8;
    ASSERT_EQ(test3, uint128(1099511627776_ui64, 281248395710955520_ui64));
    ASSERT_EQ(test3 >> 32_ui8, uint128(256_ui64, 65483245_ui64));
    ASSERT_EQ(test2 << 32_ui16, uint128(1099511627776_ui64, 281248395710955520_ui64));
    ASSERT_EQ(test2 << 32_ui32, uint128(1099511627776_ui64, 281248395710955520_ui64));
    ASSERT_EQ(test2 << 32_ui64, uint128(1099511627776_ui64, 281248395710955520_ui64));
    ASSERT_EQ(test2 << 32_ui128, uint128(1099511627776_ui64, 281248395710955520_ui64));
    ASSERT_EQ(test2 << 32_i8, uint128(1099511627776_ui64, 281248395710955520_ui64));
    ASSERT_EQ(test2 << 32_i16, uint128(1099511627776_ui64, 281248395710955520_ui64));
    ASSERT_EQ(test2 << 32_i32, uint128(1099511627776_ui64, 281248395710955520_ui64));
    ASSERT_EQ(test2 << 32_i64, uint128(1099511627776_ui64, 281248395710955520_ui64));
    ASSERT_EQ(test2 << 32_i128, uint128(1099511627776_ui64, 281248395710955520_ui64));
    ASSERT_EQ(test3 >> 32_ui16, uint128(256_ui64, 65483245_ui64));
    ASSERT_EQ(test3 >> 32_ui32, uint128(256_ui64, 65483245_ui64));
    ASSERT_EQ(test3 >> 32_ui64, uint128(256_ui64, 65483245_ui64));
    ASSERT_EQ(test3 >> 32_ui128, uint128(256_ui64, 65483245_ui64));
    ASSERT_EQ(test3 >> 32_i8, uint128(256_ui64, 65483245_ui64));
    ASSERT_EQ(test3 >> 32_i16, uint128(256_ui64, 65483245_ui64));
    ASSERT_EQ(test3 >> 32_i32, uint128(256_ui64, 65483245_ui64));
    ASSERT_EQ(test3 >> 32_i64, uint128(256_ui64, 65483245_ui64));
    ASSERT_EQ(test3 >> 32_i128, uint128(256_ui64, 65483245_ui64));

    test2 = uint128(256_ui64, 65483245_ui64);
    test2 <<= 32_ui8;
    ASSERT_EQ(test2, uint128(1099511627776_ui64, 281248395710955520_ui64));
    test2 >>= 32_ui8;
    ASSERT_EQ(test2, uint128(256_ui64, 65483245_ui64));
    test2 <<= 32_ui16;
    ASSERT_EQ(test2, uint128(1099511627776_ui64, 281248395710955520_ui64));
    test2 >>= 32_ui16;
    ASSERT_EQ(test2, uint128(256_ui64, 65483245_ui64));
    test2 <<= 32_ui32;
    ASSERT_EQ(test2, uint128(1099511627776_ui64, 281248395710955520_ui64));
    test2 >>= 32_ui32;
    ASSERT_EQ(test2, uint128(256_ui64, 65483245_ui64));
    test2 <<= 32_ui64;
    ASSERT_EQ(test2, uint128(1099511627776_ui64, 281248395710955520_ui64));
    test2 >>= 32_ui64;
    ASSERT_EQ(test2, uint128(256_ui64, 65483245_ui64));
    test2 <<= 32_ui128;
    ASSERT_EQ(test2, uint128(1099511627776_ui64, 281248395710955520_ui64));
    test2 >>= 32_ui128;
    ASSERT_EQ(test2, uint128(256_ui64, 65483245_ui64));
    test2 <<= 32_i8;
    ASSERT_EQ(test2, uint128(1099511627776_ui64, 281248395710955520_ui64));
    test2 >>= 32_i8;
    ASSERT_EQ(test2, uint128(256_ui64, 65483245_ui64));
    test2 <<= 32_i16;
    ASSERT_EQ(test2, uint128(1099511627776_ui64, 281248395710955520_ui64));
    test2 >>= 32_i16;
    ASSERT_EQ(test2, uint128(256_ui64, 65483245_ui64));
    test2 <<= 32_i32;
    ASSERT_EQ(test2, uint128(1099511627776_ui64, 281248395710955520_ui64));
    test2 >>= 32_i32;
    ASSERT_EQ(test2, uint128(256_ui64, 65483245_ui64));
    test2 <<= 32_i64;
    ASSERT_EQ(test2, uint128(1099511627776_ui64, 281248395710955520_ui64));
    test2 >>= 32_i64;
    ASSERT_EQ(test2, uint128(256_ui64, 65483245_ui64));
    test2 <<= 32_i128;
    ASSERT_EQ(test2, uint128(1099511627776_ui64, 281248395710955520_ui64));
    test2 >>= 32_i128;
    ASSERT_EQ(test2, uint128(256_ui64, 65483245_ui64));

    const uint128 test4 = test3 << 32_ui8;
    ASSERT_EQ(test4, uint128(65483245_ui64, 0_ui64));
    ASSERT_EQ(test4 >> 32_ui8, uint128(0_ui64, 281248395710955520_ui64));

    uint128 test5 = test2 << 64_ui8;
    ASSERT_EQ(test5, test4);
    ASSERT_EQ(test5 >> 64_ui8, uint128(0_ui64, 65483245_ui64));
    ASSERT_EQ(test2 << 64_ui16, test4);
    ASSERT_EQ(test2 << 64_ui32, test4);
    ASSERT_EQ(test2 << 64_ui64, test4);
    ASSERT_EQ(test2 << 64_ui128, test4);
    ASSERT_EQ(test2 << 64_i8, test4);
    ASSERT_EQ(test2 << 64_i16, test4);
    ASSERT_EQ(test2 << 64_i32, test4);
    ASSERT_EQ(test2 << 64_i64, test4);
    ASSERT_EQ(test2 << 64_i128, test4);
    ASSERT_EQ(test5 >> 64_ui16, uint128(0_ui64, 65483245_ui64));
    ASSERT_EQ(test5 >> 64_ui32, uint128(0_ui64, 65483245_ui64));
    ASSERT_EQ(test5 >> 64_ui64, uint128(0_ui64, 65483245_ui64));
    ASSERT_EQ(test5 >> 64_ui128, uint128(0_ui64, 65483245_ui64));
    ASSERT_EQ(test5 >> 64_i8, uint128(0_ui64, 65483245_ui64));
    ASSERT_EQ(test5 >> 64_i16, uint128(0_ui64, 65483245_ui64));
    ASSERT_EQ(test5 >> 64_i32, uint128(0_ui64, 65483245_ui64));
    ASSERT_EQ(test5 >> 64_i64, uint128(0_ui64, 65483245_ui64));
    ASSERT_EQ(test5 >> 64_i128, uint128(0_ui64, 65483245_ui64));

    test5 = test2;
    test2 <<= 64_ui8;
    ASSERT_EQ(test2, test4);
    test2 >>= 64_ui8;
    ASSERT_EQ(test2, uint128(0_ui64, 65483245_ui64));
    test5 = test2;
    test2 <<= 64_ui16;
    ASSERT_EQ(test2, test4);
    test2 >>= 64_ui16;
    ASSERT_EQ(test2, uint128(0_ui64, 65483245_ui64));
    test5 = test2;
    test2 <<= 64_ui32;
    ASSERT_EQ(test2, test4);
    test2 >>= 64_ui32;
    ASSERT_EQ(test2, uint128(0_ui64, 65483245_ui64));
    test5 = test2;
    test2 <<= 64_ui64;
    ASSERT_EQ(test2, test4);
    test2 >>= 64_ui64;
    ASSERT_EQ(test2, uint128(0_ui64, 65483245_ui64));
    test5 = test2;
    test2 <<= 64_ui128;
    ASSERT_EQ(test2, test4);
    test2 >>= 64_ui128;
    ASSERT_EQ(test2, uint128(0_ui64, 65483245_ui64));
    test5 = test2;
    test2 <<= 64_i8;
    ASSERT_EQ(test2, test4);
    test2 >>= 64_i8;
    ASSERT_EQ(test2, uint128(0_ui64, 65483245_ui64));
    test5 = test2;
    test2 <<= 64_i16;
    ASSERT_EQ(test2, test4);
    test2 >>= 64_i16;
    ASSERT_EQ(test2, uint128(0_ui64, 65483245_ui64));
    test5 = test2;
    test2 <<= 64_i32;
    ASSERT_EQ(test2, test4);
    test2 >>= 64_i32;
    ASSERT_EQ(test2, uint128(0_ui64, 65483245_ui64));
    test5 = test2;
    test2 <<= 64_i64;
    ASSERT_EQ(test2, test4);
    test2 >>= 64_i64;
    ASSERT_EQ(test2, uint128(0_ui64, 65483245_ui64));
    test5 = test2;
    test2 <<= 64_i128;
    ASSERT_EQ(test2, test4);
    test2 >>= 64_i128;
    ASSERT_EQ(test2, uint128(0_ui64, 65483245_ui64));
}

TEST(UInt128, Comparison)
{
    const uint128 test1 = uint128(37);
    const uint128 test2 = uint128(1_ui64, UINT64_MAX);

    ASSERT_FALSE(test1 == test2);
    ASSERT_TRUE(test1 == test1);
    ASSERT_TRUE(test2 == test2);

    ASSERT_TRUE(37_ui8 == test1);
    ASSERT_TRUE(37_ui16 == test1);
    ASSERT_TRUE(37_ui32 == test1);
    ASSERT_TRUE(37_ui64 == test1);
    ASSERT_TRUE(37_i8 == test1);
    ASSERT_TRUE(37_i16 == test1);
    ASSERT_TRUE(37_i32 == test1);
    ASSERT_TRUE(37_i64 == test1);

    ASSERT_TRUE(test1 == 37_ui8);
    ASSERT_TRUE(test1 == 37_ui16);
    ASSERT_TRUE(test1 == 37_ui32);
    ASSERT_TRUE(test1 == 37_ui64);
    ASSERT_TRUE(test1 == 37_i8);
    ASSERT_TRUE(test1 == 37_i16);
    ASSERT_TRUE(test1 == 37_i32);
    ASSERT_TRUE(test1 == 37_i64);

    ASSERT_FALSE(16_ui8 == test1);
    ASSERT_FALSE(16_ui16 == test1);
    ASSERT_FALSE(16_ui32 == test1);
    ASSERT_FALSE(16_ui64 == test1);
    ASSERT_FALSE(16_i8 == test1);
    ASSERT_FALSE(16_i16 == test1);
    ASSERT_FALSE(16_i32 == test1);
    ASSERT_FALSE(16_i64 == test1);

    ASSERT_FALSE(test1 == 16_ui8);
    ASSERT_FALSE(test1 == 16_ui16);
    ASSERT_FALSE(test1 == 16_ui32);
    ASSERT_FALSE(test1 == 16_ui64);
    ASSERT_FALSE(test1 == 16_i8);
    ASSERT_FALSE(test1 == 16_i16);
    ASSERT_FALSE(test1 == 16_i32);
    ASSERT_FALSE(test1 == 16_i64);

    ASSERT_TRUE(test1 != test2);
    ASSERT_FALSE(test1 != test1);

    ASSERT_FALSE(37_ui8 != test1);
    ASSERT_FALSE(37_ui16 != test1);
    ASSERT_FALSE(37_ui32 != test1);
    ASSERT_FALSE(37_ui64 != test1);
    ASSERT_FALSE(37_i8 != test1);
    ASSERT_FALSE(37_i16 != test1);
    ASSERT_FALSE(37_i32 != test1);
    ASSERT_FALSE(37_i64 != test1);

    ASSERT_FALSE(test1 != 37_ui8);
    ASSERT_FALSE(test1 != 37_ui16);
    ASSERT_FALSE(test1 != 37_ui32);
    ASSERT_FALSE(test1 != 37_ui64);
    ASSERT_FALSE(test1 != 37_i8);
    ASSERT_FALSE(test1 != 37_i16);
    ASSERT_FALSE(test1 != 37_i32);
    ASSERT_FALSE(test1 != 37_i64);

    ASSERT_TRUE(16_ui8 != test1);
    ASSERT_TRUE(16_ui16 != test1);
    ASSERT_TRUE(16_ui32 != test1);
    ASSERT_TRUE(16_ui64 != test1);
    ASSERT_TRUE(16_i8 != test1);
    ASSERT_TRUE(16_i16 != test1);
    ASSERT_TRUE(16_i32 != test1);
    ASSERT_TRUE(16_i64 != test1);

    ASSERT_TRUE(test1 != 16_ui8);
    ASSERT_TRUE(test1 != 16_ui16);
    ASSERT_TRUE(test1 != 16_ui32);
    ASSERT_TRUE(test1 != 16_ui64);
    ASSERT_TRUE(test1 != 16_i8);
    ASSERT_TRUE(test1 != 16_i16);
    ASSERT_TRUE(test1 != 16_i32);
    ASSERT_TRUE(test1 != 16_i64);

    ASSERT_FALSE(test1 > test2);
    ASSERT_FALSE(test2 > test2);
    ASSERT_TRUE(test2 > test1);

    ASSERT_TRUE(38_ui8 > test1);
    ASSERT_TRUE(38_ui16 > test1);
    ASSERT_TRUE(38_ui32 > test1);
    ASSERT_TRUE(38_ui64 > test1);
    ASSERT_TRUE(38_i8 > test1);
    ASSERT_TRUE(38_i16 > test1);
    ASSERT_TRUE(38_i32 > test1);
    ASSERT_TRUE(38_i64 > test1);

    ASSERT_TRUE(test1 > 36_ui8);
    ASSERT_TRUE(test1 > 36_ui16);
    ASSERT_TRUE(test1 > 36_ui32);
    ASSERT_TRUE(test1 > 36_ui64);
    ASSERT_TRUE(test1 > 36_i8);
    ASSERT_TRUE(test1 > 36_i16);
    ASSERT_TRUE(test1 > 36_i32);
    ASSERT_TRUE(test1 > 36_i64);

    ASSERT_FALSE(36_ui8 > test1);
    ASSERT_FALSE(36_ui16 > test1);
    ASSERT_FALSE(36_ui32 > test1);
    ASSERT_FALSE(36_ui64 > test1);
    ASSERT_FALSE(36_i8 > test1);
    ASSERT_FALSE(36_i16 > test1);
    ASSERT_FALSE(36_i32 > test1);
    ASSERT_FALSE(36_i64 > test1);

    ASSERT_FALSE(test1 > 38_ui8);
    ASSERT_FALSE(test1 > 38_ui16);
    ASSERT_FALSE(test1 > 38_ui32);
    ASSERT_FALSE(test1 > 38_ui64);
    ASSERT_FALSE(test1 > 38_i8);
    ASSERT_FALSE(test1 > 38_i16);
    ASSERT_FALSE(test1 > 38_i32);
    ASSERT_FALSE(test1 > 38_i64);

    ASSERT_FALSE(37_ui8 > test1);
    ASSERT_FALSE(37_ui16 > test1);
    ASSERT_FALSE(37_ui32 > test1);
    ASSERT_FALSE(37_ui64 > test1);
    ASSERT_FALSE(37_i8 > test1);
    ASSERT_FALSE(37_i16 > test1);
    ASSERT_FALSE(37_i32 > test1);
    ASSERT_FALSE(37_i64 > test1);

    ASSERT_FALSE(test1 > 37_ui8);
    ASSERT_FALSE(test1 > 37_ui16);
    ASSERT_FALSE(test1 > 37_ui32);
    ASSERT_FALSE(test1 > 37_ui64);
    ASSERT_FALSE(test1 > 37_i8);
    ASSERT_FALSE(test1 > 37_i16);
    ASSERT_FALSE(test1 > 37_i32);
    ASSERT_FALSE(test1 > 37_i64);

    ASSERT_TRUE(test1 < test2);
    ASSERT_FALSE(test1 < test1);
    ASSERT_FALSE(test2 < test1);

    ASSERT_FALSE(38_ui8 < test1);
    ASSERT_FALSE(38_ui16 < test1);
    ASSERT_FALSE(38_ui32 < test1);
    ASSERT_FALSE(38_ui64 < test1);
    ASSERT_FALSE(38_i8 < test1);
    ASSERT_FALSE(38_i16 < test1);
    ASSERT_FALSE(38_i32 < test1);
    ASSERT_FALSE(38_i64 < test1);

    ASSERT_FALSE(test1 < 36_ui8);
    ASSERT_FALSE(test1 < 36_ui16);
    ASSERT_FALSE(test1 < 36_ui32);
    ASSERT_FALSE(test1 < 36_ui64);
    ASSERT_FALSE(test1 < 36_i8);
    ASSERT_FALSE(test1 < 36_i16);
    ASSERT_FALSE(test1 < 36_i32);
    ASSERT_FALSE(test1 < 36_i64);

    ASSERT_TRUE(36_ui8 < test1);
    ASSERT_TRUE(36_ui16 < test1);
    ASSERT_TRUE(36_ui32 < test1);
    ASSERT_TRUE(36_ui64 < test1);
    ASSERT_TRUE(36_i8 < test1);
    ASSERT_TRUE(36_i16 < test1);
    ASSERT_TRUE(36_i32 < test1);
    ASSERT_TRUE(36_i64 < test1);

    ASSERT_TRUE(test1 < 38_ui8);
    ASSERT_TRUE(test1 < 38_ui16);
    ASSERT_TRUE(test1 < 38_ui32);
    ASSERT_TRUE(test1 < 38_ui64);
    ASSERT_TRUE(test1 < 38_i8);
    ASSERT_TRUE(test1 < 38_i16);
    ASSERT_TRUE(test1 < 38_i32);
    ASSERT_TRUE(test1 < 38_i64);

    ASSERT_FALSE(37_ui8 < test1);
    ASSERT_FALSE(37_ui16 < test1);
    ASSERT_FALSE(37_ui32 < test1);
    ASSERT_FALSE(37_ui64 < test1);
    ASSERT_FALSE(37_i8 < test1);
    ASSERT_FALSE(37_i16 < test1);
    ASSERT_FALSE(37_i32 < test1);
    ASSERT_FALSE(37_i64 < test1);

    ASSERT_FALSE(test1 < 37_ui8);
    ASSERT_FALSE(test1 < 37_ui16);
    ASSERT_FALSE(test1 < 37_ui32);
    ASSERT_FALSE(test1 < 37_ui64);
    ASSERT_FALSE(test1 < 37_i8);
    ASSERT_FALSE(test1 < 37_i16);
    ASSERT_FALSE(test1 < 37_i32);
    ASSERT_FALSE(test1 < 37_i64);

    ASSERT_FALSE(test1 >= test2);
    ASSERT_TRUE(test1 >= test1);
    ASSERT_TRUE(test2 >= test1);

    ASSERT_TRUE(38_ui8 >= test1);
    ASSERT_TRUE(38_ui16 >= test1);
    ASSERT_TRUE(38_ui32 >= test1);
    ASSERT_TRUE(38_ui64 >= test1);
    ASSERT_TRUE(38_i8 >= test1);
    ASSERT_TRUE(38_i16 >= test1);
    ASSERT_TRUE(38_i32 >= test1);
    ASSERT_TRUE(38_i64 >= test1);

    ASSERT_TRUE(test1 >= 36_ui8);
    ASSERT_TRUE(test1 >= 36_ui16);
    ASSERT_TRUE(test1 >= 36_ui32);
    ASSERT_TRUE(test1 >= 36_ui64);
    ASSERT_TRUE(test1 >= 36_i8);
    ASSERT_TRUE(test1 >= 36_i16);
    ASSERT_TRUE(test1 >= 36_i32);
    ASSERT_TRUE(test1 >= 36_i64);

    ASSERT_FALSE(36_ui8 >= test1);
    ASSERT_FALSE(36_ui16 >= test1);
    ASSERT_FALSE(36_ui32 >= test1);
    ASSERT_FALSE(36_ui64 >= test1);
    ASSERT_FALSE(36_i8 >= test1);
    ASSERT_FALSE(36_i16 >= test1);
    ASSERT_FALSE(36_i32 >= test1);
    ASSERT_FALSE(36_i64 >= test1);

    ASSERT_FALSE(test1 >= 38_ui8);
    ASSERT_FALSE(test1 >= 38_ui16);
    ASSERT_FALSE(test1 >= 38_ui32);
    ASSERT_FALSE(test1 >= 38_ui64);
    ASSERT_FALSE(test1 >= 38_i8);
    ASSERT_FALSE(test1 >= 38_i16);
    ASSERT_FALSE(test1 >= 38_i32);
    ASSERT_FALSE(test1 >= 38_i64);

    ASSERT_TRUE(37_ui8 >= test1);
    ASSERT_TRUE(37_ui16 >= test1);
    ASSERT_TRUE(37_ui32 >= test1);
    ASSERT_TRUE(37_ui64 >= test1);
    ASSERT_TRUE(37_i8 >= test1);
    ASSERT_TRUE(37_i16 >= test1);
    ASSERT_TRUE(37_i32 >= test1);
    ASSERT_TRUE(37_i64 >= test1);

    ASSERT_TRUE(test1 >= 37_ui8);
    ASSERT_TRUE(test1 >= 37_ui16);
    ASSERT_TRUE(test1 >= 37_ui32);
    ASSERT_TRUE(test1 >= 37_ui64);
    ASSERT_TRUE(test1 >= 37_i8);
    ASSERT_TRUE(test1 >= 37_i16);
    ASSERT_TRUE(test1 >= 37_i32);
    ASSERT_TRUE(test1 >= 37_i64);

    ASSERT_TRUE(test1 <= test2);
    ASSERT_TRUE(test1 <= test1);
    ASSERT_FALSE(test2 <= test1);

    ASSERT_FALSE(38_ui8 <= test1);
    ASSERT_FALSE(38_ui16 <= test1);
    ASSERT_FALSE(38_ui32 <= test1);
    ASSERT_FALSE(38_ui64 <= test1);
    ASSERT_FALSE(38_i8 <= test1);
    ASSERT_FALSE(38_i16 <= test1);
    ASSERT_FALSE(38_i32 <= test1);
    ASSERT_FALSE(38_i64 <= test1);

    ASSERT_FALSE(test1 <= 36_ui8);
    ASSERT_FALSE(test1 <= 36_ui16);
    ASSERT_FALSE(test1 <= 36_ui32);
    ASSERT_FALSE(test1 <= 36_ui64);
    ASSERT_FALSE(test1 <= 36_i8);
    ASSERT_FALSE(test1 <= 36_i16);
    ASSERT_FALSE(test1 <= 36_i32);
    ASSERT_FALSE(test1 <= 36_i64);

    ASSERT_TRUE(36_ui8 <= test1);
    ASSERT_TRUE(36_ui16 <= test1);
    ASSERT_TRUE(36_ui32 <= test1);
    ASSERT_TRUE(36_ui64 <= test1);
    ASSERT_TRUE(36_i8 <= test1);
    ASSERT_TRUE(36_i16 <= test1);
    ASSERT_TRUE(36_i32 <= test1);
    ASSERT_TRUE(36_i64 <= test1);

    ASSERT_TRUE(test1 <= 38_ui8);
    ASSERT_TRUE(test1 <= 38_ui16);
    ASSERT_TRUE(test1 <= 38_ui32);
    ASSERT_TRUE(test1 <= 38_ui64);
    ASSERT_TRUE(test1 <= 38_i8);
    ASSERT_TRUE(test1 <= 38_i16);
    ASSERT_TRUE(test1 <= 38_i32);
    ASSERT_TRUE(test1 <= 38_i64);

    ASSERT_TRUE(37_ui8 <= test1);
    ASSERT_TRUE(37_ui16 <= test1);
    ASSERT_TRUE(37_ui32 <= test1);
    ASSERT_TRUE(37_ui64 <= test1);
    ASSERT_TRUE(37_i8 <= test1);
    ASSERT_TRUE(37_i16 <= test1);
    ASSERT_TRUE(37_i32 <= test1);
    ASSERT_TRUE(37_i64 <= test1);

    ASSERT_TRUE(test1 <= 37_ui8);
    ASSERT_TRUE(test1 <= 37_ui16);
    ASSERT_TRUE(test1 <= 37_ui32);
    ASSERT_TRUE(test1 <= 37_ui64);
    ASSERT_TRUE(test1 <= 37_i8);
    ASSERT_TRUE(test1 <= 37_i16);
    ASSERT_TRUE(test1 <= 37_i32);
    ASSERT_TRUE(test1 <= 37_i64);
}

TEST(UInt128, Add)
{
    const uint128 test1 = uint128(352);
    const uint128 test2 = uint128(1_ui64, UINT64_MAX);

    uint128 test7 = test2 + test1;
    ASSERT_EQ(test7, uint128(2_ui64, 351_ui64));
    ASSERT_EQ(test1 + test2, uint128(2_ui64, 351_ui64));
    ASSERT_EQ(test2 + test2, uint128(3_ui64, UINT64_MAX - 1));

    ASSERT_EQ(test2 + 57_ui8, uint128(2_ui64, 56_ui64));
    ASSERT_EQ(test2 + 57_ui16, uint128(2_ui64, 56_ui64));
    ASSERT_EQ(test2 + 57_ui32, uint128(2_ui64, 56_ui64));
    ASSERT_EQ(test2 + 57_ui64, uint128(2_ui64, 56_ui64));
    ASSERT_EQ(test2 + 57_i8, uint128(2_ui64, 56_ui64));
    ASSERT_EQ(test2 + 57_i16, uint128(2_ui64, 56_ui64));
    ASSERT_EQ(test2 + 57_i32, uint128(2_ui64, 56_ui64));
    ASSERT_EQ(test2 + 57_i64, uint128(2_ui64, 56_ui64));
    ASSERT_EQ(57_ui8 + test2, uint128(2_ui64, 56_ui64));
    ASSERT_EQ(57_ui16 + test2, uint128(2_ui64, 56_ui64));
    ASSERT_EQ(57_ui32 + test2, uint128(2_ui64, 56_ui64));
    ASSERT_EQ(57_ui64 + test2, uint128(2_ui64, 56_ui64));
    ASSERT_EQ(57_i8 + test2, uint128(2_ui64, 56_ui64));
    ASSERT_EQ(57_i16 + test2, uint128(2_ui64, 56_ui64));
    ASSERT_EQ(57_i32 + test2, uint128(2_ui64, 56_ui64));
    ASSERT_EQ(57_i64 + test2, uint128(2_ui64, 56_ui64));

    test7 = test2;
    test7 += test1;
    ASSERT_EQ(test7, uint128(2_ui64, 351_ui64));
    test7 = test2;
    test7 += test2;
    ASSERT_EQ(test7, uint128(3_ui64, UINT64_MAX - 1));

    test7 = test2;
    test7 += 57_ui8;
    ASSERT_EQ(test7, uint128(2_ui64, 56_ui64));
    test7 = test2;
    test7 += 57_ui16;
    ASSERT_EQ(test7, uint128(2_ui64, 56_ui64));
    test7 = test2;
    test7 += 57_ui32;
    ASSERT_EQ(test7, uint128(2_ui64, 56_ui64));
    test7 = test2;
    test7 += 57_ui64;
    ASSERT_EQ(test7, uint128(2_ui64, 56_ui64));

    test7 = test2;
    test7 += 57_i8;
    ASSERT_EQ(test7, uint128(2_ui64, 56_ui64));
    test7 = test2;
    test7 += 57_i16;
    ASSERT_EQ(test7, uint128(2_ui64, 56_ui64));
    test7 = test2;
    test7 += 57_i32;
    ASSERT_EQ(test7, uint128(2_ui64, 56_ui64));
    test7 = test2;
    test7 += 57_i64;
    ASSERT_EQ(test7, uint128(2_ui64, 56_ui64));

    test7 = test2;
    ASSERT_EQ(test7++, test2);
    ASSERT_EQ(test7++, uint128(2_ui64, 0_ui64));
    test7 = test2;
    ASSERT_EQ(++test7, uint128(2_ui64, 0_ui64));
}

TEST(UInt128, Sub)
{
    const uint128 test1 = uint128(352_ui64);
    const uint128 test2 = uint128(2_ui64, 351_ui64);
    const uint128 test3 = uint128(1_ui64, UINT64_MAX);

    ASSERT_EQ(test2 - test1, uint128(1_ui64, UINT64_MAX));
    ASSERT_EQ(test2 - test3, uint128(352_ui64));

    ASSERT_EQ(test2 - 57_ui8, uint128(2_ui64, 294_ui64));
    ASSERT_EQ(test2 - 57_ui16, uint128(2_ui64, 294_ui64));
    ASSERT_EQ(test2 - 57_ui32, uint128(2_ui64, 294_ui64));
    ASSERT_EQ(test2 - 57_ui64, uint128(2_ui64, 294_ui64));
    ASSERT_EQ(test2 - 57_i8, uint128(2_ui64, 294_ui64));
    ASSERT_EQ(test2 - 57_i16, uint128(2_ui64, 294_ui64));
    ASSERT_EQ(test2 - 57_i32, uint128(2_ui64, 294_ui64));
    ASSERT_EQ(test2 - 57_i64, uint128(2_ui64, 294_ui64));

    uint128 test7 = test2;
    test7 -= test1;
    ASSERT_EQ(test7, uint128(1_ui64, UINT64_MAX));
    test7 = test2;
    test7 -= test3;
    ASSERT_EQ(test7, uint128(352_ui64));

    test7 = test2;
    test7 -= 57_ui8;
    ASSERT_EQ(test7, uint128(2_ui64, 294_ui64));
    test7 = test2;
    test7 -= 57_ui16;
    ASSERT_EQ(test7, uint128(2_ui64, 294_ui64));
    test7 = test2;
    test7 -= 57_ui32;
    ASSERT_EQ(test7, uint128(2_ui64, 294_ui64));
    test7 = test2;
    test7 -= 57_ui64;
    ASSERT_EQ(test7, uint128(2_ui64, 294_ui64));
    test7 = test2;
    test7 -= 57_i8;
    ASSERT_EQ(test7, uint128(2_ui64, 294_ui64));
    test7 = test2;
    test7 -= 57_i16;
    ASSERT_EQ(test7, uint128(2_ui64, 294_ui64));
    test7 = test2;
    test7 -= 57_i32;
    ASSERT_EQ(test7, uint128(2_ui64, 294_ui64));
    test7 = test2;
    test7 -= 57_i64;
    ASSERT_EQ(test7, uint128(2_ui64, 294_ui64));

    test7 = test2;
    ASSERT_EQ(test7--, uint128(2_ui64, 351_ui64));
    ASSERT_EQ(test7, uint128(2_ui64, 350_ui64));
    test7 = test2;
    ASSERT_EQ(--test7, uint128(2_ui64, 350_ui64));
}

TEST(UInt128, Mul)
{
    const uint128 test1 = uint128(352_ui64);
    const uint128 test3 = uint128(1_ui64, UINT64_MAX);

    ASSERT_EQ(test1 * test1, uint128(0_ui64, 123904_ui64));
    ASSERT_EQ(test3 * test1, uint128(703_ui64, 18446744073709551264_ui64));
    ASSERT_EQ(test1 * test3, uint128(703_ui64, 18446744073709551264_ui64));

    ASSERT_EQ(test3 * 57_ui8, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    ASSERT_EQ(test3 * 57_ui16, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    ASSERT_EQ(test3 * 57_ui32, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    ASSERT_EQ(test3 * 57_ui64, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    ASSERT_EQ(test3 * 57_i8, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    ASSERT_EQ(test3 * 57_i16, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    ASSERT_EQ(test3 * 57_i32, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    ASSERT_EQ(test3 * 57_i64, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    ASSERT_EQ(57_ui8 * test3, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    ASSERT_EQ(57_ui16 * test3, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    ASSERT_EQ(57_ui32 * test3, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    ASSERT_EQ(57_ui64 * test3, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    ASSERT_EQ(57_i8 * test3, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    ASSERT_EQ(57_i16 * test3, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    ASSERT_EQ(57_i32 * test3, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    ASSERT_EQ(57_i64 * test3, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));

    uint128 test7 = test3;
    test7 *= test1;
    ASSERT_EQ(test7, uint128(703_ui64, 18446744073709551264_ui64));
    test7 = test3;
    test7 *= 57_ui8;
    ASSERT_EQ(test7, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    test7 = test3;
    test7 *= 57_ui16;
    ASSERT_EQ(test7, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    test7 = test3;
    test7 *= 57_ui32;
    ASSERT_EQ(test7, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    test7 = test3;
    test7 *= 57_ui64;
    ASSERT_EQ(test7, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    test7 = test3;
    test7 *= 57_i8;
    ASSERT_EQ(test7, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    test7 = test3;
    test7 *= 57_i16;
    ASSERT_EQ(test7, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    test7 = test3;
    test7 *= 57_i32;
    ASSERT_EQ(test7, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
    test7 = test3;
    test7 *= 57_i64;
    ASSERT_EQ(test7, uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64));
}

TEST(UInt128, Divide)
{
    const uint128 test3 = uint128(1_ui64, UINT64_MAX);
    const uint128 test4 = uint128(703_ui64, 18446744073709551264_ui64);
    const uint128 test5 = uint128(0x71_ui64, 0xFFFFFFFFFFFFFFC7_ui64);

    ASSERT_EQ(test4 / test3, uint128(352_ui64));

    ASSERT_EQ(test5 / 57_ui8, uint128(1_ui64, UINT64_MAX));
    ASSERT_EQ(test5 / 57_ui16, uint128(1_ui64, UINT64_MAX));
    ASSERT_EQ(test5 / 57_ui32, uint128(1_ui64, UINT64_MAX));
    ASSERT_EQ(test5 / 57_ui64, uint128(1_ui64, UINT64_MAX));
    ASSERT_EQ(test5 / 57_i8, uint128(1_ui64, UINT64_MAX));
    ASSERT_EQ(test5 / 57_i16, uint128(1_ui64, UINT64_MAX));
    ASSERT_EQ(test5 / 57_i32, uint128(1_ui64, UINT64_MAX));
    ASSERT_EQ(test5 / 57_i64, uint128(1_ui64, UINT64_MAX));
    ASSERT_EQ(56_ui8 / uint128(14_ui64), uint128(0_ui64, 4_ui64));
    ASSERT_EQ(56_ui16 / uint128(14_ui64), uint128(0_ui64, 4_ui64));
    ASSERT_EQ(56_ui32 / uint128(14_ui64), uint128(0_ui64, 4_ui64));
    ASSERT_EQ(56_ui64 / uint128(14_ui64), uint128(0_ui64, 4_ui64));
    ASSERT_EQ(56_i8 / uint128(14_ui64), uint128(0_ui64, 4_ui64));
    ASSERT_EQ(56_i16 / uint128(14_ui64), uint128(0_ui64, 4_ui64));
    ASSERT_EQ(56_i32 / uint128(14_ui64), uint128(0_ui64, 4_ui64));
    ASSERT_EQ(56_i64 / uint128(14_ui64), uint128(0_ui64, 4_ui64));

    uint128 test7 = test4;
    test7 /= test3;
    ASSERT_EQ(test7, uint128(352_ui64));

    test7 = test5;
    test7 /= 57_ui8;
    ASSERT_EQ(test7, uint128(1_ui64, UINT64_MAX));
    test7 = test5;
    test7 /= 57_ui16;
    ASSERT_EQ(test7, uint128(1_ui64, UINT64_MAX));
    test7 = test5;
    test7 /= 57_ui32;
    ASSERT_EQ(test7, uint128(1_ui64, UINT64_MAX));
    test7 = test5;
    test7 /= 57_ui64;
    ASSERT_EQ(test7, uint128(1_ui64, UINT64_MAX));
    test7 = test5;
    test7 /= 57_i8;
    ASSERT_EQ(test7, uint128(1_ui64, UINT64_MAX));
    test7 = test5;
    test7 /= 57_i16;
    ASSERT_EQ(test7, uint128(1_ui64, UINT64_MAX));
    test7 = test5;
    test7 /= 57_i32;
    ASSERT_EQ(test7, uint128(1_ui64, UINT64_MAX));
    test7 = test5;
    test7 /= 57_i64;
    ASSERT_EQ(test7, uint128(1_ui64, UINT64_MAX));
}
#endif
