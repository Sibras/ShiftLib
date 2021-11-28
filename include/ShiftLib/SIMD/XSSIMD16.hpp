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

#include "SIMD/XSSIMD12.hpp"
#include "SIMD/XSSIMD3x4.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class SIMD16;

template<typename T>
class alignas(maxAlignment<T, 16>) SIMD16Data
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    T value0, value1, value2, value3;
    T value4, value5, value6, value7;
    T value8, value9, value10, value11;
    T value12, value13, value14, value15;

    /** Default constructor. */
    XS_FUNCTION SIMD16Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMD16Data(const SIMD16<T, Width>& other) noexcept
    {
        store(other);
    }

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param other0  The first value.
     * @param other1  The second value.
     * @param other2  The third value.
     * @param other3  The fourth value.
     * @param other4  The fifth value.
     * @param other5  The sixth value.
     * @param other6  The seventh value.
     * @param other7  The eighth value.
     * @param other8  The ninth value.
     * @param other9  The tenth value.
     * @param other10 The eleventh value.
     * @param other11 The twelfth value.
     * @param other12 The thirteenth value.
     * @param other13 The fourteenth value.
     * @param other14 The fifteenth value.
     * @param other15 The sixteenth value.
     */
    XS_FUNCTION void setData(T other0, T other1, T other2, T other3, T other4, T other5, T other6, T other7, T other8,
        T other9, T other10, T other11, T other12, T other13, T other14, T other15) noexcept
    {
        value0 = other0;
        value1 = other1;
        value2 = other2;
        value3 = other3;
        value4 = other4;
        value5 = other5;
        value6 = other6;
        value7 = other7;
        value8 = other8;
        value9 = other9;
        value10 = other10;
        value11 = other11;
        value12 = other12;
        value13 = other13;
        value14 = other14;
        value15 = other15;
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const SIMD16<T, Width>& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            _mm512_store_ps(&value0, other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            _mm256_store_ps(&value0, other.values0);
            _mm256_store_ps(&value8, other.values1);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            _mm_store_ps(&value0, other.values0);
            _mm_store_ps(&value4, other.values1);
            _mm_store_ps(&value8, other.values2);
            _mm_store_ps(&value12, other.values3);
        } else
#endif
        {
            value0 = other.values0;
            value1 = other.values1;
            value2 = other.values2;
            value3 = other.values3;
            value4 = other.values4;
            value5 = other.values5;
            value6 = other.values6;
            value7 = other.values7;
            value8 = other.values8;
            value9 = other.values9;
            value10 = other.values10;
            value11 = other.values11;
            value12 = other.values12;
            value13 = other.values13;
            value14 = other.values14;
            value15 = other.values15;
        }
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION SIMD16<T, Width> load() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD16<T, Width>(_mm512_load_ps(&value0));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD16<T, Width>(_mm256_load_ps(&value0), _mm256_load_ps(&value8));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD16<T, Width>(
                _mm_load_ps(&value0), _mm_load_ps(&value4), _mm_load_ps(&value8), _mm_load_ps(&value12));
        } else
#endif
        {
            return SIMD16<T, Width>(value0, value1, value2, value3, value4, value5, value6, value7, value8, value9,
                value10, value11, value12, value13, value14, value15);
        }
    }
};

template<typename T>
using SIMD16DataPad = SIMD16Data<T>;

template<typename T, SIMDWidth Width = widthSIMD<T>>
class SIMD16 : public NoExport::SIMDData<T, 16, 0, Width>
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    using Type = T;
    using Data = NoExport::SIMDData<T, 16, 0, Width>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Data::width;
    static constexpr uint32 numValues = 16;
    static constexpr uint32 size = Data::size;
    using BaseDef = SIMDBase<T, SIMDBase<T, widthImpl>::widthImpl>;
    using InBaseDef = SIMDInBase<T, SIMDInBase<T, widthImpl>::widthImpl>;
    using SIMD2Def = SIMD2<T, SIMD2<T, widthImpl>::widthImpl>;
    using SIMD4Def = SIMD4<T, SIMD4<T, widthImpl>::widthImpl>;
    using SIMD8Def = SIMD8<T, SIMD8<T, widthImpl>::widthImpl>;
    using SIMD12Def = SIMD12<T, SIMD12<T, widthImpl>::widthImpl>;
    using SIMD3x4Def = SIMD3x4<T, SIMD3x4<T, widthImpl>::widthImpl>;
    using Data::SIMDData;

    /** Default constructor. */
    XS_FUNCTION SIMD16() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION SIMD16(const SIMD16& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION SIMD16(SIMD16&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD16& operator=(const SIMD16& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD16& operator=(SIMD16&& other) noexcept = default;

    /**
     * Construct a from 16 different values.
     * @param value0  The first value.
     * @param value1  The second value.
     * @param value2  The third value.
     * @param value3  The fourth value.
     * @param value4  The fifth value.
     * @param value5  The sixth value.
     * @param value6  The seventh value.
     * @param value7  The eighth value.
     * @param value8  The ninth value.
     * @param value9  The tenth value.
     * @param value10 The eleventh value.
     * @param value11 The twelfth value.
     * @param value12 The thirteenth value.
     * @param value13 The fourteenth value.
     * @param value14 The fifteenth value.
     * @param value15 The sixteenth value.
     */
    XS_FUNCTION SIMD16(T value0, T value1, T value2, T value3, T value4, T value5, T value6, T value7, T value8,
        T value9, T value10, T value11, T value12, T value13, T value14, T value15) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_set_ps(value15, value14, value13, value12, value11, value10, value9, value8, value7,
                value6, value5, value4, value3, value2, value1, value0);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = _mm256_set_ps(value7, value6, value5, value4, value3, value2, value1, value0);
            this->values1 = _mm256_set_ps(value15, value14, value13, value12, value11, value10, value9, value8);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_set_ps(value3, value2, value1, value0);
            this->values1 = _mm_set_ps(value7, value6, value5, value4);
            this->values2 = _mm_set_ps(value11, value10, value9, value8);
            this->values3 = _mm_set_ps(value15, value14, value13, value12);
        } else
#endif
        {
            this->values0 = value0;
            this->values1 = value1;
            this->values2 = value2;
            this->values3 = value3;
            this->values4 = value4;
            this->values5 = value5;
            this->values6 = value6;
            this->values7 = value7;
            this->values8 = value8;
            this->values9 = value9;
            this->values10 = value10;
            this->values11 = value11;
            this->values12 = value12;
            this->values13 = value13;
            this->values14 = value14;
            this->values15 = value15;
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param value Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD16(T value) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_set1_ps(value);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = _mm256_set1_ps(value);
            this->values1 = this->values0;
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_set1_ps(value);
            this->values1 = this->values0;
            this->values2 = this->values0;
            this->values3 = this->values0;
        } else
#endif
        {
            this->values0 = value;
            this->values1 = value;
            this->values2 = value;
            this->values3 = value;
            this->values4 = value;
            this->values5 = value;
            this->values6 = value;
            this->values7 = value;
            this->values8 = value;
            this->values9 = value;
            this->values10 = value;
            this->values11 = value;
            this->values12 = value;
            this->values13 = value;
            this->values14 = value;
            this->values15 = value;
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD16(BaseDef other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = other.values;
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = other.values;
            this->values1 = other.values;
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = other.values;
            this->values1 = other.values;
            this->values2 = other.values;
            this->values3 = other.values;
        } else
#endif
        {
            this->values0 = other.value;
            this->values1 = other.value;
            this->values2 = other.value;
            this->values3 = other.value;
            this->values4 = other.value;
            this->values5 = other.value;
            this->values6 = other.value;
            this->values7 = other.value;
            this->values8 = other.value;
            this->values9 = other.value;
            this->values10 = other.value;
            this->values11 = other.value;
            this->values12 = other.value;
            this->values13 = other.value;
            this->values14 = other.value;
            this->values15 = other.value;
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD16(InBaseDef other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_broadcastss_ps(other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                this->values0 = _mm256_broadcastss_ps(other.values);
            } else {
                const __m128 val = _mm_shuffle0000_ps(other.values);
                this->values0 = _mm256_broadcastf128_ps(val);
            }
            this->values1 = this->values0;
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_shuffle0000_ps(other.values);
            this->values1 = this->values0;
            this->values2 = this->values0;
            this->values3 = this->values0;
        } else
#endif
        {
            this->values0 = other.value;
            this->values1 = other.value;
            this->values2 = other.value;
            this->values3 = other.value;
            this->values4 = other.value;
            this->values5 = other.value;
            this->values6 = other.value;
            this->values7 = other.value;
            this->values8 = other.value;
            this->values9 = other.value;
            this->values10 = other.value;
            this->values11 = other.value;
            this->values12 = other.value;
            this->values13 = other.value;
            this->values14 = other.value;
            this->values15 = other.value;
        }
    }

    /**
     * Construct a SIMD16 from 8 different SIMD2s.
     * @param other0 The first SIMD2.
     * @param other1 The second SIMD2.
     * @param other2 The third SIMD2.
     * @param other3 The fourth SIMD2.
     * @param other4 The fifth SIMD2.
     * @param other5 The sixth SIMD2.
     * @param other6 The seventh SIMD2.
     * @param other7 The eighth SIMD2.
     */
    XS_FUNCTION SIMD16(const SIMD2Def& other0, const SIMD2Def& other1, const SIMD2Def& other2, const SIMD2Def& other3,
        const SIMD2Def& other4, const SIMD2Def& other5, const SIMD2Def& other6, const SIMD2Def& other7) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_set_m128(_mm_movelh_ps(other6.values, other7.values),
                _mm_movelh_ps(other4.values, other5.values), _mm_movelh_ps(other2.values, other3.values),
                _mm_movelh_ps(other0.values, other1.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = _mm256_set_m128(
                _mm_movelh_ps(other2.values, other3.values), _mm_movelh_ps(other0.values, other1.values));
            this->values1 = _mm256_set_m128(
                _mm_movelh_ps(other6.values, other7.values), _mm_movelh_ps(other4.values, other5.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_movelh_ps(other0.values, other1.values);
            this->values1 = _mm_movelh_ps(other2.values, other3.values);
            this->values2 = _mm_movelh_ps(other4.values, other5.values);
            this->values3 = _mm_movelh_ps(other6.values, other7.values);
        } else
#endif
        {
            this->values0 = other0.values0;
            this->values1 = other0.values1;
            this->values2 = other1.values0;
            this->values3 = other1.values1;
            this->values4 = other2.values0;
            this->values5 = other2.values1;
            this->values6 = other3.values0;
            this->values7 = other3.values1;
            this->values8 = other4.values0;
            this->values9 = other4.values1;
            this->values10 = other5.values0;
            this->values11 = other5.values1;
            this->values12 = other6.values0;
            this->values13 = other6.values1;
            this->values14 = other7.values0;
            this->values15 = other7.values1;
        }
    }

    /**
     * Construct from a single SIMD2.
     * @param other The SIMD2.
     */
    XS_FUNCTION explicit SIMD16(const SIMD2Def& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_broadcastf64_ps(other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                this->values0 = _mm256_broadcastf64_ps(other.values);
                this->values1 = this->values0;
            } else {
                const __m128 val = _mm_shuffle1010_ps(other.values);
                this->values0 = _mm256_broadcastf128_ps(val);
                this->values1 = this->values0;
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_shuffle1010_ps(other.values);
            this->values1 = this->values0;
            this->values2 = this->values0;
            this->values3 = this->values0;
        } else
#endif
        {
            this->values0 = other.values0;
            this->values1 = other.values1;
            this->values2 = other.values0;
            this->values3 = other.values1;
            this->values4 = other.values0;
            this->values5 = other.values1;
            this->values6 = other.values0;
            this->values7 = other.values1;
            this->values8 = other.values0;
            this->values9 = other.values1;
            this->values10 = other.values0;
            this->values11 = other.values1;
            this->values12 = other.values0;
            this->values13 = other.values1;
            this->values14 = other.values0;
            this->values15 = other.values1;
        }
    }

    /**
     * Construct from 4 different SIMD4s.
     * @param other0 The first SIMD4.
     * @param other1 The second SIMD4.
     * @param other2 The third SIMD4.
     * @param other3 The fourth SIMD4.
     */
    XS_FUNCTION SIMD16(
        const SIMD4Def& other0, const SIMD4Def& other1, const SIMD4Def& other2, const SIMD4Def& other3) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_set_m128(other3.values, other2.values, other1.values, other0.values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = _mm256_set_m128(other1.values, other0.values);
            this->values1 = _mm256_set_m128(other3.values, other2.values);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = other0.values;
            this->values1 = other1.values;
            this->values2 = other2.values;
            this->values3 = other3.values;
        } else
#endif
        {
            this->values0 = other0.values0;
            this->values1 = other0.values1;
            this->values2 = other0.values2;
            this->values3 = other0.values3;
            this->values4 = other1.values0;
            this->values5 = other1.values1;
            this->values6 = other1.values2;
            this->values7 = other1.values3;
            this->values8 = other2.values0;
            this->values9 = other2.values1;
            this->values10 = other2.values2;
            this->values11 = other2.values3;
            this->values12 = other3.values0;
            this->values13 = other3.values1;
            this->values14 = other3.values2;
            this->values15 = other3.values3;
        }
    }

    /**
     * Construct from a single SIMD4.
     * @param other The SIMD4.
     */
    XS_FUNCTION explicit SIMD16(const SIMD4Def& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_broadcastf128_ps(other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = _mm256_broadcastf128_ps(other.values);
            this->values1 = this->values0;
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = other.values;
            this->values1 = other.values;
            this->values2 = other.values;
            this->values3 = other.values;
        } else
#endif
        {
            this->values0 = other.values0;
            this->values1 = other.values1;
            this->values2 = other.values2;
            this->values3 = other.values3;
            this->values4 = other.values0;
            this->values5 = other.values1;
            this->values6 = other.values2;
            this->values7 = other.values3;
            this->values8 = other.values0;
            this->values9 = other.values1;
            this->values10 = other.values2;
            this->values11 = other.values3;
            this->values12 = other.values0;
            this->values13 = other.values1;
            this->values14 = other.values2;
            this->values15 = other.values3;
        }
    }

    /**
     * Construct from 2 different SIMD8s.
     * @param other0 The first SIMD8.
     * @param other1 The second SIMD8.
     */
    XS_FUNCTION SIMD16(const SIMD8Def& other0, const SIMD8Def& other1) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_set_m256(other1.values, other0.values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = other0.values;
            this->values1 = other1.values;
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = other0.values0;
            this->values1 = other0.values1;
            this->values2 = other1.values0;
            this->values3 = other1.values1;
        } else
#endif
        {
            this->values0 = other0.values0;
            this->values1 = other0.values1;
            this->values2 = other0.values2;
            this->values3 = other0.values3;
            this->values4 = other0.values4;
            this->values5 = other0.values5;
            this->values6 = other0.values6;
            this->values7 = other0.values7;
            this->values8 = other1.values0;
            this->values9 = other1.values1;
            this->values10 = other1.values2;
            this->values11 = other1.values3;
            this->values12 = other1.values4;
            this->values13 = other1.values5;
            this->values14 = other1.values6;
            this->values15 = other1.values7;
        }
    }

    /**
     * Construct from a single SIMD8.
     * @param other The SIMD8.
     */
    XS_FUNCTION explicit SIMD16(const SIMD8Def& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_broadcastf256_ps(other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = other.values;
            this->values1 = other.values;
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = other.values0;
            this->values1 = other.values1;
            this->values2 = other.values0;
            this->values3 = other.values1;
        } else
#endif
        {
            this->values0 = other.values0;
            this->values1 = other.values1;
            this->values2 = other.values2;
            this->values3 = other.values3;
            this->values4 = other.values4;
            this->values5 = other.values5;
            this->values6 = other.values6;
            this->values7 = other.values7;
            this->values8 = other.values0;
            this->values9 = other.values1;
            this->values10 = other.values2;
            this->values11 = other.values3;
            this->values12 = other.values4;
            this->values13 = other.values5;
            this->values14 = other.values6;
            this->values15 = other.values7;
        }
    }

    /**
     * Construct from a SIMD12 and a SIMD4.
     * @param other0 The SIMD12.
     * @param other1 The SIMD4.
     */
    XS_FUNCTION SIMD16(const SIMD12Def& other0, const SIMD4Def& other1) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_insertf32x4(other0.values, other1.values, 3);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = other0.values0;
            this->values1 = _mm256_set_m128(other1.values, other0.values1);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = other0.values0;
            this->values1 = other0.values1;
            this->values2 = other0.values2;
            this->values3 = other1.values;
        } else
#endif
        {
            this->values0 = other0.values0;
            this->values1 = other0.values1;
            this->values2 = other0.values2;
            this->values3 = other0.values3;
            this->values4 = other0.values4;
            this->values5 = other0.values5;
            this->values6 = other0.values6;
            this->values7 = other0.values7;
            this->values8 = other0.values8;
            this->values9 = other0.values9;
            this->values10 = other0.values10;
            this->values11 = other0.values11;
            this->values12 = other1.values0;
            this->values13 = other1.values1;
            this->values14 = other1.values2;
            this->values15 = other1.values3;
        }
    }

    /**
     * Construct from a SIMD3x4 and a SIMD4.
     * @param other0 The SIMD3x4.
     * @param other1 The SIMD4.
     */
    XS_FUNCTION SIMD16(const SIMD3x4Def& other0, const SIMD4Def& other1) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_mask_insertf32x8(other0.values, _cvtu32_mask16(0x8888),
                _mm512_castps256_ps512(
                    _mm256_set_m128(_mm_shuffle1010_ps(other1.values), _mm_shuffle0000_ps(other1.values))),
                _mm256_set_m128(_mm_shuffle3232_ps(other1.values), _mm_shuffle2200_ps(other1.values)), 1);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = _mm256_blend_set_m128(other0.values0, _MM256_BLEND(1, 0, 0, 0, 1, 0, 0, 0),
                _mm_shuffle1010_ps(other1.values), _mm_shuffle0000_ps(other1.values));
            this->values1 = _mm256_blend_set_m128(other0.values1, _MM256_BLEND(1, 0, 0, 0, 1, 0, 0, 0),
                _mm_shuffle3232_ps(other1.values), _mm_shuffle2200_ps(other1.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_insert_ps(other0.values0, other1.values, _MM_MK_INSERTPS_NDX(0, 3, 0));
            this->values1 = _mm_insert_ps(other0.values1, other1.values, _MM_MK_INSERTPS_NDX(1, 3, 0));
            this->values2 = _mm_insert_ps(other0.values2, other1.values, _MM_MK_INSERTPS_NDX(2, 3, 0));
            this->values3 = _mm_blend_ps(other0.values3, other1.values, _MM_BLEND(1, 0, 0, 0));
        } else
#endif
        {
            this->values0 = other0.values0;
            this->values1 = other0.values1;
            this->values2 = other0.values2;
            this->values3 = other1.values0;
            this->values4 = other0.values3;
            this->values5 = other0.values4;
            this->values6 = other0.values5;
            this->values7 = other1.values1;
            this->values8 = other0.values6;
            this->values9 = other0.values7;
            this->values10 = other0.values8;
            this->values11 = other1.values2;
            this->values12 = other0.values9;
            this->values13 = other0.values10;
            this->values14 = other0.values11;
            this->values15 = other1.values3;
        }
    }

    /**
     * Constructor to build set to 0.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD16 Zero() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD16(_mm512_setzero_ps());
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 value = _mm256_setzero_ps();
            return SIMD16(value, value);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 value = _mm_setzero_ps();
            return SIMD16(value, value, value, value);
        } else
#endif
        {
            return SIMD16(T{0});
        }
    }

    /**
     * Constructor to build set to 1.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD16 One() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD16(_mm512_set1_ps(1.0f));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 value = _mm256_set1_ps(1.0f);
            return SIMD16(value, value);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 value = _mm_set1_ps(1.0f);
            return SIMD16(value, value, value, value);
        } else
#endif
        {
            return SIMD16(T{1});
        }
    }

    /**
     * Construct from a SIMD3x4 by duplicating values within each sub SIMD3 .
     * @note This differs from the normal shuffle3 as it returns 4 elements from each subgroup.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-2).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-2).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-2).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-2).
     * @param other The SIMD3x4.
     * @returns Newly constructed object with required attributes.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
    XS_FUNCTION static SIMD16 Shuffle4(const SIMD3x4Def& other) noexcept
    {
        static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3 && Index3 < 3, "Invalid Index: Indexes must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
                return SIMD16(_mm512_shuffle2200_ps(other.values));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
                return SIMD16(_mm512_shuffle1100_ps(other.values));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
                return SIMD16(_mm512_shuffle1010_ps(other.values));
            } else {
                return SIMD16(_mm512_shuffle1_ps(other.values, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
                return SIMD16(_mm256_shuffle2200_ps(other.values0), _mm256_shuffle2200_ps(other.values1));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
                return SIMD16(_mm256_shuffle1100_ps(other.values0), _mm256_shuffle1100_ps(other.values1));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
                return SIMD16(_mm256_shuffle1010_ps(other.values0), _mm256_shuffle1010_ps(other.values1));
            } else {
                return SIMD16(_mm256_shuffle1_ps(other.values0, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                    _mm256_shuffle1_ps(other.values1, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
                return SIMD16(_mm_shuffle1010_ps(other.values0), _mm_shuffle1010_ps(other.values1),
                    _mm_shuffle1010_ps(other.values2), _mm_shuffle1010_ps(other.values3));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
                return SIMD16(_mm_shuffle1100_ps(other.values0), _mm_shuffle1100_ps(other.values1),
                    _mm_shuffle1100_ps(other.values2), _mm_shuffle1100_ps(other.values3));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
                return SIMD16(_mm_shuffle2200_ps(other.values0), _mm_shuffle2200_ps(other.values1),
                    _mm_shuffle2200_ps(other.values2), _mm_shuffle2200_ps(other.values3));
            } else {
                return SIMD16(_mm_shuffle1_ps(other.values0, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                    _mm_shuffle1_ps(other.values1, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                    _mm_shuffle1_ps(other.values2, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                    _mm_shuffle1_ps(other.values3, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
            }
        } else
#endif
        {
            return SIMD16((&other.values0)[Index0], (&other.values0)[Index1], (&other.values0)[Index2],
                (&other.values0)[Index3], (&other.values0)[Index0 + 3], (&other.values0)[Index1 + 3],
                (&other.values0)[Index2 + 3], (&other.values0)[Index3 + 3], (&other.values0)[Index0 + 6],
                (&other.values0)[Index1 + 6], (&other.values0)[Index2 + 6], (&other.values0)[Index3 + 6],
                (&other.values0)[Index0 + 9], (&other.values0)[Index1 + 9], (&other.values0)[Index2 + 9],
                (&other.values0)[Index3 + 9]);
        }
    }

    /**
     * Construct from a SIMD12 by duplicating values within each sub SIMD3 .
     * @note This differs from the normal shuffle3 as it returns 4 elements from each subgroup.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-2).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-2).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-2).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-2).
     * @param other The SIMD12.
     * @returns Newly constructed object with required attributes.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
    XS_FUNCTION static SIMD16 Shuffle4(const SIMD12Def& other) noexcept
    {
        static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3 && Index3 < 3, "Invalid Index: Indexes must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD16(_mm512_permutexvar_ps(
                _mm512_set_epi32(Index3 * 4 + 3, Index2 * 4 + 3, Index1 * 4 + 3, Index0 * 4 + 3, Index3 * 4 + 2,
                    Index2 * 4 + 2, Index1 * 4 + 2, Index0 * 4 + 2, Index3 * 4 + 1, Index2 * 4 + 1, Index1 * 4 + 1,
                    Index0 * 4 + 1, Index3 * 4, Index2 * 4, Index1 * 4, Index0 * 4),
                other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 vals[3] = {
                _mm256_castps256_ps128(other.values0), _mm256_extractf128_ps(other.values0, 1), other.values1};
            const __m128 val1 = _mm_unpacklo_ps(vals[Index0], vals[Index1]);
            const __m128 val2 = _mm_unpacklo_ps(vals[Index2], vals[Index3]);
            const __m128 val3 = _mm_unpackhi_ps(vals[Index0], vals[Index1]);
            const __m128 val4 = _mm_unpackhi_ps(vals[Index2], vals[Index3]);
            return SIMD16(_mm256_set_m128(_mm_movehl_ps(val2, val1), _mm_movelh_ps(val1, val2)),
                _mm256_set_m128(_mm_movehl_ps(val4, val3), _mm_movelh_ps(val3, val4)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val1 = _mm_unpacklo_ps((&other.values0)[Index0], (&other.values0)[Index1]);
            const __m128 val2 = _mm_unpacklo_ps((&other.values0)[Index2], (&other.values0)[Index3]);
            const __m128 val3 = _mm_unpackhi_ps((&other.values0)[Index0], (&other.values0)[Index1]);
            const __m128 val4 = _mm_unpackhi_ps((&other.values0)[Index2], (&other.values0)[Index3]);
            return SIMD16(_mm_movelh_ps(val1, val2), _mm_movehl_ps(val2, val1), _mm_movelh_ps(val3, val4),
                _mm_movehl_ps(val4, val3));
        } else
#endif
        {
            return SIMD16((&other.values0)[Index0 * 4], (&other.values0)[Index1 * 4], (&other.values0)[Index2 * 4],
                (&other.values0)[Index3 * 4], (&other.values0)[Index0 * 4 + 1], (&other.values0)[Index1 * 4 + 1],
                (&other.values0)[Index2 * 4 + 1], (&other.values0)[Index3 * 4 + 1], (&other.values0)[Index0 * 4 + 2],
                (&other.values0)[Index1 * 4 + 2], (&other.values0)[Index2 * 4 + 2], (&other.values0)[Index3 * 4 + 2],
                (&other.values0)[Index0 * 4 + 3], (&other.values0)[Index1 * 4 + 3], (&other.values0)[Index2 * 4 + 3],
                (&other.values0)[Index3 * 4 + 3]);
        }
    }

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-15).
     * @returns SIMDInBase containing the desired value.
     */
    template<uint32 Index>
    XS_FUNCTION InBaseDef getValueInBase() const noexcept
    {
        static_assert(Index < 16, "Invalid Index: Index must be <16");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index == 0) {
                return InBaseDef(_mm512_castps512_ps128(this->values));
            } else if constexpr (Index == 1) {
                return InBaseDef(_mm_shuffle3311_ps(_mm512_castps512_ps128(this->values)));
            } else if constexpr (Index == 2) {
                return InBaseDef(_mm_shuffle3232_ps(_mm512_castps512_ps128(this->values)));
            } else if constexpr (Index == 3) {
                return InBaseDef(_mm_shuffle3333_ps(_mm512_castps512_ps128(this->values)));
            } else if constexpr (Index % 4 == 0) {
                return InBaseDef(_mm512_extractf32x4_ps(this->values, Index / 4));
            } else if constexpr (Index % 4 == 1) {
                const __m128 val = _mm512_extractf32x4_ps(this->values, Index / 4);
                return InBaseDef(_mm_shuffle3311_ps(val));
            } else if constexpr (Index % 4 == 2) {
                const __m128 val = _mm512_extractf32x4_ps(this->values, Index / 4);
                return InBaseDef(_mm_shuffle3232_ps(val));
            } else /*Index % 4 == 3*/ {
                const __m128 val = _mm512_extractf32x4_ps(this->values, Index / 4);
                return InBaseDef(_mm_shuffle3333_ps(val));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index % 8 == 0) {
                return InBaseDef(_mm256_castps256_ps128((&this->values0)[Index / 8]));
            } else if constexpr (Index % 8 == 1) {
                return InBaseDef(_mm_shuffle3311_ps(_mm256_castps256_ps128((&this->values0)[Index / 8])));
            } else if constexpr (Index % 8 == 2) {
                return InBaseDef(_mm_shuffle3232_ps(_mm256_castps256_ps128((&this->values0)[Index / 8])));
            } else if constexpr (Index % 8 == 3) {
                return InBaseDef(_mm_shuffle3333_ps(_mm256_castps256_ps128((&this->values0)[Index / 8])));
            } else if constexpr (Index % 8 == 4) {
                return InBaseDef(_mm256_extractf128_ps((&this->values0)[Index / 8], 1));
            } else if constexpr (Index % 8 == 5) {
                const __m128 val = _mm256_extractf128_ps((&this->values0)[Index / 8], 1);
                return InBaseDef(_mm_shuffle3311_ps(val));
            } else if constexpr (Index % 8 == 6) {
                const __m128 val = _mm256_extractf128_ps((&this->values0)[Index / 8], 1);
                return InBaseDef(_mm_shuffle3232_ps(val));
            } else /*Index % 8 == 7*/ {
                const __m128 val = _mm256_extractf128_ps((&this->values0)[Index / 8], 1);
                return InBaseDef(_mm_shuffle3333_ps(val));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 4 == 0) {
                return InBaseDef((&this->values0)[Index / 4]);
            } else if constexpr (Index % 4 == 1) {
                return InBaseDef(_mm_shuffle3311_ps((&this->values0)[Index / 4]));
            } else if constexpr (Index % 4 == 2) {
                return InBaseDef(_mm_shuffle3232_ps((&this->values0)[Index / 4]));
            } else /*Index % 4 == 3*/ {
                return InBaseDef(_mm_shuffle3333_ps((&this->values0)[Index / 4]));
            }
        } else
#endif
        {
            return InBaseDef((&this->values0)[Index]);
        }
    }

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-15).
     * @returns SIMDBase containing the desired value.
     */
    template<uint32 Index>
    XS_FUNCTION BaseDef getValue() const noexcept
    {
        static_assert(Index < 16, "Invalid Index: Index must be <16");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index == 0) {
                return BaseDef(_mm512_broadcastss_ps(_mm512_castps512_ps128(this->values)));
            } else {
                const __m512 val =
                    _mm512_shuffle1_ps(this->values, _MM_SHUFFLE(Index % 4, Index % 4, Index % 4, Index % 4));
                return BaseDef(_mm512_shuffle_f32x4(val, val, _MM_SHUFFLE(Index / 4, Index / 4, Index / 4, Index / 4)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX2> && Index % 8 == 0) {
                return BaseDef(_mm256_broadcastss_ps(_mm256_castps256_ps128((&this->values0)[Index / 8])));
            } else if constexpr (Index % 8 == 0) {
                const __m128 val = _mm_shuffle0000_ps(_mm256_castps256_ps128((&this->values0)[Index / 8]));
                return BaseDef(_mm256_broadcastf128_ps(val));
            } else if constexpr (Index % 8 == 1) {
                const __m128 val = _mm_shuffle1111_ps(_mm256_castps256_ps128((&this->values0)[Index / 8]));
                return BaseDef(_mm256_broadcastf128_ps(val));
            } else if constexpr (Index % 8 == 2) {
                const __m128 val = _mm_shuffle2222_ps(_mm256_castps256_ps128((&this->values0)[Index / 8]));
                return BaseDef(_mm256_broadcastf128_ps(val));
            } else if constexpr (Index % 8 == 3) {
                const __m128 val = _mm_shuffle3333_ps(_mm256_castps256_ps128((&this->values0)[Index / 8]));
                return BaseDef(_mm256_broadcastf128_ps(val));
            } else if constexpr (Index % 8 == 4) {
                const __m256 val = _mm256_shuffle1_ps((&this->values0)[Index / 8], _MM_SHUFFLE(0, 0, 0, 0));
                return BaseDef(_mm256_shuffle76547654_ps(val));
            } else if constexpr (Index % 8 == 5) {
                const __m256 val = _mm256_shuffle1_ps((&this->values0)[Index / 8], _MM_SHUFFLE(1, 1, 1, 1));
                return BaseDef(_mm256_shuffle76547654_ps(val));
            } else if constexpr (Index % 8 == 6) {
                const __m256 val = _mm256_shuffle1_ps((&this->values0)[Index / 8], _MM_SHUFFLE(2, 2, 2, 2));
                return BaseDef(_mm256_shuffle76547654_ps(val));
            } else /*Index % 8  == 7*/ {
                const __m256 val = _mm256_shuffle1_ps((&this->values0)[Index / 8], _MM_SHUFFLE(3, 3, 3, 3));
                return BaseDef(_mm256_shuffle76547654_ps(val));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 4 == 0) {
                return BaseDef(_mm_shuffle0000_ps((&this->values0)[Index / 4]));
            } else if constexpr (Index % 4 == 1) {
                return BaseDef(_mm_shuffle1111_ps((&this->values0)[Index / 4]));
            } else if constexpr (Index % 4 == 2) {
                return BaseDef(_mm_shuffle2222_ps((&this->values0)[Index / 4]));
            } else /*Index % 4 == 3*/ {
                return BaseDef(_mm_shuffle3333_ps((&this->values0)[Index / 4]));
            }
        } else
#endif
        {
            return BaseDef((&this->values0)[Index]);
        }
    }

    /**
     * Get a SIMD2.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-7).
     * @returns SIMD2 containing the desired values.
     */
    template<uint32 Index>
    XS_FUNCTION SIMD2Def getValue2() const noexcept
    {
        static_assert(Index < 8, "Invalid Index: Index must be <8");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index == 0) {
                return SIMD2Def(_mm512_castps512_ps128(this->values));
            } else if constexpr (Index == 1) {
                return SIMD2Def(_mm_shuffle3232_ps(_mm512_castps512_ps128(this->values)));
            } else if constexpr (Index % 2 == 0) {
                return SIMD2Def(_mm512_extractf32x4_ps(this->values, Index / 2));
            } else {
                const __m128 val = _mm512_extractf32x4_ps(this->values, Index / 2);
                return SIMD2Def(_mm_shuffle3232_ps(val));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index % 4 == 0) {
                return SIMD2Def(_mm256_castps256_ps128((&this->values0)[Index / 4]));
            } else if constexpr (Index % 4 == 1) {
                return SIMD2Def(_mm_shuffle3232_ps(_mm256_castps256_ps128((&this->values0)[Index / 4])));
            } else if constexpr (Index % 4 == 2) {
                return SIMD2Def(_mm256_extractf128_ps((&this->values0)[Index / 4], 1));
            } else /*Index % 4 == 3*/ {
                const __m128 val = _mm256_extractf128_ps((&this->values0)[Index / 4], 1);
                return SIMD2Def(_mm_shuffle3232_ps(val));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 2 == 0) {
                return SIMD2Def((&this->values0)[Index / 2]);
            } else {
                return SIMD2Def(_mm_shuffle3232_ps((&this->values0)[Index / 2]));
            }
        } else
#endif
        {
            return SIMD2Def((&this->values0)[Index * 2], (&this->values0)[Index * 2 + 1]);
        }
    }

    /**
     * Get a SIMD4.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-3).
     * @returns SIMD4 containing the desired values.
     */
    template<uint32 Index>
    XS_FUNCTION SIMD4Def getValue4() const noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index == 0) {
                return SIMD4Def(_mm512_castps512_ps128(this->values));
            } else {
                return SIMD4Def(_mm512_extractf32x4_ps(this->values, Index));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index % 2 == 0) {
                return SIMD4Def(_mm256_castps256_ps128((&this->values0)[Index / 2]));
            } else /*Index % 2 == 1*/ {
                return SIMD4Def(_mm256_extractf128_ps((&this->values0)[Index / 2], 1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD4Def((&this->values0)[Index]);
        } else
#endif
        {
            return SIMD4Def((&this->values0)[Index * 4], (&this->values0)[Index * 4 + 1],
                (&this->values0)[Index * 4 + 2], (&this->values0)[Index * 4 + 3]);
        }
    }

    /**
     * Get a SIMD8.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-1).
     * @returns SIMD8 containing the desired values.
     */
    template<uint32 Index>
    XS_FUNCTION SIMD8Def getValue8() const noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index == 0) {
                return SIMD8Def(_mm512_castps512_ps256(this->values));
            } else {
                return SIMD8Def(_mm512_extractf32x8_ps(this->values, 1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD8Def((&this->values0)[Index]);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD8Def((&this->values0)[Index * 2], (&this->values0)[(Index * 2) + 1]);
        } else
#endif
        {
            return SIMD8Def((&this->values0)[Index * 8], (&this->values0)[Index * 8 + 1],
                (&this->values0)[Index * 8 + 2], (&this->values0)[Index * 8 + 3], (&this->values0)[Index * 8 + 4],
                (&this->values0)[Index * 8 + 5], (&this->values0)[Index * 8 + 6], (&this->values0)[Index * 8 + 7]);
        }
    }

    /**
     * Get 2 sets of SIMD4s.
     * @note Optimised for getting value from register.
     * @tparam Index0 The index of the first SIMD4 to retrieve (range is 0-3).
     * @tparam Index1 The index of the second SIMD4 to retrieve (range is 0-3).
     * @returns SIMD8 containing the desired values.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD8Def getValue4x2() const noexcept
    {
        static_assert(Index0 < 4 && Index1 < 4, "Invalid Index: Indexes must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == 0 && Index1 == 1) {
                return SIMD8Def(_mm512_castps512_ps256(this->values));
            } else if constexpr (Index0 == 2 && Index1 == 3) {
                return SIMD8Def(_mm512_extractf32x8_ps(this->values, 1));
            } else {
                return SIMD8Def(_mm512_castps512_ps256(
                    _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 2, Index1, Index0))));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 1) {
                return SIMD8Def(this->values0);
            } else if constexpr (Index0 == 2 && Index1 == 3) {
                return SIMD8Def(this->values1);
            } else {
                return SIMD8Def(_mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD8Def((&this->values0)[Index0], (&this->values0)[Index1]);
        } else
#endif
        {
            return SIMD8Def((&this->values0)[Index0 * 4], (&this->values0)[Index0 * 4 + 1],
                (&this->values0)[Index0 * 4 + 2], (&this->values0)[Index0 * 4 + 3], (&this->values0)[Index1 * 4],
                (&this->values0)[Index1 * 4 + 1], (&this->values0)[Index1 * 4 + 2], (&this->values0)[Index1 * 4 + 3]);
        }
    }

    /**
     * Get 3 sets of SIMD4s.
     * @note Optimised for getting value from register.
     * @tparam Index0 The index of the first SIMD4 to retrieve (range is 0-3).
     * @tparam Index1 The index of the second SIMD4 to retrieve (range is 0-3).
     * @tparam Index2 The index of the third SIMD4 to retrieve (range is 0-3).
     * @returns SIMD12 containing the desired values.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2>
    XS_FUNCTION SIMD12Def getValue4x3() const noexcept
    {
        static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4, "Invalid Index: Indexes must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
                return SIMD12Def(this->values);
            } else {
                return SIMD12Def(
                    _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, Index2, Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 1) {
                if constexpr (Index2 % 2 == 0) {
                    return SIMD12Def(this->values0, _mm256_castps256_ps128((&this->values0)[Index2 / 2]));
                } else /*Index2 % 2 == 1*/ {
                    return SIMD12Def(this->values0, _mm256_extractf128_ps((&this->values0)[Index2 / 2], 1));
                }
            } else if constexpr (Index0 == 2 && Index1 == 3) {
                if constexpr (Index2 % 2 == 0) {
                    return SIMD12Def(this->values1, _mm256_castps256_ps128((&this->values0)[Index2 / 2]));
                } else /*Index2 % 2 == 1*/ {
                    return SIMD12Def(this->values1, _mm256_extractf128_ps((&this->values0)[Index2 / 2], 1));
                }
            } else {
                if constexpr (Index2 % 2 == 0) {
                    return SIMD12Def(
                        _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(Index1, Index0)),
                        _mm256_castps256_ps128((&this->values0)[Index2 / 2]));
                } else /*Index2 % 2 == 1*/ {
                    return SIMD12Def(
                        _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(Index1, Index0)),
                        _mm256_extractf128_ps((&this->values0)[Index2 / 2], 1));
                }
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD12Def((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2]);
        } else
#endif
        {
            return SIMD12Def((&this->values0)[Index0 * 4], (&this->values0)[Index0 * 4 + 1],
                (&this->values0)[Index0 * 4 + 2], (&this->values0)[Index0 * 4 + 3], (&this->values0)[Index1 * 4],
                (&this->values0)[Index1 * 4 + 1], (&this->values0)[Index1 * 4 + 2], (&this->values0)[Index1 * 4 + 3],
                (&this->values0)[Index2 * 4], (&this->values0)[Index2 * 4 + 1], (&this->values0)[Index2 * 4 + 2],
                (&this->values0)[Index2 * 4 + 3]);
        }
    }

    /**
     * Get any 3 elements of each sub SIMD4.
     * @note Optimised for getting value from register.
     * @tparam Index0 The index of the first element to retrieve (range is 0-3).
     * @tparam Index1 The index of the second element to retrieve (range is 0-3).
     * @tparam Index2 The index of the third element to retrieve (range is 0-3).
     * @returns SIMD3x4 containing the desired values.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2>
    XS_FUNCTION SIMD3x4Def getValue3x4() const noexcept
    {
        static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4, "Invalid Index: Indexes must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
                return SIMD3x4Def(this->values);
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
                return SIMD3x4Def(_mm512_shuffle2200_ps(this->values));
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3) {
                return SIMD3x4Def(_mm512_shuffle3311_ps(this->values));
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3) {
                return SIMD3x4Def(_mm512_shuffle3322_ps(this->values));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
                return SIMD3x4Def(_mm512_shuffle1100_ps(this->values));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
                return SIMD3x4Def(_mm512_shuffle1010_ps(this->values));
            } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2) {
                return SIMD3x4Def(_mm512_shuffle3232_ps(this->values));
            } else {
                return SIMD3x4Def(_mm512_shuffle1_ps(this->values, _MM_SHUFFLE(3, Index2, Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
                return SIMD3x4Def(this->values0, this->values1);
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3) {
                return SIMD3x4Def(_mm256_shuffle3311_ps(this->values0), _mm256_shuffle3311_ps(this->values1));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
                return SIMD3x4Def(_mm256_shuffle2200_ps(this->values0), _mm256_shuffle2200_ps(this->values1));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
                return SIMD3x4Def(_mm256_shuffle1100_ps(this->values0), _mm256_shuffle1100_ps(this->values1));
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3) {
                return SIMD3x4Def(_mm256_shuffle3322_ps(this->values0), _mm256_shuffle3322_ps(this->values1));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
                return SIMD3x4Def(_mm256_shuffle1010_ps(this->values0), _mm256_shuffle1010_ps(this->values1));
            } else {
                return SIMD3x4Def(_mm256_shuffle1_ps(this->values0, _MM_SHUFFLE(3, Index2, Index1, Index0)),
                    _mm256_shuffle1_ps(this->values1, _MM_SHUFFLE(3, Index2, Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
                return SIMD3x4Def(this->values0, this->values1, this->values2, this->values3);
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
                return SIMD3x4Def(this->values0, this->values1, this->values2, this->values3);
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
                return SIMD3x4Def(_mm_shuffle1010_ps(this->values0), _mm_shuffle1010_ps(this->values1),
                    _mm_shuffle1010_ps(this->values2), _mm_shuffle1010_ps(this->values3));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
                return SIMD3x4Def(_mm_shuffle1100_ps(this->values0), _mm_shuffle1100_ps(this->values1),
                    _mm_shuffle1100_ps(this->values2), _mm_shuffle1100_ps(this->values3));
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3) {
                return SIMD3x4Def(_mm_shuffle3322_ps(this->values0), _mm_shuffle3322_ps(this->values1),
                    _mm_shuffle3322_ps(this->values2), _mm_shuffle3322_ps(this->values3));
            } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2) {
                return SIMD3x4Def(_mm_shuffle3232_ps(this->values0), _mm_shuffle3232_ps(this->values1),
                    _mm_shuffle3232_ps(this->values2), _mm_shuffle3232_ps(this->values3));
            } else {
                return SIMD3x4Def(_mm_shuffle1_ps(this->values0, _MM_SHUFFLE(3, Index2, Index1, Index0)),
                    _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(3, Index2, Index1, Index0)),
                    _mm_shuffle1_ps(this->values2, _MM_SHUFFLE(3, Index2, Index1, Index0)),
                    _mm_shuffle1_ps(this->values3, _MM_SHUFFLE(3, Index2, Index1, Index0)));
            }
        } else
#endif
        {
            return SIMD3x4Def((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2],
                (&this->values0)[Index0 + 4], (&this->values0)[Index1 + 4], (&this->values0)[Index2 + 4],
                (&this->values0)[Index0 + 8], (&this->values0)[Index1 + 8], (&this->values0)[Index2 + 8],
                (&this->values0)[Index0 + 12], (&this->values0)[Index1 + 12], (&this->values0)[Index2 + 12]);
        }
    }

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to retrieve (range is 0-15).
     * @param other The new value.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue(InBaseDef other) noexcept
    {
        static_assert(Index < 16, "Invalid Index: Index must be <16");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index == 0) {
                this->values =
                    _mm512_mask_mov_ps(this->values, _cvtu32_mask16(1), _mm512_castps128_ps512(other.values));
            } else if constexpr (Index < 4) {
                this->values = _mm512_mask_permute_ps(this->values, _cvtu32_mask16(1 << Index),
                    _mm512_castps128_ps512(other.values), _MM_SHUFFLE(0, 0, 0, 0));
            } else {
                this->values = _mm512_mask_broadcastss_ps(this->values, _cvtu32_mask16(1 << Index), other.values);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index % 8 == 0) {
                (&this->values0)[Index / 8] = _mm256_blend_ps((&this->values0)[Index / 8],
                    _mm256_castps128_ps256(other.values), _MM256_BLEND(0, 0, 0, 0, 0, 0, 0, 1));
            } else if constexpr (Index % 8 < 4 && hasISAFeature<ISAFeature::AVX512F>) {
                (&this->values0)[Index / 8] = _mm256_mask_permute_ps((&this->values0)[Index / 8],
                    _cvtu32_mask8(1 << (Index % 8)), _mm256_castps128_ps256(other.values), _MM_SHUFFLE(0, 0, 0, 0));
            } else if constexpr (Index % 8 == 1) {
                const __m128 value = _mm_shuffle1100_ps(other.values);
                (&this->values0)[Index / 8] = _mm256_blend_ps(
                    (&this->values0)[Index / 8], _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 0, 0, 1, 0));
            } else if constexpr (Index % 8 == 2) {
                const __m128 value = _mm_shuffle1010_ps(other.values); //(x,0,x,x)
                (&this->values0)[Index / 8] = _mm256_blend_ps(
                    (&this->values0)[Index / 8], _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 0, 1, 0, 0));
            } else if constexpr (Index % 8 == 3) {
                const __m128 value = _mm_shuffle1_ps(other.values, _MM_SHUFFLE(0, 2, 1, 3)); //(0,x,x,x);
                (&this->values0)[Index / 8] = _mm256_blend_ps(
                    (&this->values0)[Index / 8], _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 1, 0, 0, 0));
            } else if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                (&this->values0)[Index / 8] = _mm256_mask_broadcastss_ps(
                    (&this->values0)[Index / 8], _cvtu32_mask8(1 << (Index % 8)), other.values);
            } else if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                (&this->values0)[Index / 8] = _mm256_blend_ps(
                    (&this->values0)[Index / 8], _mm256_broadcastss_ps(other.values), (1 << (Index % 8)));
            } else if constexpr (Index % 8 == 4) {
                (&this->values0)[Index / 8] = _mm256_blend_ps((&this->values0)[Index / 8],
                    _mm256_broadcastf128_ps(other.values), _MM256_BLEND(0, 0, 0, 1, 0, 0, 0, 0));
            } else if constexpr (Index % 8 == 5) {
                const __m128 value = _mm_shuffle1100_ps(other.values);
                (&this->values0)[Index / 8] = _mm256_blend_ps(
                    (&this->values0)[Index / 8], _mm256_broadcastf128_ps(value), _MM256_BLEND(0, 0, 1, 0, 0, 0, 0, 0));
            } else if constexpr (Index % 8 == 6) {
                const __m128 value = _mm_shuffle1010_ps(other.values); //(x,0,x,x)
                (&this->values0)[Index / 8] = _mm256_blend_ps(
                    (&this->values0)[Index / 8], _mm256_broadcastf128_ps(value), _MM256_BLEND(0, 1, 0, 0, 0, 0, 0, 0));
            } else /*Index % 8 == 7*/ {
                const __m128 value = _mm_shuffle1_ps(other.values, _MM_SHUFFLE(0, 2, 1, 3)); //(0,x,x,x);
                (&this->values0)[Index / 8] = _mm256_blend_ps(
                    (&this->values0)[Index / 8], _mm256_broadcastf128_ps(value), _MM256_BLEND(1, 0, 0, 0, 0, 0, 0, 0));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 4 == 0) {
                (&this->values0)[Index / 4] = _mm_blend_ss((&this->values0)[Index / 4], other.values);
            } else {
                (&this->values0)[Index / 4] =
                    _mm_insert_ps((&this->values0)[Index / 4], other.values, _MM_MK_INSERTPS_NDX(0, Index % 4, 0));
            }
        } else
#endif
        {
            (&this->values0)[Index] = other.value;
        }
    }

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to retrieve (range is 0-15).
     * @param other The new value.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue(BaseDef other) noexcept
    {
        static_assert(Index < 16, "Invalid Index: Index must be <16");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_mask_mov_ps(this->values, _cvtu32_mask16(1 << Index), other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            (&this->values0)[Index / 8] =
                _mm256_blend_ps((&this->values0)[Index / 8], other.values, (1 << (Index % 8)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 4 == 0) {
                (&this->values0)[Index / 4] = _mm_blend_ss((&this->values0)[Index / 4], other.values);
            } else {
                (&this->values0)[Index / 4] =
                    _mm_blend_ps((&this->values0)[Index / 4], other.values, (1 << (Index % 4)));
            }
        } else
#endif
        {
            (&this->values0)[Index] = other.value;
        }
    }

    /**
     * Set a SIMD2.
     * @tparam Index The index of the element to retrieve (range is 0-7).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue2(const SIMD2Def& other) noexcept
    {
        static_assert(Index < 8, "Invalid Index: Index must be <8");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index == 0) {
                this->values =
                    _mm512_mask_mov_ps(this->values, _cvtu32_mask16(3), _mm512_castps128_ps512(other.values));
            } else if constexpr (Index == 1) {
                this->values = _mm512_mask_permute_ps(
                    this->values, _cvtu32_mask16(0xC), _mm512_castps128_ps512(other.values), _MM_SHUFFLE(1, 0, 1, 0));
            } else {
                this->values =
                    _mm512_mask_broadcast_f32x2(this->values, _cvtu32_mask16(3 << (Index * 2)), other.values);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index % 4 == 0) {
                (&this->values0)[Index / 4] = _mm256_blend_ps((&this->values0)[Index / 4],
                    _mm256_castps128_ps256(other.values), _MM256_BLEND(0, 0, 0, 0, 0, 0, 1, 1));
            } else if constexpr (Index % 4 == 1 && hasISAFeature<ISAFeature::AVX512F>) {
                (&this->values0)[Index / 4] =
                    _mm256_mask_permute_ps((&this->values0)[Index / 4], _cvtu32_mask8(3 << ((Index % 4) * 2)),
                        _mm256_castps128_ps256(other.values), _MM_SHUFFLE(1, 0, 1, 0));
            } else if constexpr (Index % 4 == 1) {
                const __m128 value = _mm_shuffle1010_ps(other.values); //(1,0,x,x)
                (&this->values0)[Index / 4] = _mm256_blend_ps(
                    (&this->values0)[Index / 4], _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 1, 1, 0, 0));
            } else if constexpr ((Index % 4) < 3 && hasISAFeature<ISAFeature::AVX512F>) {
                (&this->values0)[Index / 4] = _mm256_mask_broadcast_f32x2(
                    (&this->values0)[Index / 4], _cvtu32_mask8(3 << ((Index % 4) * 2)), other.values);
            } else if constexpr (Index % 4 == 2) {
                (&this->values0)[Index / 4] = _mm256_blend_ps((&this->values0)[Index / 4],
                    _mm256_broadcastf128_ps(other.values), _MM256_BLEND(0, 0, 1, 1, 0, 0, 0, 0));
            } else /*Index % 4 == 3*/ {
                const __m128 value = _mm_shuffle1010_ps(other.values); //(1,0,x,x)
                (&this->values0)[Index / 4] = _mm256_blend_ps(
                    (&this->values0)[Index / 4], _mm256_broadcastf128_ps(value), _MM256_BLEND(1, 1, 0, 0, 0, 0, 0, 0));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 2 == 0) {
                (&this->values0)[Index / 2] =
                    _mm_shuffle_ps(other.values, (&this->values0)[Index / 2], _MM_SHUFFLE(3, 2, 1, 0));
            } else {
                (&this->values0)[Index / 2] = _mm_movelh_ps((&this->values0)[Index / 2], other.values);
            }
        } else
#endif
        {
            (&this->values0)[Index * 2] = other.values0;
            (&this->values0)[Index * 2 + 1] = other.values1;
        }
    }

    /**
     * Set a SIMD4.
     * @tparam Index The index of the element to retrieve (range is 0-3).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue4(const SIMD4Def& other) noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index == 0) {
                this->values =
                    _mm512_mask_mov_ps(this->values, _cvtu32_mask16(0xF), _mm512_castps128_ps512(other.values));
            } else {
                this->values = _mm512_insertf32x4(this->values, other.values, Index);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index % 2 == 0) {
                (&this->values0)[Index / 2] = _mm256_blend_ps((&this->values0)[Index / 2],
                    _mm256_castps128_ps256(other.values), _MM256_BLEND(0, 0, 0, 0, 1, 1, 1, 1));
            } else /*Index % 2 == 1*/ {
                (&this->values0)[Index / 2] = _mm256_insertf128_ps((&this->values0)[Index / 2], other.values, 1);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            (&this->values0)[Index] = other.values;
        } else
#endif
        {
            (&this->values0)[Index * 4] = other.values0;
            (&this->values0)[Index * 4 + 1] = other.values1;
            (&this->values0)[Index * 4 + 2] = other.values2;
            (&this->values0)[Index * 4 + 3] = other.values3;
        }
    }

    /**
     * Set a SIMD8.
     * @tparam Index The index of the element to retrieve (range is 0-1).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue8(const SIMD8Def& other) noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index == 0) {
                this->values =
                    _mm512_mask_mov_ps(this->values, _cvtu32_mask16(0xFF), _mm512_castps256_ps512(other.values));
            } else {
                this->values = _mm512_insertf32x8(this->values, other.values, 1);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            (&this->values0)[Index] = other.values;
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            (&this->values0)[(Index * 2)] = other.values0;
            (&this->values0)[(Index * 2) + 1] = other.values1;
        } else
#endif
        {
            (&this->values0)[Index * 8] = other.values0;
            (&this->values0)[Index * 8 + 1] = other.values1;
            (&this->values0)[Index * 8 + 2] = other.values2;
            (&this->values0)[Index * 8 + 3] = other.values3;
            (&this->values0)[Index * 8 + 4] = other.values4;
            (&this->values0)[Index * 8 + 5] = other.values5;
            (&this->values0)[Index * 8 + 6] = other.values6;
            (&this->values0)[Index * 8 + 7] = other.values7;
        }
    }

    /**
     * Set 2 sets of SIMD4s.
     * @tparam Index0 The index of the first element to set (range is 0-3).
     * @tparam Index1 The index of the second element to set (range is 0-3)(Index1 != Index0).
     * @param other The new values.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION void setValue4x2(const SIMD8Def& other) noexcept
    {
        static_assert(Index0 < 4 && Index1 < 4, "Invalid Index: Indexes must be <4");
        static_assert(Index0 != Index1, "Invalid Index: Indexes must both be unique");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == 0 && Index1 == 1) {
                this->values =
                    _mm512_mask_mov_ps(this->values, _cvtu32_mask16(0xFF), _mm512_castps256_ps512(other.values));
            } else {
                this->values = _mm512_mask_shuffle_f32x4(this->values,
                    _cvtu32_mask16((0xF << (Index0 * 4)) | (0xF << (Index1 * 4))), _mm512_castps256_ps512(other.values),
                    _mm512_castps256_ps512(other.values),
                    _MM_SHUFFLE(Index0 == 3 ? 0 : 1, Index0 == 2 ? 0 : 1, Index0 == 1 ? 0 : 1, Index0 == 0 ? 0 : 1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 1) {
                this->values0 = other.values;
            } else if constexpr (Index0 == 0 && Index1 == 2) {
                this->values0 = _mm256_blend_ps(this->values0, other.values, _MM256_BLEND(0, 0, 0, 0, 1, 1, 1, 1));
                this->values1 = _mm256_permute2f128_ps(this->values1, other.values, _MM256_PERMUTE(1, 3));
            } else if constexpr (Index0 == 0 && Index1 == 3) {
                this->values0 = _mm256_blend_ps(this->values0, other.values, _MM256_BLEND(0, 0, 0, 0, 1, 1, 1, 1));
                this->values1 = _mm256_blend_ps(this->values1, other.values, _MM256_BLEND(1, 1, 1, 1, 0, 0, 0, 0));
            } else if constexpr (Index0 == 1 && Index1 == 0) {
                this->values0 = _mm256_shuffle32107654_ps(other.values);
            } else if constexpr (Index0 == 1 && Index1 == 2) {
                this->values0 = _mm256_insertf128_ps(this->values0, _mm256_castps256_ps128(other.values), 1);
                this->values1 = _mm256_permute2f128_ps(this->values1, other.values, _MM256_PERMUTE(1, 3));
            } else if constexpr (Index0 == 1 && Index1 == 3) {
                this->values0 = _mm256_insertf128_ps(this->values0, _mm256_castps256_ps128(other.values), 1);
                this->values1 = _mm256_blend_ps(this->values1, other.values, _MM256_BLEND(1, 1, 1, 1, 0, 0, 0, 0));
            } else if constexpr (Index0 == 2 && Index1 == 0) {
                this->values0 = _mm256_permute2f128_ps(this->values0, other.values, _MM256_PERMUTE(1, 3));
                this->values1 = _mm256_blend_ps(this->values1, other.values, _MM256_BLEND(0, 0, 0, 0, 1, 1, 1, 1));
            } else if constexpr (Index0 == 2 && Index1 == 1) {
                this->values0 = _mm256_blend_ps(this->values0, other.values, _MM256_BLEND(1, 1, 1, 1, 0, 0, 0, 0));
                this->values1 = _mm256_blend_ps(this->values1, other.values, _MM256_BLEND(0, 0, 0, 0, 1, 1, 1, 1));
            } else if constexpr (Index0 == 2 && Index1 == 3) {
                this->values1 = other.values;
            } else if constexpr (Index0 == 3 && Index1 == 0) {
                this->values0 = _mm256_permute2f128_ps(this->values0, other.values, _MM256_PERMUTE(1, 3));
                this->values1 = _mm256_insertf128_ps(this->values1, _mm256_castps256_ps128(other.values), 1);
            } else if constexpr (Index0 == 3 && Index1 == 1) {
                this->values0 = _mm256_blend_ps(this->values0, other.values, _MM256_BLEND(1, 1, 1, 1, 0, 0, 0, 0));
                this->values1 = _mm256_insertf128_ps(this->values1, _mm256_castps256_ps128(other.values), 1);
            } else /*Index0 == 3 && Index1 == 2*/ {
                this->values1 = _mm256_shuffle32107654_ps(other.values);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            (&this->values0)[Index0] = other.values0;
            (&this->values0)[Index1] = other.values1;
        } else
#endif
        {
            (&this->values0)[Index0 * 4] = other.values0;
            (&this->values0)[Index0 * 4 + 1] = other.values1;
            (&this->values0)[Index0 * 4 + 2] = other.values2;
            (&this->values0)[Index0 * 4 + 3] = other.values3;
            (&this->values0)[Index1 * 4] = other.values4;
            (&this->values0)[Index1 * 4 + 1] = other.values5;
            (&this->values0)[Index1 * 4 + 2] = other.values6;
            (&this->values0)[Index1 * 4 + 3] = other.values7;
        }
    }

    /**
     * Negate an element of the object.
     * @tparam Elem0  Boolean indicating if first element should be negated.
     * @tparam Elem1  Boolean indicating if second element should be negated.
     * @tparam Elem2  Boolean indicating if third element should be negated.
     * @tparam Elem3  Boolean indicating if fourth element should be negated.
     * @tparam Elem4  Boolean indicating if fifth element should be negated.
     * @tparam Elem5  Boolean indicating if sixth element should be negated.
     * @tparam Elem6  Boolean indicating if seventh element should be negated.
     * @tparam Elem7  Boolean indicating if eighth element should be negated.
     * @tparam Elem8  Boolean indicating if ninth element should be negated.
     * @tparam Elem9  Boolean indicating if tenth element should be negated.
     * @tparam Elem10 Boolean indicating if eleventh element should be negated.
     * @tparam Elem11 Boolean indicating if twelfth element should be negated.
     * @tparam Elem12 Boolean indicating if thirteenth element should be negated.
     * @tparam Elem13 Boolean indicating if fourteenth element should be negated.
     * @tparam Elem14 Boolean indicating if fifteenth element should be negated.
     * @tparam Elem15 Boolean indicating if sixteenth element should be negated.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7, bool Elem8,
        bool Elem9, bool Elem10, bool Elem11, bool Elem12, bool Elem13, bool Elem14, bool Elem15>
    XS_FUNCTION SIMD16 negate() const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && !Elem8 &&
            !Elem9 && !Elem10 && !Elem11 && !Elem12 && !Elem13 && !Elem14 && !Elem15) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 && Elem9 &&
                Elem10 && Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
                return SIMD16(_mm512_xor_ps(this->values, _mm512_set1_ps(-0.0f)));
            } else {
                return SIMD16(_mm512_xor_ps(this->values,
                    _mm512_set_ps(Elem15 ? -0.0f : 0.0f, Elem14 ? -0.0f : 0.0f, Elem13 ? -0.0f : 0.0f,
                        Elem12 ? -0.0f : 0.0f, Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f, Elem9 ? -0.0f : 0.0f,
                        Elem8 ? -0.0f : 0.0f, Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f,
                        Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f,
                        Elem0 ? -0.0f : 0.0f)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 && Elem9 &&
                Elem10 && Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
                const __m256 val = _mm256_set1_ps(-0.0f);
                return SIMD16(_mm256_xor_ps(this->values0, val), _mm256_xor_ps(this->values1, val));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && !Elem8 &&
                !Elem9 && !Elem10 && !Elem11 && !Elem12 && !Elem13 && !Elem14 && !Elem15) {
                return SIMD16(_mm256_xor_ps(this->values0, _mm256_set1_ps(-0.0f)), this->values1);
            } else if constexpr (Elem8 && Elem9 && Elem10 && Elem11 && Elem12 && Elem13 && Elem14 && Elem15 && !Elem0 &&
                !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7) {
                return SIMD16(this->values0, _mm256_xor_ps(this->values1, _mm256_set1_ps(-0.0f)));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7) {
                return SIMD16(_mm256_xor_ps(this->values0, _mm256_set1_ps(-0.0f)),
                    _mm256_xor_ps(this->values1,
                        _mm256_set_ps(Elem15 ? -0.0f : 0.0f, Elem14 ? -0.0f : 0.0f, Elem13 ? -0.0f : 0.0f,
                            Elem12 ? -0.0f : 0.0f, Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f, Elem9 ? -0.0f : 0.0f,
                            Elem8 ? -0.0f : 0.0f)));
            } else if constexpr (Elem8 && Elem9 && Elem10 && Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
                return SIMD16(_mm256_xor_ps(this->values0,
                                  _mm256_set_ps(Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f,
                                      Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f,
                                      Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                    _mm256_xor_ps(this->values1, _mm256_set1_ps(-0.0f)));
            } else if constexpr (Elem0 == Elem8 && Elem1 == Elem9 && Elem2 == Elem10 && Elem3 == Elem11 &&
                Elem4 == Elem12 && Elem5 == Elem13 && Elem6 == Elem14 && Elem7 == Elem15) {
                const __m256 val = _mm256_set_ps(Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f,
                    Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f,
                    Elem0 ? -0.0f : 0.0f);
                return SIMD16(_mm256_xor_ps(this->values0, val), _mm256_xor_ps(this->values1, val));
            } else {
                return SIMD16(_mm256_xor_ps(this->values0,
                                  _mm256_set_ps(Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f,
                                      Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f,
                                      Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                    _mm256_xor_ps(this->values1,
                        _mm256_set_ps(Elem15 ? -0.0f : 0.0f, Elem14 ? -0.0f : 0.0f, Elem13 ? -0.0f : 0.0f,
                            Elem12 ? -0.0f : 0.0f, Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f, Elem9 ? -0.0f : 0.0f,
                            Elem8 ? -0.0f : 0.0f)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && !Elem5 && !Elem6 && !Elem7 && Elem8 &&
                !Elem9 && !Elem10 && !Elem11 && Elem12 && !Elem13 && !Elem14 && !Elem15) {
                const __m128 value = _mm_set_ss(-0.0f);
                return SIMD16(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value),
                    _mm_xor_ps(this->values2, value), _mm_xor_ps(this->values3, value));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 &&
                Elem9 && Elem10 && Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD16(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value),
                    _mm_xor_ps(this->values2, value), _mm_xor_ps(this->values3, value));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && !Elem8 &&
                !Elem9 && !Elem10 && !Elem11 && !Elem12 && !Elem13 && !Elem14 && !Elem15) {
                return SIMD16(
                    _mm_xor_ps(this->values0, _mm_set1_ps(-0.0f)), this->values1, this->values2, this->values3);
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && !Elem8 &&
                !Elem9 && !Elem10 && !Elem11 && !Elem12 && !Elem13 && !Elem14 && !Elem15) {
                return SIMD16(
                    this->values0, _mm_xor_ps(this->values1, _mm_set1_ps(-0.0f)), this->values2, this->values3);
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 &&
                Elem8 && Elem9 && Elem10 && Elem11 && !Elem12 && !Elem13 && !Elem14 && !Elem15) {
                return SIMD16(
                    this->values0, this->values1, _mm_xor_ps(this->values2, _mm_set1_ps(-0.0f)), this->values3);
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 &&
                !Elem8 && !Elem9 && !Elem10 && !Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
                return SIMD16(
                    this->values0, this->values1, this->values2, _mm_xor_ps(this->values3, _mm_set1_ps(-0.0f)));
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 &&
                Elem8 && Elem9 && Elem10 && Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD16(
                    this->values0, this->values1, _mm_xor_ps(this->values2, value), _mm_xor_ps(this->values3, value));
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && !Elem8 &&
                !Elem9 && !Elem10 && !Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD16(
                    this->values0, _mm_xor_ps(this->values1, value), this->values2, _mm_xor_ps(this->values3, value));
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 &&
                Elem9 && Elem10 && Elem11 && !Elem12 && !Elem13 && !Elem14 && !Elem15) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD16(
                    this->values0, _mm_xor_ps(this->values1, value), _mm_xor_ps(this->values2, value), this->values3);
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && !Elem8 &&
                !Elem9 && !Elem10 && !Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD16(
                    _mm_xor_ps(this->values0, value), this->values1, this->values2, _mm_xor_ps(this->values3, value));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && Elem8 &&
                Elem9 && Elem10 && Elem11 && !Elem12 && !Elem13 && !Elem14 && !Elem15) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD16(
                    _mm_xor_ps(this->values0, value), this->values1, _mm_xor_ps(this->values2, value), this->values3);
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && !Elem8 &&
                !Elem9 && !Elem10 && !Elem11 && !Elem12 && !Elem13 && !Elem14 && !Elem15) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD16(
                    _mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value), this->values2, this->values3);
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 &&
                Elem9 && Elem10 && Elem11 && !Elem12 && !Elem13 && !Elem14 && !Elem15) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD16(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value),
                    _mm_xor_ps(this->values2, value), this->values3);
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && !Elem8 &&
                !Elem9 && !Elem10 && !Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD16(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value), this->values2,
                    _mm_xor_ps(this->values3, value));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && Elem8 &&
                Elem9 && Elem10 && Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD16(_mm_xor_ps(this->values0, value), this->values1, _mm_xor_ps(this->values2, value),
                    _mm_xor_ps(this->values3, value));
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 &&
                Elem9 && Elem10 && Elem11 && Elem12 && Elem13 && Elem14 && Elem15) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD16(this->values0, _mm_xor_ps(this->values1, value), _mm_xor_ps(this->values2, value),
                    _mm_xor_ps(this->values3, value));
            } else if constexpr (Elem0 == Elem4 && Elem0 == Elem8 && Elem0 == Elem12 && Elem1 == Elem5 &&
                Elem1 == Elem9 && Elem1 == Elem13 && Elem2 == Elem6 && Elem2 == Elem10 && Elem2 == Elem14 &&
                Elem3 == Elem7 && Elem3 == Elem11 && Elem3 == Elem15) {
                const __m128 value = _mm_set_ps(0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f);
                return SIMD16(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value),
                    _mm_xor_ps(this->values2, value));
            } else {
                return SIMD16(_mm_xor_ps(this->values0,
                                  _mm_set_ps(Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f,
                                      Elem0 ? -0.0f : 0.0f)),
                    _mm_xor_ps(this->values1,
                        _mm_set_ps(
                            Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f)),
                    _mm_xor_ps(this->values2,
                        _mm_set_ps(
                            Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f, Elem9 ? -0.0f : 0.0f, Elem8 ? -0.0f : 0.0f)),
                    _mm_xor_ps(this->values3,
                        _mm_set_ps(Elem15 ? -0.0f : 0.0f, Elem14 ? -0.0f : 0.0f, Elem13 ? -0.0f : 0.0f,
                            Elem12 ? -0.0f : 0.0f)));
            }
        }
#endif
        else {
            return SIMD16(Elem0 ? -this->values0 : this->values0, Elem1 ? -this->values1 : this->values1,
                Elem2 ? -this->values2 : this->values2, Elem3 ? -this->values3 : this->values3,
                Elem4 ? -this->values4 : this->values4, Elem5 ? -this->values5 : this->values5,
                Elem6 ? -this->values6 : this->values6, Elem7 ? -this->values7 : this->values7,
                Elem8 ? -this->values8 : this->values8, Elem9 ? -this->values9 : this->values9,
                Elem10 ? -this->values10 : this->values10, Elem11 ? -this->values11 : this->values11,
                Elem12 ? -this->values12 : this->values12, Elem13 ? -this->values13 : this->values13,
                Elem14 ? -this->values14 : this->values14, Elem15 ? -this->values15 : this->values15);
        }
    }

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD16 mad(const SIMD16& other1, const SIMD16& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm512_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD16(_mm512_add_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm256_fmadd_ps(this->values0, other1.values0, other2.values0),
                    _mm256_fmadd_ps(this->values1, other1.values1, other2.values1));
            } else {
                return SIMD16(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values0), other2.values0),
                    _mm256_add_ps(_mm256_mul_ps(this->values1, other1.values1), other2.values1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm_fmadd_ps(this->values0, other1.values0, other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values1, other2.values1),
                    _mm_fmadd_ps(this->values2, other1.values2, other2.values2),
                    _mm_fmadd_ps(this->values3, other1.values3, other2.values3));
            } else {
                return SIMD16(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1),
                    _mm_add_ps(_mm_mul_ps(this->values2, other1.values2), other2.values2),
                    _mm_add_ps(_mm_mul_ps(this->values3, other1.values3), other2.values3));
            }
        } else
#endif
        {
            return SIMD16(Shift::fma<T>(this->values0, other1.values0, other2.values0),
                Shift::fma<T>(this->values1, other1.values1, other2.values1),
                Shift::fma<T>(this->values2, other1.values2, other2.values2),
                Shift::fma<T>(this->values3, other1.values3, other2.values3),
                Shift::fma<T>(this->values4, other1.values4, other2.values4),
                Shift::fma<T>(this->values5, other1.values5, other2.values5),
                Shift::fma<T>(this->values6, other1.values6, other2.values6),
                Shift::fma<T>(this->values7, other1.values7, other2.values7),
                Shift::fma<T>(this->values8, other1.values8, other2.values8),
                Shift::fma<T>(this->values9, other1.values9, other2.values9),
                Shift::fma<T>(this->values10, other1.values10, other2.values10),
                Shift::fma<T>(this->values11, other1.values11, other2.values11),
                Shift::fma<T>(this->values12, other1.values12, other2.values12),
                Shift::fma<T>(this->values13, other1.values13, other2.values13),
                Shift::fma<T>(this->values14, other1.values14, other2.values14),
                Shift::fma<T>(this->values15, other1.values15, other2.values15));
        }
    }

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD16 mad(BaseDef other1, const SIMD16& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm512_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD16(_mm512_add_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm256_fmadd_ps(this->values0, other1.values, other2.values0),
                    _mm256_fmadd_ps(this->values1, other1.values, other2.values1));
            } else {
                return SIMD16(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values), other2.values0),
                    _mm256_add_ps(_mm256_mul_ps(this->values1, other1.values), other2.values1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values, other2.values1),
                    _mm_fmadd_ps(this->values2, other1.values, other2.values2),
                    _mm_fmadd_ps(this->values3, other1.values, other2.values3));
            } else {
                return SIMD16(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1),
                    _mm_add_ps(_mm_mul_ps(this->values2, other1.values), other2.values2),
                    _mm_add_ps(_mm_mul_ps(this->values3, other1.values), other2.values3));
            }
        } else
#endif
        {
            return SIMD16(Shift::fma<T>(this->values0, other1.value, other2.values0),
                Shift::fma<T>(this->values1, other1.value, other2.values1),
                Shift::fma<T>(this->values2, other1.value, other2.values2),
                Shift::fma<T>(this->values3, other1.value, other2.values3),
                Shift::fma<T>(this->values4, other1.value, other2.values4),
                Shift::fma<T>(this->values5, other1.value, other2.values5),
                Shift::fma<T>(this->values6, other1.value, other2.values6),
                Shift::fma<T>(this->values7, other1.value, other2.values7),
                Shift::fma<T>(this->values8, other1.value, other2.values8),
                Shift::fma<T>(this->values9, other1.value, other2.values9),
                Shift::fma<T>(this->values10, other1.value, other2.values10),
                Shift::fma<T>(this->values11, other1.value, other2.values11),
                Shift::fma<T>(this->values12, other1.value, other2.values12),
                Shift::fma<T>(this->values13, other1.value, other2.values13),
                Shift::fma<T>(this->values14, other1.value, other2.values14),
                Shift::fma<T>(this->values15, other1.value, other2.values15));
        }
    }

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD16 mad(const SIMD16& other1, BaseDef other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm512_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD16(_mm512_add_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm256_fmadd_ps(this->values0, other1.values0, other2.values),
                    _mm256_fmadd_ps(this->values1, other1.values1, other2.values));
            } else {
                return SIMD16(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values0), other2.values),
                    _mm256_add_ps(_mm256_mul_ps(this->values1, other1.values1), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm_fmadd_ps(this->values0, other1.values0, other2.values),
                    _mm_fmadd_ps(this->values1, other1.values1, other2.values),
                    _mm_fmadd_ps(this->values2, other1.values2, other2.values),
                    _mm_fmadd_ps(this->values3, other1.values3, other2.values));
            } else {
                return SIMD16(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values),
                    _mm_add_ps(_mm_mul_ps(this->values2, other1.values2), other2.values),
                    _mm_add_ps(_mm_mul_ps(this->values3, other1.values3), other2.values));
            }
        } else
#endif
        {
            return SIMD16(Shift::fma<T>(this->values0, other1.values0, other2.value),
                Shift::fma<T>(this->values1, other1.values1, other2.value),
                Shift::fma<T>(this->values2, other1.values2, other2.value),
                Shift::fma<T>(this->values3, other1.values3, other2.value),
                Shift::fma<T>(this->values4, other1.values4, other2.value),
                Shift::fma<T>(this->values5, other1.values5, other2.value),
                Shift::fma<T>(this->values6, other1.values6, other2.value),
                Shift::fma<T>(this->values7, other1.values7, other2.value),
                Shift::fma<T>(this->values8, other1.values8, other2.value),
                Shift::fma<T>(this->values9, other1.values9, other2.value),
                Shift::fma<T>(this->values10, other1.values10, other2.value),
                Shift::fma<T>(this->values11, other1.values11, other2.value),
                Shift::fma<T>(this->values12, other1.values12, other2.value),
                Shift::fma<T>(this->values13, other1.values13, other2.value),
                Shift::fma<T>(this->values14, other1.values14, other2.value),
                Shift::fma<T>(this->values15, other1.values15, other2.value));
        }
    }

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD16 mad(const SIMD4Def& other1, const SIMD4Def& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm512_fmadd_ps(
                    this->values, _mm512_broadcastf128_ps(other1.values), _mm512_broadcastf128_ps(other2.values)));
            } else {
                return SIMD16(_mm512_add_ps(_mm512_mul_ps(this->values, _mm512_broadcastf128_ps(other1.values)),
                    _mm512_broadcastf128_ps(other2.values)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val0 = _mm256_broadcastf128_ps(other1.values);
            const __m256 val1 = _mm256_broadcastf128_ps(other2.values);
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm256_fmadd_ps(this->values0, val0, val1), _mm256_fmadd_ps(this->values1, val0, val1));
            } else {
                return SIMD16(_mm256_add_ps(_mm256_mul_ps(this->values0, val0), val1),
                    _mm256_add_ps(_mm256_mul_ps(this->values1, val0), val1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm_fmadd_ps(this->values0, other1.values, other2.values),
                    _mm_fmadd_ps(this->values1, other1.values, other2.values),
                    _mm_fmadd_ps(this->values2, other1.values, other2.values),
                    _mm_fmadd_ps(this->values3, other1.values, other2.values));
            } else {
                return SIMD16(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values),
                    _mm_add_ps(_mm_mul_ps(this->values2, other1.values), other2.values),
                    _mm_add_ps(_mm_mul_ps(this->values3, other1.values), other2.values));
            }
        } else
#endif
        {
            return SIMD16(Shift::fma<T>(this->values0, other1.values0, other2.values0),
                Shift::fma<T>(this->values1, other1.values1, other2.values1),
                Shift::fma<T>(this->values2, other1.values2, other2.values2),
                Shift::fma<T>(this->values3, other1.values3, other2.values3),
                Shift::fma<T>(this->values4, other1.values0, other2.values0),
                Shift::fma<T>(this->values5, other1.values1, other2.values1),
                Shift::fma<T>(this->values6, other1.values2, other2.values2),
                Shift::fma<T>(this->values7, other1.values3, other2.values3),
                Shift::fma<T>(this->values8, other1.values0, other2.values0),
                Shift::fma<T>(this->values9, other1.values1, other2.values1),
                Shift::fma<T>(this->values10, other1.values2, other2.values2),
                Shift::fma<T>(this->values11, other1.values3, other2.values3),
                Shift::fma<T>(this->values12, other1.values0, other2.values0),
                Shift::fma<T>(this->values13, other1.values1, other2.values1),
                Shift::fma<T>(this->values14, other1.values2, other2.values2),
                Shift::fma<T>(this->values15, other1.values3, other2.values3));
        }
    }

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD16 mad(const SIMD4Def& other1, const SIMD16& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm512_fmadd_ps(this->values, _mm512_broadcastf128_ps(other1.values), other2.values));
            } else {
                return SIMD16(
                    _mm512_add_ps(_mm512_mul_ps(this->values, _mm512_broadcastf128_ps(other1.values)), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val = _mm256_broadcastf128_ps(other1.values);
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm256_fmadd_ps(this->values0, val, other2.values0),
                    _mm256_fmadd_ps(this->values1, val, other2.values1));
            } else {
                return SIMD16(_mm256_add_ps(_mm256_mul_ps(this->values0, val), other2.values0),
                    _mm256_add_ps(_mm256_mul_ps(this->values1, val), other2.values1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values, other2.values1),
                    _mm_fmadd_ps(this->values2, other1.values, other2.values2),
                    _mm_fmadd_ps(this->values3, other1.values, other2.values3));
            } else {
                return SIMD16(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1),
                    _mm_add_ps(_mm_mul_ps(this->values2, other1.values), other2.values2),
                    _mm_add_ps(_mm_mul_ps(this->values3, other1.values), other2.values3));
            }
        } else
#endif
        {
            return SIMD16(Shift::fma<T>(this->values0, other1.values0, other2.values0),
                Shift::fma<T>(this->values1, other1.values1, other2.values1),
                Shift::fma<T>(this->values2, other1.values2, other2.values2),
                Shift::fma<T>(this->values3, other1.values3, other2.values3),
                Shift::fma<T>(this->values4, other1.values0, other2.values4),
                Shift::fma<T>(this->values5, other1.values1, other2.values5),
                Shift::fma<T>(this->values6, other1.values2, other2.values6),
                Shift::fma<T>(this->values7, other1.values3, other2.values7),
                Shift::fma<T>(this->values8, other1.values0, other2.values8),
                Shift::fma<T>(this->values9, other1.values1, other2.values9),
                Shift::fma<T>(this->values10, other1.values2, other2.values10),
                Shift::fma<T>(this->values11, other1.values3, other2.values11),
                Shift::fma<T>(this->values12, other1.values0, other2.values12),
                Shift::fma<T>(this->values13, other1.values1, other2.values13),
                Shift::fma<T>(this->values14, other1.values2, other2.values14),
                Shift::fma<T>(this->values15, other1.values3, other2.values15));
        }
    }

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD16 mad(const SIMD8Def& other1, const SIMD8Def& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm512_fmadd_ps(
                    this->values, _mm512_broadcastf256_ps(other1.values), _mm512_broadcastf256_ps(other2.values)));
            } else {
                return SIMD16(_mm512_add_ps(_mm512_mul_ps(this->values, _mm512_broadcastf256_ps(other1.values)),
                    _mm512_broadcastf256_ps(other2.values)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm256_fmadd_ps(this->values0, other1.values, other2.values),
                    _mm256_fmadd_ps(this->values1, other1.values, other2.values));
            } else {
                return SIMD16(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values), other2.values),
                    _mm256_add_ps(_mm256_mul_ps(this->values1, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm_fmadd_ps(this->values0, other1.values0, other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values1, other2.values1),
                    _mm_fmadd_ps(this->values2, other1.values0, other2.values0),
                    _mm_fmadd_ps(this->values3, other1.values1, other2.values1));
            } else {
                return SIMD16(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1),
                    _mm_add_ps(_mm_mul_ps(this->values2, other1.values0), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values3, other1.values1), other2.values1));
            }
        } else
#endif
        {
            return SIMD16(Shift::fma<T>(this->values0, other1.values0, other2.values0),
                Shift::fma<T>(this->values1, other1.values1, other2.values1),
                Shift::fma<T>(this->values2, other1.values2, other2.values2),
                Shift::fma<T>(this->values3, other1.values3, other2.values3),
                Shift::fma<T>(this->values4, other1.values4, other2.values4),
                Shift::fma<T>(this->values5, other1.values5, other2.values5),
                Shift::fma<T>(this->values6, other1.values6, other2.values6),
                Shift::fma<T>(this->values7, other1.values7, other2.values7),
                Shift::fma<T>(this->values8, other1.values0, other2.values0),
                Shift::fma<T>(this->values9, other1.values1, other2.values1),
                Shift::fma<T>(this->values10, other1.values2, other2.values2),
                Shift::fma<T>(this->values11, other1.values3, other2.values3),
                Shift::fma<T>(this->values12, other1.values4, other2.values4),
                Shift::fma<T>(this->values13, other1.values5, other2.values5),
                Shift::fma<T>(this->values14, other1.values6, other2.values6),
                Shift::fma<T>(this->values15, other1.values7, other2.values7));
        }
    }

    /**
     * Multiply this object by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD16 mad(const SIMD8Def& other1, const SIMD16& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm512_fmadd_ps(this->values, _mm512_broadcastf256_ps(other1.values), other2.values));
            } else {
                return SIMD16(
                    _mm512_add_ps(_mm512_mul_ps(this->values, _mm512_broadcastf256_ps(other1.values)), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm256_fmadd_ps(this->values0, other1.values, other2.values0),
                    _mm256_fmadd_ps(this->values1, other1.values, other2.values1));
            } else {
                return SIMD16(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values), other2.values0),
                    _mm256_add_ps(_mm256_mul_ps(this->values1, other1.values), other2.values1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm_fmadd_ps(this->values0, other1.values0, other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values1, other2.values1),
                    _mm_fmadd_ps(this->values2, other1.values0, other2.values2),
                    _mm_fmadd_ps(this->values3, other1.values1, other2.values3));
            } else {
                return SIMD16(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1),
                    _mm_add_ps(_mm_mul_ps(this->values2, other1.values0), other2.values2),
                    _mm_add_ps(_mm_mul_ps(this->values3, other1.values1), other2.values3));
            }
        } else
#endif
        {
            return SIMD16(Shift::fma<T>(this->values0, other1.values0, other2.values0),
                Shift::fma<T>(this->values1, other1.values1, other2.values1),
                Shift::fma<T>(this->values2, other1.values2, other2.values2),
                Shift::fma<T>(this->values3, other1.values3, other2.values3),
                Shift::fma<T>(this->values4, other1.values4, other2.values4),
                Shift::fma<T>(this->values5, other1.values5, other2.values5),
                Shift::fma<T>(this->values6, other1.values6, other2.values6),
                Shift::fma<T>(this->values7, other1.values7, other2.values7),
                Shift::fma<T>(this->values8, other1.values0, other2.values8),
                Shift::fma<T>(this->values9, other1.values1, other2.values9),
                Shift::fma<T>(this->values10, other1.values2, other2.values10),
                Shift::fma<T>(this->values11, other1.values3, other2.values11),
                Shift::fma<T>(this->values12, other1.values4, other2.values12),
                Shift::fma<T>(this->values13, other1.values5, other2.values13),
                Shift::fma<T>(this->values14, other1.values6, other2.values14),
                Shift::fma<T>(this->values15, other1.values7, other2.values15));
        }
    }

    /**
     * Multiply this object by another and then subtract another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to subtract.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_FUNCTION SIMD16 msub(const SIMD16& other1, const SIMD16& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm512_fmsub_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD16(_mm512_sub_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm256_fmsub_ps(this->values0, other1.values0, other2.values0),
                    _mm256_fmsub_ps(this->values1, other1.values1, other2.values1));
            } else {
                return SIMD16(_mm256_sub_ps(_mm256_mul_ps(this->values0, other1.values0), other2.values0),
                    _mm256_sub_ps(_mm256_mul_ps(this->values1, other1.values1), other2.values1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD16(_mm_fmsub_ps(this->values0, other1.values0, other2.values0),
                    _mm_fmsub_ps(this->values1, other1.values1, other2.values1),
                    _mm_fmsub_ps(this->values2, other1.values2, other2.values2),
                    _mm_fmsub_ps(this->values3, other1.values3, other2.values3));
            } else {
                return SIMD16(_mm_sub_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                    _mm_sub_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1),
                    _mm_sub_ps(_mm_mul_ps(this->values2, other1.values2), other2.values2),
                    _mm_sub_ps(_mm_mul_ps(this->values3, other1.values3), other2.values3));
            }
        } else
#endif
        {
            return SIMD16(Shift::fma<T>(this->values0, other1.values0, -other2.values0),
                Shift::fma<T>(this->values1, other1.values1, -other2.values1),
                Shift::fma<T>(this->values2, other1.values2, -other2.values2),
                Shift::fma<T>(this->values3, other1.values3, -other2.values3),
                Shift::fma<T>(this->values4, other1.values4, -other2.values4),
                Shift::fma<T>(this->values5, other1.values5, -other2.values5),
                Shift::fma<T>(this->values6, other1.values6, -other2.values6),
                Shift::fma<T>(this->values7, other1.values7, -other2.values7),
                Shift::fma<T>(this->values8, other1.values8, -other2.values8),
                Shift::fma<T>(this->values9, other1.values9, -other2.values9),
                Shift::fma<T>(this->values10, other1.values10, -other2.values10),
                Shift::fma<T>(this->values11, other1.values11, -other2.values11),
                Shift::fma<T>(this->values12, other1.values12, -other2.values12),
                Shift::fma<T>(this->values13, other1.values13, -other2.values13),
                Shift::fma<T>(this->values14, other1.values14, -other2.values14),
                Shift::fma<T>(this->values15, other1.values15, -other2.values15));
        }
    }

    /**
     * Negate this object based on the sign of another.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @param other The second object whose sign to check.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD16 sign(const SIMD16& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD16(_mm512_xor_ps(this->values, _mm512_and_ps(other.values, _mm512_set1_ps(-0.0f))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val = _mm256_set1_ps(-0.0f);
            return SIMD16(_mm256_xor_ps(this->values0, _mm256_and_ps(other.values0, val)),
                _mm256_xor_ps(this->values1, _mm256_and_ps(other.values1, val)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_set1_ps(-0.0f);
            return SIMD16(_mm_xor_ps(this->values0, _mm_and_ps(val, other.values0)),
                _mm_xor_ps(this->values1, _mm_and_ps(val, other.values1)),
                _mm_xor_ps(this->values2, _mm_and_ps(val, other.values2)),
                _mm_xor_ps(this->values3, _mm_and_ps(val, other.values3)));
        } else
#endif
        {
            return SIMD16(Shift::sign<T>(this->values0, other.values0), Shift::sign<T>(this->values1, other.values1),
                Shift::sign<T>(this->values2, other.values2), Shift::sign<T>(this->values3, other.values3),
                Shift::sign<T>(this->values4, other.values4), Shift::sign<T>(this->values5, other.values5),
                Shift::sign<T>(this->values6, other.values6), Shift::sign<T>(this->values7, other.values7),
                Shift::sign<T>(this->values8, other.values8), Shift::sign<T>(this->values9, other.values9),
                Shift::sign<T>(this->values10, other.values10), Shift::sign<T>(this->values11, other.values11),
                Shift::sign<T>(this->values12, other.values12), Shift::sign<T>(this->values13, other.values13),
                Shift::sign<T>(this->values14, other.values14), Shift::sign<T>(this->values15, other.values15));
        }
    }

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD16 abs() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD16(_mm512_andnot_ps(_mm512_set1_ps(-0.0f), this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val = _mm256_set1_ps(-0.0f);
            return SIMD16(_mm256_andnot_ps(val, this->values0), _mm256_andnot_ps(val, this->values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_set1_ps(-0.0f);
            return SIMD16(_mm_andnot_ps(val, this->values0), _mm_andnot_ps(val, this->values1),
                _mm_andnot_ps(val, this->values2), _mm_andnot_ps(val, this->values3));
        } else
#endif
        {
            return SIMD16(Shift::abs<T>(this->values0), Shift::abs<T>(this->values1), Shift::abs<T>(this->values2),
                Shift::abs<T>(this->values3), Shift::abs<T>(this->values4), Shift::abs<T>(this->values5),
                Shift::abs<T>(this->values6), Shift::abs<T>(this->values7), Shift::abs<T>(this->values8),
                Shift::abs<T>(this->values9), Shift::abs<T>(this->values10), Shift::abs<T>(this->values11),
                Shift::abs<T>(this->values12), Shift::abs<T>(this->values13), Shift::abs<T>(this->values14),
                Shift::abs<T>(this->values15));
        }
    }

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMD16 max(const SIMD16& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD16(_mm512_max_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD16(_mm256_max_ps(this->values0, other.values0), _mm256_max_ps(this->values1, other.values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD16(_mm_max_ps(this->values0, other.values0), _mm_max_ps(this->values1, other.values1),
                _mm_max_ps(this->values2, other.values2), _mm_max_ps(this->values3, other.values3));
        } else
#endif
        {
            return SIMD16(Shift::max<T>(this->values0, other.values0), Shift::max<T>(this->values1, other.values1),
                Shift::max<T>(this->values2, other.values2), Shift::max<T>(this->values3, other.values3),
                Shift::max<T>(this->values4, other.values4), Shift::max<T>(this->values5, other.values5),
                Shift::max<T>(this->values6, other.values6), Shift::max<T>(this->values7, other.values7),
                Shift::max<T>(this->values8, other.values8), Shift::max<T>(this->values9, other.values9),
                Shift::max<T>(this->values10, other.values10), Shift::max<T>(this->values11, other.values11),
                Shift::max<T>(this->values12, other.values12), Shift::max<T>(this->values13, other.values13),
                Shift::max<T>(this->values14, other.values14), Shift::max<T>(this->values15, other.values15));
        }
    }

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMD16 min(const SIMD16& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD16(_mm512_min_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD16(_mm256_min_ps(this->values0, other.values0), _mm256_min_ps(this->values1, other.values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD16(_mm_min_ps(this->values0, other.values0), _mm_min_ps(this->values1, other.values1),
                _mm_min_ps(this->values2, other.values2), _mm_min_ps(this->values3, other.values3));
        } else
#endif
        {
            return SIMD16(Shift::min<T>(this->values0, other.values0), Shift::min<T>(this->values1, other.values1),
                Shift::min<T>(this->values2, other.values2), Shift::min<T>(this->values3, other.values3),
                Shift::min<T>(this->values4, other.values4), Shift::min<T>(this->values5, other.values5),
                Shift::min<T>(this->values6, other.values6), Shift::min<T>(this->values7, other.values7),
                Shift::min<T>(this->values8, other.values8), Shift::min<T>(this->values9, other.values9),
                Shift::min<T>(this->values10, other.values10), Shift::min<T>(this->values11, other.values11),
                Shift::min<T>(this->values12, other.values12), Shift::min<T>(this->values13, other.values13),
                Shift::min<T>(this->values14, other.values14), Shift::min<T>(this->values15, other.values15));
        }
    }

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMD16 max(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD16(_mm512_max_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD16(_mm256_max_ps(this->values0, other.values), _mm256_max_ps(this->values1, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD16(_mm_max_ps(this->values0, other.values), _mm_max_ps(this->values1, other.values),
                _mm_max_ps(this->values2, other.values), _mm_max_ps(this->values3, other.values));
        } else
#endif
        {
            return SIMD16(Shift::max<T>(this->values0, other.value), Shift::max<T>(this->values1, other.value),
                Shift::max<T>(this->values2, other.value), Shift::max<T>(this->values3, other.value),
                Shift::max<T>(this->values4, other.value), Shift::max<T>(this->values5, other.value),
                Shift::max<T>(this->values6, other.value), Shift::max<T>(this->values7, other.value),
                Shift::max<T>(this->values8, other.value), Shift::max<T>(this->values9, other.value),
                Shift::max<T>(this->values10, other.value), Shift::max<T>(this->values11, other.value),
                Shift::max<T>(this->values12, other.value), Shift::max<T>(this->values13, other.value),
                Shift::max<T>(this->values14, other.value), Shift::max<T>(this->values15, other.value));
        }
    }

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMD16 min(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD16(_mm512_min_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD16(_mm256_min_ps(this->values0, other.values), _mm256_min_ps(this->values1, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD16(_mm_min_ps(this->values0, other.values), _mm_min_ps(this->values1, other.values),
                _mm_min_ps(this->values2, other.values), _mm_min_ps(this->values3, other.values));
        } else
#endif
        {
            return SIMD16(Shift::min<T>(this->values0, other.value), Shift::min<T>(this->values1, other.value),
                Shift::min<T>(this->values2, other.value), Shift::min<T>(this->values3, other.value),
                Shift::min<T>(this->values4, other.value), Shift::min<T>(this->values5, other.value),
                Shift::min<T>(this->values6, other.value), Shift::min<T>(this->values7, other.value),
                Shift::min<T>(this->values8, other.value), Shift::min<T>(this->values9, other.value),
                Shift::min<T>(this->values10, other.value), Shift::min<T>(this->values11, other.value),
                Shift::min<T>(this->values12, other.value), Shift::min<T>(this->values13, other.value),
                Shift::min<T>(this->values14, other.value), Shift::min<T>(this->values15, other.value));
        }
    }

    /**
     * Maximum of each internally stored SIMD4.
     * @returns SIMD4 set to the largest value between each internal SIMD4.
     */
    XS_FUNCTION SIMD4Def max4() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m256 val =
                _mm256_max_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
            return SIMD4Def(_mm_max_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val = _mm256_max_ps(this->values0, this->values1);
            return SIMD4Def(_mm_max_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_max_ps(this->values0, this->values1);
            const __m128 val1 = _mm_max_ps(this->values2, this->values3);
            return SIMD4Def(_mm_max_ps(val0, val1));
        } else
#endif
        {
            T other1 = Shift::max<T>(this->values0, this->values4);
            T other2 = Shift::max<T>(this->values1, this->values5);
            T other3 = Shift::max<T>(this->values2, this->values6);
            T other4 = Shift::max<T>(this->values3, this->values7);
            other1 = Shift::max<T>(other1, this->values8);
            other2 = Shift::max<T>(other2, this->values9);
            other3 = Shift::max<T>(other3, this->values10);
            other4 = Shift::max<T>(other4, this->values11);
            return SIMD4Def(Shift::max<T>(other1, this->values12), Shift::max<T>(other2, this->values13),
                Shift::max<T>(other3, this->values14), Shift::max<T>(other4, this->values15));
        }
    }

    /**
     * Minimum of each internally stored SIMD4.
     * @returns SIMD4 set to the largest value between each internal SIMD4.
     */
    XS_FUNCTION SIMD4Def min4() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m256 val =
                _mm256_min_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
            return SIMD4Def(_mm_min_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val = _mm256_min_ps(this->values0, this->values1);
            return SIMD4Def(_mm_min_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_min_ps(this->values0, this->values1);
            const __m128 val1 = _mm_min_ps(this->values2, this->values3);
            return SIMD4Def(_mm_min_ps(val0, val1));
        } else
#endif
        {
            T other1 = Shift::min<T>(this->values0, this->values4);
            T other2 = Shift::min<T>(this->values1, this->values5);
            T other3 = Shift::min<T>(this->values2, this->values6);
            T other4 = Shift::min<T>(this->values3, this->values7);
            other1 = Shift::min<T>(other1, this->values8);
            other2 = Shift::min<T>(other2, this->values9);
            other3 = Shift::min<T>(other3, this->values10);
            other4 = Shift::min<T>(other4, this->values11);
            return SIMD4Def(Shift::min<T>(other1, this->values12), Shift::min<T>(other2, this->values13),
                Shift::min<T>(other3, this->values14), Shift::min<T>(other4, this->values15));
        }
    }

    /**
     * Maximum of each internally stored SIMD8.
     * @returns SIMD8 set to the largest value between each internal SIMD8.
     */
    XS_FUNCTION SIMD8Def max8() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD8Def(
                _mm256_max_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD8Def(_mm256_max_ps(this->values0, this->values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD8Def(_mm_max_ps(this->values0, this->values2), _mm_max_ps(this->values1, this->values3));
        } else
#endif
        {
            return SIMD8Def(Shift::max<T>(this->values0, this->values8), Shift::max<T>(this->values1, this->values9),
                Shift::max<T>(this->values2, this->values10), Shift::max<T>(this->values3, this->values11),
                Shift::max<T>(this->values4, this->values12), Shift::max<T>(this->values5, this->values13),
                Shift::max<T>(this->values6, this->values14), Shift::max<T>(this->values7, this->values15));
        }
    }

    /**
     * Minimum of each internally stored SIMD8.
     * @returns SIMD8 set to the largest value between each internal SIMD8.
     */
    XS_FUNCTION SIMD8Def min8() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD8Def(
                _mm256_min_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD8Def(_mm256_min_ps(this->values0, this->values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD8Def(_mm_min_ps(this->values0, this->values2), _mm_min_ps(this->values1, this->values3));
        } else
#endif
        {
            return SIMD8Def(Shift::min<T>(this->values0, this->values8), Shift::min<T>(this->values1, this->values9),
                Shift::min<T>(this->values2, this->values10), Shift::min<T>(this->values3, this->values11),
                Shift::min<T>(this->values4, this->values12), Shift::min<T>(this->values5, this->values13),
                Shift::min<T>(this->values6, this->values14), Shift::min<T>(this->values7, this->values15));
        }
    }

    /**
     * Maximum element of this object.
     * @returns SIMDBase set to the largest value.
     */
    XS_FUNCTION BaseDef hmax() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m512 val =
                _mm512_max_ps(this->values, _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 3, 0, 1)));
            val = _mm512_max_ps(_mm512_shuffle_f32x4(val, val, _MM_SHUFFLE(1, 0, 3, 2)), val);
            val = _mm512_max_ps(_mm512_shuffle1_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
            return BaseDef(_mm512_max_ps(_mm512_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val = _mm256_max_ps(this->values0, this->values1);
            val = _mm256_max_ps(_mm256_shuffle32107654_ps(val), val);
            val = _mm256_max_ps(_mm256_shuffle1_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
            return BaseDef(_mm256_max_ps(_mm256_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_max_ps(this->values0, this->values1);
            const __m128 val1 = _mm_max_ps(this->values2, this->values3);
            val0 = _mm_max_ps(val0, val1);
            val0 = _mm_max_ps(_mm_shuffle1_ps(val0, _MM_SHUFFLE(1, 0, 3, 2)), val0);
            return BaseDef(_mm_max_ps(_mm_shuffle1_ps(val0, _MM_SHUFFLE(2, 3, 0, 1)), val0));
        } else
#endif
        {
            T other = Shift::max<T>(this->values0, this->values1);
            T other2 = Shift::max<T>(this->values2, this->values3);
            T other3 = Shift::max<T>(this->values4, this->values5);
            T other4 = Shift::max<T>(this->values6, this->values7);
            other = Shift::max<T>(this->values8, other);
            other2 = Shift::max<T>(this->values9, other2);
            other3 = Shift::max<T>(this->values10, other3);
            other4 = Shift::max<T>(this->values11, other4);
            other = Shift::max<T>(this->values12, other);
            other2 = Shift::max<T>(this->values13, other2);
            other3 = Shift::max<T>(this->values14, other3);
            other4 = Shift::max<T>(this->values15, other4);
            other = Shift::max<T>(other2, other);
            other3 = Shift::max<T>(other4, other3);
            return BaseDef(Shift::max<T>(other3, other));
        }
    }

    /**
     * Minimum element of this object.
     * @returns SIMDBase set to the smallest value.
     */
    XS_FUNCTION BaseDef hmin() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m512 val =
                _mm512_min_ps(this->values, _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 3, 0, 1)));
            val = _mm512_min_ps(_mm512_shuffle_f32x4(val, val, _MM_SHUFFLE(1, 0, 3, 2)), val);
            val = _mm512_min_ps(_mm512_shuffle1_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
            return BaseDef(_mm512_min_ps(_mm512_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val = _mm256_min_ps(this->values0, this->values1);
            val = _mm256_min_ps(_mm256_shuffle32107654_ps(val), val);
            val = _mm256_min_ps(_mm256_shuffle1_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
            return BaseDef(_mm256_min_ps(_mm256_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_min_ps(this->values0, this->values1);
            const __m128 val1 = _mm_min_ps(this->values2, this->values3);
            val0 = _mm_min_ps(val0, val1);
            val0 = _mm_min_ps(_mm_shuffle1_ps(val0, _MM_SHUFFLE(1, 0, 3, 2)), val0);
            return BaseDef(_mm_min_ps(_mm_shuffle1_ps(val0, _MM_SHUFFLE(2, 3, 0, 1)), val0));
        } else
#endif
        {
            T other = Shift::min<T>(this->values0, this->values1);
            T other2 = Shift::min<T>(this->values2, this->values3);
            T other3 = Shift::min<T>(this->values4, this->values5);
            T other4 = Shift::min<T>(this->values6, this->values7);
            other = Shift::min<T>(this->values8, other);
            other2 = Shift::min<T>(this->values9, other);
            other3 = Shift::min<T>(this->values10, other);
            other4 = Shift::min<T>(this->values11, other);
            other = Shift::min<T>(this->values12, other);
            other2 = Shift::min<T>(this->values13, other);
            other3 = Shift::min<T>(this->values14, other3);
            other4 = Shift::min<T>(this->values15, other4);
            other = Shift::min<T>(other2, other);
            other3 = Shift::min<T>(other4, other3);
            return BaseDef(Shift::min<T>(other3, other));
        }
    }

    /**
     * Maximum element of this object.
     * @returns SIMDInBase set to the largest value.
     */
    XS_FUNCTION InBaseDef hmaxInBase() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m256 val0 =
                _mm256_max_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
            __m128 val1 = _mm_max_ps(_mm256_extractf128_ps(val0, 1), _mm256_castps256_ps128(val0));
            val1 = _mm_max_ps(_mm_shuffle3232_ps(val1), val1);
            return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(val1), val1));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val0 = _mm256_max_ps(this->values0, this->values1);
            __m128 val1 = _mm_max_ps(_mm256_extractf128_ps(val0, 1), _mm256_castps256_ps128(val0));
            val1 = _mm_max_ps(_mm_shuffle3232_ps(val1), val1);
            return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(val1), val1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_max_ps(this->values0, this->values1);
            const __m128 val1 = _mm_max_ps(this->values2, this->values3);
            val0 = _mm_max_ps(val0, val1);
            val0 = _mm_max_ps(_mm_shuffle3232_ps(val0), val0);            //(x,x,3,2)
            return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(val0), val0)); //(x,x,x,1)
        } else
#endif
        {
            return InBaseDef(hmax().value);
        }
    }

    /**
     * Minimum element this object returned as a SIMDInBase.
     * @returns SIMDInBase set to the smallest value.
     */
    XS_FUNCTION InBaseDef hminInBase() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m256 val0 =
                _mm256_min_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
            __m128 val1 = _mm_min_ps(_mm256_extractf128_ps(val0, 1), _mm256_castps256_ps128(val0));
            val1 = _mm_min_ps(_mm_shuffle3232_ps(val1), val1);
            return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(val1), val1));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val0 = _mm256_min_ps(this->values0, this->values1);
            __m128 val1 = _mm_min_ps(_mm256_extractf128_ps(val0, 1), _mm256_castps256_ps128(val0));
            val1 = _mm_min_ps(_mm_shuffle3232_ps(val1), val1);
            return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(val1), val1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_min_ps(this->values0, this->values1);
            const __m128 val1 = _mm_min_ps(this->values2, this->values3);
            val0 = _mm_min_ps(val0, val1);
            val0 = _mm_min_ps(_mm_shuffle3232_ps(val0), val0);            //(x,x,3,2)
            return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(val0), val0)); //(x,x,x,1)
        } else
#endif
        {
            return InBaseDef(hmin().value);
        }
    }

    /**
     * Maximum element from each SIMD2.
     * @returns SIMD8 set to the largest value in each SIMD2.
     */
    XS_FUNCTION SIMD8Def hmax2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 1, 2, 0));
            const __m256 val1 = _mm512_extractf32x8_ps(val0, 1);
            const __m256 val2 = _mm256_shuffle_ps(_mm512_castps512_ps256(val0), val1, _MM_SHUFFLE(2, 0, 2, 0));
            const __m256 val3 = _mm256_shuffle_ps(_mm512_castps512_ps256(val0), val1, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD8Def(_mm256_max_ps(val2, val3));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val0 =
                _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(2, 0)); //(13,12,11,10,03,02,01,00)
            const __m256 val1 =
                _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(3, 1)); //(17,16,15,14,07,06,05,04)
            const __m256 val2 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(2, 0, 2, 0));     //(16,14,12,10,06,04,02,00)
            const __m256 val3 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 1, 3, 1));     //(17,15,13,11,07,05,03,01)
            return SIMD8Def(_mm256_max_ps(val2, val3));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(2, 0, 2, 0));
            const __m128 val1 = _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(3, 1, 3, 1));
            const __m128 val2 = _mm_shuffle_ps(this->values2, this->values3, _MM_SHUFFLE(2, 0, 2, 0));
            const __m128 val3 = _mm_shuffle_ps(this->values2, this->values3, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD8Def(_mm_max_ps(val0, val1), _mm_max_ps(val2, val3));
        } else
#endif
        {
            return SIMD8Def(Shift::max<T>(this->values0, this->values1), Shift::max<T>(this->values2, this->values3),
                Shift::max<T>(this->values4, this->values5), Shift::max<T>(this->values6, this->values7),
                Shift::max<T>(this->values8, this->values9), Shift::max<T>(this->values10, this->values11),
                Shift::max<T>(this->values12, this->values13), Shift::max<T>(this->values14, this->values15));
        }
    }

    /**
     * Minimum element from each SIMD2.
     * @returns SIMD8 set to the smallest value in each SIMD2.
     */
    XS_FUNCTION SIMD8Def hmin2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 1, 2, 0));
            const __m256 val1 = _mm512_extractf32x8_ps(val0, 1);
            const __m256 val2 = _mm256_shuffle_ps(_mm512_castps512_ps256(val0), val1, _MM_SHUFFLE(2, 0, 2, 0));
            const __m256 val3 = _mm256_shuffle_ps(_mm512_castps512_ps256(val0), val1, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD8Def(_mm256_min_ps(val2, val3));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val0 =
                _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(2, 0)); //(13,12,11,10,03,02,01,00)
            const __m256 val1 =
                _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(3, 1)); //(17,16,15,14,07,06,05,04)
            const __m256 val2 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(2, 0, 2, 0));     //(16,14,12,10,06,04,02,00)
            const __m256 val3 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 1, 3, 1));     //(17,15,13,11,07,05,03,01)
            return SIMD8Def(_mm256_min_ps(val2, val3));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(2, 0, 2, 0));
            const __m128 val1 = _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(3, 1, 3, 1));
            const __m128 val2 = _mm_shuffle_ps(this->values2, this->values3, _MM_SHUFFLE(2, 0, 2, 0));
            const __m128 val3 = _mm_shuffle_ps(this->values2, this->values3, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD8Def(_mm_min_ps(val0, val1), _mm_min_ps(val2, val3));
        } else
#endif
        {
            return SIMD8Def(Shift::min<T>(this->values0, this->values1), Shift::min<T>(this->values2, this->values3),
                Shift::min<T>(this->values4, this->values5), Shift::min<T>(this->values6, this->values7),
                Shift::min<T>(this->values8, this->values9), Shift::min<T>(this->values10, this->values11),
                Shift::min<T>(this->values12, this->values13), Shift::min<T>(this->values14, this->values15));
        }
    }

    /**
     * Maximum element from each SIMD4.
     * @returns SIMD4 set to the largest value in each SIMD4.
     */
    XS_FUNCTION SIMD4Def hmax4() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m256 hi = _mm512_extractf32x8_ps(this->values, 1);
            __m256 val0 = _mm256_unpacklo_ps(_mm512_castps512_ps256(this->values), hi);
            const __m256 val1 = _mm256_unpackhi_ps(_mm512_castps512_ps256(this->values), hi);
            val0 = _mm256_max_ps(val0, val1);
            const __m128 val2 = _mm256_extractf128_ps(val0, 1);
            const __m128 val3 = _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2);
            const __m128 val4 = _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2);
            return SIMD4Def(_mm_max_ps(val3, val4));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val0 = _mm256_unpacklo_ps(this->values0, this->values1);       //(15,05,14,04,11,01,10,00)
            const __m256 val1 = _mm256_unpackhi_ps(this->values0, this->values1); //(17,07,16,06,13,03,12,02)
            val0 = _mm256_max_ps(val0, val1); //(1-57, 0-57, 1-46, 0-46, 1-13, 0-13, 1-02, 0-02)
            const __m128 val2 = _mm256_extractf128_ps(val0, 1);
            const __m128 val3 = _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2); //(1-46, 1-02, 0-46, 0-02)
            const __m128 val4 = _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2); //(1-57, 1-13, 0-57, 0-13)
            return SIMD4Def(_mm_max_ps(val3, val4));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_unpacklo_ps(this->values0, this->values1);       //(00,10,01,11)
            __m128 val1 = _mm_unpackhi_ps(this->values0, this->values1);       //(02,12,03,13)
            __m128 val2 = _mm_unpacklo_ps(this->values2, this->values3);       //(20,30,21,31)
            const __m128 val3 = _mm_unpackhi_ps(this->values2, this->values3); //(22,32,23,33)
            val0 = _mm_max_ps(val0, val1);                                     //(00+02, 10+12, 01+03, 11+13)
            val2 = _mm_max_ps(val2, val3);                                     //(20+22, 30+32, 21+03, 31+33)
            val1 = _mm_movelh_ps(val0, val2);
            val2 = _mm_movehl_ps(val2, val0);
            return SIMD4Def(_mm_max_ps(val1, val2));
        } else
#endif
        {
            T other1 = Shift::max<T>(this->values0, this->values1);
            T other2 = Shift::max<T>(this->values4, this->values5);
            T other3 = Shift::max<T>(this->values8, this->values9);
            T other4 = Shift::max<T>(this->values12, this->values13);
            other1 = Shift::max<T>(this->values2, other1);
            other2 = Shift::max<T>(this->values6, other2);
            other3 = Shift::max<T>(this->values10, other3);
            other4 = Shift::max<T>(this->values14, other3);
            return SIMD4Def(Shift::max<T>(this->values3, other1), Shift::max<T>(this->values7, other2),
                Shift::max<T>(this->values11, other3), Shift::max<T>(this->values15, other4));
        }
    }

    /**
     * Minimum element from each SIMD4.
     * @returns SIMD4 set to the smallest value in each SIMD4.
     */
    XS_FUNCTION SIMD4Def hmin4() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m256 hi = _mm512_extractf32x8_ps(this->values, 1);
            __m256 val0 = _mm256_unpacklo_ps(_mm512_castps512_ps256(this->values), hi);
            const __m256 val1 = _mm256_unpackhi_ps(_mm512_castps512_ps256(this->values), hi);
            val0 = _mm256_min_ps(val0, val1);
            const __m128 val2 = _mm256_extractf128_ps(val0, 1);
            const __m128 val3 = _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2);
            const __m128 val4 = _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2);
            return SIMD4Def(_mm_min_ps(val3, val4));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val0 = _mm256_unpacklo_ps(this->values0, this->values1);       //(15,05,14,04,11,01,10,00)
            const __m256 val1 = _mm256_unpackhi_ps(this->values0, this->values1); //(17,07,16,06,13,03,12,02)
            val0 = _mm256_min_ps(val0, val1); //(1-57, 0-57, 1-46, 0-46, 1-13, 0-13, 1-02, 0-02)
            const __m128 val2 = _mm256_extractf128_ps(val0, 1);
            const __m128 val3 = _mm_unpacklo_ps(_mm256_castps256_ps128(val0), val2); //(1-46, 1-02, 0-46, 0-02)
            const __m128 val4 = _mm_unpackhi_ps(_mm256_castps256_ps128(val0), val2); //(1-57, 1-13, 0-57, 0-13)
            return SIMD4Def(_mm_min_ps(val3, val4));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_unpacklo_ps(this->values0, this->values1);       //(00,10,01,11)
            __m128 val1 = _mm_unpackhi_ps(this->values0, this->values1);       //(02,12,03,13)
            __m128 val2 = _mm_unpacklo_ps(this->values2, this->values3);       //(20,30,21,31)
            const __m128 val3 = _mm_unpackhi_ps(this->values2, this->values3); //(22,32,23,33)
            val0 = _mm_min_ps(val0, val1);                                     //(00+02, 10+12, 01+03, 11+13)
            val2 = _mm_min_ps(val2, val3);                                     //(20+22, 30+32, 21+03, 31+33)
            val1 = _mm_movelh_ps(val0, val2);
            val2 = _mm_movehl_ps(val2, val0);
            return SIMD4Def(_mm_min_ps(val1, val2));
        } else
#endif
        {
            T other1 = Shift::min<T>(this->values0, this->values1);
            T other2 = Shift::min<T>(this->values4, this->values5);
            T other3 = Shift::min<T>(this->values8, this->values9);
            T other4 = Shift::min<T>(this->values12, this->values13);
            other1 = Shift::min<T>(this->values2, other1);
            other2 = Shift::min<T>(this->values6, other2);
            other3 = Shift::min<T>(this->values10, other3);
            other4 = Shift::min<T>(this->values14, other3);
            return SIMD4Def(Shift::min<T>(this->values3, other1), Shift::min<T>(this->values7, other2),
                Shift::min<T>(this->values11, other3), Shift::min<T>(this->values15, other4));
        }
    }

    /**
     * Maximum element from each SIMD8.
     * @returns SIMD2 set to the largest value in each SIMD8.
     */
    XS_FUNCTION SIMD2Def hmax8() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m256 hi = _mm512_extractf32x8_ps(this->values, 1);
            __m256 val0 = _mm256_unpacklo_ps(_mm512_castps512_ps256(this->values), hi);
            const __m256 val1 = _mm256_unpackhi_ps(_mm512_castps512_ps256(this->values), hi);
            val0 = _mm256_max_ps(val0, val1);
            __m128 val2 = _mm256_extractf128_ps(val0, 1);
            val2 = _mm_max_ps(val2, _mm256_castps256_ps128(val0));
            return SIMD2Def(_mm_max_ps(_mm_movehl_ps(val2, val2), val2));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val0 = _mm256_unpacklo_ps(this->values0, this->values1);       //(15,05,14,04,11,01,10,00)
            const __m256 val1 = _mm256_unpackhi_ps(this->values0, this->values1); //(17,07,16,06,13,03,12,02)
            val0 = _mm256_max_ps(val0, val1); //(1-57, 0-57, 1-46, 0-46, 1-13, 0-13, 1-02, 0-02)
            __m128 val2 = _mm256_extractf128_ps(val0, 1);
            val2 = _mm_max_ps(val2, _mm256_castps256_ps128(val0)); //(1-1357,0-1357,1-0246,0-0246)
            return SIMD2Def(_mm_max_ps(_mm_movehl_ps(val2, val2), val2));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_max_ps(this->values0, this->values1);
            const __m128 val1 = _mm_max_ps(this->values2, this->values3);
            __m128 val2 = _mm_unpacklo_ps(val0, val1);
            const __m128 val3 = _mm_unpackhi_ps(val0, val1);
            val2 = _mm_max_ps(val2, val3);
            return SIMD2Def(_mm_max_ps(_mm_movehl_ps(val2, val2), val2));
        } else
#endif
        {
            T other1 = Shift::max<T>(this->values0, this->values1);
            T other2 = Shift::max<T>(this->values8, this->values9);
            T other3 = Shift::max<T>(this->values2, this->values3);
            T other4 = Shift::max<T>(this->values10, this->values11);
            other1 = Shift::max<T>(this->values4, other1);
            other2 = Shift::max<T>(this->values12, other2);
            other3 = Shift::max<T>(this->values5, other3);
            other4 = Shift::max<T>(this->values13, other4);
            other1 = Shift::max<T>(this->values6, other1);
            other2 = Shift::max<T>(this->values14, other2);
            other3 = Shift::max<T>(this->values7, other3);
            other4 = Shift::max<T>(this->values15, other4);
            return SIMD2Def(Shift::max<T>(other3, other1), Shift::max<T>(other4, other2));
        }
    }

    /**
     * Minimum element from each SIMD8.
     * @returns SIMD2 set to the smallest value in each SIMD8.
     */
    XS_FUNCTION SIMD2Def hmin8() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m256 hi = _mm512_extractf32x8_ps(this->values, 1);
            __m256 val0 = _mm256_unpacklo_ps(_mm512_castps512_ps256(this->values), hi);
            const __m256 val1 = _mm256_unpackhi_ps(_mm512_castps512_ps256(this->values), hi);
            val0 = _mm256_min_ps(val0, val1);
            __m128 val2 = _mm256_extractf128_ps(val0, 1);
            val2 = _mm_min_ps(val2, _mm256_castps256_ps128(val0));
            return SIMD2Def(_mm_min_ps(_mm_movehl_ps(val2, val2), val2));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val0 = _mm256_unpacklo_ps(this->values0, this->values1);       //(15,05,14,04,11,01,10,00)
            const __m256 val1 = _mm256_unpackhi_ps(this->values0, this->values1); //(17,07,16,06,13,03,12,02)
            val0 = _mm256_min_ps(val0, val1); //(1-57, 0-57, 1-46, 0-46, 1-13, 0-13, 1-02, 0-02)
            __m128 val2 = _mm256_extractf128_ps(val0, 1);
            val2 = _mm_min_ps(val2, _mm256_castps256_ps128(val0)); //(1-1357,0-1357,1-0246,0-0246)
            return SIMD2Def(_mm_min_ps(_mm_movehl_ps(val2, val2), val2));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_min_ps(this->values0, this->values1);
            const __m128 val1 = _mm_min_ps(this->values2, this->values3);
            __m128 val2 = _mm_unpacklo_ps(val0, val1);
            const __m128 val3 = _mm_unpackhi_ps(val0, val1);
            val2 = _mm_min_ps(val2, val3);
            return SIMD2Def(_mm_min_ps(_mm_movehl_ps(val2, val2), val2));
        } else
#endif
        {
            T other1 = Shift::min<T>(this->values0, this->values1);
            T other2 = Shift::min<T>(this->values8, this->values9);
            T other3 = Shift::min<T>(this->values2, this->values3);
            T other4 = Shift::min<T>(this->values10, this->values11);
            other1 = Shift::min<T>(this->values4, other1);
            other2 = Shift::min<T>(this->values12, other2);
            other3 = Shift::min<T>(this->values5, other3);
            other4 = Shift::min<T>(this->values13, other4);
            other1 = Shift::min<T>(this->values6, other1);
            other2 = Shift::min<T>(this->values14, other2);
            other3 = Shift::min<T>(this->values7, other3);
            other4 = Shift::min<T>(this->values15, other4);
            return SIMD2Def(Shift::min<T>(other3, other1), Shift::min<T>(other4, other2));
        }
    }

    /**
     * Compute the sum of each internally stored SIMD4.
     * @returns SIMD4 set to the sum of all elements between each internal SIMD4.
     */
    XS_FUNCTION SIMD4Def add4() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m256 val =
                _mm256_add_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
            return SIMD4Def(_mm_add_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val = _mm256_add_ps(this->values0, this->values1);
            return SIMD4Def(_mm_add_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_add_ps(this->values0, this->values1);
            const __m128 val1 = _mm_add_ps(this->values2, this->values3);
            return SIMD4Def(_mm_add_ps(val0, val1));
        } else
#endif
        {
            T other1 = this->values0 + this->values4;
            T other2 = this->values1 + this->values5;
            T other3 = this->values2 + this->values6;
            T other4 = this->values3 + this->values7;
            other1 = other1 + this->values8;
            other2 = other2 + this->values9;
            other3 = other3 + this->values10;
            other4 = other4 + this->values11;
            return SIMD4Def(
                other1 + this->values12, other2 + this->values13, other3 + this->values14, other4 + this->values15);
        }
    }

    /**
     * Compute the sum of each internally stored SIMD8.
     * @returns SIMD8 set to the sum of all elements between each internal SIMD8.
     */
    XS_FUNCTION SIMD8Def add8() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD8Def(
                _mm256_add_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD8Def(_mm256_add_ps(this->values0, this->values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD8Def(_mm_add_ps(this->values0, this->values2), _mm_add_ps(this->values1, this->values3));
        } else
#endif
        {
            return SIMD8Def(this->values0 + this->values8, this->values1 + this->values9,
                this->values2 + this->values10, this->values3 + this->values11, this->values4 + this->values12,
                this->values5 + this->values13, this->values6 + this->values14, this->values7 + this->values15);
        }
    }

    /**
     * Compute the difference of each internally stored SIMD8.
     * @returns SIMD8 set to the difference of all elements between each internal SIMD8.
     */
    XS_FUNCTION SIMD8Def sub8() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD8Def(
                _mm256_sub_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD8Def(_mm256_sub_ps(this->values0, this->values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD8Def(_mm_sub_ps(this->values0, this->values2), _mm_sub_ps(this->values1, this->values3));
        } else
#endif
        {
            return SIMD8Def(this->values0 - this->values8, this->values1 - this->values9,
                this->values2 - this->values10, this->values3 - this->values11, this->values4 - this->values12,
                this->values5 - this->values13, this->values6 - this->values14, this->values7 - this->values15);
        }
    }

    /**
     * Compute the sum of all elements.
     * @returns BaseDef set to the sum of all elements.
     */
    XS_FUNCTION BaseDef hadd() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m512 val =
                _mm512_add_ps(this->values, _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 3, 0, 1)));
            val = _mm512_add_ps(_mm512_shuffle_f32x4(val, val, _MM_SHUFFLE(1, 0, 3, 2)), val);
            val = _mm512_add_ps(_mm512_shuffle1_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
            return BaseDef(_mm512_add_ps(_mm512_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val = _mm256_add_ps(this->values0, this->values1);
            val = _mm256_add_ps(_mm256_shuffle32107654_ps(val), val);
            val = _mm256_add_ps(_mm256_shuffle1_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
            return BaseDef(_mm256_add_ps(_mm256_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_add_ps(this->values0, this->values1);
            const __m128 val1 = _mm_add_ps(this->values2, this->values3);
            val0 = _mm_add_ps(val0, val1);
            val0 = _mm_add_ps(_mm_shuffle1_ps(val0, _MM_SHUFFLE(1, 0, 3, 2)), val0);
            return BaseDef(_mm_add_ps(_mm_shuffle1_ps(val0, _MM_SHUFFLE(2, 3, 0, 1)), val0));
        } else
#endif
        {
            T res = (this->values0 + this->values1);
            T res2 = (this->values2 + this->values3);
            T res3 = (this->values4 + this->values5);
            T res4 = (this->values6 + this->values7);
            res += this->values8;
            res2 += this->values9;
            res3 += this->values10;
            res4 += this->values11;
            res += this->values12;
            res2 += this->values13;
            res3 += this->values14;
            res4 += this->values15;
            res += res3;
            res2 += res4;
            res += res2;
            return BaseDef(res);
        }
    }

    /**
     * Compute the sum of all elements.
     * @returns SIMDInBase set to the sum of all elements.
     */
    XS_FUNCTION InBaseDef haddInBase() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m256 val0 =
                _mm256_add_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
            __m128 val1 = _mm_add_ps(_mm256_extractf128_ps(val0, 1), _mm256_castps256_ps128(val0));
            val1 = _mm_add_ps(_mm_shuffle3232_ps(val1), val1);
            return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(val1), val1));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val0 = _mm256_add_ps(this->values0, this->values1);
            __m128 val1 = _mm_add_ps(_mm256_extractf128_ps(val0, 1), _mm256_castps256_ps128(val0));
            val1 = _mm_add_ps(_mm_shuffle3232_ps(val1), val1);
            return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(val1), val1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_add_ps(this->values0, this->values1);
            const __m128 val1 = _mm_add_ps(this->values2, this->values3);
            val0 = _mm_add_ps(val0, val1);
            val0 = _mm_add_ps(_mm_shuffle3232_ps(val0), val0);            //(x,x,3,2)
            return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(val0), val0)); //(x,x,x,1)
        } else
#endif
        {
            T res = (this->values0 + this->values1);
            T res2 = (this->values2 + this->values3);
            T res3 = (this->values4 + this->values5);
            T res4 = (this->values6 + this->values7);
            res += this->values8;
            res2 += this->values9;
            res3 += this->values10;
            res4 += this->values11;
            res += this->values12;
            res2 += this->values13;
            res3 += this->values14;
            res4 += this->values15;
            res += res3;
            res2 += res4;
            res += res2;
            return InBaseDef(res);
        }
    }

    /**
     * Compute the sum of each SIMD2.
     * @returns SIMD8 set to the sum of all elements in each SIMD2.
     */
    XS_FUNCTION SIMD8Def hadd2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD8Def(_mm256_castpd_ps(_mm256_permute4x64_pd(
                _mm256_castps_pd(
                    _mm256_hadd_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1))),
                _MM_SHUFFLE(3, 1, 2, 0))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                return SIMD8Def(_mm256_castpd_ps(_mm256_permute4x64_pd(
                    _mm256_castps_pd(_mm256_hadd_ps(this->values0, this->values1)), _MM_SHUFFLE(3, 1, 2, 0))));
            } else {
                const __m256 val0 = _mm256_hadd_ps(this->values0, this->values1);
                __m256 val1 = _mm256_shuffle32107654_ps(val0);
                val1 = _mm256_shuffle1_ps(val1, _MM_SHUFFLE(1, 0, 3, 2));
                return SIMD8Def(_mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 1, 1, 0, 0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD8Def(_mm_hadd_ps(this->values0, this->values1), _mm_hadd_ps(this->values2, this->values3));
        } else
#endif
        {
            return SIMD8Def(this->values0 + this->values1, this->values2 + this->values3, this->values4 + this->values5,
                this->values6 + this->values7, this->values8 + this->values9, this->values10 + this->values11,
                this->values12 + this->values13, this->values14 + this->values15);
        }
    }

    /**
     * Compute the sum of each SIMD4.
     * @returns SIMD4 set to the sum of all elements in each SIMD4.
     */
    XS_FUNCTION SIMD4Def hadd4() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m256 val =
                _mm256_hadd_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
            const __m128 val1 = _mm_hadd_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1));
            return SIMD4Def(_mm_shuffle1_ps(val1, _MM_SHUFFLE(3, 1, 2, 0)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val = _mm256_hadd_ps(this->values0, this->values1);
            const __m128 val1 = _mm_hadd_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1));
            return SIMD4Def(_mm_shuffle1_ps(val1, _MM_SHUFFLE(3, 1, 2, 0)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_hadd_ps(this->values0, this->values1);
            const __m128 val1 = _mm_hadd_ps(this->values2, this->values3);
            return SIMD4Def(_mm_hadd_ps(val0, val1));
        } else
#endif
        {
            T res = (this->values0 + this->values1);
            T res2 = (this->values4 + this->values5);
            T res3 = (this->values8 + this->values9);
            T res4 = (this->values12 + this->values13);
            res += this->values2;
            res2 += this->values6;
            res3 += this->values10;
            res4 += this->values14;
            res += this->values3;
            res2 += this->values7;
            res3 += this->values11;
            res4 += this->values15;
            return SIMD4Def(res, res2, res3, res4);
        }
    }

    /**
     * Compute the sum of each SIMD8.
     * @returns SIMD2 set to the sum of all elements in each SIMD8.
     */
    XS_FUNCTION SIMD2Def hadd8() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m256 hi = _mm512_extractf32x8_ps(this->values, 1);
            __m256 val0 = _mm256_unpacklo_ps(_mm512_castps512_ps256(this->values), hi);
            const __m256 val1 = _mm256_unpackhi_ps(_mm512_castps512_ps256(this->values), hi);
            val0 = _mm256_add_ps(val0, val1);
            __m128 val2 = _mm256_extractf128_ps(val0, 1);
            val2 = _mm_add_ps(val2, _mm256_castps256_ps128(val0));
            return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(val2), val2));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val0 = _mm256_unpacklo_ps(this->values0, this->values1);       //(15,05,14,04,11,01,10,00)
            const __m256 val1 = _mm256_unpackhi_ps(this->values0, this->values1); //(17,07,16,06,13,03,12,02)
            val0 = _mm256_add_ps(val0, val1); //(1-57, 0-57, 1-46, 0-46, 1-13, 0-13, 1-02, 0-02)
            __m128 val2 = _mm256_extractf128_ps(val0, 1);
            val2 = _mm_add_ps(val2, _mm256_castps256_ps128(val0)); //(1-1357,0-1357,1-0246,0-0246)
            return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(val2), val2));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_add_ps(this->values0, this->values1);
            const __m128 val1 = _mm_add_ps(this->values2, this->values3);
            __m128 val2 = _mm_unpacklo_ps(val0, val1);
            const __m128 val3 = _mm_unpackhi_ps(val0, val1);
            val2 = _mm_add_ps(val2, val3);
            return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(val2), val2));
        } else
#endif
        {
            T res = (this->values0 + this->values1);
            T res2 = (this->values4 + this->values5);
            T res3 = (this->values8 + this->values9);
            T res4 = (this->values12 + this->values13);
            res += this->values2;
            res2 += this->values6;
            res3 += this->values10;
            res4 += this->values14;
            res += this->values3;
            res2 += this->values7;
            res3 += this->values11;
            res4 += this->values15;
            res += res2;
            res3 += res4;
            return SIMD2Def(res, res3);
        }
    }

    /**
     * Compute the difference of each SIMD2.
     * @returns SIMD8 set to the difference of all elements in each SIMD2.
     */
    XS_FUNCTION SIMD8Def hsub2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD8Def(_mm256_castpd_ps(_mm256_permute4x64_pd(
                _mm256_castps_pd(
                    _mm256_hsub_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1))),
                _MM_SHUFFLE(3, 1, 2, 0))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                return SIMD8Def(_mm256_castpd_ps(_mm256_permute4x64_pd(
                    _mm256_castps_pd(_mm256_hsub_ps(this->values0, this->values1)), _MM_SHUFFLE(3, 1, 2, 0))));
            } else {
                const __m256 val0 = _mm256_hsub_ps(this->values0, this->values1);
                __m256 val1 = _mm256_shuffle32107654_ps(val0);
                val1 = _mm256_shuffle1_ps(val1, _MM_SHUFFLE(1, 0, 3, 2));
                return SIMD8Def(_mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 1, 1, 0, 0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD8Def(_mm_hsub_ps(this->values0, this->values1), _mm_hsub_ps(this->values2, this->values3));
        } else
#endif
        {
            return SIMD8Def(this->values0 - this->values1, this->values2 - this->values3, this->values4 - this->values5,
                this->values6 - this->values7, this->values8 - this->values9, this->values10 - this->values11,
                this->values12 - this->values13, this->values14 - this->values15);
        }
    }

    /**
     * Compute the 4 dimensional dot product of each SIMD4 in two objects.
     * @param other The second object.
     * @returns The value of the dot products.
     */
    XS_FUNCTION SIMD4Def dot4(const SIMD16& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 val0 = _mm512_mul_ps(this->values, other.values);
            const __m256 val1 = _mm256_add_ps(_mm512_castps512_ps256(val0), _mm512_extractf32x8_ps(val0, 1));
            return SIMD4Def(_mm_add_ps(_mm256_castps256_ps128(val1), _mm256_extractf128_ps(val1, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val0 = _mm256_mul_ps(this->values0, other.values0);
            const __m256 val1 = _mm256_mul_ps(this->values1, other.values1);
            val0 = _mm256_add_ps(val0, val1);
            return SIMD4Def(_mm_add_ps(_mm256_castps256_ps128(val0), _mm256_extractf128_ps(val0, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_mul_ps(this->values0, other.values0);
            const __m128 val1 = _mm_mul_ps(this->values1, other.values1);
            __m128 val2 = _mm_mul_ps(this->values2, other.values2);
            const __m128 val3 = _mm_mul_ps(this->values3, other.values3);
            val0 = _mm_add_ps(val0, val1);
            val2 = _mm_add_ps(val2, val3);
            return SIMD4Def(_mm_add_ps(val0, val2));
        } else
#endif
        {
            T temp0 = this->values0 * other.values0;
            T temp1 = this->values1 * other.values1;
            T temp2 = this->values2 * other.values2;
            T temp3 = this->values3 * other.values3;
            const T temp4 = this->values4 * other.values4;
            const T temp5 = this->values5 * other.values5;
            const T temp6 = this->values6 * other.values6;
            const T temp7 = this->values7 * other.values7;
            T temp8 = this->values8 * other.values8;
            T temp9 = this->values9 * other.values9;
            T temp10 = this->values10 * other.values10;
            T temp11 = this->values11 * other.values11;
            const T temp12 = this->values12 * other.values12;
            const T temp13 = this->values13 * other.values13;
            const T temp14 = this->values14 * other.values14;
            const T temp15 = this->values15 * other.values15;
            temp0 += temp4;
            temp1 += temp5;
            temp2 += temp6;
            temp3 += temp7;
            temp8 += temp12;
            temp9 += temp13;
            temp10 += temp14;
            temp11 += temp15;
            temp0 += temp8;
            temp1 += temp9;
            temp2 += temp10;
            temp3 += temp11;
            return SIMD4Def(temp0, temp1, temp2, temp3);
        }
    }

    /**
     * Transpose the internal SIMD4s.
     * @returns The transposed object.
     */
    XS_FUNCTION SIMD16 transpose4() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 1, 2, 0));
            __m256 val1 = _mm512_extractf32x8_ps(val0, 1);
            const __m256 v256Lo = _mm256_unpacklo_ps(_mm512_castps512_ps256(val0), val1);
            const __m256 v256Hi = _mm256_unpackhi_ps(_mm512_castps512_ps256(val0), val1);
            __m256 val3 = _mm256_permute2f128_ps(v256Lo, v256Hi, _MM256_PERMUTE(2, 0));
            val1 = _mm256_permute2f128_ps(v256Lo, v256Hi, _MM256_PERMUTE(3, 1));
            const __m256 val2 = _mm256_shuffle_ps(val3, val1, _MM_SHUFFLE(1, 0, 1, 0));
            val3 = _mm256_shuffle_ps(val3, val1, _MM_SHUFFLE(3, 2, 3, 2));
            return SIMD16(_mm512_set_m256(_mm256_permute2f128_ps(val2, val3, _MM256_PERMUTE(3, 1)),
                _mm256_permute2f128_ps(val2, val3, _MM256_PERMUTE(2, 0))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val0 = _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(2, 0));
            __m256 val1 = _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(3, 1));
            const __m256 v256Lo = _mm256_unpacklo_ps(val0, val1);
            const __m256 v256Hi = _mm256_unpackhi_ps(val0, val1);
            val0 = _mm256_permute2f128_ps(v256Lo, v256Hi, _MM256_PERMUTE(2, 0));
            val1 = _mm256_permute2f128_ps(v256Lo, v256Hi, _MM256_PERMUTE(3, 1));
            const __m256 val2 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 0, 1, 0));
            val1 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 2, 3, 2));
            return SIMD16(_mm256_permute2f128_ps(val2, val1, _MM256_PERMUTE(2, 0)),
                _mm256_permute2f128_ps(val2, val1, _MM256_PERMUTE(3, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val1 = _mm_unpacklo_ps(this->values0, this->values1);
            const __m128 val2 = _mm_unpacklo_ps(this->values2, this->values3);
            const __m128 val3 = _mm_unpackhi_ps(this->values0, this->values1);
            const __m128 val4 = _mm_unpackhi_ps(this->values2, this->values3);
            return SIMD16(_mm_movelh_ps(val1, val2), _mm_movehl_ps(val2, val1), _mm_movelh_ps(val3, val4),
                _mm_movehl_ps(val4, val3));
        } else
#endif
        {
            return SIMD16(this->values0, this->values4, this->values8, this->values12, this->values1, this->values5,
                this->values9, this->values13, this->values2, this->values6, this->values10, this->values14,
                this->values3, this->values7, this->values11, this->values15);
        }
    }

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD2.
     * @tparam Index0 The index in the first object to insert the element (range is 0-2).
     * @tparam Index1 The index in the second object to copy element from (range is 0-2).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD16 insert2(const SIMD16& other) const noexcept
    {
        static_assert(Index0 < 2 && Index1 < 2, "Invalid Index: Indexes must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD16(_mm512_mask_mov_ps(this->values, _cvtu32_mask16(0x5555), other.values));
            } else if constexpr (Index0 == 0 && Index1 == 1) {
                const __m512 ret = _mm512_shuffle_ps(this->values, other.values, _MM_SHUFFLE(3, 1, 3, 1));
                return SIMD16(_mm512_shuffle1_ps(ret, _MM_SHUFFLE(1, 3, 0, 2)));
            } else if constexpr (Index0 == 1 && Index1 == 0) {
                const __m512 ret = _mm512_shuffle_ps(this->values, other.values, _MM_SHUFFLE(2, 0, 2, 0));
                return SIMD16(_mm512_shuffle1_ps(ret, _MM_SHUFFLE(3, 1, 2, 0)));
            } else /*Index0 == 1 && Index1 == 1*/ {
                return SIMD16(_mm512_mask_mov_ps(this->values, _cvtu32_mask16(0xAAAA), other.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD16(_mm256_blend_ps(this->values0, other.values0, _MM256_BLEND(0, 1, 0, 1, 0, 1, 0, 1)),
                    _mm256_blend_ps(this->values1, other.values1, _MM256_BLEND(0, 1, 0, 1, 0, 1, 0, 1)));
            } else if constexpr (Index0 == 0 && Index1 == 1) {
                const __m256 ret = _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(3, 1, 3, 1));
                const __m256 ret2 = _mm256_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(3, 1, 3, 1));
                return SIMD16(_mm256_shuffle1_ps(ret, _MM_SHUFFLE(1, 3, 0, 2)),
                    _mm256_shuffle1_ps(ret2, _MM_SHUFFLE(1, 3, 0, 2)));
            } else if constexpr (Index0 == 1 && Index1 == 0) {
                const __m256 ret = _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(2, 0, 2, 0));
                const __m256 ret2 = _mm256_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(2, 0, 2, 0));
                return SIMD16(_mm256_shuffle1_ps(ret, _MM_SHUFFLE(3, 1, 2, 0)),
                    _mm256_shuffle1_ps(ret2, _MM_SHUFFLE(3, 1, 2, 0)));
            } else /*Index0 == 1 && Index1 == 1*/ {
                return SIMD16(_mm256_blend_ps(this->values0, other.values0, _MM256_BLEND(1, 0, 1, 0, 1, 0, 1, 0)),
                    _mm256_blend_ps(this->values1, other.values1, _MM256_BLEND(1, 0, 1, 0, 1, 0, 1, 0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD16(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(0, 1, 0, 1)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(0, 1, 0, 1)),
                    _mm_blend_ps(this->values2, other.values2, _MM_BLEND(0, 1, 0, 1)),
                    _mm_blend_ps(this->values3, other.values3, _MM_BLEND(0, 1, 0, 1)));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD16(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(1, 0, 1, 0)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(1, 0, 1, 0)),
                    _mm_blend_ps(this->values2, other.values2, _MM_BLEND(1, 0, 1, 0)),
                    _mm_blend_ps(this->values3, other.values3, _MM_BLEND(1, 0, 1, 0)));
            } else {
                const __m128 val0 = _mm_insert_ps(this->values0, other.values0, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                const __m128 val1 = _mm_insert_ps(this->values1, other.values1, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                const __m128 val2 = _mm_insert_ps(this->values2, other.values2, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                const __m128 val3 = _mm_insert_ps(this->values3, other.values3, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                return SIMD16(_mm_insert_ps(val0, other.values0, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)),
                    _mm_insert_ps(val1, other.values1, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)),
                    _mm_insert_ps(val2, other.values2, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)),
                    _mm_insert_ps(val3, other.values3, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)));
            }
        } else
#endif
        {
            return SIMD16(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
                Index0 != 1 ? this->values1 : (&other.values0)[Index1],
                Index0 != 0 ? this->values2 : (&other.values0)[Index1 + 2],
                Index0 != 1 ? this->values3 : (&other.values0)[Index1 + 2],
                Index0 != 0 ? this->values4 : (&other.values0)[Index1 + 4],
                Index0 != 1 ? this->values5 : (&other.values0)[Index1 + 4],
                Index0 != 0 ? this->values6 : (&other.values0)[Index1 + 6],
                Index0 != 1 ? this->values7 : (&other.values0)[Index1 + 6],
                Index0 != 0 ? this->values8 : (&other.values0)[Index1 + 8],
                Index0 != 1 ? this->values9 : (&other.values0)[Index1 + 8],
                Index0 != 0 ? this->values10 : (&other.values0)[Index1 + 10],
                Index0 != 1 ? this->values11 : (&other.values0)[Index1 + 10],
                Index0 != 0 ? this->values12 : (&other.values0)[Index1 + 12],
                Index0 != 1 ? this->values13 : (&other.values0)[Index1 + 12],
                Index0 != 0 ? this->values14 : (&other.values0)[Index1 + 14],
                Index0 != 1 ? this->values15 : (&other.values0)[Index1 + 14]);
        }
    }

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD4.
     * @tparam Index0 The index in the first object to insert the element (range is 0-2).
     * @tparam Index1 The index in the second object to copy element from (range is 0-2).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD16 insert4(const SIMD16& other) const noexcept
    {
        static_assert(Index0 < 4 && Index1 < 4, "Invalid Index: Indexes must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == Index1) {
                return SIMD16(_mm512_mask_mov_ps(this->values, _cvtu32_mask16(0x1111 << Index0), other.values));
            } else {
                return SIMD16(_mm512_mask_shuffle_ps(this->values, _cvtu32_mask16(0x1111 << Index0), other.values,
                    other.values, _MM_SHUFFLE(Index1, Index1, Index1, Index1)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == Index1) {
                return SIMD16(_mm256_blend_ps(this->values0, other.values0, 1 << Index0 | 1 << (Index0 + 4)),
                    _mm256_blend_ps(this->values1, other.values1, 1 << Index0 | 1 << (Index0 + 4)));
            } else if constexpr (Index0 == 0) {
                const __m256 val = _mm256_shuffle_ps(
                    this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 1, 1)); /*(x,Index1,x,1)*/
                const __m256 val1 = _mm256_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 1, 1));
                return SIMD16(_mm256_shuffle_ps(val, this->values0, _MM_SHUFFLE(3, 2, 0, 2)),
                    _mm256_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 0, 2)));
            } else if constexpr (Index0 == 1) {
                const __m256 val = _mm256_shuffle_ps(
                    this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 0, 0)); /*(x,Index1,x,0)*/
                const __m256 val1 = _mm256_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 0, 0));
                return SIMD16(_mm256_shuffle_ps(val, this->values0, _MM_SHUFFLE(3, 2, 2, 0)),
                    _mm256_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 2, 0)));
            } else if constexpr (Index0 == 2) {
                const __m256 val = _mm256_shuffle_ps(
                    this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 3, 3)); /*(x,Index1,x,3)*/
                const __m256 val1 = _mm256_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 3, 3));
                return SIMD16(_mm256_shuffle_ps(this->values0, val, _MM_SHUFFLE(0, 2, 1, 0)),
                    _mm256_shuffle_ps(this->values1, val1, _MM_SHUFFLE(0, 2, 1, 0)));
            } else /*Index0 == 3*/ {
                const __m256 val = _mm256_shuffle_ps(
                    this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 2, 2)); /*(x,Index1,x,2)*/
                const __m256 val1 = _mm256_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 2, 2));
                return SIMD16(_mm256_shuffle_ps(this->values0, val, _MM_SHUFFLE(2, 0, 1, 0)),
                    _mm256_shuffle_ps(this->values1, val1, _MM_SHUFFLE(2, 0, 1, 0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD16(_mm_blend_ss(this->values0, other.values0), _mm_blend_ss(this->values1, other.values1),
                    _mm_blend_ss(this->values2, other.values2), _mm_blend_ss(this->values3, other.values3));
            } else if constexpr (Index0 == Index1) {
                return SIMD16(_mm_blend_ps(this->values0, other.values0, 1UL << Index0),
                    _mm_blend_ps(this->values1, other.values1, 1UL << Index0),
                    _mm_blend_ps(this->values2, other.values2, 1UL << Index0),
                    _mm_blend_ps(this->values3, other.values3, 1UL << Index0));
            } else {
                return SIMD16(_mm_insert_ps(this->values0, other.values0, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)),
                    _mm_insert_ps(this->values1, other.values1, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)),
                    _mm_insert_ps(this->values2, other.values2, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)),
                    _mm_insert_ps(this->values3, other.values3, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)));
            }
        } else
#endif
        {
            return SIMD16(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
                Index0 != 1 ? this->values1 : (&other.values0)[Index1],
                Index0 != 2 ? this->values2 : (&other.values0)[Index1],
                Index0 != 3 ? this->values3 : (&other.values0)[Index1],
                Index0 != 0 ? this->values4 : (&other.values0)[Index1 + 4],
                Index0 != 1 ? this->values5 : (&other.values0)[Index1 + 4],
                Index0 != 2 ? this->values6 : (&other.values0)[Index1 + 4],
                Index0 != 3 ? this->values7 : (&other.values0)[Index1 + 4],
                Index0 != 0 ? this->values8 : (&other.values0)[Index1 + 8],
                Index0 != 1 ? this->values9 : (&other.values0)[Index1 + 8],
                Index0 != 2 ? this->values10 : (&other.values0)[Index1 + 8],
                Index0 != 3 ? this->values11 : (&other.values0)[Index1 + 8],
                Index0 != 0 ? this->values12 : (&other.values0)[Index1 + 12],
                Index0 != 1 ? this->values13 : (&other.values0)[Index1 + 12],
                Index0 != 2 ? this->values14 : (&other.values0)[Index1 + 12],
                Index0 != 3 ? this->values15 : (&other.values0)[Index1 + 12]);
        }
    }

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD8.
     * @tparam Index0 The index in the first object to insert the element (range is 0-2).
     * @tparam Index1 The index in the second object to copy element from (range is 0-2).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD16 insert8(const SIMD16& other) const noexcept
    {
        static_assert(Index0 < 8 && Index1 < 8, "Invalid Index: Indexes must be <8");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == Index1) {
                return SIMD16(_mm512_mask_mov_ps(
                    this->values, _cvtu32_mask16((1UL << Index0) | (1UL << (Index0 + 8))), other.values));
            } else if constexpr ((Index0 / 4) == (Index1 / 4)) {
                return SIMD16(
                    _mm512_mask_permute_ps(this->values, _cvtu32_mask16((1UL << Index0) | (1UL << (Index0 + 8))),
                        other.values, _MM_SHUFFLE(Index1 % 4, Index1 % 4, Index1 % 4, Index1 % 4)));
            } else if constexpr ((Index0 % 4) == (Index1 % 4)) {
                const __m512 val = _mm512_shuffle_f32x4(other.values, other.values, _MM_SHUFFLE(2, 3, 0, 1));
                return SIMD16(
                    _mm512_mask_mov_ps(this->values, _cvtu32_mask16((1UL << Index0) | (1UL << (Index0 + 8))), val));
            } else {
                const __m512 val = _mm512_shuffle_f32x4(other.values, other.values, _MM_SHUFFLE(2, 3, 0, 1));
                return SIMD16(
                    _mm512_mask_permute_ps(this->values, _cvtu32_mask16((1UL << Index0) | (1UL << (Index0 + 8))), val,
                        _MM_SHUFFLE(Index1 % 4, Index1 % 4, Index1 % 4, Index1 % 4)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == Index1) {
                return SIMD16(_mm256_blend_ps(this->values0, other.values0, 1UL << Index0),
                    _mm256_blend_ps(this->values1, other.values1, 1UL << Index0));
            } else if constexpr ((Index0 / 4) == (Index1 / 4)) {
                return SIMD16(
                    _mm256_blend_ps(this->values0,
                        _mm256_shuffle1_ps(other.values0, _MM_SHUFFLE(Index1 % 4, Index1 % 4, Index1 % 4, Index1 % 4)),
                        _MM256_BLEND(Index0 == 7, Index0 == 6, Index0 == 5, Index0 == 4, Index0 == 3, Index0 == 2,
                            Index0 == 1, Index0 == 0)),
                    _mm256_blend_ps(this->values1,
                        _mm256_shuffle1_ps(other.values1, _MM_SHUFFLE(Index1 % 4, Index1 % 4, Index1 % 4, Index1 % 4)),
                        _MM256_BLEND(Index0 == 7, Index0 == 6, Index0 == 5, Index0 == 4, Index0 == 3, Index0 == 2,
                            Index0 == 1, Index0 == 0)));
            } else if constexpr ((Index0 % 4) == (Index1 % 4)) {
                const __m256 val = _mm256_shuffle32107654_ps(other.values0);
                const __m256 val1 = _mm256_shuffle32107654_ps(other.values1);
                return SIMD16(_mm256_blend_ps(this->values0, val,
                                  _MM256_BLEND(Index0 == 7, Index0 == 6, Index0 == 5, Index0 == 4, Index0 == 3,
                                      Index0 == 2, Index0 == 1, Index0 == 0)),
                    _mm256_blend_ps(this->values1, val1,
                        _MM256_BLEND(Index0 == 7, Index0 == 6, Index0 == 5, Index0 == 4, Index0 == 3, Index0 == 2,
                            Index0 == 1, Index0 == 0)));
            } else {
                const __m256 val = _mm256_shuffle32107654_ps(other.values0);
                const __m256 val1 = _mm256_shuffle32107654_ps(other.values1);
                return SIMD16(_mm256_blend_ps(this->values0,
                                  _mm256_shuffle1_ps(val, _MM_SHUFFLE(Index1 % 4, Index1 % 4, Index1 % 4, Index1 % 4)),
                                  _MM256_BLEND(Index0 == 7, Index0 == 6, Index0 == 5, Index0 == 4, Index0 == 3,
                                      Index0 == 2, Index0 == 1, Index0 == 0)),
                    _mm256_blend_ps(this->values1,
                        _mm256_shuffle1_ps(val1, _MM_SHUFFLE(Index1 % 4, Index1 % 4, Index1 % 4, Index1 % 4)),
                        _MM256_BLEND(Index0 == 7, Index0 == 6, Index0 == 5, Index0 == 4, Index0 == 3, Index0 == 2,
                            Index0 == 1, Index0 == 0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 half0 = (&this->values0)[Index0 / 4];
            const __m128 half1 = (&other.values0)[Index1 / 4];
            __m128 half2 = (&this->values0)[Index0 / 4 + 2];
            const __m128 half3 = (&other.values0)[Index1 / 4 + 2];
            constexpr uint32 index0 = Index0 % 4;
            constexpr uint32 index1 = Index1 % 4;
            if constexpr (index0 == 0 && index1 == 0) {
                half0 = _mm_blend_ss(half0, half1);
                half2 = _mm_blend_ss(half2, half3);
            } else if constexpr (index0 == index1) {
                half0 = _mm_blend_ps(half0, half1, 1UL << index0);
                half2 = _mm_blend_ps(half2, half3, 1UL << index0);
            } else {
                half0 = _mm_insert_ps(half0, half1, _MM_MK_INSERTPS_NDX(index1, index0, 0));
                half2 = _mm_insert_ps(half2, half3, _MM_MK_INSERTPS_NDX(index1, index0, 0));
            }

            if constexpr (Index0 / 4 == 0) {
                return SIMD16(half0, this->values1, half2, this->values3);
            } else {
                return SIMD16(this->values0, half0, this->values2, half2);
            }
        } else
#endif
        {
            return SIMD16(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
                Index0 != 1 ? this->values1 : (&other.values0)[Index1],
                Index0 != 2 ? this->values2 : (&other.values0)[Index1],
                Index0 != 3 ? this->values3 : (&other.values0)[Index1],
                Index0 != 4 ? this->values4 : (&other.values0)[Index1],
                Index0 != 5 ? this->values5 : (&other.values0)[Index1],
                Index0 != 6 ? this->values6 : (&other.values0)[Index1],
                Index0 != 7 ? this->values7 : (&other.values0)[Index1],
                Index0 != 0 ? this->values8 : (&other.values0)[Index1 + 8],
                Index0 != 1 ? this->values9 : (&other.values0)[Index1 + 8],
                Index0 != 2 ? this->values10 : (&other.values0)[Index1 + 8],
                Index0 != 3 ? this->values11 : (&other.values0)[Index1 + 8],
                Index0 != 4 ? this->values12 : (&other.values0)[Index1 + 8],
                Index0 != 5 ? this->values13 : (&other.values0)[Index1 + 8],
                Index0 != 6 ? this->values14 : (&other.values0)[Index1 + 8],
                Index0 != 7 ? this->values15 : (&other.values0)[Index1 + 8]);
        }
    }

    /**
     * Blends values from the second object into the first. This performs the same operation on each SIMD2.
     * @note A value form the second SIMD16 is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1>
    XS_FUNCTION SIMD16 blend2(const SIMD16& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1) {
            return *this;
        } else if constexpr (Elem0 && Elem1) {
            return other;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD16(
                _mm512_mask_mov_ps(this->values, _cvtu32_mask16(((Elem1 << 1) | Elem0) * 0x5555), other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD16(_mm256_blend_ps(this->values0, other.values0,
                              _MM256_BLEND(Elem1, Elem0, Elem1, Elem0, Elem1, Elem0, Elem1, Elem0)),
                _mm256_blend_ps(this->values1, other.values1,
                    _MM256_BLEND(Elem1, Elem0, Elem1, Elem0, Elem1, Elem0, Elem1, Elem0)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD16(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)),
                _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)),
                _mm_blend_ps(this->values2, other.values2, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)),
                _mm_blend_ps(this->values3, other.values3, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)));
        }
#endif
        else {
            return SIMD16(!Elem0 ? this->values0 : other.values0, !Elem1 ? this->values1 : other.values1,
                !Elem0 ? this->values2 : other.values2, !Elem1 ? this->values3 : other.values3,
                !Elem0 ? this->values4 : other.values4, !Elem1 ? this->values5 : other.values5,
                !Elem0 ? this->values6 : other.values6, !Elem1 ? this->values7 : other.values7,
                !Elem0 ? this->values8 : other.values8, !Elem1 ? this->values9 : other.values9,
                !Elem0 ? this->values10 : other.values10, !Elem1 ? this->values11 : other.values11,
                !Elem0 ? this->values12 : other.values12, !Elem1 ? this->values13 : other.values13,
                !Elem0 ? this->values14 : other.values14, !Elem1 ? this->values15 : other.values15);
        }
    }

    /**
     * Blends values from the second object into the first. This performs the same operation on each SIMD4.
     * @note A value form the second SIMD16 is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @tparam Elem2 Whether to copy the third element of the first input into the returned object.
     * @tparam Elem3 Whether to copy the fourth element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3>
    XS_FUNCTION SIMD16 blend4(const SIMD16& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
            return *this;
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
            return other;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD16(_mm512_mask_mov_ps(this->values,
                _cvtu32_mask16((Elem0 * 0x1111) | (Elem1 * 0x2222) | (Elem2 * 0x4444) | (Elem3 * 0x8888)),
                other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD16(_mm256_blend_ps(this->values0, other.values0,
                              _MM256_BLEND(Elem3, Elem2, Elem1, Elem0, Elem3, Elem2, Elem1, Elem0)),
                _mm256_blend_ps(this->values1, other.values1,
                    _MM256_BLEND(Elem3, Elem2, Elem1, Elem0, Elem3, Elem2, Elem1, Elem0)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
                return SIMD16(_mm_blend_ss(this->values0, other.values0), _mm_blend_ss(this->values1, other.values1),
                    _mm_blend_ss(this->values2, other.values2), _mm_blend_ss(this->values3, other.values3));
            } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
                return SIMD16(_mm_blend_ss(other.values0, this->values0), _mm_blend_ss(other.values1, this->values1),
                    _mm_blend_ss(other.values2, this->values2), _mm_blend_ss(other.values3, this->values3));
            } else {
                return SIMD16(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)),
                    _mm_blend_ps(this->values2, other.values2, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)),
                    _mm_blend_ps(this->values3, other.values3, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)));
            }
        }
#endif
        else {
            return SIMD16(!Elem0 ? this->values0 : other.values0, !Elem1 ? this->values1 : other.values1,
                !Elem2 ? this->values2 : other.values2, !Elem3 ? this->values3 : other.values3,
                !Elem0 ? this->values4 : other.values4, !Elem1 ? this->values5 : other.values5,
                !Elem2 ? this->values6 : other.values6, !Elem3 ? this->values7 : other.values7,
                !Elem0 ? this->values8 : other.values8, !Elem1 ? this->values9 : other.values9,
                !Elem2 ? this->values10 : other.values10, !Elem3 ? this->values11 : other.values11,
                !Elem0 ? this->values12 : other.values12, !Elem1 ? this->values13 : other.values13,
                !Elem2 ? this->values14 : other.values14, !Elem3 ? this->values15 : other.values15);
        }
    }

    /**
     * Blends values from the second object into the first. This performs the same operation on each SIMD8.
     * @note A value form the second SIMD16 is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @tparam Elem2 Whether to copy the third element of the first input into the returned object.
     * @tparam Elem3 Whether to copy the fourth element of the first input into the returned object.
     * @tparam Elem4 Whether to copy the fifth element of the first input into the returned object.
     * @tparam Elem5 Whether to copy the sixth element of the first input into the returned object.
     * @tparam Elem6 Whether to copy the seventh element of the first input into the returned object.
     * @tparam Elem7 Whether to copy the eighth element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7>
    XS_FUNCTION SIMD16 blend8(const SIMD16& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7) {
            return *this;
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7) {
            return other;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD16(_mm512_mask_mov_ps(this->values,
                _cvtu32_mask16((Elem0 * 0x101) | (Elem1 * 0x202) | (Elem2 * 0x404) | (Elem3 * 0x808) |
                    (Elem4 * 0x1010) | (Elem5 * 0x2020) | (Elem6 * 0x4040) | (Elem7 * 0x8080)),
                other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD16(_mm256_blend_ps(this->values0, other.values0,
                              _MM256_BLEND(Elem7, Elem6, Elem5, Elem4, Elem3, Elem2, Elem1, Elem0)),
                _mm256_blend_ps(this->values1, other.values1,
                    _MM256_BLEND(Elem7, Elem6, Elem5, Elem4, Elem3, Elem2, Elem1, Elem0)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 ret0, ret1, ret2, ret3;
            if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
                ret0 = other.values0;
                ret2 = other.values2;
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
                ret0 = this->values0;
                ret2 = this->values2;
            } else if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
                ret0 = _mm_blend_ss(this->values0, other.values0);
                ret2 = _mm_blend_ss(this->values2, other.values2);
            } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
                ret0 = _mm_blend_ss(other.values0, this->values0);
                ret2 = _mm_blend_ss(other.values2, this->values2);
            } else {
                ret0 = _mm_blend_ps(this->values0, other.values0, _MM_BLEND(Elem3, Elem2, Elem1, Elem0));
                ret2 = _mm_blend_ps(this->values2, other.values2, _MM_BLEND(Elem3, Elem2, Elem1, Elem0));
            }
            if constexpr (Elem4 && Elem5 && Elem6 && Elem7) {
                ret1 = other.values1;
                ret3 = other.values3;
            } else if constexpr (!Elem4 && !Elem5 && !Elem6 && !Elem7) {
                ret1 = this->values1;
                ret3 = this->values3;
            } else if constexpr (Elem4 && !Elem5 && !Elem6 && !Elem7) {
                ret1 = _mm_blend_ss(this->values1, other.values1);
                ret3 = _mm_blend_ss(this->values3, other.values3);
            } else if constexpr (!Elem4 && Elem5 && Elem6 && Elem7) {
                ret1 = _mm_blend_ss(other.values1, this->values1);
                ret3 = _mm_blend_ss(other.values3, this->values3);
            } else {
                ret1 = _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem7, Elem6, Elem5, Elem4));
                ret3 = _mm_blend_ps(this->values3, other.values3, _MM_BLEND(Elem7, Elem6, Elem5, Elem4));
            }
            return SIMD16(ret0, ret1, ret2, ret3);
        }
#endif
        else {
            return SIMD16((Elem0) ? other.values0 : this->values0, (Elem1) ? other.values1 : this->values1,
                (Elem2) ? other.values2 : this->values2, (Elem3) ? other.values3 : this->values3,
                (Elem4) ? other.values4 : this->values4, (Elem5) ? other.values5 : this->values5,
                (Elem6) ? other.values6 : this->values6, (Elem7) ? other.values7 : this->values7,
                (Elem0) ? other.values8 : this->values8, (Elem1) ? other.values9 : this->values9,
                (Elem2) ? other.values10 : this->values10, (Elem3) ? other.values11 : this->values11,
                (Elem4) ? other.values12 : this->values12, (Elem5) ? other.values13 : this->values13,
                (Elem6) ? other.values14 : this->values14, (Elem7) ? other.values15 : this->values15);
        }
    }

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD2.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-1).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-1).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD16 shuffle2() const noexcept
    {
        static_assert(Index0 < 2 && Index1 < 2, "Invalid Index: Indexes must be <2");
        if constexpr (Index0 == 0 && Index1 == 1) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD16(_mm512_shuffle2200_ps(this->values));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD16(_mm512_shuffle3311_ps(this->values));
            } else {
                return SIMD16(_mm512_shuffle1_ps(this->values, _MM_SHUFFLE(Index1 + 2, Index0 + 2, Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD16(_mm256_shuffle2200_ps(this->values0), _mm256_shuffle2200_ps(this->values1));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD16(_mm256_shuffle3311_ps(this->values0), _mm256_shuffle3311_ps(this->values1));
            } else {
                return SIMD16(_mm256_shuffle1_ps(this->values0, _MM_SHUFFLE(Index1 + 2, Index0 + 2, Index1, Index0)),
                    _mm256_shuffle1_ps(this->values1, _MM_SHUFFLE(Index1 + 2, Index0 + 2, Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD16(_mm_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1),
                    _mm_shuffle2200_ps(this->values2), _mm_shuffle2200_ps(this->values3));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD16(_mm_shuffle3311_ps(this->values0), _mm_shuffle3311_ps(this->values1),
                    _mm_shuffle3311_ps(this->values2), _mm_shuffle3311_ps(this->values3));
            } else /*Index0 == 1 && Index1 == 0*/ {
                return SIMD16(_mm_shuffle1_ps(this->values0, _MM_SHUFFLE(2, 3, 0, 1)),
                    _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(2, 3, 0, 1)),
                    _mm_shuffle1_ps(this->values2, _MM_SHUFFLE(2, 3, 0, 1)),
                    _mm_shuffle1_ps(this->values3, _MM_SHUFFLE(2, 3, 0, 1)));
            }
        }
#endif
        else {
            return SIMD16((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index0 + 2],
                (&this->values0)[Index1 + 2], (&this->values0)[Index0 + 4], (&this->values0)[Index1 + 4],
                (&this->values0)[Index0 + 6], (&this->values0)[Index1 + 6], (&this->values0)[Index0 + 8],
                (&this->values0)[Index1 + 8], (&this->values0)[Index0 + 10], (&this->values0)[Index1 + 10],
                (&this->values0)[Index0 + 12], (&this->values0)[Index1 + 12], (&this->values0)[Index0 + 14],
                (&this->values0)[Index1 + 14]);
        }
    }

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD4.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-4).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-4).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-4).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-4).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
    XS_FUNCTION SIMD16 shuffle4() const noexcept
    {
        static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4, "Invalid Index: Indexes must be <4");
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
                return SIMD16<T, Width>(_mm512_shuffle2200_ps(this->values));
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3 && Index3 == 3) {
                return SIMD16<T, Width>(_mm512_shuffle3311_ps(this->values));
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3) {
                return SIMD16<T, Width>(_mm512_shuffle3322_ps(this->values));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
                return SIMD16<T, Width>(_mm512_shuffle1100_ps(this->values));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
                return SIMD16<T, Width>(_mm512_shuffle1010_ps(this->values));
            } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2 && Index3 == 3) {
                return SIMD16<T, Width>(_mm512_shuffle3232_ps(this->values));
            } else {
                return SIMD16<T, Width>(_mm512_shuffle1_ps(this->values, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3 && Index3 == 3) {
                return SIMD16(_mm256_shuffle3311_ps(this->values0), _mm256_shuffle3311_ps(this->values1));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
                return SIMD16(_mm256_shuffle2200_ps(this->values0), _mm256_shuffle2200_ps(this->values1));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
                return SIMD16(_mm256_shuffle1100_ps(this->values0), _mm256_shuffle1100_ps(this->values1));
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3) {
                return SIMD16(_mm256_shuffle3322_ps(this->values0), _mm256_shuffle3322_ps(this->values1));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
                return SIMD16(_mm256_shuffle1010_ps(this->values0), _mm256_shuffle1010_ps(this->values1));
            } else {
                return SIMD16(_mm256_shuffle1_ps(this->values0, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                    _mm256_shuffle1_ps(this->values1, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
                return SIMD16(_mm_shuffle1010_ps(this->values0), _mm_shuffle1010_ps(this->values1),
                    _mm_shuffle1010_ps(this->values2), _mm_shuffle1010_ps(this->values3));
            } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2 && Index3 == 3) {
                return SIMD16(_mm_shuffle3232_ps(this->values0), _mm_shuffle3232_ps(this->values1),
                    _mm_shuffle3232_ps(this->values2), _mm_shuffle3232_ps(this->values3));
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3) {
                return SIMD16(_mm_shuffle3322_ps(this->values0), _mm_shuffle3322_ps(this->values1),
                    _mm_shuffle3322_ps(this->values2), _mm_shuffle3322_ps(this->values3));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
                return SIMD16(_mm_shuffle1100_ps(this->values0), _mm_shuffle1100_ps(this->values1),
                    _mm_shuffle1100_ps(this->values2), _mm_shuffle1100_ps(this->values3));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
                return SIMD16(_mm_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1),
                    _mm_shuffle2200_ps(this->values2), _mm_shuffle2200_ps(this->values3));
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3 && Index3 == 3) {
                return SIMD16(_mm_shuffle3311_ps(this->values0), _mm_shuffle3311_ps(this->values1),
                    _mm_shuffle3311_ps(this->values2), _mm_shuffle3311_ps(this->values3));
            } else {
                return SIMD16(_mm_shuffle1_ps(this->values0, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                    _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                    _mm_shuffle1_ps(this->values2, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                    _mm_shuffle1_ps(this->values3, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
            }
        }
#endif
        else {
            return SIMD16((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2],
                (&this->values0)[Index3], (&this->values0)[4 + Index0], (&this->values0)[4 + Index1],
                (&this->values0)[4 + Index2], (&this->values0)[4 + Index3], (&this->values0)[8 + Index0],
                (&this->values0)[8 + Index1], (&this->values0)[8 + Index2], (&this->values0)[8 + Index3],
                (&this->values0)[12 + Index0], (&this->values0)[12 + Index1], (&this->values0)[12 + Index2],
                (&this->values0)[12 + Index3]);
        }
    }

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD8.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-7).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-7).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-7).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-7).
     * @tparam Index4 The index of the fifth element to insert into the returned object (range is 0-7).
     * @tparam Index5 The index of the sixth element to insert into the returned object (range is 0-7).
     * @tparam Index6 The index of the seventh element to insert into the returned object (range is 0-7).
     * @tparam Index7 The index of the eighth element to insert into the returned object (range is 0-7).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3, uint32 Index4, uint32 Index5, uint32 Index6,
        uint32 Index7>
    XS_FUNCTION SIMD16 shuffle8() const noexcept
    {
        static_assert(Index0 < 8 && Index1 < 8 && Index2 < 8 && Index3 < 8 && Index4 < 8 && Index5 < 8 && Index6 < 8 &&
                Index7 < 8,
            "Invalid Index: Indexes must be <8");
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3 && Index4 == 4 && Index5 == 5 &&
            Index6 == 6 && Index7 == 7) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4 && Index4 >= 4 && Index5 >= 4 &&
                Index6 >= 4 && Index7 >= 4 && (Index0 % 4) == (Index4 % 4) && (Index1 % 4) == (Index5 % 4) &&
                (Index2 % 4) == (Index6 % 4) && (Index3 % 4) == (Index7 % 4)) {
                return SIMD16(
                    _mm512_shuffle1_ps(this->values, _MM_SHUFFLE(Index3 % 4, Index2 % 4, Index1 % 4, Index0 % 4)));
            } else if constexpr (Index0 == 4 && Index1 == 5 && Index2 == 6 && Index3 == 7 && Index4 == 0 &&
                Index5 == 1 && Index6 == 2 && Index7 == 3) {
                return SIMD16(_mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 3, 0, 1)));
            } else if constexpr (Index0 >= 4 && Index1 >= 4 && Index2 >= 4 && Index3 >= 4 && Index4 < 4 && Index5 < 4 &&
                Index6 < 4 && Index7 < 4 && (Index0 % 4) == (Index4 % 4) && (Index1 % 4) == (Index5 % 4) &&
                (Index2 % 4) == (Index6 % 4) && (Index3 % 4) == (Index7 % 4)) {
                const __m512 val = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 3, 0, 1));
                return SIMD16(_mm512_shuffle1_ps(val, _MM_SHUFFLE(Index3 % 4, Index2 % 4, Index1 % 4, Index0 % 4)));
            } else {
                return SIMD16(_mm512_permutexvar_ps(
                    _mm512_set_epi32(Index7 + 8, Index6 + 8, Index5 + 8, Index4 + 8, Index3 + 8, Index2 + 8, Index1 + 8,
                        Index0 + 8, Index7, Index6, Index5, Index4, Index3, Index2, Index1, Index0),
                    this->values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD16(SIMD8Def(this->values0)
                              .template shuffle<Index0, Index1, Index2, Index3, Index4, Index5, Index6, Index7>()
                              .values,
                SIMD8Def(this->values1)
                    .template shuffle<Index0, Index1, Index2, Index3, Index4, Index5, Index6, Index7>()
                    .values);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr ((Index0 == Index4) && (Index1 == Index5) && (Index2 == Index6) && (Index3 == Index7)) {
                const __m128 ret = SIMD4Def(this->values0)
                                       .template combine<Index0, Index1, Index2, Index3>(SIMD4Def(this->values1))
                                       .values;
                const __m128 ret1 = SIMD4Def(this->values2)
                                        .template combine<Index0, Index1, Index2, Index3>(SIMD4Def(this->values3))
                                        .values;
                return SIMD16(ret, ret, ret1, ret1);
            } else {
                const SIMD4Def low(this->values0);
                const SIMD4Def high(this->values1);
                const SIMD4Def low1(this->values2);
                const SIMD4Def high1(this->values3);
                return SIMD16(low.template combine<Index0, Index1, Index2, Index3>(high).values,
                    low.template combine<Index4, Index5, Index6, Index7>(high).values,
                    low1.template combine<Index0, Index1, Index2, Index3>(high1).values,
                    low1.template combine<Index4, Index5, Index6, Index7>(high1).values);
            }
        }
#endif
        else {
            return SIMD16((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2],
                (&this->values0)[Index3], (&this->values0)[Index4], (&this->values0)[Index5], (&this->values0)[Index6],
                (&this->values0)[Index7], (&this->values0)[Index0 + 8], (&this->values0)[Index1 + 8],
                (&this->values0)[Index2 + 8], (&this->values0)[Index3 + 8], (&this->values0)[Index4 + 8],
                (&this->values0)[Index5 + 8], (&this->values0)[Index6 + 8], (&this->values0)[Index7 + 8]);
        }
    }

    /**
     * Shuffles the elements of the object by shuffling each SIMD4.
     * @tparam Index0 The index of the first SIMD4 to insert into the returned object (range is 0-3).
     * @tparam Index1 The index of the second SIMD4 to insert into the returned object (range is 0-3).
     * @tparam Index2 The index of the third SIMD4 to insert into the returned object (range is 0-3).
     * @tparam Index3 The index of the fourth SIMD4 to insert into the returned object (range is 0-3).
     * @returns Result of operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
    XS_FUNCTION SIMD16 shuffleH4() const noexcept
    {
        static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4, "Invalid Index: Indexes must be <4");
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD16(
                _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == Index2 && Index1 == Index3 && Index0 < 2 && Index1 < 2) {
                const __m256 val = _mm256_permute2f128_ps(this->values0, this->values0, _MM256_PERMUTE(Index1, Index0));
                return SIMD16(val, val);
            } else if constexpr (Index0 == Index2 && Index1 == Index3 && Index0 >= 2 && Index1 >= 2) {
                const __m256 val =
                    _mm256_permute2f128_ps(this->values1, this->values1, _MM256_PERMUTE(Index1 - 2, Index0 - 2));
                return SIMD16(val, val);
            } else if constexpr (Index0 == Index2 && Index1 == Index3) {
                const __m256 val = _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(Index1, Index0));
                return SIMD16(val, val);
            } else {
                return SIMD16(_mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(Index1, Index0)),
                    _mm256_permute2f128_ps(this->values0, this->values1, _MM256_PERMUTE(Index3, Index2)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD16(
                (&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2], (&this->values0)[Index3]);
        }
#endif
        else {
            return SIMD16((&this->values0)[Index0 * 4], (&this->values0)[Index0 * 4 + 1],
                (&this->values0)[Index0 * 4 + 2], (&this->values0)[Index0 * 4 + 3], (&this->values0)[Index1 * 4],
                (&this->values0)[Index1 * 4 + 1], (&this->values0)[Index1 * 4 + 2], (&this->values0)[Index1 * 4 + 3],
                (&this->values0)[Index2 * 4], (&this->values0)[Index2 * 4 + 1], (&this->values0)[Index2 * 4 + 2],
                (&this->values0)[Index2 * 4 + 3], (&this->values0)[Index3 * 4], (&this->values0)[Index3 * 4 + 1],
                (&this->values0)[Index3 * 4 + 2], (&this->values0)[Index3 * 4 + 3]);
        }
    }
};

/**
 * Add two SIMD16s.
 * @param other1 The first SIMD16.
 * @param other2 SIMD16 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator+(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_add_ps(other1.values0, other2.values0), _mm256_add_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_add_ps(other1.values0, other2.values0), _mm_add_ps(other1.values1, other2.values1),
            _mm_add_ps(other1.values2, other2.values2), _mm_add_ps(other1.values3, other2.values3));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values3, other1.values4 + other2.values4,
            other1.values5 + other2.values5, other1.values6 + other2.values6, other1.values7 + other2.values7,
            other1.values8 + other2.values8, other1.values9 + other2.values9, other1.values10 + other2.values10,
            other1.values11 + other2.values11, other1.values12 + other2.values12, other1.values13 + other2.values13,
            other1.values14 + other2.values14, other1.values15 + other2.values15);
    }
}

/**
 * Add a value to all elements of a SIMD16.
 * @param other1 The SIMD16.
 * @param other2 Value to add to the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator+(
    const SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_add_ps(other1.values0, other2.values), _mm256_add_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values),
            _mm_add_ps(other1.values2, other2.values), _mm_add_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 + other2.value, other1.values1 + other2.value,
            other1.values2 + other2.value, other1.values3 + other2.value, other1.values4 + other2.value,
            other1.values5 + other2.value, other1.values6 + other2.value, other1.values7 + other2.value,
            other1.values8 + other2.value, other1.values9 + other2.value, other1.values10 + other2.value,
            other1.values11 + other2.value, other1.values12 + other2.value, other1.values13 + other2.value,
            other1.values14 + other2.value, other1.values15 + other2.value);
    }
}

/**
 * Add a SIMD4 to each 4 elements of a SIMD16.
 * @param other1 The SIMD16.
 * @param other2 Value to add to the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator+(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_add_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other2.values);
        return SIMD16<T, Width>(_mm256_add_ps(other1.values0, val), _mm256_add_ps(other1.values1, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values),
            _mm_add_ps(other1.values2, other2.values), _mm_add_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values3, other1.values4 + other2.values0,
            other1.values5 + other2.values1, other1.values6 + other2.values2, other1.values7 + other2.values3,
            other1.values8 + other2.values0, other1.values9 + other2.values1, other1.values10 + other2.values2,
            other1.values11 + other2.values3, other1.values12 + other2.values0, other1.values13 + other2.values1,
            other1.values14 + other2.values2, other1.values15 + other2.values3);
    }
}

/**
 * Add a SIMD8 to each 8 elements of a SIMD16.
 * @param other1 The SIMD16.
 * @param other2 Value to add to the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator+(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD8Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_add_ps(other1.values, _mm512_broadcastf256_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_add_ps(other1.values0, other2.values), _mm256_add_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_add_ps(other1.values0, other2.values0), _mm_add_ps(other1.values1, other2.values1),
            _mm_add_ps(other1.values2, other2.values0), _mm_add_ps(other1.values3, other2.values1));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values3, other1.values4 + other2.values4,
            other1.values5 + other2.values5, other1.values6 + other2.values6, other1.values7 + other2.values7,
            other1.values8 + other2.values0, other1.values9 + other2.values1, other1.values10 + other2.values2,
            other1.values11 + other2.values3, other1.values12 + other2.values4, other1.values13 + other2.values5,
            other1.values14 + other2.values6, other1.values15 + other2.values7);
    }
}

/**
 * Subtract a SIMD16 from another SIMD16.
 * @param other1 The first SIMD16.
 * @param other2 SIMD16 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator-(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_sub_ps(other1.values0, other2.values0), _mm256_sub_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_sub_ps(other1.values0, other2.values0), _mm_sub_ps(other1.values1, other2.values1),
            _mm_sub_ps(other1.values2, other2.values2), _mm_sub_ps(other1.values3, other2.values3));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values3, other1.values4 - other2.values4,
            other1.values5 - other2.values5, other1.values6 - other2.values6, other1.values7 - other2.values7,
            other1.values8 - other2.values8, other1.values9 - other2.values9, other1.values10 - other2.values10,
            other1.values11 - other2.values11, other1.values12 - other2.values12, other1.values13 - other2.values13,
            other1.values14 - other2.values14, other1.values15 - other2.values15);
    }
}

/**
 * Subtract a value from all elements of a SIMD16.
 * @param other1 The SIMD16.
 * @param other2 Value to subtract from the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator-(
    const SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_sub_ps(other1.values0, other2.values), _mm256_sub_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values),
            _mm_sub_ps(other1.values2, other2.values), _mm_sub_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 - other2.value, other1.values1 - other2.value,
            other1.values2 - other2.value, other1.values3 - other2.value, other1.values4 - other2.value,
            other1.values5 - other2.value, other1.values6 - other2.value, other1.values7 - other2.value,
            other1.values8 - other2.value, other1.values9 - other2.value, other1.values10 - other2.value,
            other1.values11 - other2.value, other1.values12 - other2.value, other1.values13 - other2.value,
            other1.values14 - other2.value, other1.values15 - other2.value);
    }
}

/**
 * Subtract all elements of a SIMD16 from a value.
 * @param other1 Value to subtract from the SIMD16.
 * @param other2 The SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator-(
    typename SIMD16<T, Width>::BaseDef other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_sub_ps(other1.values, other2.values0), _mm256_sub_ps(other1.values, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_sub_ps(other1.values, other2.values0), _mm_sub_ps(other1.values, other2.values1),
            _mm_sub_ps(other1.values, other2.values2), _mm_sub_ps(other1.values, other2.values3));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.value - other2.values0, other1.value - other2.values1,
            other1.value - other2.values2, other1.value - other2.values3, other1.value - other2.values4,
            other1.value - other2.values5, other1.value - other2.values6, other1.value - other2.values7,
            other1.value - other2.values8, other1.value - other2.values9, other1.value - other2.values10,
            other1.value - other2.values11, other1.value - other2.values12, other1.value - other2.values13,
            other1.value - other2.values14, other1.value - other2.values15);
    }
}

/**
 * Subtract a SIMD4 from each 4 elements of a SIMD16.
 * @param other1 The SIMD16.
 * @param other2 Value to subtract from the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator-(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_sub_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other2.values);
        return SIMD16<T, Width>(_mm256_sub_ps(other1.values0, val), _mm256_sub_ps(other1.values1, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values),
            _mm_sub_ps(other1.values2, other2.values), _mm_sub_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values3, other1.values4 - other2.values0,
            other1.values5 - other2.values1, other1.values6 - other2.values2, other1.values7 - other2.values3,
            other1.values8 - other2.values0, other1.values9 - other2.values1, other1.values10 - other2.values2,
            other1.values11 - other2.values3, other1.values12 - other2.values0, other1.values13 - other2.values1,
            other1.values14 - other2.values2, other1.values15 - other2.values3);
    }
}

/**
 * Subtract a SIMD8 from each 8 elements of a SIMD16.
 * @param other1 The SIMD16.
 * @param other2 Value to subtract from the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator-(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD8Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_sub_ps(other1.values, _mm512_broadcastf256_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_sub_ps(other1.values0, other2.values), _mm256_sub_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_sub_ps(other1.values0, other2.values0), _mm_sub_ps(other1.values1, other2.values1),
            _mm_sub_ps(other1.values2, other2.values0), _mm_sub_ps(other1.values3, other2.values1));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values3, other1.values4 - other2.values4,
            other1.values5 - other2.values5, other1.values6 - other2.values6, other1.values7 - other2.values7,
            other1.values8 - other2.values0, other1.values9 - other2.values1, other1.values10 - other2.values2,
            other1.values11 - other2.values3, other1.values12 - other2.values4, other1.values13 - other2.values5,
            other1.values14 - other2.values6, other1.values15 - other2.values7);
    }
}

/**
 * Multiply two SIMD16s per element.
 * @param other1 The first SIMD16.
 * @param other2 The second SIMD16 to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator*(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_mul_ps(other1.values0, other2.values0), _mm256_mul_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_mul_ps(other1.values0, other2.values0), _mm_mul_ps(other1.values1, other2.values1),
            _mm_mul_ps(other1.values2, other2.values2), _mm_mul_ps(other1.values3, other2.values3));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values3, other1.values4 * other2.values4,
            other1.values5 * other2.values5, other1.values6 * other2.values6, other1.values7 * other2.values7,
            other1.values8 * other2.values8, other1.values9 * other2.values9, other1.values10 * other2.values10,
            other1.values11 * other2.values11, other1.values12 * other2.values12, other1.values13 * other2.values13,
            other1.values14 * other2.values14, other1.values15 * other2.values15);
    }
}

/**
 * Multiply a SIMD16 by a scalar.
 * @param other1 The SIMD16.
 * @param other2 Value to multiply the SIMD16 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator*(
    const SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_mul_ps(other1.values0, other2.values), _mm256_mul_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values),
            _mm_mul_ps(other1.values2, other2.values), _mm_mul_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 * other2.value, other1.values1 * other2.value,
            other1.values2 * other2.value, other1.values3 * other2.value, other1.values4 * other2.value,
            other1.values5 * other2.value, other1.values6 * other2.value, other1.values7 * other2.value,
            other1.values8 * other2.value, other1.values9 * other2.value, other1.values10 * other2.value,
            other1.values11 * other2.value, other1.values12 * other2.value, other1.values13 * other2.value,
            other1.values14 * other2.value, other1.values15 * other2.value);
    }
}

/**
 * Multiply each 4 elements of a SIMD16 by a SIMD4.
 * @param other1 The SIMD16.
 * @param other2 Value to multiply the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator*(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_mul_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other2.values);
        return SIMD16<T, Width>(_mm256_mul_ps(other1.values0, val), _mm256_mul_ps(other1.values1, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values),
            _mm_mul_ps(other1.values2, other2.values), _mm_mul_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values3, other1.values4 * other2.values0,
            other1.values5 * other2.values1, other1.values6 * other2.values2, other1.values7 * other2.values3,
            other1.values8 * other2.values0, other1.values9 * other2.values1, other1.values10 * other2.values2,
            other1.values11 * other2.values3, other1.values12 * other2.values0, other1.values13 * other2.values1,
            other1.values14 * other2.values2, other1.values15 * other2.values3);
    }
}

/**
 * Multiply each 8 elements of a SIMD16 by a SIMD8.
 * @param other1 The SIMD16.
 * @param other2 Value to multiply the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator*(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD8Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_mul_ps(other1.values, _mm512_broadcastf256_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_mul_ps(other1.values0, other2.values), _mm256_mul_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_mul_ps(other1.values0, other2.values0), _mm_mul_ps(other1.values1, other2.values1),
            _mm_mul_ps(other1.values2, other2.values0), _mm_mul_ps(other1.values3, other2.values1));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values3, other1.values4 * other2.values4,
            other1.values5 * other2.values5, other1.values6 * other2.values6, other1.values7 * other2.values7,
            other1.values8 * other2.values0, other1.values9 * other2.values1, other1.values10 * other2.values2,
            other1.values11 * other2.values3, other1.values12 * other2.values4, other1.values13 * other2.values5,
            other1.values14 * other2.values6, other1.values15 * other2.values7);
    }
}

/**
 * Divide two SIMD16s per element.
 * @param other1 The first SIMD16.
 * @param other2 The second SIMD16 to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator/(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_div_ps(other1.values0, other2.values0), _mm256_div_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_div_ps(other1.values0, other2.values0), _mm_div_ps(other1.values1, other2.values1),
            _mm_div_ps(other1.values2, other2.values2), _mm_div_ps(other1.values3, other2.values3));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values3, other1.values4 / other2.values4,
            other1.values5 / other2.values5, other1.values6 / other2.values6, other1.values7 / other2.values7,
            other1.values8 / other2.values8, other1.values9 / other2.values9, other1.values10 / other2.values10,
            other1.values11 / other2.values11, other1.values12 / other2.values12, other1.values13 / other2.values13,
            other1.values14 / other2.values14, other1.values15 / other2.values15);
    }
}

/**
 * Divide a SIMD16 by a scalar.
 * @param other1 The SIMD16.
 * @param other2 Value to divide the SIMD16 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator/(
    const SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_div_ps(other1.values0, other2.values), _mm256_div_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values),
            _mm_div_ps(other1.values2, other2.values), _mm_div_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 / other2.value, other1.values1 / other2.value,
            other1.values2 / other2.value, other1.values3 / other2.value, other1.values4 / other2.value,
            other1.values5 / other2.value, other1.values6 / other2.value, other1.values7 / other2.value,
            other1.values8 / other2.value, other1.values9 / other2.value, other1.values10 / other2.value,
            other1.values11 / other2.value, other1.values12 / other2.value, other1.values13 / other2.value,
            other1.values14 / other2.value, other1.values15 / other2.value);
    }
}

/**
 * Divide a scalar by all elements of a SIMD16.
 * @param other1 Value to divide the SIMD16 by.
 * @param other2 The SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator/(
    typename SIMD16<T, Width>::BaseDef other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_div_ps(other1.values, other2.values0), _mm256_div_ps(other1.values, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_div_ps(other1.values, other2.values0), _mm_div_ps(other1.values, other2.values1),
            _mm_div_ps(other1.values, other2.values2), _mm_div_ps(other1.values, other2.values3));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.value / other2.values0, other1.value / other2.values1,
            other1.value / other2.values2, other1.value / other2.values3, other1.value / other2.values4,
            other1.value / other2.values5, other1.value / other2.values6, other1.value / other2.values7,
            other1.value / other2.values8, other1.value / other2.values9, other1.value / other2.values10,
            other1.value / other2.values11, other1.value / other2.values12, other1.value / other2.values13,
            other1.value / other2.values14, other1.value / other2.values15);
    }
}

/**
 * Divide each 4 elements of a SIMD16 by a SIMD4.
 * @param other1 The SIMD16.
 * @param other2 Value to divide the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator/(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_div_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_broadcastf128_ps(other2.values);
        return SIMD16<T, Width>(_mm256_div_ps(other1.values0, val), _mm256_div_ps(other1.values1, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values),
            _mm_div_ps(other1.values2, other2.values), _mm_div_ps(other1.values3, other2.values));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values3, other1.values4 / other2.values0,
            other1.values5 / other2.values1, other1.values6 / other2.values2, other1.values7 / other2.values3,
            other1.values8 / other2.values0, other1.values9 / other2.values1, other1.values10 / other2.values2,
            other1.values11 / other2.values3, other1.values12 / other2.values0, other1.values13 / other2.values1,
            other1.values14 / other2.values2, other1.values15 / other2.values3);
    }
}

/**
 * Divide each 8 elements of a SIMD16 by a SIMD8.
 * @param other1 The SIMD16.
 * @param other2 Value to divide the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator/(
    const SIMD16<T, Width>& other1, const typename SIMD16<T, Width>::SIMD8Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_div_ps(other1.values, _mm512_broadcastf256_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_div_ps(other1.values0, other2.values), _mm256_div_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_div_ps(other1.values0, other2.values0), _mm_div_ps(other1.values1, other2.values1),
            _mm_div_ps(other1.values2, other2.values0), _mm_div_ps(other1.values3, other2.values1));
    } else
#endif
    {
        return SIMD16<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values3, other1.values4 / other2.values4,
            other1.values5 / other2.values5, other1.values6 / other2.values6, other1.values7 / other2.values7,
            other1.values8 / other2.values0, other1.values9 / other2.values1, other1.values10 / other2.values2,
            other1.values11 / other2.values3, other1.values12 / other2.values4, other1.values13 / other2.values5,
            other1.values14 / other2.values6, other1.values15 / other2.values7);
    }
}

/**
 * Negate all elements of a SIMD16.
 * @param other The SIMD16 to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator-(const SIMD16<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_sub_ps(_mm512_setzero_ps(), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_setzero_ps();
        return SIMD16<T, Width>(_mm256_sub_ps(val, other.values0), _mm256_sub_ps(val, other.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_setzero_ps();
        return SIMD16<T, Width>(_mm_sub_ps(val, other.values0), _mm_sub_ps(val, other.values1),
            _mm_sub_ps(val, other.values2), _mm_sub_ps(val, other.values3));
    } else
#endif
    {
        return SIMD16<T, Width>(-other.values0, -other.values1, -other.values2, -other.values3, -other.values4,
            -other.values5, -other.values6, -other.values7, -other.values8, -other.values9, -other.values10,
            -other.values11, -other.values12, -other.values13, -other.values14, -other.values15);
    }
}

/**
 * Perform compound assignment and addition with a SIMD16.
 * @param [in,out] other1 The first SIMD16.
 * @param          other2 SIMD16 to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width>& operator+=(SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_add_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_add_ps(other1.values0, other2.values0);
        other1.values1 = _mm256_add_ps(other1.values1, other2.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, other2.values0);
        other1.values1 = _mm_add_ps(other1.values1, other2.values1);
        other1.values2 = _mm_add_ps(other1.values2, other2.values2);
        other1.values3 = _mm_add_ps(other1.values3, other2.values3);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
        other1.values2 += other2.values2;
        other1.values3 += other2.values3;
        other1.values4 += other2.values4;
        other1.values5 += other2.values5;
        other1.values6 += other2.values6;
        other1.values7 += other2.values7;
        other1.values8 += other2.values8;
        other1.values9 += other2.values9;
        other1.values10 += other2.values10;
        other1.values11 += other2.values11;
        other1.values12 += other2.values12;
        other1.values13 += other2.values13;
        other1.values14 += other2.values14;
        other1.values15 += other2.values15;
    }
    return other1;
}

/**
 * Perform compound assignment and addition with a SIMD16.
 * @param [in,out] other1 The SIMD16.
 * @param          other2 Value to add to the first SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width>& operator+=(SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_add_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_add_ps(other1.values0, other2.values);
        other1.values1 = _mm256_add_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, other2.values);
        other1.values1 = _mm_add_ps(other1.values1, other2.values);
        other1.values2 = _mm_add_ps(other1.values2, other2.values);
        other1.values3 = _mm_add_ps(other1.values3, other2.values);
    } else
#endif
    {
        other1.values0 += other2.value;
        other1.values1 += other2.value;
        other1.values2 += other2.value;
        other1.values3 += other2.value;
        other1.values4 += other2.value;
        other1.values5 += other2.value;
        other1.values6 += other2.value;
        other1.values7 += other2.value;
        other1.values8 += other2.value;
        other1.values9 += other2.value;
        other1.values10 += other2.value;
        other1.values11 += other2.value;
        other1.values12 += other2.value;
        other1.values13 += other2.value;
        other1.values14 += other2.value;
        other1.values15 += other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction by a SIMD16.
 * @param [in,out] other1 The first SIMD16.
 * @param          other2 SIMD16 to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width>& operator-=(SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_sub_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_sub_ps(other1.values0, other2.values0);
        other1.values1 = _mm256_sub_ps(other1.values1, other2.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, other2.values0);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values1);
        other1.values2 = _mm_sub_ps(other1.values2, other2.values2);
        other1.values3 = _mm_sub_ps(other1.values3, other2.values3);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
        other1.values2 -= other2.values2;
        other1.values3 -= other2.values3;
        other1.values4 -= other2.values4;
        other1.values5 -= other2.values5;
        other1.values6 -= other2.values6;
        other1.values7 -= other2.values7;
        other1.values8 -= other2.values8;
        other1.values9 -= other2.values9;
        other1.values10 -= other2.values10;
        other1.values11 -= other2.values11;
        other1.values12 -= other2.values12;
        other1.values13 -= other2.values13;
        other1.values14 -= other2.values14;
        other1.values15 -= other2.values15;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction with a SIMD16.
 * @param [in,out] other1 The SIMD16.
 * @param          other2 Value to subtract from the first SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width>& operator-=(SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_sub_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_sub_ps(other1.values0, other2.values);
        other1.values1 = _mm256_sub_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, other2.values);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values);
        other1.values2 = _mm_sub_ps(other1.values2, other2.values);
        other1.values3 = _mm_sub_ps(other1.values3, other2.values);
    } else
#endif
    {
        other1.values0 -= other2.value;
        other1.values1 -= other2.value;
        other1.values2 -= other2.value;
        other1.values3 -= other2.value;
        other1.values4 -= other2.value;
        other1.values5 -= other2.value;
        other1.values6 -= other2.value;
        other1.values7 -= other2.value;
        other1.values8 -= other2.value;
        other1.values9 -= other2.value;
        other1.values10 -= other2.value;
        other1.values11 -= other2.value;
        other1.values12 -= other2.value;
        other1.values13 -= other2.value;
        other1.values14 -= other2.value;
        other1.values15 -= other2.value;
    }
    return other1;
}

/**
 * Perform SIMD16 compound assignment and multiplication.
 * @param [in,out] other1 The first SIMD16.
 * @param          other2 SIMD16 to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width>& operator*=(SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_mul_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_mul_ps(other1.values0, other2.values0);
        other1.values1 = _mm256_mul_ps(other1.values1, other2.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, other2.values0);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values1);
        other1.values2 = _mm_mul_ps(other1.values2, other2.values2);
        other1.values3 = _mm_mul_ps(other1.values3, other2.values3);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
        other1.values2 *= other2.values2;
        other1.values3 *= other2.values3;
        other1.values4 *= other2.values4;
        other1.values5 *= other2.values5;
        other1.values6 *= other2.values6;
        other1.values7 *= other2.values7;
        other1.values8 *= other2.values8;
        other1.values9 *= other2.values9;
        other1.values10 *= other2.values10;
        other1.values11 *= other2.values11;
        other1.values12 *= other2.values12;
        other1.values13 *= other2.values13;
        other1.values14 *= other2.values14;
        other1.values15 *= other2.values15;
    }
    return other1;
}

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] other1 The SIMD16.
 * @param          other2 Value to multiply the SIMD16 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width>& operator*=(SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_mul_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_mul_ps(other1.values0, other2.values);
        other1.values1 = _mm256_mul_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, other2.values);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values);
        other1.values2 = _mm_mul_ps(other1.values2, other2.values);
        other1.values3 = _mm_mul_ps(other1.values3, other2.values);
    } else
#endif
    {
        other1.values0 *= other2.value;
        other1.values1 *= other2.value;
        other1.values2 *= other2.value;
        other1.values3 *= other2.value;
        other1.values4 *= other2.value;
        other1.values5 *= other2.value;
        other1.values6 *= other2.value;
        other1.values7 *= other2.value;
        other1.values8 *= other2.value;
        other1.values9 *= other2.value;
        other1.values10 *= other2.value;
        other1.values11 *= other2.value;
        other1.values12 *= other2.value;
        other1.values13 *= other2.value;
        other1.values14 *= other2.value;
        other1.values15 *= other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and division.
 * @param [in,out] other1 The SIMD16.
 * @param          other2 Value to divide the SIMD16 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width>& operator/=(SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_div_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_div_ps(other1.values0, other2.values0);
        other1.values1 = _mm256_div_ps(other1.values1, other2.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, other2.values0);
        other1.values1 = _mm_div_ps(other1.values1, other2.values1);
        other1.values2 = _mm_div_ps(other1.values2, other2.values2);
        other1.values3 = _mm_div_ps(other1.values3, other2.values3);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
        other1.values2 /= other2.values2;
        other1.values3 /= other2.values3;
        other1.values4 /= other2.values4;
        other1.values5 /= other2.values5;
        other1.values6 /= other2.values6;
        other1.values7 /= other2.values7;
        other1.values8 /= other2.values8;
        other1.values9 /= other2.values9;
        other1.values10 /= other2.values10;
        other1.values11 /= other2.values11;
        other1.values12 /= other2.values12;
        other1.values13 /= other2.values13;
        other1.values14 /= other2.values14;
        other1.values15 /= other2.values15;
    }
    return other1;
}

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] other1 The SIMD16.
 * @param          other2 Value to divide the SIMD16 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width>& operator/=(SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_div_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_div_ps(other1.values0, other2.values);
        other1.values1 = _mm256_div_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, other2.values);
        other1.values1 = _mm_div_ps(other1.values1, other2.values);
        other1.values2 = _mm_div_ps(other1.values2, other2.values);
        other1.values3 = _mm_div_ps(other1.values3, other2.values);
    } else
#endif
    {
        other1.values0 /= other2.value;
        other1.values1 /= other2.value;
        other1.values2 /= other2.value;
        other1.values3 /= other2.value;
        other1.values4 /= other2.value;
        other1.values5 /= other2.value;
        other1.values6 /= other2.value;
        other1.values7 /= other2.value;
        other1.values8 /= other2.value;
        other1.values9 /= other2.value;
        other1.values10 /= other2.value;
        other1.values11 /= other2.value;
        other1.values12 /= other2.value;
        other1.values13 /= other2.value;
        other1.values14 /= other2.value;
        other1.values15 /= other2.value;
    }
    return other1;
}

/**
 * And 2 SIMD16s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD16.
 * @param other2 SIMD16 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator&(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_and_ps(other1.values0, other2.values0), _mm256_and_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_and_ps(other1.values0, other2.values0), _mm_and_ps(other1.values1, other2.values1),
            _mm_and_ps(other1.values2, other2.values2), _mm_and_ps(other1.values3, other2.values3));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(other1.values0 & other2.values0, other1.values1 & other2.values1,
                other1.values2 & other2.values2, other1.values3 & other2.values3, other1.values4 & other2.values4,
                other1.values5 & other2.values5, other1.values6 & other2.values6, other1.values7 & other2.values7,
                other1.values8 & other2.values8, other1.values9 & other2.values9, other1.values10 & other2.values10,
                other1.values11 & other2.values11, other1.values12 & other2.values12, other1.values13 & other2.values13,
                other1.values14 & other2.values14, other1.values15 & other2.values15);
        } else {
            return SIMD16<T, Width>(Shift::bitAnd(other1.values0, other2.values0),
                Shift::bitAnd(other1.values1, other2.values1), Shift::bitAnd(other1.values2, other2.values2),
                Shift::bitAnd(other1.values3, other2.values3), Shift::bitAnd(other1.values4, other2.values4),
                Shift::bitAnd(other1.values5, other2.values5), Shift::bitAnd(other1.values6, other2.values6),
                Shift::bitAnd(other1.values7, other2.values7), Shift::bitAnd(other1.values8, other2.values8),
                Shift::bitAnd(other1.values9, other2.values9), Shift::bitAnd(other1.values10, other2.values10),
                Shift::bitAnd(other1.values11, other2.values11), Shift::bitAnd(other1.values12, other2.values12),
                Shift::bitAnd(other1.values13, other2.values13), Shift::bitAnd(other1.values14, other2.values14),
                Shift::bitAnd(other1.values15, other2.values15));
        }
    }
}

/**
 * And a SIMD16s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD16.
 * @param other2 Value to and with the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator&(
    const SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_and_ps(other1.values0, other2.values), _mm256_and_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_and_ps(other1.values0, other2.values), _mm_and_ps(other1.values1, other2.values),
            _mm_and_ps(other1.values2, other2.values), _mm_and_ps(other1.values3, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(other1.values0 & other2.value, other1.values1 & other2.value,
                other1.values2 & other2.value, other1.values3 & other2.value, other1.values4 & other2.value,
                other1.values5 & other2.value, other1.values6 & other2.value, other1.values7 & other2.value,
                other1.values8 & other2.value, other1.values9 & other2.value, other1.values10 & other2.value,
                other1.values11 & other2.value, other1.values12 & other2.value, other1.values13 & other2.value,
                other1.values14 & other2.value, other1.values15 & other2.value);
        } else {
            return SIMD16<T, Width>(Shift::bitAnd(other1.values0, other2.value),
                Shift::bitAnd(other1.values1, other2.value), Shift::bitAnd(other1.values2, other2.value),
                Shift::bitAnd(other1.values3, other2.value), Shift::bitAnd(other1.values4, other2.value),
                Shift::bitAnd(other1.values5, other2.value), Shift::bitAnd(other1.values6, other2.value),
                Shift::bitAnd(other1.values7, other2.value), Shift::bitAnd(other1.values8, other2.value),
                Shift::bitAnd(other1.values9, other2.value), Shift::bitAnd(other1.values10, other2.value),
                Shift::bitAnd(other1.values11, other2.value), Shift::bitAnd(other1.values12, other2.value),
                Shift::bitAnd(other1.values13, other2.value), Shift::bitAnd(other1.values14, other2.value),
                Shift::bitAnd(other1.values15, other2.value));
        }
    }
}

/**
 * Or 2 SIMD16s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD16.
 * @param other2 SIMD16 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator|(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_or_ps(other1.values0, other2.values0), _mm256_or_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_or_ps(other1.values0, other2.values0), _mm_or_ps(other1.values1, other2.values1),
            _mm_or_ps(other1.values2, other2.values2), _mm_or_ps(other1.values3, other2.values3));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(other1.values0 | other2.values0, other1.values1 | other2.values1,
                other1.values2 | other2.values2, other1.values3 | other2.values3, other1.values4 | other2.values4,
                other1.values5 | other2.values5, other1.values6 | other2.values6, other1.values7 | other2.values7,
                other1.values8 | other2.values8, other1.values9 | other2.values9, other1.values10 | other2.values10,
                other1.values11 | other2.values11, other1.values12 | other2.values12, other1.values13 | other2.values13,
                other1.values14 | other2.values14, other1.values15 | other2.values15);
        } else {
            return SIMD16<T, Width>(Shift::bitOr(other1.values0, other2.values0),
                Shift::bitOr(other1.values1, other2.values1), Shift::bitOr(other1.values2, other2.values2),
                Shift::bitOr(other1.values3, other2.values3), Shift::bitOr(other1.values4, other2.values4),
                Shift::bitOr(other1.values5, other2.values5), Shift::bitOr(other1.values6, other2.values6),
                Shift::bitOr(other1.values7, other2.values7), Shift::bitOr(other1.values8, other2.values8),
                Shift::bitOr(other1.values9, other2.values9), Shift::bitOr(other1.values10, other2.values10),
                Shift::bitOr(other1.values11, other2.values11), Shift::bitOr(other1.values12, other2.values12),
                Shift::bitOr(other1.values13, other2.values13), Shift::bitOr(other1.values14, other2.values14),
                Shift::bitOr(other1.values15, other2.values15));
        }
    }
}

/**
 * Or a SIMD16s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD16.
 * @param other2 Value to and with the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator|(
    const SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_or_ps(other1.values0, other2.values), _mm256_or_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_or_ps(other1.values0, other2.values), _mm_or_ps(other1.values1, other2.values),
            _mm_or_ps(other1.values2, other2.values), _mm_or_ps(other1.values3, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(other1.values0 | other2.value, other1.values1 | other2.value,
                other1.values2 | other2.value, other1.values3 | other2.value, other1.values4 | other2.value,
                other1.values5 | other2.value, other1.values6 | other2.value, other1.values7 | other2.value,
                other1.values8 | other2.value, other1.values9 | other2.value, other1.values10 | other2.value,
                other1.values11 | other2.value, other1.values12 | other2.value, other1.values13 | other2.value,
                other1.values14 | other2.value, other1.values15 | other2.value);
        } else {
            return SIMD16<T, Width>(Shift::bitOr(other1.values0, other2.value),
                Shift::bitOr(other1.values1, other2.value), Shift::bitOr(other1.values2, other2.value),
                Shift::bitOr(other1.values3, other2.value), Shift::bitOr(other1.values4, other2.value),
                Shift::bitOr(other1.values5, other2.value), Shift::bitOr(other1.values6, other2.value),
                Shift::bitOr(other1.values7, other2.value), Shift::bitOr(other1.values8, other2.value),
                Shift::bitOr(other1.values9, other2.value), Shift::bitOr(other1.values10, other2.value),
                Shift::bitOr(other1.values11, other2.value), Shift::bitOr(other1.values12, other2.value),
                Shift::bitOr(other1.values13, other2.value), Shift::bitOr(other1.values14, other2.value),
                Shift::bitOr(other1.values15, other2.value));
        }
    }
}

/**
 * Xor 2 SIMD16s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD16.
 * @param other2 SIMD16 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator^(const SIMD16<T, Width>& other1, const SIMD16<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_xor_ps(other1.values0, other2.values0), _mm256_xor_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_xor_ps(other1.values0, other2.values0), _mm_xor_ps(other1.values1, other2.values1),
            _mm_xor_ps(other1.values2, other2.values2), _mm_xor_ps(other1.values3, other2.values3));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(other1.values0 ^ other2.values0, other1.values1 ^ other2.values1,
                other1.values2 ^ other2.values2, other1.values3 ^ other2.values3, other1.values4 ^ other2.values4,
                other1.values5 ^ other2.values5, other1.values6 ^ other2.values6, other1.values7 ^ other2.values7,
                other1.values8 ^ other2.values8, other1.values9 ^ other2.values9, other1.values10 ^ other2.values10,
                other1.values11 ^ other2.values11, other1.values12 ^ other2.values12, other1.values13 ^ other2.values13,
                other1.values14 ^ other2.values14, other1.values15 ^ other2.values15);
        } else {
            return SIMD16<T, Width>(Shift::bitXor(other1.values0, other2.values0),
                Shift::bitXor(other1.values1, other2.values1), Shift::bitXor(other1.values2, other2.values2),
                Shift::bitXor(other1.values3, other2.values3), Shift::bitXor(other1.values4, other2.values4),
                Shift::bitXor(other1.values5, other2.values5), Shift::bitXor(other1.values6, other2.values6),
                Shift::bitXor(other1.values7, other2.values7), Shift::bitXor(other1.values8, other2.values8),
                Shift::bitXor(other1.values9, other2.values9), Shift::bitXor(other1.values10, other2.values10),
                Shift::bitXor(other1.values11, other2.values11), Shift::bitXor(other1.values12, other2.values12),
                Shift::bitXor(other1.values13, other2.values13), Shift::bitXor(other1.values14, other2.values14),
                Shift::bitXor(other1.values15, other2.values15));
        }
    }
}

/**
 * Xor a SIMD16s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD16.
 * @param other2 Value to and with the SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator^(
    const SIMD16<T, Width>& other1, typename SIMD16<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_xor_ps(other1.values, other2.valus));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD16<T, Width>(
            _mm256_xor_ps(other1.values0, other2.values), _mm256_xor_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD16<T, Width>(_mm_xor_ps(other1.values0, other2.values), _mm_xor_ps(other1.values1, other2.values),
            _mm_xor_ps(other1.values2, other2.values), _mm_xor_ps(other1.values3, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(other1.values0 ^ other2.value, other1.values1 ^ other2.value,
                other1.values2 ^ other2.value, other1.values3 ^ other2.value, other1.values4 ^ other2.value,
                other1.values5 ^ other2.value, other1.values6 ^ other2.value, other1.values7 ^ other2.value,
                other1.values8 ^ other2.value, other1.values9 ^ other2.value, other1.values10 ^ other2.value,
                other1.values11 ^ other2.value, other1.values12 ^ other2.value, other1.values13 ^ other2.value,
                other1.values14 ^ other2.value, other1.values15 ^ other2.value);
        } else {
            return SIMD16<T, Width>(Shift::bitXor(other1.values0, other2.value),
                Shift::bitXor(other1.values1, other2.value), Shift::bitXor(other1.values2, other2.value),
                Shift::bitXor(other1.values3, other2.value), Shift::bitXor(other1.values4, other2.value),
                Shift::bitXor(other1.values5, other2.value), Shift::bitXor(other1.values6, other2.value),
                Shift::bitXor(other1.values7, other2.value), Shift::bitXor(other1.values8, other2.value),
                Shift::bitXor(other1.values9, other2.value), Shift::bitXor(other1.values10, other2.value),
                Shift::bitXor(other1.values11, other2.value), Shift::bitXor(other1.values12, other2.value),
                Shift::bitXor(other1.values13, other2.value), Shift::bitXor(other1.values14, other2.value),
                Shift::bitXor(other1.values15, other2.value));
        }
    }
}

/**
 * Not a SIMD16.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other The SIMD16.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD16<T, Width> operator~(const SIMD16<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD16<T, Width>(_mm512_xor_ps(other.values, _mm512_castsi512_ps(_mm512_set1_epi32(-1))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val = _mm256_undefined_ps();
        val = _mm256_cmp_ps(val, val, _CMP_EQ_UQ);
        return SIMD16<T, Width>(_mm256_xor_ps(other.values0, val), _mm256_xor_ps(other.values1, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_undefined_ps();
        val = _mm_cmpeq_ps(val, val);
        return SIMD16<T, Width>(_mm_xor_ps(other.values0, val), _mm_xor_ps(other.values1, val),
            _mm_xor_ps(other.values2, val), _mm_xor_ps(other.values3, val));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD16<T, Width>(~other.values0, ~other.values1, ~other.values2, ~other.values3, ~other.values4,
                ~other.values5, ~other.values6, ~other.values7, ~other.values8, ~other.values9, ~other.values10,
                ~other.values11, ~other.values12, ~other.values13, ~other.values14, ~other.values15);
        } else {
            return SIMD16<T, Width>(Shift::bitNot(other.values0), Shift::bitNot(other.values1),
                Shift::bitNot(other.values2), Shift::bitNot(other.values3), Shift::bitNot(other.values4),
                Shift::bitNot(other.values5), Shift::bitNot(other.values6), Shift::bitNot(other.values7),
                Shift::bitNot(other.values8), Shift::bitNot(other.values9), Shift::bitNot(other.values10),
                Shift::bitNot(other.values11), Shift::bitNot(other.values12), Shift::bitNot(other.values13),
                Shift::bitNot(other.values14), Shift::bitNot(other.values15));
        }
    }
}
} // namespace Shift
