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

#include "SIMD/XSSIMD3x3.hpp"
#include "SIMD/XSSIMD8.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class SIMD12;
template<typename T, SIMDWidth Width>
class SIMD3x4;

template<typename T>
class alignas(maxAlignment<T, 4>) SIMD12Data
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    T value0, value1, value2, value3;
    T value4, value5, value6, value7;
    T value8, value9, value10, value11;

    /** Default constructor. */
    XS_INLINE SIMD12Data() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE SIMD12Data(const SIMD12Data& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE SIMD12Data(SIMD12Data&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMD12Data& operator=(const SIMD12Data& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMD12Data& operator=(SIMD12Data&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit SIMD12Data(const SIMD12<T, Width>& other) noexcept
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
     */
    XS_INLINE void setData(T other0, T other1, T other2, T other3, T other4, T other5, T other6, T other7, T other8,
        T other9, T other10, T other11) noexcept
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
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const SIMD12<T, Width>& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            _mm512_mask_storeu_ps(&value0, _cvtu32_mask16(0x0FFF), other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            _mm256_storeu_ps(&value0, other.values0);
            _mm_store_ps(&value8, other.values1);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            _mm_store_ps(&value0, other.values0);
            _mm_store_ps(&value4, other.values1);
            _mm_store_ps(&value8, other.values2);
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
        }
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD<T>>
    XS_INLINE SIMD12<T, Width> load() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD12<T, Width>(_mm512_loadu_ps(&value0));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD12<T, Width>(_mm256_loadu_ps(&value0), _mm_load_ps(&value8));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD12<T, Width>(_mm_load_ps(&value0), _mm_load_ps(&value4), _mm_load_ps(&value8));
        } else
#endif
        {
            return SIMD12<T, Width>(
                value0, value1, value2, value3, value4, value5, value6, value7, value8, value9, value10, value11);
        }
    }
};

template<typename T>
class alignas(maxAlignment<T, 16>) SIMD12DataPad
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    T value0, value1, value2, value3;
    T value4, value5, value6, value7;
    T value8, value9, value10, value11;
    T pad0, pad1, pad2, pad3;

    /** Default constructor. */
    XS_INLINE SIMD12DataPad() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE SIMD12DataPad(const SIMD12DataPad& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE SIMD12DataPad(SIMD12DataPad&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMD12DataPad& operator=(const SIMD12DataPad& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMD12DataPad& operator=(SIMD12DataPad&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit SIMD12DataPad(const SIMD12<T, Width>& other) noexcept
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
     */
    XS_INLINE void setData(T other0, T other1, T other2, T other3, T other4, T other5, T other6, T other7, T other8,
        T other9, T other10, T other11) noexcept
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
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const SIMD12<T, Width>& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            _mm512_mask_store_ps(&value0, _cvtu32_mask16(0x0FFF), other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            _mm256_store_ps(&value0, other.values0);
            _mm_store_ps(&value8, other.values1);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            _mm_store_ps(&value0, other.values0);
            _mm_store_ps(&value4, other.values1);
            _mm_store_ps(&value8, other.values2);
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
        }
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD<T>>
    XS_INLINE SIMD12<T, Width> load() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD12<T, Width>(_mm512_load_ps(&value0));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD12<T, Width>(_mm256_load_ps(&value0), _mm_load_ps(&value8));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD12<T, Width>(_mm_load_ps(&value0), _mm_load_ps(&value4), _mm_load_ps(&value8));
        } else
#endif
        {
            return SIMD12<T, Width>(
                value0, value1, value2, value3, value4, value5, value6, value7, value8, value9, value10, value11);
        }
    }
};

template<typename T, SIMDWidth Width = defaultWidthSIMD<T>>
class SIMD12 : public NoExport::SIMDData<T, 12, 0, Width>
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    using Type = T;
    using InternalData = NoExport::SIMDData<T, 12, 0, Width>;
    using Data = SIMD12Data<T>;
    using DataPad = SIMD12DataPad<T>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = InternalData::width;
    static constexpr uint32 numValues = 12;
    static constexpr uint32 size = InternalData::size;
    using BaseDef = SIMDBase<T, SIMDBase<T, widthImpl>::widthImpl>;
    using InBaseDef = SIMDInBase<T, SIMDInBase<T, widthImpl>::widthImpl>;
    using SIMD2Def = SIMD2<T, SIMD2<T, widthImpl>::widthImpl>;
    using SIMD3Def = SIMD3<T, SIMD3<T, widthImpl>::widthImpl>;
    using SIMD4Def = SIMD4<T, SIMD4<T, widthImpl>::widthImpl>;
    using SIMD6Def = SIMD6<T, SIMD6<T, widthImpl>::widthImpl>;
    using SIMD8Def = SIMD8<T, SIMD8<T, widthImpl>::widthImpl>;
    using SIMD3x3Def = SIMD3x3<T, SIMD3x3<T, widthImpl>::widthImpl>;
    using InternalData::SIMDData;

    /**
     * Shuffle class used to store shuffle information for SIMD12s. Currently limited to only shuffling within dual or
     * quad values.
     */
    class Shuffle : public NoExport::SIMDShuffleData<T, 12, 0, Width>
    {
    public:
        using Type = T;
        using InternalData = NoExport::SIMDShuffleData<T, 12, 0, Width>;
        static constexpr SIMDWidth width = Width;
        static constexpr SIMDWidth widthImpl = InternalData::width;
        using InternalData::SIMDShuffleData;

        /** Default constructor. */
        XS_INLINE Shuffle() noexcept = default;

        /**
         * Copy constructor.
         * @param other The other.
         */
        XS_INLINE Shuffle(const Shuffle& other) = default;

        /**
         * Move constructor.
         * @param [in,out] other The other.
         */
        XS_INLINE Shuffle(Shuffle&& other) noexcept = default;

        /**
         * Assignment operator.
         * @param other The other.
         * @returns A shallow copy of this object.
         */
        XS_INLINE Shuffle& operator=(const Shuffle& other) = default;

        /**
         * Move assignment operator.
         * @param [in,out] other The other.
         * @returns A shallow copy of this object.
         */
        XS_INLINE Shuffle& operator=(Shuffle&& other) noexcept = default;

        /**
         * Construct a shuffle from integer components.
         * @param shuffle0  The first input integer component (range is 0-3).
         * @param shuffle1  The second input integer component (range is 0-3).
         * @param shuffle2  The third input integer component (range is 0-3).
         * @param shuffle3  The fourth input integer component (range is 0-3).
         * @param shuffle4  The fifth input integer component (range is 0-3).
         * @param shuffle5  The sixth input integer component (range is 0-3).
         * @param shuffle6  The seventh input integer component (range is 0-3).
         * @param shuffle7  The eight input integer component (range is 0-3).
         * @param shuffle8  The ninth input integer component (range is 0-3).
         * @param shuffle9  The tenth input integer component (range is 0-3).
         * @param shuffle10 The eleventh input integer component (range is 0-3).
         * @param shuffle11 The twelfth input integer component (range is 0-3).
         */
        XS_INLINE Shuffle(uint32 shuffle0, uint32 shuffle1, uint32 shuffle2, uint32 shuffle3, uint32 shuffle4,
            uint32 shuffle5, uint32 shuffle6, uint32 shuffle7, uint32 shuffle8, uint32 shuffle9, uint32 shuffle10,
            uint32 shuffle11) noexcept
        {
            XS_ASSERT(shuffle0 < 4);
            XS_ASSERT(shuffle1 < 4);
            XS_ASSERT(shuffle2 < 4);
            XS_ASSERT(shuffle3 < 4);
            XS_ASSERT(shuffle4 >= 4 && shuffle4 < 8);
            XS_ASSERT(shuffle5 >= 4 && shuffle5 < 8);
            XS_ASSERT(shuffle6 >= 4 && shuffle6 < 8);
            XS_ASSERT(shuffle7 >= 4 && shuffle7 < 8);
            XS_ASSERT(shuffle8 >= 8 && shuffle8 < 12);
            XS_ASSERT(shuffle9 >= 8 && shuffle9 < 12);
            XS_ASSERT(shuffle10 >= 8 && shuffle10 < 12);
            XS_ASSERT(shuffle11 >= 8 && shuffle11 < 12);
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                this->values = _mm512_set_epi32(0, 0, 0, 0, shuffle11 - 8, shuffle10 - 8, shuffle9 - 8, shuffle8 - 8,
                    shuffle7 - 4, shuffle6 - 4, shuffle5 - 4, shuffle4 - 4, shuffle3, shuffle2, shuffle1, shuffle0);
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                this->values0 = _mm256_set_epi32(
                    shuffle7 - 4, shuffle6 - 4, shuffle5 - 4, shuffle4 - 4, shuffle3, shuffle2, shuffle1, shuffle0);
                this->values1 = _mm_set_epi32(shuffle11 - 8, shuffle10 - 8, shuffle9 - 8, shuffle8 - 8);
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    this->values0 = _mm_set_epi32(shuffle3, shuffle2, shuffle1, shuffle0);
                    this->values1 = _mm_set_epi32(shuffle7 - 4, shuffle6 - 4, shuffle5 - 4, shuffle4 - 4);
                    this->values2 = _mm_set_epi32(shuffle11 - 8, shuffle10 - 8, shuffle9 - 8, shuffle8 - 8);
                } else {
                    this->values0 =
                        _mm_set_epi32((shuffle3 * 0x04040404) + 0x03020100, (shuffle2 * 0x04040404) + 0x03020100,
                            (shuffle1 * 0x04040404) + 0x03020100, (shuffle0 * 0x04040404) + 0x03020100);
                    this->values1 = _mm_set_epi32(((shuffle7 - 4) * 0x04040404) + 0x03020100,
                        ((shuffle6 - 4) * 0x04040404) + 0x03020100, ((shuffle5 - 4) * 0x04040404) + 0x03020100,
                        ((shuffle4 - 4) * 0x04040404) + 0x03020100);
                    this->values2 = _mm_set_epi32(((shuffle11 - 8) * 0x04040404) + 0x03020100,
                        ((shuffle10 - 8) * 0x04040404) + 0x03020100, ((shuffle9 - 8) * 0x04040404) + 0x03020100,
                        ((shuffle8 - 8) * 0x04040404) + 0x03020100);
                }
            } else
#endif
            {
                this->values0 = shuffle0;
                this->values1 = shuffle1;
                this->values2 = shuffle2;
                this->values3 = shuffle3;
                this->values4 = shuffle4;
                this->values5 = shuffle5;
                this->values6 = shuffle6;
                this->values7 = shuffle7;
                this->values8 = shuffle8;
                this->values9 = shuffle9;
                this->values10 = shuffle10;
                this->values11 = shuffle11;
            }
        }

        /**
         * Constructor to build a Shuffle that swaps the high element with the low element of each internal pair of
         * values.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_INLINE static Shuffle Swap2() noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return Shuffle(_mm512_set_epi32(0, 0, 0, 0, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1));
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                const __m256i swap = _mm256_set_epi32(2, 3, 0, 1, 2, 3, 0, 1);
                return Shuffle(swap, _mm256_castsi256_si128(swap));
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    const __m128i swap = _mm_set_epi32(2, 3, 0, 1);
                    return Shuffle(swap, swap, swap);
                } else {
                    const __m128i swap = _mm_set_epi32(0x0B0A0908, 0x0F0E0D0C, 0x03020100, 0x07060504);
                    return Shuffle(swap, swap, swap);
                }
            } else
#endif
            {
                return Shuffle(
                    1_ui32, 0_ui32, 3_ui32, 2_ui32, 5_ui32, 4_ui32, 7_ui32, 6_ui32, 9_ui32, 8_ui32, 11_ui32, 10_ui32);
            }
        }

        /**
         * Constructor to build a Shuffle that swaps the high element with the low element of each internal pair of
         * values with a mask.
         * @note Where the mask is valid the shuffled value will be output else the
         * original an shuffled value will be copied.
         * @param mask Mask used to determine which element pairs are swapped.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_INLINE static Shuffle Swap2(const typename SIMD6Def::Mask& mask) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                uint32 val = _cvtmask8_u32(mask.values);
                val = _pdep_u32(val, 0x555);
                val = val | (val << 1);
                const __m512i norm = _mm512_set_epi32(0, 0, 0, 0, 3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0);
                return Shuffle(_mm512_mask_shuffle_epi32(norm, _cvtu32_mask16(val), norm, _MM_SHUFFLE(2, 3, 0, 1)));
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    uint32 val = _cvtmask8_u32(mask.values);
                    val = _pdep_u32(val, 0x555);
                    val = val | (val << 1);
                    const __m256i norm = _mm256_set_epi32(3, 2, 1, 0, 3, 2, 1, 0);
                    return Shuffle(_mm256_mask_shuffle_epi32(norm, _cvtu32_mask8(val), norm, _MM_SHUFFLE(2, 3, 0, 1)),
                        _mm_mask_shuffle_epi32(_mm256_castsi256_si128(norm), _cvtu32_mask8(val >> 8),
                            _mm256_castsi256_si128(norm), _MM_SHUFFLE(2, 3, 0, 1)));
                } else if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                    // Swap holds a 1 (or 4 in non AVX) if the mask is positive or 0 if it is negative. When Norm is
                    // Xored with 0 it has no effect but the binary xor with 1 results in the sequence 2,3,0,1 which is
                    // what we want in order to swap the top and bottom half
                    const __m256i norm = _mm256_set_epi32(3, 2, 1, 0, 3, 2, 1, 0);
                    const __m256i swap = _mm256_and_si256(
                        _mm256_shuffle_epi32(norm, _MM_SHUFFLE(1, 1, 1, 1)), _mm256_castps_si256(mask.values));
                    const __m256i lo = _mm256_shuffle_epi32(swap, _MM_SHUFFLE(1, 1, 0, 0));
                    const __m256i hi = _mm256_shuffle_epi32(swap, _MM_SHUFFLE(3, 3, 2, 2));
                    return Shuffle(_mm256_xor_si256(_mm256_permute2x128_si256(lo, hi, _MM256_PERMUTE(2, 0)), norm),
                        _mm_xor_si128(_mm256_extracti128_si256(lo, 1), _mm256_castsi256_si128(norm)));
                } else {
                    const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
                    const __m128i swap = _mm_shuffle1111_epi32(norm);
                    const __m256i maski = _mm256_castps_si256(mask.values);
                    const __m128i swap1 = _mm_and_si128(swap, _mm256_castsi256_si128(maski));
                    const __m128i swap2 = _mm_and_si128(swap, _mm256_extractf128_si256(maski, 1));
                    return Shuffle(
                        _mm256_insertf128_si256(_mm256_castsi128_si256(_mm_xor_si128(
                                                    _mm_shuffle_epi32(swap1, _MM_SHUFFLE(1, 1, 0, 0)), norm)),
                            _mm_xor_si128(_mm_shuffle_epi32(swap1, _MM_SHUFFLE(3, 3, 2, 2)), norm), 1),
                        _mm_xor_si128(_mm_shuffle_epi32(swap2, _MM_SHUFFLE(1, 1, 0, 0)), norm));
                }
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    uint32 val = _cvtmask8_u32(mask.values0) | (_cvtmask8_u32(mask.values1) << 4);
                    val = _pdep_u32(val, 0x555);
                    val = val | (val << 1);
                    const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
                    return Shuffle(_mm_mask_shuffle_epi32(norm, _cvtu32_mask8(val), norm, _MM_SHUFFLE(2, 3, 0, 1)),
                        _mm_mask_shuffle_epi32(norm, _cvtu32_mask8(val >> 4), norm, _MM_SHUFFLE(2, 3, 0, 1)),
                        _mm_mask_shuffle_epi32(norm, _cvtu32_mask8(val >> 8), norm, _MM_SHUFFLE(2, 3, 0, 1)));
                } else {
                    __m128i norm, swap;
                    if constexpr (hasISAFeature<ISAFeature::AVX>) {
                        norm = _mm_set_epi32(3, 2, 1, 0);
                        swap = _mm_shuffle1111_epi32(norm);
                    } else {
                        norm = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
                        swap = _mm_set1_epi8(4);
                    }
                    const __m128i swap1 = _mm_and_si128(swap, _mm_castps_si128(mask.values0));
                    const __m128i swap2 = _mm_and_si128(swap, _mm_castps_si128(mask.values1));
                    return Shuffle(_mm_xor_si128(_mm_shuffle_epi32(swap1, _MM_SHUFFLE(1, 1, 0, 0)), norm),
                        _mm_xor_si128(_mm_shuffle_epi32(swap1, _MM_SHUFFLE(3, 3, 2, 2)), norm),
                        _mm_xor_si128(_mm_shuffle_epi32(swap2, _MM_SHUFFLE(1, 1, 0, 0)), norm));
                }
            } else
#endif
            {
                return Shuffle(static_cast<uint32>(mask.values0), 1_ui32 ^ static_cast<uint32>(mask.values0),
                    2_ui32 ^ static_cast<uint32>(mask.values1), 3_ui32 ^ static_cast<uint32>(mask.values1),
                    4_ui32 ^ static_cast<uint32>(mask.values2), 5_ui32 ^ static_cast<uint32>(mask.values2),
                    6_ui32 ^ static_cast<uint32>(mask.values3), 7_ui32 ^ static_cast<uint32>(mask.values3),
                    8_ui32 ^ static_cast<uint32>(mask.values4), 9_ui32 ^ static_cast<uint32>(mask.values4),
                    10_ui32 ^ static_cast<uint32>(mask.values5), 11_ui32 ^ static_cast<uint32>(mask.values5));
            }
        }

        /**
         * Constructor to build a Shuffle that swaps the high 2 elements with the low 2 elements of each internal quad
         * value.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_INLINE static Shuffle Swap4() noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return Shuffle(_mm512_set_epi32(0, 0, 0, 0, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0, 3, 2));
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                const __m256i swap = _mm256_set_epi32(1, 0, 3, 2, 1, 0, 3, 2);
                return Shuffle(swap, _mm256_castsi256_si128(swap));
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    const __m128i swap = _mm_set_epi32(1, 0, 3, 2);
                    return Shuffle(swap, swap, swap);
                } else {
                    const __m128i swap = _mm_set_epi32(0x07060504, 0x03020100, 0x0F0E0D0C, 0x0B0A0908);
                    return Shuffle(swap, swap, swap);
                }
            } else
#endif
            {
                return Shuffle(
                    2_ui32, 3_ui32, 0_ui32, 1_ui32, 6_ui32, 7_ui32, 4_ui32, 5_ui32, 10_ui32, 11_ui32, 8_ui32, 9_ui32);
            }
        }

        /**
         * Constructor to build a Shuffle that swaps the high 2 elements with the low 2 elements of each internal quad
         * value with a mask.
         * @note Where the mask is valid the shuffled value will be output else the
         * original an shuffled value will be copied.
         * @param mask Mask used to determine which element pairs are swapped.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_INLINE static Shuffle Swap4(const typename SIMD3Def::Mask& mask) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                uint32 val = _cvtmask8_u32(mask.values);
                val = _pdep_u32(val, 0x111);
                val = val | (val << 1) | (val << 2) | (val << 3);
                const __m512i norm = _mm512_set_epi32(0, 0, 0, 0, 3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0);
                return Shuffle(_mm512_mask_shuffle_epi32(norm, _cvtu32_mask16(val), norm, _MM_SHUFFLE(1, 0, 3, 2)));
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    uint32 val = _cvtmask8_u32(mask.values);
                    val = _pdep_u32(val, 0x111);
                    val = val | (val << 1) | (val << 2) | (val << 3);
                    const __m256i norm = _mm256_set_epi32(3, 2, 1, 0, 3, 2, 1, 0);
                    return Shuffle(_mm256_mask_shuffle_epi32(norm, _cvtu32_mask8(val), norm, _MM_SHUFFLE(1, 0, 3, 2)),
                        _mm_mask_shuffle_epi32(_mm256_castsi256_si128(norm), _cvtu32_mask8(val >> 8),
                            _mm256_castsi256_si128(norm), _MM_SHUFFLE(1, 0, 3, 2)));
                } else if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                    // Swap holds a 2 (or 8 in non AVX) if the mask is positive or 0 if it is negative. When Norm is
                    // Xored with 0 it has no effect but the binary xor with 2 results in the sequence 1,0,3,2 which is
                    // what we want in order to swap the top and bottom half
                    const __m256i norm = _mm256_set_epi32(3, 2, 1, 0, 3, 2, 1, 0);
                    __m128i swap = _mm_shuffle2222_epi32(_mm256_castsi256_si128(norm));
                    swap = _mm_and_si128(swap, _mm_castps_si128(mask.values));
                    return Shuffle(
                        _mm256_xor_si256(_mm256_inserti128_si256(_mm256_castsi128_si256(_mm_shuffle0000_epi32(swap)),
                                             _mm_shuffle1111_epi32(swap), 1),
                            norm),
                        _mm_xor_si128(_mm_shuffle2222_epi32(swap), _mm256_castsi256_si128(norm)));
                } else {
                    const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
                    __m128i swap = _mm_shuffle2222_epi32(norm);
                    swap = _mm_and_si128(swap, _mm_castps_si128(mask.values));
                    return Shuffle(_mm256_insertf128_si256(
                                       _mm256_castsi128_si256(_mm_xor_si128(_mm_shuffle0000_epi32(swap), norm)),
                                       _mm_xor_si128(_mm_shuffle1111_epi32(swap), norm), 1),
                        _mm_xor_si128(_mm_shuffle2222_epi32(swap), norm));
                }
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    uint32 val = _cvtmask8_u32(mask.values);
                    val = _pdep_u32(val, 0x111);
                    val = val | (val << 1) | (val << 2) | (val << 3);
                    const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
                    return Shuffle(_mm_mask_shuffle_epi32(norm, _cvtu32_mask8(val), norm, _MM_SHUFFLE(1, 0, 3, 2)),
                        _mm_mask_shuffle_epi32(norm, _cvtu32_mask8(val >> 4), norm, _MM_SHUFFLE(1, 0, 3, 2)),
                        _mm_mask_shuffle_epi32(norm, _cvtu32_mask8(val >> 8), norm, _MM_SHUFFLE(1, 0, 3, 2)));
                } else {
                    __m128i norm, swap;
                    if constexpr (hasISAFeature<ISAFeature::AVX>) {
                        norm = _mm_set_epi32(3, 2, 1, 0);
                        swap = _mm_shuffle2222_epi32(norm);
                    } else {
                        norm = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
                        swap = _mm_set1_epi8(8);
                    }
                    swap = _mm_and_si128(swap, _mm_castps_si128(mask.values));
                    return Shuffle(_mm_xor_si128(_mm_shuffle0000_epi32(swap), norm),
                        _mm_xor_si128(_mm_shuffle1111_epi32(swap), norm),
                        _mm_xor_si128(_mm_shuffle2222_epi32(swap), norm));
                }
            } else
#endif
            {
                return Shuffle(static_cast<uint32>(mask.values0) << 1_ui8,
                    1_ui32 ^ (static_cast<uint32>(mask.values0) << 1_ui8),
                    2_ui32 ^ (static_cast<uint32>(mask.values0) << 1_ui8),
                    3_ui32 ^ (static_cast<uint32>(mask.values0) << 1_ui8),
                    4_ui32 ^ (static_cast<uint32>(mask.values1) << 1_ui8),
                    5_ui32 ^ (static_cast<uint32>(mask.values1) << 1_ui8),
                    6_ui32 ^ (static_cast<uint32>(mask.values1) << 1_ui8),
                    7_ui32 ^ (static_cast<uint32>(mask.values1) << 1_ui8),
                    8_ui32 ^ (static_cast<uint32>(mask.values2) << 1_ui8),
                    9_ui32 ^ (static_cast<uint32>(mask.values2) << 1_ui8),
                    10_ui32 ^ (static_cast<uint32>(mask.values2) << 1_ui8),
                    11_ui32 ^ (static_cast<uint32>(mask.values2) << 1_ui8));
            }
        }
    };

    /** Default constructor. */
    XS_INLINE SIMD12() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE SIMD12(const SIMD12& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE SIMD12(SIMD12&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMD12& operator=(const SIMD12& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMD12& operator=(SIMD12&& other) noexcept = default;

    /**
     * Construct a from 12 different values.
     * @param value0  The first value.
     * @param value1  The second value.
     * @param value2  The third value.
     * @param value3  The fourth value.
     * @param value4  The fifth value.
     * @param value5  The sixth value.
     * @param value6  The seventh value.
     * @param value7  The eighth value.
     * @param value12 The ninth value.
     * @param value9  The tenth value.
     * @param value10 The eleventh value.
     * @param value11 The twelfth value.
     */
    XS_INLINE SIMD12(T value0, T value1, T value2, T value3, T value4, T value5, T value6, T value7, T value8, T value9,
        T value10, T value11) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_set_ps(0, 0, 0, 0, value11, value10, value9, value8, value7, value6, value5, value4,
                value3, value2, value1, value0);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = _mm256_set_ps(value7, value6, value5, value4, value3, value2, value1, value0);
            this->values1 = _mm_set_ps(value11, value10, value9, value8);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_set_ps(value3, value2, value1, value0);
            this->values1 = _mm_set_ps(value7, value6, value5, value4);
            this->values2 = _mm_set_ps(value11, value10, value9, value8);
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
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param value Value to set all elements to.
     */
    XS_INLINE explicit SIMD12(T value) noexcept
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
            this->values9 = value;
            this->values10 = value;
            this->values11 = value;
        }
    }

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_INLINE explicit SIMD12(SIMD12<T, Width2> other) noexcept
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
                this->values = _mm512_insertf128_ps(
                    _mm512_castps128_ps512(other.values2), _mm256_set_m128(other.values1, other.values0));
            } else if constexpr (isSame<T, float32> && (Width >= SIMDWidth::B64) && (Width2 == SIMDWidth::B32)) {
                this->values = _mm512_insertf128_ps(_mm512_castps128_ps512(other.values1), other.values0);
            } else if constexpr (isSame<T, float32> && (Width >= SIMDWidth::B64) && (Width2 >= SIMDWidth::B64)) {
                this->values = other.values;
            }
        } else if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B64)) {
                this->values = _mm512_set_ps(0.0f, 0.0f, 0.0f, 0.0f, other.values11, other.values10, other.values9,
                    other.values8, other.values7, other.values6, other.values5, other.values4, other.values3,
                    other.values2, other.values1, other.values0);
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width == SIMDWidth::B32)) {
                this->values0 = _mm256_set_ps(other.values7, other.values6, other.values5, other.values4, other.values3,
                    other.values2, other.values1, other.values0);
                this->values1 = _mm_set_ps(other.values11, other.values10, other.values9, other.values8);
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width == SIMDWidth::B16)) {
                this->values0 = _mm_set_ps(other.values3, other.values2, other.values1, other.values0);
                this->values1 = _mm_set_ps(other.values7, other.values6, other.values5, other.values4);
                this->values2 = _mm_set_ps(other.values11, other.values10, other.values9, other.values8);
            }
        } else if constexpr (Width2 > SIMDWidth::Scalar) {
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width2 >= SIMDWidth::B64)) {
                this->values0 = _mm_cvtss_f32(_mm512_castps512_ps128(other.values));
                this->values1 = _mm_cvtss_f32(_mm_shuffle3311_ps(_mm512_castps512_ps128(other.values)));
                this->values2 = _mm_cvtss_f32(_mm_shuffle3232_ps(_mm512_castps512_ps128(other.values)));
                this->values3 = _mm_cvtss_f32(_mm_shuffle3333_ps(_mm512_castps512_ps128(other.values)));
                const auto hi = _mm512_extractf128_ps(this->values, 1);
                this->values4 = _mm_cvtss_f32(hi);
                this->values5 = _mm_cvtss_f32(_mm_shuffle3311_ps(hi));
                this->values6 = _mm_cvtss_f32(_mm_shuffle3232_ps(hi));
                this->values7 = _mm_cvtss_f32(_mm_shuffle3333_ps(hi));
                const auto hi2 = _mm512_extractf128_ps(this->values, 1);
                this->values8 = _mm_cvtss_f32(hi2);
                this->values9 = _mm_cvtss_f32(_mm_shuffle3311_ps(hi2));
                this->values10 = _mm_cvtss_f32(_mm_shuffle3232_ps(hi2));
                this->values11 = _mm_cvtss_f32(_mm_shuffle3333_ps(hi2));
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width2 == SIMDWidth::B32)) {
                this->values0 = _mm_cvtss_f32(_mm256_castps256_ps128(other.values0));
                this->values1 = _mm_cvtss_f32(_mm_shuffle3311_ps(_mm256_castps256_ps128(other.values0)));
                this->values2 = _mm_cvtss_f32(_mm_shuffle3232_ps(_mm256_castps256_ps128(other.values0)));
                this->values3 = _mm_cvtss_f32(_mm_shuffle3333_ps(_mm256_castps256_ps128(other.values0)));
                const auto hi = _mm256_extractf128_ps(this->values, 1);
                this->values4 = _mm_cvtss_f32(hi);
                this->values5 = _mm_cvtss_f32(_mm_shuffle3311_ps(hi));
                this->values6 = _mm_cvtss_f32(_mm_shuffle3232_ps(hi));
                this->values7 = _mm_cvtss_f32(_mm_shuffle3333_ps(hi));
                this->values8 = _mm_cvtss_f32(_mm256_castps256_ps128(other.values1));
                this->values9 = _mm_cvtss_f32(_mm_shuffle3311_ps(_mm256_castps256_ps128(other.values1)));
                this->values10 = _mm_cvtss_f32(_mm_shuffle3232_ps(_mm256_castps256_ps128(other.values1)));
                this->values11 = _mm_cvtss_f32(_mm_shuffle3333_ps(_mm256_castps256_ps128(other.values1)));
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width2 == SIMDWidth::B16)) {
                this->values0 = _mm_cvtss_f32(other.values0);
                this->values1 = _mm_cvtss_f32(_mm_shuffle3311_ps(other.values0)); //(x,x,x,1)
                this->values2 = _mm_cvtss_f32(_mm_shuffle3232_ps(other.values0)); //(x,x,x,2)
                this->values3 = _mm_cvtss_f32(_mm_shuffle3333_ps(other.values0)); //(x,x,x,3)
                this->values4 = _mm_cvtss_f32(other.values1);
                this->values5 = _mm_cvtss_f32(_mm_shuffle3311_ps(other.values1)); //(x,x,x,1)
                this->values6 = _mm_cvtss_f32(_mm_shuffle3232_ps(other.values1)); //(x,x,x,2)
                this->values7 = _mm_cvtss_f32(_mm_shuffle3333_ps(other.values1)); //(x,x,x,3)
                this->values8 = _mm_cvtss_f32(other.values2);
                this->values9 = _mm_cvtss_f32(_mm_shuffle3311_ps(other.values2));  //(x,x,x,1)
                this->values10 = _mm_cvtss_f32(_mm_shuffle3232_ps(other.values2)); //(x,x,x,2)
                this->values11 = _mm_cvtss_f32(_mm_shuffle3333_ps(other.values2)); //(x,x,x,3)
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
            this->values9 = other.values9;
            this->values10 = other.values10;
            this->values11 = other.values11;
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_INLINE explicit SIMD12(BaseDef other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = other.values;
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = other.values;
            this->values1 = _mm256_castps256_ps128(other.values);
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
            this->values9 = other.value;
            this->values10 = other.value;
            this->values11 = other.value;
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_INLINE explicit SIMD12(InBaseDef other) noexcept
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
            this->values9 = other.value;
            this->values10 = other.value;
            this->values11 = other.value;
        }
    }

    /**
     * Construct from 6 different SIMD2s.
     * @param other0 The first SIMD2.
     * @param other1 The second SIMD2.
     * @param other2 The third SIMD2.
     * @param other3 The fourth SIMD2.
     * @param other4 The fifth SIMD2.
     * @param other5 The sixth SIMD2.
     */
    XS_INLINE SIMD12(const SIMD2Def& other0, const SIMD2Def& other1, const SIMD2Def& other2, const SIMD2Def& other3,
        const SIMD2Def& other4, const SIMD2Def& other5) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values =
                _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(_mm_movelh_ps(other2.values, other3.values),
                                       _mm_movelh_ps(other0.values, other1.values))),
                    _mm_movelh_ps(other4.values, other5.values), 2);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = _mm256_set_m128(
                _mm_movelh_ps(other2.values, other3.values), _mm_movelh_ps(other0.values, other1.values));
            this->values1 = _mm_movelh_ps(other4.values, other5.values);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_movelh_ps(other0.values, other1.values);
            this->values1 = _mm_movelh_ps(other2.values, other3.values);
            this->values2 = _mm_movelh_ps(other4.values, other5.values);
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
        }
    }

    /**
     * Construct from 4 different SIMD3s.
     * @note Note SIMD3s are packed when put into a SIMD12. This means that the first 4 elements will
     * actually consist of the packed first element of each of the 4 SIMD3s etc.
     * @param other0 The first SIMD3.
     * @param other1 The second SIMD3.
     * @param other2 The third SIMD3.
     * @param other3 The fourth SIMD3.
     */
    XS_INLINE SIMD12(
        const SIMD3Def& other0, const SIMD3Def& other1, const SIMD3Def& other2, const SIMD3Def& other3) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 val1 = _mm_movehl_ps(other0.values, other1.values);
            const __m128 val12 = _mm_movelh_ps(other0.values, other1.values);
            const __m128 val34 = _mm_movelh_ps(other2.values, other3.values);
            const __m128 val2 = _mm_movehl_ps(other2.values, other3.values);
            this->values = _mm512_insertf32x4(
                _mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle_ps(val12, val34, _MM_SHUFFLE(3, 1, 3, 1)),
                    _mm_shuffle_ps(val12, val34, _MM_SHUFFLE(2, 0, 2, 0)))),
                _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(0, 2, 0, 2)), 2);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val1 = _mm_movehl_ps(other0.values, other1.values);
            const __m128 val12 = _mm_movelh_ps(other0.values, other1.values);
            const __m128 val34 = _mm_movelh_ps(other2.values, other3.values);
            const __m128 val2 = _mm_movehl_ps(other2.values, other3.values);
            this->values0 = _mm256_set_m128(_mm_shuffle_ps(val12, val34, _MM_SHUFFLE(3, 1, 3, 1)),
                _mm_shuffle_ps(val12, val34, _MM_SHUFFLE(2, 0, 2, 0)));
            this->values1 = _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(0, 2, 0, 2));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val1 = _mm_movehl_ps(other0.values, other1.values);
            const __m128 val12 = _mm_movelh_ps(other0.values, other1.values);
            const __m128 val34 = _mm_movelh_ps(other2.values, other3.values);
            const __m128 val2 = _mm_movehl_ps(other2.values, other3.values);
            this->values0 = _mm_shuffle_ps(val12, val34, _MM_SHUFFLE(2, 0, 2, 0));
            this->values1 = _mm_shuffle_ps(val12, val34, _MM_SHUFFLE(3, 1, 3, 1));
            this->values2 = _mm_shuffle_ps(val1, val2, _MM_SHUFFLE(0, 2, 0, 2));
        } else
#endif
        {
            this->values0 = other0.values0;
            this->values1 = other1.values0;
            this->values2 = other2.values0;
            this->values3 = other3.values0;
            this->values4 = other0.values1;
            this->values5 = other1.values1;
            this->values6 = other2.values1;
            this->values7 = other3.values1;
            this->values8 = other0.values2;
            this->values9 = other1.values2;
            this->values10 = other2.values2;
            this->values11 = other3.values2;
        }
    }

    /**
     * Construct from a single SIMD3.
     * @note Note SIMD3s are packed when put into a SIMD12. This means that the first 4 elements will
     * actually consist of the packed first element of each of the 4 SIMD3s etc.
     * @param other The SIMD3.
     */
    XS_INLINE explicit SIMD12(const SIMD3Def& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(
                                                  _mm_shuffle1111_ps(other.values), _mm_shuffle0000_ps(other.values))),
                _mm_shuffle2222_ps(other.values), 2);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = _mm256_set_m128(_mm_shuffle1111_ps(other.values), _mm_shuffle0000_ps(other.values));
            this->values1 = _mm_shuffle2222_ps(other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_shuffle0000_ps(other.values);
            this->values1 = _mm_shuffle1111_ps(other.values);
            this->values2 = _mm_shuffle2222_ps(other.values);
        } else
#endif
        {
            this->values0 = other.values0;
            this->values1 = other.values0;
            this->values2 = other.values0;
            this->values3 = other.values0;
            this->values4 = other.values1;
            this->values5 = other.values1;
            this->values6 = other.values1;
            this->values7 = other.values1;
            this->values8 = other.values2;
            this->values9 = other.values2;
            this->values10 = other.values2;
            this->values11 = other.values2;
        }
    }

    /**
     * Construct from 3 different SIMD4s.
     * @param other0 The first SIMD4.
     * @param other1 The second SIMD4.
     * @param other2 The third SIMD4.
     */
    XS_INLINE
    SIMD12(const SIMD4Def& other0, const SIMD4Def& other1, const SIMD4Def& other2) noexcept
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
            this->values3 = other0.values3;
            this->values4 = other1.values0;
            this->values5 = other1.values1;
            this->values6 = other1.values2;
            this->values7 = other1.values3;
            this->values8 = other2.values0;
            this->values9 = other2.values1;
            this->values10 = other2.values2;
            this->values11 = other2.values3;
        }
    }

    /**
     * Construct from a single SIMD4.
     * @param other The SIMD4.
     */
    XS_INLINE explicit SIMD12(const SIMD4Def& other) noexcept
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
            this->values3 = other.values3;
            this->values4 = other.values0;
            this->values5 = other.values1;
            this->values6 = other.values2;
            this->values7 = other.values3;
            this->values8 = other.values0;
            this->values9 = other.values1;
            this->values10 = other.values2;
            this->values11 = other.values3;
        }
    }

    /**
     * Construct from 2 different SIMD6s.
     * @note Note SIMD6s are packed when put into a SIMD12. This means that the first 4 elements will
     * actually consist of the packed first 2 elements of each of the 2 SIMD6s etc.
     * @param other0 The first SIMD6.
     * @param other1 The second SIMD6.
     */
    XS_INLINE SIMD12(const SIMD6Def& other0, const SIMD6Def& other1) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m256 val = _mm256_shuffle_ps(other0.values, other1.values, _MM_SHUFFLE(1, 0, 1, 0));
            const __m512 val2 =
                _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle_ps(_mm256_castps256_ps128(other0.values),
                                    _mm256_castps256_ps128(other1.values), _MM_SHUFFLE(3, 2, 3, 2))),
                    val);
            this->values = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(2, 1, 2, 0));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val = _mm256_shuffle_ps(other0.values, other1.values, _MM_SHUFFLE(1, 0, 1, 0));
            this->values0 = _mm256_insertf128_ps(val,
                _mm_shuffle_ps(_mm256_castps256_ps128(other0.values), _mm256_castps256_ps128(other1.values),
                    _MM_SHUFFLE(3, 2, 3, 2)),
                1);
            this->values1 = _mm256_extractf128_ps(val, 1);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_movelh_ps(other0.values0, other1.values0);
            this->values1 = _mm_movehl_ps(other1.values0, other0.values0);
            this->values2 = _mm_movelh_ps(other0.values1, other1.values1);
        } else
#endif
        {
            this->values0 = other0.values0;
            this->values1 = other0.values1;
            this->values2 = other1.values0;
            this->values3 = other1.values1;
            this->values4 = other0.values2;
            this->values5 = other0.values3;
            this->values6 = other1.values2;
            this->values7 = other1.values3;
            this->values8 = other0.values4;
            this->values9 = other0.values5;
            this->values10 = other1.values4;
            this->values11 = other1.values5;
        }
    }

    /**
     * Construct from a single SIMD6.
     * @note Note SIMD6s are packed when put into a SIMD12. This means that the first 4 elements will
     * actually consist of the packed first 2 elements of each of the 2 SIMD6s etc.
     * @param other The SIMD6.
     */
    XS_INLINE explicit SIMD12(const SIMD6Def& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m256 val = _mm256_shuffle1010_ps(other.values);
            const __m512 val2 =
                _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle3232_ps(_mm256_castps256_ps128(other.values))), val);
            this->values = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(2, 1, 2, 0));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val = _mm256_shuffle1010_ps(other.values);
            this->values0 = _mm256_insertf128_ps(val, _mm_shuffle3232_ps(_mm256_castps256_ps128(other.values)), 1);
            this->values1 = _mm256_extractf128_ps(val, 1);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_movelh_ps(other.values0, other.values0);
            this->values1 = _mm_movehl_ps(other.values0, other.values0);
            this->values2 = _mm_movelh_ps(other.values1, other.values1);
        } else
#endif
        {
            this->values0 = other.values0;
            this->values1 = other.values1;
            this->values2 = other.values0;
            this->values3 = other.values1;
            this->values4 = other.values2;
            this->values5 = other.values3;
            this->values6 = other.values2;
            this->values7 = other.values3;
            this->values8 = other.values4;
            this->values9 = other.values5;
            this->values10 = other.values4;
            this->values11 = other.values5;
        }
    }

    /**
     * Construct from a SIMD8 and a SIMD4.
     * @param other0 The SIMD8.
     * @param other1 The SIMD4.
     */
    XS_INLINE SIMD12(const SIMD8Def& other0, const SIMD4Def& other1) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_insertf32x4(_mm512_castps256_ps512(other0.values), other1.values, 2);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = other0.values;
            this->values1 = other1.values;
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = other0.values0;
            this->values1 = other0.values1;
            this->values2 = other1.values;
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
        }
    }

    /**
     * Construct from a SIMD3x3 and a SIMD3.
     * @param other0 The SIMD3x3.
     * @param other1 The SIMD3.
     */
    XS_INLINE SIMD12(const SIMD3x3Def& other0, const SIMD3Def& other1) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_mask_insertf32x4(other0.values, _cvtu32_mask16(0x8888),
                _mm512_castps256_ps512(
                    _mm256_set_m128(_mm_shuffle1010_ps(other1.values), _mm_shuffle0000_ps(other1.values))),
                _mm_shuffle2200_ps(other1.values), 2);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values0 = _mm256_blend_set_m128(other0.values0, _MM256_BLEND(1, 0, 0, 0, 1, 0, 0, 0),
                _mm_shuffle1010_ps(other1.values), _mm_shuffle0000_ps(other1.values));
            this->values1 = _mm_insert_ps(other0.values1, other1.values, _MM_MK_INSERTPS_NDX(2, 3, 0));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_insert_ps(other0.values0, other1.values, _MM_MK_INSERTPS_NDX(0, 3, 0));
            this->values1 = _mm_insert_ps(other0.values1, other1.values, _MM_MK_INSERTPS_NDX(1, 3, 0));
            this->values2 = _mm_insert_ps(other0.values2, other1.values, _MM_MK_INSERTPS_NDX(2, 3, 0));
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
        }
    }

    /**
     * Constructor to build set to 0.
     * @returns Newly constructed object with required attributes.
     */
    XS_INLINE static SIMD12 Zero() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD12(_mm512_setzero_ps());
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 value = _mm256_setzero_ps();
            return SIMD12(value, _mm256_castps256_ps128(value));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 value = _mm_setzero_ps();
            return SIMD12(value, value, value);
        } else
#endif
        {
            return SIMD12(T{0});
        }
    }

    /**
     * Constructor to build set to 1.
     * @returns Newly constructed object with required attributes.
     */
    XS_INLINE static SIMD12 One() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD12(_mm512_set1_ps(1.0f));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 value = _mm256_set1_ps(1.0f);
            return SIMD12(value, _mm256_castps256_ps128(value));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 value = _mm_set1_ps(1.0f);
            return SIMD12(value, value, value);
        } else
#endif
        {
            return SIMD12(T{1});
        }
    }

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to get (range is 0-10).
     * @returns SIMDInBase containing the desired value.
     */
    template<uint32 Index>
    XS_INLINE InBaseDef getValueInBase() const noexcept
    {
        static_assert(Index < 12, "Invalid Index: Index must be <12");
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
            if constexpr (Index == 0) {
                return InBaseDef(_mm256_castps256_ps128(this->values0));
            } else if constexpr (Index == 1) {
                return InBaseDef(_mm_shuffle3311_ps(_mm256_castps256_ps128(this->values0)));
            } else if constexpr (Index == 2) {
                return InBaseDef(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values0)));
            } else if constexpr (Index == 3) {
                return InBaseDef(_mm_shuffle3333_ps(_mm256_castps256_ps128(this->values0)));
            } else if constexpr (Index == 4) {
                return InBaseDef(_mm256_extractf128_ps(this->values0, 1));
            } else if constexpr (Index == 5) {
                const __m128 val = _mm256_extractf128_ps(this->values0, 1);
                return InBaseDef(_mm_shuffle3311_ps(val));
            } else if constexpr (Index == 6) {
                const __m128 val = _mm256_extractf128_ps(this->values0, 1);
                return InBaseDef(_mm_shuffle3232_ps(val));
            } else if constexpr (Index == 7) {
                const __m128 val = _mm256_extractf128_ps(this->values0, 1);
                return InBaseDef(_mm_shuffle3333_ps(val));
            } else if constexpr (Index == 8) {
                return InBaseDef(this->values1);
            } else if constexpr (Index == 9) {
                return InBaseDef(_mm_shuffle3311_ps(this->values1));
            } else if constexpr (Index == 10) {
                return InBaseDef(_mm_shuffle3232_ps(this->values1));
            } else /*Index == 11*/ {
                return InBaseDef(_mm_shuffle3333_ps(this->values1));
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
     * Get a SIMD2.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to get (range is 0-5).
     * @returns SIMD2 containing the desired values.
     */
    template<uint32 Index>
    XS_INLINE SIMD2Def getValue2() const noexcept
    {
        static_assert(Index < 6, "Invalid Index: Index must be <6");
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
            if constexpr (Index == 0) {
                return SIMD2Def(_mm256_castps256_ps128(this->values0));
            } else if constexpr (Index == 1) {
                return SIMD2Def(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values0)));
            } else if constexpr (Index == 2) {
                return SIMD2Def(_mm256_extractf128_ps(this->values0, 1));
            } else if constexpr (Index == 3) {
                const __m128 val = _mm256_extractf128_ps(this->values0, 1);
                return SIMD2Def(_mm_shuffle3232_ps(val));
            } else if constexpr (Index == 4) {
                return SIMD2Def(this->values1);
            } else /*Index == 5*/ {
                return SIMD2Def(_mm_shuffle3232_ps(this->values1));
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
     * Get a SIMD3
     * @note Optimised for getting value from register. Note SIMD3s are packed when put into a SIMD12.
     * This means that the first 4 elements will actually consist of the packed first element of each of
     * the 4 SIMD3s etc.
     * @tparam Index The index of the element to get (range is 0-3).
     * @returns SIMD3 containing the desired values.
     */
    template<uint32 Index>
    XS_INLINE SIMD3Def getValue3() const noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index / 2 == 0) {
                const __m128 val =
                    _mm_movelh_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
                return SIMD3Def(_mm_shuffle_ps(
                    val, _mm512_extractf32x4_ps(this->values, 2), _MM_SHUFFLE(3, Index, (Index % 2) + 2, Index % 2)));
            } else {
                const __m128 val =
                    _mm_movehl_ps(_mm512_extractf32x4_ps(this->values, 1), _mm512_castps512_ps128(this->values));
                return SIMD3Def(_mm_shuffle_ps(
                    val, _mm512_extractf32x4_ps(this->values, 2), _MM_SHUFFLE(3, Index, (Index % 2) + 2, Index % 2)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index / 2 == 0) {
                const __m128 val =
                    _mm_movelh_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
                return SIMD3Def(_mm_shuffle_ps(val, this->values1, _MM_SHUFFLE(3, Index, (Index % 2) + 2, Index % 2)));
            } else {
                const __m128 val =
                    _mm_movehl_ps(_mm256_extractf128_ps(this->values0, 1), _mm256_castps256_ps128(this->values0));
                return SIMD3Def(_mm_shuffle_ps(val, this->values1, _MM_SHUFFLE(3, Index, (Index % 2) + 2, Index % 2)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index / 2 == 0) {
                const __m128 val = _mm_movelh_ps(this->values0, this->values1);
                return SIMD3Def(_mm_shuffle_ps(val, this->values2, _MM_SHUFFLE(3, Index, (Index % 2) + 2, Index % 2)));
            } else {
                const __m128 val = _mm_movehl_ps(this->values1, this->values0);
                return SIMD3Def(_mm_shuffle_ps(val, this->values2, _MM_SHUFFLE(3, Index, (Index % 2) + 2, Index % 2)));
            }
        } else
#endif
        {
            return SIMD3Def((&this->values0)[Index], (&this->values0)[Index + 4], (&this->values0)[Index + 8]);
        }
    }

    /**
     * Get a SIMD4.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to get (range is 0-2).
     * @returns SIMD4 containing the desired values.
     */
    template<uint32 Index>
    XS_INLINE SIMD4Def getValue4() const noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index == 0) {
                return SIMD4Def(_mm512_castps512_ps128(this->values));
            } else {
                return SIMD4Def(_mm512_extractf32x4_ps(this->values, Index));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                return SIMD4Def(_mm256_castps256_ps128(this->values0));
            } else if constexpr (Index == 1) {
                return SIMD4Def(_mm256_extractf128_ps(this->values0, 1));
            } else /*Index == 2*/ {
                return SIMD4Def(this->values1);
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
     * Get a SIMD6.
     * @note Note SIMD6s are packed when put into a SIMD12. This means that the first 4 elements will
     * actually consist of the packed first 2 elements of each of the 2 SIMD6s etc.
     * @tparam Index The index of the element to get (range is 0-1).
     * @returns SIMD6 containing the desired values.
     */
    template<uint32 Index>
    XS_INLINE SIMD6Def getValue6() const noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 val = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 1, 2, 0));
            if constexpr (Index == 0) {
                return SIMD6Def(_mm256_shuffle_ps(
                    _mm512_castps512_ps256(val), _mm512_extractf32x8_ps(val, 1), _MM_SHUFFLE(1, 0, 1, 0)));
            } else {
                return SIMD6Def(_mm256_shuffle_ps(
                    _mm512_castps512_ps256(val), _mm512_extractf32x8_ps(val, 1), _MM_SHUFFLE(3, 2, 3, 2)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                return SIMD6Def(_mm256_shuffle_ps(_mm256_insertf128_ps(this->values0, this->values1, 1),
                    _mm256_shuffle32107654_ps(this->values0), _MM_SHUFFLE(1, 0, 1, 0)));
            } else {
                return SIMD6Def(_mm256_shuffle_ps(_mm256_insertf128_ps(this->values0, this->values1, 1),
                    _mm256_shuffle32107654_ps(this->values0), _MM_SHUFFLE(3, 2, 3, 2)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                return SIMD6Def(_mm_movelh_ps(this->values0, this->values1), this->values2);
            } else {
                return SIMD6Def(
                    _mm_movehl_ps(this->values1, this->values0), _mm_movehl_ps(this->values2, this->values2));
            }
        } else
#endif
        {
            return SIMD6Def((&this->values0)[Index * 2], (&this->values0)[Index * 2 + 1],
                (&this->values0)[Index * 2 + 4], (&this->values0)[Index * 2 + 5], (&this->values0)[Index * 2 + 8],
                (&this->values0)[Index * 2 + 9]);
        }
    }

    /**
     * Get two lots of SIMD4s.
     * @note Optimised for getting value from register.
     * @tparam Index0 The index of the first element to get (range is 0-2).
     * @tparam Index1 The index of the second element to get (range is 0-2).
     * @returns SIMD8 containing the desired values.
     */
    template<uint32 Index0, uint32 Index1>
    XS_INLINE SIMD8Def getValue4x2() const noexcept
    {
        static_assert(Index0 < 3 && Index1 < 3, "Invalid Index: Indexes must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == 0 && Index1 == 1) {
                return SIMD8Def(_mm512_castps512_ps256(this->values));
            } else if constexpr (Index0 == 1 && Index1 == 0) {
                return SIMD8Def(_mm256_shuffle32107654_ps(_mm512_castps512_ps256(this->values)));
            } else {
                return SIMD8Def(_mm512_castps512_ps256(
                    _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 2, Index1, Index0))));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 1) {
                return SIMD8Def(this->values0);
            } else if constexpr (Index0 == 0 && Index1 == 2) {
                return SIMD8Def(_mm256_insertf128_ps(this->values0, this->values1, 1));
            } else if constexpr (Index0 == 2 && Index1 == 2) {
                return SIMD8Def(_mm256_broadcastf128_ps(this->values1));
            } else {
                return SIMD8Def(_mm256_permute2f128_ps(
                    this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(Index1, Index0)));
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
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-11).
     * @param other The new value.
     */
    template<uint32 Index>
    XS_INLINE void setValue(BaseDef other) noexcept
    {
        static_assert(Index < 12, "Invalid Index: Index must be <12");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_mask_mov_ps(this->values, _cvtu32_mask16(1 << Index), other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index < 8) {
                this->values0 = _mm256_blend_ps(this->values0, other.values, (1 << Index));
            } else if constexpr (Index == 8) {
                this->values1 = _mm_blend_ss(this->values1, _mm256_castps256_ps128(other.values));
            } else {
                this->values1 = _mm_blend_ps(this->values1, _mm256_castps256_ps128(other.values), (1 << (Index - 8)));
            }
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
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-11).
     * @param other The new value.
     */
    template<uint32 Index>
    XS_INLINE void setValue(InBaseDef other) noexcept
    {
        static_assert(Index < 12, "Invalid Index: Index must be <12");
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
            if constexpr (Index == 0) {
                this->values0 = _mm256_blend_ps(
                    this->values0, _mm256_castps128_ps256(other.values), _MM256_BLEND(0, 0, 0, 0, 0, 0, 0, 1));
            } else if constexpr (Index < 4 && hasISAFeature<ISAFeature::AVX512F>) {
                this->values0 = _mm256_mask_permute_ps(this->values0, _cvtu32_mask8(1 << Index),
                    _mm256_castps128_ps256(other.values), _MM_SHUFFLE(0, 0, 0, 0));
            } else if constexpr (Index == 1) {
                const __m128 value = _mm_shuffle1100_ps(other.values); //(x,x,0,x)
                this->values0 =
                    _mm256_blend_ps(this->values0, _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 0, 0, 1, 0));
            } else if constexpr (Index == 2) {
                const __m128 value = _mm_shuffle1010_ps(other.values); //(x,0,x,x)
                this->values0 =
                    _mm256_blend_ps(this->values0, _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 0, 1, 0, 0));
            } else if constexpr (Index == 3) {
                const __m128 value = _mm_shuffle0000_ps(other.values); //(0,x,x,x);
                this->values0 =
                    _mm256_blend_ps(this->values0, _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 1, 0, 0, 0));
            } else if constexpr (Index < 8 && hasISAFeature<ISAFeature::AVX512F>) {
                this->values0 = _mm256_mask_broadcastss_ps(this->values0, _cvtu32_mask8(1 << Index), other.values);
            } else if constexpr (Index < 8 && hasISAFeature<ISAFeature::AVX2>) {
                this->values0 = _mm256_blend_ps(this->values0, _mm256_broadcastss_ps(other.values), (1 << Index));
            } else if constexpr (Index == 4) {
                this->values0 = _mm256_blend_ps(
                    this->values0, _mm256_broadcastf128_ps(other.values), _MM256_BLEND(0, 0, 0, 1, 0, 0, 0, 0));
            } else if constexpr (Index == 5) {
                const __m128 value = _mm_shuffle1100_ps(other.values); //(x,x,0,x)
                this->values0 = _mm256_blend_ps(
                    this->values0, _mm256_broadcastf128_ps(value), _MM256_BLEND(0, 0, 1, 0, 0, 0, 0, 0));
            } else if constexpr (Index == 6) {
                const __m128 value = _mm_shuffle1010_ps(other.values); //(x,0,x,x)
                this->values0 = _mm256_blend_ps(
                    this->values0, _mm256_broadcastf128_ps(value), _MM256_BLEND(0, 1, 0, 0, 0, 0, 0, 0));
            } else if constexpr (Index == 7) {
                const __m128 value = _mm_shuffle0000_ps(other.values); //(0,x,x,x);
                this->values0 = _mm256_blend_ps(
                    this->values0, _mm256_broadcastf128_ps(value), _MM256_BLEND(1, 0, 0, 0, 0, 0, 0, 0));
            } else if constexpr (Index == 8) {
                this->values1 = _mm_blend_ss(this->values1, other.values);
            } else {
                this->values1 = _mm_insert_ps(this->values1, other.values, _MM_MK_INSERTPS_NDX(0, Index - 8, 0));
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
     * Set a SIMD2.
     * @tparam Index The index of the element to set (range is 0-5).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_INLINE void setValue2(const SIMD2Def& other) noexcept
    {
        static_assert(Index < 6, "Invalid Index: Index must be <6");
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
            if constexpr (Index == 0) {
                this->values0 = _mm256_blend_ps(
                    this->values0, _mm256_castps128_ps256(other.values), _MM256_BLEND(0, 0, 0, 0, 0, 0, 1, 1));
            } else if constexpr (Index == 1 && hasISAFeature<ISAFeature::AVX512F>) {
                this->values0 = _mm256_mask_permute_ps(this->values0, _cvtu32_mask8(3 << (Index * 2)),
                    _mm256_castps128_ps256(other.values), _MM_SHUFFLE(1, 0, 1, 0));
            } else if constexpr (Index == 1) {
                const __m128 value = _mm_shuffle1010_ps(other.values); //(1,0,x,x)
                this->values0 =
                    _mm256_blend_ps(this->values0, _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 1, 1, 0, 0));
            } else if constexpr (Index < 3 && hasISAFeature<ISAFeature::AVX512F>) {
                this->values0 =
                    _mm256_mask_broadcast_f32x2(this->values0, _cvtu32_mask8(3 << (Index * 2)), other.values);
            } else if constexpr (Index == 2) {
                this->values0 = _mm256_blend_ps(
                    this->values0, _mm256_broadcastf128_ps(other.values), _MM256_BLEND(0, 0, 1, 1, 0, 0, 0, 0));
            } else if constexpr (Index == 3) {
                const __m128 value = _mm_shuffle1010_ps(other.values); //(1,0,x,x)
                this->values0 = _mm256_blend_ps(
                    this->values0, _mm256_broadcastf128_ps(value), _MM256_BLEND(1, 1, 0, 0, 0, 0, 0, 0));
            } else if constexpr (Index % 4 == 0) {
                this->values1 = _mm_shuffle_ps(other.values, this->values1, _MM_SHUFFLE(3, 2, 1, 0));
            } else {
                this->values1 = _mm_movelh_ps(this->values1, other.values);
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
     * Set a SIMD3.
     * @note Note SIMD3s are packed when put into a SIMD12. This means that the first 4 elements of a SIMD12 will
     * actually consist of the packed first element of each of the 4 SIMD3s etc.
     * @tparam Index The index of the element to set (range is 0-3).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_INLINE void setValue3(const SIMD3Def& other) noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index == 0) {
                this->values = _mm512_mask_insertf32x4(this->values, _cvtu32_mask16(0x1111),
                    _mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle3311_ps(other.values), other.values)),
                    _mm_shuffle3232_ps(other.values), 2);
            } else if constexpr (Index == 1) {
                this->values = _mm512_mask_insertf32x4(this->values, _cvtu32_mask16(0x1111 << Index),
                    _mm512_castps256_ps512(_mm256_set_m128(other.values, _mm_shuffle1100_ps(other.values))),
                    _mm_shuffle3322_ps(other.values), 2);
            } else if constexpr (Index == 2) {
                this->values = _mm512_mask_insertf32x4(this->values, _cvtu32_mask16(0x1111 << Index),
                    _mm512_castps256_ps512(
                        _mm256_set_m128(_mm_shuffle1100_ps(other.values), _mm_shuffle1010_ps(other.values))),
                    other.values, 2);
            } else {
                this->values = _mm512_mask_insertf32x4(this->values, _cvtu32_mask16(0x1111 << Index),
                    _mm512_castps256_ps512(
                        _mm256_set_m128(_mm_shuffle1100_ps(other.values), _mm_shuffle0000_ps(other.values))),
                    _mm_shuffle2200_ps(other.values), 2);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                const __m128 val1 = _mm_shuffle3311_ps(other.values); //(x,x,x,1)
                this->values0 =
                    _mm256_blend_insertf128_ps(this->values0, 0x11, _mm256_castps128_ps256(other.values), val1, 1);
            } else if constexpr (Index == 1) {
                const __m128 val0 = _mm_shuffle2200_ps(other.values); //(x,x,0,x)
                const __m128 val1 = _mm_shuffle3311_ps(other.values); //(x,x,1,x)
                this->values0 =
                    _mm256_blend_insertf128_ps(this->values0, 0x11 << Index, _mm256_castps128_ps256(val0), val1, 1);
            } else {
                const __m128 val0 = _mm_shuffle0000_ps(other.values); //(0,0,x,x)
                const __m128 val1 = _mm_shuffle1100_ps(other.values); //(1,1,x,x)
                this->values0 =
                    _mm256_blend_insertf128_ps(this->values0, 0x11 << Index, _mm256_castps128_ps256(val0), val1, 1);
            }
            this->values1 = _mm_insert_ps(this->values1, other.values, _MM_MK_INSERTPS_NDX(2, Index, 0));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                this->values0 = _mm_blend_ss(this->values0, other.values);
                this->values1 = _mm_insert_ps(this->values1, other.values, _MM_MK_INSERTPS_NDX(1, Index, 0));
                this->values2 = _mm_insert_ps(this->values2, other.values, _MM_MK_INSERTPS_NDX(2, Index, 0));
            } else if constexpr (Index == 1) {
                this->values0 = _mm_insert_ps(this->values0, other.values, _MM_MK_INSERTPS_NDX(0, Index, 0));
                this->values1 = _mm_blend_ps(this->values1, other.values, 1 << Index);
                this->values2 = _mm_insert_ps(this->values2, other.values, _MM_MK_INSERTPS_NDX(2, Index, 0));
            } else if constexpr (Index == 2) {
                this->values0 = _mm_insert_ps(this->values0, other.values, _MM_MK_INSERTPS_NDX(0, Index, 0));
                this->values1 = _mm_insert_ps(this->values1, other.values, _MM_MK_INSERTPS_NDX(1, Index, 0));
                this->values2 = _mm_blend_ps(this->values2, other.values, 1 << Index);
            } else {
                this->values0 = _mm_insert_ps(this->values0, other.values, _MM_MK_INSERTPS_NDX(0, Index, 0));
                this->values1 = _mm_insert_ps(this->values1, other.values, _MM_MK_INSERTPS_NDX(1, Index, 0));
                this->values2 = _mm_insert_ps(this->values2, other.values, _MM_MK_INSERTPS_NDX(2, Index, 0));
            }
        } else
#endif
        {
            (&this->values0)[Index] = other.values0;
            (&this->values0)[Index + 4] = other.values1;
            (&this->values0)[Index + 8] = other.values2;
        }
    }

    /**
     * Set a SIMD4.
     * @tparam Index The index of the element to set (range is 0-2).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_INLINE void setValue4(const SIMD4Def& other) noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index == 0) {
                this->values =
                    _mm512_mask_mov_ps(this->values, _cvtu32_mask16(0xF), _mm512_castps128_ps512(other.values));
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
            (&this->values0)[Index * 4] = other.values0;
            (&this->values0)[Index * 4 + 1] = other.values1;
            (&this->values0)[Index * 4 + 2] = other.values2;
            (&this->values0)[Index * 4 + 3] = other.values3;
        }
    }

    /**
     * Set a SIMD6.
     * @note Note SIMD6s are packed when put into a SIMD12. This means that the first 4 elements
     * will actually consist of the packed first 2 elements of each of the 2 SIMD6s etc.
     * @tparam Index The index of the element to set (range is 0-1).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_INLINE void setValue6(const SIMD6Def& other) noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index == 0) {
                const __m128 val = _mm_shuffle3232_ps(_mm256_castps256_ps128(other.values));
                const __m256 val1 = _mm256_insertf128_ps(other.values, val, 1);
                this->values = _mm512_mask_shuffle_f32x4(this->values, _cvtu32_mask16(0x0333),
                    _mm512_castps256_ps512(val1), _mm512_castps256_ps512(other.values), _MM_SHUFFLE(1, 1, 1, 0));
            } else {
                const __m128 val = _mm_shuffle1010_ps(_mm256_castps256_ps128(other.values));
                const __m256 val1 = _mm256_insertf128_ps(_mm256_shuffle32103210_ps(other.values), val, 0);
                this->values =
                    _mm512_mask_shuffle_f32x4(this->values, _cvtu32_mask16(0x0CCC), _mm512_castps256_ps512(val1),
                        _mm512_castps256_ps512(_mm256_shuffle1010_ps(other.values)), _MM_SHUFFLE(1, 1, 1, 0));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                const __m128 val = _mm_shuffle3232_ps(_mm256_castps256_ps128(other.values));
                this->values0 = _mm256_blend_insertf128_ps(
                    this->values0, _MM256_BLEND(0, 0, 1, 1, 0, 0, 1, 1), other.values, val, 1);
                this->values1 = _mm256_blend_extractf128_ps(this->values1, _MM_BLEND(0, 0, 1, 1), other.values, 1);
            } else {
                const __m128 val = _mm_shuffle1010_ps(_mm256_castps256_ps128(other.values)); //(1,0,x,x)
                this->values0 = _mm256_blend_insertf128_ps(this->values0, _MM256_BLEND(1, 1, 0, 0, 1, 1, 0, 0),
                    _mm256_shuffle32103210_ps(other.values), val, 0);
                this->values1 = _mm_movelh_ps(this->values1, _mm256_extractf128_ps(other.values, 1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                this->values0 = _mm_blend_ps(other.values0, this->values0, _MM_BLEND(1, 1, 0, 0));
                this->values1 = _mm_movehl_ps(this->values1, other.values0);
                this->values2 = _mm_blend_ps(other.values1, this->values2, _MM_BLEND(1, 1, 0, 0));
            } else {
                this->values0 = _mm_movelh_ps(this->values0, other.values0);
                this->values1 = _mm_blend_ps(this->values1, other.values0, _MM_BLEND(1, 1, 0, 0));
                this->values2 = _mm_movelh_ps(this->values2, other.values1);
            }
        } else
#endif
        {
            (&this->values0)[Index * 2] = other.values0;
            (&this->values0)[Index * 2 + 1] = other.values1;
            (&this->values0)[Index * 2 + 4] = other.values2;
            (&this->values0)[Index * 2 + 5] = other.values3;
            (&this->values0)[Index * 2 + 8] = other.values4;
            (&this->values0)[Index * 2 + 9] = other.values5;
        }
    }

    /**
     * Set 2 sets of SIMD4s
     * @tparam Index0 The index of the first element to set (range is 0-2).
     * @tparam Index1 The index of the second element to set (range is 0-2)(Index1 != Index0).
     * @param other The new values.
     */
    template<uint32 Index0, uint32 Index1>
    XS_INLINE void setValue4x2(const SIMD8Def& other) noexcept
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
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5, bool Elem6, bool Elem7, bool Elem8,
        bool Elem9, bool Elem10, bool Elem11>
    XS_INLINE SIMD12 negate() const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && !Elem8 &&
            !Elem9 && !Elem10 && !Elem11) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 && Elem9 &&
                Elem10 && Elem11) {
                return SIMD12(_mm512_xor_ps(this->values, _mm512_set1_ps(-0.0f)));
            } else {
                return SIMD12(_mm512_xor_ps(this->values,
                    _mm512_set_ps(0.0f, 0.0f, 0.0f, 0.0f, Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f,
                        Elem9 ? -0.0f : 0.0f, Elem8 ? -0.0f : 0.0f, Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f,
                        Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f,
                        Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 && Elem9 &&
                Elem10 && Elem11) {
                const __m256 val = _mm256_set1_ps(-0.0f);
                return SIMD12(
                    _mm256_xor_ps(this->values0, val), _mm_xor_ps(this->values1, _mm256_castps256_ps128(val)));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7) {
                return SIMD12(_mm256_xor_ps(this->values0, _mm256_set1_ps(-0.0f)),
                    _mm_xor_ps(this->values1,
                        _mm_set_ps(
                            Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f, Elem9 ? -0.0f : 0.0f, Elem8 ? -0.0f : 0.0f)));
            } else if constexpr (Elem8 && Elem9 && Elem10 && Elem11) {
                return SIMD12(_mm256_xor_ps(this->values0,
                                  _mm256_set_ps(Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f,
                                      Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f,
                                      Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                    _mm_xor_ps(this->values1, _mm_set1_ps(-0.0f)));
            } else if constexpr (Elem0 == Elem8 && Elem1 == Elem9 && Elem2 == Elem10 && Elem3 == Elem11) {
                const __m256 val = _mm256_set_ps(Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f,
                    Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f,
                    Elem0 ? -0.0f : 0.0f);
                return SIMD12(
                    _mm256_xor_ps(this->values0, val), _mm_xor_ps(this->values1, _mm256_castps256_ps128(val)));
            } else if constexpr (Elem8 && !Elem9 && !Elem10 && !Elem11) {
                return SIMD12(_mm256_xor_ps(this->values0,
                                  _mm256_set_ps(Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f,
                                      Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f,
                                      Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                    _mm_xor_ps(this->values1, _mm_set_ss(-0.0f)));
            } else {
                return SIMD12(_mm256_xor_ps(this->values0,
                                  _mm256_set_ps(Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f,
                                      Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f,
                                      Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                    _mm_xor_ps(this->values1,
                        _mm_set_ps(
                            Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f, Elem9 ? -0.0f : 0.0f, Elem8 ? -0.0f : 0.0f)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && !Elem5 && !Elem6 && !Elem7 && Elem8 &&
                !Elem9 && !Elem10 && !Elem11) {
                const __m128 value = _mm_set_ss(-0.0f);
                return SIMD12(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value),
                    _mm_xor_ps(this->values2, value));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 &&
                Elem9 && Elem10 && Elem11) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD12(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value),
                    _mm_xor_ps(this->values2, value));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && !Elem8 &&
                !Elem9 && !Elem10 && !Elem11) {
                return SIMD12(_mm_xor_ps(this->values0, _mm_set1_ps(-0.0f)), this->values1, this->values2);
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && !Elem8 &&
                !Elem9 && !Elem10 && !Elem11) {
                return SIMD12(this->values0, _mm_xor_ps(this->values1, _mm_set1_ps(-0.0f)), this->values2);
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 &&
                Elem8 && Elem9 && Elem10 && Elem11) {
                return SIMD12(this->values0, this->values1, _mm_xor_ps(this->values2, _mm_set1_ps(-0.0f)));
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && Elem8 &&
                Elem9 && Elem10 && Elem11) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD12(this->values0, _mm_xor_ps(this->values1, value), _mm_xor_ps(this->values2, value));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && !Elem4 && !Elem5 && !Elem6 && !Elem7 && Elem8 &&
                Elem9 && Elem10 && Elem11) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD12(_mm_xor_ps(this->values0, value), this->values1, _mm_xor_ps(this->values2, value));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5 && Elem6 && Elem7 && !Elem8 &&
                !Elem9 && !Elem10 && !Elem11) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD12(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value), this->values2);
            } else if constexpr (Elem0 == Elem4 && Elem0 == Elem8 && Elem1 == Elem5 && Elem1 == Elem9 &&
                Elem2 == Elem6 && Elem2 == Elem10 && Elem3 == Elem7 && Elem3 == Elem11) {
                const __m128 value = _mm_set_ps(0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f);
                return SIMD12(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value),
                    _mm_xor_ps(this->values2, value));
            } else {
                return SIMD12(_mm_xor_ps(this->values0,
                                  _mm_set_ps(Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f,
                                      Elem0 ? -0.0f : 0.0f)),
                    _mm_xor_ps(this->values1,
                        _mm_set_ps(
                            Elem7 ? -0.0f : 0.0f, Elem6 ? -0.0f : 0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f)),
                    _mm_xor_ps(this->values2,
                        _mm_set_ps(
                            Elem11 ? -0.0f : 0.0f, Elem10 ? -0.0f : 0.0f, Elem9 ? -0.0f : 0.0f, Elem8 ? -0.0f : 0.0f)));
            }
        }
#endif
        else {
            return SIMD12(Elem0 ? -this->values0 : this->values0, Elem1 ? -this->values1 : this->values1,
                Elem2 ? -this->values2 : this->values2, Elem3 ? -this->values3 : this->values3,
                Elem4 ? -this->values4 : this->values4, Elem5 ? -this->values5 : this->values5,
                Elem6 ? -this->values6 : this->values6, Elem7 ? -this->values7 : this->values7,
                Elem8 ? -this->values8 : this->values8, Elem9 ? -this->values9 : this->values9,
                Elem10 ? -this->values10 : this->values10, Elem11 ? -this->values11 : this->values11);
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
    XS_INLINE SIMD12 mad(const SIMD12& other1, const SIMD12& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm512_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD12(_mm512_add_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm256_fmadd_ps(this->values0, other1.values0, other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values1, other2.values1));
            } else {
                return SIMD12(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values0), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm_fmadd_ps(this->values0, other1.values0, other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values1, other2.values1),
                    _mm_fmadd_ps(this->values2, other1.values2, other2.values2));
            } else {
                return SIMD12(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1),
                    _mm_add_ps(_mm_mul_ps(this->values2, other1.values2), other2.values2));
            }
        } else
#endif
        {
            return SIMD12(Shift::fma<T>(this->values0, other1.values0, other2.values0),
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
                Shift::fma<T>(this->values11, other1.values11, other2.values11));
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
    XS_INLINE SIMD12 mad(BaseDef other1, const SIMD12& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm512_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD12(_mm512_add_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm256_fmadd_ps(this->values0, other1.values, other2.values0),
                    _mm_fmadd_ps(this->values1, _mm256_castps256_ps128(other1.values), other2.values1));
            } else {
                return SIMD12(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, _mm256_castps256_ps128(other1.values)), other2.values1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values, other2.values1),
                    _mm_fmadd_ps(this->values2, other1.values, other2.values2));
            } else {
                return SIMD12(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1),
                    _mm_add_ps(_mm_mul_ps(this->values2, other1.values), other2.values2));
            }
        } else
#endif
        {
            return SIMD12(Shift::fma<T>(this->values0, other1.value, other2.values0),
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
                Shift::fma<T>(this->values11, other1.value, other2.values11));
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
    XS_INLINE SIMD12 mad(const SIMD12& other1, BaseDef other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm512_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD12(_mm512_add_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm256_fmadd_ps(this->values0, other1.values0, other2.values),
                    _mm_fmadd_ps(this->values1, other1.values1, _mm256_castps256_ps128(other2.values)));
            } else {
                return SIMD12(_mm256_add_ps(_mm256_mul_ps(this->values0, other1.values0), other2.values),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), _mm256_castps256_ps128(other2.values)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm_fmadd_ps(this->values0, other1.values0, other2.values),
                    _mm_fmadd_ps(this->values1, other1.values1, other2.values),
                    _mm_fmadd_ps(this->values2, other1.values2, other2.values));
            } else {
                return SIMD12(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values),
                    _mm_add_ps(_mm_mul_ps(this->values2, other1.values2), other2.values));
            }
        } else
#endif
        {
            return SIMD12(Shift::fma<T>(this->values0, other1.values0, other2.value),
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
                Shift::fma<T>(this->values11, other1.values11, other2.value));
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
    XS_INLINE SIMD12 mad(const SIMD4Def& other1, const SIMD12& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm512_fmadd_ps(this->values, _mm512_broadcastf128_ps(other1.values), other2.values));
            } else {
                return SIMD12(
                    _mm512_add_ps(_mm512_mul_ps(this->values, _mm512_broadcastf128_ps(other1.values)), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm256_fmadd_ps(this->values0, _mm256_broadcastf128_ps(other1.values), other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values, other2.values1));
            } else {
                return SIMD12(
                    _mm256_add_ps(_mm256_mul_ps(this->values0, _mm256_broadcastf128_ps(other1.values)), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values, other2.values1),
                    _mm_fmadd_ps(this->values2, other1.values, other2.values2));
            } else {
                return SIMD12(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1),
                    _mm_add_ps(_mm_mul_ps(this->values2, other1.values), other2.values2));
            }
        } else
#endif
        {
            return SIMD12(Shift::fma<T>(this->values0, other1.values0, other2.values0),
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
                Shift::fma<T>(this->values11, other1.values3, other2.values11));
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
    XS_INLINE SIMD12 mad(const SIMD3Def& other1, const SIMD3Def& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 val0 = _mm512_insertf32x4(
                _mm512_castps256_ps512(
                    _mm256_set_m128(_mm_shuffle1111_ps(other1.values), _mm_shuffle0000_ps(other1.values))),
                _mm_shuffle2222_ps(other1.values), 2);
            const __m512 val1 = _mm512_insertf32x4(
                _mm512_castps256_ps512(
                    _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values))),
                _mm_shuffle2222_ps(other2.values), 2);
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm512_fmadd_ps(this->values, val0, val1));
            } else {
                return SIMD12(_mm512_add_ps(_mm512_mul_ps(this->values, val0), val1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other1.values), _mm_shuffle0000_ps(other1.values));
            const __m256 val1 = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm256_fmadd_ps(this->values0, val0, val1),
                    _mm_fmadd_ps(this->values1, _mm_shuffle2222_ps(other1.values), _mm_shuffle2222_ps(other2.values)));
            } else {
                return SIMD12(_mm256_add_ps(_mm256_mul_ps(this->values0, val0), val1),
                    _mm_add_ps(_mm_mul_ps(this->values1, _mm_shuffle2222_ps(other1.values)),
                        _mm_shuffle2222_ps(other2.values)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(
                    _mm_fmadd_ps(this->values0, _mm_shuffle0000_ps(other1.values), _mm_shuffle0000_ps(other2.values)),
                    _mm_fmadd_ps(this->values1, _mm_shuffle1111_ps(other1.values), _mm_shuffle1111_ps(other2.values)),
                    _mm_fmadd_ps(this->values2, _mm_shuffle2222_ps(other1.values), _mm_shuffle2222_ps(other2.values)));
            } else {
                return SIMD12(_mm_add_ps(_mm_mul_ps(this->values0, _mm_shuffle0000_ps(other1.values)),
                                  _mm_shuffle0000_ps(other2.values)),
                    _mm_add_ps(_mm_mul_ps(this->values1, _mm_shuffle1111_ps(other1.values)),
                        _mm_shuffle1111_ps(other2.values)),
                    _mm_add_ps(_mm_mul_ps(this->values2, _mm_shuffle2222_ps(other1.values)),
                        _mm_shuffle2222_ps(other2.values)));
            }
        } else
#endif
        {
            return SIMD12(Shift::fma<T>(this->values0, other1.values0, other2.values0),
                Shift::fma<T>(this->values1, other1.values0, other2.values0),
                Shift::fma<T>(this->values2, other1.values0, other2.values0),
                Shift::fma<T>(this->values3, other1.values0, other2.values0),
                Shift::fma<T>(this->values4, other1.values1, other2.values1),
                Shift::fma<T>(this->values5, other1.values1, other2.values1),
                Shift::fma<T>(this->values6, other1.values1, other2.values1),
                Shift::fma<T>(this->values7, other1.values1, other2.values1),
                Shift::fma<T>(this->values8, other1.values2, other2.values2),
                Shift::fma<T>(this->values9, other1.values2, other2.values2),
                Shift::fma<T>(this->values10, other1.values2, other2.values2),
                Shift::fma<T>(this->values11, other1.values2, other2.values2));
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
    XS_INLINE SIMD12 mad(const SIMD3Def& other1, const SIMD12& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 val = _mm512_insertf32x4(
                _mm512_castps256_ps512(
                    _mm256_set_m128(_mm_shuffle1111_ps(other1.values), _mm_shuffle0000_ps(other1.values))),
                _mm_shuffle2222_ps(other1.values), 2);
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm512_fmadd_ps(this->values, val, other2.values));
            } else {
                return SIMD12(_mm512_add_ps(_mm512_mul_ps(this->values, val), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val = _mm256_set_m128(_mm_shuffle1111_ps(other1.values), _mm_shuffle0000_ps(other1.values));
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm256_fmadd_ps(this->values0, val, other2.values0),
                    _mm_fmadd_ps(this->values1, _mm_shuffle2222_ps(other1.values), other2.values1));
            } else {
                return SIMD12(_mm256_add_ps(_mm256_mul_ps(this->values0, val), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, _mm_shuffle2222_ps(other1.values)), other2.values1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm_fmadd_ps(this->values0, _mm_shuffle0000_ps(other1.values), other2.values0),
                    _mm_fmadd_ps(this->values1, _mm_shuffle1111_ps(other1.values), other2.values1),
                    _mm_fmadd_ps(this->values2, _mm_shuffle2222_ps(other1.values), other2.values2));
            } else {
                return SIMD12(_mm_add_ps(_mm_mul_ps(this->values0, _mm_shuffle0000_ps(other1.values)), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, _mm_shuffle1111_ps(other1.values)), other2.values1),
                    _mm_add_ps(_mm_mul_ps(this->values2, _mm_shuffle2222_ps(other1.values)), other2.values2));
            }
        } else
#endif
        {
            return SIMD12(Shift::fma<T>(this->values0, other1.values0, other2.values0),
                Shift::fma<T>(this->values1, other1.values0, other2.values1),
                Shift::fma<T>(this->values2, other1.values0, other2.values2),
                Shift::fma<T>(this->values3, other1.values0, other2.values3),
                Shift::fma<T>(this->values4, other1.values1, other2.values4),
                Shift::fma<T>(this->values5, other1.values1, other2.values5),
                Shift::fma<T>(this->values6, other1.values1, other2.values6),
                Shift::fma<T>(this->values7, other1.values1, other2.values7),
                Shift::fma<T>(this->values8, other1.values2, other2.values8),
                Shift::fma<T>(this->values9, other1.values2, other2.values9),
                Shift::fma<T>(this->values10, other1.values2, other2.values10),
                Shift::fma<T>(this->values11, other1.values2, other2.values11));
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
    XS_INLINE SIMD12 msub(const SIMD12& other1, const SIMD12& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm512_fmsub_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD12(_mm512_sub_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm256_fmsub_ps(this->values0, other1.values0, other2.values0),
                    _mm_fmsub_ps(this->values1, other1.values1, other2.values1));
            } else {
                return SIMD12(_mm256_sub_ps(_mm256_mul_ps(this->values0, other1.values0), other2.values0),
                    _mm_sub_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD12(_mm_fmsub_ps(this->values0, other1.values0, other2.values0),
                    _mm_fmsub_ps(this->values1, other1.values1, other2.values1),
                    _mm_fmsub_ps(this->values2, other1.values2, other2.values2));
            } else {
                return SIMD12(_mm_sub_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                    _mm_sub_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1),
                    _mm_sub_ps(_mm_mul_ps(this->values2, other1.values2), other2.values2));
            }
        } else
#endif
        {
            return SIMD12(Shift::fma<T>(this->values0, other1.values0, -other2.values0),
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
                Shift::fma<T>(this->values11, other1.values11, -other2.values11));
        }
    }

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_INLINE SIMD12 abs() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD12(_mm512_andnot_ps(_mm512_set1_ps(-0.0f), this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val = _mm256_set1_ps(-0.0f);
            return SIMD12(
                _mm256_andnot_ps(val, this->values0), _mm_andnot_ps(_mm256_castps256_ps128(val), this->values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_set1_ps(-0.0f);
            return SIMD12(_mm_andnot_ps(val, this->values0), _mm_andnot_ps(val, this->values1),
                _mm_andnot_ps(val, this->values2));
        } else
#endif
        {
            return SIMD12(Shift::abs<T>(this->values0), Shift::abs<T>(this->values1), Shift::abs<T>(this->values2),
                Shift::abs<T>(this->values3), Shift::abs<T>(this->values4), Shift::abs<T>(this->values5),
                Shift::abs<T>(this->values6), Shift::abs<T>(this->values7), Shift::abs<T>(this->values8),
                Shift::abs<T>(this->values9), Shift::abs<T>(this->values10), Shift::abs<T>(this->values11));
        }
    }

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_INLINE SIMD12 max(const SIMD12& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD12(_mm512_max_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD12(_mm256_max_ps(this->values0, other.values0), _mm_max_ps(this->values1, other.values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD12(_mm_max_ps(this->values0, other.values0), _mm_max_ps(this->values1, other.values1),
                _mm_max_ps(this->values2, other.values2));
        } else
#endif
        {
            return SIMD12(Shift::max<T>(this->values0, other.values0), Shift::max<T>(this->values1, other.values1),
                Shift::max<T>(this->values2, other.values2), Shift::max<T>(this->values3, other.values3),
                Shift::max<T>(this->values4, other.values4), Shift::max<T>(this->values5, other.values5),
                Shift::max<T>(this->values6, other.values6), Shift::max<T>(this->values7, other.values7),
                Shift::max<T>(this->values8, other.values8), Shift::max<T>(this->values9, other.values9),
                Shift::max<T>(this->values10, other.values10), Shift::max<T>(this->values11, other.values11));
        }
    }

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_INLINE SIMD12 min(const SIMD12& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD12(_mm512_min_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD12(_mm256_min_ps(this->values0, other.values0), _mm_min_ps(this->values1, other.values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD12(_mm_min_ps(this->values0, other.values0), _mm_min_ps(this->values1, other.values1),
                _mm_min_ps(this->values2, other.values2));
        } else
#endif
        {
            return SIMD12(Shift::min<T>(this->values0, other.values0), Shift::min<T>(this->values1, other.values1),
                Shift::min<T>(this->values2, other.values2), Shift::min<T>(this->values3, other.values3),
                Shift::min<T>(this->values4, other.values4), Shift::min<T>(this->values5, other.values5),
                Shift::min<T>(this->values6, other.values6), Shift::min<T>(this->values7, other.values7),
                Shift::min<T>(this->values8, other.values8), Shift::min<T>(this->values9, other.values9),
                Shift::min<T>(this->values10, other.values10), Shift::min<T>(this->values11, other.values11));
        }
    }

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_INLINE SIMD12 max(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD12(_mm512_max_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD12(_mm256_max_ps(this->values0, other.values),
                _mm_max_ps(this->values1, _mm256_castps256_ps128(other.values)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD12(_mm_max_ps(this->values0, other.values), _mm_max_ps(this->values1, other.values),
                _mm_max_ps(this->values2, other.values));
        } else
#endif
        {
            return SIMD12(Shift::max<T>(this->values0, other.value), Shift::max<T>(this->values1, other.value),
                Shift::max<T>(this->values2, other.value), Shift::max<T>(this->values3, other.value),
                Shift::max<T>(this->values4, other.value), Shift::max<T>(this->values5, other.value),
                Shift::max<T>(this->values6, other.value), Shift::max<T>(this->values7, other.value),
                Shift::max<T>(this->values8, other.value), Shift::max<T>(this->values9, other.value),
                Shift::max<T>(this->values10, other.value), Shift::max<T>(this->values11, other.value));
        }
    }

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_INLINE SIMD12 min(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD12(_mm512_min_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD12(_mm256_min_ps(this->values0, other.values),
                _mm_min_ps(this->values1, _mm256_castps256_ps128(other.values)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD12(_mm_min_ps(this->values0, other.values), _mm_min_ps(this->values1, other.values),
                _mm_min_ps(this->values2, other.values));
        } else
#endif
        {
            return SIMD12(Shift::min<T>(this->values0, other.value), Shift::min<T>(this->values1, other.value),
                Shift::min<T>(this->values2, other.value), Shift::min<T>(this->values3, other.value),
                Shift::min<T>(this->values4, other.value), Shift::min<T>(this->values5, other.value),
                Shift::min<T>(this->values6, other.value), Shift::min<T>(this->values7, other.value),
                Shift::min<T>(this->values8, other.value), Shift::min<T>(this->values9, other.value),
                Shift::min<T>(this->values10, other.value), Shift::min<T>(this->values11, other.value));
        }
    }

    /**
     * Maximum of each internally stored SIMD2.
     * @returns SIMD2 set to the largest value in each internal SIMD2.
     */
    XS_INLINE SIMD2Def max2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m128 val = _mm_max_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            val = _mm_max_ps(_mm512_extractf32x4_ps(this->values, 2), val);
            return SIMD2Def(_mm_max_ps(_mm_movehl_ps(val, val), val));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m128 val = _mm_max_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            val = _mm_max_ps(val, this->values1);
            return SIMD2Def(_mm_max_ps(_mm_movehl_ps(val, val), val));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val = _mm_max_ps(this->values0, this->values1);
            val = _mm_max_ps(val, this->values2);
            return SIMD2Def(_mm_max_ps(_mm_movehl_ps(val, val), val));
        } else
#endif
        {
            T other0 = Shift::max<T>(this->values0, this->values2);
            T other1 = Shift::max<T>(this->values1, this->values3);
            const T other2 = Shift::max<T>(this->values4, this->values6);
            const T other3 = Shift::max<T>(this->values5, this->values7);
            const T other4 = Shift::max<T>(this->values8, this->values10);
            const T other5 = Shift::max<T>(this->values9, this->values11);
            other0 = Shift::max<T>(other0, other2);
            other1 = Shift::max<T>(other1, other3);
            other0 = Shift::max<T>(other0, other4);
            other1 = Shift::max<T>(other1, other5);
            return SIMD2Def(other0, other1);
        }
    }

    /**
     * Minimum of each internally stored SIMD2.
     * @returns SIMD2 set to the largest value in each internal SIMD2.
     */
    XS_INLINE SIMD2Def min2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m128 val = _mm_min_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            val = _mm_min_ps(_mm512_extractf32x4_ps(this->values, 2), val);
            return SIMD2Def(_mm_min_ps(_mm_movehl_ps(val, val), val));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m128 val = _mm_min_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            val = _mm_min_ps(val, this->values1);
            return SIMD2Def(_mm_min_ps(_mm_movehl_ps(val, val), val));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val = _mm_min_ps(this->values0, this->values1);
            val = _mm_min_ps(val, this->values2);
            return SIMD2Def(_mm_min_ps(_mm_movehl_ps(val, val), val));
        } else
#endif
        {
            T other0 = Shift::min<T>(this->values0, this->values2);
            T other1 = Shift::min<T>(this->values1, this->values3);
            const T other2 = Shift::min<T>(this->values4, this->values6);
            const T other3 = Shift::min<T>(this->values5, this->values7);
            const T other4 = Shift::min<T>(this->values8, this->values10);
            const T other5 = Shift::min<T>(this->values9, this->values11);
            other0 = Shift::min<T>(other0, other2);
            other1 = Shift::min<T>(other1, other3);
            other0 = Shift::min<T>(other0, other4);
            other1 = Shift::min<T>(other1, other5);
            return SIMD2Def(other0, other1);
        }
    }

    /**
     * Maximum of each internally stored SIMD3.
     * @returns SIMD3 set to the largest value in each internal SIMD3.
     */
    XS_INLINE SIMD3Def max3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 lo = _mm512_castps512_ps128(this->values);
            const __m128 hi = _mm512_extractf32x4_ps(this->values, 1);
            const __m128 hi2 = _mm512_extractf32x4_ps(this->values, 2);
            __m128 val0 = _mm_movelh_ps(lo, hi);
            __m128 val1 = _mm_movehl_ps(hi, lo);
            __m128 val2 = _mm_movehl_ps(hi2, hi2);
            val0 = _mm_max_ps(val0, val1); //(x,x,y,y)
            val2 = _mm_max_ps(val2, hi2);  //(z,z,-,-)
            val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
            val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD3Def(_mm_max_ps(val1, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 lo = _mm256_castps256_ps128(this->values0);
            const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
            __m128 val0 = _mm_movelh_ps(lo, hi);
            __m128 val1 = _mm_movehl_ps(hi, lo);
            __m128 val2 = _mm_movehl_ps(this->values1, this->values1);
            val0 = _mm_max_ps(val0, val1);          //(x,x,y,y)
            val2 = _mm_max_ps(val2, this->values1); //(z,z,-,-)
            val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
            val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD3Def(_mm_max_ps(val1, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_movelh_ps(this->values0, this->values1);
            __m128 val1 = _mm_movehl_ps(this->values1, this->values0);
            __m128 val2 = _mm_movehl_ps(this->values2, this->values2);
            val0 = _mm_max_ps(val0, val1);          //(x,x,y,y)
            val2 = _mm_max_ps(val2, this->values2); //(z,z,-,-)
            val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
            val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD3Def(_mm_max_ps(val1, val2));
        } else
#endif
        {
            T other0 = Shift::max<T>(this->values0, this->values1);
            const T other1 = Shift::max<T>(this->values2, this->values3);
            T other2 = Shift::max<T>(this->values4, this->values5);
            const T other3 = Shift::max<T>(this->values6, this->values7);
            T other4 = Shift::max<T>(this->values8, this->values9);
            const T other5 = Shift::max<T>(this->values10, this->values11);
            other0 = Shift::max<T>(other0, other1);
            other2 = Shift::max<T>(other2, other3);
            other4 = Shift::max<T>(other4, other5);
            return SIMD3Def(other0, other2, other4);
        }
    }

    /**
     * Minimum of each internally stored SIMD3.
     * @returns SIMD3 set to the largest value in each internal SIMD3.
     */
    XS_INLINE SIMD3Def min3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 lo = _mm512_castps512_ps128(this->values);
            const __m128 hi = _mm512_extractf32x4_ps(this->values, 1);
            const __m128 hi2 = _mm512_extractf32x4_ps(this->values, 2);
            __m128 val0 = _mm_movelh_ps(lo, hi);
            __m128 val1 = _mm_movehl_ps(hi, lo);
            __m128 val2 = _mm_movehl_ps(hi2, hi2);
            val0 = _mm_min_ps(val0, val1); //(x,x,y,y)
            val2 = _mm_min_ps(val2, hi2);  //(z,z,-,-)
            val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
            val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD3Def(_mm_min_ps(val1, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 lo = _mm256_castps256_ps128(this->values0);
            const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
            __m128 val0 = _mm_movelh_ps(lo, hi);
            __m128 val1 = _mm_movehl_ps(hi, lo);
            __m128 val2 = _mm_movehl_ps(this->values1, this->values1);
            val0 = _mm_min_ps(val0, val1);          //(x,x,y,y)
            val2 = _mm_min_ps(val2, this->values1); //(z,z,-,-)
            val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
            val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD3Def(_mm_min_ps(val1, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_movelh_ps(this->values0, this->values1);
            __m128 val1 = _mm_movehl_ps(this->values1, this->values0);
            __m128 val2 = _mm_movehl_ps(this->values2, this->values2);
            val0 = _mm_min_ps(val0, val1);          //(x,x,y,y)
            val2 = _mm_min_ps(val2, this->values2); //(z,z,-,-)
            val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
            val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD3Def(_mm_min_ps(val1, val2));
        } else
#endif
        {
            T other0 = Shift::min<T>(this->values0, this->values1);
            const T other1 = Shift::min<T>(this->values2, this->values3);
            T other2 = Shift::min<T>(this->values4, this->values5);
            const T other3 = Shift::min<T>(this->values6, this->values7);
            T other4 = Shift::min<T>(this->values8, this->values9);
            const T other5 = Shift::min<T>(this->values10, this->values11);
            other0 = Shift::min<T>(other0, other1);
            other2 = Shift::min<T>(other2, other3);
            other4 = Shift::min<T>(other4, other5);
            return SIMD3Def(other0, other2, other4);
        }
    }

    /**
     * Maximum of each internally stored SIMD4.
     * @returns SIMD4 set to the largest value in each internal SIMD4.
     */
    XS_INLINE SIMD4Def max4() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 val =
                _mm_max_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            return SIMD4Def(_mm_max_ps(val, _mm512_extractf32x4_ps(this->values, 2)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val =
                _mm_max_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            return SIMD4Def(_mm_max_ps(val, this->values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_max_ps(this->values0, this->values1);
            return SIMD4Def(_mm_max_ps(val, this->values2));
        } else
#endif
        {
            T other0 = Shift::max<T>(this->values0, this->values4);
            T other1 = Shift::max<T>(this->values1, this->values5);
            T other2 = Shift::max<T>(this->values2, this->values6);
            T other3 = Shift::max<T>(this->values3, this->values7);
            other0 = Shift::max<T>(other0, this->values8);
            other1 = Shift::max<T>(other1, this->values9);
            other2 = Shift::max<T>(other2, this->values10);
            other3 = Shift::max<T>(other3, this->values11);
            return SIMD4Def(other0, other1, other2, other3);
        }
    }

    /**
     * Minimum of each internally stored SIMD4.
     * @returns SIMD4 set to the largest value in each internal SIMD4.
     */
    XS_INLINE SIMD4Def min4() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 val =
                _mm_min_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            return SIMD4Def(_mm_min_ps(val, _mm512_extractf32x4_ps(this->values, 2)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val =
                _mm_min_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            return SIMD4Def(_mm_min_ps(val, this->values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_min_ps(this->values0, this->values1);
            return SIMD4Def(_mm_min_ps(val, this->values2));
        } else
#endif
        {
            T other0 = Shift::min<T>(this->values0, this->values4);
            T other1 = Shift::min<T>(this->values1, this->values5);
            T other2 = Shift::min<T>(this->values2, this->values6);
            T other3 = Shift::min<T>(this->values3, this->values7);
            other0 = Shift::min<T>(other0, this->values8);
            other1 = Shift::min<T>(other1, this->values9);
            other2 = Shift::min<T>(other2, this->values10);
            other3 = Shift::min<T>(other3, this->values11);
            return SIMD4Def(other0, other1, other2, other3);
        }
    }

    /**
     * Maximum of each internally stored SIMD6.
     * @returns SIMD6 set to the largest value in each internal SIMD6.
     */
    XS_INLINE SIMD6Def max6() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 1, 2, 0));
            __m256 val1 = _mm512_castps512_ps256(val0);
            __m256 val2 = _mm512_extractf32x8_ps(val0, 1);
            __m256 val3 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(1, 0, 1, 0));
            __m256 val4 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 2, 3, 2));
            return SIMD6Def(_mm256_max_ps(val3, val4));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 lo = _mm256_castps256_ps128(this->values0);
            const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
            const __m128 val0 = _mm_movelh_ps(lo, hi);
            const __m128 val1 = _mm_movehl_ps(hi, lo);
            const __m128 val2 = _mm_movehl_ps(this->values1, this->values1);
            __m256 val4 = _mm256_set_m128(val2, val0);
            __m256 val5 = _mm256_set_m128(this->values1, val1);
            return SIMD6Def(_mm256_max_ps(val4, val5));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_movelh_ps(this->values0, this->values1);
            const __m128 val1 = _mm_movehl_ps(this->values1, this->values0);
            const __m128 val2 = _mm_movehl_ps(this->values2, this->values2);
            return SIMD6Def(_mm_max_ps(val0, val1), _mm_max_ps(val2, this->values2));
        } else
#endif
        {
            const T other0 = Shift::max<T>(this->values0, this->values2);
            const T other1 = Shift::max<T>(this->values1, this->values3);
            const T other2 = Shift::max<T>(this->values4, this->values6);
            const T other3 = Shift::max<T>(this->values5, this->values7);
            const T other4 = Shift::max<T>(this->values8, this->values10);
            const T other5 = Shift::max<T>(this->values9, this->values11);
            return SIMD6Def(other0, other1, other2, other3, other4, other5);
        }
    }

    /**
     * Minimum of each internally stored SIMD6.
     * @returns SIMD6 set to the largest value in each internal SIMD6.
     */
    XS_INLINE SIMD6Def min6() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 1, 2, 0));
            __m256 val1 = _mm512_castps512_ps256(val0);
            __m256 val2 = _mm512_extractf32x8_ps(val0, 1);
            __m256 val3 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(1, 0, 1, 0));
            __m256 val4 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 2, 3, 2));
            return SIMD6Def(_mm256_min_ps(val3, val4));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 lo = _mm256_castps256_ps128(this->values0);
            const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
            const __m128 val0 = _mm_movelh_ps(lo, hi);
            const __m128 val1 = _mm_movehl_ps(hi, lo);
            const __m128 val2 = _mm_movehl_ps(this->values1, this->values1);
            __m256 val4 = _mm256_set_m128(val2, val0);
            __m256 val5 = _mm256_set_m128(this->values1, val1);
            return SIMD6Def(_mm256_min_ps(val4, val5));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_movelh_ps(this->values0, this->values1);
            const __m128 val1 = _mm_movehl_ps(this->values1, this->values0);
            const __m128 val2 = _mm_movehl_ps(this->values2, this->values2);
            return SIMD6Def(_mm_min_ps(val0, val1), _mm_min_ps(val2, this->values2));
        } else
#endif
        {
            const T other0 = Shift::min<T>(this->values0, this->values2);
            const T other1 = Shift::min<T>(this->values1, this->values3);
            const T other2 = Shift::min<T>(this->values4, this->values6);
            const T other3 = Shift::min<T>(this->values5, this->values7);
            const T other4 = Shift::min<T>(this->values8, this->values10);
            const T other5 = Shift::min<T>(this->values9, this->values11);
            return SIMD6Def(other0, other1, other2, other3, other4, other5);
        }
    }

    /**
     * Maximum element of this object.
     * @returns SIMDBase set to the largest value.
     */
    XS_INLINE BaseDef hmax() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m128 val = _mm_max_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            val = _mm_max_ps(val, _mm512_extractf32x4_ps(this->values, 2));
            val = _mm_max_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
            val = _mm_max_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val);
            return BaseDef(_mm512_broadcastf128_ps(val));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m128 val = _mm_max_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            val = _mm_max_ps(val, this->values1);
            val = _mm_max_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
            val = _mm_max_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val);
            return BaseDef(_mm256_broadcastf128_ps(val));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val = _mm_max_ps(this->values0, this->values1);
            val = _mm_max_ps(val, this->values2);
            val = _mm_max_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
            return BaseDef(_mm_max_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
        } else
#endif
        {
            T other = Shift::max<T>(this->values0, this->values1);
            T other2 = Shift::max<T>(this->values2, this->values3);
            T other3 = Shift::max<T>(this->values4, this->values5);
            other = Shift::max<T>(this->values6, other);
            other2 = Shift::max<T>(this->values7, other2);
            other3 = Shift::max<T>(this->values8, other3);
            other = Shift::max<T>(this->values9, other);
            other2 = Shift::max<T>(this->values10, other2);
            other3 = Shift::max<T>(this->values11, other3);
            other = Shift::max<T>(other2, other);
            other = Shift::max<T>(other3, other);
            return BaseDef(other);
        }
    }

    /**
     * Minimum element of this object.
     * @returns SIMDBase set to the smallest value.
     */
    XS_INLINE BaseDef hmin() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m128 val = _mm_min_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            val = _mm_min_ps(val, _mm512_extractf32x4_ps(this->values, 2));
            val = _mm_min_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
            val = _mm_min_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val);
            return BaseDef(_mm512_broadcastf128_ps(val));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m128 val = _mm_min_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            val = _mm_min_ps(val, this->values1);
            val = _mm_min_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
            val = _mm_min_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val);
            return BaseDef(_mm256_broadcastf128_ps(val));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val = _mm_min_ps(this->values0, this->values1);
            val = _mm_min_ps(val, this->values2);
            val = _mm_min_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
            return BaseDef(_mm_min_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
        } else
#endif
        {
            T other = Shift::min<T>(this->values0, this->values1);
            T other2 = Shift::min<T>(this->values2, this->values3);
            T other3 = Shift::min<T>(this->values4, this->values5);
            other = Shift::min<T>(this->values6, other);
            other2 = Shift::min<T>(this->values7, other2);
            other3 = Shift::min<T>(this->values8, other3);
            other = Shift::min<T>(this->values9, other);
            other2 = Shift::min<T>(this->values10, other2);
            other3 = Shift::min<T>(this->values11, other3);
            other = Shift::min<T>(other2, other);
            other = Shift::min<T>(other3, other);
            return BaseDef(other);
        }
    }

    /**
     * Maximum element of this object.
     * @returns SIMDInBase set to the largest value.
     */
    XS_INLINE InBaseDef hmaxInBase() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m128 val = _mm_max_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            val = _mm_max_ps(val, _mm512_extractf32x4_ps(this->values, 2));
            val = _mm_max_ps(_mm_movehl_ps(val, val), val);
            return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m128 val = _mm_max_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            val = _mm_max_ps(val, this->values1);
            val = _mm_max_ps(_mm_movehl_ps(val, val), val);
            return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val = _mm_max_ps(this->values0, this->values1);
            val = _mm_max_ps(val, this->values2);
            val = _mm_max_ps(_mm_movehl_ps(val, val), val);
            return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
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
    XS_INLINE InBaseDef hminInBase() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m128 val = _mm_min_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            val = _mm_min_ps(val, _mm512_extractf32x4_ps(this->values, 2));
            val = _mm_min_ps(_mm_movehl_ps(val, val), val);
            return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m128 val = _mm_min_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            val = _mm_min_ps(val, this->values1);
            val = _mm_min_ps(_mm_movehl_ps(val, val), val);
            return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val = _mm_min_ps(this->values0, this->values1);
            val = _mm_min_ps(val, this->values2);
            val = _mm_min_ps(_mm_movehl_ps(val, val), val);
            return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
        } else
#endif
        {
            return InBaseDef(hmin().value);
        }
    }

    /**
     * Maximum element from each SIMD2.
     * @returns SIMD6 set to the largest value in each SIMD2.
     */
    XS_INLINE SIMD6Def hmax2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 1, 2, 0));
            __m256 val1 = _mm512_castps512_ps256(val0);
            __m256 val2 = _mm512_extractf32x8_ps(val0, 1);
            __m256 val3 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(2, 0, 2, 0));
            __m256 val4 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD6Def(_mm256_max_ps(val3, val4));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 lo = _mm256_castps256_ps128(this->values0);
            const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
            __m128 val0 = _mm_shuffle_ps(lo, hi, _MM_SHUFFLE(2, 0, 2, 0));
            const __m128 val1 = _mm_shuffle_ps(lo, hi, _MM_SHUFFLE(3, 1, 3, 1));
            __m128 val2 = _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(2, 0, 2, 0));
            const __m128 val3 = _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(3, 1, 3, 1));
            __m256 val4 = _mm256_set_m128(val2, val0);
            __m256 val5 = _mm256_set_m128(val3, val1);
            return SIMD6Def(_mm256_max_ps(val4, val5));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(2, 0, 2, 0));
            const __m128 val1 = _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(3, 1, 3, 1));
            __m128 val2 = _mm_shuffle1_ps(this->values2, _MM_SHUFFLE(2, 0, 2, 0));
            const __m128 val3 = _mm_shuffle1_ps(this->values2, _MM_SHUFFLE(3, 1, 3, 1));
            val0 = _mm_max_ps(val0, val1);
            val2 = _mm_max_ps(val2, val3);
            return SIMD6Def(val0, val2);
        } else
#endif
        {
            return SIMD6Def(Shift::max<T>(this->values0, this->values1), Shift::max<T>(this->values2, this->values3),
                Shift::max<T>(this->values4, this->values5), Shift::max<T>(this->values6, this->values7),
                Shift::max<T>(this->values8, this->values9), Shift::max<T>(this->values10, this->values11));
        }
    }

    /**
     * Minimum element from each SIMD2.
     * @returns SIMD6 set to the smallest value in each SIMD2.
     */
    XS_INLINE SIMD6Def hmin2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 1, 2, 0));
            __m256 val1 = _mm512_castps512_ps256(val0);
            __m256 val2 = _mm512_extractf32x8_ps(val0, 1);
            __m256 val3 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(2, 0, 2, 0));
            __m256 val4 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD6Def(_mm256_min_ps(val3, val4));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 lo = _mm256_castps256_ps128(this->values0);
            const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
            __m128 val0 = _mm_shuffle_ps(lo, hi, _MM_SHUFFLE(2, 0, 2, 0));
            const __m128 val1 = _mm_shuffle_ps(lo, hi, _MM_SHUFFLE(3, 1, 3, 1));
            __m128 val2 = _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(2, 0, 2, 0));
            const __m128 val3 = _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(3, 1, 3, 1));
            __m256 val4 = _mm256_set_m128(val2, val0);
            __m256 val5 = _mm256_set_m128(val3, val1);
            return SIMD6Def(_mm256_min_ps(val4, val5));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(2, 0, 2, 0));
            const __m128 val1 = _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(3, 1, 3, 1));
            __m128 val2 = _mm_shuffle1_ps(this->values2, _MM_SHUFFLE(2, 0, 2, 0));
            const __m128 val3 = _mm_shuffle1_ps(this->values2, _MM_SHUFFLE(3, 1, 3, 1));
            val0 = _mm_min_ps(val0, val1);
            val2 = _mm_min_ps(val2, val3);
            return SIMD6Def(val0, val2);
        } else
#endif
        {
            return SIMD6Def(Shift::min<T>(this->values0, this->values1), Shift::min<T>(this->values2, this->values3),
                Shift::min<T>(this->values4, this->values5), Shift::min<T>(this->values6, this->values7),
                Shift::min<T>(this->values8, this->values9), Shift::min<T>(this->values10, this->values11));
        }
    }

    /**
     * Maximum element from each SIMD3.
     * @returns SIMD4 set to the largest value in each SIMD3.
     */
    XS_INLINE SIMD4Def hmax3() const noexcept
    {
        // max4
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 val =
                _mm_max_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            return SIMD4Def(_mm_max_ps(val, _mm512_extractf32x4_ps(this->values, 2)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val =
                _mm_max_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            return SIMD4Def(_mm_max_ps(val, this->values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_max_ps(this->values0, this->values1);
            return SIMD4Def(_mm_max_ps(val, this->values2));
        } else
#endif
        {
            T other0 = Shift::max<T>(this->values0, this->values4);
            T other1 = Shift::max<T>(this->values1, this->values5);
            T other2 = Shift::max<T>(this->values2, this->values6);
            T other3 = Shift::max<T>(this->values3, this->values7);
            other0 = Shift::max<T>(other0, this->values8);
            other1 = Shift::max<T>(other1, this->values9);
            other2 = Shift::max<T>(other2, this->values10);
            other3 = Shift::max<T>(other3, this->values11);
            return SIMD4Def(other0, other1, other2, other3);
        }
    }

    /**
     * Minimum element from each SIMD3.
     * @returns SIMD4 set to the largest value in each SIMD3.
     */
    XS_INLINE SIMD4Def hmin3() const noexcept
    {
        // min4
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 val =
                _mm_min_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            return SIMD4Def(_mm_min_ps(val, _mm512_extractf32x4_ps(this->values, 2)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val =
                _mm_min_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            return SIMD4Def(_mm_min_ps(val, this->values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_min_ps(this->values0, this->values1);
            return SIMD4Def(_mm_min_ps(val, this->values2));
        } else
#endif
        {
            T other0 = Shift::min<T>(this->values0, this->values4);
            T other1 = Shift::min<T>(this->values1, this->values5);
            T other2 = Shift::min<T>(this->values2, this->values6);
            T other3 = Shift::min<T>(this->values3, this->values7);
            other0 = Shift::min<T>(other0, this->values8);
            other1 = Shift::min<T>(other1, this->values9);
            other2 = Shift::min<T>(other2, this->values10);
            other3 = Shift::min<T>(other3, this->values11);
            return SIMD4Def(other0, other1, other2, other3);
        }
    }

    /**
     * Maximum element from each SIMD4.
     * @returns SIMD3 set to the largest value in each SIMD4.
     */
    XS_INLINE SIMD3Def hmax4() const noexcept
    {
        // max3
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 lo = _mm512_castps512_ps128(this->values);
            const __m128 hi = _mm512_extractf32x4_ps(this->values, 1);
            const __m128 hi2 = _mm512_extractf32x4_ps(this->values, 2);
            __m128 val0 = _mm_movelh_ps(lo, hi);
            __m128 val1 = _mm_movehl_ps(hi, lo);
            __m128 val2 = _mm_movehl_ps(hi2, hi2);
            val0 = _mm_max_ps(val0, val1); //(x,x,y,y)
            val2 = _mm_max_ps(val2, hi2);  //(z,z,-,-)
            val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
            val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD3Def(_mm_max_ps(val1, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 lo = _mm256_castps256_ps128(this->values0);
            const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
            __m128 val0 = _mm_movelh_ps(lo, hi);
            __m128 val1 = _mm_movehl_ps(hi, lo);
            __m128 val2 = _mm_movehl_ps(this->values1, this->values1);
            val0 = _mm_max_ps(val0, val1);          //(x,x,y,y)
            val2 = _mm_max_ps(val2, this->values1); //(z,z,-,-)
            val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
            val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD3Def(_mm_max_ps(val1, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_movelh_ps(this->values0, this->values1);
            __m128 val1 = _mm_movehl_ps(this->values1, this->values0);
            __m128 val2 = _mm_movehl_ps(this->values2, this->values2);
            val0 = _mm_max_ps(val0, val1);          //(x,x,y,y)
            val2 = _mm_max_ps(val2, this->values2); //(z,z,-,-)
            val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
            val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD3Def(_mm_max_ps(val1, val2));
        } else
#endif
        {
            T other0 = Shift::max<T>(this->values0, this->values1);
            const T other1 = Shift::max<T>(this->values2, this->values3);
            T other2 = Shift::max<T>(this->values4, this->values5);
            const T other3 = Shift::max<T>(this->values6, this->values7);
            T other4 = Shift::max<T>(this->values8, this->values9);
            const T other5 = Shift::max<T>(this->values10, this->values11);
            other0 = Shift::max<T>(other0, other1);
            other2 = Shift::max<T>(other2, other3);
            other4 = Shift::max<T>(other4, other5);
            return SIMD3Def(other0, other2, other4);
        }
    }

    /**
     * Minimum element from each SIMD4.
     * @returns SIMD3 set to the largest value in each SIMD4.
     */
    XS_INLINE SIMD3Def hmin4() const noexcept
    {
        // min3
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 lo = _mm512_castps512_ps128(this->values);
            const __m128 hi = _mm512_extractf32x4_ps(this->values, 1);
            const __m128 hi2 = _mm512_extractf32x4_ps(this->values, 2);
            __m128 val0 = _mm_movelh_ps(lo, hi);
            __m128 val1 = _mm_movehl_ps(hi, lo);
            __m128 val2 = _mm_movehl_ps(hi2, hi2);
            val0 = _mm_min_ps(val0, val1); //(x,x,y,y)
            val2 = _mm_min_ps(val2, hi2);  //(z,z,-,-)
            val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
            val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD3Def(_mm_min_ps(val1, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 lo = _mm256_castps256_ps128(this->values0);
            const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
            __m128 val0 = _mm_movelh_ps(lo, hi);
            __m128 val1 = _mm_movehl_ps(hi, lo);
            __m128 val2 = _mm_movehl_ps(this->values1, this->values1);
            val0 = _mm_min_ps(val0, val1);          //(x,x,y,y)
            val2 = _mm_min_ps(val2, this->values1); //(z,z,-,-)
            val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
            val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD3Def(_mm_min_ps(val1, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_movelh_ps(this->values0, this->values1);
            __m128 val1 = _mm_movehl_ps(this->values1, this->values0);
            __m128 val2 = _mm_movehl_ps(this->values2, this->values2);
            val0 = _mm_min_ps(val0, val1);          //(x,x,y,y)
            val2 = _mm_min_ps(val2, this->values2); //(z,z,-,-)
            val1 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(2, 0, 2, 0));
            val2 = _mm_shuffle_ps(val0, val2, _MM_SHUFFLE(3, 1, 3, 1));
            return SIMD3Def(_mm_min_ps(val1, val2));
        } else
#endif
        {
            T other0 = Shift::min<T>(this->values0, this->values1);
            const T other1 = Shift::min<T>(this->values2, this->values3);
            T other2 = Shift::min<T>(this->values4, this->values5);
            const T other3 = Shift::min<T>(this->values6, this->values7);
            T other4 = Shift::min<T>(this->values8, this->values9);
            const T other5 = Shift::min<T>(this->values10, this->values11);
            other0 = Shift::min<T>(other0, other1);
            other2 = Shift::min<T>(other2, other3);
            other4 = Shift::min<T>(other4, other5);
            return SIMD3Def(other0, other2, other4);
        }
    }

    /**
     * Maximum element from each SIMD6.
     * @returns SIMD2 set to the largest value in each SIMD6.
     */
    XS_INLINE SIMD2Def hmax6() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m128 val0 = _mm_max_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            val0 = _mm_max_ps(val0, _mm512_extractf32x4_ps(this->values, 2));
            const __m128 val1 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(3, 1, 3, 1));
            val0 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(2, 0, 2, 0));
            return SIMD2Def(_mm_max_ps(val0, val1));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m128 val0 = _mm_max_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            val0 = _mm_max_ps(val0, this->values1);
            const __m128 val1 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(3, 1, 3, 1));
            val0 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(2, 0, 2, 0));
            return SIMD2Def(_mm_max_ps(val0, val1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_max_ps(this->values0, this->values1);
            val0 = _mm_max_ps(val0, this->values2);
            const __m128 val1 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(3, 1, 3, 1));
            val0 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(2, 0, 2, 0));
            return SIMD2Def(_mm_max_ps(val0, val1));
        } else
#endif
        {
            T other0 = Shift::max<T>(this->values0, this->values1);
            T other1 = Shift::max<T>(this->values2, this->values3);
            const T other2 = Shift::max<T>(this->values4, this->values5);
            const T other3 = Shift::max<T>(this->values6, this->values7);
            const T other4 = Shift::max<T>(this->values8, this->values9);
            const T other5 = Shift::max<T>(this->values10, this->values11);
            other0 = Shift::max<T>(other0, other2);
            other1 = Shift::max<T>(other1, other3);
            other0 = Shift::max<T>(other0, other4);
            other1 = Shift::max<T>(other1, other5);
            return SIMD2Def(other0, other1);
        }
    }

    /**
     * Minimum element from each SIMD6.
     * @returns SIMD2 set to the largest value in each SIMD6.
     */
    XS_INLINE SIMD2Def hmin6() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m128 val0 = _mm_min_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            val0 = _mm_min_ps(val0, _mm512_extractf32x4_ps(this->values, 2));
            const __m128 val1 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(3, 1, 3, 1));
            val0 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(2, 0, 2, 0));
            return SIMD2Def(_mm_min_ps(val0, val1));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m128 val0 = _mm_min_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            val0 = _mm_min_ps(val0, this->values1);
            const __m128 val1 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(3, 1, 3, 1));
            val0 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(2, 0, 2, 0));
            return SIMD2Def(_mm_min_ps(val0, val1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_min_ps(this->values0, this->values1);
            val0 = _mm_min_ps(val0, this->values2);
            const __m128 val1 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(3, 1, 3, 1));
            val0 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(2, 0, 2, 0));
            return SIMD2Def(_mm_min_ps(val0, val1));
        } else
#endif
        {
            T other0 = Shift::min<T>(this->values0, this->values1);
            T other1 = Shift::min<T>(this->values2, this->values3);
            const T other2 = Shift::min<T>(this->values4, this->values5);
            const T other3 = Shift::min<T>(this->values6, this->values7);
            const T other4 = Shift::min<T>(this->values8, this->values9);
            const T other5 = Shift::min<T>(this->values10, this->values11);
            other0 = Shift::min<T>(other0, other2);
            other1 = Shift::min<T>(other1, other3);
            other0 = Shift::min<T>(other0, other4);
            other1 = Shift::min<T>(other1, other5);
            return SIMD2Def(other0, other1);
        }
    }

    /**
     * Compute the sum of each internally stored SIMD2.
     * @returns SIMD2 set to the sum of all elements between each internal SIMD2.
     */
    XS_INLINE SIMD2Def add2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m128 val = _mm_add_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            val = _mm_add_ps(val, _mm512_extractf32x4_ps(this->values, 2));
            return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(val), val));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m128 val = _mm_add_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            val = _mm_add_ps(val, this->values1);
            return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(val), val));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val = _mm_add_ps(this->values0, this->values1);
            val = _mm_add_ps(val, this->values2);
            return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(val), val));
        } else
#endif
        {
            T other0 = this->values0 + this->values2;
            T other1 = this->values1 + this->values3;
            const T other2 = this->values4 + this->values6;
            const T other3 = this->values5 + this->values7;
            const T other4 = this->values8 + this->values10;
            const T other5 = this->values9 + this->values11;
            other0 += other2;
            other1 += other3;
            other0 += other4;
            other1 += other5;
            return SIMD2Def(other0, other1);
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
            __m256 val = _mm256_hadd_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
            val = _mm256_hadd_ps(val, val);
            return SIMD3Def(_mm_unpacklo_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val = _mm256_hadd_ps(this->values0, _mm256_castps128_ps256(this->values1));
            val = _mm256_hadd_ps(val, val);
            return SIMD3Def(_mm_unpacklo_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_hadd_ps(this->values0, this->values1);
            const __m128 val1 = _mm_add_ps(_mm_movehl_ps(this->values2, this->values2), this->values2);
            return SIMD3Def(_mm_hadd_ps(val0, val1));
        } else
#endif
        {
            T other0 = this->values0 + this->values1;
            const T other1 = this->values2 + this->values3;
            T other2 = this->values4 + this->values5;
            const T other3 = this->values6 + this->values7;
            T other4 = this->values8 + this->values9;
            const T other5 = this->values10 + this->values11;
            other0 += other1;
            other2 += other3;
            other4 += other5;
            return SIMD3Def(other0, other2, other4);
        }
    }

    /**
     * Compute the sum of each internally stored SIMD4.
     * @returns SIMD4 set to the sum of all elements between each internal SIMD4.
     */
    XS_INLINE SIMD4Def add4() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 val =
                _mm_add_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            return SIMD4Def(_mm_add_ps(val, _mm512_extractf32x4_ps(this->values, 2)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val =
                _mm_add_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            return SIMD4Def(_mm_add_ps(val, this->values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_add_ps(this->values0, this->values1);
            return SIMD4Def(_mm_add_ps(val, this->values2));
        } else
#endif
        {
            T other0 = this->values0 + this->values4;
            T other1 = this->values1 + this->values5;
            T other2 = this->values2 + this->values6;
            T other3 = this->values3 + this->values7;
            other0 += this->values8;
            other1 += this->values9;
            other2 += this->values10;
            other3 += this->values11;
            return SIMD4Def(other0, other1, other2, other3);
        }
    }

    /**
     * Compute the sum of each internally stored SIMD6.
     * @returns SIMD6 set to the sum of all elements between each internal SIMD6.
     */
    XS_INLINE SIMD6Def add6() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 1, 2, 0));
            __m256 val1 = _mm512_castps512_ps256(val0);
            __m256 val2 = _mm512_extractf32x8_ps(val0, 1);
            __m256 val3 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(1, 0, 1, 0));
            __m256 val4 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 2, 3, 2));
            return SIMD6Def(_mm256_add_ps(val3, val4));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 lo = _mm256_castps256_ps128(this->values0);
            const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
            const __m128 val0 = _mm_movelh_ps(lo, hi);
            const __m128 val1 = _mm_movehl_ps(hi, lo);
            const __m128 val2 = _mm_shuffle3232_ps(this->values1);
            __m256 val4 = _mm256_set_m128(this->values1, val0);
            __m256 val5 = _mm256_set_m128(val2, val1);
            return SIMD6Def(_mm256_add_ps(val4, val5));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_movelh_ps(this->values0, this->values1);
            const __m128 val1 = _mm_movehl_ps(this->values1, this->values0);
            const __m128 val2 = _mm_shuffle3232_ps(this->values2);
            return SIMD6Def(_mm_add_ps(val0, val1), _mm_add_ps(this->values2, val2));
        } else
#endif
        {
            const T other0 = this->values0 + this->values2;
            const T other1 = this->values1 + this->values3;
            const T other2 = this->values4 + this->values6;
            const T other3 = this->values5 + this->values7;
            const T other4 = this->values8 + this->values10;
            const T other5 = this->values9 + this->values11;
            return SIMD6Def(other0, other1, other2, other3, other4, other5);
        }
    }

    /**
     * Compute the difference of each internally stored SIMD4.
     * @returns SIMD4 set to the difference of all elements between each internal SIMD4.
     */
    XS_INLINE SIMD4Def sub4() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 val =
                _mm_sub_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            return SIMD4Def(_mm_sub_ps(val, _mm512_extractf32x4_ps(this->values, 2)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val =
                _mm_sub_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            return SIMD4Def(_mm_sub_ps(val, this->values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_sub_ps(this->values0, this->values1);
            return SIMD4Def(_mm_sub_ps(val, this->values2));
        } else
#endif
        {
            T other0 = this->values0 - this->values4;
            T other1 = this->values1 - this->values5;
            T other2 = this->values2 - this->values6;
            T other3 = this->values3 - this->values7;
            other0 -= this->values8;
            other1 -= this->values9;
            other2 -= this->values10;
            other3 -= this->values11;
            return SIMD4Def(other0, other1, other2, other3);
        }
    }

    /**
     * Compute the difference of each internally stored SIMD6.
     * @returns SIMD6 set to the difference of all elements between each internal SIMD6.
     */
    XS_INLINE SIMD6Def sub6() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 1, 2, 0));
            __m256 val1 = _mm512_castps512_ps256(val0);
            __m256 val2 = _mm512_extractf32x8_ps(val0, 1);
            __m256 val3 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(1, 0, 1, 0));
            __m256 val4 = _mm256_shuffle_ps(val1, val2, _MM_SHUFFLE(3, 2, 3, 2));
            return SIMD6Def(_mm256_sub_ps(val3, val4));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 lo = _mm256_castps256_ps128(this->values0);
            const __m128 hi = _mm256_extractf128_ps(this->values0, 1);
            const __m128 val0 = _mm_movelh_ps(lo, hi);
            const __m128 val1 = _mm_movehl_ps(hi, lo);
            const __m128 val2 = _mm_shuffle3232_ps(this->values1);
            __m256 val4 = _mm256_set_m128(this->values1, val0);
            __m256 val5 = _mm256_set_m128(val2, val1);
            return SIMD6Def(_mm256_sub_ps(val4, val5));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_movelh_ps(this->values0, this->values1);
            const __m128 val1 = _mm_movehl_ps(this->values1, this->values0);
            const __m128 val2 = _mm_shuffle3232_ps(this->values2);
            return SIMD6Def(_mm_sub_ps(val0, val1), _mm_sub_ps(this->values2, val2));
        } else
#endif
        {
            const T other0 = this->values0 - this->values2;
            const T other1 = this->values1 - this->values3;
            const T other2 = this->values4 - this->values6;
            const T other3 = this->values5 - this->values7;
            const T other4 = this->values8 - this->values10;
            const T other5 = this->values9 - this->values11;
            return SIMD6Def(other0, other1, other2, other3, other4, other5);
        }
    }

    /**
     * Compute the sum of all elements.
     * @returns BaseDef set to the sum of all elements.
     */
    XS_INLINE BaseDef hadd() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m128 val = _mm_add_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            val = _mm_add_ps(val, _mm512_extractf32x4_ps(this->values, 2));
            val = _mm_add_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
            val = _mm_add_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val);
            return BaseDef(_mm512_broadcastf128_ps(val));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m128 val = _mm_add_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            val = _mm_add_ps(val, this->values1);
            val = _mm_add_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
            val = _mm_add_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val);
            return BaseDef(_mm256_broadcastf128_ps(val));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val = _mm_add_ps(this->values0, this->values1);
            val = _mm_add_ps(val, this->values2);
            val = _mm_add_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(1, 0, 3, 2)), val);
            return BaseDef(_mm_add_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
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
    XS_INLINE InBaseDef haddInBase() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m128 val = _mm_add_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            val = _mm_add_ps(val, _mm512_extractf32x4_ps(this->values, 2));
            val = _mm_add_ps(_mm_shuffle3232_ps(val), val);
            return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m128 val = _mm_add_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            val = _mm_add_ps(val, this->values1);
            val = _mm_add_ps(_mm_shuffle3232_ps(val), val);
            return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val = _mm_add_ps(this->values0, this->values1);
            val = _mm_add_ps(val, this->values2);
            val = _mm_add_ps(_mm_shuffle3232_ps(val), val);
            return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
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
            res += res3;
            res2 += res4;
            res += res2;
            return InBaseDef(res);
        }
    }

    /**
     * Compute the sum of each SIMD2.
     * @returns SIMD6 set to the sum of all elements in each SIMD2.
     */
    XS_INLINE SIMD6Def hadd2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 1, 2, 0));
            const __m256 val1 = _mm512_castps512_ps256(val0);
            const __m256 val2 = _mm512_extractf32x8_ps(val0, 1);
            return SIMD6Def(_mm256_hadd_ps(val1, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                return SIMD6Def(_mm256_castpd_ps(_mm256_permute4x64_pd(
                    _mm256_castps_pd(_mm256_hadd_ps(this->values0, _mm256_castps128_ps256(this->values1))),
                    _MM_SHUFFLE(3, 1, 2, 0))));
            } else {
                const __m256 val0 = _mm256_hadd_ps(this->values0, _mm256_castps128_ps256(this->values1));
                __m256 val1 = _mm256_shuffle32107654_ps(val0);
                val1 = _mm256_shuffle1_ps(val1, _MM_SHUFFLE(1, 0, 3, 2));
                return SIMD6Def(_mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 1, 1, 0, 0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_hadd_ps(this->values0, this->values1);
            __m128 val1 = _mm_shuffle1_ps(this->values2, _MM_SHUFFLE(2, 0, 2, 0));
            const __m128 val2 = _mm_shuffle1_ps(this->values2, _MM_SHUFFLE(3, 1, 3, 1));
            val1 = _mm_add_ps(val1, val2);
            return SIMD6Def(val0, val1);
        } else
#endif
        {
            return SIMD6Def(this->values0 + this->values1, this->values2 + this->values3, this->values4 + this->values5,
                this->values6 + this->values7, this->values8 + this->values9, this->values10 + this->values11);
        }
    }

    /**
     * Compute the sum of each SIMD3.
     * @returns SIMD4 set to the sum of all elements in each SIMD3.
     */
    XS_INLINE SIMD4Def hadd3() const noexcept
    {
        // add4
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 val =
                _mm_add_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            return SIMD4Def(_mm_add_ps(val, _mm512_extractf32x4_ps(this->values, 2)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val =
                _mm_add_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            return SIMD4Def(_mm_add_ps(val, this->values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_add_ps(this->values0, this->values1);
            return SIMD4Def(_mm_add_ps(val, this->values2));
        } else
#endif
        {
            T res = (this->values0 + this->values4);
            T res2 = (this->values1 + this->values5);
            T res3 = (this->values2 + this->values6);
            T res4 = (this->values3 + this->values7);
            res += this->values8;
            res2 += this->values9;
            res3 += this->values10;
            res4 += this->values11;
            return SIMD4Def(res, res2, res3, res4);
        }
    }

    /**
     * Compute the sum of each SIMD4.
     * @returns SIMD3 set to the sum of all elements in each SIMD4.
     */
    XS_INLINE SIMD3Def hadd4() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m256 val = _mm256_hadd_ps(_mm512_castps512_ps256(this->values), _mm512_extractf32x8_ps(this->values, 1));
            val = _mm256_hadd_ps(val, val);
            return SIMD3Def(_mm_unpacklo_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val = _mm256_hadd_ps(this->values0, _mm256_castps128_ps256(this->values1));
            val = _mm256_hadd_ps(val, val);
            return SIMD3Def(_mm_unpacklo_ps(_mm256_castps256_ps128(val), _mm256_extractf128_ps(val, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_hadd_ps(this->values0, this->values1);
            const __m128 val1 = _mm_add_ps(_mm_shuffle3232_ps(this->values2), this->values2);
            return SIMD3Def(_mm_hadd_ps(val0, val1));
        } else
#endif
        {
            T res = (this->values0 + this->values1);
            T res2 = (this->values4 + this->values5);
            T res3 = (this->values8 + this->values9);
            res += this->values2;
            res2 += this->values6;
            res3 += this->values10;
            res += this->values3;
            res2 += this->values7;
            res3 += this->values11;
            return SIMD3Def(res, res2, res3);
        }
    }

    /**
     * Compute the sum of each SIMD6.
     * @returns SIMD2 set to the sum of all elements in each SIMD6.
     */
    XS_INLINE SIMD2Def hadd6() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            __m128 val0 = _mm_add_ps(_mm512_castps512_ps128(this->values), _mm512_extractf32x4_ps(this->values, 1));
            val0 = _mm_add_ps(val0, _mm512_extractf32x4_ps(this->values, 2));
            const __m128 val1 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(3, 1, 3, 1));
            val0 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(2, 0, 2, 0));
            return SIMD2Def(_mm_add_ps(val0, val1));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m128 val0 = _mm_add_ps(_mm256_castps256_ps128(this->values0), _mm256_extractf128_ps(this->values0, 1));
            val0 = _mm_add_ps(val0, this->values1);
            const __m128 val1 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(3, 1, 3, 1));
            val0 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(2, 0, 2, 0));
            return SIMD2Def(_mm_add_ps(val0, val1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_add_ps(this->values0, this->values1);
            val0 = _mm_add_ps(val0, this->values2);
            const __m128 val1 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(3, 1, 3, 1));
            val0 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(2, 0, 2, 0));
            return SIMD2Def(_mm_add_ps(val0, val1));
        } else
#endif
        {
            T other0 = this->values0 + this->values1;
            T other1 = this->values2 + this->values3;
            const T other2 = this->values4 + this->values5;
            const T other3 = this->values6 + this->values7;
            const T other4 = this->values8 + this->values9;
            const T other5 = this->values10 + this->values11;
            other0 += other2;
            other1 += other3;
            other0 += other4;
            other1 += other5;
            return SIMD2Def(other0, other1);
        }
    }

    /**
     * Compute the difference of each SIMD2.
     * @returns SIMD6 set to the difference of all elements in each SIMD2.
     */
    XS_INLINE SIMD6Def hsub2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 val0 = _mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(2, 1, 2, 0));
            const __m256 val1 = _mm512_castps512_ps256(val0);
            const __m256 val2 = _mm512_extractf32x8_ps(val0, 1);
            return SIMD6Def(_mm256_hsub_ps(val1, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val0 = _mm256_hsub_ps(this->values0, _mm256_castps128_ps256(this->values1));
            __m256 val1 = _mm256_shuffle32107654_ps(val0);
            val1 = _mm256_shuffle1_ps(val1, _MM_SHUFFLE(1, 0, 3, 2));
            return SIMD6Def(_mm256_blend_ps(val0, val1, _MM256_BLEND(0, 0, 1, 1, 1, 1, 0, 0)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_hsub_ps(this->values0, this->values1);
            __m128 val1 = _mm_shuffle1_ps(this->values2, _MM_SHUFFLE(2, 0, 2, 0));
            const __m128 val2 = _mm_shuffle1_ps(this->values2, _MM_SHUFFLE(3, 1, 3, 1));
            val1 = _mm_sub_ps(val1, val2);
            return SIMD6Def(val0, val1);
        } else
#endif
        {
            return SIMD6Def(this->values0 - this->values1, this->values2 - this->values3, this->values4 - this->values5,
                this->values6 - this->values7, this->values8 - this->values9, this->values10 - this->values11);
        }
    }

    /**
     * Compute the 3 dimensional dot product of each SIMD3 in two objects.
     * @param other The second object.
     * @returns The value of the dot products.
     */
    XS_INLINE SIMD4Def dot3(const SIMD12& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 val0 = _mm512_mul_ps(this->values, other.values);
            const __m128 val = _mm_add_ps(_mm512_castps512_ps128(val0), _mm512_extractf32x4_ps(val0, 1));
            return SIMD4Def(_mm_add_ps(val, _mm512_extractf32x4_ps(val0, 2)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val0 = _mm256_mul_ps(this->values0, other.values0);
            const __m128 val1 = _mm_mul_ps(this->values1, other.values1);
            const __m128 val = _mm_add_ps(_mm256_castps256_ps128(val0), _mm256_extractf128_ps(val0, 1));
            return SIMD4Def(_mm_add_ps(val, val1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_mul_ps(this->values0, other.values0);
            const __m128 val1 = _mm_mul_ps(this->values1, other.values1);
            const __m128 val2 = _mm_mul_ps(this->values2, other.values2);
            const __m128 val = _mm_add_ps(val0, val1);
            return SIMD4Def(_mm_add_ps(val, val2));
        } else
#endif
        {
            T v0Sq = this->values0 * other.values0;
            T v1Sq = this->values1 * other.values1;
            T v2Sq = this->values2 * other.values2;
            T v3Sq = this->values3 * other.values3;
            v0Sq = Shift::fma<T>(this->values4, other.values4, v0Sq);
            v1Sq = Shift::fma<T>(this->values5, other.values5, v1Sq);
            v2Sq = Shift::fma<T>(this->values6, other.values6, v2Sq);
            v3Sq = Shift::fma<T>(this->values7, other.values7, v3Sq);
            return SIMD4Def(Shift::fma<T>(this->values8, other.values8, v0Sq),
                Shift::fma<T>(this->values9, other.values9, v1Sq), Shift::fma<T>(this->values10, other.values10, v2Sq),
                Shift::fma<T>(this->values11, other.values11, v3Sq));
        }
    }

    /**
     * Compute the 3 dimensional cross product of each SIMD3 in two objects.
     * @note Uses Right-handed coordinate system.
     * @param other The second object.
     * @returns The cross product of the inputs.
     */
    XS_INLINE SIMD12 cross3(const SIMD12& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 val = _mm512_mul_ps(_mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 0, 2, 1)),
                _mm512_shuffle_f32x4(other.values, other.values, _MM_SHUFFLE(3, 1, 0, 2)));
            return SIMD12(_mm512_fnmadd_ps(_mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(3, 1, 0, 2)),
                _mm512_shuffle_f32x4(other.values, other.values, _MM_SHUFFLE(3, 0, 2, 1)), val));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val0 = _mm256_mul_ps(
                _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(2, 1)),
                _mm256_permute2f128_ps(other.values0, _mm256_castps128_ps256(other.values1), _MM256_PERMUTE(0, 2)));
            const __m256 val1 = _mm256_mul_ps(this->values0, _mm256_shuffle32107654_ps(other.values0));
            return SIMD12(
                _mm256_fnmadd_ps(
                    _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(0, 2)),
                    _mm256_permute2f128_ps(other.values0, _mm256_castps128_ps256(other.values1), _MM256_PERMUTE(2, 1)),
                    val0),
                _mm_sub_ps(_mm256_castps256_ps128(val1), _mm256_extractf128_ps(val1, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_mul_ps(this->values1, other.values2);
            const __m128 val1 = _mm_mul_ps(this->values2, other.values0);
            const __m128 val2 = _mm_mul_ps(this->values0, other.values1);
            return SIMD12(_mm_fnmadd_ps(this->values2, other.values1, val0),
                _mm_fnmadd_ps(this->values0, other.values2, val1), _mm_fnmadd_ps(this->values1, other.values0, val2));
        } else
#endif
        {
            const T v0CrossX = -(this->values8 * other.values4);
            const T v1CrossX = -(this->values9 * other.values5);
            const T v2CrossX = -(this->values10 * other.values6);
            const T v3CrossX = -(this->values11 * other.values7);
            const T v0CrossY = -(this->values0 * other.values8);
            const T v1CrossY = -(this->values1 * other.values9);
            const T v2CrossY = -(this->values2 * other.values10);
            const T v3CrossY = -(this->values3 * other.values11);
            const T v0CrossZ = -(this->values4 * other.values0);
            const T v1CrossZ = -(this->values5 * other.values1);
            const T v2CrossZ = -(this->values6 * other.values2);
            const T v3CrossZ = -(this->values7 * other.values3);

            return SIMD12(Shift::fma<T>(this->values4, other.values8, v0CrossX),
                Shift::fma<T>(this->values5, other.values9, v1CrossX),
                Shift::fma<T>(this->values6, other.values10, v2CrossX),
                Shift::fma<T>(this->values7, other.values11, v3CrossX),
                Shift::fma<T>(this->values8, other.values0, v0CrossY),
                Shift::fma<T>(this->values9, other.values1, v1CrossY),
                Shift::fma<T>(this->values10, other.values2, v2CrossY),
                Shift::fma<T>(this->values11, other.values3, v3CrossY),
                Shift::fma<T>(this->values0, other.values4, v0CrossZ),
                Shift::fma<T>(this->values1, other.values5, v1CrossZ),
                Shift::fma<T>(this->values2, other.values6, v2CrossZ),
                Shift::fma<T>(this->values3, other.values7, v3CrossZ));
        }
    }

    /**
     * Compute the square of the length of each internal SIMD3.
     * @returns Squared lengths.
     */
    XS_INLINE SIMD4Def lengthSqr3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 val0 = _mm512_mul_ps(this->values, this->values);
            const __m128 val = _mm_add_ps(_mm512_castps512_ps128(val0), _mm512_extractf32x4_ps(val0, 1));
            return SIMD4Def(_mm_add_ps(val, _mm512_extractf32x4_ps(val0, 2)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val0 = _mm256_mul_ps(this->values0, this->values0);
            const __m128 val1 = _mm_mul_ps(this->values1, this->values1);
            const __m128 val = _mm_add_ps(_mm256_castps256_ps128(val0), _mm256_extractf128_ps(val0, 1));
            return SIMD4Def(_mm_add_ps(val, val1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_mul_ps(this->values0, this->values0);
            const __m128 val1 = _mm_mul_ps(this->values1, this->values1);
            const __m128 val2 = _mm_mul_ps(this->values2, this->values2);
            const __m128 val = _mm_add_ps(val0, val1);
            return SIMD4Def(_mm_add_ps(val, val2));
        } else
#endif
        {
            T v0Sq = this->values0 * this->values0;
            T v1Sq = this->values1 * this->values1;
            T v2Sq = this->values2 * this->values2;
            T v3Sq = this->values3 * this->values3;
            v0Sq = Shift::fma<T>(this->values4, this->values4, v0Sq);
            v1Sq = Shift::fma<T>(this->values5, this->values5, v1Sq);
            v2Sq = Shift::fma<T>(this->values6, this->values6, v2Sq);
            v3Sq = Shift::fma<T>(this->values7, this->values7, v3Sq);
            return SIMD4Def(Shift::fma<T>(this->values8, this->values8, v0Sq),
                Shift::fma<T>(this->values9, this->values9, v1Sq), Shift::fma<T>(this->values10, this->values10, v2Sq),
                Shift::fma<T>(this->values11, this->values11, v3Sq));
        }
    }

    /**
     * Compute the length of each internal SIMD3.
     * @returns The length.
     */
    XS_INLINE SIMD4Def length3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 val0 = _mm512_mul_ps(this->values, this->values);
            const __m128 val = _mm_add_ps(_mm512_castps512_ps128(val0), _mm512_extractf32x4_ps(val0, 1));
            return SIMD4Def(_mm_sqrt_ps(_mm_add_ps(val, _mm512_extractf32x4_ps(val0, 2))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val0 = _mm256_mul_ps(this->values0, this->values0);
            const __m128 val1 = _mm_mul_ps(this->values1, this->values1);
            const __m128 val = _mm_add_ps(_mm256_castps256_ps128(val0), _mm256_extractf128_ps(val0, 1));
            return SIMD4Def(_mm_sqrt_ps(_mm_add_ps(val, val1)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_mul_ps(this->values0, this->values0);
            const __m128 val1 = _mm_mul_ps(this->values1, this->values1);
            const __m128 val2 = _mm_mul_ps(this->values2, this->values2);
            const __m128 val = _mm_add_ps(val0, val1);
            return SIMD4Def(_mm_sqrt_ps(_mm_add_ps(val, val2)));
        } else
#endif
        {
            T v0Sq = this->values0 * this->values0;
            T v1Sq = this->values1 * this->values1;
            T v2Sq = this->values2 * this->values2;
            T v3Sq = this->values3 * this->values3;
            v0Sq = Shift::fma<T>(this->values4, this->values4, v0Sq);
            v1Sq = Shift::fma<T>(this->values5, this->values5, v1Sq);
            v2Sq = Shift::fma<T>(this->values6, this->values6, v2Sq);
            v3Sq = Shift::fma<T>(this->values7, this->values7, v3Sq);
            v0Sq = Shift::fma<T>(this->values8, this->values8, v0Sq);
            v1Sq = Shift::fma<T>(this->values9, this->values9, v1Sq);
            v2Sq = Shift::fma<T>(this->values10, this->values10, v2Sq);
            v3Sq = Shift::fma<T>(this->values11, this->values11, v3Sq);
            return SIMD4Def(Shift::sqrt<T>(v0Sq), Shift::sqrt<T>(v1Sq), Shift::sqrt<T>(v2Sq), Shift::sqrt<T>(v3Sq));
        }
    }

    /**
     * Normalise each internal SIMD3.
     * @note The result is unpredictable when all elements are at or near zero.
     * @returns The normalised values.
     */
    XS_INLINE SIMD12 normalize3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 val0 = _mm512_mul_ps(this->values, this->values);
            __m128 val = _mm_add_ps(_mm512_castps512_ps128(val0), _mm512_extractf32x4_ps(val0, 1));
            val = _mm_sqrt_ps(_mm_add_ps(val, _mm512_extractf32x4_ps(val0, 2)));
            return SIMD12(_mm512_div_ps(this->values, _mm512_broadcastf128_ps(val)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val0 = _mm256_mul_ps(this->values0, this->values0);
            const __m128 val1 = _mm_mul_ps(this->values1, this->values1);
            __m128 val = _mm_add_ps(_mm256_castps256_ps128(val0), _mm256_extractf128_ps(val0, 1));
            val = _mm_sqrt_ps(_mm_add_ps(val, val1));
            return SIMD12(_mm256_div_ps(this->values0, _mm256_broadcastf128_ps(val)), _mm_div_ps(this->values1, val));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_mul_ps(this->values0, this->values0);
            const __m128 val1 = _mm_mul_ps(this->values1, this->values1);
            const __m128 val2 = _mm_mul_ps(this->values2, this->values2);
            __m128 val = _mm_add_ps(val0, val1);
            val = _mm_sqrt_ps(_mm_add_ps(val, val2));
            return SIMD12(
                _mm_div_ps(this->values0, val), _mm_div_ps(this->values1, val), _mm_div_ps(this->values2, val));
        } else
#endif
        {
            T v0Sq = this->values0 * this->values0;
            T v1Sq = this->values1 * this->values1;
            T v2Sq = this->values2 * this->values2;
            T v3Sq = this->values3 * this->values3;
            v0Sq = Shift::fma<T>(this->values4, this->values4, v0Sq);
            v1Sq = Shift::fma<T>(this->values5, this->values5, v1Sq);
            v2Sq = Shift::fma<T>(this->values6, this->values6, v2Sq);
            v3Sq = Shift::fma<T>(this->values7, this->values7, v3Sq);
            v0Sq = Shift::fma<T>(this->values8, this->values8, v0Sq);
            v1Sq = Shift::fma<T>(this->values9, this->values9, v1Sq);
            v2Sq = Shift::fma<T>(this->values10, this->values10, v2Sq);
            v3Sq = Shift::fma<T>(this->values11, this->values11, v3Sq);
            v0Sq = Shift::sqrt<T>(v0Sq);
            v1Sq = Shift::sqrt<T>(v1Sq);
            v2Sq = Shift::sqrt<T>(v2Sq);
            v3Sq = Shift::sqrt<T>(v3Sq);
            return SIMD12(this->values0 / v0Sq, this->values1 / v1Sq, this->values2 / v2Sq, this->values3 / v3Sq,
                this->values4 / v0Sq, this->values5 / v1Sq, this->values6 / v2Sq, this->values7 / v3Sq,
                this->values8 / v0Sq, this->values9 / v1Sq, this->values10 / v2Sq, this->values11 / v3Sq);
        }
    }

    /**
     * Returns binary logarithm of each element in the object.
     * @returns Object containing the binary logarithm of the input.
     */
    XS_INLINE SIMD12 log2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD12(NoExport::log2f16(this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32)) {
        } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16)) {
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        } else
#endif
        {
        }
    }

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD2.
     * @tparam Index0 The index in the first object to insert the element (range is 0-1).
     * @tparam Index1 The index in the second object to copy element from (range is 0-1).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_INLINE SIMD12 insert2(const SIMD12& other) const noexcept
    {
        static_assert(Index0 < 2 && Index1 < 2, "Invalid Index: Indexes must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD12(_mm512_mask_mov_ps(this->values, _cvtu32_mask16(0x5555), other.values));
            } else if constexpr (Index0 == 0 && Index1 == 1) {
                const __m512 ret = _mm512_shuffle_ps(this->values, other.values, _MM_SHUFFLE(3, 1, 3, 1));
                return SIMD12(_mm512_shuffle1_ps(ret, _MM_SHUFFLE(1, 3, 0, 2)));
            } else if constexpr (Index0 == 1 && Index1 == 0) {
                const __m512 ret = _mm512_shuffle_ps(this->values, other.values, _MM_SHUFFLE(2, 0, 2, 0));
                return SIMD12(_mm512_shuffle1_ps(ret, _MM_SHUFFLE(3, 1, 2, 0)));
            } else /*Index0 == 1 && Index1 == 1*/ {
                return SIMD12(_mm512_mask_mov_ps(this->values, _cvtu32_mask16(0xAAAA), other.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD12(_mm256_blend_ps(this->values0, other.values0, _MM256_BLEND(0, 1, 0, 1, 0, 1, 0, 1)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(0, 1, 0, 1)));
            } else if constexpr (Index0 == 0 && Index1 == 1) {
                const __m256 ret = _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(3, 1, 3, 1));
                const __m128 ret2 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(3, 1, 3, 1));
                return SIMD12(
                    _mm256_shuffle1_ps(ret, _MM_SHUFFLE(1, 3, 0, 2)), _mm_shuffle1_ps(ret2, _MM_SHUFFLE(1, 3, 0, 2)));
            } else if constexpr (Index0 == 1 && Index1 == 0) {
                const __m256 ret = _mm256_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(2, 0, 2, 0));
                const __m128 ret2 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(2, 0, 2, 0));
                return SIMD12(
                    _mm256_shuffle1_ps(ret, _MM_SHUFFLE(3, 1, 2, 0)), _mm_shuffle1_ps(ret2, _MM_SHUFFLE(3, 1, 2, 0)));
            } else /*Index0 == 1 && Index1 == 1*/ {
                return SIMD12(_mm256_blend_ps(this->values0, other.values0, _MM256_BLEND(1, 0, 1, 0, 1, 0, 1, 0)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(1, 0, 1, 0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD12(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(0, 1, 0, 1)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(0, 1, 0, 1)),
                    _mm_blend_ps(this->values2, other.values2, _MM_BLEND(0, 1, 0, 1)));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD12(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(1, 0, 1, 0)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(1, 0, 1, 0)),
                    _mm_blend_ps(this->values2, other.values2, _MM_BLEND(1, 0, 1, 0)));
            } else {
                const __m128 val0 = _mm_insert_ps(this->values0, other.values0, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                const __m128 val1 = _mm_insert_ps(this->values1, other.values1, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                const __m128 val2 = _mm_insert_ps(this->values2, other.values2, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                return SIMD12(_mm_insert_ps(val0, other.values0, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)),
                    _mm_insert_ps(val1, other.values1, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)),
                    _mm_insert_ps(val2, other.values2, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)));
            }
        } else
#endif
        {
            return SIMD12(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
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
                Index0 != 1 ? this->values11 : (&other.values0)[Index1 + 10]);
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
    XS_INLINE SIMD12 insert3(const SIMD12& other) const noexcept
    {
        static_assert(Index0 < 3 && Index1 < 3, "Invalid Index: Indexes must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD12(_mm512_mask_blend_ps(_cvtu32_mask16(0xF), this->values, other.values));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD12(_mm512_mask_blend_ps(_cvtu32_mask16(0xF0), this->values, other.values));
            } else if constexpr (Index0 == 2 && Index1 == 2) {
                return SIMD12(_mm512_mask_blend_ps(_cvtu32_mask16(0xF00), this->values, other.values));
            } else {
                return SIMD12(_mm512_mask_shuffle_f32x4(this->values, _cvtu32_mask16(0xF << (Index0 * 4)), other.values,
                    other.values, _MM_SHUFFLE(Index1, Index1, Index1, Index1)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD12(
                    _mm256_blend_ps(this->values0, other.values0, _MM256_BLEND(0, 0, 0, 0, 1, 1, 1, 1)), this->values1);
            } else if constexpr (Index0 == 0 && Index1 == 1) {
                return SIMD12(
                    _mm256_permute2f128_ps(this->values0, other.values0, _MM256_PERMUTE(1, 3)), this->values1);
            } else if constexpr (Index0 == 0 && Index1 == 2) {
                return SIMD12(_mm256_insertf128_ps(this->values0, other.values1, 0), this->values1);
            } else if constexpr (Index0 == 1 && Index1 == 0) {
                return SIMD12(
                    _mm256_permute2f128_ps(this->values0, other.values0, _MM256_PERMUTE(2, 0)), this->values1);
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD12(
                    _mm256_blend_ps(this->values0, other.values0, _MM256_BLEND(1, 1, 1, 1, 0, 0, 0, 0)), this->values1);
            } else if constexpr (Index0 == 1 && Index1 == 2) {
                return SIMD12(_mm256_insertf128_ps(this->values0, other.values1, 1), this->values1);
            } else if constexpr (Index0 == 2 && Index1 == 0) {
                return SIMD12(this->values0, _mm256_castps256_ps128(other.values0));
            } else if constexpr (Index0 == 2 && Index1 == 1) {
                return SIMD12(this->values0, _mm256_extractf128_ps(other.values0, 1));
            } else /*Index0 == 2 && Index1 == 2*/ {
                return SIMD12(this->values0, other.values1);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD12(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
                Index0 != 1 ? this->values1 : (&other.values0)[Index1],
                Index0 != 2 ? this->values2 : (&other.values0)[Index1]);
        } else
#endif
        {
            return SIMD12(SIMD3Def(this->values0, this->values4, this->values8)
                              .template insert<Index0, Index1>(SIMD3Def(other.values0, other.values4, other.values8)),
                SIMD3Def(this->values1, this->values5, this->values9)
                    .template insert<Index0, Index1>(SIMD3Def(other.values1, other.values5, other.values9)),
                SIMD3Def(this->values2, this->values6, this->values10)
                    .template insert<Index0, Index1>(SIMD3Def(other.values2, other.values6, other.values10)),
                SIMD3Def(this->values3, this->values7, this->values11)
                    .template insert<Index0, Index1>(SIMD3Def(other.values3, other.values7, other.values11)));
        }
    }

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD4.
     * @tparam Index0 The index in the first object to insert the element (range is 0-3).
     * @tparam Index1 The index in the second object to copy element from (range is 0-3).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_INLINE SIMD12 insert4(const SIMD12& other) const noexcept
    {
        static_assert(Index0 < 4 && Index1 < 4, "Invalid Index: Indexes must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == Index1) {
                return SIMD12(_mm512_mask_mov_ps(this->values,
                    _cvtu32_mask16((1 << Index0) | (1 << (Index0 + 4)) | (1 << (Index0 + 8))), other.values));
            } else {
                return SIMD12(_mm512_mask_shuffle_ps(this->values, _cvtu32_mask16(0x1111 << Index0), other.values,
                    other.values, _MM_SHUFFLE(Index1, Index1, Index1, Index1)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == Index1) {
                return SIMD12(_mm256_blend_ps(this->values0, other.values0, (1 << Index0) | (1 << (Index0 + 4))),
                    _mm_blend_ps(this->values1, other.values1, 1 << Index0));
            } else if constexpr (Index0 == 0) {
                const __m256 val = _mm256_shuffle_ps(
                    this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 1, 1)); /*(x,Index1,x,1)*/
                const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 1, 1));
                return SIMD12(_mm256_shuffle_ps(val, this->values0, _MM_SHUFFLE(3, 2, 0, 2)),
                    _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 0, 2)));
            } else if constexpr (Index0 == 1) {
                const __m256 val = _mm256_shuffle_ps(
                    this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 0, 0)); /*(x,Index1,x,0)*/
                const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 0, 0));
                return SIMD12(_mm256_shuffle_ps(val, this->values0, _MM_SHUFFLE(3, 2, 2, 0)),
                    _mm_shuffle_ps(val1, this->values1, _MM_SHUFFLE(3, 2, 2, 0)));
            } else if constexpr (Index0 == 2) {
                const __m256 val = _mm256_shuffle_ps(
                    this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 3, 3)); /*(x,Index1,x,3)*/
                const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 3, 3));
                return SIMD12(_mm256_shuffle_ps(this->values0, val, _MM_SHUFFLE(0, 2, 1, 0)),
                    _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(0, 2, 1, 0)));
            } else /*Index0 == 3*/ {
                const __m256 val = _mm256_shuffle_ps(
                    this->values0, other.values0, _MM_SHUFFLE(Index1, Index1, 2, 2)); /*(x,Index1,x,2)*/
                const __m128 val1 = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE(Index1, Index1, 2, 2));
                return SIMD12(_mm256_shuffle_ps(this->values0, val, _MM_SHUFFLE(2, 0, 1, 0)),
                    _mm_shuffle_ps(this->values1, val1, _MM_SHUFFLE(2, 0, 1, 0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD12(_mm_blend_ss(this->values0, other.values0), _mm_blend_ss(this->values1, other.values1),
                    _mm_blend_ss(this->values2, other.values2));
            } else if constexpr (Index0 == Index1) {
                return SIMD12(_mm_blend_ps(this->values0, other.values0, 1UL << Index0),
                    _mm_blend_ps(this->values1, other.values1, 1UL << Index0),
                    _mm_blend_ps(this->values2, other.values2, 1UL << Index0));
            } else {
                return SIMD12(_mm_insert_ps(this->values0, other.values0, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)),
                    _mm_insert_ps(this->values1, other.values1, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)),
                    _mm_insert_ps(this->values2, other.values2, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)));
            }
        } else
#endif
        {
            return SIMD12(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
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
                Index0 != 3 ? this->values11 : (&other.values0)[Index1 + 8]);
        }
    }

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index. This
     * performs the same operation on each internal SIMD6.
     * @tparam Index0 The index in the first object to insert the element (range is 0-5).
     * @tparam Index1 The index in the second object to copy element from (range is 0-5).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_INLINE SIMD12 insert6(const SIMD12& other) const noexcept
    {
        static_assert(Index0 < 6 && Index1 < 6, "Invalid Index: Indexes must be <6");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == Index1) {
                return SIMD12(_mm512_mask_mov_ps(
                    this->values, _cvtu32_mask16(0x5 << ((Index0 % 2) + (Index0 / 2 * 4))), other.values));
            } else if constexpr ((Index1 + (Index0 % 2)) % 2 != 0 && (Index0 / 2 == Index1 / 2)) {
                return SIMD12(_mm512_mask_permute_ps(this->values,
                    _cvtu32_mask16(0x5 << ((Index0 % 2) + (Index0 / 2 * 4))), other.values, _MM_SHUFFLE(2, 3, 0, 1)));
            } else if constexpr ((Index1 + (Index0 % 2)) % 2 != 0) {
                const __m512 val = _mm512_shuffle1_ps(other.values, _MM_SHUFFLE(2, 3, 0, 1));
                return SIMD12(
                    _mm512_mask_shuffle_f32x4(this->values, _cvtu32_mask16(0x5 << ((Index0 % 2) + (Index0 / 2 * 4))),
                        val, val, _MM_SHUFFLE(Index1 / 2, Index1 / 2, Index1 / 2, Index1 / 2)));
            } else {
                return SIMD12(
                    _mm512_mask_shuffle_f32x4(this->values, _cvtu32_mask16(0x5 << ((Index0 % 2) + (Index0 / 2 * 4))),
                        other.values, other.values, _MM_SHUFFLE(Index1 / 2, Index1 / 2, Index1 / 2, Index1 / 2)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == Index1 && Index0 < 4) {
                return SIMD12(_mm256_blend_ps(this->values0, other.values0, 0x5 << ((Index0 % 2) + (Index0 / 2 * 4))),
                    this->values1);
            } else if constexpr (Index0 == Index1) {
                return SIMD12(this->values0, _mm_blend_ps(this->values1, other.values1, 0x5 << (Index0 % 2)));
            } else if constexpr (Index0 < 2) {
                __m256 val;
                if constexpr (Index1 < 2) {
                    val = other.values0;
                } else if constexpr (Index1 < 4) {
                    val = _mm256_shuffle32107654_ps(other.values0);
                } else {
                    val = _mm256_castps128_ps256(other.values1);
                }
                if constexpr ((Index1 + (Index0 % 2)) % 2 != 0) {
                    val = _mm256_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1));
                }
                return SIMD12(_mm256_blend_ps(this->values0, val, 0x5 << (Index0 % 2)), this->values1);
            } else if constexpr (Index0 < 4) {
                __m256 val;
                if constexpr (Index1 < 2) {
                    val = _mm256_shuffle32107654_ps(other.values0);
                } else if constexpr (Index1 < 4) {
                    val = other.values0;
                } else {
                    val = _mm256_broadcastf128_ps(other.values1);
                }
                if constexpr ((Index1 + (Index0 % 2)) % 2 != 0) {
                    val = _mm256_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1));
                }
                return SIMD12(_mm256_blend_ps(this->values0, val, 0x50 << (Index0 % 2)), this->values1);
            } else if constexpr (Index0 == 4) {
                __m128 val;
                if constexpr (Index1 < 2) {
                    val = _mm_shuffle_ps(this->values1, _mm256_castps256_ps128(other.values0),
                        _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 3, 1));
                } else if constexpr (Index1 < 4) {
                    val = _mm_shuffle_ps(this->values1, _mm256_extractf128_ps(other.values0, 1),
                        _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 3, 1));
                } else {
                    val = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 3, 1));
                }
                return SIMD12(this->values0, _mm_shuffle1_ps(val, _MM_SHUFFLE(1, 3, 0, 2)));
            } else /*Index0 == 5*/ {
                __m128 val;
                if constexpr (Index1 < 2) {
                    val = _mm_shuffle_ps(this->values1, _mm256_castps256_ps128(other.values0),
                        _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 2, 0));
                } else if constexpr (Index1 < 4) {
                    val = _mm_shuffle_ps(this->values1, _mm256_extractf128_ps(other.values0, 1),
                        _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 2, 0));
                } else {
                    val = _mm_shuffle_ps(this->values1, other.values1, _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 2, 0));
                }
                return SIMD12(this->values0, _mm_shuffle1_ps(val, _MM_SHUFFLE(3, 1, 2, 0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == Index1 && Index0 < 2) {
                return SIMD12(
                    _mm_blend_ps(this->values0, other.values0, 0x5 << (Index0 % 2)), this->values1, this->values2);
            } else if constexpr (Index0 == Index1 && Index0 < 4) {
                return SIMD12(
                    this->values0, _mm_blend_ps(this->values1, other.values1, 0x5 << (Index0 % 2)), this->values2);
            } else if constexpr (Index0 == Index1) {
                return SIMD12(
                    this->values0, this->values1, _mm_blend_ps(this->values2, other.values2, 0x5 << (Index0 % 2)));
            } else if constexpr (Index0 == 0) {
                const __m128 val = _mm_shuffle_ps(
                    this->values0, (&other.values0)[Index1 / 2], _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 3, 1));
                return SIMD12(_mm_shuffle1_ps(val, _MM_SHUFFLE(1, 3, 0, 2)), this->values1, this->values2);
            } else if constexpr (Index0 == 1) {
                const __m128 val = _mm_shuffle_ps(
                    this->values0, (&other.values0)[Index1 / 2], _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 2, 0));
                return SIMD12(_mm_shuffle1_ps(val, _MM_SHUFFLE(3, 1, 2, 0)), this->values1, this->values2);
            } else if constexpr (Index0 == 2) {
                const __m128 val = _mm_shuffle_ps(
                    this->values1, (&other.values0)[Index1 / 2], _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 3, 1));
                return SIMD12(this->values0, _mm_shuffle1_ps(val, _MM_SHUFFLE(1, 3, 0, 2)), this->values2);
            } else if constexpr (Index0 == 3) {
                const __m128 val = _mm_shuffle_ps(
                    this->values1, (&other.values0)[Index1 / 2], _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 2, 0));
                return SIMD12(this->values0, _mm_shuffle1_ps(val, _MM_SHUFFLE(3, 1, 2, 0)), this->values2);
            } else if constexpr (Index0 == 4) {
                const __m128 val = _mm_shuffle_ps(
                    this->values2, (&other.values0)[Index1 / 2], _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 3, 1));
                return SIMD12(this->values0, this->values1, _mm_shuffle1_ps(val, _MM_SHUFFLE(1, 3, 0, 2)));
            } else /*Index0 == 5*/ {
                const __m128 val = _mm_shuffle_ps(
                    this->values2, (&other.values0)[Index1 / 2], _MM_SHUFFLE((Index1 % 2) + 2, Index1 % 2, 2, 0));
                return SIMD12(this->values0, this->values1, _mm_shuffle1_ps(val, _MM_SHUFFLE(3, 1, 2, 0)));
            }
        } else
#endif
        {
            return SIMD12(Index0 != 0 ? this->values0 : (&other.values0)[Index1 + ((Index1 / 2) * 2)],
                Index0 != 1 ? this->values1 : (&other.values0)[Index1 + ((Index1 / 2) * 2)],
                Index0 != 0 ? this->values2 : (&other.values0)[Index1 + ((Index1 / 2) * 2) + 2],
                Index0 != 1 ? this->values3 : (&other.values0)[Index1 + ((Index1 / 2) * 2) + 2],
                Index0 != 2 ? this->values4 : (&other.values0)[Index1 + ((Index1 / 2) * 2)],
                Index0 != 3 ? this->values5 : (&other.values0)[Index1 + ((Index1 / 2) * 2)],
                Index0 != 2 ? this->values6 : (&other.values0)[Index1 + ((Index1 / 2) * 2) + 2],
                Index0 != 3 ? this->values7 : (&other.values0)[Index1 + ((Index1 / 2) * 2) + 2],
                Index0 != 4 ? this->values8 : (&other.values0)[Index1 + ((Index1 / 2) * 2)],
                Index0 != 5 ? this->values9 : (&other.values0)[Index1 + ((Index1 / 2) * 2)],
                Index0 != 4 ? this->values10 : (&other.values0)[Index1 + ((Index1 / 2) * 2) + 2],
                Index0 != 5 ? this->values11 : (&other.values0)[Index1 + ((Index1 / 2) * 2) + 2]);
        }
    }

    /**
     * Blends values from the second object into the first based on a mask. This performs the same operation on each
     * SIMD2.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1>
    XS_INLINE SIMD12 blend2(const SIMD12& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1) {
            return *this;
        } else if constexpr (Elem0 && Elem1) {
            return other;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD12(
                _mm512_mask_mov_ps(this->values, _cvtu32_mask16(((Elem1 << 1) | Elem0) * 0x555), other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD12(_mm256_blend_ps(this->values0, other.values0,
                              _MM256_BLEND(Elem1, Elem0, Elem1, Elem0, Elem1, Elem0, Elem1, Elem0)),
                _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD12(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)),
                _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)),
                _mm_blend_ps(this->values2, other.values2, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)));
        }
#endif
        else {
            return SIMD12(!Elem0 ? this->values0 : other.values0, !Elem1 ? this->values1 : other.values1,
                !Elem0 ? this->values2 : other.values2, !Elem1 ? this->values3 : other.values3,
                !Elem0 ? this->values4 : other.values4, !Elem1 ? this->values5 : other.values5,
                !Elem0 ? this->values6 : other.values6, !Elem1 ? this->values7 : other.values7,
                !Elem0 ? this->values8 : other.values8, !Elem1 ? this->values9 : other.values9,
                !Elem0 ? this->values10 : other.values10, !Elem1 ? this->values11 : other.values11);
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
    XS_INLINE SIMD12 blend3(const SIMD12& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2) {
            return *this;
        } else if constexpr (Elem0 && Elem1 && Elem2) {
            return other;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD12(_mm512_mask_mov_ps(
                this->values, _cvtu32_mask16((Elem0 * 0xF) | (Elem1 * 0xF0) | (Elem2 * 0xF00)), other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (!Elem0 && !Elem1 && Elem2) {
                return SIMD12(this->values0, other.values1);
            } else if constexpr (Elem0 && Elem1 && !Elem2) {
                return SIMD12(other.values0, this->values1);
            } else if constexpr (!Elem0 && !Elem1) {
                return SIMD12(
                    this->values0, _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem2, Elem2, Elem2, Elem2)));
            } else if constexpr (Elem0 && Elem1) {
                return SIMD12(
                    other.values0, _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem2, Elem2, Elem2, Elem2)));
            } else if constexpr (!Elem2) {
                return SIMD12(_mm256_blend_ps(this->values0, other.values0,
                                  _MM256_BLEND(Elem1, Elem1, Elem1, Elem1, Elem0, Elem0, Elem0, Elem0)),
                    this->values1);
            } else if constexpr (Elem2) {
                return SIMD12(_mm256_blend_ps(this->values0, other.values0,
                                  _MM256_BLEND(Elem1, Elem1, Elem1, Elem1, Elem0, Elem0, Elem0, Elem0)),
                    other.values1);
            } else {
                return SIMD12(_mm256_blend_ps(this->values0, other.values0,
                                  _MM256_BLEND(Elem1, Elem1, Elem1, Elem1, Elem0, Elem0, Elem0, Elem0)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem2, Elem2, Elem2, Elem2)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD12(!Elem0 ? this->values0 : other.values0, !Elem1 ? this->values1 : other.values1,
                !Elem2 ? this->values2 : other.values2);
        }
#endif
        else {
            return SIMD12(!Elem0 ? this->values0 : other.values0, !Elem0 ? this->values1 : other.values1,
                !Elem0 ? this->values2 : other.values2, !Elem0 ? this->values3 : other.values3,
                !Elem1 ? this->values4 : other.values4, !Elem1 ? this->values5 : other.values5,
                !Elem1 ? this->values6 : other.values6, !Elem1 ? this->values7 : other.values7,
                !Elem2 ? this->values8 : other.values8, !Elem2 ? this->values9 : other.values9,
                !Elem2 ? this->values10 : other.values10, !Elem2 ? this->values11 : other.values11);
        }
    }

    /**
     * Blends values from the second object into the first based on a mask. This performs the same operation on each
     * SIMD4.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @tparam Elem2 Whether to copy the third element of the first input into the returned object.
     * @tparam Elem3 Whether to copy the fourth element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3>
    XS_INLINE SIMD12 blend4(const SIMD12& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
            return *this;
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
            return other;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD12(_mm512_mask_mov_ps(this->values,
                _cvtu32_mask16((Elem0 * 0x111) | (Elem1 * 0x222) | (Elem2 * 0x444) | (Elem3 * 0x888)), other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD12(_mm256_blend_ps(this->values0, other.values0,
                              _MM256_BLEND(Elem3, Elem2, Elem1, Elem0, Elem3, Elem2, Elem1, Elem0)),
                _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
                return SIMD12(_mm_blend_ss(this->values0, other.values0), _mm_blend_ss(this->values1, other.values1),
                    _mm_blend_ss(this->values2, other.values2));
            } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
                return SIMD12(_mm_blend_ss(other.values0, this->values0), _mm_blend_ss(other.values1, this->values1),
                    _mm_blend_ss(other.values2, this->values2));
            } else {
                return SIMD12(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)),
                    _mm_blend_ps(this->values2, other.values2, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)));
            }
        }
#endif
        else {
            return SIMD12(!Elem0 ? this->values0 : other.values0, !Elem1 ? this->values1 : other.values1,
                !Elem2 ? this->values2 : other.values2, !Elem3 ? this->values3 : other.values3,
                !Elem0 ? this->values4 : other.values4, !Elem1 ? this->values5 : other.values5,
                !Elem2 ? this->values6 : other.values6, !Elem3 ? this->values7 : other.values7,
                !Elem0 ? this->values8 : other.values8, !Elem1 ? this->values9 : other.values9,
                !Elem2 ? this->values10 : other.values10, !Elem3 ? this->values11 : other.values11);
        }
    }

    /**
     * Blends values from the second object into the first based on a mask. This performs the same operation on each
     * SIMD6.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @tparam Elem2 Whether to copy the third element of the first input into the returned object.
     * @tparam Elem3 Whether to copy the fourth element of the first input into the returned object.
     * @tparam Elem4 Whether to copy the fifth element of the first input into the returned object.
     * @tparam Elem5 Whether to copy the sixth element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5>
    XS_INLINE SIMD12 blend6(const SIMD12& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5) {
            return *this;
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5) {
            return other;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD12(_mm512_mask_mov_ps(this->values,
                _cvtu32_mask16((Elem0 * 0x5) | (Elem1 * 0xA) | (Elem2 * 0x50) | (Elem3 * 0xA0) | (Elem4 * 0x500) |
                    (Elem5 * 0xA00)),
                other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && Elem5) {
                return SIMD12(this->values0, other.values1);
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
                return SIMD12(
                    this->values0, _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem5, Elem4, Elem5, Elem4)));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && !Elem4 && !Elem5) {
                return SIMD12(other.values0, this->values1);
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
                return SIMD12(
                    other.values0, _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem5, Elem4, Elem5, Elem4)));
            } else if constexpr (!Elem4 && !Elem5) {
                return SIMD12(_mm256_blend_ps(this->values0, other.values0,
                                  _MM256_BLEND(Elem3, Elem2, Elem3, Elem2, Elem1, Elem0, Elem1, Elem0)),
                    this->values1);
            } else if constexpr (Elem4 && Elem5) {
                return SIMD12(_mm256_blend_ps(this->values0, other.values0,
                                  _MM256_BLEND(Elem3, Elem2, Elem3, Elem2, Elem1, Elem0, Elem1, Elem0)),
                    other.values1);
            } else {
                return SIMD12(_mm256_blend_ps(this->values0, other.values0,
                                  _MM256_BLEND(Elem3, Elem2, Elem3, Elem2, Elem1, Elem0, Elem1, Elem0)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem5, Elem4, Elem5, Elem4)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 ret0, ret1, ret2;
            if constexpr (Elem0 && Elem1) {
                ret0 = other.values0;
            } else if constexpr (!Elem0 && !Elem1) {
                ret0 = this->values0;
            } else {
                ret0 = _mm_blend_ps(this->values0, other.values0, _MM_BLEND(Elem1, Elem0, Elem1, Elem0));
            }
            if constexpr (Elem2 && Elem3) {
                ret1 = other.values1;
            } else if constexpr (!Elem2 && !Elem3) {
                ret1 = this->values1;
            } else {
                ret1 = _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem3, Elem2, Elem3, Elem2));
            }
            if constexpr (Elem4 && Elem5) {
                ret2 = other.values2;
            } else if constexpr (!Elem4 && !Elem5) {
                ret2 = this->values2;
            } else {
                ret2 = _mm_blend_ps(this->values2, other.values2, _MM_BLEND(Elem5, Elem4, Elem5, Elem4));
            }
            return SIMD12(ret0, ret1, ret2);
        }
#endif
        else {
            return SIMD12(!Elem0 ? this->values0 : other.values0, !Elem1 ? this->values1 : other.values1,
                !Elem0 ? this->values2 : other.values2, !Elem1 ? this->values3 : other.values3,
                !Elem2 ? this->values4 : other.values4, !Elem3 ? this->values5 : other.values5,
                !Elem2 ? this->values6 : other.values6, !Elem3 ? this->values7 : other.values7,
                !Elem4 ? this->values8 : other.values8, !Elem5 ? this->values9 : other.values9,
                !Elem4 ? this->values10 : other.values10, !Elem5 ? this->values11 : other.values11);
        }
    }

    /**
     * Dynamically shuffles the elements of the object.
     * @param shuffle The shuffle mask used to shuffle.
     * @returns The result of the operation.
     */
    XS_INLINE SIMD12 shuffleVar(const Shuffle& shuffle) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD12(_mm512_permutevar_ps(this->values, shuffle.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            XS_ASSERT(_mm256_movemask_epi8(_mm256_cmpgt_epi32(shuffle.values0, _mm256_set1_epi32(3))) == 0 &&
                _mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values1, _mm_set1_epi32(3))) == 0);
            return SIMD12(_mm256_permutevar_ps(this->values0, shuffle.values0),
                _mm_permutevar_ps(this->values1, shuffle.values1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX>) {
                XS_ASSERT(_mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values0, _mm_set1_epi32(3))) == 0 &&
                    _mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values1, _mm_set1_epi32(3))) == 0 &&
                    _mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values2, _mm_set1_epi32(3))) == 0);
                return SIMD12(_mm_permutevar_ps(this->values0, shuffle.values0),
                    _mm_permutevar_ps(this->values1, shuffle.values1),
                    _mm_permutevar_ps(this->values2, shuffle.values2));
            } else {
                XS_ASSERT(_mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values0, _mm_set1_epi32(0x0F0E0D0C))) == 0 &&
                    _mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values1, _mm_set1_epi32(0x0F0E0D0C))) == 0 &&
                    _mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values2, _mm_set1_epi32(0x0F0E0D0C))) == 0);
                return SIMD12(_mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(this->values0), shuffle.values0)),
                    _mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(this->values1), shuffle.values1)),
                    _mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(this->values2), shuffle.values2)));
            }
        } else
#endif
        {
            XS_ASSERT(shuffle.values0 < 12 && shuffle.values1 < 12 && shuffle.values2 < 12 && shuffle.values3 < 12 &&
                shuffle.values4 < 12 && shuffle.values5 < 12 && shuffle.values6 < 12 && shuffle.values7 < 12 &&
                shuffle.values8 < 12 && shuffle.values9 < 12 && shuffle.values10 < 12 && shuffle.values11 < 12);
            return SIMD12((&this->values0)[shuffle.values0], (&this->values0)[shuffle.values1],
                (&this->values0)[shuffle.values2], (&this->values0)[shuffle.values3], (&this->values0)[shuffle.values4],
                (&this->values0)[shuffle.values5], (&this->values0)[shuffle.values6], (&this->values0)[shuffle.values7],
                (&this->values0)[shuffle.values8], (&this->values0)[shuffle.values9],
                (&this->values0)[shuffle.values10], (&this->values0)[shuffle.values11]);
        }
    }

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD2.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-1).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-1).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_INLINE SIMD12 shuffle2() const noexcept
    {
        static_assert(Index0 < 4 && Index1 < 4, "Invalid Index: Indexes must be <4");
        if constexpr (Index0 == 0 && Index1 == 1) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD12(_mm512_shuffle2200_ps(this->values));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD12(_mm512_shuffle3311_ps(this->values));
            } else {
                return SIMD12(_mm512_shuffle1_ps(this->values, _MM_SHUFFLE(Index1 + 2, Index0 + 2, Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD12(_mm256_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD12(_mm256_shuffle3311_ps(this->values0), _mm_shuffle3311_ps(this->values1));
            } else {
                return SIMD12(_mm256_shuffle1_ps(this->values0, _MM_SHUFFLE(Index1 + 2, Index0 + 2, Index1, Index0)),
                    _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(Index1 + 2, Index0 + 2, Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD12(_mm_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1),
                    _mm_shuffle2200_ps(this->values2));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD12(_mm_shuffle3311_ps(this->values0), _mm_shuffle3311_ps(this->values1),
                    _mm_shuffle3311_ps(this->values2));
            } else /*Index0 == 1 && Index1 == 0*/ {
                return SIMD12(_mm_shuffle1_ps(this->values0, _MM_SHUFFLE(2, 3, 0, 1)),
                    _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(2, 3, 0, 1)),
                    _mm_shuffle1_ps(this->values2, _MM_SHUFFLE(2, 3, 0, 1)));
            }
        }
#endif
        else {
            return SIMD12((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index0 + 2],
                (&this->values0)[Index1 + 2], (&this->values0)[Index0 + 4], (&this->values0)[Index1 + 4],
                (&this->values0)[Index0 + 6], (&this->values0)[Index1 + 6], (&this->values0)[Index0 + 8],
                (&this->values0)[Index1 + 8], (&this->values0)[Index0 + 10], (&this->values0)[Index1 + 10]);
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
    XS_INLINE SIMD12 shuffle3() const noexcept
    {
        static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3, "Invalid Index: Indexes must be <3");
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMD12(_mm512_shuffle_f32x4(this->values, this->values, _MM_SHUFFLE(0, Index2, Index1, Index0)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 0) {
                return SIMD12(_mm256_shuffle32103210_ps(this->values0), _mm256_castps256_ps128(this->values0));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
                return SIMD12(_mm256_shuffle32103210_ps(this->values0), _mm256_extractf128_ps(this->values0, 1));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
                return SIMD12(_mm256_shuffle32103210_ps(this->values0), this->values1);
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
                return SIMD12(this->values0, _mm256_castps256_ps128(this->values0));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 1) {
                return SIMD12(this->values0, _mm256_extractf128_ps(this->values0, 1));
            } else if constexpr (Index0 == 0 && Index1 == 2 && Index2 == 0) {
                return SIMD12(
                    _mm256_insertf128_ps(this->values0, this->values1, 1), _mm256_castps256_ps128(this->values0));
            } else if constexpr (Index0 == 0 && Index1 == 2 && Index2 == 1) {
                return SIMD12(
                    _mm256_insertf128_ps(this->values0, this->values1, 1), _mm256_extractf128_ps(this->values0, 1));
            } else if constexpr (Index0 == 0 && Index1 == 2 && Index2 == 2) {
                return SIMD12(_mm256_insertf128_ps(this->values0, this->values1, 1), this->values1);
            } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 0) {
                return SIMD12(_mm256_shuffle32107654_ps(this->values0), _mm256_castps256_ps128(this->values0));
            } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 1) {
                const __m256 val = _mm256_shuffle32107654_ps(this->values0);
                return SIMD12(val, _mm256_castps256_ps128(val));
            } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 2) {
                return SIMD12(_mm256_shuffle32107654_ps(this->values0), this->values1);
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 0) {
                return SIMD12(_mm256_shuffle76547654_ps(this->values0), _mm256_castps256_ps128(this->values0));
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 1) {
                const __m256 val = _mm256_shuffle76547654_ps(this->values0);
                return SIMD12(val, _mm256_castps256_ps128(val));
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 2) {
                return SIMD12(_mm256_shuffle76547654_ps(this->values0), this->values1);
            } else if constexpr (Index0 == 1 && Index1 == 2 && Index2 == 0) {
                return SIMD12(
                    _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(2, 1)),
                    _mm256_castps256_ps128(this->values0));
            } else if constexpr (Index0 == 1 && Index1 == 2 && Index2 == 1) {
                const __m256 val =
                    _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(2, 1));
                return SIMD12(val, _mm256_castps256_ps128(val));
            } else if constexpr (Index0 == 1 && Index1 == 2 && Index2 == 2) {
                return SIMD12(
                    _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(2, 1)),
                    this->values1);
            } else if constexpr (Index0 == 2 && Index1 == 0 && Index2 == 0) {
                return SIMD12(
                    _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(0, 2)),
                    _mm256_castps256_ps128(this->values0));
            } else if constexpr (Index0 == 2 && Index1 == 0 && Index2 == 1) {
                return SIMD12(
                    _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(0, 2)),
                    _mm256_extractf128_ps(this->values0, 1));
            } else if constexpr (Index0 == 2 && Index1 == 0 && Index2 == 2) {
                return SIMD12(
                    _mm256_permute2f128_ps(this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(0, 2)),
                    this->values1);
            } else if constexpr (Index0 == 2 && Index1 == 1 && Index2 == 0) {
                return SIMD12(
                    _mm256_insertf128_ps(this->values0, this->values1, 0), _mm256_castps256_ps128(this->values0));
            } else if constexpr (Index0 == 2 && Index1 == 1 && Index2 == 1) {
                return SIMD12(
                    _mm256_insertf128_ps(this->values0, this->values1, 0), _mm256_extractf128_ps(this->values0, 1));
            } else if constexpr (Index0 == 2 && Index1 == 1 && Index2 == 2) {
                return SIMD12(_mm256_insertf128_ps(this->values0, this->values1, 0), this->values1);
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 0) {
                return SIMD12(_mm256_broadcastf128_ps(this->values1), _mm256_castps256_ps128(this->values0));
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 1) {
                return SIMD12(_mm256_broadcastf128_ps(this->values1), _mm256_extractf128_ps(this->values0, 1));
            } else /*Index0 == 2 && Index1 == 2 && Index2 == 2*/ {
                return SIMD12(_mm256_broadcastf128_ps(this->values1), this->values1);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD12((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2]);
        }
#endif
        else {
            return SIMD12((&this->values0)[Index0 * 4], (&this->values0)[Index0 * 4 + 1],
                (&this->values0)[Index0 * 4 + 2], (&this->values0)[Index0 * 4 + 3], (&this->values0)[Index1 * 4],
                (&this->values0)[Index1 * 4 + 1], (&this->values0)[Index1 * 4 + 2], (&this->values0)[Index1 * 4 + 3],
                (&this->values0)[Index2 * 4], (&this->values0)[Index2 * 4 + 1], (&this->values0)[Index2 * 4 + 2],
                (&this->values0)[Index2 * 4 + 3]);
        }
    }

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD4.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-3).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-3).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-3).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-3).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
    XS_INLINE SIMD12 shuffle4() const noexcept
    {
        static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4, "Invalid Index: Indexes must be <4");
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
                return SIMD12<T, Width>(_mm512_shuffle2200_ps(this->values));
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3 && Index3 == 3) {
                return SIMD12<T, Width>(_mm512_shuffle3311_ps(this->values));
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3) {
                return SIMD12<T, Width>(_mm512_shuffle3322_ps(this->values));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
                return SIMD12<T, Width>(_mm512_shuffle1100_ps(this->values));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
                return SIMD12<T, Width>(_mm512_shuffle1010_ps(this->values));
            } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2 && Index3 == 3) {
                return SIMD12<T, Width>(_mm512_shuffle3232_ps(this->values));
            } else {
                return SIMD12<T, Width>(_mm512_shuffle1_ps(this->values, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3 && Index3 == 3) {
                return SIMD12(_mm256_shuffle3311_ps(this->values0), _mm_shuffle3311_ps(this->values1));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
                return SIMD12(_mm256_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
                return SIMD12(_mm256_shuffle1100_ps(this->values0), _mm_shuffle1100_ps(this->values1));
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3) {
                return SIMD12(_mm256_shuffle3322_ps(this->values0), _mm_shuffle3322_ps(this->values1));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
                return SIMD12(_mm256_shuffle1010_ps(this->values0), _mm_shuffle1010_ps(this->values1));
            } else {
                return SIMD12(_mm256_shuffle1_ps(this->values0, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                    _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
                return SIMD12(_mm_shuffle1010_ps(this->values0), _mm_shuffle1010_ps(this->values1),
                    _mm_shuffle1010_ps(this->values2));
            } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2 && Index3 == 3) {
                return SIMD12(_mm_shuffle3232_ps(this->values0), _mm_shuffle3232_ps(this->values1),
                    _mm_shuffle3232_ps(this->values2));
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3) {
                return SIMD12(_mm_shuffle3322_ps(this->values0), _mm_shuffle3322_ps(this->values1),
                    _mm_shuffle3322_ps(this->values2));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
                return SIMD12(_mm_shuffle1100_ps(this->values0), _mm_shuffle1100_ps(this->values1),
                    _mm_shuffle1100_ps(this->values2));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
                return SIMD12(_mm_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1),
                    _mm_shuffle2200_ps(this->values2));
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3 && Index3 == 3) {
                return SIMD12(_mm_shuffle3311_ps(this->values0), _mm_shuffle3311_ps(this->values1),
                    _mm_shuffle3311_ps(this->values2));
            } else {
                return SIMD12(_mm_shuffle1_ps(this->values0, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                    _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(Index3, Index2, Index1, Index0)),
                    _mm_shuffle1_ps(this->values2, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
            }
        }
#endif
        else {
            return SIMD12((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2],
                (&this->values0)[Index3], (&this->values0)[4 + Index0], (&this->values0)[4 + Index1],
                (&this->values0)[4 + Index2], (&this->values0)[4 + Index3], (&this->values0)[8 + Index0],
                (&this->values0)[8 + Index1], (&this->values0)[8 + Index2], (&this->values0)[8 + Index3]);
        }
    }

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD6.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-5).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-5).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-5).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-5).
     * @tparam Index4 The index of the fifth element to insert into the returned object (range is 0-5).
     * @tparam Index5 The index of the sixth element to insert into the returned object (range is 0-5).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3, uint32 Index4, uint32 Index5>
    XS_INLINE SIMD12 shuffle6() const noexcept
    {
        static_assert(Index0 < 6 && Index1 < 6 && Index2 < 6 && Index3 < 6 && Index4 < 6 && Index5 < 6,
            "Invalid Index: Indexes must be <6");
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3 && Index4 == 4 && Index5 == 5) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1 && Index4 == 0 && Index5 == 1) {
                return SIMD12(_mm512_broadcastf128_ps(_mm512_castps512_ps128(this->values)));
            } else if constexpr (Index0 % 2 == 0 && Index1 == (Index0 + 1) && Index2 % 2 == 0 &&
                Index3 == (Index2 + 1) && Index4 % 2 == 0 && Index5 == (Index4 + 1)) {
                return SIMD12(_mm512_shuffle_f32x4(
                    this->values, this->values, _MM_SHUFFLE(3, Index4 / 2, Index2 / 2, Index0 / 2)));
            } else {
                return SIMD12(_mm512_permutexvar_ps(
                    _mm512_set_epi32(0, 0, 0, 0, ((Index5 / 2) * 2) + Index5 + 2, ((Index4 / 2) * 2) + Index4 + 2,
                        ((Index5 / 2) * 2) + Index5, ((Index4 / 2) * 2) + Index4, ((Index3 / 2) * 2) + Index3 + 2,
                        ((Index2 / 2) * 2) + Index2 + 2, ((Index3 / 2) * 2) + Index3, ((Index2 / 2) * 2) + Index2,
                        ((Index1 / 2) * 2) + Index1 + 2, ((Index0 / 2) * 2) + Index0 + 2, ((Index1 / 2) * 2) + Index1,
                        ((Index0 / 2) * 2) + Index0),
                    this->values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1 && Index4 == 0 && Index5 == 1) {
                const __m256 val = _mm256_shuffle32103210_ps(this->values0);
                return SIMD12(val, _mm256_castps256_ps128(this->values0));
            } else {
                __m256 val0;
                if constexpr (((Index0 % 2) == (Index2 % 2)) && ((Index1 % 2) == (Index3 % 2))) {
                    // If both require the same sort values then we can do a single sort within the m256 directly
                    if constexpr (Index0 < 2) {
                        if constexpr (Index2 < 2) {
                            val0 = _mm256_shuffle32103210_ps(this->values0);
                        } else if constexpr (Index2 < 4) {
                            val0 = this->values0;
                        } else {
                            val0 = _mm256_insertf128_ps(this->values0, this->values1, 1);
                        }
                    } else if constexpr (Index0 < 4) {
                        if constexpr (Index2 < 2) {
                            val0 = _mm256_shuffle32107654_ps(this->values0);
                        } else if constexpr (Index2 < 4) {
                            val0 = _mm256_shuffle76547654_ps(this->values0);
                        } else {
                            val0 = _mm256_permute2f128_ps(
                                this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(2, 1));
                        }
                    } else {
                        if constexpr (Index2 < 2) {
                            val0 = _mm256_permute2f128_ps(
                                this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(0, 2));
                        } else if constexpr (Index2 < 4) {
                            val0 = _mm256_insertf128_ps(this->values0, this->values1, 0);
                        } else {
                            val0 = _mm256_broadcastf128_ps(this->values1);
                        }
                    }
                    __m256 val1;
                    if constexpr (Index1 < 2) {
                        if constexpr (Index3 < 2) {
                            val1 = _mm256_shuffle32103210_ps(this->values0);
                        } else if constexpr (Index3 < 4) {
                            val1 = this->values0;
                        } else {
                            val1 = _mm256_insertf128_ps(this->values0, this->values1, 1);
                        }
                    } else if constexpr (Index1 < 4) {
                        if constexpr (Index3 < 2) {
                            val1 = _mm256_shuffle32107654_ps(this->values0);
                        } else if constexpr (Index3 < 4) {
                            val1 = _mm256_shuffle76547654_ps(this->values0);
                        } else {
                            val1 = _mm256_permute2f128_ps(
                                this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(2, 1));
                        }
                    } else {
                        if constexpr (Index3 < 2) {
                            val1 = _mm256_permute2f128_ps(
                                this->values0, _mm256_castps128_ps256(this->values1), _MM256_PERMUTE(0, 2));
                        } else if constexpr (Index3 < 4) {
                            val1 = _mm256_insertf128_ps(this->values0, this->values1, 0);
                        } else {
                            val1 = _mm256_broadcastf128_ps(this->values1);
                        }
                    }

                    if constexpr (Index0 % 2) {
                        if constexpr (Index1 % 2) {
                            val0 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 1, 3, 1));
                        } else {
                            val0 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(2, 0, 3, 1));
                        }
                    } else {
                        if constexpr (Index1 % 2) {
                            val0 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 1, 2, 0));
                        } else {
                            val0 = _mm256_shuffle_ps(val0, val1, _MM_SHUFFLE(2, 0, 2, 0));
                        }
                    }
                    val0 = _mm256_shuffle1_ps(val0, _MM_SHUFFLE(3, 1, 2, 0));
                } else {
                    __m128 val2;
                    if constexpr (Index0 < 2) {
                        val2 = _mm256_castps256_ps128(this->values0);
                    } else if constexpr (Index0 < 4) {
                        val2 = _mm256_extractf128_ps(this->values0, 1);
                    } else {
                        val2 = this->values1;
                    }
                    __m128 val3;
                    if constexpr (Index1 < 2) {
                        val3 = _mm256_castps256_ps128(this->values0);
                    } else if constexpr (Index1 < 4) {
                        val3 = _mm256_extractf128_ps(this->values0, 1);
                    } else {
                        val3 = this->values1;
                    }

                    if constexpr (Index0 % 2) {
                        if constexpr (Index1 % 2) {
                            val2 = _mm_shuffle_ps(val2, val3, _MM_SHUFFLE(3, 1, 3, 1));
                        } else {
                            val2 = _mm_shuffle_ps(val2, val3, _MM_SHUFFLE(2, 0, 3, 1));
                        }
                    } else {
                        if constexpr (Index1 % 2) {
                            val2 = _mm_shuffle_ps(val2, val3, _MM_SHUFFLE(3, 1, 2, 0));
                        } else {
                            val2 = _mm_shuffle_ps(val2, val3, _MM_SHUFFLE(2, 0, 2, 0));
                        }
                    }
                    val2 = _mm_shuffle1_ps(val2, _MM_SHUFFLE(3, 1, 2, 0));
                    __m128 val4;
                    if constexpr (Index2 < 2) {
                        val4 = _mm256_castps256_ps128(this->values0);
                    } else if constexpr (Index2 < 4) {
                        val4 = _mm256_extractf128_ps(this->values0, 1);
                    } else {
                        val4 = this->values1;
                    }
                    __m128 val5;
                    if constexpr (Index3 < 2) {
                        val5 = _mm256_castps256_ps128(this->values0);
                    } else if constexpr (Index3 < 4) {
                        val5 = _mm256_extractf128_ps(this->values0, 1);
                    } else {
                        val5 = this->values1;
                    }
                    if constexpr (Index2 % 2) {
                        if constexpr (Index3 % 2) {
                            val4 = _mm_shuffle_ps(val4, val5, _MM_SHUFFLE(3, 1, 3, 1));
                        } else {
                            val4 = _mm_shuffle_ps(val4, val5, _MM_SHUFFLE(2, 0, 3, 1));
                        }
                    } else {
                        if constexpr (Index3 % 2) {
                            val4 = _mm_shuffle_ps(val4, val5, _MM_SHUFFLE(3, 1, 2, 0));
                        } else {
                            val4 = _mm_shuffle_ps(val4, val5, _MM_SHUFFLE(2, 0, 2, 0));
                        }
                    }
                    val4 = _mm_shuffle1_ps(val4, _MM_SHUFFLE(3, 1, 2, 0));
                    val0 = _mm256_set_m128(val4, val2);
                }
                __m128 val6;
                if constexpr (Index4 < 2) {
                    val6 = _mm256_castps256_ps128(this->values0);
                } else if constexpr (Index4 < 4) {
                    val6 = _mm256_extractf128_ps(this->values0, 1);
                } else {
                    val6 = this->values1;
                }
                __m128 val7;
                if constexpr (Index5 < 2) {
                    val7 = _mm256_castps256_ps128(this->values0);
                } else if constexpr (Index5 < 4) {
                    val7 = _mm256_extractf128_ps(this->values0, 1);
                } else {
                    val7 = this->values1;
                }
                if constexpr (Index4 % 2) {
                    if constexpr (Index5 % 2) {
                        val6 = _mm_shuffle_ps(val6, val7, _MM_SHUFFLE(3, 1, 3, 1));
                    } else {
                        val6 = _mm_shuffle_ps(val6, val7, _MM_SHUFFLE(2, 0, 3, 1));
                    }
                } else {
                    if constexpr (Index5 % 2) {
                        val6 = _mm_shuffle_ps(val6, val7, _MM_SHUFFLE(3, 1, 2, 0));
                    } else {
                        val6 = _mm_shuffle_ps(val6, val7, _MM_SHUFFLE(2, 0, 2, 0));
                    }
                }
                val6 = _mm_shuffle1_ps(val6, _MM_SHUFFLE(3, 1, 2, 0));
                return SIMD12(val0, val6);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val2 = (&this->values0)[Index0 / 2];
            const __m128 val3 = (&this->values0)[Index1 / 2];
            if constexpr (Index0 % 2) {
                if constexpr (Index1 % 2) {
                    val2 = _mm_shuffle_ps(val2, val3, _MM_SHUFFLE(3, 1, 3, 1));
                } else {
                    val2 = _mm_shuffle_ps(val2, val3, _MM_SHUFFLE(2, 0, 3, 1));
                }
            } else {
                if constexpr (Index1 % 2) {
                    val2 = _mm_shuffle_ps(val2, val3, _MM_SHUFFLE(3, 1, 2, 0));
                } else {
                    val2 = _mm_shuffle_ps(val2, val3, _MM_SHUFFLE(2, 0, 2, 0));
                }
            }
            val2 = _mm_shuffle1_ps(val2, _MM_SHUFFLE(3, 1, 2, 0));

            __m128 val4 = (&this->values0)[Index2 / 2];
            const __m128 val5 = (&this->values0)[Index3 / 2];
            if constexpr (Index2 % 2) {
                if constexpr (Index3 % 2) {
                    val4 = _mm_shuffle_ps(val4, val5, _MM_SHUFFLE(3, 1, 3, 1));
                } else {
                    val4 = _mm_shuffle_ps(val4, val5, _MM_SHUFFLE(2, 0, 3, 1));
                }
            } else {
                if constexpr (Index3 % 2) {
                    val4 = _mm_shuffle_ps(val4, val5, _MM_SHUFFLE(3, 1, 2, 0));
                } else {
                    val4 = _mm_shuffle_ps(val4, val5, _MM_SHUFFLE(2, 0, 2, 0));
                }
            }
            val4 = _mm_shuffle1_ps(val4, _MM_SHUFFLE(3, 1, 2, 0));

            __m128 val6 = (&this->values0)[Index4 / 2];
            const __m128 val7 = (&this->values0)[Index5 / 2];
            if constexpr (Index4 % 2) {
                if constexpr (Index5 % 2) {
                    val6 = _mm_shuffle_ps(val6, val7, _MM_SHUFFLE(3, 1, 3, 1));
                } else {
                    val6 = _mm_shuffle_ps(val6, val7, _MM_SHUFFLE(2, 0, 3, 1));
                }
            } else {
                if constexpr (Index5 % 2) {
                    val6 = _mm_shuffle_ps(val6, val7, _MM_SHUFFLE(3, 1, 2, 0));
                } else {
                    val6 = _mm_shuffle_ps(val6, val7, _MM_SHUFFLE(2, 0, 2, 0));
                }
            }
            val6 = _mm_shuffle1_ps(val6, _MM_SHUFFLE(3, 1, 2, 0));
            return SIMD12(val2, val4, val6);
        }
#endif
        else {
            return SIMD12((&this->values0)[((Index0 / 2) * 2) + Index0], (&this->values0)[((Index1 / 2) * 2) + Index1],
                (&this->values0)[((Index0 / 2) * 2) + Index0 + 2], (&this->values0)[((Index1 / 2) * 2) + Index1 + 2],
                (&this->values0)[((Index2 / 2) * 2) + Index2], (&this->values0)[((Index3 / 2) * 2) + Index3],
                (&this->values0)[((Index2 / 2) * 2) + Index2 + 2], (&this->values0)[((Index3 / 2) * 2) + Index3 + 2],
                (&this->values0)[((Index4 / 2) * 2) + Index4], (&this->values0)[((Index5 / 2) * 2) + Index5],
                (&this->values0)[((Index4 / 2) * 2) + Index4 + 2], (&this->values0)[((Index5 / 2) * 2) + Index5 + 2]);
        }
    }

    /**
     * Shuffles the elements of the object by shuffling each SIMD4.
     * @tparam Index0 The index of the first SIMD4 to insert into the returned object (range is 0-2).
     * @tparam Index1 The index of the second SIMD4 to insert into the returned object (range is 0-2).
     * @tparam Index2 The index of the third SIMD4 to insert into the returned object (range is 0-2).
     * @returns Result of operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2>
    XS_INLINE SIMD12 shuffleH4() const noexcept
    {
        return shuffle3<Index0, Index1, Index2>();
    }
};

/**
 * Add two SIMD12s.
 * @param other1 The first SIMD12.
 * @param other2 SIMD12 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator+(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(
            _mm256_add_ps(other1.values0, other2.values0), _mm_add_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_add_ps(other1.values0, other2.values0), _mm_add_ps(other1.values1, other2.values1),
            _mm_add_ps(other1.values2, other2.values2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values3, other1.values4 + other2.values4,
            other1.values5 + other2.values5, other1.values6 + other2.values6, other1.values7 + other2.values7,
            other1.values8 + other2.values8, other1.values9 + other2.values9, other1.values10 + other2.values10,
            other1.values11 + other2.values11);
    }
}

/**
 * Add a value to all elements of a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Value to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator+(const SIMD12<T, Width>& other1, typename SIMD12<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_add_ps(other1.values0, other2.values),
            _mm_add_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values),
            _mm_add_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 + other2.value, other1.values1 + other2.value,
            other1.values2 + other2.value, other1.values3 + other2.value, other1.values4 + other2.value,
            other1.values5 + other2.value, other1.values6 + other2.value, other1.values7 + other2.value,
            other1.values8 + other2.value, other1.values9 + other2.value, other1.values10 + other2.value,
            other1.values11 + other2.value);
    }
}

/**
 * Add a SIMD2 to each pair of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator+(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_add_ps(other1.values, _mm512_broadcastf64_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            const __m256 val = _mm256_broadcastf64_ps(other2.values);
            return SIMD12<T, Width>(
                _mm256_add_ps(other1.values0, val), _mm_add_ps(other1.values1, _mm256_castps256_ps128(val)));
        } else {
            const __m128 val0 = _mm_shuffle1010_ps(other2.values);
            return SIMD12<T, Width>(
                _mm256_add_ps(other1.values0, _mm256_broadcastf128_ps(val0)), _mm_add_ps(other1.values1, val0));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        return SIMD12<T, Width>(
            _mm_add_ps(other1.values0, val), _mm_add_ps(other1.values1, val), _mm_add_ps(other1.values2, val));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values0, other1.values3 + other2.values1, other1.values4 + other2.values0,
            other1.values5 + other2.values1, other1.values6 + other2.values0, other1.values7 + other2.values1,
            other1.values8 + other2.values0, other1.values9 + other2.values1, other1.values10 + other2.values0,
            other1.values11 + other2.values1);
    }
}

/**
 * Add a SIMD3 to each triple of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator+(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle1111_ps(other2.values),
                                                  _mm_shuffle0000_ps(other2.values))),
            _mm_shuffle2222_ps(other2.values), 2);
        return SIMD12<T, Width>(_mm512_add_ps(other1.values, val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        return SIMD12<T, Width>(
            _mm256_add_ps(other1.values0, val0), _mm_add_ps(other1.values1, _mm_shuffle2222_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_add_ps(other1.values0, _mm_shuffle0000_ps(other2.values)),
            _mm_add_ps(other1.values1, _mm_shuffle1111_ps(other2.values)),
            _mm_add_ps(other1.values2, _mm_shuffle2222_ps(other2.values)));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values0,
            other1.values2 + other2.values0, other1.values3 + other2.values0, other1.values4 + other2.values1,
            other1.values5 + other2.values1, other1.values6 + other2.values1, other1.values7 + other2.values1,
            other1.values8 + other2.values2, other1.values9 + other2.values2, other1.values10 + other2.values2,
            other1.values11 + other2.values2);
    }
}

/**
 * Add a SIMD4 to each quadruple of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator+(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_add_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_add_ps(other1.values0, _mm256_broadcastf128_ps(other2.values)),
            _mm_add_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values),
            _mm_add_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values3, other1.values4 + other2.values0,
            other1.values5 + other2.values1, other1.values6 + other2.values2, other1.values7 + other2.values3,
            other1.values8 + other2.values0, other1.values9 + other2.values1, other1.values10 + other2.values2,
            other1.values11 + other2.values3);
    }
}

/**
 * Add a SIMD6 to each sextuple of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator+(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        __m512 val2 =
            _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values))), val);
        val2 = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(1, 1, 2, 0));
        return SIMD12<T, Width>(_mm512_add_ps(other1.values, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        const __m256 val0 = _mm256_insertf128_ps(val, _mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values)), 1);
        const __m128 val1 = _mm256_extractf128_ps(val, 1);
        return SIMD12<T, Width>(_mm256_add_ps(other1.values0, val0), _mm_add_ps(other1.values1, val1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(other2.values0, other2.values0);
        const __m128 val1 = _mm_movehl_ps(other2.values0, other2.values0);
        const __m128 val2 = _mm_movelh_ps(other2.values1, other2.values1);
        return SIMD12<T, Width>(
            _mm_add_ps(other1.values0, val0), _mm_add_ps(other1.values1, val1), _mm_add_ps(other1.values2, val2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values0, other1.values3 + other2.values1, other1.values4 + other2.values2,
            other1.values5 + other2.values3, other1.values6 + other2.values2, other1.values7 + other2.values3,
            other1.values8 + other2.values4, other1.values9 + other2.values5, other1.values10 + other2.values4,
            other1.values11 + other2.values5);
    }
}

/**
 * Subtract a SIMD12 from another SIMD12.
 * @param other1 The first SIMD12.
 * @param other2 SIMD12 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator-(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(
            _mm256_sub_ps(other1.values0, other2.values0), _mm_sub_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_sub_ps(other1.values0, other2.values0), _mm_sub_ps(other1.values1, other2.values1),
            _mm_sub_ps(other1.values2, other2.values2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values3, other1.values4 - other2.values4,
            other1.values5 - other2.values5, other1.values6 - other2.values6, other1.values7 - other2.values7,
            other1.values8 - other2.values8, other1.values9 - other2.values9, other1.values10 - other2.values10,
            other1.values11 - other2.values11);
    }
}

/**
 * Subtract a value from all elements of a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Value to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator-(const SIMD12<T, Width>& other1, typename SIMD12<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_sub_ps(other1.values0, other2.values),
            _mm_sub_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values),
            _mm_sub_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 - other2.value, other1.values1 - other2.value,
            other1.values2 - other2.value, other1.values3 - other2.value, other1.values4 - other2.value,
            other1.values5 - other2.value, other1.values6 - other2.value, other1.values7 - other2.value,
            other1.values8 - other2.value, other1.values9 - other2.value, other1.values10 - other2.value,
            other1.values11 - other2.value);
    }
}

/**
 * Subtract all elements of a SIMD12 from a value.
 * @param other1 Value to subtract from the SIMD12.
 * @param other2 The SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator-(typename SIMD12<T, Width>::BaseDef other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_sub_ps(other1.values, other2.values0),
            _mm_sub_ps(_mm256_castps256_ps128(other1.values), other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_sub_ps(other1.values, other2.values0), _mm_sub_ps(other1.values, other2.values1),
            _mm_sub_ps(other1.values, other2.values2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.value - other2.values0, other1.value - other2.values1,
            other1.value - other2.values2, other1.value - other2.values3, other1.value - other2.values4,
            other1.value - other2.values5, other1.value - other2.values6, other1.value - other2.values7,
            other1.value - other2.values8, other1.value - other2.values9, other1.value - other2.values10,
            other1.value - other2.values11);
    }
}

/**
 * Subtract a SIMD2 from each pair of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator-(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_sub_ps(other1.values, _mm512_broadcastf64_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            const __m256 val = _mm256_broadcastf64_ps(other2.values);
            return SIMD12<T, Width>(
                _mm256_sub_ps(other1.values0, val), _mm_sub_ps(other1.values1, _mm256_castps256_ps128(val)));
        } else {
            const __m128 val0 = _mm_shuffle1010_ps(other2.values);
            return SIMD12<T, Width>(
                _mm256_sub_ps(other1.values0, _mm256_broadcastf128_ps(val0)), _mm_sub_ps(other1.values1, val0));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        return SIMD12<T, Width>(
            _mm_sub_ps(other1.values0, val), _mm_sub_ps(other1.values1, val), _mm_sub_ps(other1.values2, val));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values0, other1.values3 - other2.values1, other1.values4 - other2.values0,
            other1.values5 - other2.values1, other1.values6 - other2.values0, other1.values7 - other2.values1,
            other1.values8 - other2.values0, other1.values9 - other2.values1, other1.values10 - other2.values0,
            other1.values11 - other2.values1);
    }
}

/**
 * Subtract a SIMD3 from each triple of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator-(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle1111_ps(other2.values),
                                                  _mm_shuffle0000_ps(other2.values))),
            _mm_shuffle2222_ps(other2.values), 2);
        return SIMD12<T, Width>(_mm512_sub_ps(other1.values, val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        return SIMD12<T, Width>(
            _mm256_sub_ps(other1.values0, val0), _mm_sub_ps(other1.values1, _mm_shuffle2222_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_sub_ps(other1.values0, _mm_shuffle0000_ps(other2.values)),
            _mm_sub_ps(other1.values1, _mm_shuffle1111_ps(other2.values)),
            _mm_sub_ps(other1.values2, _mm_shuffle2222_ps(other2.values)));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values0,
            other1.values2 - other2.values0, other1.values3 - other2.values0, other1.values4 - other2.values1,
            other1.values5 - other2.values1, other1.values6 - other2.values1, other1.values7 - other2.values1,
            other1.values8 - other2.values2, other1.values9 - other2.values2, other1.values10 - other2.values2,
            other1.values11 - other2.values2);
    }
}

/**
 * Subtract a SIMD4 from each quadruple of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator-(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_sub_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_sub_ps(other1.values0, _mm256_broadcastf128_ps(other2.values)),
            _mm_sub_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values),
            _mm_sub_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values3, other1.values4 - other2.values0,
            other1.values5 - other2.values1, other1.values6 - other2.values2, other1.values7 - other2.values3,
            other1.values8 - other2.values0, other1.values9 - other2.values1, other1.values10 - other2.values2,
            other1.values11 - other2.values3);
    }
}

/**
 * Subtract a SIMD6 from each sextuple of elements in a SIMD12.
 * @param other1 The SIMD12.
 * @param other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator-(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        __m512 val2 =
            _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values))), val);
        val2 = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(1, 1, 2, 0));
        return SIMD12<T, Width>(_mm512_sub_ps(other1.values, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        const __m256 val0 = _mm256_insertf128_ps(val, _mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values)), 1);
        const __m128 val1 = _mm256_extractf128_ps(val, 1);
        return SIMD12<T, Width>(_mm256_sub_ps(other1.values0, val0), _mm_sub_ps(other1.values1, val1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(other2.values0, other2.values0);
        const __m128 val1 = _mm_movehl_ps(other2.values0, other2.values0);
        const __m128 val2 = _mm_movelh_ps(other2.values1, other2.values1);
        return SIMD12<T, Width>(
            _mm_sub_ps(other1.values0, val0), _mm_sub_ps(other1.values1, val1), _mm_sub_ps(other1.values2, val2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values0, other1.values3 - other2.values1, other1.values4 - other2.values2,
            other1.values5 - other2.values3, other1.values6 - other2.values2, other1.values7 - other2.values3,
            other1.values8 - other2.values4, other1.values9 - other2.values5, other1.values10 - other2.values4,
            other1.values11 - other2.values5);
    }
}

/**
 * Multiply two SIMD12s per element.
 * @param other1 The first SIMD12.
 * @param other2 The second SIMD12 to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator*(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(
            _mm256_mul_ps(other1.values0, other2.values0), _mm_mul_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_mul_ps(other1.values0, other2.values0), _mm_mul_ps(other1.values1, other2.values1),
            _mm_mul_ps(other1.values2, other2.values2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values3, other1.values4 * other2.values4,
            other1.values5 * other2.values5, other1.values6 * other2.values6, other1.values7 * other2.values7,
            other1.values8 * other2.values8, other1.values9 * other2.values9, other1.values10 * other2.values10,
            other1.values11 * other2.values11);
    }
}

/**
 * Multiply a SIMD12 by a scalar.
 * @param other1 The SIMD12.
 * @param other2 Value to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator*(const SIMD12<T, Width>& other1, typename SIMD12<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_mul_ps(other1.values0, other2.values),
            _mm_mul_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values),
            _mm_mul_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 * other2.value, other1.values1 * other2.value,
            other1.values2 * other2.value, other1.values3 * other2.value, other1.values4 * other2.value,
            other1.values5 * other2.value, other1.values6 * other2.value, other1.values7 * other2.value,
            other1.values8 * other2.value, other1.values9 * other2.value, other1.values10 * other2.value,
            other1.values11 * other2.value);
    }
}

/**
 * Multiply each pair of elements in a SIMD12 by a SIMD2.
 * @param other1 The SIMD12.
 * @param other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator*(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_mul_ps(other1.values, _mm512_broadcastf64_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            const __m256 val = _mm256_broadcastf64_ps(other2.values);
            return SIMD12<T, Width>(
                _mm256_mul_ps(other1.values0, val), _mm_mul_ps(other1.values1, _mm256_castps256_ps128(val)));
        } else {
            const __m128 val0 = _mm_shuffle1010_ps(other2.values);
            return SIMD12<T, Width>(
                _mm256_mul_ps(other1.values0, _mm256_broadcastf128_ps(val0)), _mm_mul_ps(other1.values1, val0));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        return SIMD12<T, Width>(
            _mm_mul_ps(other1.values0, val), _mm_mul_ps(other1.values1, val), _mm_mul_ps(other1.values2, val));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values0, other1.values3 * other2.values1, other1.values4 * other2.values0,
            other1.values5 * other2.values1, other1.values6 * other2.values0, other1.values7 * other2.values1,
            other1.values8 * other2.values0, other1.values9 * other2.values1, other1.values10 * other2.values0,
            other1.values11 * other2.values1);
    }
}

/**
 * Multiply each triple of elements in a SIMD12 by a SIMD3.
 * @param other1 The SIMD12.
 * @param other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator*(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle1111_ps(other2.values),
                                                  _mm_shuffle0000_ps(other2.values))),
            _mm_shuffle2222_ps(other2.values), 2);
        return SIMD12<T, Width>(_mm512_mul_ps(other1.values, val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        return SIMD12<T, Width>(
            _mm256_mul_ps(other1.values0, val0), _mm_mul_ps(other1.values1, _mm_shuffle2222_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_mul_ps(other1.values0, _mm_shuffle0000_ps(other2.values)),
            _mm_mul_ps(other1.values1, _mm_shuffle1111_ps(other2.values)),
            _mm_mul_ps(other1.values2, _mm_shuffle2222_ps(other2.values)));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values0,
            other1.values2 * other2.values0, other1.values3 * other2.values0, other1.values4 * other2.values1,
            other1.values5 * other2.values1, other1.values6 * other2.values1, other1.values7 * other2.values1,
            other1.values8 * other2.values2, other1.values9 * other2.values2, other1.values10 * other2.values2,
            other1.values11 * other2.values2);
    }
}

/**
 * Multiply each quadruple of elements in a SIMD12 by a SIMD4.
 * @param other1 The SIMD12.
 * @param other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator*(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_mul_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_mul_ps(other1.values0, _mm256_broadcastf128_ps(other2.values)),
            _mm_mul_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values),
            _mm_mul_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values3, other1.values4 * other2.values0,
            other1.values5 * other2.values1, other1.values6 * other2.values2, other1.values7 * other2.values3,
            other1.values8 * other2.values0, other1.values9 * other2.values1, other1.values10 * other2.values2,
            other1.values11 * other2.values3);
    }
}

/**
 * Multiply each sextuple of elements in a SIMD12 by a SIMD6.
 * @param other1 The SIMD12.
 * @param other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator*(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        __m512 val2 =
            _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values))), val);
        val2 = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(1, 1, 2, 0));
        return SIMD12<T, Width>(_mm512_mul_ps(other1.values, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        const __m256 val0 = _mm256_insertf128_ps(val, _mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values)), 1);
        const __m128 val1 = _mm256_extractf128_ps(val, 1);
        return SIMD12<T, Width>(_mm256_mul_ps(other1.values0, val0), _mm_mul_ps(other1.values1, val1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(other2.values0, other2.values0);
        const __m128 val1 = _mm_movehl_ps(other2.values0, other2.values0);
        const __m128 val2 = _mm_movelh_ps(other2.values1, other2.values1);
        return SIMD12<T, Width>(
            _mm_mul_ps(other1.values0, val0), _mm_mul_ps(other1.values1, val1), _mm_mul_ps(other1.values2, val2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values0, other1.values3 * other2.values1, other1.values4 * other2.values2,
            other1.values5 * other2.values3, other1.values6 * other2.values2, other1.values7 * other2.values3,
            other1.values8 * other2.values4, other1.values9 * other2.values5, other1.values10 * other2.values4,
            other1.values11 * other2.values5);
    }
}

/**
 * Divide two SIMD12s per element.
 * @param other1 The first SIMD12.
 * @param other2 The second SIMD12 to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator/(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(
            _mm256_div_ps(other1.values0, other2.values0), _mm_div_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_div_ps(other1.values0, other2.values0), _mm_div_ps(other1.values1, other2.values1),
            _mm_div_ps(other1.values2, other2.values2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values3, other1.values4 / other2.values4,
            other1.values5 / other2.values5, other1.values6 / other2.values6, other1.values7 / other2.values7,
            other1.values8 / other2.values8, other1.values9 / other2.values9, other1.values10 / other2.values10,
            other1.values11 / other2.values11);
    }
}

/**
 * Divide a SIMD12 by a scalar.
 * @param other1 The SIMD12.
 * @param other2 Value to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator/(const SIMD12<T, Width>& other1, typename SIMD12<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_div_ps(other1.values0, other2.values),
            _mm_div_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values),
            _mm_div_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 / other2.value, other1.values1 / other2.value,
            other1.values2 / other2.value, other1.values3 / other2.value, other1.values4 / other2.value,
            other1.values5 / other2.value, other1.values6 / other2.value, other1.values7 / other2.value,
            other1.values8 / other2.value, other1.values9 / other2.value, other1.values10 / other2.value,
            other1.values11 / other2.value);
    }
}

/**
 * Divide a scalar by all elements of a SIMD12.
 * @param other1 Value to divide the SIMD12 by.
 * @param other2 The SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator/(typename SIMD12<T, Width>::BaseDef other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_div_ps(other1.values, other2.values0),
            _mm_div_ps(_mm256_castps256_ps128(other1.values), other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_div_ps(other1.values, other2.values0), _mm_div_ps(other1.values, other2.values1),
            _mm_div_ps(other1.values, other2.values2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.value / other2.values0, other1.value / other2.values1,
            other1.value / other2.values2, other1.value / other2.values3, other1.value / other2.values4,
            other1.value / other2.values5, other1.value / other2.values6, other1.value / other2.values7,
            other1.value / other2.values8, other1.value / other2.values9, other1.value / other2.values10,
            other1.value / other2.values11);
    }
}

/**
 * Divide each pair of elements in a SIMD12 by a SIMD2.
 * @param other1 The SIMD12.
 * @param other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator/(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_div_ps(other1.values, _mm512_broadcastf64_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            const __m256 val = _mm256_broadcastf64_ps(other2.values);
            return SIMD12<T, Width>(
                _mm256_div_ps(other1.values0, val), _mm_div_ps(other1.values1, _mm256_castps256_ps128(val)));
        } else {
            const __m128 val0 = _mm_shuffle1010_ps(other2.values);
            return SIMD12<T, Width>(
                _mm256_div_ps(other1.values0, _mm256_broadcastf128_ps(val0)), _mm_div_ps(other1.values1, val0));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        return SIMD12<T, Width>(
            _mm_div_ps(other1.values0, val), _mm_div_ps(other1.values1, val), _mm_div_ps(other1.values2, val));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values0, other1.values3 / other2.values1, other1.values4 / other2.values0,
            other1.values5 / other2.values1, other1.values6 / other2.values0, other1.values7 / other2.values1,
            other1.values8 / other2.values0, other1.values9 / other2.values1, other1.values10 / other2.values0,
            other1.values11 / other2.values1);
    }
}

/**
 * Divide each triple of elements in a SIMD12 by a SIMD3.
 * @param other1 The SIMD12.
 * @param other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator/(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle1111_ps(other2.values),
                                                  _mm_shuffle0000_ps(other2.values))),
            _mm_shuffle2222_ps(other2.values), 2);
        return SIMD12<T, Width>(_mm512_div_ps(other1.values, val));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        return SIMD12<T, Width>(
            _mm256_div_ps(other1.values0, val0), _mm_div_ps(other1.values1, _mm_shuffle2222_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_div_ps(other1.values0, _mm_shuffle0000_ps(other2.values)),
            _mm_div_ps(other1.values1, _mm_shuffle1111_ps(other2.values)),
            _mm_div_ps(other1.values2, _mm_shuffle2222_ps(other2.values)));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values0,
            other1.values2 / other2.values0, other1.values3 / other2.values0, other1.values4 / other2.values1,
            other1.values5 / other2.values1, other1.values6 / other2.values1, other1.values7 / other2.values1,
            other1.values8 / other2.values2, other1.values9 / other2.values2, other1.values10 / other2.values2,
            other1.values11 / other2.values2);
    }
}

/**
 * Divide each quadruple of elements in a SIMD12 by a SIMD4.
 * @param other1 The SIMD12.
 * @param other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator/(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_div_ps(other1.values, _mm512_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_div_ps(other1.values0, _mm256_broadcastf128_ps(other2.values)),
            _mm_div_ps(other1.values1, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values),
            _mm_div_ps(other1.values2, other2.values));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values3, other1.values4 / other2.values0,
            other1.values5 / other2.values1, other1.values6 / other2.values2, other1.values7 / other2.values3,
            other1.values8 / other2.values0, other1.values9 / other2.values1, other1.values10 / other2.values2,
            other1.values11 / other2.values3);
    }
}

/**
 * Divide each sextuple of elements in a SIMD12 by a SIMD6.
 * @param other1 The SIMD12.
 * @param other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator/(
    const SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        __m512 val2 =
            _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values))), val);
        val2 = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(1, 1, 2, 0));
        return SIMD12<T, Width>(_mm512_div_ps(other1.values, val2));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        const __m256 val0 = _mm256_insertf128_ps(val, _mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values)), 1);
        const __m128 val1 = _mm256_extractf128_ps(val, 1);
        return SIMD12<T, Width>(_mm256_div_ps(other1.values0, val0), _mm_div_ps(other1.values1, val1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(other2.values0, other2.values0);
        const __m128 val1 = _mm_movehl_ps(other2.values0, other2.values0);
        const __m128 val2 = _mm_movelh_ps(other2.values1, other2.values1);
        return SIMD12<T, Width>(
            _mm_div_ps(other1.values0, val0), _mm_div_ps(other1.values1, val1), _mm_div_ps(other1.values2, val2));
    } else
#endif
    {
        return SIMD12<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values0, other1.values3 / other2.values1, other1.values4 / other2.values2,
            other1.values5 / other2.values3, other1.values6 / other2.values2, other1.values7 / other2.values3,
            other1.values8 / other2.values4, other1.values9 / other2.values5, other1.values10 / other2.values4,
            other1.values11 / other2.values5);
    }
}

/**
 * Negate all elements of a SIMD12.
 * @param other The SIMD12 to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator-(const SIMD12<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_sub_ps(_mm512_setzero_ps(), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_setzero_ps();
        return SIMD12<T, Width>(
            _mm256_sub_ps(val, other.values0), _mm_sub_ps(_mm256_castps256_ps128(val), other.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_setzero_ps();
        return SIMD12<T, Width>(
            _mm_sub_ps(val, other.values0), _mm_sub_ps(val, other.values1), _mm_sub_ps(val, other.values2));
    } else
#endif
    {
        return SIMD12<T, Width>(-other.values0, -other.values1, -other.values2, -other.values3, -other.values4,
            -other.values5, -other.values6, -other.values7, -other.values8, -other.values9, -other.values10,
            -other.values11);
    }
}

/**
 * Perform compound assignment and addition with a SIMD12.
 * @param [in,out] other1 The first SIMD12.
 * @param          other2 SIMD12 to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator+=(SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
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
        other1.values9 += other2.values9;
        other1.values10 += other2.values10;
        other1.values11 += other2.values11;
    }
    return other1;
}

/**
 * Perform compound assignment and addition with a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Value to add to the first SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator+=(SIMD12<T, Width>& other1, typename SIMD12<T, Width>::BaseDef other2) noexcept
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
        other1.values9 += other2.value;
        other1.values10 += other2.value;
        other1.values11 += other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and addition of a SIMD2 to each pair of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator+=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_add_ps(other1.values, _mm512_broadcastf64_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            const __m256 val = _mm256_broadcastf64_ps(other2.values);
            other1.values0 = _mm256_add_ps(other1.values0, val);
            other1.values1 = _mm_add_ps(other1.values1, _mm256_castps256_ps128(val));
        } else {
            const __m128 val0 = _mm_shuffle1010_ps(other2.values);
            other1.values0 = _mm256_add_ps(other1.values0, _mm256_broadcastf128_ps(val0));
            other1.values1 = _mm_add_ps(other1.values1, val0);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        other1.values0 = _mm_add_ps(other1.values0, val);
        other1.values1 = _mm_add_ps(other1.values1, val);
        other1.values2 = _mm_add_ps(other1.values2, val);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
        other1.values2 += other2.values0;
        other1.values3 += other2.values1;
        other1.values4 += other2.values0;
        other1.values5 += other2.values1;
        other1.values6 += other2.values0;
        other1.values7 += other2.values1;
        other1.values8 += other2.values0;
        other1.values9 += other2.values1;
        other1.values10 += other2.values0;
        other1.values11 += other2.values1;
    }
    return other1;
}

/**
 * Perform compound assignment and addition of a SIMD3 to each triple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator+=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle1111_ps(other2.values),
                                                  _mm_shuffle0000_ps(other2.values))),
            _mm_shuffle2222_ps(other2.values), 2);
        other1.values = _mm512_add_ps(other1.values, val);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        other1.values0 = _mm256_add_ps(other1.values0, val0);
        other1.values1 = _mm_add_ps(other1.values1, _mm_shuffle2222_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, _mm_shuffle0000_ps(other2.values));
        other1.values1 = _mm_add_ps(other1.values1, _mm_shuffle1111_ps(other2.values));
        other1.values2 = _mm_add_ps(other1.values2, _mm_shuffle2222_ps(other2.values));
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values0;
        other1.values2 += other2.values0;
        other1.values3 += other2.values0;
        other1.values4 += other2.values1;
        other1.values5 += other2.values1;
        other1.values6 += other2.values1;
        other1.values7 += other2.values1;
        other1.values8 += other2.values2;
        other1.values9 += other2.values2;
        other1.values10 += other2.values2;
        other1.values11 += other2.values2;
    }
    return other1;
}

/**
 * Perform compound assignment and addition of a SIMD4 to each quadruple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator+=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept
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
        other1.values3 += other2.values3;
        other1.values4 += other2.values0;
        other1.values5 += other2.values1;
        other1.values6 += other2.values2;
        other1.values7 += other2.values3;
        other1.values8 += other2.values0;
        other1.values9 += other2.values1;
        other1.values10 += other2.values2;
        other1.values11 += other2.values3;
    }
    return other1;
}

/**
 * Perform compound assignment and addition of a SIMD6 to each sextuple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to add to the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator+=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        __m512 val2 =
            _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values))), val);
        val2 = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(1, 1, 2, 0));
        other1.values = _mm512_add_ps(other1.values, val2);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        const __m256 val0 = _mm256_insertf128_ps(val, _mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values)), 1);
        const __m128 val1 = _mm256_extractf128_ps(val, 1);
        other1.values0 = _mm256_add_ps(other1.values0, val0);
        other1.values1 = _mm_add_ps(other1.values1, val1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(other2.values0, other2.values0);
        const __m128 val1 = _mm_movehl_ps(other2.values0, other2.values0);
        const __m128 val2 = _mm_movelh_ps(other2.values1, other2.values1);
        other1.values0 = _mm_add_ps(other1.values0, val0);
        other1.values1 = _mm_add_ps(other1.values1, val1);
        other1.values2 = _mm_add_ps(other1.values2, val2);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
        other1.values2 += other2.values0;
        other1.values3 += other2.values1;
        other1.values4 += other2.values2;
        other1.values5 += other2.values3;
        other1.values6 += other2.values2;
        other1.values7 += other2.values3;
        other1.values8 += other2.values4;
        other1.values9 += other2.values5;
        other1.values10 += other2.values4;
        other1.values11 += other2.values5;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction by a SIMD12.
 * @param [in,out] other1 The first SIMD12.
 * @param          other2 SIMD12 to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator-=(SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
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
        other1.values9 -= other2.values9;
        other1.values10 -= other2.values10;
        other1.values11 -= other2.values11;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction with a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Value to subtract from the first SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator-=(SIMD12<T, Width>& other1, typename SIMD12<T, Width>::BaseDef other2) noexcept
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
        other1.values9 -= other2.value;
        other1.values10 -= other2.value;
        other1.values11 -= other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction of a SIMD2 to each pair of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator-=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_sub_ps(other1.values, _mm512_broadcastf64_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            const __m256 val = _mm256_broadcastf64_ps(other2.values);
            other1.values0 = _mm256_sub_ps(other1.values0, val);
            other1.values1 = _mm_sub_ps(other1.values1, _mm256_castps256_ps128(val));
        } else {
            const __m128 val0 = _mm_shuffle1010_ps(other2.values);
            other1.values0 = _mm256_sub_ps(other1.values0, _mm256_broadcastf128_ps(val0));
            other1.values1 = _mm_sub_ps(other1.values1, val0);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        other1.values0 = _mm_sub_ps(other1.values0, val);
        other1.values1 = _mm_sub_ps(other1.values1, val);
        other1.values2 = _mm_sub_ps(other1.values2, val);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
        other1.values2 -= other2.values0;
        other1.values3 -= other2.values1;
        other1.values4 -= other2.values0;
        other1.values5 -= other2.values1;
        other1.values6 -= other2.values0;
        other1.values7 -= other2.values1;
        other1.values8 -= other2.values0;
        other1.values9 -= other2.values1;
        other1.values10 -= other2.values0;
        other1.values11 -= other2.values1;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction of a SIMD3 to each triple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator-=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle1111_ps(other2.values),
                                                  _mm_shuffle0000_ps(other2.values))),
            _mm_shuffle2222_ps(other2.values), 2);
        other1.values = _mm512_sub_ps(other1.values, val);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        other1.values0 = _mm256_sub_ps(other1.values0, val0);
        other1.values1 = _mm_sub_ps(other1.values1, _mm_shuffle2222_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, _mm_shuffle0000_ps(other2.values));
        other1.values1 = _mm_sub_ps(other1.values1, _mm_shuffle1111_ps(other2.values));
        other1.values2 = _mm_sub_ps(other1.values2, _mm_shuffle2222_ps(other2.values));
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values0;
        other1.values2 -= other2.values0;
        other1.values3 -= other2.values0;
        other1.values4 -= other2.values1;
        other1.values5 -= other2.values1;
        other1.values6 -= other2.values1;
        other1.values7 -= other2.values1;
        other1.values8 -= other2.values2;
        other1.values9 -= other2.values2;
        other1.values10 -= other2.values2;
        other1.values11 -= other2.values2;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction of a SIMD4 to each quadruple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator-=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept
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
        other1.values3 -= other2.values3;
        other1.values4 -= other2.values0;
        other1.values5 -= other2.values1;
        other1.values6 -= other2.values2;
        other1.values7 -= other2.values3;
        other1.values8 -= other2.values0;
        other1.values9 -= other2.values1;
        other1.values10 -= other2.values2;
        other1.values11 -= other2.values3;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction of a SIMD6 to each sextuple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to subtract from the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator-=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        __m512 val2 =
            _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values))), val);
        val2 = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(1, 1, 2, 0));
        other1.values = _mm512_sub_ps(other1.values, val2);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        const __m256 val0 = _mm256_insertf128_ps(val, _mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values)), 1);
        const __m128 val1 = _mm256_extractf128_ps(val, 1);
        other1.values0 = _mm256_sub_ps(other1.values0, val0);
        other1.values1 = _mm_sub_ps(other1.values1, val1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(other2.values0, other2.values0);
        const __m128 val1 = _mm_movehl_ps(other2.values0, other2.values0);
        const __m128 val2 = _mm_movelh_ps(other2.values1, other2.values1);
        other1.values0 = _mm_sub_ps(other1.values0, val0);
        other1.values1 = _mm_sub_ps(other1.values1, val1);
        other1.values2 = _mm_sub_ps(other1.values2, val2);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
        other1.values2 -= other2.values0;
        other1.values3 -= other2.values1;
        other1.values4 -= other2.values2;
        other1.values5 -= other2.values3;
        other1.values6 -= other2.values2;
        other1.values7 -= other2.values3;
        other1.values8 -= other2.values4;
        other1.values9 -= other2.values5;
        other1.values10 -= other2.values4;
        other1.values11 -= other2.values5;
    }
    return other1;
}

/**
 * Perform SIMD12 compound assignment and multiplication.
 * @param [in,out] other1 The first SIMD12.
 * @param          other2 SIMD12 to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator*=(SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
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
        other1.values9 *= other2.values9;
        other1.values10 *= other2.values10;
        other1.values11 *= other2.values11;
    }
    return other1;
}

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Value to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator*=(SIMD12<T, Width>& other1, typename SIMD12<T, Width>::BaseDef other2) noexcept
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
        other1.values9 *= other2.value;
        other1.values10 *= other2.value;
        other1.values11 *= other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and multiplication of a SIMD2 to each pair of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator*=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_mul_ps(other1.values, _mm512_broadcastf64_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            const __m256 val = _mm256_broadcastf64_ps(other2.values);
            other1.values0 = _mm256_mul_ps(other1.values0, val);
            other1.values1 = _mm_mul_ps(other1.values1, _mm256_castps256_ps128(val));
        } else {
            const __m128 val0 = _mm_shuffle1010_ps(other2.values);
            other1.values0 = _mm256_mul_ps(other1.values0, _mm256_broadcastf128_ps(val0));
            other1.values1 = _mm_mul_ps(other1.values1, val0);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        other1.values0 = _mm_mul_ps(other1.values0, val);
        other1.values1 = _mm_mul_ps(other1.values1, val);
        other1.values2 = _mm_mul_ps(other1.values2, val);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
        other1.values2 *= other2.values0;
        other1.values3 *= other2.values1;
        other1.values4 *= other2.values0;
        other1.values5 *= other2.values1;
        other1.values6 *= other2.values0;
        other1.values7 *= other2.values1;
        other1.values8 *= other2.values0;
        other1.values9 *= other2.values1;
        other1.values10 *= other2.values0;
        other1.values11 *= other2.values1;
    }
    return other1;
}

/**
 * Perform compound assignment and multiplication of a SIMD3 to each triple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator*=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle1111_ps(other2.values),
                                                  _mm_shuffle0000_ps(other2.values))),
            _mm_shuffle2222_ps(other2.values), 2);
        other1.values = _mm512_mul_ps(other1.values, val);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        other1.values0 = _mm256_mul_ps(other1.values0, val0);
        other1.values1 = _mm_mul_ps(other1.values1, _mm_shuffle2222_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, _mm_shuffle0000_ps(other2.values));
        other1.values1 = _mm_mul_ps(other1.values1, _mm_shuffle1111_ps(other2.values));
        other1.values2 = _mm_mul_ps(other1.values2, _mm_shuffle2222_ps(other2.values));
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values0;
        other1.values2 *= other2.values0;
        other1.values3 *= other2.values0;
        other1.values4 *= other2.values1;
        other1.values5 *= other2.values1;
        other1.values6 *= other2.values1;
        other1.values7 *= other2.values1;
        other1.values8 *= other2.values2;
        other1.values9 *= other2.values2;
        other1.values10 *= other2.values2;
        other1.values11 *= other2.values2;
    }
    return other1;
}

/**
 * Perform compound assignment and multiplication of a SIMD4 to each quadruple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator*=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept
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
        other1.values3 *= other2.values3;
        other1.values4 *= other2.values0;
        other1.values5 *= other2.values1;
        other1.values6 *= other2.values2;
        other1.values7 *= other2.values3;
        other1.values8 *= other2.values0;
        other1.values9 *= other2.values1;
        other1.values10 *= other2.values2;
        other1.values11 *= other2.values3;
    }
    return other1;
}

/**
 * Perform compound assignment and multiplication of a SIMD6 to each sextuple of elements in a SIMD12.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to multiply the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator*=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        __m512 val2 =
            _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values))), val);
        val2 = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(1, 1, 2, 0));
        other1.values = _mm512_mul_ps(other1.values, val2);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        const __m256 val0 = _mm256_insertf128_ps(val, _mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values)), 1);
        const __m128 val1 = _mm256_extractf128_ps(val, 1);
        other1.values0 = _mm256_mul_ps(other1.values0, val0);
        other1.values1 = _mm_mul_ps(other1.values1, val1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(other2.values0, other2.values0);
        const __m128 val1 = _mm_movehl_ps(other2.values0, other2.values0);
        const __m128 val2 = _mm_movelh_ps(other2.values1, other2.values1);
        other1.values0 = _mm_mul_ps(other1.values0, val0);
        other1.values1 = _mm_mul_ps(other1.values1, val1);
        other1.values2 = _mm_mul_ps(other1.values2, val2);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
        other1.values2 *= other2.values0;
        other1.values3 *= other2.values1;
        other1.values4 *= other2.values2;
        other1.values5 *= other2.values3;
        other1.values6 *= other2.values2;
        other1.values7 *= other2.values3;
        other1.values8 *= other2.values4;
        other1.values9 *= other2.values5;
        other1.values10 *= other2.values4;
        other1.values11 *= other2.values5;
    }
    return other1;
}

/**
 * Perform compound assignment and division.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Value to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator/=(SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
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
        other1.values9 /= other2.values9;
        other1.values10 /= other2.values10;
        other1.values11 /= other2.values11;
    }
    return other1;
}

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Value to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator/=(SIMD12<T, Width>& other1, typename SIMD12<T, Width>::BaseDef other2) noexcept
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
        other1.values9 /= other2.value;
        other1.values10 /= other2.value;
        other1.values11 /= other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and division of each pair of elements in a SIMD12 by a SIMD2.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator/=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_div_ps(other1.values, _mm512_broadcastf64_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            const __m256 val = _mm256_broadcastf64_ps(other2.values);
            other1.values0 = _mm256_div_ps(other1.values0, val);
            other1.values1 = _mm_div_ps(other1.values1, _mm256_castps256_ps128(val));
        } else {
            const __m128 val0 = _mm_shuffle1010_ps(other2.values);
            other1.values0 = _mm256_div_ps(other1.values0, _mm256_broadcastf128_ps(val0));
            other1.values1 = _mm_div_ps(other1.values1, val0);
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        other1.values0 = _mm_div_ps(other1.values0, val);
        other1.values1 = _mm_div_ps(other1.values1, val);
        other1.values2 = _mm_div_ps(other1.values2, val);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
        other1.values2 /= other2.values0;
        other1.values3 /= other2.values1;
        other1.values4 /= other2.values0;
        other1.values5 /= other2.values1;
        other1.values6 /= other2.values0;
        other1.values7 /= other2.values1;
        other1.values8 /= other2.values0;
        other1.values9 /= other2.values1;
        other1.values10 /= other2.values0;
        other1.values11 /= other2.values1;
    }
    return other1;
}

/**
 * Perform compound assignment and division of each triple of elements in a SIMD12 by a SIMD3.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator/=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m512 val = _mm512_insertf32x4(_mm512_castps256_ps512(_mm256_set_m128(_mm_shuffle1111_ps(other2.values),
                                                  _mm_shuffle0000_ps(other2.values))),
            _mm_shuffle2222_ps(other2.values), 2);
        other1.values = _mm512_div_ps(other1.values, val);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val0 = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        other1.values0 = _mm256_div_ps(other1.values0, val0);
        other1.values1 = _mm_div_ps(other1.values1, _mm_shuffle2222_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, _mm_shuffle0000_ps(other2.values));
        other1.values1 = _mm_div_ps(other1.values1, _mm_shuffle1111_ps(other2.values));
        other1.values2 = _mm_div_ps(other1.values2, _mm_shuffle2222_ps(other2.values));
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values0;
        other1.values2 /= other2.values0;
        other1.values3 /= other2.values0;
        other1.values4 /= other2.values1;
        other1.values5 /= other2.values1;
        other1.values6 /= other2.values1;
        other1.values7 /= other2.values1;
        other1.values8 /= other2.values2;
        other1.values9 /= other2.values2;
        other1.values10 /= other2.values2;
        other1.values11 /= other2.values2;
    }
    return other1;
}

/**
 * Perform compound assignment and division of each quadruple of elements in a SIMD12 by a SIMD4.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator/=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD4Def& other2) noexcept
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
        other1.values3 /= other2.values3;
        other1.values4 /= other2.values0;
        other1.values5 /= other2.values1;
        other1.values6 /= other2.values2;
        other1.values7 /= other2.values3;
        other1.values8 /= other2.values0;
        other1.values9 /= other2.values1;
        other1.values10 /= other2.values2;
        other1.values11 /= other2.values3;
    }
    return other1;
}

/**
 * Perform compound assignment and division of each sextuple of elements in a SIMD12 by a SIMD6.
 * @param [in,out] other1 The SIMD12.
 * @param          other2 Values to divide the SIMD12 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width>& operator/=(
    SIMD12<T, Width>& other1, const typename SIMD12<T, Width>::SIMD6Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        __m512 val2 =
            _mm512_set_m256(_mm256_castps128_ps256(_mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values))), val);
        val2 = _mm512_shuffle_f32x4(val2, val2, _MM_SHUFFLE(1, 1, 2, 0));
        other1.values = _mm512_div_ps(other1.values, val2);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_shuffle1010_ps(other2.values);
        const __m256 val0 = _mm256_insertf128_ps(val, _mm_shuffle3232_ps(_mm256_castps256_ps128(other2.values)), 1);
        const __m128 val1 = _mm256_extractf128_ps(val, 1);
        other1.values0 = _mm256_div_ps(other1.values0, val0);
        other1.values1 = _mm_div_ps(other1.values1, val1);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_movelh_ps(other2.values0, other2.values0);
        const __m128 val1 = _mm_movehl_ps(other2.values0, other2.values0);
        const __m128 val2 = _mm_movelh_ps(other2.values1, other2.values1);
        other1.values0 = _mm_div_ps(other1.values0, val0);
        other1.values1 = _mm_div_ps(other1.values1, val1);
        other1.values2 = _mm_div_ps(other1.values2, val2);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
        other1.values2 /= other2.values0;
        other1.values3 /= other2.values1;
        other1.values4 /= other2.values2;
        other1.values5 /= other2.values3;
        other1.values6 /= other2.values2;
        other1.values7 /= other2.values3;
        other1.values8 /= other2.values4;
        other1.values9 /= other2.values5;
        other1.values10 /= other2.values4;
        other1.values11 /= other2.values5;
    }
    return other1;
}

/**
 * And 2 SIMD12s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD12.
 * @param other2 SIMD12 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator&(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(
            _mm256_and_ps(other1.values0, other2.values0), _mm_and_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_and_ps(other1.values0, other2.values0), _mm_and_ps(other1.values1, other2.values1),
            _mm_and_ps(other1.values2, other2.values2));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(other1.values0 & other2.values0, other1.values1 & other2.values1,
                other1.values2 & other2.values2, other1.values3 & other2.values3, other1.values4 & other2.values4,
                other1.values5 & other2.values5, other1.values6 & other2.values6, other1.values7 & other2.values7,
                other1.values8 & other2.values8, other1.values9 & other2.values9, other1.values10 & other2.values10,
                other1.values11 & other2.values11);
        } else {
            return SIMD12<T, Width>(Shift::bitAnd(other1.values0, other2.values0),
                Shift::bitAnd(other1.values1, other2.values1), Shift::bitAnd(other1.values2, other2.values2),
                Shift::bitAnd(other1.values3, other2.values3), Shift::bitAnd(other1.values4, other2.values4),
                Shift::bitAnd(other1.values5, other2.values5), Shift::bitAnd(other1.values6, other2.values6),
                Shift::bitAnd(other1.values7, other2.values7), Shift::bitAnd(other1.values8, other2.values8),
                Shift::bitAnd(other1.values9, other2.values9), Shift::bitAnd(other1.values10, other2.values10),
                Shift::bitAnd(other1.values11, other2.values11));
        }
    }
}

/**
 * And a SIMD12s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD12.
 * @param other2 Value to and with the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator&(const SIMD12<T, Width>& other1, typename SIMD12<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_and_ps(other1.values0, other2.values),
            _mm_and_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_and_ps(other1.values0, other2.values), _mm_and_ps(other1.values1, other2.values),
            _mm_and_ps(other1.values2, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(other1.values0 & other2.value, other1.values1 & other2.value,
                other1.values2 & other2.value, other1.values3 & other2.value, other1.values4 & other2.value,
                other1.values5 & other2.value, other1.values6 & other2.value, other1.values7 & other2.value,
                other1.values8 & other2.value, other1.values9 & other2.value, other1.values10 & other2.value,
                other1.values11 & other2.value);
        } else {
            return SIMD12<T, Width>(Shift::bitAnd(other1.values0, other2.value),
                Shift::bitAnd(other1.values1, other2.value), Shift::bitAnd(other1.values2, other2.value),
                Shift::bitAnd(other1.values3, other2.value), Shift::bitAnd(other1.values4, other2.value),
                Shift::bitAnd(other1.values5, other2.value), Shift::bitAnd(other1.values6, other2.value),
                Shift::bitAnd(other1.values7, other2.value), Shift::bitAnd(other1.values8, other2.value),
                Shift::bitAnd(other1.values9, other2.value), Shift::bitAnd(other1.values10, other2.value),
                Shift::bitAnd(other1.values11, other2.value));
        }
    }
}

/**
 * Or 2 SIMD12s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD12.
 * @param other2 SIMD12 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator|(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(
            _mm256_or_ps(other1.values0, other2.values0), _mm_or_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_or_ps(other1.values0, other2.values0), _mm_or_ps(other1.values1, other2.values1),
            _mm_or_ps(other1.values2, other2.values2));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(other1.values0 | other2.values0, other1.values1 | other2.values1,
                other1.values2 | other2.values2, other1.values3 | other2.values3, other1.values4 | other2.values4,
                other1.values5 | other2.values5, other1.values6 | other2.values6, other1.values7 | other2.values7,
                other1.values8 | other2.values8, other1.values9 | other2.values9, other1.values10 | other2.values10,
                other1.values11 | other2.values11);
        } else {
            return SIMD12<T, Width>(Shift::bitOr(other1.values0, other2.values0),
                Shift::bitOr(other1.values1, other2.values1), Shift::bitOr(other1.values2, other2.values2),
                Shift::bitOr(other1.values3, other2.values3), Shift::bitOr(other1.values4, other2.values4),
                Shift::bitOr(other1.values5, other2.values5), Shift::bitOr(other1.values6, other2.values6),
                Shift::bitOr(other1.values7, other2.values7), Shift::bitOr(other1.values8, other2.values8),
                Shift::bitOr(other1.values9, other2.values9), Shift::bitOr(other1.values10, other2.values10),
                Shift::bitOr(other1.values11, other2.values11));
        }
    }
}

/**
 * Or a SIMD12s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD12.
 * @param other2 Value to and with the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator|(const SIMD12<T, Width>& other1, typename SIMD12<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_or_ps(other1.values0, other2.values),
            _mm_or_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_or_ps(other1.values0, other2.values), _mm_or_ps(other1.values1, other2.values),
            _mm_or_ps(other1.values2, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(other1.values0 | other2.value, other1.values1 | other2.value,
                other1.values2 | other2.value, other1.values3 | other2.value, other1.values4 | other2.value,
                other1.values5 | other2.value, other1.values6 | other2.value, other1.values7 | other2.value,
                other1.values8 | other2.value, other1.values9 | other2.value, other1.values10 | other2.value,
                other1.values11 | other2.value);
        } else {
            return SIMD12<T, Width>(Shift::bitOr(other1.values0, other2.value),
                Shift::bitOr(other1.values1, other2.value), Shift::bitOr(other1.values2, other2.value),
                Shift::bitOr(other1.values3, other2.value), Shift::bitOr(other1.values4, other2.value),
                Shift::bitOr(other1.values5, other2.value), Shift::bitOr(other1.values6, other2.value),
                Shift::bitOr(other1.values7, other2.value), Shift::bitOr(other1.values8, other2.value),
                Shift::bitOr(other1.values9, other2.value), Shift::bitOr(other1.values10, other2.value),
                Shift::bitOr(other1.values11, other2.value));
        }
    }
}

/**
 * Xor 2 SIMD12s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD12.
 * @param other2 SIMD12 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator^(const SIMD12<T, Width>& other1, const SIMD12<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(
            _mm256_xor_ps(other1.values0, other2.values0), _mm_xor_ps(other1.values1, other2.values1));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_xor_ps(other1.values0, other2.values0), _mm_xor_ps(other1.values1, other2.values1),
            _mm_xor_ps(other1.values2, other2.values2));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(other1.values0 ^ other2.values0, other1.values1 ^ other2.values1,
                other1.values2 ^ other2.values2, other1.values3 ^ other2.values3, other1.values4 ^ other2.values4,
                other1.values5 ^ other2.values5, other1.values6 ^ other2.values6, other1.values7 ^ other2.values7,
                other1.values8 ^ other2.values8, other1.values9 ^ other2.values9, other1.values10 ^ other2.values10,
                other1.values11 ^ other2.values11);
        } else {
            return SIMD12<T, Width>(Shift::bitXor(other1.values0, other2.values0),
                Shift::bitXor(other1.values1, other2.values1), Shift::bitXor(other1.values2, other2.values2),
                Shift::bitXor(other1.values3, other2.values3), Shift::bitXor(other1.values4, other2.values4),
                Shift::bitXor(other1.values5, other2.values5), Shift::bitXor(other1.values6, other2.values6),
                Shift::bitXor(other1.values7, other2.values7), Shift::bitXor(other1.values8, other2.values8),
                Shift::bitXor(other1.values9, other2.values9), Shift::bitXor(other1.values10, other2.values10),
                Shift::bitXor(other1.values11, other2.values11));
        }
    }
}

/**
 * Xor a SIMD12s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD12.
 * @param other2 Value to and with the SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator^(const SIMD12<T, Width>& other1, typename SIMD12<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD12<T, Width>(_mm256_xor_ps(other1.values0, other2.values),
            _mm_xor_ps(other1.values1, _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD12<T, Width>(_mm_xor_ps(other1.values0, other2.values), _mm_xor_ps(other1.values1, other2.values),
            _mm_xor_ps(other1.values2, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(other1.values0 ^ other2.value, other1.values1 ^ other2.value,
                other1.values2 ^ other2.value, other1.values3 ^ other2.value, other1.values4 ^ other2.value,
                other1.values5 ^ other2.value, other1.values6 ^ other2.value, other1.values7 ^ other2.value,
                other1.values8 ^ other2.value, other1.values9 ^ other2.value, other1.values10 ^ other2.value,
                other1.values11 ^ other2.value);
        } else {
            return SIMD12<T, Width>(Shift::bitXor(other1.values0, other2.value),
                Shift::bitXor(other1.values1, other2.value), Shift::bitXor(other1.values2, other2.value),
                Shift::bitXor(other1.values3, other2.value), Shift::bitXor(other1.values4, other2.value),
                Shift::bitXor(other1.values5, other2.value), Shift::bitXor(other1.values6, other2.value),
                Shift::bitXor(other1.values7, other2.value), Shift::bitXor(other1.values8, other2.value),
                Shift::bitXor(other1.values9, other2.value), Shift::bitXor(other1.values10, other2.value),
                Shift::bitXor(other1.values11, other2.value));
        }
    }
}

/**
 * Not a SIMD12.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other The SIMD12.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD12<T, Width> operator~(const SIMD12<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMD12<T, Width>(_mm512_xor_ps(other.values, _mm512_castsi512_ps(_mm512_set1_epi32(-1))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        __m256 val = _mm256_undefined_ps();
        val = _mm256_cmp_ps(val, val, _CMP_EQ_UQ);
        return SIMD8(_mm256_xor_ps(other.values0, val), _mm_xor_ps(other.values1, _mm256_castps256_ps128(val)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_undefined_ps();
        val = _mm_cmpeq_ps(val, val);
        return SIMD12<T, Width>(
            _mm_xor_ps(other.values0, val), _mm_xor_ps(other.values1, val), _mm_xor_ps(other.values2, val));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD12<T, Width>(~other.values0, ~other.values1, ~other.values2, ~other.values3, ~other.values4,
                ~other.values5, ~other.values6, ~other.values7, ~other.values8, ~other.values9, ~other.values10,
                ~other.values11);
        } else {
            return SIMD12<T, Width>(Shift::bitNot(other.values0), Shift::bitNot(other.values1),
                Shift::bitNot(other.values2), Shift::bitNot(other.values3), Shift::bitNot(other.values4),
                Shift::bitNot(other.values5), Shift::bitNot(other.values6), Shift::bitNot(other.values7),
                Shift::bitNot(other.values8), Shift::bitNot(other.values9), Shift::bitNot(other.values10),
                Shift::bitNot(other.values11));
        }
    }
}
} // namespace Shift
