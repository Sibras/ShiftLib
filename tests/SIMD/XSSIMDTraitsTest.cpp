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

#ifndef XSTESTMAIN
#    include "SIMD/XSSIMDTraits.hpp"

#    include "XSCompilerOptions.h"
#    include "XSTypes.hpp"

using namespace Shift;

TEST_NS(SIMDTraits, TESTISA(SIMDTraits), numValues)
{
    static_assert(numValues<float32, SIMDWidth::Scalar> == 1);
    static_assert(numValues<uint32, SIMDWidth::Scalar> == 1);
    static_assert(numValues<float64, SIMDWidth::Scalar> == 1);

    static_assert(numValues<float32, SIMDWidth::B16> == (XS_ARCH_SSE ? 4 : 1));
    static_assert(numValues<uint32, SIMDWidth::B16> == (XS_ARCH_SSE2 ? 4 : 1));
    static_assert(numValues<float64, SIMDWidth::B16> == 1);

    static_assert(numValues<float32, SIMDWidth::B32> == (XS_ARCH_AVX ? 8 : (XS_ARCH_SSE ? 4 : 1)));
    static_assert(numValues<uint32, SIMDWidth::B32> == (XS_ARCH_AVX2 ? 8 : (XS_ARCH_SSE2 ? 4 : 1)));
    static_assert(numValues<float64, SIMDWidth::B32> == 1);

    static_assert(
        numValues<float32, SIMDWidth::B64> == (XS_ARCH_AVX512F ? 16 : (XS_ARCH_AVX ? 8 : (XS_ARCH_SSE ? 4 : 1))));
    static_assert(
        numValues<uint32, SIMDWidth::B64> == (XS_ARCH_AVX512F ? 16 : (XS_ARCH_AVX2 ? 8 : (XS_ARCH_SSE2 ? 4 : 1))));
    static_assert(numValues<float64, SIMDWidth::B64> == 1);
}

TEST_NS(SIMDTraits, TESTISA(SIMDTraits), widthSIMD)
{
    static_assert(widthSIMD<float32> ==
        (XS_ARCH_AVX512F ? SIMDWidth::B64 :
                           (XS_ARCH_AVX ? SIMDWidth::B32 : (XS_ARCH_SSE ? SIMDWidth::B16 : SIMDWidth::Scalar))));

    static_assert(widthSIMD<uint32> ==
        (XS_ARCH_AVX512F ? SIMDWidth::B64 :
                           (XS_ARCH_AVX2 ? SIMDWidth::B32 : (XS_ARCH_SSE2 ? SIMDWidth::B16 : SIMDWidth::Scalar))));
}

TEST_NS(SIMDTraits, TESTISA(SIMDTraits), maxAlignment)
{
    static_assert(maxAlignment<float32, 1> == alignof(float32));
    static_assert(maxAlignment<float32, 2> == (XS_ARCH_SSE ? 8 : 4));
    static_assert(maxAlignment<float32, 3> == (XS_ARCH_SSE ? 16 : 4));
    static_assert(maxAlignment<float32, 4> == (XS_ARCH_SSE ? 16 : 4));
    static_assert(maxAlignment<float32, 5> == (XS_ARCH_AVX ? 32 : (XS_ARCH_SSE ? 16 : 4)));
    static_assert(maxAlignment<float32, 6> == (XS_ARCH_AVX ? 32 : (XS_ARCH_SSE ? 16 : 4)));
    static_assert(maxAlignment<float32, 7> == (XS_ARCH_AVX ? 32 : (XS_ARCH_SSE ? 16 : 4)));
    static_assert(maxAlignment<float32, 8> == (XS_ARCH_AVX ? 32 : (XS_ARCH_SSE ? 16 : 4)));
    static_assert(maxAlignment<float32, 9> == (XS_ARCH_AVX512F ? 64 : (XS_ARCH_AVX ? 32 : (XS_ARCH_SSE ? 16 : 4))));
    static_assert(maxAlignment<float32, 10> == (XS_ARCH_AVX512F ? 64 : (XS_ARCH_AVX ? 32 : (XS_ARCH_SSE ? 16 : 4))));
    static_assert(maxAlignment<float32, 11> == (XS_ARCH_AVX512F ? 64 : (XS_ARCH_AVX ? 32 : (XS_ARCH_SSE ? 16 : 4))));
    static_assert(maxAlignment<float32, 12> == (XS_ARCH_AVX512F ? 64 : (XS_ARCH_AVX ? 32 : (XS_ARCH_SSE ? 16 : 4))));
    static_assert(maxAlignment<float32, 13> == (XS_ARCH_AVX512F ? 64 : (XS_ARCH_AVX ? 32 : (XS_ARCH_SSE ? 16 : 4))));
    static_assert(maxAlignment<float32, 14> == (XS_ARCH_AVX512F ? 64 : (XS_ARCH_AVX ? 32 : (XS_ARCH_SSE ? 16 : 4))));
    static_assert(maxAlignment<float32, 15> == (XS_ARCH_AVX512F ? 64 : (XS_ARCH_AVX ? 32 : (XS_ARCH_SSE ? 16 : 4))));
    static_assert(maxAlignment<float32, 16> == (XS_ARCH_AVX512F ? 64 : (XS_ARCH_AVX ? 32 : (XS_ARCH_SSE ? 16 : 4))));
}

TEST_NS(SIMDTraits, TESTISA(SIMDTraits), hasSIMD)
{
    static_assert(hasSIMD512<float32> == (XS_ARCH_AVX512F == 1));
    static_assert(hasSIMD512<uint32> == (XS_ARCH_AVX512F == 1));
    static_assert(hasSIMD512<float64> == false);

    static_assert(hasSIMD256<float32> == (XS_ARCH_AVX == 1));
    static_assert(hasSIMD256<uint32> == (XS_ARCH_AVX2 == 1));
    static_assert(hasSIMD256<float64> == false);

    static_assert(hasSIMD128<float32> == (XS_ARCH_SSE == 1));
    static_assert(hasSIMD128<uint32> == (XS_ARCH_SSE2 == 1));
    static_assert(hasSIMD128<float64> == false);
}

// TODO: tests sameImpl
#endif
