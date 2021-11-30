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

#include "SIMD/XSSIMD6.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class SIMD3x2;

template<typename T>
class alignas(maxAlignment<T, 2>) SIMD3x2Data
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    T value0, value1, value2, value3;
    T value4, value5;

    /** Default constructor. */
    XS_INLINE SIMD3x2Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit SIMD3x2Data(const SIMD3x2<T, Width>& other) noexcept
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
     */
    XS_INLINE void setData(T other0, T other1, T other2, T other3, T other4, T other5) noexcept
    {
        value0 = other0;
        value1 = other1;
        value2 = other2;
        value3 = other3;
        value4 = other4;
        value5 = other5;
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const SIMD3x2<T, Width>& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                _mm256_mask_compressstoreu_ps(&value0, _cvtu32_mask8(0x77), other.values);
            } else {
                _mm_storeu_ps(&value0, _mm256_castps256_ps128(other.values));
                const __m128 back = _mm_load_ss(&(this[1].value0));
                _mm_storeu_ps(&value3, _mm256_extractf128_ps(other.values, 1));
                _mm_store_ss(&(this[1].value0), back);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            _mm_storeu_ps(&value0, other.values0);
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                _mm_mask_compressstoreu_ps(&value3, _cvtu32_mask8(0x7), other.values1);
            } else {
                // Need to backup the 4th element to prevent memory corruption by using the 4 element store
                const __m128 back = _mm_load_ss(&(this[1].value0));
                _mm_storeu_ps(&value3, other.values1);
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
        }
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_INLINE SIMD3x2<T, Width> load() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3x2<T, Width>(_mm256_set_m128(_mm_loadu_ps(&value3), _mm_loadu_ps(&value0)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3x2<T, Width>(_mm_loadu_ps(&value0), _mm_loadu_ps(&value3));
        } else
#endif
        {
            return SIMD3x2<T, Width>(value0, value1, value2, value3, value4, value5);
        }
    }
};

template<typename T>
class alignas(maxAlignment<T, 6>) SIMD3x2DataPad
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    T value0, value1, value2, pad0;
    T value3, value4, value5, pad1;

    /** Default constructor. */
    XS_INLINE SIMD3x2DataPad() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit SIMD3x2DataPad(const SIMD3x2<T, Width>& other) noexcept
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
     */
    XS_INLINE void setData(T other0, T other1, T other2, T other3, T other4, T other5) noexcept
    {
        value0 = other0;
        value1 = other1;
        value2 = other2;
        value3 = other3;
        value4 = other4;
        value5 = other5;
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const SIMD3x2<T, Width>& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            _mm256_store_ps(&value0, other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            _mm_store_ps(&value0, other.values0);
            _mm_store_ps(&value3, other.values1);
        } else
#endif
        {
            value0 = other.values0;
            value1 = other.values1;
            value2 = other.values2;
            value3 = other.values3;
            value4 = other.values4;
            value5 = other.values5;
        }
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_INLINE SIMD3x2<T, Width> load() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3x2<T, Width>(_mm256_load_ps(&value0));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3x2<T, Width>(_mm_load_ps(&value0), _mm_load_ps(&value3));
        } else
#endif
        {
            return SIMD3x2<T, Width>(value0, value1, value2, value3, value4, value5);
        }
    }
};

template<typename T, SIMDWidth Width = widthSIMD<T>>
class SIMD3x2 : public NoExport::SIMDData<T, 6, 2, Width>
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    using Type = T;
    using InternalData = NoExport::SIMDData<T, 6, 2, Width>;
    using Data = SIMD3x2Data<T>;
    using DataPad = SIMD3x2DataPad<T>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = InternalData::width;
    static constexpr uint32 numValues = 6;
    static constexpr uint32 size = InternalData::size;
    using BaseDef = SIMDBase<T, SIMDBase<T, widthImpl>::widthImpl>;
    using InBaseDef = SIMDInBase<T, SIMDInBase<T, widthImpl>::widthImpl>;
    using SIMD2Def = SIMD2<T, SIMD2<T, widthImpl>::widthImpl>;
    using SIMD3Def = SIMD3<T, SIMD3<T, widthImpl>::widthImpl>;
    using SIMD6Def = SIMD6<T, SIMD6<T, widthImpl>::widthImpl>;
    using InternalData::SIMDData;

    /** Hex Mask object used to store 6 different masks at once. */
    class Mask : public NoExport::SIMDMaskData<T, 6, 2, Width>
    {
    public:
        using Type = T;
        using InternalData = NoExport::SIMDMaskData<T, 6, 2, Width>;
        static constexpr SIMDWidth width = Width;
        static constexpr SIMDWidth widthImpl = InternalData::width;
        using InternalData::SIMDMaskData;
        using Bool = Bool3x2<hasSIMD<T> && (Width > SIMDWidth::Scalar)>;

        /** Default constructor. */
        XS_INLINE Mask() noexcept = default;

        /**
         * Copy constructor.
         * @param other The other.
         */
        XS_INLINE Mask(const Mask& other) = default;

        /**
         * Move constructor.
         * @param [in,out] other The other.
         */
        XS_INLINE Mask(Mask&& other) noexcept = default;

        /**
         * Assignment operator.
         * @param other The other.
         * @returns A shallow copy of this object.
         */
        XS_INLINE Mask& operator=(const Mask& other) = default;

        /**
         * Move assignment operator.
         * @param [in,out] other The other.
         * @returns A shallow copy of this object.
         */
        XS_INLINE Mask& operator=(Mask&& other) noexcept = default;

        /**
         * Construct a mask from 2 bitwise integer representations of mask3s.
         * @note Each bit in the inputs is used to set the mask accordingly. The low 3 bits of the first input and the
         * low 3 bits of the second input are used.
         * @param mask0 First input bitwise representation.
         * @param mask1 Second input bitwise representation.
         */
        XS_INLINE Mask(uint32 mask0, uint32 mask1) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    this->values = _cvtu32_mask8(mask0 | (mask1 << 4));
                } else {
                    __m256i values = _mm256_set_m128i(_mm_cvtsi32_si128(mask1), _mm_cvtsi32_si128(mask0));
                    values = _mm256_shuffle_epi32(values, _MM_SHUFFLE(0, 0, 0, 0));
                    const __m256i bitMask = _mm256_set_epi32(0x8, 0x4, 0x2, 0x1, 0x8, 0x4, 0x2, 0x1);
                    values = _mm256_and_si256(values, bitMask);
                    values = _mm256_cmpeq_epi32(values, bitMask);
                    this->values = _mm256_castsi256_ps(values);
                }
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    this->values0 = _cvtu32_mask8(mask0);
                    this->values1 = _cvtu32_mask8(mask1);
                } else {
                    __m128i val0 = _mm_shuffle0000_epi32(_mm_cvtsi32_si128(mask0));
                    __m128i val1 = _mm_shuffle0000_epi32(_mm_cvtsi32_si128(mask1));
                    const __m128i bitMask = _mm_set_epi32(0x8, 0x4, 0x2, 0x1);
                    val0 = _mm_and_si128(val0, bitMask);
                    val1 = _mm_and_si128(val1, bitMask);
                    val0 = _mm_cmpeq_epi32(val0, bitMask);
                    val1 = _mm_cmpeq_epi32(val1, bitMask);
                    this->values0 = _mm_castsi128_ps(val0);
                    this->values1 = _mm_castsi128_ps(val1);
                }
            } else
#endif
            {
                this->values0 = mask0 & 0x1;
                this->values1 = mask0 & 0x2;
                this->values2 = mask0 & 0x4;
                this->values3 = mask1 & 0x1;
                this->values4 = mask1 & 0x2;
                this->values5 = mask1 & 0x4;
            }
        }

        /**
         * Construct a mask from a bitwise integer representation.
         * @note Each bit in the input is used to set the mask accordingly.
         * @param mask Input bitwise representation.
         */
        XS_INLINE explicit Mask(uint32 mask) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    this->values = _cvtu32_mask8(mask);
                } else {
                    __m256i values;
                    if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                        values = _mm256_broadcastd_epi32(_mm_cvtsi32_si128(mask));
                    } else {
                        const __m128i low = _mm_shuffle_epi32(_mm_cvtsi32_si128(mask), _MM_SHUFFLE(0, 0, 0, 0));
                        values = _mm256_set_m128i(low, low);
                    }
                    const __m256i bitMask = _mm256_set_epi32(0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1);
                    values = _mm256_and_si256(values, bitMask);
                    values = _mm256_cmpeq_epi32(values, bitMask);
                    this->values = _mm256_castsi256_ps(values);
                }
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    this->values0 = _cvtu32_mask8(mask);
                    this->values1 = _cvtu32_mask8(mask >> 4);
                } else {
                    __m128i val0 = _mm_shuffle0000_epi32(_mm_cvtsi32_si128(mask));
                    __m128i val1 = val0;
                    const __m128i bitMask0 = _mm_set_epi32(0x8, 0x4, 0x2, 0x1);
                    const __m128i bitMask1 = _mm_set_epi32(0x80, 0x40, 0x20, 0x10);
                    val0 = _mm_and_si128(val0, bitMask0);
                    val1 = _mm_and_si128(val1, bitMask1);
                    val0 = _mm_cmpeq_epi32(val0, bitMask0);
                    val1 = _mm_cmpeq_epi32(val1, bitMask1);
                    this->values0 = _mm_castsi128_ps(val0);
                    this->values1 = _mm_castsi128_ps(val1);
                }
            } else
#endif
            {
                this->values0 = mask & 0x1;
                this->values1 = mask & 0x2;
                this->values2 = mask & 0x4;
                this->values3 = mask & 0x10;
                this->values4 = mask & 0x20;
                this->values5 = mask & 0x40;
            }
        }

        /**
         * Construct a mask from individual booleans.
         * @param bool0 The first boolean value.
         * @param bool1 The second boolean value.
         * @param bool2 The third boolean value.
         * @param bool3 The fourth boolean value.
         * @param bool4 The fifth boolean value.
         * @param bool5 The sixth boolean value.
         */
        XS_INLINE Mask(bool bool0, bool bool1, bool bool2, bool bool3, bool bool4, bool bool5) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    this->values =
                        _cvtu32_mask8((bool5 << 6) | (bool4 << 5) | (bool3 << 4) | (bool2 << 2) | (bool1 << 1) | bool0);
                } else {
                    this->values = _mm256_cmp_ps(_mm256_set_ps(0, bool5, bool4, bool3, 0, bool2, bool1, bool0),
                        _mm256_setzero_ps(), _CMP_NEQ_OQ);
                }
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    this->values0 = _cvtu32_mask8((bool2 << 2) & (bool1 << 1) & bool0);
                    this->values1 = _cvtu32_mask8((bool5 << 2) & (bool4 << 1) & bool3);
                } else {
                    const __m128 zero = _mm_setzero_ps();
                    this->values0 = _mm_cmpneq_ps(_mm_set_ps(0, bool2, bool1, bool0), zero);
                    this->values1 = _mm_cmpneq_ps(_mm_set_ps(0, bool5, bool4, bool3), zero);
                }
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

        /**
         * Get the boolean equivalent of the mask.
         * @returns The bool.
         */
        XS_INLINE Bool getBool3x2() const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return Bool3x2<true>(static_cast<uint8>(_cvtmask8_u32(this->values)));
                } else {
                    return Bool3x2<true>(static_cast<uint8>(_mm256_movemask_ps(this->values)));
                }
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return Bool3x2<true>(static_cast<uint8>(_cvtmask8_u32(this->values0) << 4UL) |
                        static_cast<uint8>(_cvtmask8_u32(this->values1)));
                } else {
                    return Bool3x2<true>(static_cast<uint8>(_mm_movemask_ps(this->values0) << 4UL) |
                        static_cast<uint8>(_mm_movemask_ps(this->values1)));
                }
            } else
#endif
            {
                return Bool3x2(
                    this->values0, this->values1, this->values2, this->values3, this->values4, this->values5);
            }
        }

        /**
         * Get boolean signalling if any element of mask is set.
         * @returns Boolean.
         */
        XS_INLINE bool getAny() const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return static_cast<bool>(_cvtmask8_u32(this->values) & 0x77);
                } else {
                    return static_cast<bool>(_mm256_movemask_ps(this->values) & 0x77);
                }
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return static_cast<bool>(_cvtmask8_u32(this->values0) | (_cvtmask8_u32(this->values1) & 0x7));
                } else {
                    return static_cast<bool>(_mm_movemask_ps(this->values0) | (_mm_movemask_ps(this->values1) & 0x7));
                }
            } else
#endif
            {
                return (this->values0 | this->values1) | (this->values2 | this->values3) |
                    (this->values4 | this->values5);
            }
        }

        /**
         * Get boolean signalling if all elements of mask is set.
         * @returns Boolean.
         */
        XS_INLINE bool getAll() const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return ((_cvtmask8_u32(this->values) & 0x77) == 0x77);
                } else {
                    return ((_mm256_movemask_ps(this->values) & 0x77) == 0x77);
                }
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return ((_cvtmask8_u32(this->values0) & (_cvtmask8_u32(this->values1) & 0x7)) == 0x7);
                } else {
                    return ((_mm_movemask_ps(this->values0) & (_mm_movemask_ps(this->values1) & 0x7)) == 0x7);
                }
            } else
#endif
            {
                return (this->values0 & this->values1) & (this->values2 & this->values3) &
                    (this->values4 & this->values5);
            }
        }

        /**
         * Get boolean signalling if no elements of mask is set.
         * @returns Boolean.
         */
        XS_INLINE bool getNone() const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return ((_cvtmask8_u32(this->values) & 0x77) == 0x0);
                } else {
                    return ((_mm256_movemask_ps(this->values) & 0x77) == 0x0);
                }
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return ((_cvtmask8_u32(this->values0) & (_cvtmask8_u32(this->values1) & 0x7)) == 0x0);
                } else {
                    return ((_mm_movemask_ps(this->values0) & (_mm_movemask_ps(this->values1) & 0x7)) == 0x0);
                }
            } else
#endif
            {
                return !getAny();
            }
        }

    private:
#if XS_ISA == XS_X86
        class SIMDMasker3x2X86
        {
        public:
            XS_INLINE static SIMD3x2& ToType(SIMD3x2 other)
            {
                return other;
            }
        };
#endif

        template<uint8 Index>
        class SIMDMasker3x2
        {
        public:
            XS_INLINE static InBaseDef ToType(SIMD3x2 other)
            {
                return reinterpret_cast<InBaseDef>(other.values)[Index];
            }
        };

    public:
        /**
         * Perform automated instruction execution and masking operations.
         * @note This function can be used to apply optimisations when using masking to avoid conditional branches.
         *  This macro will automatically convert code in expression to an optimised form based on whether the program
         * is running in SIMD mode or not. Example usage:
         * class MaskFunc {
         *  SIMD3<float32> m_force; SIMD3<float32> m_init;
         *  MaskFunc(const SIMD3<float32>& init) :
         *      m_init(init){}
         *  template<typename MaskType, typename Masker>
         *  MaskType expression(){
         *      MaskType force = Masker::ToType(overlap) * MaskType(3.0);
         *      return Masker::ToType(force) - (Masker::ToType(velocity)) * MaskType(2.0)))
         *  }
         *  template<typename MaskType, typename Masker>
         *  void finalExpression(const MaskType& final) {
         *      m_force = final; m_init += m_force;
         *  }
         * };
         * init.lessOrEqualMask(SIMD3<float>::Zero()).maskFunction(MaskFunc);
         * @tparam MaskOperator The masking function type.
         * @param [in,out] maskFunc class that contains function to execute as part of masking operation.
         */
        template<typename MaskOperator>
        XS_INLINE void mask3x2Function(MaskOperator& maskFunc) const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                auto value = maskFunc.template expression<SIMD3x2, SIMDMasker3x2X86>();
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    value.values = _mm256_mask_blend_ps(this->values, _mm256_setzero_ps(), value.values);
                } else {
                    value.values = _mm256_and_ps(value.values, this->values);
                }
                maskFunc.template finalExpression<SIMD3x2, SIMDMasker3x2X86>(value);
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                auto value = maskFunc.template expression<SIMD3x2, SIMDMasker3x2X86>();
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    const __m128 zero = _mm_setzero_ps();
                    value.values0 = _mm_mask_blend_ps(this->values0, zero, value.values0);
                    value.values1 = _mm_mask_blend_ps(this->values1, zero, value.values1);
                } else {
                    value.values0 = _mm_and_ps(value.values0, this->values0);
                    value.values1 = _mm_and_ps(value.values1, this->values1);
                }
                maskFunc.template finalExpression<SIMD3x2, SIMDMasker3x2X86>(value);
            } else
#endif
            {
                if (this->values0) {
                    SIMDInBase value = maskFunc.template expression<InBaseDef, SIMDMasker3x2<0>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<0>>(value);
                }
                if (this->values1) {
                    SIMDInBase value = maskFunc.template expression<InBaseDef, SIMDMasker3x2<1>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<1>>(value);
                }
                if (this->values2) {
                    SIMDInBase value = maskFunc.template expression<InBaseDef, SIMDMasker3x2<2>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<2>>(value);
                }
                if (this->values3) {
                    SIMDInBase value = maskFunc.template expression<InBaseDef, SIMDMasker3x2<3>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<3>>(value);
                }
                if (this->values4) {
                    SIMDInBase value = maskFunc.template expression<InBaseDef, SIMDMasker3x2<4>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<4>>(value);
                }
                if (this->values5) {
                    SIMDInBase value = maskFunc.template expression<InBaseDef, SIMDMasker3x2<5>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<5>>(value);
                }
            }
        }

        /**
         * Perform automated instruction execution and masking operations.
         * @note This function can be used to apply optimisations when using masking to avoid conditional branches.
         *  This macro will automatically convert code in expression to an optimised form based on whether the program
         * is running in SIMD mode or not. Example usage:
         *  class SRGBToRGBMask { SIMD3<float32> m_SRGB; SIMD3<float> return;
         *  SRGBToRGBMask(const SIMD3<float32>& SRGB) :
         *      m_SRGB(SRGB){}
         *  template<typename MaskType, typename Masker>
         *  MaskType expression1( ){
         *      return (Masker::ToType(m_SRGB) / MaskType(12.92)); }
         *  template<typename MaskType, typename Masker>
         *  MaskType expression2( ) {
         *      MaskType retGT = Masker::ToType(m_SRGB) + MaskType(0.055);
         *      retGT /= MaskType(1.055);
         *      return retGT.powr(MaskType(2.4)); }
         *  template<typename MaskType, typename Masker>
         *  void finalExpression(const MaskType& final) {
         *      Masker::toType(m_return) = final; }
         *  };
         *  SRGB.lessOrEqualMask(BaseDef(0.04045)).mask3ElseFunction<SRGBToRGBMask>(maskFunction);
         * @tparam MaskOperator The masking function type.
         * @param [in,out] maskFunc class that contains function to execute as part of masking operation.
         */
        template<typename MaskOperator>
        XS_INLINE void mask3x2ElseFunction(MaskOperator& maskFunc) const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                auto value1 = maskFunc.template expression1<SIMD3x2, SIMDMasker3x2>();
                auto value2 = maskFunc.template expression2<SIMD3x2, SIMDMasker3x2>();
                const auto final(value1.blendVar(value2, *this));
                maskFunc.template finalExpression<SIMD3x2, SIMDMasker3x2>(final);
            } else
#endif
            {
                auto value = (this->values0) ? maskFunc.template expression1<InBaseDef, SIMDMasker3x2<0>>() :
                                               maskFunc.template expression2<InBaseDef, SIMDMasker3x2<0>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<0>>(value);
                value = (this->values1) ? maskFunc.template expression1<InBaseDef, SIMDMasker3x2<1>>() :
                                          maskFunc.template expression2<InBaseDef, SIMDMasker3x2<1>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<1>>(value);
                value = (this->values2) ? maskFunc.template expression1<InBaseDef, SIMDMasker3x2<2>>() :
                                          maskFunc.template expression2<InBaseDef, SIMDMasker3x2<2>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<2>>(value);
                value = (this->values3) ? maskFunc.template expression1<InBaseDef, SIMDMasker3x2<3>>() :
                                          maskFunc.template expression2<InBaseDef, SIMDMasker3x2<3>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<3>>(value);
                value = (this->values4) ? maskFunc.template expression1<InBaseDef, SIMDMasker3x2<4>>() :
                                          maskFunc.template expression2<InBaseDef, SIMDMasker3x2<4>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<4>>(value);
                value = (this->values5) ? maskFunc.template expression1<InBaseDef, SIMDMasker3x2<5>>() :
                                          maskFunc.template expression2<InBaseDef, SIMDMasker3x2<5>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker3x2<5>>(value);
            }
        }
    };

    /** Default constructor. */
    XS_INLINE SIMD3x2() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE SIMD3x2(const SIMD3x2& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE SIMD3x2(SIMD3x2&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMD3x2& operator=(const SIMD3x2& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMD3x2& operator=(SIMD3x2&& other) noexcept = default;

    /**
     * Construct from 6 different values.
     * @param value0 The first value.
     * @param value1 The second value.
     * @param value2 The third value.
     * @param value3 The fourth value.
     * @param value4 The fifth value.
     * @param value5 The sixth value.
     */
    XS_INLINE SIMD3x2(T value0, T value1, T value2, T value3, T value4, T value5) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values = _mm256_set_ps(0, value5, value4, value3, 0, value2, value1, value0);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_set_ps(0, value2, value1, value0);
            this->values1 = _mm_set_ps(0, value5, value4, value3);
        } else
#endif
        {
            this->values0 = value0;
            this->values1 = value1;
            this->values2 = value2;
            this->values3 = value3;
            this->values4 = value4;
            this->values5 = value5;
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param value Value to set all elements to.
     */
    XS_INLINE explicit SIMD3x2(T value) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values = _mm256_set1_ps(value);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_set1_ps(value);
            this->values1 = this->values0;
        } else
#endif
        {
            this->values0 = value;
            this->values1 = value;
            this->values2 = value;
            this->values3 = value;
            this->values4 = value;
            this->values5 = value;
        }
    }

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_INLINE explicit SIMD3x2(SIMD3x2<T, Width2> other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar) && (Width2 > SIMDWidth::Scalar)) {
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width <= SIMDWidth::B16) &&
                (Width2 >= SIMDWidth::B32)) {
                this->values0 = _mm256_castps256_ps128(other.values);
                this->values1 = _mm256_extractf128_ps(other.values, 1);
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32) &&
                (Width2 <= SIMDWidth::B16)) {
                this->values = _mm256_set_m128(other.values1, other.values0);
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32) &&
                (Width2 >= SIMDWidth::B32)) {
                this->values = other.values;
            } else if constexpr (isSame<T, float32>) {
                this->values0 = other.values0;
                this->values1 = other.values1;
            }
        } else if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                this->values = _mm256_set_ps(
                    0, 0, other.values5, other.values4, other.values3, other.values2, other.values1, other.values0);
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                this->values0 = _mm_set_ps(0, other.values2, other.values1, other.values0);
                this->values1 = _mm_set_ps(0, other.values5, other.values4, other.values3);
            }
        } else if constexpr (Width2 > SIMDWidth::Scalar) {
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width2 >= SIMDWidth::B32)) {
                this->values0 = _mm_cvtss_f32(_mm256_castps256_ps128(other.values));
                this->values1 = _mm_cvtss_f32(_mm_shuffle3311_ps(_mm256_castps256_ps128(other.values)));
                this->values2 = _mm_cvtss_f32(_mm_shuffle3232_ps(_mm256_castps256_ps128(other.values)));
                const auto hi = _mm256_extractf128_ps(this->values, 1);
                this->values3 = _mm_cvtss_f32(hi);
                this->values4 = _mm_cvtss_f32(_mm_shuffle3311_ps(hi));
                this->values5 = _mm_cvtss_f32(_mm_shuffle3232_ps(hi));
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width2 >= SIMDWidth::B16)) {
                this->values0 = _mm_cvtss_f32(other.values0);
                this->values1 = _mm_cvtss_f32(_mm_shuffle3311_ps(other.values0)); //(x,x,x,1)
                this->values2 = _mm_cvtss_f32(_mm_shuffle3232_ps(other.values0)); //(x,x,x,2)
                this->values3 = _mm_cvtss_f32(_mm_shuffle3333_ps(other.values0)); //(x,x,x,3)
                this->values4 = _mm_cvtss_f32(other.values1);
                this->values5 = _mm_cvtss_f32(_mm_shuffle3311_ps(other.values1)); //(x,x,x,1)
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
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_INLINE explicit SIMD3x2(BaseDef other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values = other.values;
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = other.values;
            this->values1 = other.values;
        } else
#endif
        {
            this->values0 = other.value;
            this->values1 = other.value;
            this->values2 = other.value;
            this->values3 = other.value;
            this->values4 = other.value;
            this->values5 = other.value;
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_INLINE explicit SIMD3x2(InBaseDef other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                this->values = _mm256_broadcastss_ps(other.values);
            } else {
                const __m128 val = _mm_shuffle0000_ps(other.values);
                this->values = _mm256_broadcastf128_ps(val);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_shuffle0000_ps(other.values);
            this->values1 = this->values0;
        } else
#endif
        {
            this->values0 = other.value;
            this->values1 = other.value;
            this->values2 = other.value;
            this->values3 = other.value;
            this->values4 = other.value;
            this->values5 = other.value;
        }
    }

    /**
     * Construct from 2 identical SIMD2s.
     * @param other The SIMD2.
     */
    XS_INLINE explicit SIMD3x2(const SIMD2Def& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values = _mm256_set_m128(_mm_shuffle1111_ps(other.values), _mm_shuffle0000_ps(other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_shuffle0000_ps(other.values);
            this->values1 = _mm_shuffle1111_ps(other.values);
        } else
#endif
        {
            this->values0 = other.values0;
            this->values1 = other.values0;
            this->values2 = other.values0;
            this->values3 = other.values1;
            this->values4 = other.values1;
            this->values5 = other.values1;
        }
    }

    /**
     * Construct from 2 different SIMD3s.
     * @param other0 The first SIMD3.
     * @param other1 The second SIMD3.
     */
    XS_INLINE SIMD3x2(const SIMD3Def& other0, const SIMD3Def& other1) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values = _mm256_set_m128(other1.values, other0.values);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = other0.values;
            this->values1 = other1.values;
        } else
#endif
        {
            this->values0 = other0.values0;
            this->values1 = other0.values1;
            this->values2 = other0.values2;
            this->values3 = other1.values0;
            this->values4 = other1.values1;
            this->values5 = other1.values2;
        }
    }

    /**
     * Construct from a different SIMD3.
     * @param other The SIMD3.
     */
    XS_INLINE explicit SIMD3x2(const SIMD3Def& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values = _mm256_broadcastf128_ps(other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = other.values;
            this->values1 = other.values;
        } else
#endif
        {
            this->values0 = other.values0;
            this->values1 = other.values1;
            this->values2 = other.values2;
            this->values3 = other.values0;
            this->values4 = other.values1;
            this->values5 = other.values2;
        }
    }

    /**
     * Construct from a SIMD6.
     * @note SIMD6 uses packing when storing SIMD3s as a result the returned SIMD3x2 will have values in a
     * different (unpacked) order.
     * @param other The SIMD6.
     */
    XS_INLINE explicit SIMD3x2(const SIMD6Def& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val = _mm256_extractf128_ps(other.values, 1);
            const __m128 val0 = _mm_shuffle_ps(_mm256_castps256_ps128(other.values), val, _MM_SHUFFLE(2, 0, 2, 0));
            const __m128 val1 = _mm_shuffle_ps(_mm256_castps256_ps128(other.values), val, _MM_SHUFFLE(3, 1, 3, 1));
            this->values = _mm256_set_m128(val1, val0);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_shuffle_ps(other.values0, other.values1, _MM_SHUFFLE(2, 0, 2, 0));
            this->values1 = _mm_shuffle_ps(other.values0, other.values1, _MM_SHUFFLE(3, 1, 3, 1));
        } else
#endif
        {
            this->values0 = other.values0;
            this->values1 = other.values2;
            this->values2 = other.values4;
            this->values3 = other.values1;
            this->values4 = other.values3;
            this->values5 = other.values5;
        }
    }

    /**
     * Construct a SIMD6 from a SIMD3x2.
     * @note Note SIMD3s are packed when put into a SIMD6. This means that the first 2 elements will
     * actually consist of the packed first element of each of the 2 SIMD3s etc.
     */
    XS_INLINE explicit operator SIMD6Def() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val = _mm256_extractf128_ps(this->values, 1);
            return SIMD6Def(_mm256_set_m128(_mm_unpackhi_ps(_mm256_castps256_ps128(this->values), val),
                _mm_unpacklo_ps(_mm256_castps256_ps128(this->values), val)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6Def(
                _mm_unpacklo_ps(this->values0, this->values1), _mm_unpackhi_ps(this->values0, this->values1));
        } else
#endif
        {
            return SIMD6Def(this->values0, this->values3, this->values1, this->values4, this->values2, this->values5);
        }
    }

    /**
     * Constructor to build set to 0.
     * @returns Newly constructed object with required attributes.
     */
    XS_INLINE static SIMD3x2 Zero() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3x2(_mm256_setzero_ps());
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const auto value = _mm_setzero_ps();
            return SIMD3x2(value, value);
        } else
#endif
        {
            return SIMD3x2(T{0});
        }
    }

    /**
     * Constructor to build set to 1.
     * @returns Newly constructed object with required attributes.
     */
    XS_INLINE static SIMD3x2 One() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3x2(_mm256_set1_ps(1.0f));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const auto value = _mm_set1_ps(1.0f);
            return SIMD3x2(value, value);
        } else
#endif
        {
            return SIMD3x2(T{1});
        }
    }

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-5).
     * @returns SIMDInBase containing the desired value.
     */
    template<uint32 Index>
    XS_INLINE InBaseDef getValueInBase() const noexcept
    {
        static_assert(Index < 6, "Invalid Index: Index must be <6");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                return InBaseDef(_mm256_castps256_ps128(this->values));
            } else if constexpr (Index == 1) {
                return InBaseDef(_mm_shuffle3311_ps(_mm256_castps256_ps128(this->values))); //(x,x,x,1)
            } else if constexpr (Index == 2) {
                return InBaseDef(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values)));
            } else if constexpr (Index == 3) {
                return InBaseDef(_mm256_extractf128_ps(this->values, 1));
            } else if constexpr (Index == 4) {
                return InBaseDef(_mm_shuffle3311_ps(_mm256_extractf128_ps(this->values, 1))); //(x,x,x,1)
            } else /*Index == 5*/ {
                const __m128 val = _mm256_extractf128_ps(this->values, 1);
                return InBaseDef(_mm_shuffle3232_ps(val));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 3 == 0) {
                return InBaseDef((&this->values0)[Index / 3]);
            } else if constexpr (Index % 3 == 1) {
                return InBaseDef(_mm_shuffle3311_ps((&this->values0)[Index / 3])); //(x,x,x,1)
            } else /*Index % 3 == 2*/ {
                return InBaseDef(_mm_shuffle3232_ps((&this->values0)[Index / 3])); //(x,x,x,2)
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
     * @tparam Index The index of the element to retrieve (range is 0-5).
     * @returns BaseDef containing the desired value.
     */
    template<uint32 Index>
    XS_INLINE BaseDef getValue() const noexcept
    {
        static_assert(Index < 6, "Invalid Index: Index must be <6");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX2> && Index == 0) {
                return BaseDef(_mm256_broadcastss_ps(_mm256_castps256_ps128(this->values)));
            } else if constexpr (Index == 0) {
                const __m128 val = _mm_shuffle0000_ps(_mm256_castps256_ps128(this->values));
                return BaseDef(_mm256_broadcastf128_ps(val));
            } else if constexpr (Index == 1) {
                const __m128 val = _mm_shuffle1111_ps(_mm256_castps256_ps128(this->values));
                return BaseDef(_mm256_broadcastf128_ps(val));
            } else if constexpr (Index == 2) {
                const __m128 val = _mm_shuffle2222_ps(_mm256_castps256_ps128(this->values));
                return BaseDef(_mm256_broadcastf128_ps(val));
            } else if constexpr (Index == 3) {
                const __m256 val = _mm256_shuffle1_ps(this->values, _MM_SHUFFLE(0, 0, 0, 0));
                return BaseDef(_mm256_shuffle76547654_ps(val));
            } else if constexpr (Index == 4) {
                const __m256 val = _mm256_shuffle1_ps(this->values, _MM_SHUFFLE(1, 1, 1, 1));
                return BaseDef(_mm256_shuffle76547654_ps(val));
            } else /*Index == 5*/ {
                const __m256 val = _mm256_shuffle1_ps(this->values, _MM_SHUFFLE(2, 2, 2, 2));
                return BaseDef(_mm256_shuffle76547654_ps(val));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 3 == 0) {
                return BaseDef(_mm_shuffle0000_ps((&this->values0)[Index / 3]));
            } else if constexpr (Index % 3 == 1) {
                return BaseDef(_mm_shuffle1111_ps((&this->values0)[Index / 3]));
            } else /*Index % 3 == 2*/ {
                return BaseDef(_mm_shuffle2222_ps((&this->values0)[Index / 3]));
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
     * @tparam Index The index of the element to retrieve (range is 0-1).
     * @returns SIMD3 containing the desired values.
     */
    template<uint32 Index>
    XS_INLINE SIMD3Def getValue3() const noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                return SIMD3Def(_mm256_castps256_ps128(this->values));
            } else {
                return SIMD3Def(_mm256_extractf128_ps(this->values, 1));
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
     * Set a SIMD3 in a SIMD3x2.
     * @tparam Index The index of the element to set (range is 0-1).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_INLINE void setValue3(const SIMD3Def& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                this->values = _mm256_blend_ps(
                    this->values, _mm256_castps128_ps256(other.values), _MM256_BLEND(0, 0, 0, 0, 1, 1, 1, 1));
            } else /*Index == 1*/ {
                this->values = _mm256_insertf128_ps(this->values, other.values, 1);
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
     * Negate an element of the object.
     * @tparam Elem0 Boolean indicating if first element should be negated.
     * @tparam Elem1 Boolean indicating if second element should be negated.
     * @tparam Elem2 Boolean indicating if third element should be negated.
     * @tparam Elem3 Boolean indicating if fourth element should be negated.
     * @tparam Elem4 Boolean indicating if fifth element should be negated.
     * @tparam Elem5 Boolean indicating if sixth element should be negated.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5>
    XS_INLINE SIMD3x2 negate() const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5) {
                return SIMD3x2(_mm256_xor_ps(this->values, _mm256_set1_ps(-0.0f)));
            } else {
                return SIMD3x2(_mm256_xor_ps(this->values,
                    _mm256_set_ps(0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f, 0.0f,
                        Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Elem0 && !Elem1 && !Elem2 && Elem3 && !Elem4 && !Elem5) {
                const __m128 value = _mm_set_ss(-0.0f);
                return SIMD3x2(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD3x2(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value));
            } else if constexpr (Elem0 && Elem1 && Elem2 && !Elem3 && !Elem4 && !Elem5) {
                return SIMD3x2(_mm_xor_ps(this->values0, _mm_set1_ps(-0.0f)), this->values1);
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && Elem3 && Elem4 && Elem5) {
                return SIMD3x2(this->values0, _mm_xor_ps(this->values1, _mm_set1_ps(-0.0f)));
            } else if constexpr (Elem0 == Elem3 && Elem1 == Elem4 && Elem2 == Elem5) {
                const __m128 value = _mm_set_ps(0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f);
                return SIMD3x2(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value));
            } else {
                return SIMD3x2(_mm_xor_ps(this->values0,
                                   _mm_set_ps(0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)),
                    _mm_xor_ps(this->values1,
                        _mm_set_ps(0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f)));
            }
        }
#endif
        else {
            return SIMD3x2(Elem0 ? -this->values0 : this->values0, Elem1 ? -this->values1 : this->values1,
                Elem2 ? -this->values2 : this->values2, Elem3 ? -this->values3 : this->values3,
                Elem4 ? -this->values4 : this->values4, Elem5 ? -this->values5 : this->values5);
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
    XS_INLINE SIMD3x2 mad(const SIMD3x2& other1, const SIMD3x2& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x2(_mm256_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD3x2(_mm256_add_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x2(_mm_fmadd_ps(this->values0, other1.values0, other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values1, other2.values1));
            } else {
                return SIMD3x2(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1));
            }
        } else
#endif
        {
            return SIMD3x2(Shift::fma<T>(this->values0, other1.values0, other2.values0),
                Shift::fma<T>(this->values1, other1.values1, other2.values1),
                Shift::fma<T>(this->values2, other1.values2, other2.values2),
                Shift::fma<T>(this->values3, other1.values3, other2.values3),
                Shift::fma<T>(this->values4, other1.values4, other2.values4),
                Shift::fma<T>(this->values5, other1.values5, other2.values5));
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
    XS_INLINE SIMD3x2 mad(BaseDef other1, const SIMD3x2& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x2(_mm256_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD3x2(_mm256_add_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x2(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values, other2.values1));
            } else {
                return SIMD3x2(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1));
            }
        } else
#endif
        {
            return SIMD3x2(Shift::fma<T>(this->values0, other1.value, other2.values0),
                Shift::fma<T>(this->values1, other1.value, other2.values1),
                Shift::fma<T>(this->values2, other1.value, other2.values2),
                Shift::fma<T>(this->values3, other1.value, other2.values3),
                Shift::fma<T>(this->values4, other1.value, other2.values4),
                Shift::fma<T>(this->values5, other1.value, other2.values5));
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
    XS_INLINE SIMD3x2 mad(const SIMD2Def& other1, const SIMD3x2& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val = _mm256_set_m128(_mm_shuffle1111_ps(other1.values), _mm_shuffle0000_ps(other1.values));
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x2(_mm256_fmadd_ps(this->values, val, other2.values));
            } else {
                return SIMD3x2(_mm256_add_ps(_mm256_mul_ps(this->values, val), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val0 = _mm_shuffle0000_ps(other1.values);
            const __m128 val1 = _mm_shuffle1111_ps(other1.values);
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x2(_mm_fmadd_ps(this->values0, val0, other2.values0),
                    _mm_fmadd_ps(this->values1, val1, other2.values1));
            } else {
                return SIMD3x2(_mm_add_ps(_mm_mul_ps(this->values0, val0), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, val1), other2.values1));
            }
        } else
#endif
        {
            return SIMD3x2(Shift::fma<T>(this->values0, other1.values0, other2.values0),
                Shift::fma<T>(this->values1, other1.values0, other2.values1),
                Shift::fma<T>(this->values2, other1.values0, other2.values2),
                Shift::fma<T>(this->values3, other1.values1, other2.values3),
                Shift::fma<T>(this->values4, other1.values1, other2.values4),
                Shift::fma<T>(this->values5, other1.values1, other2.values5));
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
    XS_INLINE SIMD3x2 mad(const SIMD3Def& other1, const SIMD3x2& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val = _mm256_broadcastf128_ps(other1.values);
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x2(_mm256_fmadd_ps(this->values, val, other2.values));
            } else {
                return SIMD3x2(_mm256_add_ps(_mm256_mul_ps(this->values, val), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x2(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values, other2.values1));
            } else {
                return SIMD3x2(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1));
            }
        } else
#endif
        {
            return SIMD3x2(Shift::fma<T>(this->values0, other1.values0, other2.values0),
                Shift::fma<T>(this->values1, other1.values1, other2.values1),
                Shift::fma<T>(this->values2, other1.values2, other2.values2),
                Shift::fma<T>(this->values3, other1.values0, other2.values3),
                Shift::fma<T>(this->values4, other1.values1, other2.values4),
                Shift::fma<T>(this->values5, other1.values2, other2.values5));
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
    XS_INLINE SIMD3x2 mad(const SIMD3x2& other1, BaseDef other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x2(_mm256_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD3x2(_mm256_add_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x2(_mm_fmadd_ps(this->values0, other1.values0, other2.values),
                    _mm_fmadd_ps(this->values1, other1.values1, other2.values));
            } else {
                return SIMD3x2(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values));
            }
        } else
#endif
        {
            return SIMD3x2(Shift::fma<T>(this->values0, other1.values0, other2.value),
                Shift::fma<T>(this->values1, other1.values1, other2.value),
                Shift::fma<T>(this->values2, other1.values2, other2.value),
                Shift::fma<T>(this->values3, other1.values3, other2.value),
                Shift::fma<T>(this->values4, other1.values4, other2.value),
                Shift::fma<T>(this->values5, other1.values5, other2.value));
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
    XS_INLINE SIMD3x2 mad(const SIMD3Def& other1, const SIMD3Def& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val1 = _mm256_broadcastf128_ps(other1.values);
            const __m256 val2 = _mm256_broadcastf128_ps(other2.values);
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x2(_mm256_fmadd_ps(this->values, val1, val2));
            } else {
                return SIMD3x2(_mm256_add_ps(_mm256_mul_ps(this->values, val1), val2));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x2(_mm_fmadd_ps(this->values0, other1.values, other2.values),
                    _mm_fmadd_ps(this->values1, other1.values, other2.values));
            } else {
                return SIMD3x2(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values));
            }
        } else
#endif
        {
            return SIMD3x2(Shift::fma<T>(this->values0, other1.values0, other2.values0),
                Shift::fma<T>(this->values1, other1.values1, other2.values1),
                Shift::fma<T>(this->values2, other1.values2, other2.values2),
                Shift::fma<T>(this->values3, other1.values0, other2.values0),
                Shift::fma<T>(this->values4, other1.values1, other2.values1),
                Shift::fma<T>(this->values5, other1.values2, other2.values2));
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
    XS_INLINE SIMD3x2 msub(const SIMD3x2& other1, const SIMD3x2& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x2(_mm256_fmsub_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD3x2(_mm256_sub_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3x2(_mm_fmsub_ps(this->values0, other1.values0, other2.values0),
                    _mm_fmsub_ps(this->values1, other1.values1, other2.values1));
            } else {
                return SIMD3x2(_mm_sub_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                    _mm_sub_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1));
            }
        } else
#endif
        {
            return SIMD3x2(Shift::fma<T>(this->values0, other1.values0, -other2.values0),
                Shift::fma<T>(this->values1, other1.values1, -other2.values1),
                Shift::fma<T>(this->values2, other1.values2, -other2.values2),
                Shift::fma<T>(this->values3, other1.values3, -other2.values3),
                Shift::fma<T>(this->values4, other1.values4, -other2.values4),
                Shift::fma<T>(this->values5, other1.values5, -other2.values5));
        }
    }

    /**
     * Compare two objects are equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask equalMask(const SIMD3x2& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_EQ_OQ));
            } else {
                return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_EQ_OQ));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values0, other.values0, _CMP_EQ_OQ),
                    _mm_cmp_ps_mask(this->values1, other.values1, _CMP_EQ_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values0, other.values0, _CMP_EQ_OQ),
                    _mm_cmp_ps(this->values1, other.values1, _CMP_EQ_OQ));
            } else {
                return Mask(_mm_cmpeq_ps(this->values0, other.values0), _mm_cmpeq_ps(this->values1, other.values1));
            }
        } else
#endif
        {
            return Mask(this->values0 == other.values0, this->values1 == other.values1, this->values2 == other.values2,
                this->values3 == other.values3, this->values4 == other.values4, this->values5 == other.values5);
        }
    }

    /**
     * Compare two objects are less or equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask lessOrEqualMask(const SIMD3x2& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_LE_OQ));
            } else {
                return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_LE_OQ));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values0, other.values0, _CMP_LE_OQ),
                    _mm_cmp_ps_mask(this->values1, other.values1, _CMP_LE_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values0, other.values0, _CMP_LE_OQ),
                    _mm_cmp_ps(this->values1, other.values1, _CMP_LE_OQ));
            } else {
                return Mask(_mm_cmple_ps(this->values0, other.values0), _mm_cmple_ps(this->values1, other.values1));
            }
        } else
#endif
        {
            return Mask(this->values0 <= other.values0, this->values1 <= other.values1, this->values2 <= other.values2,
                this->values3 <= other.values3, this->values4 <= other.values4, this->values5 <= other.values5);
        }
    }

    /**
     * Compare two objects are less than.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask lessThanMask(const SIMD3x2& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_LT_OQ));
            } else {
                return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_LT_OQ));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values0, other.values0, _CMP_LT_OQ),
                    _mm_cmp_ps_mask(this->values1, other.values1, _CMP_LT_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values0, other.values0, _CMP_LT_OQ),
                    _mm_cmp_ps(this->values1, other.values1, _CMP_LT_OQ));
            } else {
                return Mask(_mm_cmplt_ps(this->values0, other.values0), _mm_cmplt_ps(this->values1, other.values1));
            }
        } else
#endif
        {
            return Mask(this->values0 < other.values0, this->values1 < other.values1, this->values2 < other.values2,
                this->values3 < other.values3, this->values4 < other.values4, this->values5 < other.values5);
        }
    }

    /**
     * Compare two objects are not equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask notEqualMask(const SIMD3x2& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_NEQ_OQ));
            } else {
                return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_NEQ_OQ));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values0, other.values0, _CMP_NEQ_OQ),
                    _mm_cmp_ps_mask(this->values1, other.values1, _CMP_NEQ_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values0, other.values0, _CMP_NEQ_OQ),
                    _mm_cmp_ps(this->values1, other.values1, _CMP_NEQ_OQ));
            } else {
                return Mask(_mm_cmpneq_ps(this->values0, other.values0), _mm_cmpneq_ps(this->values1, other.values1));
            }
        } else
#endif
        {
            return Mask(this->values0 != other.values0, this->values1 != other.values1, this->values2 != other.values2,
                this->values3 != other.values3, this->values4 != other.values4, this->values5 != other.values5);
        }
    }

    /**
     * Compare two objects are equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask equalMask(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_EQ_OQ));
            } else {
                return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_EQ_OQ));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values0, other.values, _CMP_EQ_OQ),
                    _mm_cmp_ps_mask(this->values1, other.values, _CMP_EQ_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values0, other.values, _CMP_EQ_OQ),
                    _mm_cmp_ps(this->values1, other.values, _CMP_EQ_OQ));
            } else {
                return Mask(_mm_cmpeq_ps(this->values0, other.values), _mm_cmpeq_ps(this->values1, other.values));
            }
        } else
#endif
        {
            return Mask(this->values0 == other.value, this->values1 == other.value, this->values2 == other.value,
                this->values3 == other.value, this->values4 == other.value, this->values5 == other.value);
        }
    }

    /**
     * Compare two objects are less or equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask lessOrEqualMask(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_LE_OQ));
            } else {
                return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_LE_OQ));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values0, other.values, _CMP_LE_OQ),
                    _mm_cmp_ps_mask(this->values1, other.values, _CMP_LE_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values0, other.values, _CMP_LE_OQ),
                    _mm_cmp_ps(this->values1, other.values, _CMP_LE_OQ));
            } else {
                return Mask(_mm_cmple_ps(this->values0, other.values), _mm_cmple_ps(this->values1, other.values));
            }
        } else
#endif
        {
            return Mask(this->values0 <= other.value, this->values1 <= other.value, this->values2 <= other.value,
                this->values3 <= other.value, this->values4 <= other.value, this->values5 <= other.value);
        }
    }

    /**
     * Compare two objects are less than.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask lessThanMask(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_LT_OQ));
            } else {
                return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_LT_OQ));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values0, other.values, _CMP_LT_OQ),
                    _mm_cmp_ps_mask(this->values1, other.values, _CMP_LT_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values0, other.values, _CMP_LT_OQ),
                    _mm_cmp_ps(this->values1, other.values, _CMP_LT_OQ));
            } else {
                return Mask(_mm_cmplt_ps(this->values0, other.values), _mm_cmplt_ps(this->values1, other.values));
            }
        } else
#endif
        {
            return Mask(this->values0 < other.value, this->values1 < other.value, this->values2 < other.value,
                this->values3 < other.value, this->values4 < other.value, this->values5 < other.value);
        }
    }

    /**
     * Compare two objects are greater or equal.
     * @param other The object to compare to the object.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask greaterOrEqualMask(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_GE_OQ));
            } else {
                return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_GE_OQ));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values0, other.values, _CMP_GE_OQ),
                    _mm_cmp_ps_mask(this->values1, other.values, _CMP_GE_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values0, other.values, _CMP_GE_OQ),
                    _mm_cmp_ps(this->values1, other.values, _CMP_GE_OQ));
            } else {
                return Mask(_mm_cmplt_ps(other.values, this->values0), _mm_cmplt_ps(other.values, this->values1));
            }
        } else
#endif
        {
            return Mask(this->values0 >= other.value, this->values1 >= other.value, this->values2 >= other.value,
                this->values3 >= other.value, this->values4 >= other.value, this->values5 >= other.value);
        }
    }

    /**
     * Compare two objects are greater than.
     * @param other The object to compare to the object.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask greaterThanMask(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_GT_OQ));
            } else {
                return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_GT_OQ));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values0, other.values, _CMP_GT_OQ),
                    _mm_cmp_ps_mask(this->values1, other.values, _CMP_GT_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values0, other.values, _CMP_GT_OQ),
                    _mm_cmp_ps(this->values1, other.values, _CMP_GT_OQ));
            } else {
                return Mask(_mm_cmple_ps(other.values, this->values0), _mm_cmple_ps(other.values, this->values1));
            }
        } else
#endif
        {
            return Mask(this->values0 > other.value, this->values1 > other.value, this->values2 > other.value,
                this->values3 > other.value, this->values4 > other.value, this->values5 > other.value);
        }
    }

    /**
     * Compare two objects are not equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask notEqualMask(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm256_cmp_ps_mask(this->values, other.values, _CMP_NEQ_OQ));
            } else {
                return Mask(_mm256_cmp_ps(this->values, other.values, _CMP_NEQ_OQ));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values0, other.values, _CMP_NEQ_OQ),
                    _mm_cmp_ps_mask(this->values1, other.values, _CMP_NEQ_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values0, other.values, _CMP_NEQ_OQ),
                    _mm_cmp_ps(this->values1, other.values, _CMP_NEQ_OQ));
            } else {
                return Mask(_mm_cmpneq_ps(this->values0, other.values), _mm_cmpneq_ps(this->values1, other.values));
            }
        } else
#endif
        {
            return Mask(this->values0 != other.value, this->values1 != other.value, this->values2 != other.value,
                this->values3 != other.value, this->values4 != other.value, this->values5 != other.value);
        }
    }

    /**
     * Negate this object based on the sign of another.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @param other The second object whose sign to check.
     * @returns Result of operation.
     */
    XS_INLINE SIMD3x2 sign(const SIMD3x2& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3x2(_mm256_xor_ps(this->values, _mm256_and_ps(other.values, _mm256_set1_ps(-0.0f))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3x2(_mm_xor_ps(this->values0, _mm_and_ps(_mm_set1_ps(-0.0f), other.values0)),
                _mm_xor_ps(this->values1, _mm_and_ps(_mm_set1_ps(-0.0f), other.values1)));
        } else
#endif
        {
            return SIMD3x2(Shift::sign<T>(this->values0, other.values0), Shift::sign<T>(this->values1, other.values1),
                Shift::sign<T>(this->values2, other.values2), Shift::sign<T>(this->values3, other.values3),
                Shift::sign<T>(this->values4, other.values4), Shift::sign<T>(this->values5, other.values5));
        }
    }

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_INLINE SIMD3x2 abs() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3x2(_mm256_andnot_ps(_mm256_set1_ps(-0.0f), this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 abs = _mm_set1_ps(-0.0f);
            return SIMD3x2(_mm_andnot_ps(abs, this->values0), _mm_andnot_ps(abs, this->values1));
        } else
#endif
        {
            return SIMD3x2(Shift::abs<T>(this->values0), Shift::abs<T>(this->values1), Shift::abs<T>(this->values2),
                Shift::abs<T>(this->values3), Shift::abs<T>(this->values4), Shift::abs<T>(this->values5));
        }
    }

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_INLINE SIMD3x2 max(const SIMD3x2& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3x2(_mm256_max_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3x2(_mm_max_ps(this->values0, other.values0), _mm_max_ps(this->values1, other.values1));
        } else
#endif
        {
            return SIMD3x2(Shift::max<T>(this->values0, other.values0), Shift::max<T>(this->values1, other.values1),
                Shift::max<T>(this->values2, other.values2), Shift::max<T>(this->values3, other.values3),
                Shift::max<T>(this->values4, other.values4), Shift::max<T>(this->values5, other.values5));
        }
    }

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_INLINE SIMD3x2 min(const SIMD3x2& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3x2(_mm256_min_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3x2(_mm_min_ps(this->values0, other.values0), _mm_min_ps(this->values1, other.values1));
        } else
#endif
        {
            return SIMD3x2(Shift::min<T>(this->values0, other.values0), Shift::min<T>(this->values1, other.values1),
                Shift::min<T>(this->values2, other.values2), Shift::min<T>(this->values3, other.values3),
                Shift::min<T>(this->values4, other.values4), Shift::min<T>(this->values5, other.values5));
        }
    }

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_INLINE SIMD3x2 max(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3x2(_mm256_max_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3x2(_mm_max_ps(this->values0, other.values), _mm_max_ps(this->values1, other.values));
        } else
#endif
        {
            return SIMD3x2(Shift::max<T>(this->values0, other.value), Shift::max<T>(this->values1, other.value),
                Shift::max<T>(this->values2, other.value), Shift::max<T>(this->values3, other.value),
                Shift::max<T>(this->values4, other.value), Shift::max<T>(this->values5, other.value));
        }
    }

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_INLINE SIMD3x2 min(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3x2(_mm256_min_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3x2(_mm_min_ps(this->values0, other.values), _mm_min_ps(this->values1, other.values));
        } else
#endif
        {
            return SIMD3x2(Shift::min<T>(this->values0, other.value), Shift::min<T>(this->values1, other.value),
                Shift::min<T>(this->values2, other.value), Shift::min<T>(this->values3, other.value),
                Shift::min<T>(this->values4, other.value), Shift::min<T>(this->values5, other.value));
        }
    }

    /**
     * Maximum of each internally stored SIMD3.
     * @returns SIMD3 set to the largest value in each internal SIMD3.
     */
    XS_INLINE SIMD3Def max3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3Def(_mm_max_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3Def(_mm_max_ps(this->values0, this->values1));
        } else
#endif
        {
            return SIMD3Def(Shift::max<T>(this->values0, this->values3), Shift::max<T>(this->values1, this->values4),
                Shift::max<T>(this->values2, this->values5));
        }
    }

    /**
     * Minimum of each internally stored SIMD3.
     * @returns SIMD3 set to the largest value in each internal SIMD3.
     */
    XS_INLINE SIMD3Def min3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3Def(_mm_min_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3Def(_mm_min_ps(this->values0, this->values1));
        } else
#endif
        {
            return SIMD3Def(Shift::min<T>(this->values0, this->values3), Shift::min<T>(this->values1, this->values4),
                Shift::min<T>(this->values2, this->values5));
        }
    }

    /**
     * Maximum element from each SIMD3.
     * @returns SIMD2 set to the largest value in each internal SIMD3.
     */
    XS_INLINE SIMD2Def hmax3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m128 val0 = _mm256_extractf128_ps(this->values, 1);
            __m128 val1 = _mm_unpacklo_ps(_mm256_castps256_ps128(this->values), val0);
            val1 = _mm_max_ps(val1, _mm_movehl_ps(val1, val1));
            val0 = _mm_unpackhi_ps(_mm256_castps256_ps128(this->values), val0);
            return SIMD2Def(_mm_max_ps(val1, val0));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_unpacklo_ps(this->values0, this->values1);
            val0 = _mm_max_ps(val0, _mm_movehl_ps(val0, val0));
            const __m128 val1 = _mm_unpackhi_ps(this->values0, this->values1);
            return SIMD2Def(_mm_max_ps(val0, val1));
        } else
#endif
        {
            const T other1 = Shift::max<T>(this->values0, this->values1);
            const T other2 = Shift::max<T>(this->values3, this->values4);
            return SIMD2Def(Shift::max<T>(this->values2, other1), Shift::max<T>(this->values5, other2));
        }
    }

    /**
     * Minimum element from each SIMD3.
     * @returns SIMD2 set to the smallest value in each internal SIMD3.
     */
    XS_INLINE SIMD2Def hmin3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m128 val0 = _mm256_extractf128_ps(this->values, 1);
            __m128 val1 = _mm_unpacklo_ps(_mm256_castps256_ps128(this->values), val0);
            val1 = _mm_min_ps(val1, _mm_movehl_ps(val1, val1));
            val0 = _mm_unpackhi_ps(_mm256_castps256_ps128(this->values), val0);
            return SIMD2Def(_mm_min_ps(val1, val0));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_unpacklo_ps(this->values0, this->values1);
            val0 = _mm_min_ps(val0, _mm_movehl_ps(val0, val0));
            const __m128 val1 = _mm_unpackhi_ps(this->values0, this->values1);
            return SIMD2Def(_mm_min_ps(val0, val1));
        } else
#endif
        {
            const T other1 = Shift::min<T>(this->values0, this->values1);
            const T other2 = Shift::min<T>(this->values3, this->values4);
            return SIMD2Def(Shift::min<T>(this->values2, other1), Shift::min<T>(this->values5, other2));
        }
    }

    /**
     * Compute the sum of each internally stored SIMD3.
     * @returns SIMD3 set to the sum of all elements between each internal SIMD3.
     */
    XS_INLINE SIMD3Def add3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3Def(_mm_add_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3Def(_mm_add_ps(this->values0, this->values1));
        } else
#endif
        {
            const T res1 = (this->values0 + this->values3);
            const T res2 = (this->values1 + this->values4);
            const T res3 = (this->values2 + this->values5);
            return SIMD3Def(res1, res2, res3);
        }
    }

    /**
     * Compute the difference of each internally stored SIMD3.
     * @returns SIMD3 set to the difference of all elements between each internal SIMD3.
     */
    XS_INLINE SIMD3Def sub3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3Def(_mm_sub_ps(_mm256_castps256_ps128(this->values), _mm256_extractf128_ps(this->values, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3Def(_mm_sub_ps(this->values0, this->values1));
        } else
#endif
        {
            const T res1 = (this->values0 - this->values3);
            const T res2 = (this->values1 - this->values4);
            const T res3 = (this->values2 - this->values5);
            return SIMD3Def(res1, res2, res3);
        }
    }

    /**
     * Compute the sum of each SIMD3.
     * @returns SIMD4 set to the sum of all elements in each SIMD3.
     */
    XS_INLINE SIMD2Def hadd3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m128 val0 = _mm256_extractf128_ps(this->values, 1);
            __m128 val1 = _mm_unpacklo_ps(_mm256_castps256_ps128(this->values), val0);
            val1 = _mm_add_ps(val1, _mm_movehl_ps(val1, val1));
            val0 = _mm_unpackhi_ps(_mm256_castps256_ps128(this->values), val0);
            return SIMD2Def(_mm_add_ps(val1, val0));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_unpacklo_ps(this->values0, this->values1);
            val0 = _mm_add_ps(val0, _mm_movehl_ps(val0, val0));
            const __m128 val1 = _mm_unpackhi_ps(this->values0, this->values1);
            return SIMD2Def(_mm_add_ps(val0, val1));
        } else
#endif
        {
            T res = (this->values0 + this->values1);
            T res2 = (this->values3 + this->values4);
            res += this->values2;
            res2 += this->values5;
            return SIMD2Def(res, res2);
        }
    }

    /**
     * Compute the 3 dimensional dot product of each SIMD3 in two objects.
     * @param other The second object.
     * @returns The value of the dot products.
     */
    XS_INLINE SIMD2Def dot3(const SIMD3x2& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 sq = _mm256_mul_ps(this->values, other.values);
            __m128 val0 = _mm256_extractf128_ps(sq, 1);
            __m128 val1 = _mm_unpacklo_ps(_mm256_castps256_ps128(sq), val0);
            val1 = _mm_add_ps(val1, _mm_movehl_ps(val1, val1));
            val0 = _mm_unpackhi_ps(_mm256_castps256_ps128(sq), val0);
            return SIMD2Def(_mm_add_ps(val1, val0));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 sq0 = _mm_mul_ps(this->values0, other.values0);
            const __m128 sq1 = _mm_mul_ps(this->values1, other.values1);
            __m128 val0 = _mm_unpacklo_ps(sq0, sq1);
            val0 = _mm_add_ps(val0, _mm_movehl_ps(val0, val0));
            const __m128 val1 = _mm_unpackhi_ps(sq0, sq1);
            return SIMD2Def(_mm_add_ps(val0, val1));
        } else
#endif
        {
            T sq0 = this->values0 * other.values0;
            T sq1 = this->values3 * other.values3;
            sq0 = Shift::fma<T>(this->values1, other.values1, sq0);
            sq1 = Shift::fma<T>(this->values4, other.values4, sq1);
            return SIMD2Def(
                Shift::fma<T>(this->values2, other.values2, sq0), Shift::fma<T>(this->values5, other.values5, sq1));
        }
    }

    /**
     * Compute the 3 dimensional cross product of each SIMD3 in two objects.
     * @note Uses Right-handed coordinate system.
     * @param other The second object.
     * @returns The cross product of the inputs.
     */
    XS_INLINE SIMD3x2 cross3(const SIMD3x2& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val0 = _mm256_shuffle1_ps(other.values, _MM_SHUFFLE(3, 0, 2, 1));
            val0 = _mm256_mul_ps(val0, this->values);
            __m256 val1 = _mm256_shuffle1_ps(this->values, _MM_SHUFFLE(3, 0, 2, 1));
            val1 = _mm256_mul_ps(val1, other.values);
            val0 = _mm256_sub_ps(val0, val1);
            return SIMD3x2(_mm256_shuffle1_ps(val0, _MM_SHUFFLE(3, 0, 2, 1)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val01 = _mm_shuffle1_ps(other.values0, _MM_SHUFFLE(3, 0, 2, 1));
            __m128 val11 = _mm_shuffle1_ps(other.values1, _MM_SHUFFLE(3, 0, 2, 1));
            val01 = _mm_mul_ps(val01, this->values0);
            val11 = _mm_mul_ps(val11, this->values1);
            __m128 val02 = _mm_shuffle1_ps(this->values0, _MM_SHUFFLE(3, 0, 2, 1));
            __m128 val12 = _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(3, 0, 2, 1));
            val02 = _mm_mul_ps(val02, other.values0);
            val12 = _mm_mul_ps(val12, other.values1);
            val01 = _mm_sub_ps(val01, val02);
            val11 = _mm_sub_ps(val11, val12);
            return SIMD3x2(
                _mm_shuffle1_ps(val01, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle1_ps(val11, _MM_SHUFFLE(3, 0, 2, 1)));
        } else
#endif
        {
            const T v0CrossX = -(this->values2 * other.values1);
            const T v1CrossX = -(this->values5 * other.values4);
            const T v0CrossY = -(this->values0 * other.values2);
            const T v1CrossY = -(this->values3 * other.values5);
            const T v0CrossZ = -(this->values1 * other.values0);
            const T v1CrossZ = -(this->values4 * other.values3);

            return SIMD3x2(Shift::fma<T>(this->values1, other.values2, v0CrossX),
                Shift::fma<T>(this->values2, other.values0, v0CrossY),
                Shift::fma<T>(this->values0, other.values1, v0CrossZ),
                Shift::fma<T>(this->values4, other.values5, v1CrossX),
                Shift::fma<T>(this->values5, other.values3, v1CrossY),
                Shift::fma<T>(this->values3, other.values4, v1CrossZ));
        }
    }

    /**
     * Compute the square of the length of each internal SIMD3.
     * @returns Squared lengths.
     */
    XS_INLINE SIMD2Def lengthSqr3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 sq = _mm256_mul_ps(this->values, this->values);
            __m128 val0 = _mm256_extractf128_ps(sq, 1);
            __m128 val1 = _mm_unpacklo_ps(_mm256_castps256_ps128(sq), val0);
            val1 = _mm_add_ps(val1, _mm_movehl_ps(val1, val1));
            val0 = _mm_unpackhi_ps(_mm256_castps256_ps128(sq), val0);
            return SIMD2Def(_mm_add_ps(val1, val0));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 sq0 = _mm_mul_ps(this->values0, this->values0);
            const __m128 sq1 = _mm_mul_ps(this->values1, this->values1);
            __m128 val0 = _mm_unpacklo_ps(sq0, sq1);
            val0 = _mm_add_ps(val0, _mm_movehl_ps(val0, val0));
            const __m128 val1 = _mm_unpackhi_ps(sq0, sq1);
            return SIMD2Def(_mm_add_ps(val0, val1));
        } else
#endif
        {
            T sq0 = this->values0 * this->values0;
            T sq1 = this->values3 * this->values3;
            sq0 = Shift::fma<T>(this->values1, this->values1, sq0);
            sq1 = Shift::fma<T>(this->values4, this->values4, sq1);
            return SIMD2Def(
                Shift::fma<T>(this->values2, this->values2, sq0), Shift::fma<T>(this->values5, this->values5, sq1));
        }
    }

    /**
     * Compute the length of each internal SIMD3.
     * @returns The length.
     */
    XS_INLINE SIMD2Def length3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 sq = _mm256_mul_ps(this->values, this->values);
            __m128 val0 = _mm256_extractf128_ps(sq, 1);
            __m128 val1 = _mm_unpacklo_ps(_mm256_castps256_ps128(sq), val0);
            val1 = _mm_add_ps(val1, _mm_movehl_ps(val1, val1));
            val0 = _mm_unpackhi_ps(_mm256_castps256_ps128(sq), val0);
            return SIMD2Def(_mm_sqrt_ps(_mm_add_ps(val1, val0)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 sq0 = _mm_mul_ps(this->values0, this->values0);
            const __m128 sq1 = _mm_mul_ps(this->values1, this->values1);
            __m128 val0 = _mm_unpacklo_ps(sq0, sq1);
            val0 = _mm_add_ps(val0, _mm_movehl_ps(val0, val0));
            const __m128 val1 = _mm_unpackhi_ps(sq0, sq1);
            return SIMD2Def(_mm_sqrt_ps(_mm_add_ps(val0, val1)));
        } else
#endif
        {
            T sq0 = this->values0 * this->values0;
            T sq1 = this->values3 * this->values3;
            sq0 = Shift::fma<T>(this->values1, this->values1, sq0);
            sq1 = Shift::fma<T>(this->values4, this->values4, sq1);
            return SIMD2Def(Shift::sqrt<T>(Shift::fma<T>(this->values2, this->values2, sq0)),
                Shift::sqrt<T>(Shift::fma<T>(this->values5, this->values5, sq1)));
        }
    }

    /**
     * Normalise each internal SIMD3.
     * @note The result is unpredictable when all elements are at or near zero.
     * @returns The normalised values.
     */
    XS_INLINE SIMD3x2 normalize3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 sq = _mm256_dp_ps(this->values, this->values, 0x7F);
            return SIMD3x2(_mm256_div_ps(this->values, _mm256_sqrt_ps(sq)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 sq0 = _mm_mul_ps(this->values0, this->values0);
            const __m128 sq1 = _mm_mul_ps(this->values1, this->values1);
            __m128 val0 = _mm_unpacklo_ps(sq0, sq1);
            val0 = _mm_add_ps(val0, _mm_movehl_ps(val0, val0));
            const __m128 val1 = _mm_unpackhi_ps(sq0, sq1);
            val0 = _mm_sqrt_ps(_mm_add_ps(val0, val1));
            return SIMD3x2(_mm_div_ps(this->values0, _mm_shuffle0000_ps(val0)),
                _mm_div_ps(this->values1, _mm_shuffle1111_ps(val0)));
        } else
#endif
        {
            T sq0 = this->values0 * this->values0;
            T sq1 = this->values3 * this->values3;
            sq0 = Shift::fma<T>(this->values1, this->values1, sq0);
            sq1 = Shift::fma<T>(this->values4, this->values4, sq1);
            sq0 = Shift::sqrt<T>(Shift::fma<T>(this->values2, this->values2, sq0));
            sq1 = Shift::sqrt<T>(Shift::fma<T>(this->values5, this->values5, sq1));
            return SIMD3x2(this->values0 / sq0, this->values1 / sq0, this->values2 / sq0, this->values3 / sq1,
                this->values4 / sq1, this->values5 / sq1);
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
    XS_INLINE SIMD3x2 insert3(const SIMD3x2& other) const noexcept
    {
        static_assert(Index0 < 3 && Index1 < 3, "Invalid Index: Indexes must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == Index1) {
                return SIMD3x2(_mm256_blend_ps(this->values, other.values, (1UL << Index0) | (1UL << (Index0 + 4))));
            } else if constexpr (Index0 == 0) {
                const __m256 val =
                    _mm256_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index1, Index1, 1, 1)); /*(x,Index1,x,1)*/
                return SIMD3x2(_mm256_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 0, 2)));
            } else if constexpr (Index0 == 1) {
                const __m256 val =
                    _mm256_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index1, Index1, 0, 0)); /*(x,Index1,x,0)*/
                return SIMD3x2(_mm256_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 2, 0)));
            } else /*Index0 == 2*/ {
                const __m256 val =
                    _mm256_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index1, Index1, 3, 3)); /*(x,Index1,x,3)*/
                return SIMD3x2(_mm256_shuffle_ps(this->values, val, _MM_SHUFFLE(0, 2, 1, 0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD3x2(_mm_blend_ss(this->values0, other.values0), _mm_blend_ss(this->values1, other.values1));
            } else if constexpr (Index0 == Index1) {
                return SIMD3x2(_mm_blend_ps(this->values0, other.values0, 1UL << Index0),
                    _mm_blend_ps(this->values1, other.values1, 1UL << Index0));
            } else {
                return SIMD3x2(_mm_insert_ps(this->values0, other.values0, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)),
                    _mm_insert_ps(this->values1, other.values1, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)));
            }
        } else
#endif
        {
            return SIMD3x2((Index0 == 0) ? (&other.values0)[Index1] : this->values0,
                (Index0 == 1) ? (&other.values0)[Index1] : this->values1,
                (Index0 == 2) ? (&other.values0)[Index1] : this->values2,
                (Index0 == 0) ? (&other.values0)[Index1 + 3] : this->values3,
                (Index0 == 1) ? (&other.values0)[Index1 + 3] : this->values4,
                (Index0 == 2) ? (&other.values0)[Index1 + 3] : this->values5);
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
    XS_INLINE SIMD3x2 blend3(const SIMD3x2& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2) {
            return *this;
        } else if constexpr (Elem0 && Elem1 && Elem2) {
            return other;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD3x2(_mm256_blend_ps(
                this->values, other.values, _MM256_BLEND(0, Elem2, Elem1, Elem0, 0, Elem2, Elem1, Elem0)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Elem0 && !Elem1 && !Elem2) {
                return SIMD3x2(_mm_blend_ss(this->values0, other.values0), _mm_blend_ss(this->values1, other.values1));
            } else if constexpr (!Elem0 && Elem1 && Elem2) {
                return SIMD3x2(_mm_blend_ss(other.values0, this->values0), _mm_blend_ss(other.values1, this->values1));
            } else {
                return SIMD3x2(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(0, Elem2, Elem1, Elem0)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(0, Elem2, Elem1, Elem0)));
            }
        }
#endif
        else {
            const auto temp0 =
                (Elem0) ? SIMD2Def(other.values0, other.values3) : SIMD2Def(this->values0, this->values3);
            const auto temp1 =
                (Elem1) ? SIMD2Def(other.values1, other.values4) : SIMD2Def(this->values1, this->values4);
            const auto temp2 =
                (Elem2) ? SIMD2Def(other.values2, other.values5) : SIMD2Def(this->values2, this->values5);
            return SIMD3x2(temp0.values0, temp1.values0, temp2.values0, temp0.values1, temp1.values1, temp2.values1);
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
    XS_INLINE SIMD3x2 shuffle3() const noexcept
    {
        static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3, "Invalid Index: Indexes must be <3");
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
                return SIMD3x2(_mm256_shuffle2200_ps(this->values));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
                return SIMD3x2(_mm256_shuffle1100_ps(this->values));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
                return SIMD3x2(_mm256_shuffle1010_ps(this->values));
            } else {
                return SIMD3x2(_mm256_shuffle1_ps(this->values, _MM_SHUFFLE(0, Index2, Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
                return SIMD3x2(_mm_shuffle1010_ps(this->values0), _mm_shuffle1010_ps(this->values1));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
                return SIMD3x2(_mm_shuffle1100_ps(this->values0), _mm_shuffle1100_ps(this->values1));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
                return SIMD3x2(_mm_shuffle2200_ps(this->values0), _mm_shuffle2200_ps(this->values1));
            } else {
                return SIMD3x2(_mm_shuffle1_ps(this->values0, _MM_SHUFFLE(3, Index2, Index1, Index0)),
                    _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(3, Index2, Index1, Index0)));
            }
        }
#endif
        else {
            return SIMD3x2((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2],
                (&this->values0)[Index0 + 3], (&this->values0)[Index1 + 3], (&this->values0)[Index2 + 3]);
        }
    }

    /**
     * Shuffles the elements of the object by shuffling each SIMD3.
     * @tparam Index0 The index of the first SIMD3 to insert into the returned object (range is 0-1).
     * @tparam Index1 The index of the second SIMD3 to insert into the returned object (range is 0-1).
     * @returns Result of operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_INLINE SIMD3x2 shuffleH3() const noexcept
    {
        static_assert(Index0 < 2 && Index1 < 2, "Invalid Index: Indexes must be <2");
        if constexpr (Index0 == 0 && Index1 == 1) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD3x2(_mm256_shuffle32103210_ps(this->values));
            } else {
                return SIMD3x2(_mm256_permute2f128_ps(this->values, this->values, _MM256_PERMUTE(Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3x2((&this->values0)[Index0], (&this->values0)[Index1]);
        }
#endif
        else {
            return SIMD3x2(getValue3<Index0>(), getValue3<Index1>());
        }
    }
};

/**
 * Add two SIMD3x2s.
 * @param other1 The first SIMD3x2.
 * @param other2 SIMD3x2 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator+(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(
            _mm_add_ps(other1.values0, other2.values0), _mm_add_ps(other1.values1, other2.values1));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values3, other1.values4 + other2.values4,
            other1.values5 + other2.values5);
    }
}

/**
 * Add a value to all elements of a SIMD3x2.
 * @param other1 The SIMD3x2.
 * @param other2 Value to add to the SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator+(
    const SIMD3x2<T, Width>& other1, typename SIMD3x2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 + other2.value, other1.values1 + other2.value,
            other1.values2 + other2.value, other1.values3 + other2.value, other1.values4 + other2.value,
            other1.values5 + other2.value);
    }
}

/**
 * Add a SIMD3 to each triple of elements in a SIMD3x2.
 * @param other1 The SIMD3x2.
 * @param other2 Values to add to the SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator+(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_add_ps(other1.values, _mm256_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values0, other1.values4 + other2.values1,
            other1.values5 + other2.values2);
    }
}

/**
 * Subtract a SIMD3x2 from another SIMD3x2.
 * @param other1 The first SIMD3x2.
 * @param other2 SIMD3x2 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator-(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(
            _mm_sub_ps(other1.values0, other2.values0), _mm_sub_ps(other1.values1, other2.values1));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values3, other1.values4 - other2.values4,
            other1.values5 - other2.values5);
    }
}

/**
 * Subtract a value from all elements of a SIMD3x2.
 * @param other1 The SIMD3x2.
 * @param other2 Value to subtract from the SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator-(
    const SIMD3x2<T, Width>& other1, typename SIMD3x2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 - other2.value, other1.values1 - other2.value,
            other1.values2 - other2.value, other1.values3 - other2.value, other1.values4 - other2.value,
            other1.values5 - other2.value);
    }
}

/**
 * Subtract all elements of a SIMD3x2 from a value.
 * @param other1 Value to subtract the SIMD3x2 from.
 * @param other2 The SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator-(
    typename SIMD3x2<T, Width>::BaseDef other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_sub_ps(other1.values, other2.values0), _mm_sub_ps(other1.values, other2.values1));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.value - other2.values0, other1.value - other2.values1,
            other1.value - other2.values2, other1.value - other2.values3, other1.value - other2.values4,
            other1.value - other2.values5);
    }
}

/**
 * Subtract a SIMD3 from each triple of elements in a SIMD3x2.
 * @param other1 The SIMD3x2.
 * @param other2 Values to add to the SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator-(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_sub_ps(other1.values, _mm256_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values0, other1.values4 - other2.values1,
            other1.values5 - other2.values2);
    }
}

/**
 * Multiply two SIMD3x2s per element.
 * @param other1 The first SIMD3x2.
 * @param other2 The second SIMD3x2 to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator*(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(
            _mm_mul_ps(other1.values0, other2.values0), _mm_mul_ps(other1.values1, other2.values1));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values3, other1.values4 * other2.values4,
            other1.values5 * other2.values5);
    }
}

/**
 * Multiply a SIMD3x2 by a scalar.
 * @param other1 The SIMD3x2.
 * @param other2 Value to multiply the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator*(
    const SIMD3x2<T, Width>& other1, typename SIMD3x2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 * other2.value, other1.values1 * other2.value,
            other1.values2 * other2.value, other1.values3 * other2.value, other1.values4 * other2.value,
            other1.values5 * other2.value);
    }
}

/**
 * Multiply a SIMD3x2 by a SIMD2 component wise.
 * @param other1 The SIMD3x2.
 * @param other2 Value to multiply the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator*(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        return SIMD3x2<T, Width>(_mm256_mul_ps(other1.values, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle0000_ps(other2.values);
        const __m128 val1 = _mm_shuffle1111_ps(other2.values);
        return SIMD3x2<T, Width>(_mm_mul_ps(other1.values0, val0), _mm_mul_ps(other1.values1, val1));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values0,
            other1.values2 * other2.values0, other1.values3 * other2.values1, other1.values4 * other2.values1,
            other1.values5 * other2.values1);
    }
}

/**
 * Multiply each triple of elements in a SIMD3x2 by a SIMD3.
 * @param other1 The SIMD3x2.
 * @param other2 Values to multiply the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator*(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_mul_ps(other1.values, _mm256_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values0, other1.values4 * other2.values1,
            other1.values5 * other2.values2);
    }
}

/**
 * Divide two SIMD3x2s per element.
 * @param other1 The first SIMD3x2.
 * @param other2 The second SIMD3x2 to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator/(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(
            _mm_div_ps(other1.values0, other2.values0), _mm_div_ps(other1.values1, other2.values1));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values3, other1.values4 / other2.values4,
            other1.values5 / other2.values5);
    }
}

/**
 * Divide a SIMD3x2 by a scalar.
 * @param other1 The SIMD3x2.
 * @param other2 Value to divide the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator/(
    const SIMD3x2<T, Width>& other1, typename SIMD3x2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 / other2.value, other1.values1 / other2.value,
            other1.values2 / other2.value, other1.values3 / other2.value, other1.values4 / other2.value,
            other1.values5 / other2.value);
    }
}

/**
 * Divide a SIMD3x2 by a SIMD2 component wise.
 * @param other1 The SIMD3x2.
 * @param other2 Value to divide the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator/(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        return SIMD3x2<T, Width>(_mm256_div_ps(other1.values, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle0000_ps(other2.values);
        const __m128 val1 = _mm_shuffle1111_ps(other2.values);
        return SIMD3x2<T, Width>(_mm_div_ps(other1.values0, val0), _mm_div_ps(other1.values1, val1));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values0,
            other1.values2 / other2.values0, other1.values3 / other2.values1, other1.values4 / other2.values1,
            other1.values5 / other2.values1);
    }
}

/**
 * Divide a scalar by all elements of a SIMD3x2.
 * @param other1 Value to divide the SIMD3x2 by.
 * @param other2 The SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator/(
    typename SIMD3x2<T, Width>::BaseDef other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_div_ps(other1.values, other2.values0), _mm_div_ps(other1.values, other2.values1));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.value / other2.values0, other1.value / other2.values1,
            other1.value / other2.values2, other1.value / other2.values3, other1.value / other2.values4,
            other1.value / other2.values5);
    }
}

/**
 * Divide each triple of elements in a SIMD3x2 by a SIMD3.
 * @param other1 The SIMD3x2.
 * @param other2 Values to divide the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator/(
    const SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_div_ps(other1.values, _mm256_broadcastf128_ps(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD3x2<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values0, other1.values4 / other2.values1,
            other1.values5 / other2.values2);
    }
}

/**
 * Negate all elements of a SIMD3x2.
 * @param other The SIMD3x2 to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator-(const SIMD3x2<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2<T, Width>(_mm256_sub_ps(_mm256_setzero_ps(), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_setzero_ps();
        return SIMD3x2<T, Width>(_mm_sub_ps(val, other.values0), _mm_sub_ps(val, other.values1));
    } else
#endif
    {
        return SIMD3x2<T, Width>(
            -other.values0, -other.values1, -other.values2, -other.values3, -other.values4, -other.values5);
    }
}

/**
 * Perform compound assignment and addition with a SIMD3x2.
 * @param [in,out] other1 The first SIMD3x2.
 * @param          other2 SIMD3x2 to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator+=(SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_add_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, other2.values0);
        other1.values1 = _mm_add_ps(other1.values1, other2.values1);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
        other1.values2 += other2.values2;
        other1.values3 += other2.values3;
        other1.values4 += other2.values4;
        other1.values5 += other2.values5;
    }
    return other1;
}

/**
 * Perform compound assignment and addition with a SIMD3x2.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Value to add to the first SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator+=(SIMD3x2<T, Width>& other1, typename SIMD3x2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_add_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, other2.values);
        other1.values1 = _mm_add_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 += other2.value;
        other1.values1 += other2.value;
        other1.values2 += other2.value;
        other1.values3 += other2.value;
        other1.values4 += other2.value;
        other1.values5 += other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and addition of a SIMD3 to each triple of elements in a SIMD3x2.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Values to add to the SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator+=(
    SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_add_ps(other1.values, _mm256_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_add_ps(other1.values0, other2.values);
        other1.values1 = _mm_add_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
        other1.values2 += other2.values2;
        other1.values3 += other2.values0;
        other1.values4 += other2.values1;
        other1.values5 += other2.values2;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction by a SIMD3x2.
 * @param [in,out] other1 The first SIMD3x2.
 * @param          other2 SIMD3x2 to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator-=(SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_sub_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, other2.values0);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values1);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
        other1.values2 -= other2.values2;
        other1.values3 -= other2.values3;
        other1.values4 -= other2.values4;
        other1.values5 -= other2.values5;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction with a SIMD3x2.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Value to subtract from the first SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator-=(SIMD3x2<T, Width>& other1, typename SIMD3x2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_sub_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, other2.values);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 -= other2.value;
        other1.values1 -= other2.value;
        other1.values2 -= other2.value;
        other1.values3 -= other2.value;
        other1.values4 -= other2.value;
        other1.values5 -= other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction of a SIMD3 from each triple of elements in a SIMD3x2.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Values to subtract from the SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator-=(
    SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_sub_ps(other1.values, _mm256_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_sub_ps(other1.values0, other2.values);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
        other1.values2 -= other2.values2;
        other1.values3 -= other2.values0;
        other1.values4 -= other2.values1;
        other1.values5 -= other2.values2;
    }
    return other1;
}

/**
 * Perform SIMD3x2 compound assignment and multiplication.
 * @param [in,out] other1 The first SIMD3x2.
 * @param          other2 SIMD3x2 to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator*=(SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_mul_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, other2.values0);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values1);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
        other1.values2 *= other2.values2;
        other1.values3 *= other2.values3;
        other1.values4 *= other2.values4;
        other1.values5 *= other2.values5;
    }
    return other1;
}

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Value to multiply the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator*=(SIMD3x2<T, Width>& other1, typename SIMD3x2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_mul_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, other2.values);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 *= other2.value;
        other1.values1 *= other2.value;
        other1.values2 *= other2.value;
        other1.values3 *= other2.value;
        other1.values4 *= other2.value;
        other1.values5 *= other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and multiplication by a SIMD2 component wise.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Value to multiply the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator*=(
    SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        other1.values = _mm256_mul_ps(other1.values, val);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle0000_ps(other2.values);
        const __m128 val1 = _mm_shuffle1111_ps(other2.values);
        other1.values0 = _mm_mul_ps(other1.values0, val0);
        other1.values1 = _mm_mul_ps(other1.values1, val1);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values0;
        other1.values2 *= other2.values0;
        other1.values3 *= other2.values1;
        other1.values4 *= other2.values1;
        other1.values5 *= other2.values1;
    }
    return other1;
}

/**
 * Perform compound assignment and multiplication of each triple of elements in a SIMD3x2 by a SIMD3.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Values to multiply the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator*=(
    SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_mul_ps(other1.values, _mm256_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_mul_ps(other1.values0, other2.values);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
        other1.values2 *= other2.values2;
        other1.values3 *= other2.values0;
        other1.values4 *= other2.values1;
        other1.values5 *= other2.values2;
    }
    return other1;
}

/**
 * Perform compound assignment and division.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Value to divide the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator/=(SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_div_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, other2.values0);
        other1.values1 = _mm_div_ps(other1.values1, other2.values1);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
        other1.values2 /= other2.values2;
        other1.values3 /= other2.values3;
        other1.values4 /= other2.values4;
        other1.values5 /= other2.values5;
    }
    return other1;
}

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Value to divide the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator/=(SIMD3x2<T, Width>& other1, typename SIMD3x2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_div_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, other2.values);
        other1.values1 = _mm_div_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 /= other2.value;
        other1.values1 /= other2.value;
        other1.values2 /= other2.value;
        other1.values3 /= other2.value;
        other1.values4 /= other2.value;
        other1.values5 /= other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and division by a SIMD2 component wise.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Value to divide the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator/=(
    SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set_m128(_mm_shuffle1111_ps(other2.values), _mm_shuffle0000_ps(other2.values));
        other1.values = _mm256_div_ps(other1.values, val);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle0000_ps(other2.values);
        const __m128 val1 = _mm_shuffle1111_ps(other2.values);
        other1.values0 = _mm_div_ps(other1.values0, val0);
        other1.values1 = _mm_div_ps(other1.values1, val1);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values0;
        other1.values2 /= other2.values0;
        other1.values3 /= other2.values1;
        other1.values4 /= other2.values1;
        other1.values5 /= other2.values1;
    }
    return other1;
}

/**
 * Perform compound assignment and division of each triple of elements in a SIMD3x2 by a SIMD3.
 * @param [in,out] other1 The SIMD3x2.
 * @param          other2 Values to divide the SIMD3x2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width>& operator/=(
    SIMD3x2<T, Width>& other1, const typename SIMD3x2<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_div_ps(other1.values, _mm256_broadcastf128_ps(other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values0 = _mm_div_ps(other1.values0, other2.values);
        other1.values1 = _mm_div_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
        other1.values2 /= other2.values2;
        other1.values3 /= other2.values0;
        other1.values4 /= other2.values1;
        other1.values5 /= other2.values2;
    }
    return other1;
}

/**
 * Compare two SIMD3x2s are equal.
 * @param other1 The first SIMD3x2.
 * @param other2 The second SIMD3x2 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator==(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NEQ_UQ);
        return ((_mm256_movemask_ps(val) & 0x77) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val0 = _mm_cmp_ps(other1.values0, other2.values0, _CMP_NEQ_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values1, _CMP_NEQ_UQ);
            val0 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(0, 2, 1, 0));
            val1 = _mm_shuffle1_ps(val1, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values0, other2.values0)) |
                (_mm_movemask_ps(_mm_cmpneq_ps(other1.values1, other2.values1)) & 7));
        }
    } else
#endif
    {
        return ((other1.values0 == other2.values0) & (other1.values1 == other2.values1) &
            (other1.values2 == other2.values2) & (other1.values3 == other2.values3) &
            (other1.values4 == other2.values4) & (other1.values5 == other2.values5));
    }
}

/**
 * Compare a SIMD3x2 to a value.
 * @param other1 The SIMD3x2.
 * @param other2 The value to compare to the SIMD3x2.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator==(const SIMD3x2<T, Width>& other1, typename SIMD3x2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NEQ_UQ);
        return ((_mm256_movemask_ps(val) & 0x77) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val0 = _mm_cmp_ps(other1.values0, other2.values, _CMP_NEQ_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values, _CMP_NEQ_UQ);
            val0 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(0, 2, 1, 0));
            val1 = _mm_shuffle1_ps(val1, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values0, other2.values)) |
                (_mm_movemask_ps(_mm_cmpneq_ps(other1.values1, other2.values)) & 7));
        }
    } else
#endif
    {
        return ((other1.values0 == other2.value) & (other1.values1 == other2.value) & (other1.values2 == other2.value) &
            (other1.values3 == other2.value) & (other1.values4 == other2.value) & (other1.values5 == other2.value));
    }
}

/**
 * Compare two SIMD3x2s are less or equal.
 * @param other1 The first SIMD3x2.
 * @param other2 The second SIMD3x2 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator<=(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ);
        return ((_mm256_movemask_ps(val) & 0x77) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val0 = _mm_cmp_ps(other1.values0, other2.values0, _CMP_NLE_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values1, _CMP_NLE_UQ);
            val0 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(0, 2, 1, 0));
            val1 = _mm_shuffle1_ps(val1, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values0, other2.values0)) |
                (_mm_movemask_ps(_mm_cmpnle_ps(other1.values1, other2.values1)) & 7));
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.values0) & (other1.values1 <= other2.values1) &
            (other1.values2 <= other2.values2) & (other1.values3 <= other2.values3) &
            (other1.values4 <= other2.values4) & (other1.values5 <= other2.values5));
    }
}

/**
 * Compare SIMD3x2 is less than or equal to a value.
 * @param other1 The SIMD3x2.
 * @param other2 The value to compare to the SIMD3x2.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator<=(const SIMD3x2<T, Width>& other1, typename SIMD3x2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ);
        return ((_mm256_movemask_ps(val) & 0x77) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val0 = _mm_cmp_ps(other1.values0, other2.values, _CMP_NLE_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values, _CMP_NLE_UQ);
            val0 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(0, 2, 1, 0));
            val1 = _mm_shuffle1_ps(val1, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values0, other2.values)) |
                (_mm_movemask_ps(_mm_cmpnle_ps(other1.values1, other2.values)) & 7));
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.value) & (other1.values1 <= other2.value) & (other1.values2 <= other2.value) &
            (other1.values3 <= other2.value) & (other1.values4 <= other2.value) & (other1.values5 <= other2.value));
    }
}

/**
 * Compare two SIMD3x2s are less than.
 * @param other1 The first SIMD3x2.
 * @param other2 The second SIMD3x2 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator<(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ);
        return ((_mm256_movemask_ps(val) & 0x77) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val0 = _mm_cmp_ps(other1.values0, other2.values0, _CMP_NLT_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values1, _CMP_NLT_UQ);
            val0 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(0, 2, 1, 0));
            val1 = _mm_shuffle1_ps(val1, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values0, other2.values0)) |
                (_mm_movemask_ps(_mm_cmpnlt_ps(other1.values1, other2.values1)) & 7));
        }
    } else
#endif
    {
        return ((other1.values0 < other2.values0) & (other1.values1 < other2.values1) &
            (other1.values2 < other2.values2) & (other1.values3 < other2.values3) & (other1.values4 < other2.values4) &
            (other1.values5 < other2.values5));
    }
}

/**
 * Compare SIMD3x2 is less than a value.
 * @param other1 The SIMD3x2.
 * @param other2 The value to compare to the SIMD3x2.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator<(const SIMD3x2<T, Width>& other1, typename SIMD3x2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ);
        return ((_mm256_movemask_ps(val) & 0x77) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val0 = _mm_cmp_ps(other1.values0, other2.values, _CMP_NLT_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values, _CMP_NLT_UQ);
            val0 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(0, 2, 1, 0));
            val1 = _mm_shuffle1_ps(val1, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values0, other2.values)) |
                (_mm_movemask_ps(_mm_cmpnlt_ps(other1.values1, other2.values)) & 7));
        }
    } else
#endif
    {
        return ((other1.values0 < other2.value) & (other1.values1 < other2.value) & (other1.values2 < other2.value) &
            (other1.values3 < other2.value) & (other1.values4 < other2.value) & (other1.values5 < other2.value));
    }
}

/**
 * Compare two SIMD3x2s are not equal.
 * @param other1 The first SIMD3x2.
 * @param other2 The second SIMD3x2 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator!=(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_EQ_OQ);
        return ((_mm256_movemask_ps(val) & 0x3F) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val0 = _mm_cmp_ps(other1.values0, other2.values0, _CMP_EQ_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values1, _CMP_EQ_UQ);
            val0 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(0, 2, 1, 0));
            val1 = _mm_shuffle1_ps(val1, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpeq_ps(other1.values0, other2.values0)) |
                (_mm_movemask_ps(_mm_cmpeq_ps(other1.values1, other2.values1)) & 3));
        }
    } else
#endif
    {
        return ((other1.values0 != other2.values0) & (other1.values1 != other2.values1) &
            (other1.values2 != other2.values2) & (other1.values3 != other2.values3) &
            (other1.values4 != other2.values4) & (other1.values5 != other2.values5));
    }
}

/**
 * Compare SIMD3x2 is not equal to a value.
 * @param other1 The SIMD3x2.
 * @param other2 The value to compare to the SIMD3x2.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator!=(const SIMD3x2<T, Width>& other1, typename SIMD3x2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_EQ_OQ);
        return ((_mm256_movemask_ps(val) & 0x3F) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val0 = _mm_cmp_ps(other1.values0, other2.values, _CMP_EQ_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values, _CMP_EQ_UQ);
            val0 = _mm_shuffle1_ps(val0, _MM_SHUFFLE(0, 2, 1, 0));
            val1 = _mm_shuffle1_ps(val1, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpeq_ps(other1.values0, other2.values)) |
                (_mm_movemask_ps(_mm_cmpeq_ps(other1.values1, other2.values)) & 3));
        }
    } else
#endif
    {
        return ((other1.values0 != other2.value) & (other1.values1 != other2.value) & (other1.values2 != other2.value) &
            (other1.values3 != other2.value) & (other1.values4 != other2.value) & (other1.values5 != other2.value));
    }
}

/**
 * And 2 SIMD3x2s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD3x2.
 * @param other2 SIMD3x2 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator&(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_and_ps(other1.values0, other2.values0), _mm_and_ps(other1.values1, other2.values1));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(other1.values0 & other2.values0, other1.values1 & other2.values1,
                other1.values2 & other2.values2, other1.values3 & other2.values3, other1.values4 & other2.values4,
                other1.values5 & other2.values5);
        } else {
            return SIMD3x2<T, Width>(Shift::bitAnd(other1.values0, other2.values0),
                Shift::bitAnd(other1.values1, other2.values1), Shift::bitAnd(other1.values2, other2.values2),
                Shift::bitAnd(other1.values3, other2.values3), Shift::bitAnd(other1.values4, other2.values4),
                Shift::bitAnd(other1.values5, other2.values5));
        }
    }
}

/**
 * And a SIMD3x2s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD3x2.
 * @param other2 Value to and with the SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator&(
    const SIMD3x2<T, Width>& other1, typename SIMD3x2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_and_ps(other1.values0, other2.values), _mm_and_ps(other1.values1, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(other1.values0 & other2.value, other1.values1 & other2.value,
                other1.values2 & other2.value, other1.values3 & other2.value, other1.values4 & other2.value,
                other1.values5 & other2.value);
        } else {
            return SIMD3x2<T, Width>(Shift::bitAnd(other1.values0, other2.value),
                Shift::bitAnd(other1.values1, other2.value), Shift::bitAnd(other1.values2, other2.value),
                Shift::bitAnd(other1.values3, other2.value), Shift::bitAnd(other1.values4, other2.value),
                Shift::bitAnd(other1.values5, other2.value));
        }
    }
}

/**
 * Or 2 SIMD3x2s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD3x2.
 * @param other2 SIMD3x2 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator|(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_or_ps(other1.values0, other2.values0), _mm_or_ps(other1.values1, other2.values1));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(other1.values0 | other2.values0, other1.values1 | other2.values1,
                other1.values2 | other2.values2, other1.values3 | other2.values3, other1.values4 | other2.values4,
                other1.values5 | other2.values5);
        } else {
            return SIMD3x2<T, Width>(Shift::bitOr(other1.values0, other2.values0),
                Shift::bitOr(other1.values1, other2.values1), Shift::bitOr(other1.values2, other2.values2),
                Shift::bitOr(other1.values3, other2.values3), Shift::bitOr(other1.values4, other2.values4),
                Shift::bitOr(other1.values5, other2.values5));
        }
    }
}

/**
 * Or a SIMD3x2s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD3x2.
 * @param other2 Value to and with the SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator|(
    const SIMD3x2<T, Width>& other1, typename SIMD3x2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_or_ps(other1.values0, other2.values), _mm_or_ps(other1.values1, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(other1.values0 | other2.value, other1.values1 | other2.value,
                other1.values2 | other2.value, other1.values3 | other2.value, other1.values4 | other2.value,
                other1.values5 | other2.value);
        } else {
            return SIMD3x2<T, Width>(Shift::bitOr(other1.values0, other2.value),
                Shift::bitOr(other1.values1, other2.value), Shift::bitOr(other1.values2, other2.value),
                Shift::bitOr(other1.values3, other2.value), Shift::bitOr(other1.values4, other2.value),
                Shift::bitOr(other1.values5, other2.value));
        }
    }
}

/**
 * Xor 2 SIMD3x2s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD3x2.
 * @param other2 SIMD3x2 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator^(const SIMD3x2<T, Width>& other1, const SIMD3x2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_xor_ps(other1.values0, other2.values0), _mm_xor_ps(other1.values1, other2.values1));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(other1.values0 ^ other2.values0, other1.values1 ^ other2.values1,
                other1.values2 ^ other2.values2, other1.values3 ^ other2.values3, other1.values4 ^ other2.values4,
                other1.values5 ^ other2.values5);
        } else {
            return SIMD3x2<T, Width>(Shift::bitXor(other1.values0, other2.values0),
                Shift::bitXor(other1.values1, other2.values1), Shift::bitXor(other1.values2, other2.values2),
                Shift::bitXor(other1.values3, other2.values3), Shift::bitXor(other1.values4, other2.values4),
                Shift::bitXor(other1.values5, other2.values5));
        }
    }
}

/**
 * Xor a SIMD3x2s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD3x2.
 * @param other2 Value to and with the SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator^(
    const SIMD3x2<T, Width>& other1, typename SIMD3x2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD3x2(_mm256_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3x2(_mm_xor_ps(other1.values0, other2.values), _mm_xor_ps(other1.values1, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(other1.values0 ^ other2.value, other1.values1 ^ other2.value,
                other1.values2 ^ other2.value, other1.values3 ^ other2.value, other1.values4 ^ other2.value,
                other1.values5 ^ other2.value);
        } else {
            return SIMD3x2<T, Width>(Shift::bitXor(other1.values0, other2.value),
                Shift::bitXor(other1.values1, other2.value), Shift::bitXor(other1.values2, other2.value),
                Shift::bitXor(other1.values3, other2.value), Shift::bitXor(other1.values4, other2.value),
                Shift::bitXor(other1.values5, other2.value));
        }
    }
}

/**
 * Not a SIMD3x2.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other The SIMD3x2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD3x2<T, Width> operator~(const SIMD3x2<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_undefined_ps();
        return SIMD3x2(_mm256_xor_ps(other.values, _mm256_cmp_ps(val, val, _CMP_EQ_UQ)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_undefined_ps();
        val = _mm_cmpeq_ps(val, val);
        return SIMD3x2(_mm_xor_ps(other.values0, val), _mm_xor_ps(other.values1, val));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3x2<T, Width>(
                ~other.values0, ~other.values1, ~other.values2, ~other.values3, ~other.values4, ~other.values5);
        } else {
            return SIMD3x2<T, Width>(Shift::bitNot(other.values0), Shift::bitNot(other.values1),
                Shift::bitNot(other.values2), Shift::bitNot(other.values3), Shift::bitNot(other.values4),
                Shift::bitNot(other.values5));
        }
    }
}

/**
 * And 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask operator&(
    const typename SIMD3x2<T, Width>::Mask& mask1, const typename SIMD3x2<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD3x2<T, Width>::Mask(_kand_mask8(mask1.values, mask2.values));
        } else {
            return SIMD3x2<T, Width>::Mask(_mm256_and_ps(mask1.values, mask2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD3x2<T, Width>::Mask(
                _kand_mask8(mask1.values0, mask2.values0), _kand_mask8(mask1.values1, mask2.values1));
        } else {
            return SIMD3x2<T, Width>::Mask(
                _mm_and_ps(mask1.values0, mask2.values0), _mm_and_ps(mask1.values1, mask2.values1));
        }
    } else
#endif
    {
        return SIMD3x2<T, Width>::Mask(mask1.values0 & mask2.values0, mask1.values1 & mask2.values1,
            mask1.values2 & mask2.values2, mask1.values3 & mask2.values3, mask1.values4 & mask2.values4,
            mask1.values5 & mask2.values5);
    }
}

/**
 * Or 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to or with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask operator|(
    const typename SIMD3x2<T, Width>::Mask& mask1, const typename SIMD3x2<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD3x2<T, Width>::Mask(_kor_mask8(mask1.values, mask2.values));
        } else {
            return SIMD3x2<T, Width>::Mask(_mm256_or_ps(mask1.values, mask2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD3x2<T, Width>::Mask(
                _kor_mask8(mask1.values0, mask2.values0), _kor_mask8(mask1.values1, mask2.values1));
        } else {
            return SIMD3x2<T, Width>::Mask(
                _mm_or_ps(mask1.values0, mask2.values0), _mm_or_ps(mask1.values1, mask2.values1));
        }
    } else
#endif
    {
        return SIMD3x2<T, Width>::Mask(mask1.values0 | mask2.values0, mask1.values1 | mask2.values1,
            mask1.values2 | mask2.values2, mask1.values3 | mask2.values3, mask1.values4 | mask2.values4,
            mask1.values5 | mask2.values5);
    }
}

/**
 * Xor 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 mask to xor with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask operator^(
    const typename SIMD3x2<T, Width>::Mask& mask1, const typename SIMD3x2<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD3x2<T, Width>::Mask(_kxor_mask8(mask1.values, mask2.values));
        } else {
            return SIMD3x2<T, Width>::Mask(_mm256_xor_ps(mask1.values, mask2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD3x2<T, Width>::Mask(
                _kxor_mask8(mask1.values0, mask2.values0), _kxor_mask8(mask1.values1, mask2.values1));
        } else {
            return SIMD3x2<T, Width>::Mask(
                _mm_xor_ps(mask1.values0, mask2.values0), _mm_xor_ps(mask1.values1, mask2.values1));
        }
    } else
#endif
    {
        return SIMD3x2<T, Width>::Mask(mask1.values0 ^ mask2.values0, mask1.values1 ^ mask2.values1,
            mask1.values2 ^ mask2.values2, mask1.values3 ^ mask2.values3, mask1.values4 ^ mask2.values4,
            mask1.values5 ^ mask2.values5);
    }
}

/**
 * Not a Mask.
 * @param mask The mask.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE typename SIMD3x2<T, Width>::Mask operator~(const typename SIMD3x2<T, Width>::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD3x2<T, Width>::Mask(_knot_mask8(mask.values));
        } else {
            return SIMD3x2<T, Width>::Mask(
                _mm256_xor_ps(mask.values, _mm256_cmp_ps(mask.values, mask.values, _CMP_EQ_OQ)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD3x2<T, Width>::Mask(_knot_mask8(mask.values0), _knot_mask8(mask.values1));
        } else {
            return SIMD3x2<T, Width>::Mask(_mm_xor_ps(mask.values0, _mm_cmpeq_ps(mask.values0, mask.values0)),
                _mm_xor_ps(mask.values1, _mm_cmpeq_ps(mask.values1, mask.values1)));
        }
    } else
#endif
    {
        return SIMD3x2<T, Width>::Mask(
            ~mask.values0, ~mask.values1, ~mask.values2, ~mask.values3, ~mask.values4, ~mask.values5);
    }
}
} // namespace Shift
