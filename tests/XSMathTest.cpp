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
#    include "XSMath.hpp"

#    include "XSGTest.hpp"
#    include "XSInt128.hpp"
#    include "XSUInt128.hpp"

using namespace Shift;

#    define ASSERT_FLOAT_NEAR(x, y, z) \
        ASSERT_NEAR(static_cast<float32>(x), static_cast<float32>(y), static_cast<float32>(z))

TEST_NS(Math, Math, Sign)
{
    ASSERT_EQ(sign<int8>(64_i8, 1_i8), 64_i8);
    ASSERT_EQ(sign<int8>(64_i8, -1_i8), -64_i8);
    ASSERT_EQ(sign<int8>(-64_i8, -1_i8), 64_i8);

    ASSERT_EQ(sign<int16>(64_i16, 1_i16), 64_i16);
    ASSERT_EQ(sign<int16>(64_i16, -1_i16), -64_i16);
    ASSERT_EQ(sign<int16>(-64_i16, -1_i16), 64_i16);

    ASSERT_EQ(sign<int32>(64_i32, 1_i32), 64_i32);
    ASSERT_EQ(sign<int32>(64_i32, -1_i32), -64_i32);
    ASSERT_EQ(sign<int32>(-64_i32, -1_i32), 64_i32);

    ASSERT_EQ(sign<int64>(64_i64, 1_i64), 64_i64);
    ASSERT_EQ(sign<int64>(64_i64, -1_i64), -64_i64);
    ASSERT_EQ(sign<int64>(-64_i64, -1_i64), 64_i64);

    ASSERT_FLOAT_EQ(sign<float32>(64._f32, 1.0_f32), 64._f32);
    ASSERT_FLOAT_EQ(sign<float32>(64._f32, -1._f32), -64._f32);
    ASSERT_FLOAT_EQ(sign<float32>(-64._f32, -1._f32), 64._f32);

    ASSERT_DOUBLE_EQ(sign<float64>(64._f64, 1._f64), 64._f64);
    ASSERT_DOUBLE_EQ(sign<float64>(64._f64, -1._f64), -64._f64);
    ASSERT_DOUBLE_EQ(sign<float64>(-64._f64, -1._f64), 64._f64);
}

TEST_NS(Math, Math, Abs)
{
    ASSERT_EQ(abs<int8>(-64_i8), 64_i8);
    ASSERT_EQ(abs<int8>(64_i8), 64_i8);
    ASSERT_EQ(abs<int8>(INT8_MAX), INT8_MAX);
    ASSERT_EQ(abs<int8>(-INT8_MAX), INT8_MAX);

    ASSERT_EQ(abs<int16>(-64_i16), 64_i16);
    ASSERT_EQ(abs<int16>(64_i16), 64_i16);
    ASSERT_EQ(abs<int16>(INT16_MAX), INT16_MAX);
    ASSERT_EQ(abs<int16>(-INT16_MAX), INT16_MAX);

    ASSERT_EQ(abs<int32>(-64_i32), 64_i32);
    ASSERT_EQ(abs<int32>(64_i32), 64_i32);
    ASSERT_EQ(abs<int32>(INT32_MAX), INT32_MAX);
    ASSERT_EQ(abs<int32>(-INT32_MAX), INT32_MAX);

    ASSERT_EQ(abs<int64>(-64_i64), 64_i64);
    ASSERT_EQ(abs<int64>(64_i64), 64_i64);
    ASSERT_EQ(abs<int64>(INT64_MAX), INT64_MAX);
    ASSERT_EQ(abs<int64>(-INT64_MAX), INT64_MAX);

    ASSERT_FLOAT_EQ(abs<float32>(-64._f32), 64._f32);
    ASSERT_FLOAT_EQ(abs<float32>(64._f32), 64._f32);
    ASSERT_FLOAT_EQ(abs<float32>(FLT_MAX), FLT_MAX);
    ASSERT_FLOAT_EQ(abs<float32>(-FLT_MAX), FLT_MAX);

    ASSERT_DOUBLE_EQ(abs<float64>(-64._f64), 64._f64);
    ASSERT_DOUBLE_EQ(abs<float64>(64._f64), 64._f64);
    ASSERT_DOUBLE_EQ(abs<float64>(DBL_MAX), DBL_MAX);
    ASSERT_DOUBLE_EQ(abs<float64>(-DBL_MAX), DBL_MAX);
}

TEST_NS(Math, Math, Mul)
{
    // Has separate implementations for 32b and 64b
    ASSERT_EQ(mul<int8>(INT8_MAX, -5_i8), INT8_MAX * -5_i16);
    ASSERT_EQ(mul<int8>(INT8_MAX, 5_i8), INT8_MAX * 5_i16);
    ASSERT_EQ(mul<int8>(INT8_MIN, -5_i8), INT8_MIN * -5_i16);
    ASSERT_EQ(mul<int8>(-INT8_MAX, 5_i8), -INT8_MAX * 5_i16);

    ASSERT_EQ(mul<uint8>(UINT8_MAX, 5_ui8), UINT8_MAX * 5_ui16);

    ASSERT_EQ(mul<int16>(INT16_MAX, -5_i16), INT16_MAX * -5_i32);
    ASSERT_EQ(mul<int16>(INT16_MAX, 5_i16), INT16_MAX * 5_i32);
    ASSERT_EQ(mul<int16>(INT16_MIN, -5_i16), INT16_MIN * -5_i32);
    ASSERT_EQ(mul<int16>(-INT16_MAX, 5_i16), -INT16_MAX * 5_i32);

    ASSERT_EQ(mul<uint16>(UINT16_MAX, 5_ui16), UINT16_MAX * 5_ui32);

    ASSERT_EQ(mul<int32>(INT32_MAX, -5_i32), INT32_MAX * -5_i64);
    ASSERT_EQ(mul<int32>(INT32_MAX, 5_i32), INT32_MAX * 5_i64);
    ASSERT_EQ(mul<int32>(INT32_MIN, -5_i32), INT32_MIN * -5_i64);
    ASSERT_EQ(mul<int32>(-INT32_MAX, 5_i32), -INT32_MAX * 5_i64);

    ASSERT_EQ(mul<uint32>(UINT32_MAX, 5_ui32), UINT32_MAX * 5_ui64);

    ASSERT_EQ(mul<int64>(INT64_MAX, -5_i64), int128(-3_i64, 9223372036854775813_ui64));
    ASSERT_EQ(mul<int64>(INT64_MAX, 5_i64), int128(2_i64, 9223372036854775803_ui64));
    ASSERT_EQ(mul<int64>(INT64_MIN, -5_i64), int128(2_i64, 9223372036854775808_ui64));
    ASSERT_EQ(mul<int64>(-INT64_MAX, 5_i64), int128(-3_i64, 9223372036854775813_ui64));

    ASSERT_EQ(mul<uint64>(UINT64_MAX, 5_ui64), uint128(4_ui64, 18446744073709551611_ui64));
}

TEST_NS(Math, Math, Addc)
{
    // Has separate implementations for 32b and 64b
    uint8 carryOut;
    ASSERT_EQ(addc<uint8>(UINT8_MAX - 1_ui8, 1_ui8, 0_ui8, carryOut), UINT8_MAX);
    ASSERT_EQ(carryOut, 0_ui8);
    ASSERT_EQ(addc<uint8>(UINT8_MAX, 1_ui8, 0_ui8, carryOut), 0_ui8);
    ASSERT_EQ(carryOut, 1_ui8);
    ASSERT_EQ(addc<uint8>(UINT8_MAX - 1_ui8, 0_ui8, 1_ui8, carryOut), UINT8_MAX);
    ASSERT_EQ(carryOut, 0_ui8);
    ASSERT_EQ(addc<uint8>(UINT8_MAX, 0_ui8, 1_ui8, carryOut), 0_ui8);
    ASSERT_EQ(carryOut, 1_ui8);

    ASSERT_EQ(addc<uint16>(UINT16_MAX - 1_ui16, 1_ui16, 0_ui8, carryOut), UINT16_MAX);
    ASSERT_EQ(carryOut, 0_ui8);
    ASSERT_EQ(addc<uint16>(UINT16_MAX, 1_ui16, 0_ui8, carryOut), 0_ui16);
    ASSERT_EQ(carryOut, 1_ui8);
    ASSERT_EQ(addc<uint16>(UINT16_MAX - 1_ui16, 0_ui16, 1_ui8, carryOut), UINT16_MAX);
    ASSERT_EQ(carryOut, 0_ui8);
    ASSERT_EQ(addc<uint16>(UINT16_MAX, 0_ui16, 1_ui8, carryOut), 0_ui16);
    ASSERT_EQ(carryOut, 1_ui8);

    ASSERT_EQ(addc<uint32>(UINT32_MAX - 1_ui32, 1_ui32, 0_ui8, carryOut), UINT32_MAX);
    ASSERT_EQ(carryOut, 0_ui8);
    ASSERT_EQ(addc<uint32>(UINT32_MAX, 1_ui32, 0_ui8, carryOut), 0_ui32);
    ASSERT_EQ(carryOut, 1_ui8);
    ASSERT_EQ(addc<uint32>(UINT32_MAX - 1_ui32, 0_ui32, 1_ui8, carryOut), UINT32_MAX);
    ASSERT_EQ(carryOut, 0_ui8);
    ASSERT_EQ(addc<uint32>(UINT32_MAX, 0_ui32, 1_ui8, carryOut), 0_ui32);
    ASSERT_EQ(carryOut, 1_ui8);

    ASSERT_EQ(addc<uint64>(UINT64_MAX - 1_ui64, 1_ui64, 0_ui8, carryOut), UINT64_MAX);
    ASSERT_EQ(carryOut, 0_ui8);
    ASSERT_EQ(addc<uint64>(UINT64_MAX, 1_ui64, 0_ui8, carryOut), 0_ui64);
    ASSERT_EQ(carryOut, 1_ui8);
    ASSERT_EQ(addc<uint64>(UINT64_MAX - 1_ui64, 0_ui64, 1_ui8, carryOut), UINT64_MAX);
    ASSERT_EQ(carryOut, 0_ui8);
    ASSERT_EQ(addc<uint64>(UINT64_MAX, 0_ui64, 1_ui8, carryOut), 0_ui64);
    ASSERT_EQ(carryOut, 1_ui8);
}

TEST_NS(Math, Math, Subc)
{
    // Has separate implementations for 32b and 64b
    uint8 carryOut;
    ASSERT_EQ(subc<uint8>(1_ui8, 1_ui8, 0_ui8, carryOut), 0_ui8);
    ASSERT_EQ(carryOut, 0_ui8);
    ASSERT_EQ(subc<uint8>(0, 1_ui8, 0_ui8, carryOut), UINT8_MAX);
    ASSERT_EQ(carryOut, 1_ui8);
    ASSERT_EQ(subc<uint8>(1_ui8, 0_ui8, 1_ui8, carryOut), 0_ui8);
    ASSERT_EQ(carryOut, 0_ui8);
    ASSERT_EQ(subc<uint8>(0, 0_ui8, 1_ui8, carryOut), UINT8_MAX);
    ASSERT_EQ(carryOut, 1_ui8);

    ASSERT_EQ(subc<uint16>(1_ui16, 1_ui16, 0_ui8, carryOut), 0_ui16);
    ASSERT_EQ(carryOut, 0_ui8);
    ASSERT_EQ(subc<uint16>(0, 1_ui16, 0_ui8, carryOut), UINT16_MAX);
    ASSERT_EQ(carryOut, 1_ui8);
    ASSERT_EQ(subc<uint16>(1_ui16, 0_ui16, 1_ui8, carryOut), 0_ui16);
    ASSERT_EQ(carryOut, 0_ui8);
    ASSERT_EQ(subc<uint16>(0, 0_ui16, 1_ui8, carryOut), UINT16_MAX);
    ASSERT_EQ(carryOut, 1_ui8);

    ASSERT_EQ(subc<uint32>(1_ui32, 1_ui32, 0_ui8, carryOut), 0_ui32);
    ASSERT_EQ(carryOut, 0_ui8);
    ASSERT_EQ(subc<uint32>(0, 1_ui32, 0_ui8, carryOut), UINT32_MAX);
    ASSERT_EQ(carryOut, 1_ui8);
    ASSERT_EQ(subc<uint32>(1_ui32, 0_ui32, 1_ui8, carryOut), 0_ui32);
    ASSERT_EQ(carryOut, 0_ui8);
    ASSERT_EQ(subc<uint32>(0, 0_ui32, 1_ui8, carryOut), UINT32_MAX);
    ASSERT_EQ(carryOut, 1_ui8);

    ASSERT_EQ(subc<uint64>(1_ui64, 1_ui64, 0_ui8, carryOut), 0_ui64);
    ASSERT_EQ(carryOut, 0_ui8);
    ASSERT_EQ(subc<uint64>(0, 1_ui64, 0_ui8, carryOut), UINT64_MAX);
    ASSERT_EQ(carryOut, 1_ui8);
    ASSERT_EQ(subc<uint64>(1_ui64, 0_ui64, 1_ui8, carryOut), 0_ui64);
    ASSERT_EQ(carryOut, 0_ui8);
    ASSERT_EQ(subc<uint64>(0, 0_ui64, 1_ui8, carryOut), UINT64_MAX);
    ASSERT_EQ(carryOut, 1_ui8);
}

TEST_NS(Math, Math, Exp)
{
    ASSERT_FLOAT_EQ(exp<float32>(10._f32), 22026.465794806716516957900645284_f32);
    ASSERT_FLOAT_EQ(exp<float32>(-10._f32), 4.5399929762484851535591515560551e-5_f32);
    ASSERT_FLOAT_EQ(exp<float32>(0.2_f32), 1.2214027581601698339210719946397_f32);
    ASSERT_FLOAT_EQ(exp<float32>(-0.2_f32), 0.81873075307798185866993550861904_f32);

    ASSERT_DOUBLE_EQ(exp<float64>(10._f64), 22026.465794806716516957900645284_f64);
    ASSERT_DOUBLE_EQ(exp<float64>(-10._f64), 4.5399929762484851535591515560551e-5_f64);
    ASSERT_DOUBLE_EQ(exp<float64>(0.2_f64), 1.2214027581601698339210719946397_f64);
    ASSERT_DOUBLE_EQ(exp<float64>(-0.2_f64), 0.81873075307798185866993550861904_f64);
}

TEST_NS(Math, Math, Exp2)
{
    ASSERT_FLOAT_EQ(exp2<float32>(10._f32), 1024._f32);
    ASSERT_FLOAT_EQ(exp2<float32>(-10._f32), 0.0009765625_f32);

    ASSERT_DOUBLE_EQ(exp2<float64>(10._f64), 1024._f64);
    ASSERT_DOUBLE_EQ(exp2<float64>(-10._f64), 0.0009765625_f64);
}

TEST_NS(Math, Math, Log)
{
    ASSERT_FLOAT_EQ(log<float32>(10._f32), 2.3025850929940456840179914546844_f32);
    ASSERT_FLOAT_EQ(log<float32>(0.2_f32), -1.6094379124341003746007593332262_f32);

    ASSERT_DOUBLE_EQ(log<float64>(10._f64), 2.3025850929940456840179914546844_f64);
    ASSERT_DOUBLE_EQ(log<float64>(0.2_f64), -1.6094379124341003746007593364262_f64);
}

TEST_NS(Math, Math, Log2)
{
    ASSERT_FLOAT_EQ(log2<float32>(10._f32), 3.3219280948873623478703194294894_f32);

    ASSERT_DOUBLE_EQ(log2<float64>(10._f64), 3.3219280948873623478703194294894_f64);
}

TEST_NS(Math, Math, Fma)
{
    ASSERT_FLOAT_EQ(fma<float32>(10._f32, 7._f32, 3._f32), 73._f32);
    ASSERT_FLOAT_EQ(fma<float32>(10._f32, -7._f32, 3._f32), -67._f32);
    ASSERT_FLOAT_EQ(fma<float32>(-10._f32, 7._f32, 3._f32), -67._f32);
    ASSERT_FLOAT_EQ(fma<float32>(-10._f32, -7._f32, 3._f32), 73._f32);
    ASSERT_FLOAT_EQ(fma<float32>(10._f32, 7._f32, -3._f32), 67._f32);
    ASSERT_FLOAT_EQ(fma<float32>(10._f32, -7._f32, -3._f32), -73._f32);
    ASSERT_FLOAT_EQ(fma<float32>(-10._f32, 7._f32, -3._f32), -73._f32);
    ASSERT_FLOAT_EQ(fma<float32>(-10._f32, -7._f32, -3._f32), 67._f32);

    ASSERT_DOUBLE_EQ(fma<float64>(10._f64, 7._f64, 3._f64), 73._f64);
    ASSERT_DOUBLE_EQ(fma<float64>(10._f64, -7._f64, 3._f64), -67._f64);
    ASSERT_DOUBLE_EQ(fma<float64>(-10._f64, 7._f64, 3._f64), -67._f64);
    ASSERT_DOUBLE_EQ(fma<float64>(-10._f64, -7._f64, 3._f64), 73._f64);
    ASSERT_DOUBLE_EQ(fma<float64>(10._f64, 7._f64, -3._f64), 67._f64);
    ASSERT_DOUBLE_EQ(fma<float64>(10._f64, -7._f64, -3._f64), -73._f64);
    ASSERT_DOUBLE_EQ(fma<float64>(-10._f64, 7._f64, -3._f64), -73._f64);
    ASSERT_DOUBLE_EQ(fma<float64>(-10._f64, -7._f64, -3._f64), 67._f64);
}

TEST_NS(Math, Math, Min)
{
    ASSERT_FLOAT_EQ(fmin<float32>(10._f32, 7._f32), 7._f32);
    ASSERT_FLOAT_EQ(fmin<float32>(-10._f32, 7._f32), -10._f32);
    ASSERT_FLOAT_EQ(fmin<float32>(10._f32, -7._f32), -7._f32);
    ASSERT_FLOAT_EQ(fmin<float32>(-10._f32, -7._f32), -10._f32);

    ASSERT_DOUBLE_EQ(fmin<float64>(10._f64, 7._f64), 7._f64);
    ASSERT_DOUBLE_EQ(fmin<float64>(-10._f64, 7._f64), -10._f64);
    ASSERT_DOUBLE_EQ(fmin<float64>(10._f64, -7._f64), -7._f64);
    ASSERT_DOUBLE_EQ(fmin<float64>(-10._f64, -7._f64), -10._f64);
}

TEST_NS(Math, Math, Max)
{
    ASSERT_FLOAT_EQ(fmax<float32>(10._f32, 7._f32), 10._f32);
    ASSERT_FLOAT_EQ(fmax<float32>(-10._f32, 7._f32), 7._f32);
    ASSERT_FLOAT_EQ(fmax<float32>(10._f32, -7._f32), 10._f32);
    ASSERT_FLOAT_EQ(fmax<float32>(-10._f32, -7._f32), -7._f32);

    ASSERT_DOUBLE_EQ(fmax<float64>(10._f64, 7._f64), 10._f64);
    ASSERT_DOUBLE_EQ(fmax<float64>(-10._f64, 7._f64), 7._f64);
    ASSERT_DOUBLE_EQ(fmax<float64>(10._f64, -7._f64), 10._f64);
    ASSERT_DOUBLE_EQ(fmax<float64>(-10._f64, -7._f64), -7._f64);
}

TEST_NS(Math, Math, Copysign)
{
    ASSERT_FLOAT_EQ(copysign<float32>(10._f32, 7._f32), 10._f32);
    ASSERT_FLOAT_EQ(copysign<float32>(10._f32, -7._f32), -10._f32);
    ASSERT_FLOAT_EQ(copysign<float32>(-10._f32, 7._f32), 10._f32);
    ASSERT_FLOAT_EQ(copysign<float32>(-10._f32, -7._f32), -10._f32);

    ASSERT_DOUBLE_EQ(copysign<float64>(10._f64, 7._f64), 10._f64);
    ASSERT_DOUBLE_EQ(copysign<float64>(10._f64, -7._f64), -10._f64);
    ASSERT_DOUBLE_EQ(copysign<float64>(-10._f64, 7._f64), 10._f64);
    ASSERT_DOUBLE_EQ(copysign<float64>(-10._f64, -7._f64), -10._f64);

    ASSERT_EQ(copysign<int8>(10_i8, 7_i8), 10_i8);
    ASSERT_EQ(copysign<int8>(10_i8, -7_i8), -10_i8);
    ASSERT_EQ(copysign<int8>(-10_i8, 7_i8), 10_i8);
    ASSERT_EQ(copysign<int8>(-10_i8, -7_i8), -10_i8);

    ASSERT_EQ(copysign<int16>(10_i16, 7_i16), 10_i16);
    ASSERT_EQ(copysign<int16>(10_i16, -7_i16), -10_i16);
    ASSERT_EQ(copysign<int16>(-10_i16, 7_i16), 10_i16);
    ASSERT_EQ(copysign<int16>(-10_i16, -7_i16), -10_i16);

    ASSERT_EQ(copysign<int32>(10_i32, 7_i32), 10_i32);
    ASSERT_EQ(copysign<int32>(10_i32, -7_i32), -10_i32);
    ASSERT_EQ(copysign<int32>(-10_i32, 7_i32), 10_i32);
    ASSERT_EQ(copysign<int32>(-10_i32, -7_i32), -10_i32);

    ASSERT_EQ(copysign<int64>(10_i64, 7_i64), 10_i64);
    ASSERT_EQ(copysign<int64>(10_i64, -7_i64), -10_i64);
    ASSERT_EQ(copysign<int64>(-10_i64, 7_i64), 10_i64);
    ASSERT_EQ(copysign<int64>(-10_i64, -7_i64), -10_i64);
}

TEST_NS(Math, Math, Sqrt)
{
    ASSERT_FLOAT_EQ(sqrt<float32>(10._f32), 3.1622776601683793319988935444327_f32);
    ASSERT_FLOAT_EQ(sqrt<float32>(0.2_f32), 0.44721359549995793928183473374626_f32);

    ASSERT_DOUBLE_EQ(sqrt<float64>(10._f64), 3.1622776601683793319988935444647_f64);
    ASSERT_DOUBLE_EQ(sqrt<float64>(0.2_f64), 0.44721359549995793928183473374626_f64);
}

TEST_NS(Math, Math, Rsqrt)
{
    ASSERT_FLOAT_EQ(rsqrt<float32>(10._f32), 0.31622776601683793319988935444327_f32);
    ASSERT_FLOAT_EQ(rsqrt<float32>(0.2_f32), 2.2360679774997896964091736687313_f32);

    ASSERT_DOUBLE_EQ(rsqrt<float64>(10._f64), 0.31622776601683793319988935444327_f64);
    ASSERT_DOUBLE_EQ(rsqrt<float64>(0.2_f64), 2.2360679774997896964091736687313_f64);
}

TEST_NS(Math, Math, Sin)
{
    ASSERT_FLOAT_NEAR(sin<float32>(valPi<float32>), 0._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(sin<float32>(valPi2<float32>), 1._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(sin<float32>(-valPi<float32>), 0._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(sin<float32>(-valPi2<float32>), -1._f32, FLT_EPSILON * 2.f);

    ASSERT_NEAR(sin<float64>(valPi<float64>), 0._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(sin<float64>(valPi2<float64>), 1._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(sin<float64>(-valPi<float64>), 0._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(sin<float64>(-valPi2<float64>), -1._f64, DBL_EPSILON * 2.);
}

TEST_NS(Math, Math, Cos)
{
    ASSERT_FLOAT_NEAR(cos<float32>(valPi<float32>), -1._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(cos<float32>(valPi2<float32>), 0._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(cos<float32>(-valPi<float32>), -1._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(cos<float32>(-valPi2<float32>), 0._f32, FLT_EPSILON * 2.f);

    ASSERT_NEAR(cos<float64>(valPi<float64>), -1._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(cos<float64>(valPi2<float64>), 0._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(cos<float64>(-valPi<float64>), -1._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(cos<float64>(-valPi2<float64>), 0._f64, DBL_EPSILON * 2.);
}

TEST_NS(Math, Math, Tan)
{
    ASSERT_FLOAT_NEAR(tan<float32>(valPi<float32>), 0._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(tan<float32>(valPi4<float32>), 1._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(tan<float32>(-valPi<float32>), 0._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(tan<float32>(-valPi4<float32>), -1._f32, FLT_EPSILON * 2.f);

    ASSERT_NEAR(tan<float64>(valPi<float64>), 0._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(tan<float64>(valPi4<float64>), 1._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(tan<float64>(-valPi<float64>), 0._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(tan<float64>(-valPi4<float64>), -1._f64, DBL_EPSILON * 2.);
}

TEST_NS(Math, Math, Sincos)
{
    float32 cosRes;
    ASSERT_FLOAT_NEAR(sincos<float32>(valPi<float32>, cosRes), 0._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(cosRes, -1._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(sincos<float32>(valPi2<float32>, cosRes), 1._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(cosRes, 0._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(sincos<float32>(-valPi<float32>, cosRes), 0._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(cosRes, -1._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(sincos<float32>(-valPi2<float32>, cosRes), -1._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(cosRes, 0._f32, FLT_EPSILON * 2.f);

    float64 cosRes2;
    ASSERT_NEAR(sincos<float64>(valPi<float64>, cosRes2), 0._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(cosRes2, -1._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(sincos<float64>(valPi2<float64>, cosRes2), 1._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(cosRes2, 0._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(sincos<float64>(-valPi<float64>, cosRes2), 0._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(cosRes2, -1._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(sincos<float64>(-valPi2<float64>, cosRes2), -1._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(cosRes2, 0._f64, DBL_EPSILON * 2.);
}

TEST_NS(Math, Math, Asin)
{
    ASSERT_FLOAT_NEAR(asin<float32>(0._f32), 0._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(asin<float32>(1._f32), valPi2<float32>, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(asin<float32>(-1._f32), -valPi2<float32>, FLT_EPSILON * 2.f);

    ASSERT_NEAR(asin<float64>(0._f64), 0._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(asin<float64>(1._f64), valPi2<float64>, DBL_EPSILON * 2.);
    ASSERT_NEAR(asin<float64>(-1._f64), -valPi2<float64>, DBL_EPSILON * 2.);
}

TEST_NS(Math, Math, Acos)
{
    ASSERT_FLOAT_NEAR(acos<float32>(0._f32), valPi2<float32>, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(acos<float32>(1._f32), 0._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(acos<float32>(-1._f32), valPi<float32>, FLT_EPSILON * 2.f);

    ASSERT_NEAR(acos<float64>(0._f64), valPi2<float64>, DBL_EPSILON * 2.);
    ASSERT_NEAR(acos<float64>(1._f64), 0._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(acos<float64>(-1._f64), valPi<float64>, DBL_EPSILON * 2.);
}

TEST_NS(Math, Math, Atan)
{
    ASSERT_FLOAT_NEAR(atan<float32>(0._f32), 0._f32, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(atan<float32>(1._f32), valPi4<float32>, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(atan<float32>(-1._f32), -valPi4<float32>, FLT_EPSILON * 2.f);

    ASSERT_NEAR(atan<float64>(0._f64), 0._f64, DBL_EPSILON * 2.);
    ASSERT_NEAR(atan<float64>(1._f64), valPi4<float64>, DBL_EPSILON * 2.);
    ASSERT_NEAR(atan<float64>(-1._f64), -valPi4<float64>, DBL_EPSILON * 2.);
}

TEST_NS(Math, Math, Atan2)
{
    ASSERT_FLOAT_NEAR(atan2<float32>(2._f32, 0._f32), valPi2<float32>, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(atan2<float32>(-2._f32, 0._f32), -valPi2<float32>, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(atan2<float32>(1._f32, 1._f32), valPi4<float32>, FLT_EPSILON * 2.f);
    ASSERT_FLOAT_NEAR(atan2<float32>(1._f32, -1._f32), val3Pi4<float32>, FLT_EPSILON * 2.f);

    ASSERT_NEAR(atan2<float64>(2._f64, 0._f64), valPi2<float64>, DBL_EPSILON * 2.);
    ASSERT_NEAR(atan2<float64>(-2._f64, 0._f64), -valPi2<float64>, DBL_EPSILON * 2.);
    ASSERT_NEAR(atan2<float64>(1._f64, 1._f64), valPi4<float64>, DBL_EPSILON * 2.);
    ASSERT_NEAR(atan2<float64>(1._f64, -1._f64), val3Pi4<float64>, DBL_EPSILON * 2.);
}

TEST_NS(Math, Math, Pow)
{
    ASSERT_FLOAT_EQ(pow<float32>(10._f32, 7._f32), 10000000._f32);
    ASSERT_FLOAT_EQ(pow<float32>(0.2_f32, 7._f32), 0.0000128_f32);
    ASSERT_FLOAT_EQ(pow<float32>(-10._f32, 7._f32), -10000000._f32);
    ASSERT_FLOAT_EQ(pow<float32>(-0.2_f32, 7._f32), -0.0000128_f32);
    ASSERT_FLOAT_EQ(pow<float32>(-10._f32, 6._f32), 1000000._f32);
    ASSERT_FLOAT_EQ(pow<float32>(-0.2_f32, 6._f32), 0.000064_f32);
    ASSERT_FLOAT_EQ(pow<float32>(10._f32, -7._f32), 0.0000001_f32);
    ASSERT_FLOAT_EQ(pow<float32>(0.2_f32, -7._f32), 78125._f32);
    ASSERT_FLOAT_EQ(pow<float32>(-10._f32, -7._f32), -0.0000001_f32);
    ASSERT_FLOAT_EQ(pow<float32>(-0.2_f32, -7._f32), -78125._f32);
    ASSERT_FLOAT_EQ(pow<float32>(-10._f32, -6._f32), 0.000001_f32);
    ASSERT_FLOAT_EQ(pow<float32>(-0.2_f32, -6._f32), 15625._f32);

    ASSERT_DOUBLE_EQ(pow<float64>(10._f64, 7._f64), 10000000._f64);
    ASSERT_DOUBLE_EQ(pow<float64>(0.2_f64, 7._f64), 0.0000128_f64);
    ASSERT_DOUBLE_EQ(pow<float64>(-10._f64, 7._f64), -10000000._f64);
    ASSERT_DOUBLE_EQ(pow<float64>(-0.2_f64, 7._f64), -0.0000128_f64);
    ASSERT_DOUBLE_EQ(pow<float64>(-10._f64, 6._f64), 1000000._f64);
    ASSERT_DOUBLE_EQ(pow<float64>(-0.2_f64, 6._f64), 0.000064_f64);
    ASSERT_DOUBLE_EQ(pow<float64>(10._f64, -7._f64), 0.0000001_f64);
    ASSERT_DOUBLE_EQ(pow<float64>(0.2_f64, -7._f64), 78125._f64);
    ASSERT_DOUBLE_EQ(pow<float64>(-10._f64, -7._f64), -0.0000001_f64);
    ASSERT_DOUBLE_EQ(pow<float64>(-0.2_f64, -7._f64), -78125._f64);
    ASSERT_DOUBLE_EQ(pow<float64>(-10._f64, -6._f64), 0.000001_f64);
    ASSERT_DOUBLE_EQ(pow<float64>(-0.2_f64, -6._f64), 15625._f64);
}

TEST_NS(Math, Math, Powr)
{
    ASSERT_FLOAT_EQ(pow<float32>(10._f32, 7._f32), 10000000._f32);
    ASSERT_FLOAT_EQ(pow<float32>(0.2_f32, 7._f32), 0.0000128_f32);
    ASSERT_FLOAT_EQ(pow<float32>(10._f32, -7._f32), 0.0000001_f32);
    ASSERT_FLOAT_EQ(pow<float32>(0.2_f32, -7._f32), 78125._f32);

    ASSERT_DOUBLE_EQ(pow<float64>(10._f64, 7._f64), 10000000._f64);
    ASSERT_DOUBLE_EQ(pow<float64>(0.2_f64, 7._f64), 0.0000128_f64);
    ASSERT_DOUBLE_EQ(pow<float64>(10._f64, -7._f64), 0.0000001_f64);
    ASSERT_DOUBLE_EQ(pow<float64>(0.2_f64, -7._f64), 78125._f64);
}

TEST_NS(Math, Math, Recip)
{
    ASSERT_FLOAT_EQ(recip<float32>(10._f32), 0.1_f32);
    ASSERT_FLOAT_EQ(recip<float32>(0.2_f32), 5._f32);

    ASSERT_DOUBLE_EQ(recip<float64>(10._f64), 0.1_f64);
    ASSERT_DOUBLE_EQ(recip<float64>(0.2_f64), 5._f64);
}

TEST_NS(Math, Math, Ceil)
{
    ASSERT_FLOAT_EQ(ceil<float32>(2.5_f32), 3.0_f32);
    ASSERT_FLOAT_EQ(ceil<float32>(2.01_f32), 3.0_f32);
    ASSERT_FLOAT_EQ(ceil<float32>(-2.5_f32), -2.0_f32);
    ASSERT_FLOAT_EQ(ceil<float32>(-2.01_f32), -2.0_f32);

    ASSERT_DOUBLE_EQ(ceil<float64>(2.5_f64), 3.0_f64);
    ASSERT_DOUBLE_EQ(ceil<float64>(2.01_f64), 3.0_f64);
    ASSERT_DOUBLE_EQ(ceil<float64>(-2.5_f64), -2.0_f64);
    ASSERT_DOUBLE_EQ(ceil<float64>(-2.01_f64), -2.0_f64);
}

TEST_NS(Math, Math, Floor)
{
    ASSERT_FLOAT_EQ(floor<float32>(2.5_f32), 2.0_f32);
    ASSERT_FLOAT_EQ(floor<float32>(2.01_f32), 2.0_f32);
    ASSERT_FLOAT_EQ(floor<float32>(-2.5_f32), -3.0_f32);
    ASSERT_FLOAT_EQ(floor<float32>(-2.01_f32), -3.0_f32);

    ASSERT_DOUBLE_EQ(floor<float64>(2.5_f64), 2.0_f64);
    ASSERT_DOUBLE_EQ(floor<float64>(2.01_f64), 2.0_f64);
    ASSERT_DOUBLE_EQ(floor<float64>(-2.5_f64), -3.0_f64);
    ASSERT_DOUBLE_EQ(floor<float64>(-2.01_f64), -3.0_f64);
}

TEST_NS(Math, Math, Trunc)
{
    ASSERT_FLOAT_EQ(trunc<float32>(2.5_f32), 2.0_f32);
    ASSERT_FLOAT_EQ(trunc<float32>(2.01_f32), 2.0_f32);
    ASSERT_FLOAT_EQ(trunc<float32>(-2.5_f32), -2.0_f32);
    ASSERT_FLOAT_EQ(trunc<float32>(-2.01_f32), -2.0_f32);

    ASSERT_DOUBLE_EQ(trunc<float64>(2.5_f64), 2.0_f64);
    ASSERT_DOUBLE_EQ(trunc<float64>(2.01_f64), 2.0_f64);
    ASSERT_DOUBLE_EQ(trunc<float64>(-2.5_f64), -2.0_f64);
    ASSERT_DOUBLE_EQ(trunc<float64>(-2.01_f64), -2.0_f64);
}

TEST_NS(Math, Math, Ldexp)
{
    ASSERT_FLOAT_EQ(ldexp<float32>(7._f32, -4_i32), 0.4375_f32);
    ASSERT_FLOAT_EQ(ldexp<float32>(4._f32, 3_i32), 32._f32);

    ASSERT_DOUBLE_EQ(ldexp<float64>(7._f64, -4_i32), 0.4375_f64);
    ASSERT_DOUBLE_EQ(ldexp<float64>(4._f64, 3_i32), 32._f64);
}
#endif
