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

#include "SIMD/XSSIMD3x2.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class SIMD3x3;

template<typename T>
class SIMD3x3Data
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    T value0, value1, value2;
    T value3, value4, value5;
    T value6, value7, value8;

    /** Default constructor. */
    XS_INLINE SIMD3x3Data() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE SIMD3x3Data(const SIMD3x3Data& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE SIMD3x3Data(SIMD3x3Data&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMD3x3Data& operator=(const SIMD3x3Data& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMD3x3Data& operator=(SIMD3x3Data&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit SIMD3x3Data(const SIMD3x3<T, Width>& other) noexcept
    {
        store(other);
    }

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param other0 The first value.
     * @param other1 The second value.
     * @param other2 The third value.
     * @param other3 The fourth value.
     * @param other4 The fifth value.
     * @param other5 The sixth value.
     * @param other6 The seventh value.
     * @param other7 The eighth value.
     * @param other8 The ninth value.
     */
    XS_INLINE void setData(
        T other0, T other1, T other2, T other3, T other4, T other5, T other6, T other7, T other8) noexcept
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
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const SIMD3x3<T, Width>& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            _mm512_mask_compressstoreu_ps(&value0, _cvtu32_mask16(0x0777), other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                const __mmask8 mask = _cvtu32_mask8(0x77);
                _mm256_mask_compressstoreu_ps(&value0, mask, other.values0);
                _mm_mask_compressstoreu_ps(&value6, mask, other.values1);
            } else {
                _mm_storeu_ps(&value0, _mm256_castps256_ps128(other.values0));
                _mm_storeu_ps(&value3, _mm256_extractf128_ps(other.values0, 1));
                const __m128 back = _mm_load_ss(&(this[1].value0));
                _mm_storeu_ps(&value6, other.values1);
                _mm_store_ss(&(this[1].value0), back);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            _mm_storeu_ps(&value0, other.values0);
            _mm_storeu_ps(&value3, other.values1);
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                _mm_mask_compressstoreu_ps(&value6, _cvtu32_mask8(0x7), other.values2);
            } else {
                // Need to backup the 4th element to prevent memory corruption by using the 4 element store
                const __m128 back = _mm_load_ss(&(this[1].value0));
                _mm_storeu_ps(&value6, other.values2);
                _mm_store_ss(&(this[1].value0), back);
            }
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
        }
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD512<T>>
    XS_INLINE SIMD3x3<T, Width> load() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD3x3<T, Width>(
                _mm512_mask_expandloadu_ps(_mm512_undefined_ps(), _cvtu32_mask16(0x0777), &value0));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3x3<T, Width>(
                _mm256_set_m128(_mm_loadu_ps(&value3), _mm_loadu_ps(&value0)), _mm_loadu_ps(&value6));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3x3<T, Width>(_mm_loadu_ps(&value0), _mm_loadu_ps(&value3), _mm_loadu_ps(&value6));
        } else
#endif
        {
            return SIMD3x3<T, Width>(value0, value1, value2, value3, value4, value5, value6, value7, value8);
        }
    }
};

template<typename T, typename T2 = T>
class alignas(maxAlignment<T, 4>) SIMD3x3DataPad
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    T value0, value1, value2;
    NoExport::SIMDPad<T2, sizeof(T)> pad0;
    T value3, value4, value5;
    NoExport::SIMDPad<T2, sizeof(T)> pad1;
    T value6, value7, value8;
    NoExport::SIMDPad<T2, sizeof(T)> pad2;

    /** Default constructor. */
    XS_INLINE SIMD3x3DataPad() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE SIMD3x3DataPad(const SIMD3x3DataPad& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE SIMD3x3DataPad(SIMD3x3DataPad&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMD3x3DataPad& operator=(const SIMD3x3DataPad& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMD3x3DataPad& operator=(SIMD3x3DataPad&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit SIMD3x3DataPad(const SIMD3x3<T, Width>& other) noexcept
    {
        store(other);
    }

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param other0 The first value.
     * @param other1 The second value.
     * @param other2 The third value.
     * @param other3 The fourth value.
     * @param other4 The fifth value.
     * @param other5 The sixth value.
     * @param other6 The seventh value.
     * @param other7 The eighth value.
     * @param other8 The ninth value.
     */
    XS_INLINE void setData(
        T other0, T other1, T other2, T other3, T other4, T other5, T other6, T other7, T other8) noexcept
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
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const SIMD3x3<T, Width>& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            _mm512_mask_storeu_ps(&value0, _cvtu32_mask16(0x0777), other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            _mm256_store_ps(&value0, other.values0);
            _mm_store_ps(&value6, other.values1);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            _mm_store_ps(&value0, other.values0);
            _mm_store_ps(&value3, other.values1);
            _mm_store_ps(&value6, other.values2);
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
        }
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD512<T>>
    XS_INLINE SIMD3x3<T, Width> load() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD3x3<T, Width>(_mm512_loadu_ps(&value0));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3x3<T, Width>(_mm256_loadu_ps(&value0), _mm_load_ps(&value6));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3x3<T, Width>(_mm_load_ps(&value0), _mm_load_ps(&value3), _mm_load_ps(&value6));
        } else
#endif
        {
            return SIMD3x3<T, Width>(value0, value1, value2, value3, value4, value5, value6, value7, value8);
        }
    }
};

template<typename T, SIMDWidth Width = defaultWidthSIMD512<T>>
class SIMD3x3 : public NoExport::SIMDData<T, 9, 3, Width>
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    using Type = T;
    using InternalData = NoExport::SIMDData<T, 9, 3, Width>;
    using Data = SIMD3x3Data<T>;
    template<typename T2 = Type>
    using DataPad = SIMD3x3DataPad<T, T2>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = InternalData::width;
    static constexpr uint32 numValues = InternalData::numValues;
    static constexpr uint32 totalValues = InternalData::totalValues;
    using BaseDef = SIMDBase<T, SIMDBase<T, widthImpl>::widthImpl>;
    using InBaseDef = SIMDInBase<T, SIMDInBase<T, widthImpl>::widthImpl>;
    using SIMD3Def = SIMD3<T, SIMD3<T, widthImpl>::widthImpl>;
    using SIMD3x2Def = SIMD3x2<T, SIMD3x2<T, widthImpl>::widthImpl>;
    using InternalData::SIMDData;

    /** Default constructor. */
    XS_INLINE SIMD3x3() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE SIMD3x3(const SIMD3x3& other) noexcept = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE SIMD3x3(SIMD3x3&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMD3x3& operator=(const SIMD3x3& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMD3x3& operator=(SIMD3x3&& other) noexcept = default;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_INLINE explicit SIMD3x3(const SIMD3x3<T, Width2>& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar) && (Width2 > SIMDWidth::Scalar)) {
            if constexpr (isSame<T, float32> && (Width == SIMDWidth::B16) && (Width2 == SIMDWidth::B16)) {
                this->values0 = other.values0;
                this->values1 = other.values1;
                this->values2 = other.values2;
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width == SIMDWidth::B16) &&
                (Width2 == SIMDWidth::B32)) {
                this->values0 = _mm256_castps256_ps128(other.values0);
                this->values1 = _mm256_extractf128_ps(other.values0, 1);
                this->values2 = other.values1;
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width == SIMDWidth::B32) &&
                (Width2 == SIMDWidth::B16)) {
                this->values0 = _mm256_set_m128(other.values1, other.values0);
                this->values1 = other.values2;
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width == SIMDWidth::B32) &&
                (Width2 == SIMDWidth::B32)) {
                this->values0 = other.values0;
                this->values1 = other.values1;
            } else if constexpr (isSame<T, float32> && (Width == SIMDWidth::B16) && (Width2 >= SIMDWidth::B64)) {
                this->values0 = _mm512_extractf32x4_ps(other.values, 0);
                this->values1 = _mm512_extractf32x4_ps(other.values, 1);
                this->values2 = _mm512_extractf32x4_ps(other.values, 2);
            } else if constexpr (isSame<T, float32> && (Width == SIMDWidth::B32) && (Width2 >= SIMDWidth::B64)) {
                this->values0 = _mm512_extractf32x8_ps(other.values, 0);
                this->values1 = _mm512_extractf32x4_ps(other.values, 2);
            } else if constexpr (isSame<T, float32> && (Width >= SIMDWidth::B64) && (Width2 == SIMDWidth::B16)) {
                this->values = _mm512_insertf32x4(
                    _mm512_castps256_ps512(_mm256_set_m128(other.values1, other.values0)), other.values2, 2);
            } else if constexpr (isSame<T, float32> && (Width >= SIMDWidth::B64) && (Width2 == SIMDWidth::B32)) {
                this->values = _mm512_insertf32x4(_mm512_castps256_ps512(other.values0), other.values1, 2);
            } else if constexpr (isSame<T, float32> && (Width >= SIMDWidth::B64) && (Width2 >= SIMDWidth::B64)) {
                this->values = other.values;
            }
        } else if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B64)) {
                this->values =
                    _mm512_set_ps(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, other.values8, other.values7, other.values6, 0.0f,
                        other.values5, other.values4, other.values3, 0.0f, other.values2, other.values1, other.values0);
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width == SIMDWidth::B32)) {
                this->values0 = _mm256_set_ps(0.0f, other.values5, other.values4, other.values3, 0.0f, other.values2,
                    other.values1, other.values0);
                this->values1 = _mm_set_ps(0.0f, other.values8, other.values7, other.values6);
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
                this->values0 = _mm_set_ps(0.0f, other.values2, other.values1, other.values0);
                this->values1 = _mm_set_ps(0.0f, other.values5, other.values4, other.values3);
                this->values2 = _mm_set_ps(0.0f, other.values8, other.values7, other.values6);
            }
        } else if constexpr (Width2 > SIMDWidth::Scalar) {
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width2 >= SIMDWidth::B64)) {
                this->values0 = _mm_cvtss_f32(_mm512_castps512_ps128(other.values));
                this->values1 = _mm_cvtss_f32(_mm_shuffle3311_ps(_mm512_castps512_ps128(other.values)));
                this->values2 = _mm_cvtss_f32(_mm_shuffle3232_ps(_mm512_castps512_ps128(other.values)));
                const auto hi = _mm512_extractf32x4_ps(other.values, 1);
                this->values3 = _mm_cvtss_f32(hi);
                this->values4 = _mm_cvtss_f32(_mm_shuffle3311_ps(hi));
                this->values5 = _mm_cvtss_f32(_mm_shuffle3232_ps(hi));
                const auto hi2 = _mm512_extractf32x4_ps(other.values, 2);
                this->values6 = _mm_cvtss_f32(hi2);
                this->values7 = _mm_cvtss_f32(_mm_shuffle3311_ps(hi2));
                this->values8 = _mm_cvtss_f32(_mm_shuffle3232_ps(hi2));
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width2 == SIMDWidth::B32)) {
                this->values0 = _mm_cvtss_f32(_mm256_castps256_ps128(other.values0));
                this->values1 = _mm_cvtss_f32(_mm_shuffle3311_ps(_mm256_castps256_ps128(other.values0)));
                this->values2 = _mm_cvtss_f32(_mm_shuffle3232_ps(_mm256_castps256_ps128(other.values0)));
                const auto hi = _mm256_extractf128_ps(other.values0, 1);
                this->values3 = _mm_cvtss_f32(hi);
                this->values4 = _mm_cvtss_f32(_mm_shuffle3311_ps(hi));
                this->values5 = _mm_cvtss_f32(_mm_shuffle3232_ps(hi));
                this->values6 = _mm_cvtss_f32(other.values1);
                this->values7 = _mm_cvtss_f32(_mm_shuffle3311_ps(other.values1));
                this->values8 = _mm_cvtss_f32(_mm_shuffle3232_ps(other.values1));
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width2 == SIMDWidth::B16)) {
                this->values0 = _mm_cvtss_f32(other.values0);
                this->values1 = _mm_cvtss_f32(_mm_shuffle3311_ps(other.values0)); //(x,x,x,1)
                this->values2 = _mm_cvtss_f32(_mm_shuffle3232_ps(other.values0)); //(x,x,x,2)
                this->values3 = _mm_cvtss_f32(other.values1);
                this->values4 = _mm_cvtss_f32(_mm_shuffle3311_ps(other.values1)); //(x,x,x,1)
                this->values5 = _mm_cvtss_f32(_mm_shuffle3232_ps(other.values1)); //(x,x,x,2)
                this->values6 = _mm_cvtss_f32(other.values2);
                this->values7 = _mm_cvtss_f32(_mm_shuffle3311_ps(other.values2)); //(x,x,x,1)
                this->values8 = _mm_cvtss_f32(_mm_shuffle3232_ps(other.values2)); //(x,x,x,2)
            }
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
            this->values8 = other.values8;
        }
    }

    /**
     * Construct from 12 different values.
     * @param value0 The first value.
     * @param value1 The second value.
     * @param value2 The third value.
     * @param value3 The fourth value.
     * @param value4 The fifth value.
     * @param value5 The sixth value.
     * @param value6 The seventh value.
     * @param value7 The eighth value.
     * @param value8 The ninth value.
     */
    XS_INLINE SIMD3x3(T value0, T value1, T value2, T value3, T value4, T value5, T value6, T value7, T value8) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_set_ps(
                0, 0, 0, 0, 0, value8, value7, value6, 0, value5, value4, value3, 0, value2, value1, value0);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = _mm256_set_ps(0, value5, value4, value3, 0, value2, value1, value0);
            this->values1 = _mm_set_ps(0, value8, value7, value6);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_set_ps(0, value2, value1, value0);
            this->values1 = _mm_set_ps(0, value5, value4, value3);
            this->values2 = _mm_set_ps(0, value8, value7, value6);
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
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param value Value to set all elements to.
     */
    XS_INLINE explicit SIMD3x3(T value) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_set1_ps(value);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = _mm256_set1_ps(value);
            this->values1 = _mm256_castps256_ps128(this->values0);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_set1_ps(value);
            this->values1 = this->values0;
            this->values2 = this->values0;
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
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_INLINE explicit SIMD3x3(BaseDef other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = other.values;
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = other.values;
            this->values1 = _mm256_castps256_ps128(this->values0);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = other.values;
            this->values1 = other.values;
            this->values2 = other.values;
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
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_INLINE explicit SIMD3x3(InBaseDef other) noexcept
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
            this->values1 = _mm256_castps256_ps128(this->values0);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_shuffle0000_ps(other.values);
            this->values1 = this->values0;
            this->values2 = this->values0;
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
        }
    }

    /**
     * Construct from 3 different SIMD3s.
     * @param other0 The first SIMD3.
     * @param other1 The second SIMD3.
     * @param other2 The third SIMD3.
     */
    XS_INLINE SIMD3x3(const SIMD3Def& other0, const SIMD3Def& other1, const SIMD3Def& other2) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_insertf32x4(
                _mm512_castps256_ps512(_mm256_set_m128(other1.values, other0.values)), other2.values, 2);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = _mm256_set_m128(other1.values, other0.values);
            this->values1 = other2.values;
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = other0.values;
            this->values1 = other1.values;
            this->values2 = other2.values;
        } else
#endif
        {
            this->values0 = other0.values0;
            this->values1 = other0.values1;
            this->values2 = other0.values2;
            this->values3 = other1.values0;
            this->values4 = other1.values1;
            this->values5 = other1.values2;
            this->values6 = other2.values0;
            this->values7 = other2.values1;
            this->values8 = other2.values2;
        }
    }

    /**
     * Construct from a single SIMD3.
     * @param other The SIMD3.
     */
    XS_INLINE explicit SIMD3x3(const SIMD3Def& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values =
                _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_broadcastf128_ps(other.values)), other.values, 2);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = _mm256_broadcastf128_ps(other.values);
            this->values1 = other.values;
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = other.values;
            this->values1 = other.values;
            this->values2 = other.values;
        } else
#endif
        {
            this->values0 = other.values0;
            this->values1 = other.values1;
            this->values2 = other.values2;
            this->values3 = other.values0;
            this->values4 = other.values1;
            this->values5 = other.values2;
            this->values6 = other.values0;
            this->values7 = other.values1;
            this->values8 = other.values2;
        }
    }

    /**
     * Constructor to build set to 0.
     * @returns Newly constructed object with required attributes.
     */
    XS_INLINE static SIMD3x3 Zero() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD3x3(_mm512_setzero_ps());
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 value = _mm256_setzero_ps();
            return SIMD3x3(value, _mm256_castps256_ps128(value));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 value = _mm_setzero_ps();
            return SIMD3x3(value, value, value);
        } else
#endif
        {
            return SIMD3x3(T{0});
        }
    }

    /**
     * Constructor to build set to 1.
     * @returns Newly constructed object with required attributes.
     */
    XS_INLINE static SIMD3x3 One() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD3x3(_mm512_set1_ps(1.0f));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 value = _mm256_set1_ps(1.0f);
            return SIMD3x3(value, _mm256_castps256_ps128(value));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 value = _mm_set1_ps(1.0f);
            return SIMD3x3(value, value, value);
        } else
#endif
        {
            return SIMD3x3(T{1});
        }
    }

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-8).
     * @returns SIMDInBase containing the desired value.
     */
    template<uint32 Index>
    XS_INLINE InBaseDef getValueInBase() const noexcept
    {
        static_assert(Index < 9, "Invalid Index: Index must be <9");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index == 0) {
                return InBaseDef(_mm512_castps512_ps128(this->values));
            } else if constexpr (Index == 1) {
                return InBaseDef(_mm_shuffle3311_ps(_mm512_castps512_ps128(this->values)));
            } else if constexpr (Index == 2) {
                return InBaseDef(_mm_shuffle3232_ps(_mm512_castps512_ps128(this->values)));
            } else if constexpr (Index % 3 == 0) {
                return InBaseDef(_mm512_extractf32x4_ps(this->values, Index / 3));
            } else if constexpr (Index % 3 == 1) {
                const __m128 val = _mm512_extractf32x4_ps(this->values, Index / 3);
                return InBaseDef(_mm_shuffle3311_ps(val));
            } else /*(Index % 3 == 2)*/ {
                const __m128 val = _mm512_extractf32x4_ps(this->values, Index / 3);
                return InBaseDef(_mm_shuffle3232_ps(val));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                return InBaseDef(_mm256_castps256_ps128(this->values0));
            } else if constexpr (Index == 1) {
                return InBaseDef(_mm_shuffle3311_ps(_mm256_castps256_ps128(this->values0)));
            } else if constexpr (Index == 2) {
                return InBaseDef(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values0)));
            } else if constexpr (Index == 3) {
                return InBaseDef(_mm256_extractf128_ps(this->values0, 1));
            } else if constexpr (Index == 4) {
                const __m128 val = _mm256_extractf128_ps(this->values0, 1);
                return InBaseDef(_mm_shuffle3311_ps(val));
            } else if constexpr (Index == 5) {
                const __m128 val = _mm256_extractf128_ps(this->values0, 1);
                return InBaseDef(_mm_shuffle3232_ps(val));
            } else if constexpr (Index == 6) {
                return InBaseDef(this->values1);
            } else if constexpr (Index == 7) {
                return InBaseDef(_mm_shuffle3311_ps(this->values1));
            } else /*Index == 8*/ {
                return InBaseDef(_mm_shuffle3232_ps(this->values1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 3 == 0) {
                return InBaseDef((&this->values0)[Index / 3]);
            } else if constexpr (Index % 3 == 1) {
                return InBaseDef(_mm_shuffle3311_ps((&this->values0)[Index / 3]));
            } else /*Index % 3 == 2*/ {
                return InBaseDef(_mm_shuffle3232_ps((&this->values0)[Index / 3]));
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
     * @tparam Index The index of the element to retrieve (range is 0-8).
     * @returns SIMDBase containing the desired value.
     */
    template<uint32 Index>
    XS_INLINE BaseDef getValue() const noexcept
    {
        static_assert(Index < 9, "Invalid Index: Index must be <9");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index == 0) {
                return BaseDef(_mm512_broadcastss_ps(_mm512_castps512_ps128(this->values)));
            } else {
                return BaseDef(_mm512_shuffle_f32x4(
                    _mm512_shuffle1_ps(this->values, _MM_SHUFFLE(Index % 3, Index % 3, Index % 3, Index % 3)),
                    this->values, _MM_SHUFFLE(Index / 3, Index / 3, Index / 3, Index / 3)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX2> && Index == 0) {
                return BaseDef(_mm256_broadcastss_ps(_mm256_castps256_ps128(this->values0)));
            } else if constexpr (Index == 0) {
                const __m128 val = _mm_shuffle0000_ps(_mm256_castps256_ps128(this->values0));
                return BaseDef(_mm256_broadcastf128_ps(val));
            } else if constexpr (Index == 1) {
                const __m128 val = _mm_shuffle1111_ps(_mm256_castps256_ps128(this->values0));
                return BaseDef(_mm256_broadcastf128_ps(val));
            } else if constexpr (Index == 2) {
                const __m128 val = _mm_shuffle2222_ps(_mm256_castps256_ps128(this->values0));
                return BaseDef(_mm256_broadcastf128_ps(val));
            } else if constexpr (Index == 3) {
                const __m256 val = _mm256_shuffle1_ps(this->values0, _MM_SHUFFLE(0, 0, 0, 0));
                return BaseDef(_mm256_shuffle76547654_ps(val));
            } else if constexpr (Index == 4) {
                const __m256 val = _mm256_shuffle1_ps(this->values0, _MM_SHUFFLE(1, 1, 1, 1));
                return BaseDef(_mm256_shuffle76547654_ps(val));
            } else if constexpr (Index == 5) {
                const __m256 val = _mm256_shuffle1_ps(this->values0, _MM_SHUFFLE(2, 2, 2, 2));
                return BaseDef(_mm256_shuffle76547654_ps(val));
            } else if constexpr (hasISAFeature<ISAFeature::AVX2> && Index == 6) {
                return BaseDef(_mm256_broadcastss_ps(this->values1));
            } else if constexpr (Index == 6) {
                const __m128 val = _mm_shuffle0000_ps(this->values1);
                return BaseDef(_mm256_broadcastf128_ps(val));
            } else if constexpr (Index == 7) {
                const __m128 val = _mm_shuffle1111_ps(this->values1);
                return BaseDef(_mm256_broadcastf128_ps(val));
            } else /*Index == 8*/ {
                const __m128 val = _mm_shuffle2222_ps(this->values1);
                return BaseDef(_mm256_broadcastf128_ps(val));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 3 == 0) {
                return BaseDef(_mm_shuffle0000_ps((&this->values0)[Index / 4]));
            } else if constexpr (Index % 3 == 1) {
                return BaseDef(_mm_shuffle1111_ps((&this->values0)[Index / 4]));
            } else /*Index % 3 == 2*/ {
                return BaseDef(_mm_shuffle2222_ps((&this->values0)[Index / 4]));
            }
        } else
#endif
        {
            return BaseDef((&this->values0)[Index]);
        }
    }

    /**
     * Get a SIMD3.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to get (range is 0-2).
     * @returns SIMD3 containing the desired values.
     */
    template<uint32 Index>
    XS_INLINE SIMD3Def getValue3() const noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index == 0) {
                return SIMD3Def(_mm512_castps512_ps128(this->values));
            } else {
                return SIMD3Def(_mm512_extractf32x4_ps(this->values, Index));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                return SIMD3Def(_mm256_castps256_ps128(this->values0));
            } else if constexpr (Index == 1) {
                return SIMD3Def(_mm256_extractf128_ps(this->values0, 1));
            } else /*Index == 2*/ {
                return SIMD3Def(this->values1);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3Def((&this->values0)[Index]);
        } else
#endif
        {
            return SIMD3Def(
                (&this->values0)[Index * 3], (&this->values0)[Index * 3 + 1], (&this->values0)[Index * 3 + 2]);
        }
    }

    /**
     * Get a SIMD3x2.
     * @note Optimised for getting value from register.
     * @tparam Index0 The index of the first SIMD3 to get (range is 0-2).
     * @tparam Index1 The index of the second SIMD3 to get (range is 0-2).
     * @returns SIMD3x2 containing the desired values.
     */
    template<uint32 Index0, uint32 Index1>
    XS_INLINE SIMD3x2Def getValue3x2() const noexcept
    {
        static_assert(Index0 < 3 && Index1 < 3, "Invalid Index: Indexes must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == 0 && Index1 == 1) {
                return SIMD3x2Def(_mm512_castps512_ps256(this->values));
            } else if constexpr (Index0 == 1 && Index1 == 0) {
                return SIMD3x2Def(_mm256_shuffle32107654_ps(_mm512_castps512_ps256(this->values)));
            } else {
                return SIMD3x2Def(_mm512_castps512_ps256(
                    _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 2, Index1, Index0))));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 1) {
                return SIMD3x2Def(this->values0);
            } else if constexpr (Index0 == 0 && Index1 == 2) {
                return SIMD3x2Def(_mm256_insertf128_ps(this->values0, this->values1, 1));
            } else if constexpr (Index0 == 2 && Index1 == 2) {
                return SIMD3x2Def(_mm256_broadcastf128_ps(this->values1));
            } else {
                return SIMD3x2Def(_mm256_permute2f128_ps(
                    this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3x2Def((&this->values0)[Index0], (&this->values0)[Index1]);
        } else
#endif
        {
            return SIMD3x2Def((&this->values0)[Index0 * 3], (&this->values0)[Index0 * 3 + 1],
                (&this->values0)[Index0 * 3 + 2], (&this->values0)[Index1 * 3], (&this->values0)[Index1 * 3 + 1],
                (&this->values0)[Index1 * 3 + 2]);
        }
    }

    /**
     * Set a SIMD2.
     * @tparam Index The index of the element to set (range is 0-2)
     * @param other The new values.
     */
    template<uint32 Index>
    XS_INLINE void setValue3(const SIMD3Def& other) noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index == 0) {
                this->values =
                    _mm512_mask_mov_ps(this->values, _cvtu32_mask16(0xf), _mm512_castps128_ps512(other.values));
            } else {
                this->values = _mm512_insertf32x4(this->values, other.values, Index);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                this->values0 = _mm256_blend_ps(
                    this->values0, _mm256_castps128_ps256(other.values), _MM256_BLEND(0, 0, 0, 0, 1, 1, 1, 1));
            } else if constexpr (Index == 1) {
                this->values0 = _mm256_insertf128_ps(this->values0, other.values, 1);
            } else {
                this->values1 = other.values;
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            (&this->values0)[Index] = other.values;
        } else
#endif
        {
            (&this->values0)[Index * 3] = other.values0;
            (&this->values0)[Index * 3 + 1] = other.values1;
            (&this->values0)[Index * 3 + 2] = other.values2;
        }
    }

    /**
     * Set a SIMD3x2.
     * @tparam Index0 The index of the first element to set (range is 0-2).
     * @tparam Index1 The index of the second element to set (range is 0-2)(Index1 != Index0).
     * @param other The new values.
     */
    template<uint32 Index0, uint32 Index1>
    XS_INLINE void setValue3x2(const SIMD3x2Def& other) noexcept
    {
        static_assert(Index0 < 3 && Index1 < 3, "Invalid Index: Indexes must be <3");
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
                    _MM_SHUFFLE(0, Index0 == 2 ? 0 : 1, Index0 == 1 ? 0 : 1, Index0 == 0 ? 0 : 1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 1) {
                this->values0 = other.values;
            } else if constexpr (Index0 == 0 && Index1 == 2) {
                this->values0 = _mm256_blend_ps(this->values0, other.values, _MM256_BLEND(0, 0, 0, 0, 1, 1, 1, 1));
                this->values1 = _mm256_extractf128_ps(other.values, 1);
            } else if constexpr (Index0 == 1 && Index1 == 0) {
                this->values0 = _mm256_shuffle32107654_ps(other.values);
            } else if constexpr (Index0 == 1 && Index1 == 2) {
                this->values0 = _mm256_permute2f128_ps(this->values0, other.values, _MM256_PERMUTE(2, 0));
                this->values1 = _mm256_extractf128_ps(other.values, 1);
            } else if constexpr (Index0 == 2 && Index1 == 0) {
                this->values0 = _mm256_permute2f128_ps(this->values0, other.values, _MM256_PERMUTE(1, 3));
                this->values1 = _mm256_extractf128_ps(other.values, 0);
            } else /*Index0 == 2 && Index1 == 1*/ {
                this->values0 = _mm256_blend_ps(this->values0, other.values, _MM256_BLEND(1, 1, 1, 1, 0, 0, 0, 0));
                this->values1 = _mm256_extractf128_ps(other.values, 0);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            (&this->values0)[Index0] = other.values0;
            (&this->values0)[Index1] = other.values1;
        } else
#endif
        {
            (&this->values0)[Index0 * 3] = other.values0;
            (&this->values0)[Index0 * 3 + 1] = other.values1;
            (&this->values0)[Index0 * 3 + 2] = other.values2;
            (&this->values0)[Index1 * 3] = other.values3;
            (&this->values0)[Index1 * 3 + 1] = other.values4;
            (&this->values0)[Index1 * 3 + 2] = other.values5;
        }
    }

    /**
     * Negate an element of the object.
     * @tparam Elem0 Boolean indicating if first element should be negated.
     * @tparam Elem1 Boolean indicating if second element should be negated.
     * @tparam Elem2 Boolean indicating if third element should be negated.
     * @tparam Elem3 Boolean indicating if fourth element should be negated.
     * @tparam Elem4 Boolean indicating if fifth element should be negated.
     * @tparam Elem5 Boolean indicating if sixth element should be negated.
     * @tparam Elem6 Boolean indicating if seventh element should be negated.
     * @tparam Elem7 Boolean indicating if eighth element should be negated.
     * @tparam Elem8 Boolean indicating if ninth element should be negated.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7, bool Elem8>
    XS_INLINE SIMD3x3 negate() const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && !Elem8) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8) {
                return SIMD3x3(_mm512_xor_ps(this->values, _mm512_set1_ps(-0.0f)));
            } else {
                return SIMD3x3(_mm512_xor_ps(this->values,
                    _mm512_set_ps(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Elem8 ? -0.0f : 0.0f, Elem7 ? -0.0f : 0.0f,
                        Elem6 ? -0.0f : 0.0f, 0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f,
                        0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8) {
                const __m256 val = _mm256_set1_ps(-0.0f);
                return SIMD3x3(
                    _mm256_xor_ps(this->values0, val), _mm_xor_ps(this->values1, _mm256_castps256_ps128(val)));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5) {
                return SIMD3x3(_mm256_xor_ps(this->values0, _mm256_set1_ps(-0.0f)),
                    _mm_xor_ps(this->values1,
                        _mm_set_ps(0.0f, Elem8 ? -0.0f : 0.0f, Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f)));
            } else if constexpr (Elem6 && Elem7 && Elem8) {
                return SIMD3x3(_mm256_xor_ps(this->values0,
                                   _mm256_set_ps(0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f,
                                       0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                    _mm_xor_ps(this->values1, _mm_set1_ps(-0.0f)));
            } else if constexpr (Elem0 == Elem6 && Elem1 == Elem7 && Elem2 == Elem8) {
                const __m256 val = _mm256_set_ps(0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f,
                    0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f);
                return SIMD3x3(
                    _mm256_xor_ps(this->values0, val), _mm_xor_ps(this->values1, _mm256_castps256_ps128(val)));
            } else if constexpr (Elem6 && !Elem7 && !Elem8) {
                return SIMD3x3(_mm256_xor_ps(this->values0,
                                   _mm256_set_ps(0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f,
                                       0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                    _mm_xor_ps(this->values1, _mm_set_ss(-0.0f)));
            } else {
                return SIMD3x3(_mm256_xor_ps(this->values0,
                                   _mm256_set_ps(0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f,
                                       0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                    _mm_xor_ps(this->values1,
                        _mm_set_ps(0.0f, Elem8 ? -0.0f : 0.0f, Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Elem0 && !Elem1 && !Elem2 && Elem3 && !Elem4 && !Elem5 && Elem6 && !Elem7 && !Elem8) {
                const __m128 value = _mm_set_ss(-0.0f);
                return SIMD3x3(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value),
                    _mm_xor_ps(this->values2, value));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD3x3(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value),
                    _mm_xor_ps(this->values2, value));
            } else if constexpr (Elem0 && Elem1 && Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && !Elem8) {
                return SIMD3x3(_mm_xor_ps(this->values0, _mm_set1_ps(-0.0f)), this->values1, this->values2);
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && Elem3 && Elem4 && Elem5 && !Elem6 && !Elem7 && !Elem8) {
                return SIMD3x3(this->values0, _mm_xor_ps(this->values1, _mm_set1_ps(-0.0f)), this->values2);
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && Elem6 && Elem7 && Elem8) {
                return SIMD3x3(this->values0, this->values1, _mm_xor_ps(this->values2, _mm_set1_ps(-0.0f)));
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD3x3(this->values0, _mm_xor_ps(this->values1, value), _mm_xor_ps(this->values2, value));
            } else if constexpr (Elem0 && Elem1 && Elem2 && !Elem3 && !Elem4 && !Elem5 && Elem6 && Elem7 && Elem8) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD3x3(_mm_xor_ps(this->values0, value), this->values1, _mm_xor_ps(this->values2, value));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && !Elem6 && !Elem7 && !Elem8) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD3x3(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value), this->values2);
            } else if constexpr (Elem0 == Elem3 && Elem0 == Elem6 && Elem1 == Elem4 && Elem1 == Elem7 &&
                Elem2 == Elem5 && Elem2 == Elem8) {
                const __m128 value = _mm_set_ps(0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f);
                return SIMD3x3(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value),
                    _mm_xor_ps(this->values2, value));
            } else {
                return SIMD3x3(_mm_xor_ps(this->values0,
                                   _mm_set_ps(0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                    _mm_xor_ps(this->values1,
                        _mm_set_ps(0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f)),
                    _mm_xor_ps(this->values2,
                        _mm_set_ps(0.0f, Elem8 ? -0.0f : 0.0f, Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f)));
            }
        }
#endif
        else {
            return SIMD3x3(Elem0 ? -this->values0 : this->values0, Elem1 ? -this->values1 : this->values1,
                Elem2 ? -this->values2 : this->values2, Elem3 ? -this->values3 : this->values3,
                Elem4 ? -this->values4 : this->values4, Elem5 ? -this->values5 : this->values5,
                Elem6 ? -this->values6 : this->values6, Elem7 ? -this->values7 : this->values7,
                Elem8 ? -this->values8 : this->values8);
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
    XS_INLINE SIMD3x3 mad(const SIMD3x3& other1, const SIMD3x3& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x3(_mm512_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD3x3(_mm512_add_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x3(_mm256_fmadd_ps(this->values0, other1.values0, other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values1, other2.values1));
            } else {
                return SIMD3x3(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values0), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x3(_mm_fmadd_ps(this->values0, other1.values0, other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values1, other2.values1),
                    _mm_fmadd_ps(this->values2, other1.values2, other2.values2));
            } else {
                return SIMD3x3(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1),
                    _mm_add_ps(_mm_mul_ps(this->values2, other1.values2), other2.values2));
            }
        } else
#endif
        {
            return SIMD3x3(Shift::fma<T>(this->values0, other1.values0, other2.values0),
                Shift::fma<T>(this->values1, other1.values1, other2.values1),
                Shift::fma<T>(this->values2, other1.values2, other2.values2),
                Shift::fma<T>(this->values3, other1.values3, other2.values3),
                Shift::fma<T>(this->values4, other1.values4, other2.values4),
                Shift::fma<T>(this->values5, other1.values5, other2.values5),
                Shift::fma<T>(this->values6, other1.values6, other2.values6),
                Shift::fma<T>(this->values7, other1.values7, other2.values7),
                Shift::fma<T>(this->values8, other1.values8, other2.values8));
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
    XS_INLINE SIMD3x3 mad(BaseDef other1, const SIMD3x3& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x3(_mm512_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD3x3(_mm512_add_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x3(_mm256_fmadd_ps(this->values0, other1.values, other2.values0),
                    _mm_fmadd_ps(this->values1, _mm256_castps256_ps128(other1.values), other2.values1));
            } else {
                return SIMD3x3(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, _mm256_castps256_ps128(other1.values)), other2.values1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x3(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values, other2.values1),
                    _mm_fmadd_ps(this->values2, other1.values, other2.values2));
            } else {
                return SIMD3x3(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1),
                    _mm_add_ps(_mm_mul_ps(this->values2, other1.values), other2.values2));
            }
        } else
#endif
        {
            return SIMD3x3(Shift::fma<T>(this->values0, other1.value, other2.values0),
                Shift::fma<T>(this->values1, other1.value, other2.values1),
                Shift::fma<T>(this->values2, other1.value, other2.values2),
                Shift::fma<T>(this->values3, other1.value, other2.values3),
                Shift::fma<T>(this->values4, other1.value, other2.values4),
                Shift::fma<T>(this->values5, other1.value, other2.values5),
                Shift::fma<T>(this->values6, other1.value, other2.values6),
                Shift::fma<T>(this->values7, other1.value, other2.values7),
                Shift::fma<T>(this->values8, other1.value, other2.values8));
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
    XS_INLINE SIMD3x3 mad(const SIMD3x3& other1, BaseDef other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x3(_mm512_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD3x3(_mm512_add_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x3(_mm256_fmadd_ps(this->values0, other1.values0, other2.values),
                    _mm_fmadd_ps(this->values1, other1.values1, _mm256_castps256_ps128(other2.values)));
            } else {
                return SIMD3x3(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values0), other2.values),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), _mm256_castps256_ps128(other2.values)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x3(_mm_fmadd_ps(this->values0, other1.values0, other2.values),
                    _mm_fmadd_ps(this->values1, other1.values1, other2.values),
                    _mm_fmadd_ps(this->values2, other1.values2, other2.values));
            } else {
                return SIMD3x3(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values),
                    _mm_add_ps(_mm_mul_ps(this->values2, other1.values2), other2.values));
            }
        } else
#endif
        {
            return SIMD3x3(Shift::fma<T>(this->values0, other1.values0, other2.value),
                Shift::fma<T>(this->values1, other1.values1, other2.value),
                Shift::fma<T>(this->values2, other1.values2, other2.value),
                Shift::fma<T>(this->values3, other1.values3, other2.value),
                Shift::fma<T>(this->values4, other1.values4, other2.value),
                Shift::fma<T>(this->values5, other1.values5, other2.value),
                Shift::fma<T>(this->values6, other1.values6, other2.value),
                Shift::fma<T>(this->values7, other1.values7, other2.value),
                Shift::fma<T>(this->values8, other1.values8, other2.value));
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
    XS_INLINE SIMD3x3 mad(const SIMD3Def& other1, const SIMD3x3& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x3(_mm512_fmadd_ps(this->values, _mm512_broadcastf128_ps(other1.values), other2.values));
            } else {
                return SIMD3x3(
                    _mm512_add_ps(_mm512_mul_ps(this->values, _mm512_broadcastf128_ps(other1.values)), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x3(_mm256_fmadd_ps(this->values0, _mm256_broadcastf128_ps(other1.values), other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values, other2.values1));
            } else {
                return SIMD3x3(
                    _mm256_add_ps(_mm256_mul_ps(this->values0, _mm256_broadcastf128_ps(other1.values)), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x3(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values, other2.values1),
                    _mm_fmadd_ps(this->values2, other1.values, other2.values2));
            } else {
                return SIMD3x3(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1),
                    _mm_add_ps(_mm_mul_ps(this->values2, other1.values), other2.values2));
            }
        } else
#endif
        {
            return SIMD3x3(Shift::fma<T>(this->values0, other1.values0, other2.values0),
                Shift::fma<T>(this->values1, other1.values1, other2.values1),
                Shift::fma<T>(this->values2, other1.values2, other2.values2),
                Shift::fma<T>(this->values3, other1.values0, other2.values3),
                Shift::fma<T>(this->values4, other1.values1, other2.values4),
                Shift::fma<T>(this->values5, other1.values2, other2.values5),
                Shift::fma<T>(this->values6, other1.values0, other2.values6),
                Shift::fma<T>(this->values7, other1.values1, other2.values7),
                Shift::fma<T>(this->values8, other1.values2, other2.values8));
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
    XS_INLINE SIMD3x3 msub(const SIMD3x3& other1, const SIMD3x3& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x3(_mm512_fmsub_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD3x3(_mm512_sub_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x3(_mm256_fmsub_ps(this->values0, other1.values0, other2.values0),
                    _mm_fmsub_ps(this->values1, other1.values1, other2.values1));
            } else {
                return SIMD3x3(_mm256_sub_ps(_mm256_mul_ps(this->values0, other1.values0), other2.values0),
                    _mm_sub_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x3(_mm_fmsub_ps(this->values0, other1.values0, other2.values0),
                    _mm_fmsub_ps(this->values1, other1.values1, other2.values1),
                    _mm_fmsub_ps(this->values2, other1.values2, other2.values2));
            } else {
                return SIMD3x3(_mm_sub_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                    _mm_sub_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1),
                    _mm_sub_ps(_mm_mul_ps(this->values2, other1.values2), other2.values2));
            }
        } else
#endif
        {
            return SIMD3x3(Shift::fma<T>(this->values0, other1.values0, -other2.values0),
                Shift::fma<T>(this->values1, other1.values1, -other2.values1),
                Shift::fma<T>(this->values2, other1.values2, -other2.values2),
                Shift::fma<T>(this->values3, other1.values3, -other2.values3),
                Shift::fma<T>(this->values4, other1.values4, -other2.values4),
                Shift::fma<T>(this->values5, other1.values5, -other2.values5),
                Shift::fma<T>(this->values6, other1.values6, -other2.values6),
                Shift::fma<T>(this->values7, other1.values7, -other2.values7),
                Shift::fma<T>(this->values8, other1.values8, -other2.values8));
        }
    }

    /**
     * Compute component product of each SIMD3 with a single value from a input SIMD3.
     * @param other The values to multiply each internal SIMD3 by.
     * @returns The result of the operation.
     */
    XS_INLINE SIMD3x3 component3(const SIMD3Def& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 val = _mm512_insertf32x4(
                _mm512_castps256_ps512(
                    _mm256_set_m128(_mm_shuffle1111_ps(other.values), _mm_shuffle0000_ps(other.values))),
                _mm_shuffle2222_ps(other.values), 2);
            return SIMD3x3(_mm512_mul_ps(this->values, val));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other.values), _mm_shuffle0000_ps(other.values));
            return SIMD3x3(
                _mm256_mul_ps(this->values0, val0), _mm_mul_ps(this->values1, _mm_shuffle2222_ps(other.values)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3x3(_mm_mul_ps(this->values0, _mm_shuffle0000_ps(other.values)),
                _mm_mul_ps(this->values1, _mm_shuffle1111_ps(other.values)),
                _mm_mul_ps(this->values2, _mm_shuffle2222_ps(other.values)));
        } else
#endif
        {
            return SIMD3x3(this->values0 * other.values0, this->values1 * other.values0, this->values2 * other.values0,
                this->values3 * other.values1, this->values4 * other.values1, this->values5 * other.values1,
                this->values6 * other.values2, this->values7 * other.values2, this->values8 * other.values2);
        }
    }

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_INLINE SIMD3x3 abs() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD3x3(_mm512_andnot_ps(_mm512_set1_ps(-0.0f), this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val = _mm256_set1_ps(-0.0f);
            return SIMD3x3(
                _mm256_andnot_ps(val, this->values0), _mm_andnot_ps(_mm256_castps256_ps128(val), this->values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_set1_ps(-0.0f);
            return SIMD3x3(_mm_andnot_ps(val, this->values0), _mm_andnot_ps(val, this->values1),
                _mm_andnot_ps(val, this->values2));
        } else
#endif
        {
            return SIMD3x3(Shift::abs<T>(this->values0), Shift::abs<T>(this->values1), Shift::abs<T>(this->values2),
                Shift::abs<T>(this->values3), Shift::abs<T>(this->values4), Shift::abs<T>(this->values5),
                Shift::abs<T>(this->values6), Shift::abs<T>(this->values7), Shift::abs<T>(this->values8));
        }
    }

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_INLINE SIMD3x3 max(const SIMD3x3& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD3x3(_mm512_max_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3x3(_mm256_max_ps(this->values0, other.values0), _mm_max_ps(this->values1, other.values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3x3(_mm_max_ps(this->values0, other.values0), _mm_max_ps(this->values1, other.values1),
                _mm_max_ps(this->values2, other.values2));
        } else
#endif
        {
            return SIMD3x3(Shift::max<T>(this->values0, other.values0), Shift::max<T>(this->values1, other.values1),
                Shift::max<T>(this->values2, other.values2), Shift::max<T>(this->values3, other.values3),
                Shift::max<T>(this->values4, other.values4), Shift::max<T>(this->values5, other.values5),
                Shift::max<T>(this->values6, other.values6), Shift::max<T>(this->values7, other.values7),
                Shift::max<T>(this->values8, other.values8));
        }
    }

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_INLINE SIMD3x3 min(const SIMD3x3& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD3x3(_mm512_min_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3x3(_mm256_min_ps(this->values0, other.values0), _mm_min_ps(this->values1, other.values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3x3(_mm_min_ps(this->values0, other.values0), _mm_min_ps(this->values1, other.values1),
                _mm_min_ps(this->values2, other.values2));
        } else
#endif
        {
            return SIMD3x3(Shift::min<T>(this->values0, other.values0), Shift::min<T>(this->values1, other.values1),
                Shift::min<T>(this->values2, other.values2), Shift::min<T>(this->values3, other.values3),
                Shift::min<T>(this->values4, other.values4), Shift::min<T>(this->values5, other.values5),
                Shift::min<T>(this->values6, other.values6), Shift::min<T>(this->values7, other.values7),
                Shift::min<T>(this->values8, other.values8));
        }
    }

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_INLINE SIMD3x3 max(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD3x3(_mm512_max_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3x3(_mm256_max_ps(this->values0, other.values),
                _mm_max_ps(this->values1, _mm256_castps256_ps128(other.values)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3x3(_mm_max_ps(this->values0, other.values), _mm_max_ps(this->values1, other.values),
                _mm_max_ps(this->values2, other.values));
        } else
#endif
        {
            return SIMD3x3(Shift::max<T>(this->values0, other.value), Shift::max<T>(this->values1, other.value),
                Shift::max<T>(this->values2, other.value), Shift::max<T>(this->values3, other.value),
                Shift::max<T>(this->values4, other.value), Shift::max<T>(this->values5, other.value),
                Shift::max<T>(this->values6, other.value), Shift::max<T>(this->values7, other.value),
                Shift::max<T>(this->values8, other.value));
        }
    }

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_INLINE SIMD3x3 min(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD3x3(_mm512_min_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3x3(_mm256_min_ps(this->values0, other.values),
                _mm_min_ps(this->values1, _mm256_castps256_ps128(other.values)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3x3(_mm_min_ps(this->values0, other.values), _mm_min_ps(this->values1, other.values),
                _mm_min_ps(this->values2, other.values));
        } else
#endif
        {
            return SIMD3x3(Shift::min<T>(this->values0, other.value), Shift::min<T>(this->values1, other.value),
                Shift::min<T>(this->values2, other.value), Shift::min<T>(this->values3, other.value),
                Shift::min<T>(this->values4, other.value), Shift::min<T>(this->values5, other.value),
                Shift::min<T>(this->values6, other.value), Shift::min<T>(this->values7, other.value),
                Shift::min<T>(this->values8, other.value));
        }
    }

    /**
     * Maximum of each internally stored SIMD3
     * @returns SIMD3 set to the largest value between each internal SIMD3.
     */
    XS_INLINE SIMD3Def max3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 val =
                _mm_max_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            return SIMD3Def(_mm_max_ps(val, _mm512_extractf32x4_ps(this->values, 2)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val =
                _mm_max_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            return SIMD3Def(_mm_max_ps(val, this->values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_max_ps(this->values0, this->values1);
            return SIMD3Def(_mm_max_ps(val, this->values2));
        } else
#endif
        {
            T other0 = Shift::max<T>(this->values0, this->values6);
            T other1 = Shift::max<T>(this->values1, this->values7);
            T other2 = Shift::max<T>(this->values2, this->values8);
            other0 = Shift::max<T>(other0, this->values3);
            other1 = Shift::max<T>(other1, this->values4);
            other2 = Shift::max<T>(other2, this->values5);
            return SIMD3Def(other0, other1, other2);
        }
    }

    /**
     * Minimum of each internally stored SIMD3.
     * @returns SIMD3 set to the largest value between each internal SIMD3.
     */
    XS_INLINE SIMD3Def min3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 val =
                _mm_min_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            return SIMD3Def(_mm_min_ps(val, _mm512_extractf32x4_ps(this->values, 2)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val =
                _mm_min_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            return SIMD3Def(_mm_min_ps(val, this->values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_min_ps(this->values0, this->values1);
            return SIMD3Def(_mm_min_ps(val, this->values2));
        } else
#endif
        {
            T other0 = Shift::min<T>(this->values0, this->values6);
            T other1 = Shift::min<T>(this->values1, this->values7);
            T other2 = Shift::min<T>(this->values2, this->values8);
            other0 = Shift::min<T>(other0, this->values3);
            other1 = Shift::min<T>(other1, this->values4);
            other2 = Shift::min<T>(other2, this->values5);
            return SIMD3Def(other0, other1, other2);
        }
    }

    /**
     * Maximum element from each SIMD3.
     * @returns SIMD3 set to the largest value in each SIMD3.
     */
    XS_INLINE SIMD3Def hmax3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 lo = _mm512_castps512_ps128(this->values);
            const __m128 hi = _mm512_extractf32x4_ps(this->values, 1);
            const __m128 hi2 = _mm512_extractf32x4_ps(this->values, 2);
            __m128 val0 = _mm_movehl_ps(lo, hi);
            const __m128 val1 = _mm_movelh_ps(lo, hi);
            val0 = _mm_shuffle_ps(val0, hi2, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, hi2, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_max_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, hi2, _MM_SHUFFLE(2, 2, 3, 1));
            return SIMD3Def(_mm_max_ps(val0, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 lo = _mm256_castps256_ps128(this->values0);
            const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
            __m128 val0 = _mm_movehl_ps(lo, hi);
            const __m128 val1 = _mm_movelh_ps(lo, hi);
            val0 = _mm_shuffle_ps(val0, this->values1, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_max_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(2, 2, 3, 1));
            return SIMD3Def(_mm_max_ps(val0, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_movehl_ps(this->values0, this->values1);
            const __m128 val1 = _mm_movelh_ps(this->values0, this->values1);
            val0 = _mm_shuffle_ps(val0, this->values2, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, this->values2, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_max_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, this->values2, _MM_SHUFFLE(2, 2, 3, 1));
            return SIMD3Def(_mm_max_ps(val0, val2));
        } else
#endif
        {
            const T other1 = Shift::max<T>(this->values0, this->values1);
            const T other2 = Shift::max<T>(this->values3, this->values4);
            const T other3 = Shift::max<T>(this->values6, this->values7);
            return SIMD3Def(Shift::max<T>(this->values2, other1), Shift::max<T>(this->values5, other2),
                Shift::max<T>(this->values8, other3));
        }
    }

    /**
     * Minimum element from each SIMD3
     * @returns SIMD3 set to the smallest value in each SIMD3.
     */
    XS_INLINE SIMD3Def hmin3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 lo = _mm512_castps512_ps128(this->values);
            const __m128 hi = _mm512_extractf32x4_ps(this->values, 1);
            const __m128 hi2 = _mm512_extractf32x4_ps(this->values, 2);
            __m128 val0 = _mm_movehl_ps(lo, hi);
            const __m128 val1 = _mm_movelh_ps(lo, hi);
            val0 = _mm_shuffle_ps(val0, hi2, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, hi2, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_min_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, hi2, _MM_SHUFFLE(2, 2, 3, 1));
            return SIMD3Def(_mm_min_ps(val0, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 lo = _mm256_castps256_ps128(this->values0);
            const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
            __m128 val0 = _mm_movehl_ps(lo, hi);
            const __m128 val1 = _mm_movelh_ps(lo, hi);
            val0 = _mm_shuffle_ps(val0, this->values1, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_min_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(2, 2, 3, 1));
            return SIMD3Def(_mm_min_ps(val0, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_movehl_ps(this->values0, this->values1);
            const __m128 val1 = _mm_movelh_ps(this->values0, this->values1);
            val0 = _mm_shuffle_ps(val0, this->values2, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, this->values2, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_min_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, this->values2, _MM_SHUFFLE(2, 2, 3, 1));
            return SIMD3Def(_mm_min_ps(val0, val2));
        } else
#endif
        {
            const T other1 = Shift::min<T>(this->values0, this->values1);
            const T other2 = Shift::min<T>(this->values3, this->values4);
            const T other3 = Shift::min<T>(this->values6, this->values7);
            return SIMD3Def(Shift::min<T>(this->values2, other1), Shift::min<T>(this->values5, other2),
                Shift::min<T>(this->values8, other3));
        }
    }

    /**
     * Compute the sum of each internally stored SIMD3.
     * @returns SIMD3 set to the sum of all elements between each internal SIMD3.
     */
    XS_INLINE SIMD3Def add3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 val =
                _mm_add_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            return SIMD3Def(_mm_add_ps(val, _mm512_extractf32x4_ps(this->values, 2)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val =
                _mm_add_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            return SIMD3Def(_mm_add_ps(val, this->values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_add_ps(this->values0, this->values1);
            return SIMD3Def(_mm_add_ps(val, this->values2));
        } else
#endif
        {
            T other0 = this->values0 + this->values6;
            T other1 = this->values1 + this->values7;
            T other2 = this->values2 + this->values8;
            other0 += this->values3;
            other1 += this->values4;
            other2 += this->values5;
            return SIMD3Def(other0, other1, other2);
        }
    }

    /**
     * Compute the sum of each SIMD3.
     * @returns SIMD4 set to the sum of all elements in each SIMD3.
     */
    XS_INLINE SIMD3Def hadd3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 lo = _mm512_castps512_ps128(this->values);
            const __m128 hi = _mm512_extractf32x4_ps(this->values, 1);
            const __m128 hi2 = _mm512_extractf32x4_ps(this->values, 2);
            __m128 val0 = _mm_movehl_ps(lo, hi);
            const __m128 val1 = _mm_movelh_ps(lo, hi);
            val0 = _mm_shuffle_ps(val0, hi2, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, hi2, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_add_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, hi2, _MM_SHUFFLE(2, 2, 3, 1));
            return SIMD3Def(_mm_add_ps(val0, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 lo = _mm256_castps256_ps128(this->values0);
            const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
            __m128 val0 = _mm_movehl_ps(lo, hi);
            const __m128 val1 = _mm_movelh_ps(lo, hi);
            val0 = _mm_shuffle_ps(val0, this->values1, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_add_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(2, 2, 3, 1));
            return SIMD3Def(_mm_add_ps(val0, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_movehl_ps(this->values0, this->values1);
            const __m128 val1 = _mm_movelh_ps(this->values0, this->values1);
            val0 = _mm_shuffle_ps(val0, this->values2, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, this->values2, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_add_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, this->values2, _MM_SHUFFLE(2, 2, 3, 1));
            return SIMD3Def(_mm_add_ps(val0, val2));
        } else
#endif
        {
            T res = (this->values0 + this->values1);
            T res2 = (this->values3 + this->values4);
            T res3 = (this->values6 + this->values7);
            res += this->values2;
            res2 += this->values5;
            res3 += this->values8;
            return SIMD3Def(res, res2, res3);
        }
    }

    /**
     * Compute the 3 dimensional dot product of each SIMD3 in two objects.
     * @param other The second object.
     * @returns The value of the dot products.
     */
    XS_INLINE SIMD3Def dot3(const SIMD3x3& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 mul = _mm512_mul_ps(this->values, other.values);
            const __m128 lo = _mm512_castps512_ps128(mul);
            const __m128 hi = _mm512_extractf32x4_ps(mul, 1);
            const __m128 hi2 = _mm512_extractf32x4_ps(mul, 2);
            __m128 val0 = _mm_movehl_ps(lo, hi);
            const __m128 val1 = _mm_movelh_ps(lo, hi);
            val0 = _mm_shuffle_ps(val0, hi2, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, hi2, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_add_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, hi2, _MM_SHUFFLE(2, 2, 3, 1));
            return SIMD3Def(_mm_add_ps(val0, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 mul0 = _mm256_mul_ps(this->values0, other.values0);
            const __m128 mul1 = _mm_mul_ps(this->values1, other.values1);
            const __m128 lo = _mm256_castps256_ps128(mul0);
            const __m128 hi = _mm256_extractf128_ps(mul0, 1);
            __m128 val0 = _mm_movehl_ps(lo, hi);
            const __m128 val1 = _mm_movelh_ps(lo, hi);
            val0 = _mm_shuffle_ps(val0, mul1, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, mul1, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_add_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, mul1, _MM_SHUFFLE(2, 2, 3, 1));
            return SIMD3Def(_mm_add_ps(val0, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 mul0 = _mm_mul_ps(this->values0, other.values0);
            const __m128 mul1 = _mm_mul_ps(this->values1, other.values1);
            const __m128 mul2 = _mm_mul_ps(this->values2, other.values2);
            __m128 val0 = _mm_movehl_ps(mul0, mul1);
            const __m128 val1 = _mm_movelh_ps(mul0, mul1);
            val0 = _mm_shuffle_ps(val0, mul2, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, mul2, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_add_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, mul2, _MM_SHUFFLE(2, 2, 3, 1));
            return SIMD3Def(_mm_add_ps(val0, val2));
        } else
#endif
        {
            T v0Sq = this->values0 * other.values0;
            T v1Sq = this->values3 * other.values3;
            T v2Sq = this->values6 * other.values6;
            v0Sq = Shift::fma<T>(this->values1, other.values1, v0Sq);
            v1Sq = Shift::fma<T>(this->values4, other.values4, v1Sq);
            v2Sq = Shift::fma<T>(this->values7, other.values7, v2Sq);
            return SIMD3Def(Shift::fma<T>(this->values2, other.values2, v0Sq),
                Shift::fma<T>(this->values5, other.values5, v1Sq), Shift::fma<T>(this->values8, other.values8, v2Sq));
        }
    }

    /**
     * Compute the 3 dimensional cross product of each SIMD3 in two objects.
     * @note Uses Right-handed coordinate system.
     * @param other The second object.
     * @returns The cross product of the inputs.
     */
    XS_INLINE SIMD3x3 cross3(const SIMD3x3& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m512 val1 = _mm512_shuffle1_ps(other.values, _MM_SHUFFLE(3, 0, 2, 1));
            val1 = _mm512_mul_ps(val1, this->values);
            __m512 val2 = _mm512_shuffle1_ps(this->values, _MM_SHUFFLE(3, 0, 2, 1));
            val2 = _mm512_mul_ps(val2, other.values);
            val1 = _mm512_sub_ps(val1, val2);
            return SIMD3x3(_mm512_shuffle1_ps(val1, _MM_SHUFFLE(3, 0, 2, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val1A = _mm256_shuffle1_ps(other.values0, _MM_SHUFFLE(3, 0, 2, 1));
            __m128 val1C = _mm_shuffle1_ps(other.values1, _MM_SHUFFLE(3, 0, 2, 1));
            val1A = _mm256_mul_ps(val1A, this->values0);
            val1C = _mm_mul_ps(val1C, this->values1);
            __m256 val2A = _mm256_shuffle1_ps(this->values0, _MM_SHUFFLE(3, 0, 2, 1));
            __m128 val2C = _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(3, 0, 2, 1));
            val2A = _mm256_mul_ps(val2A, other.values0);
            val2C = _mm_mul_ps(val2C, other.values1);
            val1A = _mm256_sub_ps(val1A, val2A);
            val1C = _mm_sub_ps(val1C, val2C);
            return SIMD3x3(
                _mm256_shuffle1_ps(val1A, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle1_ps(val1C, _MM_SHUFFLE(3, 0, 2, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val1A = _mm_shuffle1_ps(other.values0, _MM_SHUFFLE(3, 0, 2, 1));
            __m128 val1B = _mm_shuffle1_ps(other.values1, _MM_SHUFFLE(3, 0, 2, 1));
            __m128 val1C = _mm_shuffle1_ps(other.values2, _MM_SHUFFLE(3, 0, 2, 1));
            val1A = _mm_mul_ps(val1A, this->values0);
            val1B = _mm_mul_ps(val1B, this->values1);
            val1C = _mm_mul_ps(val1C, this->values2);
            __m128 val2A = _mm_shuffle1_ps(this->values0, _MM_SHUFFLE(3, 0, 2, 1));
            __m128 val2B = _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(3, 0, 2, 1));
            __m128 val2C = _mm_shuffle1_ps(this->values2, _MM_SHUFFLE(3, 0, 2, 1));
            val2A = _mm_mul_ps(val2A, other.values0);
            val2B = _mm_mul_ps(val2B, other.values1);
            val2C = _mm_mul_ps(val2C, other.values2);
            val1A = _mm_sub_ps(val1A, val2A);
            val1B = _mm_sub_ps(val1B, val2B);
            val1C = _mm_sub_ps(val1C, val2C);
            return SIMD3x3(_mm_shuffle1_ps(val1A, _MM_SHUFFLE(3, 0, 2, 1)),
                _mm_shuffle1_ps(val1B, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle1_ps(val1C, _MM_SHUFFLE(3, 0, 2, 1)));
        } else
#endif
        {
            const T v0CrossX = -(this->values2 * other.values1);
            const T v1CrossX = -(this->values5 * other.values4);
            const T v2CrossX = -(this->values8 * other.values7);
            const T v0CrossY = -(this->values0 * other.values2);
            const T v1CrossY = -(this->values3 * other.values5);
            const T v2CrossY = -(this->values6 * other.values8);
            const T v0CrossZ = -(this->values1 * other.values0);
            const T v1CrossZ = -(this->values4 * other.values3);
            const T v2CrossZ = -(this->values7 * other.values6);

            return SIMD3x3(Shift::fma<T>(this->values1, other.values2, v0CrossX),
                Shift::fma<T>(this->values2, other.values0, v0CrossY),
                Shift::fma<T>(this->values0, other.values1, v0CrossZ),
                Shift::fma<T>(this->values4, other.values5, v1CrossX),
                Shift::fma<T>(this->values5, other.values3, v1CrossY),
                Shift::fma<T>(this->values3, other.values4, v1CrossZ),
                Shift::fma<T>(this->values7, other.values8, v2CrossX),
                Shift::fma<T>(this->values8, other.values6, v2CrossY),
                Shift::fma<T>(this->values6, other.values7, v2CrossZ));
        }
    }

    /**
     * Compute the square of the length of each internal SIMD3.
     * @returns Squared lengths.
     */
    XS_INLINE SIMD3Def lengthSqr3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 mul = _mm512_mul_ps(this->values, this->values);
            const __m128 lo = _mm512_castps512_ps128(mul);
            const __m128 hi = _mm512_extractf32x4_ps(mul, 1);
            const __m128 hi2 = _mm512_extractf32x4_ps(mul, 2);
            __m128 val0 = _mm_movehl_ps(lo, hi);
            const __m128 val1 = _mm_movelh_ps(lo, hi);
            val0 = _mm_shuffle_ps(val0, hi2, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, hi2, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_add_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, hi2, _MM_SHUFFLE(2, 2, 3, 1));
            return SIMD3Def(_mm_add_ps(val0, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 mul0 = _mm256_mul_ps(this->values0, this->values0);
            const __m128 mul1 = _mm_mul_ps(this->values1, this->values1);
            const __m128 lo = _mm256_castps256_ps128(mul0);
            const __m128 hi = _mm256_extractf128_ps(mul0, 1);
            __m128 val0 = _mm_movehl_ps(lo, hi);
            const __m128 val1 = _mm_movelh_ps(lo, hi);
            val0 = _mm_shuffle_ps(val0, mul1, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, mul1, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_add_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, mul1, _MM_SHUFFLE(2, 2, 3, 1));
            return SIMD3Def(_mm_add_ps(val0, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 mul0 = _mm_mul_ps(this->values0, this->values0);
            const __m128 mul1 = _mm_mul_ps(this->values1, this->values1);
            const __m128 mul2 = _mm_mul_ps(this->values2, this->values2);
            __m128 val0 = _mm_movehl_ps(mul0, mul1);
            const __m128 val1 = _mm_movelh_ps(mul0, mul1);
            val0 = _mm_shuffle_ps(val0, mul2, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, mul2, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_add_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, mul2, _MM_SHUFFLE(2, 2, 3, 1));
            return SIMD3Def(_mm_add_ps(val0, val2));
        } else
#endif
        {
            T v0Sq = this->values0 * this->values0;
            T v1Sq = this->values3 * this->values3;
            T v2Sq = this->values6 * this->values6;
            v0Sq = Shift::fma<T>(this->values1, this->values1, v0Sq);
            v1Sq = Shift::fma<T>(this->values4, this->values4, v1Sq);
            v2Sq = Shift::fma<T>(this->values7, this->values7, v2Sq);
            return SIMD3Def(Shift::fma<T>(this->values2, this->values2, v0Sq),
                Shift::fma<T>(this->values5, this->values5, v1Sq), Shift::fma<T>(this->values8, this->values8, v2Sq));
        }
    }

    /**
     * Compute the length of each internal SIMD3.
     * @returns The length.
     */
    XS_INLINE SIMD3Def length3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 mul = _mm512_mul_ps(this->values, this->values);
            const __m128 lo = _mm512_castps512_ps128(mul);
            const __m128 hi = _mm512_extractf32x4_ps(mul, 1);
            const __m128 hi2 = _mm512_extractf32x4_ps(mul, 2);
            __m128 val0 = _mm_movehl_ps(lo, hi);
            const __m128 val1 = _mm_movelh_ps(lo, hi);
            val0 = _mm_shuffle_ps(val0, hi2, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, hi2, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_add_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, hi2, _MM_SHUFFLE(2, 2, 3, 1));
            return SIMD3Def(_mm_sqrt_ps(_mm_add_ps(val0, val2)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 mul0 = _mm256_mul_ps(this->values0, this->values0);
            const __m128 mul1 = _mm_mul_ps(this->values1, this->values1);
            const __m128 lo = _mm256_castps256_ps128(mul0);
            const __m128 hi = _mm256_extractf128_ps(mul0, 1);
            __m128 val0 = _mm_movehl_ps(lo, hi);
            const __m128 val1 = _mm_movelh_ps(lo, hi);
            val0 = _mm_shuffle_ps(val0, mul1, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, mul1, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_add_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, mul1, _MM_SHUFFLE(2, 2, 3, 1));
            return SIMD3Def(_mm_sqrt_ps(_mm_add_ps(val0, val2)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 mul0 = _mm_mul_ps(this->values0, this->values0);
            const __m128 mul1 = _mm_mul_ps(this->values1, this->values1);
            const __m128 mul2 = _mm_mul_ps(this->values2, this->values2);
            __m128 val0 = _mm_movehl_ps(mul0, mul1);
            const __m128 val1 = _mm_movelh_ps(mul0, mul1);
            val0 = _mm_shuffle_ps(val0, mul2, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, mul2, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_add_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, mul2, _MM_SHUFFLE(2, 2, 3, 1));
            return SIMD3Def(_mm_sqrt_ps(_mm_add_ps(val0, val2)));
        } else
#endif
        {
            T v0Sq = this->values0 * this->values0;
            T v1Sq = this->values3 * this->values3;
            T v2Sq = this->values6 * this->values6;
            v0Sq = Shift::fma<T>(this->values1, this->values1, v0Sq);
            v1Sq = Shift::fma<T>(this->values4, this->values4, v1Sq);
            v2Sq = Shift::fma<T>(this->values7, this->values7, v2Sq);
            v0Sq = Shift::fma<T>(this->values2, this->values2, v0Sq);
            v1Sq = Shift::fma<T>(this->values5, this->values5, v1Sq);
            v2Sq = Shift::fma<T>(this->values8, this->values8, v2Sq);
            return SIMD3Def(Shift::sqrt<T>(v0Sq), Shift::sqrt<T>(v1Sq), Shift::sqrt<T>(v2Sq));
        }
    }

    /**
     * Normalise each internal SIMD3.
     * @note The result is unpredictable when all elements are at or near zero.
     * @returns The normalised values.
     */
    XS_INLINE SIMD3x3 normalize3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m512 res = _mm512_mul_ps(this->values, this->values);
            const __m512 res2 = _mm512_add_ps(_mm512_shuffle3322_ps(res), res);
            res = _mm512_add_ps(_mm512_shuffle3311_ps(res), res2);
            res = _mm512_sqrt_ps(res);
            const __m512 val = _mm512_shuffle1_ps(res, _MM_SHUFFLE(0, 0, 0, 0));
            return SIMD3x3(_mm512_div_ps(this->values, val));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            // Faster to use dp except for multiple sqrt cost
            const __m256 mul0 = _mm256_mul_ps(this->values0, this->values0);
            const __m128 mul1 = _mm_mul_ps(this->values1, this->values1);
            const __m128 lo = _mm256_castps256_ps128(mul0);
            const __m128 hi = _mm256_extractf128_ps(mul0, 1);
            __m128 val0 = _mm_movehl_ps(lo, hi);
            const __m128 val1 = _mm_movelh_ps(lo, hi);
            val0 = _mm_shuffle_ps(val0, mul1, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, mul1, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_add_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, mul1, _MM_SHUFFLE(2, 2, 3, 1));
            val0 = _mm_sqrt_ps(_mm_add_ps(val0, val2));
            return SIMD3x3(
                _mm256_div_ps(this->values0, _mm256_set_m128(_mm_shuffle1111_ps(val0), _mm_shuffle0000_ps(val0))),
                _mm_div_ps(this->values1, _mm_shuffle2222_ps(val0)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 mul0 = _mm_mul_ps(this->values0, this->values0);
            const __m128 mul1 = _mm_mul_ps(this->values1, this->values1);
            const __m128 mul2 = _mm_mul_ps(this->values2, this->values2);
            __m128 val0 = _mm_movehl_ps(mul0, mul1);
            const __m128 val1 = _mm_movelh_ps(mul0, mul1);
            val0 = _mm_shuffle_ps(val0, mul2, _MM_SHUFFLE(0, 0, 0, 2));
            __m128 val2 = _mm_shuffle_ps(val1, mul2, _MM_SHUFFLE(1, 1, 2, 0));
            val0 = _mm_add_ps(val0, val2);
            val2 = _mm_shuffle_ps(val1, mul2, _MM_SHUFFLE(2, 2, 3, 1));
            val0 = _mm_sqrt_ps(_mm_add_ps(val0, val2));
            return SIMD3x3(_mm_div_ps(this->values0, _mm_shuffle0000_ps(val0)),
                _mm_div_ps(this->values1, _mm_shuffle1111_ps(val0)),
                _mm_div_ps(this->values2, _mm_shuffle2222_ps(val0)));
        } else
#endif
        {
            T v0Sq = this->values0 * this->values0;
            T v1Sq = this->values3 * this->values3;
            T v2Sq = this->values6 * this->values6;
            v0Sq = Shift::fma<T>(this->values1, this->values1, v0Sq);
            v1Sq = Shift::fma<T>(this->values4, this->values4, v1Sq);
            v2Sq = Shift::fma<T>(this->values7, this->values7, v2Sq);
            v0Sq = Shift::fma<T>(this->values2, this->values2, v0Sq);
            v1Sq = Shift::fma<T>(this->values5, this->values5, v1Sq);
            v2Sq = Shift::fma<T>(this->values8, this->values8, v2Sq);
            v0Sq = Shift::sqrt<T>(v0Sq);
            v1Sq = Shift::sqrt<T>(v1Sq);
            v2Sq = Shift::sqrt<T>(v2Sq);
            return SIMD3x3(this->values0 / v0Sq, this->values1 / v0Sq, this->values2 / v0Sq, this->values3 / v1Sq,
                this->values4 / v1Sq, this->values5 / v1Sq, this->values6 / v2Sq, this->values7 / v2Sq,
                this->values8 / v2Sq);
        }
    }

    /**
     * Transpose the internal SIMD3s.
     * @returns The transposed values.
     */
    XS_INLINE SIMD3x3 transpose() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(1, 0, 1, 0));
            const __m512 val1 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 2, 3, 2));
            __m512 val2 = _mm512_unpacklo_ps(val0, val1); //(x0,x2,y0,y2,x1,--,y1,--,...)
            __m512 val3 = _mm512_unpackhi_ps(val0, val1); //(...,z0,z2,--,--,z1,--,--,--)
            val2 =
                _mm512_mask_blend_ps(_cvtu32_mask16(0xFF00), val2, val3); //(x0,x2,y0,y2,x1,-,y1,-,z0,z2,-,-,z1,-,-,-)
            val3 = _mm512_castpd_ps(_mm512_permutex_pd(
                _mm512_castps_pd(val2), _MM_SHUFFLE(3, 1, 2, 0))); //(x0,x2,x1,-,y0,y2,y1,-,z0,z2,z1,-,-,-,-,-)
            return SIMD3x3(_mm512_shuffle1_ps(val3, _MM_SHUFFLE(3, 1, 2, 0)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val4 = _mm256_shuffle1_ps(this->values0, _MM_SHUFFLE(3, 0, 2, 1));
            __m256 val0 = _mm256_permute2f128_ps(this->values0, val4, _MM256_PERMUTE(2, 0));
            const __m256 val1 = _mm256_permute2f128_ps(this->values0, val4, _MM256_PERMUTE(3, 1));
            const __m256 val2 =
                _mm256_set_m128(_mm_shuffle1_ps(this->values1, _MM_SHUFFLE(2, 3, 0, 1)), this->values1); //(x,x,0,1)
            val0 = _mm256_unpacklo_ps(val0, val1);
            __m128 val = _mm256_extractf128_ps(val0, 1);
            val0 = _mm256_shuffle_ps(val0, val2, _MM_SHUFFLE(1, 0, 1, 0));     //(x,0,1,0)
            val = _mm_shuffle_ps(val, this->values1, _MM_SHUFFLE(3, 2, 3, 2)); //(x,2,3,2)
            return SIMD3x3(val0, val);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val1 = _mm_unpacklo_ps(this->values0, this->values1);
            const __m128 val2 = _mm_shuffle1_ps(this->values2, _MM_SHUFFLE(3, 1, 3, 0)); //(0,x,1,x))
            const __m128 val3 = _mm_unpackhi_ps(this->values0, this->values1);
            return SIMD3x3(_mm_movelh_ps(val1, val2), _mm_movehl_ps(val2, val1),
                _mm_blend_ps(val3, this->values2, _MM_BLEND(1, 1, 0, 0)));
        } else
#endif
        {
            return SIMD3x3(this->values0, this->values3, this->values6, this->values1, this->values4, this->values7,
                this->values2, this->values5, this->values8);
        }
    }

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD3.
     * @tparam Index0 The index in the first object to insert the element (range is 0-2).
     * @tparam Index1 The index in the second object to copy element from (range is 0-2).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_INLINE SIMD3x3 insert3(const SIMD3x3& other) const noexcept
    {
        static_assert(Index0 < 3 && Index1 < 3, "Invalid Index: Indexes must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == Index1) {
                return SIMD3x3(_mm512_mask_mov_ps(this->values, _cvtu32_mask16(0x1111 << Index0), other.values));
            } else {
                return SIMD3x3(_mm512_mask_shuffle_ps(this->values, _cvtu32_mask16(0x1111 << Index0), other.values,
                    other.values, _MM_SHUFFLE(Index1, Index1, Index1, Index1)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == Index1) {
                return SIMD3x3(_mm256_blend_ps(this->values0, other.values0, (1 << Index0) | (1 << (Index0 + 4))),
                    _mm_blend_ps(this->values1, other.values1, (1 << Index0)));
            } else if constexpr (Index0 == 0) {
                const __m256 val = _mm256_shuffle_ps(
                    this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 1, 1)); /*(x,Index1,x,1)*/
                const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 1, 1));
                return SIMD3x3(_mm256_shuffle_ps(val, this->values0, _MM_SHUFFLE(3, 2, 0, 2)),
                    _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 0, 2)));
            } else if constexpr (Index0 == 1) {
                const __m256 val = _mm256_shuffle_ps(
                    this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 0, 0)); /*(x,Index1,x,0)*/
                const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 0, 0));
                return SIMD3x3(_mm256_shuffle_ps(val, this->values0, _MM_SHUFFLE(3, 2, 2, 0)),
                    _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 2, 0)));
            } else /*Index0 == 2*/ {
                const __m256 val = _mm256_shuffle_ps(
                    this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 3, 3)); /*(x,Index1,x,3)*/
                const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 3, 3));
                return SIMD3x3(_mm256_shuffle_ps(this->values0, val, _MM_SHUFFLE(0, 2, 1, 0)),
                    _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(0, 2, 1, 0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD3x3(_mm_blend_ss(this->values0, other.values0), _mm_blend_ss(this->values1, other.values1),
                    _mm_blend_ss(this->values2, other.values2));
            } else if constexpr (Index0 == Index1) {
                return SIMD3x3(_mm_blend_ps(this->values0, other.values0, 1UL << Index0),
                    _mm_blend_ps(this->values1, other.values1, 1UL << Index0),
                    _mm_blend_ps(this->values2, other.values2, 1UL << Index0));
            } else {
                return SIMD3x3(_mm_insert_ps(this->values0, other.values0, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)),
                    _mm_insert_ps(this->values1, other.values1, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)),
                    _mm_insert_ps(this->values2, other.values2, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)));
            }
        } else
#endif
        {
            return SIMD3x3(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
                Index0 != 1 ? this->values1 : (&other.values0)[Index1],
                Index0 != 2 ? this->values2 : (&other.values0)[Index1],
                Index0 != 0 ? this->values3 : (&other.values0)[Index1 + 3],
                Index0 != 1 ? this->values4 : (&other.values0)[Index1 + 3],
                Index0 != 2 ? this->values5 : (&other.values0)[Index1 + 3],
                Index0 != 0 ? this->values6 : (&other.values0)[Index1 + 6],
                Index0 != 1 ? this->values7 : (&other.values0)[Index1 + 6],
                Index0 != 2 ? this->values8 : (&other.values0)[Index1 + 6]);
        }
    }

    /**
     * Blends values from the second object into the first based on a mask. This performs the same operation on each
     * SIMD3.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @tparam Elem2 Whether to copy the third element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2>
    XS_INLINE SIMD3x3 blend3(const SIMD3x3& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2) {
            return *this;
        } else if constexpr (Elem0 && Elem1 && Elem2) {
            return other;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD3x3(_mm512_mask_mov_ps(
                this->values, _cvtu32_mask16((Elem0 * 0x111) | (Elem1 * 0x222) | (Elem2 * 0x444)), other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3x3(_mm256_blend_ps(this->values0, other.values0,
                               _MM256_BLEND(0, Elem2, Elem1, Elem0, 0, Elem2, Elem1, Elem0)),
                _mm_blend_ps(this->values1, other.values1, _MM_BLEND(0, Elem2, Elem1, Elem0)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Elem0 && !Elem1 && !Elem2) {
                return SIMD3x3(_mm_blend_ss(this->values0, other.values0), _mm_blend_ss(this->values1, other.values1),
                    _mm_blend_ss(this->values2, other.values2));
            } else if constexpr (!Elem0 && Elem1 && Elem2) {
                return SIMD3x3(_mm_blend_ss(other.values0, this->values0), _mm_blend_ss(other.values1, this->values1),
                    _mm_blend_ss(other.values2, this->values2));
            } else {
                return SIMD3x3(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(0, Elem2, Elem1, Elem0)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(0, Elem2, Elem1, Elem0)),
                    _mm_blend_ps(this->values2, other.values2, _MM_BLEND(0, Elem2, Elem1, Elem0)));
            }
        }
#endif
        else {
            return SIMD3x3(!Elem0 ? this->values0 : other.values0, !Elem1 ? this->values1 : other.values1,
                !Elem2 ? this->values2 : other.values2, !Elem0 ? this->values3 : other.values3,
                !Elem1 ? this->values4 : other.values4, !Elem2 ? this->values5 : other.values5,
                !Elem0 ? this->values6 : other.values6, !Elem1 ? this->values7 : other.values7,
                !Elem2 ? this->values8 : other.values8);
        }
    }

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD3.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-2).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-2).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-2).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2>
    XS_INLINE SIMD3x3 shuffle3() const noexcept
    {
        static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3, "Invalid Index: Indexes must be <3");
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 23) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
                return SIMD3x3(_mm512_shuffle2200_ps(this->values));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
                return SIMD3x3(_mm512_shuffle1100_ps(this->values));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
                return SIMD3x3(_mm512_shuffle1010_ps(this->values));
            } else {
                return SIMD3x3(_mm512_shuffle1_ps(this->values, _MM_SHUFFLE(0, Index2, Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
                return SIMD3x3(_mm256_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
                return SIMD3x3(_mm256_shuffle1100_ps(this->values0), _mm_shuffle1100_ps(this->values1));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
                return SIMD3x3(_mm256_shuffle1010_ps(this->values0), _mm_shuffle1010_ps(this->values1));
            } else {
                return SIMD3x3(_mm256_shuffle1_ps(this->values0, _MM_SHUFFLE(0, Index2, Index1, Index0)),
                    _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(0, Index2, Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
                return SIMD3x3(_mm_shuffle1010_ps(this->values0), _mm_shuffle1010_ps(this->values1),
                    _mm_shuffle1010_ps(this->values2));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
                return SIMD3x3(_mm_shuffle1100_ps(this->values0), _mm_shuffle1100_ps(this->values1),
                    _mm_shuffle1100_ps(this->values2));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
                return SIMD3x3(_mm_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1),
                    _mm_shuffle2200_ps(this->values2));
            } else {
                return SIMD3x3(_mm_shuffle1_ps(this->values0, _MM_SHUFFLE(0, Index2, Index1, Index0)),
                    _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(0, Index2, Index1, Index0)),
                    _mm_shuffle1_ps(this->values2, _MM_SHUFFLE(0, Index2, Index1, Index0)));
            }
        }
#endif
        else {
            return SIMD3x3((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2],
                (&this->values0)[3 + Index0], (&this->values0)[3 + Index1], (&this->values0)[3 + Index2],
                (&this->values0)[6 + Index0], (&this->values0)[6 + Index1], (&this->values0)[6 + Index2]);
        }
    }

    /**
     * Shuffles the elements of the object by shuffling each SIMD3.
     * @tparam Index0 The index of the first SIMD3 to insert into the returned object (range is 0-2).
     * @tparam Index1 The index of the second SIMD3 to insert into the returned object (range is 0-2).
     * @tparam Index2 The index of the third SIMD3 to insert into the returned object (range is 0-2).
     * @returns Result of operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2>
    XS_INLINE SIMD3x3 shuffleH3() const noexcept
    {
        static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3, "Invalid Index: Indexes must be <3");
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD3x3(_mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(0, Index2, Index1, Index0)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 0) {
                return SIMD3x3(_mm256_shuffle32103210_ps(this->values0), _mm256_castps256_ps128(this->values0));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
                return SIMD3x3(_mm256_shuffle32103210_ps(this->values0), _mm256_extractf128_ps(this->values0, 1));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
                return SIMD3x3(_mm256_shuffle32103210_ps(this->values0), this->values1);
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
                return SIMD3x3(this->values0, _mm256_castps256_ps128(this->values0));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 1) {
                return SIMD3x3(this->values0, _mm256_extractf128_ps(this->values0, 1));
            } else if constexpr (Index0 == 0 && Index1 == 2 && Index2 == 0) {
                return SIMD3x3(
                    _mm256_insertf128_ps(this->values0, this->values1, 1), _mm256_castps256_ps128(this->values0));
            } else if constexpr (Index0 == 0 && Index1 == 2 && Index2 == 1) {
                return SIMD3x3(
                    _mm256_insertf128_ps(this->values0, this->values1, 1), _mm256_extractf128_ps(this->values0, 1));
            } else if constexpr (Index0 == 0 && Index1 == 2 && Index2 == 2) {
                return SIMD3x3(_mm256_insertf128_ps(this->values0, this->values1, 1), this->values1);
            } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 0) {
                return SIMD3x3(_mm256_shuffle32107654_ps(this->values0), _mm256_castps256_ps128(this->values0));
            } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 1) {
                const __m256 val = _mm256_shuffle32107654_ps(this->values0);
                return SIMD3x3(val, _mm256_castps256_ps128(val));
            } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 2) {
                return SIMD3x3(_mm256_shuffle32107654_ps(this->values0), this->values1);
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 0) {
                return SIMD3x3(_mm256_shuffle76547654_ps(this->values0), _mm256_castps256_ps128(this->values0));
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 1) {
                const __m256 val = _mm256_shuffle76547654_ps(this->values0);
                return SIMD3x3(val, _mm256_castps256_ps128(val));
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 2) {
                return SIMD3x3(_mm256_shuffle76547654_ps(this->values0), this->values1);
            } else if constexpr (Index0 == 1 && Index1 == 2 && Index2 == 0) {
                return SIMD3x3(
                    _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(2, 1)),
                    _mm256_castps256_ps128(this->values0));
            } else if constexpr (Index0 == 1 && Index1 == 2 && Index2 == 1) {
                const __m256 val =
                    _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(2, 1));
                return SIMD3x3(val, _mm256_castps256_ps128(val));
            } else if constexpr (Index0 == 1 && Index1 == 2 && Index2 == 2) {
                return SIMD3x3(
                    _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(2, 1)),
                    this->values1);
            } else if constexpr (Index0 == 2 && Index1 == 0 && Index2 == 0) {
                return SIMD3x3(
                    _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(0, 2)),
                    _mm256_castps256_ps128(this->values0));
            } else if constexpr (Index0 == 2 && Index1 == 0 && Index2 == 1) {
                return SIMD3x3(
                    _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(0, 2)),
                    _mm256_extractf128_ps(this->values0, 1));
            } else if constexpr (Index0 == 2 && Index1 == 0 && Index2 == 2) {
                return SIMD3x3(
                    _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(0, 2)),
                    this->values1);
            } else if constexpr (Index0 == 2 && Index1 == 1 && Index2 == 0) {
                return SIMD3x3(
                    _mm256_insertf128_ps(this->values0, this->values1, 0), _mm256_castps256_ps128(this->values0));
            } else if constexpr (Index0 == 2 && Index1 == 1 && Index2 == 1) {
                return SIMD3x3(
                    _mm256_insertf128_ps(this->values0, this->values1, 0), _mm256_extractf128_ps(this->values0, 1));
            } else if constexpr (Index0 == 2 && Index1 == 1 && Index2 == 2) {
                return SIMD3x3(_mm256_insertf128_ps(this->values0, this->values1, 0), this->values1);
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 0) {
                return SIMD3x3(_mm256_broadcastf128_ps(this->values1), _mm256_castps256_ps128(this->values0));
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 1) {
                return SIMD3x3(_mm256_broadcastf128_ps(this->values1), _mm256_extractf128_ps(this->values0, 1));
            } else /*Index0 == 2 && Index1 == 2 && Index2 == 2*/ {
                return SIMD3x3(_mm256_broadcastf128_ps(this->values1), this->values1);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3x3((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2]);
        }
#endif
        else {
            return SIMD3x3((&this->values0)[Index0 * 3], (&this->values0)[Index0 * 3 + 1],
                (&this->values0)[Index0 * 3 + 2], (&this->values0)[Index1 * 3], (&this->values0)[Index1 * 3 + 1],
                (&this->values0)[Index1 * 3 + 2], (&this->values0)[Index2 * 3], (&this->values0)[Index2 * 3 + 1],
                (&this->values0)[Index2 * 3 + 2]);
        }
    }
};

/**
 * Add two SIMD3x3s.
 * @param other1 The first SIMD3x3.
 * @param other2 SIMD3x3 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator+(const SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(
            _mm256_add_ps(other1.values0, other2.values0), _mm_add_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_add_ps(other1.values0, other2.values0), _mm_add_ps(other1.values1, other2.values1),
            _mm_add_ps(other1.values2, other2.values2));
    } else
#endif
    {
        return SIMD3x3<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values3, other1.values4 + other2.values4,
            other1.values5 + other2.values5, other1.values6 + other2.values6, other1.values7 + other2.values7,
            other1.values8 + other2.values8);
    }
}

/**
 * Add a value to all elements of a SIMD3x3.
 * @param other1 The SIMD3x3.
 * @param other2 Value to add to the SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator+(
    const SIMD3x3<T, Width>& other1, typename SIMD3x3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(_mm256_add_ps(other1.values0, other2.values),
            _mm_add_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values),
            _mm_add_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD3x3<T, Width>(other1.values0 + other2.value, other1.values1 + other2.value,
            other1.values2 + other2.value, other1.values3 + other2.value, other1.values4 + other2.value,
            other1.values5 + other2.value, other1.values6 + other2.value, other1.values7 + other2.value,
            other1.values8 + other2.value);
    }
}

/**
 * Add a SIMD3 to each triple of elements in a SIMD3x3.
 * @param other1 The SIMD3x3.
 * @param other2 Values to add to the SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator+(
    const SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_add_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(_mm256_add_ps(other1.values0, _mm256_broadcastf128_ps(other2.values)),
            _mm_add_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values),
            _mm_add_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD3x3<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values0, other1.values4 + other2.values1,
            other1.values5 + other2.values2, other1.values6 + other2.values0, other1.values7 + other2.values1,
            other1.values8 + other2.values2);
    }
}

/**
 * Subtract a SIMD3x3 from another SIMD3x3.
 * @param other1 The first SIMD3x3.
 * @param other2 SIMD3x3 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator-(const SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(
            _mm256_sub_ps(other1.values0, other2.values0), _mm_sub_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_sub_ps(other1.values0, other2.values0), _mm_sub_ps(other1.values1, other2.values1),
            _mm_sub_ps(other1.values2, other2.values2));
    } else
#endif
    {
        return SIMD3x3<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values3, other1.values4 - other2.values4,
            other1.values5 - other2.values5, other1.values6 - other2.values6, other1.values7 - other2.values7,
            other1.values8 - other2.values8);
    }
}

/**
 * Subtract a value from all elements of a SIMD3x3.
 * @param other1 The SIMD3x3.
 * @param other2 Value to subtract from the SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator-(
    const SIMD3x3<T, Width>& other1, typename SIMD3x3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(_mm256_sub_ps(other1.values0, other2.values),
            _mm_sub_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values),
            _mm_sub_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD3x3<T, Width>(other1.values0 - other2.value, other1.values1 - other2.value,
            other1.values2 - other2.value, other1.values3 - other2.value, other1.values4 - other2.value,
            other1.values5 - other2.value, other1.values6 - other2.value, other1.values7 - other2.value,
            other1.values8 - other2.value);
    }
}

/**
 * Subtract all elements of a SIMD3x3 from a value.
 * @param other1 Value to subtract from the SIMD3x3.
 * @param other2 The SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator-(
    typename SIMD3x3<T, Width>::BaseDef other1, const SIMD3x3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(_mm256_sub_ps(other1.values, other2.values0),
            _mm_sub_ps(_mm256_castps256_ps128(other1.values), other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_sub_ps(other1.values, other2.values0), _mm_sub_ps(other1.values, other2.values1),
            _mm_sub_ps(other1.values, other2.values2));
    } else
#endif
    {
        return SIMD3x3<T, Width>(other1.value - other2.values0, other1.value - other2.values1,
            other1.value - other2.values2, other1.value - other2.values3, other1.value - other2.values4,
            other1.value - other2.values5, other1.value - other2.values6, other1.value - other2.values7,
            other1.value - other2.values8);
    }
}

/**
 * Subtract a SIMD3 from each triple of elements in a SIMD3x3.
 * @param other1 The SIMD3x3.
 * @param other2 Values to subtract from the SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator-(
    const SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_sub_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(_mm256_sub_ps(other1.values0, _mm256_broadcastf128_ps(other2.values)),
            _mm_sub_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values),
            _mm_sub_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD3x3<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values0, other1.values4 - other2.values1,
            other1.values5 - other2.values2, other1.values6 - other2.values0, other1.values7 - other2.values1,
            other1.values8 - other2.values2);
    }
}

/**
 * Multiply two SIMD3x3s per element.
 * @param other1 The first SIMD3x3.
 * @param other2 The second SIMD3x3 to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator*(const SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(
            _mm256_mul_ps(other1.values0, other2.values0), _mm_mul_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_mul_ps(other1.values0, other2.values0), _mm_mul_ps(other1.values1, other2.values1),
            _mm_mul_ps(other1.values2, other2.values2));
    } else
#endif
    {
        return SIMD3x3<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values3, other1.values4 * other2.values4,
            other1.values5 * other2.values5, other1.values6 * other2.values6, other1.values7 * other2.values7,
            other1.values8 * other2.values8);
    }
}

/**
 * Multiply a SIMD3x3 by a scalar.
 * @param other1 The SIMD3x3.
 * @param other2 Value to multiply the SIMD3x3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator*(
    const SIMD3x3<T, Width>& other1, typename SIMD3x3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(_mm256_mul_ps(other1.values0, other2.values),
            _mm_mul_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values),
            _mm_mul_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD3x3<T, Width>(other1.values0 * other2.value, other1.values1 * other2.value,
            other1.values2 * other2.value, other1.values3 * other2.value, other1.values4 * other2.value,
            other1.values5 * other2.value, other1.values6 * other2.value, other1.values7 * other2.value,
            other1.values8 * other2.value);
    }
}

/**
 * Multiply each triple of elements in a SIMD3x3 by a SIMD3.
 * @param other1 The SIMD3x3.
 * @param other2 Values to multiply the SIMD3x3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator*(
    const SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_mul_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(_mm256_mul_ps(other1.values0, _mm256_broadcastf128_ps(other2.values)),
            _mm_mul_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values),
            _mm_mul_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD3x3<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values0, other1.values4 * other2.values1,
            other1.values5 * other2.values2, other1.values6 * other2.values0, other1.values7 * other2.values1,
            other1.values8 * other2.values2);
    }
}

/**
 * Divide two SIMD3x3s per element.
 * @param other1 The first SIMD3x3.
 * @param other2 The second SIMD3x3 to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator/(const SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(
            _mm256_div_ps(other1.values0, other2.values0), _mm_div_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_div_ps(other1.values0, other2.values0), _mm_div_ps(other1.values1, other2.values1),
            _mm_div_ps(other1.values2, other2.values2));
    } else
#endif
    {
        return SIMD3x3<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values3, other1.values4 / other2.values4,
            other1.values5 / other2.values5, other1.values6 / other2.values6, other1.values7 / other2.values7,
            other1.values8 / other2.values8);
    }
}

/**
 * Divide a SIMD3x3 by a scalar.
 * @param other1 The SIMD3x3.
 * @param other2 Value to divide the SIMD3x3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator/(
    const SIMD3x3<T, Width>& other1, typename SIMD3x3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(_mm256_div_ps(other1.values0, other2.values),
            _mm_div_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values),
            _mm_div_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD3x3<T, Width>(other1.values0 / other2.value, other1.values1 / other2.value,
            other1.values2 / other2.value, other1.values3 / other2.value, other1.values4 / other2.value,
            other1.values5 / other2.value, other1.values6 / other2.value, other1.values7 / other2.value,
            other1.values8 / other2.value);
    }
}

/**
 * Divide a scalar by all elements of a SIMD3x3.
 * @param other1 Value to divide the SIMD3x3 by.
 * @param other2 The SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator/(
    typename SIMD3x3<T, Width>::BaseDef other1, const SIMD3x3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(_mm256_div_ps(other1.values, other2.values0),
            _mm_div_ps(_mm256_castps256_ps128(other1.values), other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_div_ps(other1.values, other2.values0), _mm_div_ps(other1.values, other2.values1),
            _mm_div_ps(other1.values, other2.values2));
    } else
#endif
    {
        return SIMD3x3<T, Width>(other1.value / other2.values0, other1.value / other2.values1,
            other1.value / other2.values2, other1.value / other2.values3, other1.value / other2.values4,
            other1.value / other2.values5, other1.value / other2.values6, other1.value / other2.values7,
            other1.value / other2.values8);
    }
}

/**
 * Divide each triple of elements in a SIMD3x2 by a SIMD3.
 * @param other1 The SIMD3x3.
 * @param other2 Values to divide the SIMD3x3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator/(
    const SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_div_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(_mm256_div_ps(other1.values0, _mm256_broadcastf128_ps(other2.values)),
            _mm_div_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values),
            _mm_div_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD3x3<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values0, other1.values4 / other2.values1,
            other1.values5 / other2.values2, other1.values6 / other2.values0, other1.values7 / other2.values1,
            other1.values8 / other2.values2);
    }
}

/**
 * Negate all elements of a SIMD3x3.
 * @param other The SIMD3x3 to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator-(const SIMD3x3<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_sub_ps(_mm512_setzero_ps(), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_setzero_ps();
        return SIMD3x3<T, Width>(
            _mm256_sub_ps(val, other.values0), _mm_sub_ps(_mm256_castps256_ps128(val), other.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_setzero_ps();
        return SIMD3x3<T, Width>(
            _mm_sub_ps(val, other.values0), _mm_sub_ps(val, other.values1), _mm_sub_ps(val, other.values2));
    } else
#endif
    {
        return SIMD3x3<T, Width>(-other.values0, -other.values1, -other.values2, -other.values3, -other.values4,
            -other.values5, -other.values6, -other.values7, -other.values8);
    }
}

/**
 * Perform compound assignment and addition with a SIMD3x3.
 * @param [in,out] other1 The first SIMD3x3.
 * @param          other2 SIMD3x3 to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width>& operator+=(SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_add_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_add_ps(other1.values0, other2.values0);
        other1.values1 = _mm_add_ps(other1.values1, other2.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, other2.values0);
        other1.values1 = _mm_add_ps(other1.values1, other2.values1);
        other1.values2 = _mm_add_ps(other1.values2, other2.values2);
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
    }
    return other1;
}

/**
 * Perform compound assignment and addition with a SIMD3x3.
 * @param [in,out] other1 The SIMD3x3.
 * @param          other2 Value to add to the first SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width>& operator+=(SIMD3x3<T, Width>& other1, typename SIMD3x3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_add_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_add_ps(other1.values0, other2.values);
        other1.values1 = _mm_add_ps(other1.values1, _mm256_castps256_ps128(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, other2.values);
        other1.values1 = _mm_add_ps(other1.values1, other2.values);
        other1.values2 = _mm_add_ps(other1.values2, other2.values);
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
    }
    return other1;
}

/**
 * Perform compound assignment and addition of a SIMD3 to each triple of elements in a SIMD3x3.
 * @param [in,out] other1 The SIMD3x3.
 * @param          other2 Values to add to the SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width>& operator+=(
    SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_add_ps(other1.values, _mm512_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_add_ps(other1.values0, _mm256_broadcastf128_ps(other2.values));
        other1.values1 = _mm_add_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, other2.values);
        other1.values1 = _mm_add_ps(other1.values1, other2.values);
        other1.values2 = _mm_add_ps(other1.values2, other2.values);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
        other1.values2 += other2.values2;
        other1.values3 += other2.values0;
        other1.values4 += other2.values1;
        other1.values5 += other2.values2;
        other1.values6 += other2.values0;
        other1.values7 += other2.values1;
        other1.values8 += other2.values2;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction by a SIMD3x3.
 * @param [in,out] other1 The first SIMD3x3.
 * @param          other2 SIMD3x3 to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width>& operator-=(SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_sub_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_sub_ps(other1.values0, other2.values0);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, other2.values0);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values1);
        other1.values2 = _mm_sub_ps(other1.values2, other2.values2);
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
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction with a SIMD3x3.
 * @param [in,out] other1 The SIMD3x3.
 * @param          other2 Value to subtract from the first SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width>& operator-=(SIMD3x3<T, Width>& other1, typename SIMD3x3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_sub_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_sub_ps(other1.values0, other2.values);
        other1.values1 = _mm_sub_ps(other1.values1, _mm256_castps256_ps128(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, other2.values);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values);
        other1.values2 = _mm_sub_ps(other1.values2, other2.values);
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
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction of a SIMD3 to each triple of elements in a SIMD3x3.
 * @param [in,out] other1 The SIMD3x3.
 * @param          other2 Values to subtract from the SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width>& operator-=(
    SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_sub_ps(other1.values, _mm512_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_sub_ps(other1.values0, _mm256_broadcastf128_ps(other2.values));
        other1.values1 = _mm_sub_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, other2.values);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values);
        other1.values2 = _mm_sub_ps(other1.values2, other2.values);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
        other1.values2 -= other2.values2;
        other1.values3 -= other2.values0;
        other1.values4 -= other2.values1;
        other1.values5 -= other2.values2;
        other1.values6 -= other2.values0;
        other1.values7 -= other2.values1;
        other1.values8 -= other2.values2;
    }
    return other1;
}

/**
 * Perform SIMD3x3 compound assignment and multiplication.
 * @param [in,out] other1 The first SIMD3x3.
 * @param          other2 SIMD3x3 to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width>& operator*=(SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_mul_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_mul_ps(other1.values0, other2.values0);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, other2.values0);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values1);
        other1.values2 = _mm_mul_ps(other1.values2, other2.values2);
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
    }
    return other1;
}

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] other1 The SIMD3x3.
 * @param          other2 Value to multiply the SIMD3x3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width>& operator*=(SIMD3x3<T, Width>& other1, typename SIMD3x3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_mul_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_mul_ps(other1.values0, other2.values);
        other1.values1 = _mm_mul_ps(other1.values1, _mm256_castps256_ps128(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, other2.values);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values);
        other1.values2 = _mm_mul_ps(other1.values2, other2.values);
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
    }
    return other1;
}

/**
 * Perform compound assignment and multiplication of a SIMD3 to each triple of elements in a SIMD3x3.
 * @param [in,out] other1 The SIMD3x3.
 * @param          other2 Values to multiply the SIMD3x3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width>& operator*=(
    SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_mul_ps(other1.values, _mm512_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_mul_ps(other1.values0, _mm256_broadcastf128_ps(other2.values));
        other1.values1 = _mm_mul_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, other2.values);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values);
        other1.values2 = _mm_mul_ps(other1.values2, other2.values);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
        other1.values2 *= other2.values2;
        other1.values3 *= other2.values0;
        other1.values4 *= other2.values1;
        other1.values5 *= other2.values2;
        other1.values6 *= other2.values0;
        other1.values7 *= other2.values1;
        other1.values8 *= other2.values2;
    }
    return other1;
}

/**
 * Perform compound assignment and division.
 * @param [in,out] other1 The SIMD3x3.
 * @param          other2 Value to divide the SIMD3x3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width>& operator/=(SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_div_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_div_ps(other1.values0, other2.values0);
        other1.values1 = _mm_div_ps(other1.values1, other2.values1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, other2.values0);
        other1.values1 = _mm_div_ps(other1.values1, other2.values1);
        other1.values2 = _mm_div_ps(other1.values2, other2.values2);
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
    }
    return other1;
}

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] other1 The SIMD3x3.
 * @param          other2 Value to divide the SIMD3x3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width>& operator/=(SIMD3x3<T, Width>& other1, typename SIMD3x3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_div_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_div_ps(other1.values0, other2.values);
        other1.values1 = _mm_div_ps(other1.values1, _mm256_castps256_ps128(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, other2.values);
        other1.values1 = _mm_div_ps(other1.values1, other2.values);
        other1.values2 = _mm_div_ps(other1.values2, other2.values);
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
    }
    return other1;
}

/**
 * Perform compound assignment and division of each triple of elements in a SIMD3x3 by a SIMD3.
 * @param [in,out] other1 The SIMD3x3.
 * @param          other2 Values to divide the SIMD3x3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width>& operator/=(
    SIMD3x3<T, Width>& other1, const typename SIMD3x3<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_div_ps(other1.values, _mm512_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values0 = _mm256_div_ps(other1.values0, _mm256_broadcastf128_ps(other2.values));
        other1.values1 = _mm_div_ps(other1.values1, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, other2.values);
        other1.values1 = _mm_div_ps(other1.values1, other2.values);
        other1.values2 = _mm_div_ps(other1.values2, other2.values);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
        other1.values2 /= other2.values2;
        other1.values3 /= other2.values0;
        other1.values4 /= other2.values1;
        other1.values5 /= other2.values2;
        other1.values6 /= other2.values0;
        other1.values7 /= other2.values1;
        other1.values8 /= other2.values2;
    }
    return other1;
}

/**
 * And 2 SIMD3x3s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD3x3.
 * @param other2 SIMD3x3 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator&(const SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(
            _mm256_and_ps(other1.values0, other2.values0), _mm_and_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_and_ps(other1.values0, other2.values0), _mm_and_ps(other1.values1, other2.values1),
            _mm_and_ps(other1.values2, other2.values2));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x3<T, Width>(other1.values0 & other2.values0, other1.values1 & other2.values1,
                other1.values2 & other2.values2, other1.values3 & other2.values3, other1.values4 & other2.values4,
                other1.values5 & other2.values5, other1.values6 & other2.values6, other1.values7 & other2.values7,
                other1.values8 & other2.values8);
        } else {
            return SIMD3x3<T, Width>(Shift::bitAnd(other1.values0, other2.values0),
                Shift::bitAnd(other1.values1, other2.values1), Shift::bitAnd(other1.values2, other2.values2),
                Shift::bitAnd(other1.values3, other2.values3), Shift::bitAnd(other1.values4, other2.values4),
                Shift::bitAnd(other1.values5, other2.values5), Shift::bitAnd(other1.values6, other2.values6),
                Shift::bitAnd(other1.values7, other2.values7), Shift::bitAnd(other1.values8, other2.values8));
        }
    }
}

/**
 * And a SIMD3x3s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD3x3.
 * @param other2 Value to and with the SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator&(
    const SIMD3x3<T, Width>& other1, typename SIMD3x3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(_mm256_and_ps(other1.values0, other2.values),
            _mm_and_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_and_ps(other1.values0, other2.values), _mm_and_ps(other1.values1, other2.values),
            _mm_and_ps(other1.values2, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x3<T, Width>(other1.values0 & other2.value, other1.values1 & other2.value,
                other1.values2 & other2.value, other1.values3 & other2.value, other1.values4 & other2.value,
                other1.values5 & other2.value, other1.values6 & other2.value, other1.values7 & other2.value,
                other1.values8 & other2.value);
        } else {
            return SIMD3x3<T, Width>(Shift::bitAnd(other1.values0, other2.value),
                Shift::bitAnd(other1.values1, other2.value), Shift::bitAnd(other1.values2, other2.value),
                Shift::bitAnd(other1.values3, other2.value), Shift::bitAnd(other1.values4, other2.value),
                Shift::bitAnd(other1.values5, other2.value), Shift::bitAnd(other1.values6, other2.value),
                Shift::bitAnd(other1.values7, other2.value), Shift::bitAnd(other1.values8, other2.value));
        }
    }
}

/**
 * Or 2 SIMD3x3s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD3x3.
 * @param other2 SIMD3x3 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator|(const SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(
            _mm256_or_ps(other1.values0, other2.values0), _mm_or_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_or_ps(other1.values0, other2.values0), _mm_or_ps(other1.values1, other2.values1),
            _mm_or_ps(other1.values2, other2.values2));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x3<T, Width>(other1.values0 | other2.values0, other1.values1 | other2.values1,
                other1.values2 | other2.values2, other1.values3 | other2.values3, other1.values4 | other2.values4,
                other1.values5 | other2.values5, other1.values6 | other2.values6, other1.values7 | other2.values7,
                other1.values8 | other2.values8);
        } else {
            return SIMD3x3<T, Width>(Shift::bitOr(other1.values0, other2.values0),
                Shift::bitOr(other1.values1, other2.values1), Shift::bitOr(other1.values2, other2.values2),
                Shift::bitOr(other1.values3, other2.values3), Shift::bitOr(other1.values4, other2.values4),
                Shift::bitOr(other1.values5, other2.values5), Shift::bitOr(other1.values6, other2.values6),
                Shift::bitOr(other1.values7, other2.values7), Shift::bitOr(other1.values8, other2.values8));
        }
    }
}

/**
 * Or a SIMD3x3s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD3x3.
 * @param other2 Value to and with the SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator|(
    const SIMD3x3<T, Width>& other1, typename SIMD3x3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(_mm256_or_ps(other1.values0, other2.values),
            _mm_or_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_or_ps(other1.values0, other2.values), _mm_or_ps(other1.values1, other2.values),
            _mm_or_ps(other1.values2, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x3<T, Width>(other1.values0 | other2.value, other1.values1 | other2.value,
                other1.values2 | other2.value, other1.values3 | other2.value, other1.values4 | other2.value,
                other1.values5 | other2.value, other1.values6 | other2.value, other1.values7 | other2.value,
                other1.values8 | other2.value);
        } else {
            return SIMD3x3<T, Width>(Shift::bitOr(other1.values0, other2.value),
                Shift::bitOr(other1.values1, other2.value), Shift::bitOr(other1.values2, other2.value),
                Shift::bitOr(other1.values3, other2.value), Shift::bitOr(other1.values4, other2.value),
                Shift::bitOr(other1.values5, other2.value), Shift::bitOr(other1.values6, other2.value),
                Shift::bitOr(other1.values7, other2.value), Shift::bitOr(other1.values8, other2.value));
        }
    }
}

/**
 * Xor 2 SIMD3x3s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD3x3.
 * @param other2 SIMD3x3 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator^(const SIMD3x3<T, Width>& other1, const SIMD3x3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(
            _mm256_xor_ps(other1.values0, other2.values0), _mm_xor_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_xor_ps(other1.values0, other2.values0), _mm_xor_ps(other1.values1, other2.values1),
            _mm_xor_ps(other1.values2, other2.values2));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x3<T, Width>(other1.values0 ^ other2.values0, other1.values1 ^ other2.values1,
                other1.values2 ^ other2.values2, other1.values3 ^ other2.values3, other1.values4 ^ other2.values4,
                other1.values5 ^ other2.values5, other1.values6 ^ other2.values6, other1.values7 ^ other2.values7,
                other1.values8 ^ other2.values8);
        } else {
            return SIMD3x3<T, Width>(Shift::bitXor(other1.values0, other2.values0),
                Shift::bitXor(other1.values1, other2.values1), Shift::bitXor(other1.values2, other2.values2),
                Shift::bitXor(other1.values3, other2.values3), Shift::bitXor(other1.values4, other2.values4),
                Shift::bitXor(other1.values5, other2.values5), Shift::bitXor(other1.values6, other2.values6),
                Shift::bitXor(other1.values7, other2.values7), Shift::bitXor(other1.values8, other2.values8));
        }
    }
}

/**
 * Xor a SIMD3x3s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD3x3.
 * @param other2 Value to and with the SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator^(
    const SIMD3x3<T, Width>& other1, typename SIMD3x3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x3<T, Width>(_mm256_xor_ps(other1.values0, other2.values),
            _mm_xor_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x3<T, Width>(_mm_xor_ps(other1.values0, other2.values), _mm_xor_ps(other1.values1, other2.values),
            _mm_xor_ps(other1.values2, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x3<T, Width>(other1.values0 ^ other2.value, other1.values1 ^ other2.value,
                other1.values2 ^ other2.value, other1.values3 ^ other2.value, other1.values4 ^ other2.value,
                other1.values5 ^ other2.value, other1.values6 ^ other2.value, other1.values7 ^ other2.value,
                other1.values8 ^ other2.value);
        } else {
            return SIMD3x3<T, Width>(Shift::bitXor(other1.values0, other2.value),
                Shift::bitXor(other1.values1, other2.value), Shift::bitXor(other1.values2, other2.value),
                Shift::bitXor(other1.values3, other2.value), Shift::bitXor(other1.values4, other2.value),
                Shift::bitXor(other1.values5, other2.value), Shift::bitXor(other1.values6, other2.value),
                Shift::bitXor(other1.values7, other2.value), Shift::bitXor(other1.values8, other2.value));
        }
    }
}

/**
 * Not a SIMD3x3.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other The SIMD3x3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x3<T, Width> operator~(const SIMD3x3<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD3x3<T, Width>(_mm512_xor_ps(other.values, _mm512_castsi512_ps(_mm512_set1_epi32(-1))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val = _mm256_undefined_ps();
        val = _mm256_cmp_ps(val, val, _CMP_EQ_UQ);
        return SIMD3x3<T, Width>(
            _mm256_xor_ps(other.values0, val), _mm_xor_ps(other.values1, _mm256_castps256_ps128(val)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_undefined_ps();
        val = _mm_cmpeq_ps(val, val);
        return SIMD3x3<T, Width>(
            _mm_xor_ps(other.values0, val), _mm_xor_ps(other.values1, val), _mm_xor_ps(other.values2, val));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x3<T, Width>(~other.values0, ~other.values1, ~other.values2, ~other.values3, ~other.values4,
                ~other.values5, ~other.values6, ~other.values7, ~other.values8);
        } else {
            return SIMD3x3<T, Width>(Shift::bitNot(other.values0), Shift::bitNot(other.values1),
                Shift::bitNot(other.values2), Shift::bitNot(other.values3), Shift::bitNot(other.values4),
                Shift::bitNot(other.values5), Shift::bitNot(other.values6), Shift::bitNot(other.values7),
                Shift::bitNot(other.values8));
        }
    }
}
} // namespace Shift
