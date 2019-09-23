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

#include "XSTraits.inl"

#include "XSInt128.inl"
#include "XSUInt128.inl"

#include <gtest/gtest.h>

using namespace Shift;

class Test1;
class Test2;

TEST(Traits, IsSame)
{
    static_assert(isSame<int32, uint32> == false);
    static_assert(isSame<int32, int32> == true);
    static_assert(isSame<uint32, int32> == false);
    static_assert(isSame<float32, int32> == false);
    static_assert(isSame<float32, float64> == false);
    static_assert(isSame<Test1, Test1> == true);
    static_assert(isSame<Test1, Test2> == false);
    static_assert(isSame<void, int32> == false);

    static_assert(isSameCV<int32, const int32> == false);
    static_assert(isSameCV<const int32, int32> == false);
    static_assert(isSameCV<int32, const volatile int32> == false);
    static_assert(isSameCV<const volatile int32, int32> == false);
    static_assert(isSameCV<int32, volatile int32> == false);
    static_assert(isSameCV<volatile int32, int32> == false);

    static_assert(isSame<int32, uint32> == false);
    static_assert(isSame<int32, int32> == true);
    static_assert(isSame<uint32, int32> == false);
    static_assert(isSame<float32, int32> == false);
    static_assert(isSame<float32, float64> == false);
    static_assert(isSame<Test1, Test1> == true);
    static_assert(isSame<Test1, Test2> == false);
    static_assert(isSame<void, int32> == false);

    static_assert(isSame<int32, const int32> == true);
    static_assert(isSame<const int32, int32> == true);
    static_assert(isSame<int32, const volatile int32> == true);
    static_assert(isSame<const volatile int32, int32> == true);
    static_assert(isSame<int32, volatile int32> == true);
    static_assert(isSame<volatile int32, int32> == true);
}

TEST(Traits, IsSameAny)
{
    static_assert(isSameAny<int32, int32> == true);
    static_assert(isSameAny<int32, float32> == false);
    static_assert(isSameAny<int32, float32, int32> == true);
    static_assert(isSameAny<int32, float32, float64, uint32, int32> == true);
    static_assert(isSameAny<int32, float32, float64, uint32, int64> == false);

    static_assert(isSameAny<int32, float32, float64, const int32> == true);
    static_assert(isSameAny<const int32, float32, float64, int32> == true);
    static_assert(isSameAny<int32, float32, float64, const volatile int32> == true);
    static_assert(isSameAny<const volatile int32, float32, float64, int32> == true);
    static_assert(isSameAny<int32, float32, float64, volatile int32> == true);
    static_assert(isSameAny<volatile int32, float32, float64, int32> == true);

    static_assert(isSameAnyCV<int32, const int32, float32, float64> == false);
    static_assert(isSameAnyCV<const int32, int32, float32, float64> == false);
    static_assert(isSameAnyCV<int32, const volatile int32, float32, float64> == false);
    static_assert(isSameAnyCV<const volatile int32, int32, float32, float64> == false);
    static_assert(isSameAnyCV<int32, volatile int32, float32, float64> == false);
    static_assert(isSameAnyCV<volatile int32, int32, float32, float64> == false);
}

TEST(Traits, IsInteger)
{
    static_assert(isInteger<int8> == true);
    static_assert(isInteger<uint8> == true);
    static_assert(isInteger<char> == true);
    static_assert(isInteger<int16> == true);
    static_assert(isInteger<uint16> == true);
    static_assert(isInteger<int32> == true);
    static_assert(isInteger<uint32> == true);
    static_assert(isInteger<int64> == true);
    static_assert(isInteger<uint64> == true);
    static_assert(isInteger<Int128> == true);
    static_assert(isInteger<UInt128> == true);
    static_assert(isInteger<float32> == false);
    static_assert(isInteger<float64> == false);
    static_assert(isInteger<Test1> == false);
}

TEST(Traits, IsFloat)
{
    static_assert(isFloat<float32> == true);
    static_assert(isFloat<float64> == true);
    static_assert(isFloat<int32> == false);
    static_assert(isFloat<uint32> == false);
    static_assert(isFloat<Test1> == false);
}

TEST(Traits, IsArithmetic)
{
    static_assert(isArithmetic<int8> == true);
    static_assert(isArithmetic<uint8> == true);
    static_assert(isArithmetic<char> == true);
    static_assert(isArithmetic<int16> == true);
    static_assert(isArithmetic<uint16> == true);
    static_assert(isArithmetic<int32> == true);
    static_assert(isArithmetic<uint32> == true);
    static_assert(isArithmetic<int64> == true);
    static_assert(isArithmetic<uint64> == true);
    static_assert(isArithmetic<Int128> == true);
    static_assert(isArithmetic<UInt128> == true);
    static_assert(isArithmetic<Test1> == false);
    static_assert(isArithmetic<float32> == true);
    static_assert(isArithmetic<float64> == true);
}

TEST(Traits, IsSigned)
{
    static_assert(isSigned<int8> == true);
    static_assert(isSigned<uint8> == false);
    static_assert(isSigned<int16> == true);
    static_assert(isSigned<uint16> == false);
    static_assert(isSigned<int32> == true);
    static_assert(isSigned<uint32> == false);
    static_assert(isSigned<int64> == true);
    static_assert(isSigned<uint64> == false);
    static_assert(isSigned<Int128> == true);
    static_assert(isSigned<UInt128> == false);
    static_assert(isSigned<Test1> == false);
    static_assert(isSigned<float32> == true);
    static_assert(isSigned<float64> == true);
}

TEST(Traits, IsUnSigned)
{
    static_assert(isUnsigned<int8> == false);
    static_assert(isUnsigned<uint8> == true);
    static_assert(isUnsigned<int16> == false);
    static_assert(isUnsigned<uint16> == true);
    static_assert(isUnsigned<int32> == false);
    static_assert(isUnsigned<uint32> == true);
    static_assert(isUnsigned<int64> == false);
    static_assert(isUnsigned<uint64> == true);
    static_assert(isUnsigned<Int128> == false);
    static_assert(isUnsigned<UInt128> == true);
    static_assert(isUnsigned<Test1> == false);
    static_assert(isUnsigned<float32> == false);
    static_assert(isUnsigned<float64> == false);
}

TEST(Traits, IsNative)
{
    static_assert(isNative<int8> == true);
    static_assert(isNative<uint8> == true);
    static_assert(isNative<int16> == true);
    static_assert(isNative<uint16> == true);
    static_assert(isNative<int32> == true);
    static_assert(isNative<uint32> == true);
    static_assert(isNative<int64> == true);
    static_assert(isNative<uint64> == true);
    static_assert(isNative<Int128> == false);
    static_assert(isNative<UInt128> == false);
    static_assert(isNative<Test1> == false);
    static_assert(isNative<float32> == true);
    static_assert(isNative<float64> == true);
}

TEST(Traits, Promote)
{
    static_assert(isSame<promote<int8>, int16> == true);
    static_assert(isSame<promote<uint8>, uint16> == true);
    static_assert(isSame<promote<int16>, int32> == true);
    static_assert(isSame<promote<uint16>, uint32> == true);
    static_assert(isSame<promote<int64>, Int128> == true);
    static_assert(isSame<promote<uint64>, UInt128> == true);
    static_assert(isSame<promote<UInt128>, UInt128> == true);
    static_assert(isSame<promote<Int128>, Int128> == true);
    static_assert(isSame<promote<Int128>, UInt128> == false);

    static_assert(isSameCV<promote<const int8>, int16> == false);
    static_assert(isSameCV<promote<int8>, const int16> == false);
    static_assert(isSameCV<promote<const int8>, const int16> == true);
    static_assert(isSameCV<promote<const uint8>, uint16> == false);
    static_assert(isSameCV<promote<uint8>, const uint16> == false);
    static_assert(isSameCV<promote<const uint8>, const uint16> == true);
    static_assert(isSameCV<promote<const int16>, int32> == false);
    static_assert(isSameCV<promote<int16>, const int32> == false);
    static_assert(isSameCV<promote<const int16>, const int32> == true);
    static_assert(isSameCV<promote<const uint16>, uint32> == false);
    static_assert(isSameCV<promote<uint16>, const uint32> == false);
    static_assert(isSameCV<promote<const uint16>, const uint32> == true);
    static_assert(isSameCV<promote<const int64>, Int128> == false);
    static_assert(isSameCV<promote<int64>, const Int128> == false);
    static_assert(isSameCV<promote<const int64>, const Int128> == true);
    static_assert(isSameCV<promote<const uint64>, UInt128> == false);
    static_assert(isSameCV<promote<uint64>, const UInt128> == false);
    static_assert(isSameCV<promote<const uint64>, const UInt128> == true);
    static_assert(isSameCV<promote<const UInt128>, UInt128> == false);
    static_assert(isSameCV<promote<UInt128>, const UInt128> == false);
    static_assert(isSameCV<promote<const UInt128>, const UInt128> == true);
    static_assert(isSameCV<promote<const Int128>, Int128> == false);
    static_assert(isSameCV<promote<Int128>, const Int128> == false);
    static_assert(isSameCV<promote<const Int128>, const Int128> == true);
    static_assert(isSameCV<promote<const Int128>, const UInt128> == false);
}

TEST(Traits, hasSIMD)
{
    static_assert(hasSIMD512<SIMD::Scalar> == false);
    static_assert(hasSIMD512<SIMD::SSE3> == false);
    static_assert(hasSIMD512<SIMD::SSE41> == false);
    static_assert(hasSIMD512<SIMD::SSE42> == false);
    static_assert(hasSIMD512<SIMD::AVX> == false);
    static_assert(hasSIMD512<SIMD::AVXIn128> == false);
    static_assert(hasSIMD512<SIMD::AVX2> == false);
    static_assert(hasSIMD512<SIMD::AVX2In128> == false);
    static_assert(hasSIMD512<SIMD::AVX512> == true);
    static_assert(hasSIMD512<SIMD::AVX512In128> == false);
    static_assert(hasSIMD512<SIMD::AVX512In256> == false);

    static_assert(hasSIMD256<SIMD::Scalar> == false);
    static_assert(hasSIMD256<SIMD::SSE3> == false);
    static_assert(hasSIMD256<SIMD::SSE41> == false);
    static_assert(hasSIMD256<SIMD::SSE42> == false);
    static_assert(hasSIMD256<SIMD::AVX> == true);
    static_assert(hasSIMD256<SIMD::AVXIn128> == false);
    static_assert(hasSIMD256<SIMD::AVX2> == true);
    static_assert(hasSIMD256<SIMD::AVX2In128> == false);
    static_assert(hasSIMD256<SIMD::AVX512> == true);
    static_assert(hasSIMD256<SIMD::AVX512In128> == false);
    static_assert(hasSIMD256<SIMD::AVX512In256> == true);

    static_assert(hasSIMD128<SIMD::Scalar> == false);
    static_assert(hasSIMD128<SIMD::SSE3> == true);
    static_assert(hasSIMD128<SIMD::SSE41> == true);
    static_assert(hasSIMD128<SIMD::SSE42> == true);
    static_assert(hasSIMD128<SIMD::AVX> == true);
    static_assert(hasSIMD128<SIMD::AVXIn128> == true);
    static_assert(hasSIMD128<SIMD::AVX2> == true);
    static_assert(hasSIMD128<SIMD::AVX2In128> == true);
    static_assert(hasSIMD128<SIMD::AVX512> == true);
    static_assert(hasSIMD128<SIMD::AVX512In128> == true);
    static_assert(hasSIMD128<SIMD::AVX512In256> == true);

    static_assert(hasSIMD<SIMD::Scalar> == false);
    static_assert(hasSIMD<SIMD::SSE3> == true);
    static_assert(hasSIMD<SIMD::SSE41> == true);
    static_assert(hasSIMD<SIMD::SSE42> == true);
    static_assert(hasSIMD<SIMD::AVX> == true);
    static_assert(hasSIMD<SIMD::AVXIn128> == true);
    static_assert(hasSIMD<SIMD::AVX2> == true);
    static_assert(hasSIMD<SIMD::AVX2In128> == true);
    static_assert(hasSIMD<SIMD::AVX512> == true);
    static_assert(hasSIMD<SIMD::AVX512In128> == true);
    static_assert(hasSIMD<SIMD::AVX512In256> == true);

    static_assert(hasFMA<SIMD::Scalar> == false);
    static_assert(hasFMA<SIMD::SSE3> == false);
    static_assert(hasFMA<SIMD::SSE41> == false);
    static_assert(hasFMA<SIMD::SSE42> == false);
    static_assert(hasFMA<SIMD::AVX> == false);
    static_assert(hasFMA<SIMD::AVXIn128> == false);
    static_assert(hasFMA<SIMD::AVX2> == true);
    static_assert(hasFMA<SIMD::AVX2In128> == true);
    static_assert(hasFMA<SIMD::AVX512> == true);
    static_assert(hasFMA<SIMD::AVX512In128> == true);
    static_assert(hasFMA<SIMD::AVX512In256> == true);

    static_assert(hasFMS<SIMD::Scalar> == false);
    static_assert(hasFMS<SIMD::SSE3> == false);
    static_assert(hasFMS<SIMD::SSE41> == false);
    static_assert(hasFMS<SIMD::SSE42> == false);
    static_assert(hasFMS<SIMD::AVX> == false);
    static_assert(hasFMS<SIMD::AVXIn128> == false);
    static_assert(hasFMS<SIMD::AVX2> == true);
    static_assert(hasFMS<SIMD::AVX2In128> == true);
    static_assert(hasFMS<SIMD::AVX512> == true);
    static_assert(hasFMS<SIMD::AVX512In128> == true);
    static_assert(hasFMS<SIMD::AVX512In256> == true);

    static_assert(hasFMAFree<SIMD::Scalar> == false);
    static_assert(hasFMAFree<SIMD::SSE3> == false);
    static_assert(hasFMAFree<SIMD::SSE41> == false);
    static_assert(hasFMAFree<SIMD::SSE42> == false);
    static_assert(hasFMAFree<SIMD::AVX> == false);
    static_assert(hasFMAFree<SIMD::AVXIn128> == false);
    static_assert(hasFMAFree<SIMD::AVX2> == false);
    static_assert(hasFMAFree<SIMD::AVX2In128> == false);
    static_assert(hasFMAFree<SIMD::AVX512> == true);
    static_assert(hasFMAFree<SIMD::AVX512In128> == true);
    static_assert(hasFMAFree<SIMD::AVX512In256> == true);
}
