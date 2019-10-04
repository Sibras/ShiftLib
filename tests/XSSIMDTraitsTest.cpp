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

#include "SIMD/XSSIMDInBase.inl"

#include <gtest/gtest.h>

using namespace Shift;

TEST(SIMDTraits, hasSIMD)
{
    static_assert(hasSIMD512<float32> == (defaultSIMD == SIMD::AVX512));
    static_assert(hasSIMD512<uint32> == (defaultSIMD == SIMD::AVX512));
    static_assert(hasSIMD512<float64> == false);

    static_assert(hasSIMD256<float32> == (defaultSIMD >= SIMD::AVX));
    static_assert(hasSIMD256<uint32> == (defaultSIMD >= SIMD::AVX2));
    static_assert(hasSIMD256<float64> == false);

    static_assert(hasSIMD128<float32> == (defaultSIMD > SIMD::Scalar));
    static_assert(hasSIMD128<uint32> == (defaultSIMD > SIMD::Scalar));
    static_assert(hasSIMD128<float64> == false);
}
