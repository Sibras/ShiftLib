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

#include "SIMD/XSSIMD4.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class SIMD6;
template<typename T, SIMDWidth Width>
class SIMD3x2;

template<typename T>
class alignas(maxAlignment<T, 2>) SIMD6Data
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    T value0, value1, value2, value3;
    T value4, value5;

    /** Default constructor. */
    XS_FUNCTION SIMD6Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMD6Data(const SIMD6<T, Width>& other) noexcept
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
    XS_FUNCTION void setData(T other0, T other1, T other2, T other3, T other4, T other5) noexcept
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
    XS_FUNCTION void store(const SIMD6<T, Width>& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            _mm_storeu_ps(&value0, _mm256_castps256_ps128(other.values));
            const __m128 val = _mm256_extractf128_ps(other.values, 1);
            _mm_storel_pi(reinterpret_cast<__m64*>(&value4), val);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            _mm_storeu_ps(&value0, other.values0);
            _mm_storel_pi(reinterpret_cast<__m64*>(&value4), other.values1);
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
    XS_FUNCTION SIMD6<T, Width> load() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6<T, Width>(_mm256_loadu_ps(&value0));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6<T, Width>(
                _mm_loadu_ps(&value0), _mm_loadl_pi(_mm_undefined_ps(), reinterpret_cast<const __m64* const>(&value4)));
        } else
#endif
        {
            return SIMD6<T, Width>(value0, value1, value2, value3, value4, value5);
        }
    }
};

template<typename T>
class alignas(maxAlignment<T, 6>) SIMD6DataPad
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    T value0, value1, value2, value3;
    T value4, value5, pad0, pad1;

    /** Default constructor. */
    XS_FUNCTION SIMD6DataPad() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMD6DataPad(const SIMD6<T, Width>& other) noexcept
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
    XS_FUNCTION void setData(T other0, T other1, T other2, T other3, T other4, T other5) noexcept
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
    XS_FUNCTION void store(const SIMD6<T, Width>& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            _mm256_store_ps(&value0, other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            _mm_store_ps(&value0, other.values0);
            _mm_store_ps(&value4, other.values1);
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
    XS_FUNCTION SIMD6<T, Width> load() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6<T, Width>(_mm256_load_ps(&value0));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6<T, Width>(_mm_load_ps(&value0), _mm_load_ps(&value4));
        } else
#endif
        {
            return SIMD6<T, Width>(value0, value1, value2, value3, value4, value5);
        }
    }
};

template<typename T, SIMDWidth Width = widthSIMD<T>>
class SIMD6 : public NoExport::SIMDData<T, 6, 0, Width>
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    using Type = T;
    using Data = NoExport::SIMDData<T, 6, 0, Width>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Data::width;
    using BaseDef = SIMDBase<T, SIMDBase<T, widthImpl>::widthImpl>;
    using InBaseDef = SIMDInBase<T, SIMDInBase<T, widthImpl>::widthImpl>;
    using SIMD2Def = SIMD2<T, SIMD2<T, widthImpl>::widthImpl>;
    using SIMD3Def = SIMD3<T, SIMD3<T, widthImpl>::widthImpl>;
    using SIMD4Def = SIMD4<T, SIMD4<T, widthImpl>::widthImpl>;
    using Data::SIMDData;

    /** Hex Mask object used to store 6 different masks at once. */
    class Mask : public NoExport::SIMDMaskData<T, 6, 0, Width>
    {
    public:
        using Type = T;
        using Data = NoExport::SIMDMaskData<T, 6, 0, Width>;
        static constexpr SIMDWidth width = Width;
        static constexpr SIMDWidth widthImpl = Data::width;
        using Data::SIMDMaskData;
        using Bool = Bool6<hasSIMD<T> && (Width > SIMDWidth::Scalar)>;

        /** Default constructor. */
        XS_FUNCTION Mask() noexcept = default;

        /**
         * Copy constructor.
         * @param other The other.
         */
        XS_FUNCTION Mask(const Mask& other) = default;

        /**
         * Move constructor.
         * @param [in,out] other The other.
         */
        XS_FUNCTION Mask(Mask&& other) noexcept = default;

        /**
         * Assignment operator.
         * @param other The other.
         * @returns A shallow copy of this object.
         */
        XS_FUNCTION Mask& operator=(const Mask& other) = default;

        /**
         * Move assignment operator.
         * @param [in,out] other The other.
         * @returns A shallow copy of this object.
         */
        XS_FUNCTION Mask& operator=(Mask&& other) noexcept = default;

        /**
         * Construct a mask from a bitwise integer representation.
         * @note Each bit in the input is used to set the mask accordingly.
         * @param mask Input bitwise representation.
         */
        XS_FUNCTION explicit Mask(uint32 mask) noexcept
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
                    const __m256i bitMask = _mm256_set_epi32(0x0, 0x0, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1);
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
                    const __m128i bitMask1 = _mm_set_epi32(0x0, 0x0, 0x20, 0x10);
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
                this->values3 = mask & 0x8;
                this->values4 = mask & 0x10;
                this->values5 = mask & 0x20;
            }
        }

        /**
         * Construct a mask from 2 mask3's.
         * @note Each bit in the input is used to set the mask accordingly.
         * @param mask0 First input mask.
         * @param mask1 Second input mask.
         */
        XS_FUNCTION Mask(const typename SIMD3Def::Mask& mask0, const typename SIMD3Def::Mask& mask1) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                    this->values = _kor_mask8(
                        _kor_mask8(_kor_mask8(_kor_mask8(_kor_mask8(_kand_mask8(mask0.values & 0x1),
                                                             _kand_mask8(_kshiftli_mask8(mask1.values, 1) & 0x2)),
                                                  _kand_mask8(_kshiftli_mask8(mask0.values, 1) & 0x4)),
                                       _kand_mask8(_kshiftli_mask8(mask1.values, 2) & 0x8)),
                            _kand_mask8(_kshiftli_mask8(mask1.values, 2) & 0x10)),
                        _kand_mask8(_kshiftli_mask8(mask1.values, 5) & 0x20));
                } else {
                    this->values = _mm256_set_m128(
                        _mm_unpackhi_ps(mask0.values, mask1.values), _mm_unpacklo_ps(mask0.values, mask1.values));
                }
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    this->values0 = _kor_mask8(_kor_mask8(_kor_mask8(_kand_mask8(mask0.values & 0x1),
                                                              _kand_mask8(_kshiftli_mask8(mask1.values, 1) & 0x2)),
                                                   _kand_mask8(_kshiftli_mask8(mask0.values, 1) & 0x4)),
                        _kand_mask8(_kshiftli_mask8(mask1.values, 2) & 0x8));
                    this->values1 = _kor_mask8(_kand_mask8(_kshiftri_mask8(mask0.values, 2) & 0x1),
                        _kand_mask8(_kshiftri_mask8(mask1.values, 2) & 0x2));
                } else {
                    this->values0 = _mm_unpacklo_ps(mask0.values, mask1.values);
                    this->values1 = _mm_unpackhi_ps(mask0.values, mask1.values);
                }
            } else
#endif
            {
                this->values0 = mask0.values0;
                this->values1 = mask1.values0;
                this->values2 = mask0.values1;
                this->values3 = mask1.values1;
                this->values4 = mask0.values2;
                this->values5 = mask1.values2;
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
        XS_FUNCTION Mask(bool bool0, bool bool1, bool bool2, bool bool3, bool bool4, bool bool5) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    this->values = _cvtu32_mask8((static_cast<uint32>(bool5) << 5) | (static_cast<uint32>(bool4) << 4) |
                        (static_cast<uint32>(bool3) << 3) | (static_cast<uint32>(bool2) << 2) |
                        (static_cast<uint32>(bool1) << 1) | static_cast<uint32>(bool0));
                } else {
                    this->values = _mm256_cmp_ps(_mm256_set_ps(0, 0, bool5, bool4, bool3, bool2, bool1, bool0),
                        _mm256_setzero_ps(), _CMP_NEQ_OQ);
                }
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    this->values0 =
                        _cvtu32_mask8((static_cast<uint32>(bool3) << 3) | (static_cast<uint32>(bool2) << 2) |
                            (static_cast<uint32>(bool1) << 1) | static_cast<uint32>(bool0));
                    this->values1 = _cvtu32_mask8((static_cast<uint32>(bool5) << 1) | static_cast<uint32>(bool4));
                } else {
                    const __m128 zero = _mm_setzero_ps();
                    this->values0 = _mm_cmpneq_ps(_mm_set_ps(bool3, bool2, bool1, bool0), zero);
                    this->values1 = _mm_cmpneq_ps(_mm_set_ps(0, 0, bool5, bool4), zero);
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
        XS_FUNCTION Bool getBool6() const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return Bool6<true>(static_cast<uint8>(_cvtmask8_u32(this->values)));
                } else {
                    return Bool6<true>(static_cast<uint8>(_mm256_movemask_ps(this->values)));
                }
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return Bool6<true>(static_cast<uint8>(_cvtmask8_u32(this->values0) << 4UL) |
                        static_cast<uint8>(_cvtmask8_u32(this->values1)));
                } else {
                    return Bool6<true>(static_cast<uint8>(_mm_movemask_ps(this->values0) << 4UL) |
                        static_cast<uint8>(_mm_movemask_ps(this->values1)));
                }
            } else
#endif
            {
                return Bool6(this->values0, this->values1, this->values2, this->values3, this->values4, this->values5);
            }
        }

        /**
         * Get boolean signalling if any element of mask is set.
         * @returns Boolean.
         */
        XS_FUNCTION bool getAny() const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return static_cast<bool>(_cvtmask8_u32(this->values) & 0x3F);
                } else {
                    return static_cast<bool>(_mm256_movemask_ps(this->values) & 0x3F);
                }
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return static_cast<bool>(_cvtmask8_u32(this->values0) | (_cvtmask8_u32(this->values1) & 0x3));
                } else {
                    return static_cast<bool>(_mm_movemask_ps(this->values0) | (_mm_movemask_ps(this->values1) & 0x3));
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
        XS_FUNCTION bool getAll() const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return ((_cvtmask8_u32(this->values) & 0x3F) == 0x3F);
                } else {
                    return ((_mm256_movemask_ps(this->values) & 0x3F) == 0x3F);
                }
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return ((_cvtmask8_u32(this->values0) & (_cvtmask8_u32(this->values1) & 0x3)) == 0x3);
                } else {
                    return ((_mm_movemask_ps(this->values0) & (_mm_movemask_ps(this->values1) & 0x3)) == 0x3);
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
        XS_FUNCTION bool getNone() const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return ((_cvtmask8_u32(this->values) & 0x3F) == 0x0);
                } else {
                    return ((_mm256_movemask_ps(this->values) & 0x3F) == 0x0);
                }
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return ((_cvtmask8_u32(this->values0) & (_cvtmask8_u32(this->values1) & 0x3)) == 0x0);
                } else {
                    return ((_mm_movemask_ps(this->values0) & (_mm_movemask_ps(this->values1) & 0x3)) == 0x0);
                }
            } else
#endif
            {
                return !((this->values0 | this->values1) | (this->values2 | this->values3) |
                    (this->values4 | this->values5));
            }
        }

    private:
#if XS_ISA == XS_X86
        class SIMDMasker6X86
        {
        public:
            XS_INLINE static SIMD6& ToType(SIMD6 other)
            {
                return other;
            }
        };
#endif

        template<uint8 Index>
        class SIMDMasker6
        {
        public:
            XS_INLINE static InBaseDef ToType(SIMD6 other)
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
        XS_FUNCTION void mask6Function(MaskOperator& maskFunc) const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                auto value = maskFunc.template expression<SIMD6, SIMDMasker6X86>();
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    value.values = _mm256_mask_blend_ps(this->values, _mm256_setzero_ps(), value.values);
                } else {
                    value.values = _mm256_and_ps(value.values, this->values);
                }
                maskFunc.template finalExpression<SIMD6, SIMDMasker6X86>(value);
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                auto value = maskFunc.template expression<SIMD6, SIMDMasker6X86>();
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    const __m128 zero = _mm_setzero_ps();
                    value.values0 = _mm_mask_blend_ps(this->values0, zero, value.values0);
                    value.values1 = _mm_mask_blend_ps(this->values1, zero, value.values1);
                } else {
                    value.values0 = _mm_and_ps(value.values0, this->values0);
                    value.values1 = _mm_and_ps(value.values1, this->values1);
                }
                maskFunc.template finalExpression<SIMD6, SIMDMasker6X86>(value);
            } else
#endif
            {
                if (this->values0) {
                    auto value = maskFunc.template expression<InBaseDef, SIMDMasker6<0>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker6<0>>(value);
                }
                if (this->values1) {
                    auto value = maskFunc.template expression<InBaseDef, SIMDMasker6<1>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker6<1>>(value);
                }
                if (this->values2) {
                    auto value = maskFunc.template expression<InBaseDef, SIMDMasker6<2>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker6<2>>(value);
                }
                if (this->values3) {
                    auto value = maskFunc.template expression<InBaseDef, SIMDMasker6<3>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker6<3>>(value);
                }
                if (this->values4) {
                    auto value = maskFunc.template expression<InBaseDef, SIMDMasker6<4>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker6<4>>(value);
                }
                if (this->values5) {
                    auto value = maskFunc.template expression<InBaseDef, SIMDMasker6<5>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker6<5>>(value);
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
        XS_FUNCTION void mask6ElseFunction(MaskOperator& maskFunc) const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                auto value1 = maskFunc.template expression1<SIMD6, SIMDMasker6X86>();
                auto value2 = maskFunc.template expression2<SIMD6, SIMDMasker6X86>();
                const auto final(value1.blendVar(value2, *this));
                maskFunc.template finalExpression<SIMD6, SIMDMasker6X86>(final);
            } else
#endif
            {
                auto value = (this->values0) ? maskFunc.template expression1<InBaseDef, SIMDMasker6<0>>() :
                                               maskFunc.template expression2<InBaseDef, SIMDMasker6<0>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker6<0>>(value);
                value = (this->values1) ? maskFunc.template expression1<InBaseDef, SIMDMasker6<1>>() :
                                          maskFunc.template expression2<InBaseDef, SIMDMasker6<1>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker6<1>>(value);
                value = (this->values2) ? maskFunc.template expression1<InBaseDef, SIMDMasker6<2>>() :
                                          maskFunc.template expression2<InBaseDef, SIMDMasker6<2>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker6<2>>(value);
                value = (this->values3) ? maskFunc.template expression1<InBaseDef, SIMDMasker6<3>>() :
                                          maskFunc.template expression2<InBaseDef, SIMDMasker6<3>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker6<3>>(value);
                value = (this->values4) ? maskFunc.template expression1<InBaseDef, SIMDMasker6<4>>() :
                                          maskFunc.template expression2<InBaseDef, SIMDMasker6<4>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker6<4>>(value);
                value = (this->values5) ? maskFunc.template expression1<InBaseDef, SIMDMasker6<5>>() :
                                          maskFunc.template expression2<InBaseDef, SIMDMasker6<5>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker6<5>>(value);
            }
        }
    };

    /** Shuffle class used to store shuffle information for SIMD6s. */
    class Shuffle : public NoExport::SIMDShuffleData<T, 6, 0, Width>
    {
    public:
        using Type = T;
        using Data = NoExport::SIMDShuffleData<T, 6, 0, Width>;
        static constexpr SIMDWidth width = Width;
        static constexpr SIMDWidth widthImpl = Data::width;
        using Data::SIMDShuffleData;

        /** Default constructor. */
        XS_FUNCTION Shuffle() noexcept = default;

        /**
         * Copy constructor.
         * @param other The other.
         */
        XS_FUNCTION Shuffle(const Shuffle& other) = default;

        /**
         * Move constructor.
         * @param [in,out] other The other.
         */
        XS_FUNCTION Shuffle(Shuffle&& other) noexcept = default;

        /**
         * Assignment operator.
         * @param other The other.
         * @returns A shallow copy of this object.
         */
        XS_FUNCTION Shuffle& operator=(const Shuffle& other) = default;

        /**
         * Move assignment operator.
         * @param [in,out] other The other.
         * @returns A shallow copy of this object.
         */
        XS_FUNCTION Shuffle& operator=(Shuffle&& other) noexcept = default;

        /**
         * Construct a shuffle from integer components.
         * @param shuff0 The first input integer component (must be between 0 and 5).
         * @param shuff1 The second input integer component (must be between 0 and 5).
         * @param shuff2 The third input integer component (must be between 0 and 5).
         * @param shuff3 The fourth input integer component (must be between 0 and 5).
         * @param shuff4 The fifth input integer component (must be between 0 and 5).
         * @param shuff5 The sixth input integer component (must be between 0 and 5).
         */
        XS_FUNCTION Shuffle(
            uint32 shuff0, uint32 shuff1, uint32 shuff2, uint32 shuff3, uint32 shuff4, uint32 shuff5) noexcept
        {
            XS_ASSERT(shuff0 < 6);
            XS_ASSERT(shuff1 < 6);
            XS_ASSERT(shuff2 < 6);
            XS_ASSERT(shuff3 < 6);
            XS_ASSERT(shuff4 < 6);
            XS_ASSERT(shuff5 < 6);
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                this->values = _mm256_set_epi32(0, 0, shuff5, shuff4, shuff3, shuff2, shuff1, shuff0);
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    this->values0 = _mm_set_epi32(shuff3, shuff2, shuff1, shuff0);
                    this->values1 = _mm_set_epi32(0, 0, shuff5, shuff4);
                } else {
                    this->values0 =
                        _mm_set_epi32((shuff3 * 0x04040404) + 0x03020100, (shuff2 * 0x04040404) + 0x03020100,
                            (shuff1 * 0x04040404) + 0x03020100, (shuff0 * 0x04040404) + 0x03020100);
                    this->values1 =
                        _mm_set_epi32(0x0, 0x0, (shuff5 * 0x04040404) + 0x03020100, (shuff4 * 0x04040404) + 0x03020100);
                }
            } else
#endif
            {
                this->values0 = shuff0;
                this->values1 = shuff1;
                this->values2 = shuff2;
                this->values3 = shuff3;
                this->values4 = shuff4;
                this->values5 = shuff5;
            }
        }

        /**
         * Constructor to build a Shuffle that swaps the high element with the low element of each internal pair of
         * values.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle Swap2() noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return Shuffle(_mm256_set_epi32(6, 7, 4, 5, 2, 3, 0, 1));
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    return Shuffle(_mm_set_epi32(2, 3, 0, 1), _mm_set_epi32(6, 7, 4, 5));
                } else {
                    return Shuffle(_mm_set_epi32(0x0B0A0908, 0x0F0E0D0C, 0x03020100, 0x07060504),
                        _mm_set_epi32(0x1F1E1D1C, 0x1B1A1918, 0x13121110, 0x17161514));
                }
            } else
#endif
            {
                return Shuffle(1_ui32, 0_ui32, 3_ui32, 2_ui32, 5_ui32, 4_ui32);
            }
        }

        /**
         * Constructor to build a Shuffle that swaps the high element with the low element of each internal pair of
         * values with a mask.
         * @note Where the mask is valid the shuffled value will be output else the original un-shuffled value will
         * be copied.
         * @param mask Mask used to determine which element pairs are swapped.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle Swap2(const typename SIMD3Def::Mask& mask) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                const __m256i val = _mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0);
                // Swap holds a 1 (or 4 in non AVX) if the mask is positive or 0 if it is negative. When Norm is Xored
                // with 0 it has no effect but the binary xor with 1 results in the sequence 2,3,0,1 which is what we
                // want in order to swap the top and bottom half
                const __m256i swap = _mm256_set1_epi32(1);
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    uint32 mask2 = _cvtmask8_u32(mask.values);
                    mask2 = _pdep_u32(mask2, 0x55);
                    mask2 = mask2 | (mask2 << 1);
                    return Shuffle(_mm256_mask_xor_epi32(val, _cvtu32_mask8(mask2), val, swap));
                } else {
                    const __m256 mask2 =
                        _mm256_set_m128(_mm_shuffle3322_ps(mask.values), _mm_shuffle1100_ps(mask.values));
                    return Shuffle(_mm256_xor_si256(val, _mm256_and_si256(swap, _mm256_castps_si256(mask2))));
                }
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    const __m128i val0 = _mm_set_epi32(3, 2, 1, 0);
                    const __m128i val1 = _mm_set_epi32(7, 6, 5, 4);
                    const __m128i swap = _mm_shuffle_epi32(val0, _MM_SHUFFLE(1, 1, 1, 1));
                    if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                        uint32 val = _cvtmask8_u32(mask.values);
                        val = _pdep_u32(val, 0x55);
                        val = val | (val << 1);
                        return Shuffle(_mm_mask_xor_epi32(val0, _cvtu32_mask8(val), val0, swap),
                            _mm_mask_xor_epi32(val1, _cvtu32_mask8(val >> 4), val1, swap));
                    } else {
                        const __m128i mask0 = _mm_castps_si128(_mm_shuffle1100_ps(mask.values));
                        const __m128i mask1 = _mm_castps_si128(_mm_shuffle3322_ps(mask.values));
                        return Shuffle(_mm_xor_si128(val0, _mm_and_si128(swap, mask0)),
                            _mm_xor_si128(val1, _mm_and_si128(swap, mask1)));
                    }
                } else {
                    const __m128i val0 = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
                    const __m128i val1 = _mm_set_epi32(0x1F1E1D1C, 0x1B1A1918, 0x17161514, 0x13121110);
                    const __m128i swap = _mm_set1_epi8(4);
                    const __m128i mask0 = _mm_castps_si128(_mm_shuffle1100_ps(mask.values));
                    const __m128i mask1 = _mm_castps_si128(_mm_shuffle3322_ps(mask.values));
                    return Shuffle(_mm_xor_si128(val0, _mm_and_si128(swap, mask0)),
                        _mm_xor_si128(val1, _mm_and_si128(swap, mask1)));
                }
            } else
#endif
            {
                return Shuffle(static_cast<uint32>(mask.values0), 1_ui32 ^ static_cast<uint32>(mask.values0),
                    2_ui32 ^ static_cast<uint32>(mask.values1), 3_ui32 ^ static_cast<uint32>(mask.values1),
                    4_ui32 ^ static_cast<uint32>(mask.values2), 5_ui32 ^ static_cast<uint32>(mask.values2));
            }
        }
    };

    /** Default constructor. */
    XS_FUNCTION SIMD6() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION SIMD6(const SIMD6& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION SIMD6(SIMD6&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD6& operator=(const SIMD6& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD6& operator=(SIMD6&& other) noexcept = default;

    /**
     * Construct a from 6 different values.
     * @param value0 The first value.
     * @param value1 The second value.
     * @param value2 The third value.
     * @param value3 The fourth value.
     * @param value4 The fifth value.
     * @param value5 The sixth value.
     */
    XS_FUNCTION SIMD6(T value0, T value1, T value2, T value3, T value4, T value5) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values = _mm256_set_ps(0, 0, value5, value4, value3, value2, value1, value0);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_set_ps(value3, value2, value1, value0);
            this->values1 = _mm_set_ps(0, 0, value5, value4);
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
    XS_FUNCTION explicit SIMD6(T value) noexcept
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
    XS_FUNCTION explicit SIMD6(SIMD6<T, Width2> other) noexcept
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
                this->values0 = _mm_set_ps(other.values3, other.values2, other.values1, other.values0);
                this->values1 = _mm_set_ps(0, 0, other.values5, other.values4);
            }
        } else if constexpr (Width2 > SIMDWidth::Scalar) {
            if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width2 >= SIMDWidth::B32)) {
                this->values0 = _mm_cvtss_f32(_mm256_castps256_ps128(other.values));
                this->values1 = _mm_cvtss_f32(_mm_shuffle3311_ps(_mm256_castps256_ps128(other.values)));
                this->values2 = _mm_cvtss_f32(_mm_shuffle3232_ps(_mm256_castps256_ps128(other.values)));
                this->values3 = _mm_cvtss_f32(_mm_shuffle3333_ps(_mm256_castps256_ps128(other.values)));
                const auto hi = _mm256_extractf128_ps(this->values, 1);
                this->values4 = _mm_cvtss_f32(hi);
                this->values5 = _mm_cvtss_f32(_mm_shuffle3311_ps(hi));
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
    XS_FUNCTION explicit SIMD6(BaseDef other) noexcept
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
    XS_FUNCTION explicit SIMD6(InBaseDef other) noexcept
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
     * Construct from 3 different SIMD2s.
     * @param other0 The first SIMD2.
     * @param other1 The second SIMD2.
     * @param other2 The third SIMD2.
     */
    XS_FUNCTION
    SIMD6(const SIMD2Def& other0, const SIMD2Def& other1, const SIMD2Def& other2) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values = _mm256_set_m128(other2.values, _mm_movelh_ps(other0.values, other1.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_movelh_ps(other0.values, other1.values);
            this->values1 = other2.values;
        } else
#endif
        {
            this->values0 = other0.values0;
            this->values1 = other0.values1;
            this->values2 = other1.values0;
            this->values3 = other1.values1;
            this->values4 = other2.values0;
            this->values5 = other2.values1;
        }
    }

    /**
     * Construct from 3 identical SIMD2s.
     * @param other The SIMD2.
     */
    XS_FUNCTION explicit SIMD6(const SIMD2Def& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                this->values = _mm256_broadcastf64_ps(other.values);
            } else {
                const __m128 val = _mm_shuffle1010_ps(other.values);
                this->values = _mm256_broadcastf128_ps(val);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_shuffle1010_ps(other.values);
            this->values1 = this->values0;
        } else
#endif
        {
            this->values0 = other.values0;
            this->values1 = other.values1;
            this->values2 = other.values0;
            this->values3 = other.values1;
            this->values4 = other.values0;
            this->values5 = other.values1;
        }
    }

    /**
     * Construct from 2 different SIMD3s.
     * @note Note SIMD3s are packed when put into a SIMD6. This means that the first 2 elements will
     * actually consist of the packed first element of each of the 2 SIMD3s etc.
     * @param other0 The first SIMD3.
     * @param other1 The second SIMD3.
     */
    XS_FUNCTION SIMD6(const SIMD3Def& other0, const SIMD3Def& other1) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values = _mm256_set_m128(
                _mm_unpackhi_ps(other0.values, other1.values), _mm_unpacklo_ps(other0.values, other1.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_unpacklo_ps(other0.values, other1.values);
            this->values1 = _mm_unpackhi_ps(other0.values, other1.values);
        } else
#endif
        {
            this->values0 = other0.values0;
            this->values1 = other1.values0;
            this->values2 = other0.values1;
            this->values3 = other1.values1;
            this->values4 = other0.values2;
            this->values5 = other1.values2;
        }
    }

    /**
     * Construct from a different SIMD3.
     * @note Note SIMD3s are packed when put into a SIMD6. This means that the first 2 elements of a SIMD6 will
     * actually consist of the packed first element of each of the 2 SIMD3s etc.
     * @param other The SIMD3.
     */
    XS_FUNCTION explicit SIMD6(const SIMD3Def& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values = _mm256_set_m128(
                _mm_unpackhi_ps(other.values, other.values), _mm_unpacklo_ps(other.values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values0 = _mm_unpacklo_ps(other.values, other.values);
            this->values1 = _mm_unpackhi_ps(other.values, other.values);
        } else
#endif
        {
            this->values0 = other.values0;
            this->values1 = other.values0;
            this->values2 = other.values1;
            this->values3 = other.values1;
            this->values4 = other.values2;
            this->values5 = other.values2;
        }
    }

    /**
     * Construct from a SIMD4 and a SIMD2.
     * @param other0 The SIMD4.
     * @param other1 The SIMD2.
     */
    XS_FUNCTION SIMD6(const SIMD4Def& other0, const SIMD2Def& other1) noexcept
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
            this->values3 = other0.values3;
            this->values4 = other1.values0;
            this->values5 = other1.values1;
        }
    }

    /**
     * Constructor to build set to 0.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD6 Zero() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(_mm256_setzero_ps());
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const auto value = _mm_setzero_ps();
            return SIMD6(value, value);
        } else
#endif
        {
            return SIMD6(T{0});
        }
    }

    /**
     * Constructor to build set to 1.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD6 One() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(_mm256_set1_ps(1.0f));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const auto value = _mm_set1_ps(1.0f);
            return SIMD6(value, value);
        } else
#endif
        {
            return SIMD6(T{1});
        }
    }

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-5).
     * @returns SIMDInBase containing the desired value.
     */
    template<uint32 Index>
    XS_FUNCTION InBaseDef getValueInBase() const noexcept
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
                return InBaseDef(_mm_shuffle3333_ps(_mm256_castps256_ps128(this->values))); //(x,x,x,3)
            } else if constexpr (Index == 4) {
                return InBaseDef(_mm256_extractf128_ps(this->values, 1));
            } else /*Index == 5*/ {
                return InBaseDef(_mm_shuffle3311_ps(_mm256_extractf128_ps(this->values, 1))); //(x,x,x,1)
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 4 == 0) {
                return InBaseDef((&this->values0)[Index / 4]);
            } else if constexpr (Index % 4 == 1) {
                return InBaseDef(_mm_shuffle3311_ps((&this->values0)[Index / 4])); //(x,x,x,1)
            } else if constexpr (Index == 2) {
                return InBaseDef(_mm_shuffle3232_ps(this->values0)); //(x,x,x,2)
            } else /*Index == 3*/ {
                return InBaseDef(_mm_shuffle3333_ps(this->values0)); //(x,x,x,3)
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
     * @tparam Index The index of the element to retrieve (range is 0-2).
     * @returns SIMD2 containing the desired values.
     */
    template<uint32 Index>
    XS_FUNCTION SIMD2Def getValue2() const noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                return SIMD2Def(_mm256_castps256_ps128(this->values));
            } else if constexpr (Index == 1) {
                return SIMD2Def(
                    _mm_movehl_ps(_mm256_castps256_ps128(this->values), _mm256_castps256_ps128(this->values)));
            } else /*Index == 2*/ {
                return SIMD2Def(_mm256_extractf128_ps(this->values, 1));
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
     * Get a SIMD6.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-1).
     * @returns SIMD3 containing the desired values.
     */
    template<uint32 Index>
    XS_FUNCTION SIMD3Def getValue3() const noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                return SIMD3Def(_mm_shuffle_ps(_mm256_castps256_ps128(this->values),
                    _mm256_extractf128_ps(this->values, 1), _MM_SHUFFLE(2, 0, 2, 0)));
            } else /*Index == 1*/ {
                return SIMD3Def(_mm_shuffle_ps(_mm256_castps256_ps128(this->values),
                    _mm256_extractf128_ps(this->values, 1), _MM_SHUFFLE(3, 1, 3, 1)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                return SIMD3Def(_mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(2, 0, 2, 0)));
            } else /*Index == 1*/ {
                return SIMD3Def(_mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(3, 1, 3, 1)));
            }
        } else
#endif
        {
            return SIMD3Def((&this->values0)[Index], (&this->values0)[Index + 2], (&this->values0)[Index + 4]);
        }
    }

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-5).
     * @param other The new value.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue(InBaseDef other) noexcept
    {
        static_assert(Index < 6, "Invalid Index: Index must be <6");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                this->values = _mm256_blend_ps(
                    this->values, _mm256_castps128_ps256(other.values), _MM256_BLEND(0, 0, 0, 0, 0, 0, 0, 1));
            } else if constexpr (Index < 4 && hasISAFeature<ISAFeature::AVX512F>) {
                this->values = _mm256_mask_permute_ps(this->values, _cvtu32_mask8(1 << Index),
                    _mm256_castps128_ps256(other.values), _MM_SHUFFLE(0, 0, 0, 0));
            } else if constexpr (Index == 1) {
                const __m128 value = _mm_shuffle1100_ps(other.values);
                this->values =
                    _mm256_blend_ps(this->values, _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 0, 0, 1, 0));
            } else if constexpr (Index == 2) {
                const __m128 value = _mm_shuffle1010_ps(other.values); //(x,0,x,x)
                this->values =
                    _mm256_blend_ps(this->values, _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 0, 1, 0, 0));
            } else if constexpr (Index == 3) {
                const __m128 value = _mm_shuffle1_ps(other.values, _MM_SHUFFLE(0, 2, 1, 3)); //(0,x,x,x);
                this->values =
                    _mm256_blend_ps(this->values, _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 1, 0, 0, 0));
            } else if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                this->values = _mm256_mask_broadcastss_ps(this->values, _cvtu32_mask8(1 << Index), other.values);
            } else if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                this->values = _mm256_blend_ps(this->values, _mm256_broadcastss_ps(other.values), (1 << Index));
            } else if constexpr (Index == 4) {
                this->values = _mm256_blend_ps(
                    this->values, _mm256_broadcastf128_ps(other.values), _MM256_BLEND(0, 0, 0, 1, 0, 0, 0, 0));
            } else /*Index == 5*/ {
                const __m128 value = _mm_shuffle1100_ps(other.values);
                this->values =
                    _mm256_blend_ps(this->values, _mm256_broadcastf128_ps(value), _MM256_BLEND(0, 0, 1, 0, 0, 0, 0, 0));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 4 == 0) {
                (&this->values0)[Index / 4] = _mm_move_ss((&this->values0)[Index / 4], other.values);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                (&this->values0)[Index / 4] =
                    _mm_insert_ps((&this->values0)[Index / 4], other.values, _MM_MK_INSERTPS_NDX(0, Index % 4, 0));
            } else if constexpr (Index % 4 == 1) {
                const __m128 value = _mm_movelh_ps((&this->values0)[Index / 4], other.values); /*(x,0,x,0)*/
                (&this->values0)[Index / 4] =
                    _mm_shuffle_ps(value, (&this->values0)[Index / 4], _MM_SHUFFLE(3, 2, 2, 0));
            } else if constexpr (Index == 2) {
                const __m128 value = _mm_shuffle_ps(other.values, this->values0, _MM_SHUFFLE(3, 1, 0, 2)); /*(3,x,0,x)*/
                this->values0 = _mm_shuffle_ps(this->values0, value, _MM_SHUFFLE(3, 1, 1, 0));
            } else /*Index == 3*/ {
                const __m128 value = _mm_shuffle_ps(other.values, this->values0, _MM_SHUFFLE(3, 2, 0, 1)); /*(x,2,0,x)*/
                this->values0 = _mm_shuffle_ps(this->values0, value, _MM_SHUFFLE(1, 2, 1, 0));
            }
        } else
#endif
        {
            (&this->values0)[Index] = other.value;
        }
    }

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-5).
     * @param other The new value.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue(BaseDef other) noexcept
    {
        static_assert(Index < 6, "Invalid Index: Index must be <6");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values = _mm256_blend_ps(this->values, other.values, (1 << Index));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 4 == 0) {
                (&this->values0)[Index / 4] = _mm_move_ss((&this->values0)[Index / 4], other.values);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                (&this->values0)[Index / 4] =
                    _mm_blend_ps((&this->values0)[Index / 4], other.values, (1 << (Index % 4)));
            } else if constexpr (Index % 4 == 1) {
                const __m128 val = _mm_movelh_ps((&this->values0)[Index / 4], other.values); /*(x,0,x,0)*/
                (&this->values0)[Index / 4] = _mm_shuffle_ps(val, (&this->values0)[Index / 4], _MM_SHUFFLE(3, 2, 2, 0));
            } else if constexpr (Index == 2) {
                const __m128 val = _mm_shuffle_ps(other.values, this->values0, _MM_SHUFFLE(3, 1, 0, 2)); /*(3,x,0,x)*/
                this->values0 = _mm_shuffle_ps(this->values0, val, _MM_SHUFFLE(3, 1, 1, 0));
            } else /*Index == 3*/ {
                const __m128 val = _mm_shuffle_ps(other.values, this->values0, _MM_SHUFFLE(3, 2, 0, 1)); /*(x,2,0,x)*/
                this->values0 = _mm_shuffle_ps(this->values0, val, _MM_SHUFFLE(1, 2, 1, 0));
            }
        } else
#endif
        {
            (&this->values0)[Index] = other.value;
        }
    }

    /**
     * Set a SIMD2.
     * @tparam Index The index of the element to set (range is 0-2).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue2(const SIMD2Def& other) noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                this->values = _mm256_blend_ps(
                    this->values, _mm256_castps128_ps256(other.values), _MM256_BLEND(0, 0, 0, 0, 0, 0, 1, 1));
            } else if constexpr (Index == 1) {
                const __m128 value = _mm_shuffle1010_ps(other.values); //(1,0,x,x)
                this->values =
                    _mm256_blend_ps(this->values, _mm256_castps128_ps256(value), _MM256_BLEND(0, 0, 0, 0, 1, 1, 0, 0));
            } else /*Index == 2*/ {
                if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                    this->values = _mm256_blend_ps(
                        this->values, _mm256_broadcastf64_ps(other.values), _MM256_BLEND(0, 0, 1, 1, 0, 0, 0, 0));
                } else {
                    const __m128 value = _mm_shuffle1010_ps(other.values); //(1,0,x,x)
                    this->values = _mm256_blend_ps(
                        this->values, _mm256_broadcastf128_ps(value), _MM256_BLEND(0, 0, 1, 1, 0, 0, 0, 0));
                }
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 2 == 0) {
                (&this->values0)[Index / 2] =
                    _mm_shuffle_ps(other.values, (&this->values0)[Index / 2], _MM_SHUFFLE(3, 2, 1, 0));
            } else {
                this->values0 = _mm_movelh_ps(this->values0, other.values);
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
     * @tparam Index The index of the element to set (range is 0-1).
     * @param other The new values.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue3(const SIMD3Def& other) noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                const __m256 val = _mm256_set_m128(
                    _mm_movehl_ps(other.values, other.values), _mm_unpacklo_ps(other.values, other.values));
                this->values = _mm256_blend_ps(this->values, val, _MM256_BLEND(0, 1, 0, 1, 0, 1, 0, 1));
            } else /*Index == 1*/ {
                const __m256 val = _mm256_set_m128(
                    _mm_movehl_ps(other.values, other.values), _mm_unpacklo_ps(other.values, other.values));
                this->values = _mm256_blend_ps(this->values, _mm256_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)),
                    _MM256_BLEND(1, 0, 1, 0, 1, 0, 1, 0));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                    this->values0 = _mm_move_ss(this->values0, other.values);
                    this->values0 = _mm_insert_ps(this->values0, other.values, _MM_MK_INSERTPS_NDX(1, 2, 0));
                    this->values1 = _mm_insert_ps(this->values1, other.values, _MM_MK_INSERTPS_NDX(2, 0, 0));
                } else {
                    this->values0 = _mm_shuffle_ps(this->values0, this->values0, _MM_SHUFFLE(3, 1, 3, 1)); //(x,x,3,1)
                    this->values0 = _mm_unpacklo_ps(other.values, this->values0);
                    this->values1 = _mm_move_ss(this->values1, _mm_movehl_ps(other.values, other.values));
                }
            } else /*Index == 1*/ {
                if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                    this->values0 = _mm_insert_ps(this->values0, other.values, _MM_MK_INSERTPS_NDX(0, 1, 0));
                    this->values0 = _mm_insert_ps(this->values0, other.values, _MM_MK_INSERTPS_NDX(1, 3, 0));
                    this->values1 = _mm_insert_ps(this->values1, other.values, _MM_MK_INSERTPS_NDX(2, 1, 0));
                } else {
                    this->values0 = _mm_shuffle_ps(this->values0, this->values0, _MM_SHUFFLE(2, 0, 2, 0)); //(x,x,2,0)
                    this->values0 = _mm_unpacklo_ps(this->values0, other.values);

                    this->values1 = _mm_unpacklo_ps(this->values1, _mm_movehl_ps(other.values, other.values));
                }
            }
        } else
#endif
        {
            (&this->values0)[Index] = other.values0;
            (&this->values0)[Index + 2] = other.values1;
            (&this->values0)[Index + 4] = other.values2;
        }
    }

    /**
     * Add a value to an element of the object.
     * @tparam Index The index of the element to modify (range is 0-5).
     * @param other The value to add.
     */
    template<uint32 Index>
    XS_FUNCTION void addValue(InBaseDef other) noexcept
    {
        static_assert(Index < 6, "Invalid Index: Index must be <6");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                this->values =
                    _mm256_blend_add_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(other.values));
            } else if constexpr (Index == 1) {
                const __m128 value = _mm_shuffle2200_ps(other.values);
                this->values =
                    _mm256_blend_add_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(value));
            } else if constexpr (Index == 2 || Index == 3) {
                const __m128 value = _mm_shuffle0000_ps(other.values);
                this->values =
                    _mm256_blend_add_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(value));
            } else if constexpr (Index == 4) {
                this->values =
                    _mm256_blend_add_ps(this->values, 1 << Index, this->values, _mm256_broadcastf128_ps(other.values));
            } else /*Index == 5*/ {
                const __m128 value = _mm_shuffle2200_ps(other.values);
                this->values =
                    _mm256_blend_add_ps(this->values, 1 << Index, this->values, _mm256_broadcastf128_ps(value));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 4 == 0) {
                (&this->values0)[Index / 4] = _mm_add_ss((&this->values0)[Index / 4], other.values);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index % 4 == 1) {
                const __m128 value = _mm_shuffle2200_ps(other.values);
                (&this->values0)[Index / 4] =
                    _mm_blend_add_ps((&this->values0)[Index / 4], 1 << (Index % 4), (&this->values0)[Index / 4], value);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                const __m128 value = _mm_shuffle0000_ps(other.values);
                (&this->values0)[Index / 4] =
                    _mm_blend_add_ps((&this->values0)[Index / 4], 1 << (Index % 4), (&this->values0)[Index / 4], value);
            } else {
                constexpr auto shuffle = _MM_SHUFFLE(
                    (Index % 4) == 3 ? 0 : 3, (Index % 4) == 2 ? 0 : 2, (Index % 4) == 1 ? 0 : 1, (Index % 4));
                __m128 value = _mm_shuffle1_ps((&this->values0)[Index / 4], shuffle);
                value = _mm_add_ss(value, other.values);
                (&this->values0)[Index / 4] = _mm_shuffle1_ps(value, shuffle);
            }
        } else
#endif
        {
            (&this->values0)[Index] += other.value;
        }
    }

    /**
     * Subtract a value from an element of the object.
     * @tparam Index The index of the element to modify (range is 0-5).
     * @param other The value to subtract.
     */
    template<uint32 Index>
    XS_FUNCTION void subValue(InBaseDef other) noexcept
    {
        static_assert(Index < 6, "Invalid Index: Index must be <6");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                this->values =
                    _mm256_blend_sub_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(other.values));
            } else if constexpr (Index == 1) {
                const __m128 value = _mm_shuffle2200_ps(other.values);
                this->values =
                    _mm256_blend_sub_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(value));
            } else if constexpr (Index == 2 || Index == 3) {
                const __m128 value = _mm_shuffle0000_ps(other.values);
                this->values =
                    _mm256_blend_sub_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(value));
            } else if constexpr (Index == 4) {
                this->values =
                    _mm256_blend_sub_ps(this->values, 1 << Index, this->values, _mm256_broadcastf128_ps(other.values));
            } else /*Index == 5*/ {
                const __m128 value = _mm_shuffle2200_ps(other.values);
                this->values =
                    _mm256_blend_sub_ps(this->values, 1 << Index, this->values, _mm256_broadcastf128_ps(value));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 4 == 0) {
                (&this->values0)[Index / 4] = _mm_sub_ss((&this->values0)[Index / 4], other.values);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index % 4 == 1) {
                const __m128 value = _mm_shuffle2200_ps(other.values);
                (&this->values0)[Index / 4] =
                    _mm_blend_sub_ps((&this->values0)[Index / 4], 1 << (Index % 4), (&this->values0)[Index / 4], value);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                const __m128 value = _mm_shuffle0000_ps(other.values);
                (&this->values0)[Index / 4] =
                    _mm_blend_sub_ps((&this->values0)[Index / 4], 1 << (Index % 4), (&this->values0)[Index / 4], value);
            } else {
                constexpr auto shuffle = _MM_SHUFFLE(
                    (Index % 4) == 3 ? 0 : 3, (Index % 4) == 2 ? 0 : 2, (Index % 4) == 1 ? 0 : 1, (Index % 4));
                __m128 value = _mm_shuffle1_ps((&this->values0)[Index / 4], shuffle);
                value = _mm_sub_ss(value, other.values);
                (&this->values0)[Index / 4] = _mm_shuffle1_ps(value, shuffle);
            }
        } else
#endif
        {
            (&this->values0)[Index] -= other.value;
        }
    }

    /**
     * Multiply an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-5).
     * @param other The value to multiply by.
     */
    template<uint32 Index>
    XS_FUNCTION void mulValue(InBaseDef other) noexcept
    {
        static_assert(Index < 6, "Invalid Index: Index must be <6");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                this->values =
                    _mm256_blend_mul_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(other.values));
            } else if constexpr (Index == 1) {
                const __m128 value = _mm_shuffle2200_ps(other.values);
                this->values =
                    _mm256_blend_mul_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(value));
            } else if constexpr (Index == 2 || Index == 3) {
                const __m128 value = _mm_shuffle0000_ps(other.values);
                this->values =
                    _mm256_blend_mul_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(value));
            } else if constexpr (Index == 4) {
                this->values =
                    _mm256_blend_mul_ps(this->values, 1 << Index, this->values, _mm256_broadcastf128_ps(other.values));
            } else /*Index == 5*/ {
                const __m128 value = _mm_shuffle2200_ps(other.values);
                this->values =
                    _mm256_blend_mul_ps(this->values, 1 << Index, this->values, _mm256_broadcastf128_ps(value));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 4 == 0) {
                (&this->values0)[Index / 4] = _mm_mul_ss((&this->values0)[Index / 4], other.values);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index % 4 == 1) {
                const __m128 value = _mm_shuffle2200_ps(other.values);
                (&this->values0)[Index / 4] =
                    _mm_blend_mul_ps((&this->values0)[Index / 4], 1 << (Index % 4), (&this->values0)[Index / 4], value);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                const __m128 value = _mm_shuffle0000_ps(other.values);
                (&this->values0)[Index / 4] =
                    _mm_blend_mul_ps((&this->values0)[Index / 4], 1 << (Index % 4), (&this->values0)[Index / 4], value);
            } else {
                constexpr auto shuffle = _MM_SHUFFLE(
                    (Index % 4) == 3 ? 0 : 3, (Index % 4) == 2 ? 0 : 2, (Index % 4) == 1 ? 0 : 1, (Index % 4));
                __m128 value = _mm_shuffle1_ps((&this->values0)[Index / 4], shuffle);
                value = _mm_mul_ss(value, other.values);
                (&this->values0)[Index / 4] = _mm_shuffle1_ps(value, shuffle);
            }
        } else
#endif
        {
            (&this->values0)[Index] *= other.value;
        }
    }

    /**
     * Divide an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-5).
     * @param other The value to divide by.
     */
    template<uint32 Index>
    XS_FUNCTION void divValue(InBaseDef other) noexcept
    {
        static_assert(Index < 6, "Invalid Index: Index must be <6");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                this->values =
                    _mm256_blend_div_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(other.values));
            } else if constexpr (Index == 1) {
                const __m128 value = _mm_shuffle2200_ps(other.values);
                this->values =
                    _mm256_blend_div_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(value));
            } else if constexpr (Index == 2 || Index == 3) {
                const __m128 value = _mm_shuffle0000_ps(other.values);
                this->values =
                    _mm256_blend_div_ps(this->values, 1 << Index, this->values, _mm256_castps128_ps256(value));
            } else if constexpr (Index == 4) {
                const __m128 value = _mm_div_ss(_mm256_extractf128_ps(this->values, 1), other.values);
                this->values = _mm256_insertf128_ps(this->values, value, 1);
            } else /*Index == 5*/ {
                const __m128 hi = _mm256_extractf128_ps(this->values, 1);
                __m128 value = _mm_shuffle2200_ps(other.values);
                value = _mm_blend_div_ps(hi, 1 << (Index % 4), hi, value);
                this->values = _mm256_insertf128_ps(this->values, value, 1);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 4 == 0) {
                (&this->values0)[Index / 4] = _mm_div_ss((&this->values0)[Index / 4], other.values);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index % 4 == 1) {
                const __m128 value = _mm_shuffle2200_ps(other.values);
                (&this->values0)[Index / 4] =
                    _mm_blend_div_ps((&this->values0)[Index / 4], 1 << (Index % 4), (&this->values0)[Index / 4], value);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                const __m128 value = _mm_shuffle0000_ps(other.values);
                (&this->values0)[Index / 4] =
                    _mm_blend_div_ps((&this->values0)[Index / 4], 1 << (Index % 4), (&this->values0)[Index / 4], value);
            } else {
                constexpr auto shuffle = _MM_SHUFFLE(
                    (Index % 4) == 3 ? 0 : 3, (Index % 4) == 2 ? 0 : 2, (Index % 4) == 1 ? 0 : 1, (Index % 4));
                __m128 value = _mm_shuffle1_ps((&this->values0)[Index / 4], shuffle);
                value = _mm_div_ss(value, other.values);
                (&this->values0)[Index / 4] = _mm_shuffle1_ps(value, shuffle);
            }
        } else
#endif
        {
            (&this->values0)[Index] /= other.value;
        }
    }

    /**
     * Multiply then add to an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-5).
     * @param other1 The value to multiply by.
     * @param other2 The value to add.
     */
    template<uint32 Index>
    XS_FUNCTION void madValue(InBaseDef other1, InBaseDef other2) noexcept
    {
        static_assert(Index < 6, "Invalid Index: Index must be <6");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index == 0) {
                this->values = _mm256_blend_fmadd_ps(this->values, 1 << Index, _mm256_castps128_ps256(other1.values),
                    _mm256_castps128_ps256(other2.values));
            } else if constexpr (Index == 1) {
                const __m128 value1 = _mm_shuffle2200_ps(other1.values);
                const __m128 value2 = _mm_shuffle2200_ps(other2.values);
                this->values = _mm256_blend_fmadd_ps(
                    this->values, 1 << Index, _mm256_castps128_ps256(value1), _mm256_castps128_ps256(value2));
            } else if constexpr (Index == 2 || Index == 3) {
                const __m128 value1 = _mm_shuffle0000_ps(other1.values);
                const __m128 value2 = _mm_shuffle0000_ps(other2.values);
                this->values = _mm256_blend_fmadd_ps(
                    this->values, 1 << Index, _mm256_castps128_ps256(value1), _mm256_castps128_ps256(value2));
            } else if constexpr (Index == 4) {
                this->values = _mm256_blend_fmadd_ps(this->values, 1 << Index, _mm256_broadcastf128_ps(other1.values),
                    _mm256_broadcastf128_ps(other2.values));
            } else /*Index == 5*/ {
                const __m128 value1 = _mm_shuffle2200_ps(other1.values);
                const __m128 value2 = _mm_shuffle2200_ps(other2.values);
                this->values = _mm256_blend_fmadd_ps(
                    this->values, 1 << Index, _mm256_broadcastf128_ps(value1), _mm256_broadcastf128_ps(value2));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index % 4 == 0) {
                (&this->values0)[Index / 4] = _mm_fmadd_ss((&this->values0)[Index / 4], other1.values, other2.values);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index % 4 == 1) {
                const __m128 value1 = _mm_shuffle2200_ps(other1.values);
                const __m128 value2 = _mm_shuffle2200_ps(other2.values);
                (&this->values0)[Index / 4] =
                    _mm_blend_fmadd_ps((&this->values0)[Index / 4], 1 << (Index % 4), value1, value2);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                const __m128 value1 = _mm_shuffle0000_ps(other1.values);
                const __m128 value2 = _mm_shuffle0000_ps(other2.values);
                (&this->values0)[Index / 4] =
                    _mm_blend_fmadd_ps((&this->values0)[Index / 4], 1 << (Index % 4), value1, value2);
            } else {
                constexpr auto shuffle = _MM_SHUFFLE(
                    (Index % 4) == 3 ? 0 : 3, (Index % 4) == 2 ? 0 : 2, (Index % 4) == 1 ? 0 : 1, (Index % 4));
                __m128 value = _mm_shuffle1_ps((&this->values0)[Index / 4], shuffle);
                value = _mm_fmadd_ss(value, other1.values, other2.values);
                (&this->values0)[Index / 4] = _mm_shuffle1_ps(value, shuffle);
            }
        } else
#endif
        {
            (&this->values0)[Index] = Shift::fma<T>((&this->values0)[Index], other1.value, other2.value);
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
    XS_FUNCTION SIMD6 negate() const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5) {
                return SIMD6(_mm256_xor_ps(this->values, _mm256_set1_ps(-0.0f)));
            } else {
                return SIMD6(_mm256_xor_ps(this->values,
                    _mm256_set_ps(0.0f, 0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f, Elem3 ? -0.0f : 0.0f,
                        Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && !Elem5) {
                const __m128 value = _mm_set_ss(-0.0f);
                return SIMD6(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5) {
                const __m128 value = _mm_set1_ps(-0.0f);
                return SIMD6(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && !Elem4 && !Elem5) {
                return SIMD6(_mm_xor_ps(this->values0, _mm_set1_ps(-0.0f)), this->values1);
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && Elem4 && Elem5) {
                return SIMD6(this->values0, _mm_xor_ps(this->values1, _mm_set1_ps(-0.0f)));
            } else if constexpr (Elem0 == Elem4 && Elem1 == Elem5) {
                const __m128 value =
                    _mm_set_ps(Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f);
                return SIMD6(_mm_xor_ps(this->values0, value), _mm_xor_ps(this->values1, value));
            } else {
                return SIMD6(_mm_xor_ps(this->values0,
                                 _mm_set_ps(Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f,
                                     Elem0 ? -0.0f : 0.0f)),
                    _mm_xor_ps(this->values1, _mm_set_ps(0.0f, 0.0f, Elem5 ? -0.0f : 0.0f, Elem4 ? -0.0f : 0.0f)));
            }
        }
#endif
        else {
            return SIMD6(Elem0 ? -this->values0 : this->values0, Elem1 ? -this->values1 : this->values1,
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
    XS_FUNCTION SIMD6 mad(const SIMD6& other1, const SIMD6& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD6(_mm256_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD6(_mm256_add_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD6(_mm_fmadd_ps(this->values0, other1.values0, other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values1, other2.values1));
            } else {
                return SIMD6(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1));
            }
        } else
#endif
        {
            return SIMD6(Shift::fma<T>(this->values0, other1.values0, other2.values0),
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
    XS_FUNCTION SIMD6 mad(BaseDef other1, const SIMD6& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD6(_mm256_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD6(_mm256_add_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD6(_mm_fmadd_ps(this->values0, other1.values, other2.values0),
                    _mm_fmadd_ps(this->values1, other1.values, other2.values1));
            } else {
                return SIMD6(_mm_add_ps(_mm_mul_ps(this->values0, other1.values), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values), other2.values1));
            }
        } else
#endif
        {
            return SIMD6(Shift::fma<T>(this->values0, other1.value, other2.values0),
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
    XS_FUNCTION SIMD6 mad(const SIMD6& other1, BaseDef other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD6(_mm256_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD6(_mm256_add_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD6(_mm_fmadd_ps(this->values0, other1.values0, other2.values),
                    _mm_fmadd_ps(this->values1, other1.values1, other2.values));
            } else {
                return SIMD6(_mm_add_ps(_mm_mul_ps(this->values0, other1.values0), other2.values),
                    _mm_add_ps(_mm_mul_ps(this->values1, other1.values1), other2.values));
            }
        } else
#endif
        {
            return SIMD6(Shift::fma<T>(this->values0, other1.values0, other2.value),
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
    XS_FUNCTION SIMD6 mad(const SIMD2Def& other1, const SIMD6& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val;
            if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                val = _mm256_broadcastf64_ps(other1.values);
            } else {
                const __m128 val2 = _mm_shuffle1010_ps(other1.values);
                val = _mm256_broadcastf128_ps(val2);
            }
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD6(_mm256_fmadd_ps(this->values, val, other2.values));
            } else {
                return SIMD6(_mm256_add_ps(_mm256_mul_ps(this->values, val), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_shuffle1010_ps(other1.values);
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD6(
                    _mm_fmadd_ps(this->values0, val, other2.values0), _mm_fmadd_ps(this->values1, val, other2.values1));
            } else {
                return SIMD6(_mm_add_ps(_mm_mul_ps(this->values0, val), other2.values0),
                    _mm_add_ps(_mm_mul_ps(this->values1, val), other2.values1));
            }
        } else
#endif
        {
            return SIMD6(Shift::fma<T>(this->values0, other1.values0, other2.values0),
                Shift::fma<T>(this->values1, other1.values1, other2.values1),
                Shift::fma<T>(this->values2, other1.values0, other2.values2),
                Shift::fma<T>(this->values3, other1.values1, other2.values3),
                Shift::fma<T>(this->values4, other1.values0, other2.values4),
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
    XS_FUNCTION SIMD6 mad(const SIMD3Def& other1, const SIMD6& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val0 = _mm256_set_m128(
                _mm_unpackhi_ps(other1.values, other1.values), _mm_unpacklo_ps(other1.values, other1.values));
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD6(_mm256_fmadd_ps(this->values, val0, other2.values));
            } else {
                return SIMD6(_mm256_add_ps(_mm256_mul_ps(this->values, val0), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD6(_mm_fmadd_ps(this->values0, _mm_unpacklo_ps(other1.values, other1.values), other2.values0),
                    _mm_fmadd_ps(this->values1, _mm_unpackhi_ps(other1.values, other1.values), other2.values1));
            } else {
                return SIMD6(_mm_add_ps(_mm_mul_ps(this->values0, _mm_unpacklo_ps(other1.values, other1.values)),
                                 other2.values0),
                    _mm_add_ps(
                        _mm_mul_ps(this->values1, _mm_unpackhi_ps(other1.values, other1.values)), other2.values1));
            }
        } else
#endif
        {
            return SIMD6(Shift::fma<T>(this->values0, other1.values0, other2.values0),
                Shift::fma<T>(this->values1, other1.values0, other2.values1),
                Shift::fma<T>(this->values2, other1.values1, other2.values2),
                Shift::fma<T>(this->values3, other1.values1, other2.values3),
                Shift::fma<T>(this->values4, other1.values2, other2.values4),
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
    XS_FUNCTION SIMD6 mad(const SIMD3Def& other1, const SIMD3Def& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val0 = _mm256_set_m128(
                _mm_unpackhi_ps(other1.values, other1.values), _mm_unpacklo_ps(other1.values, other1.values));
            const __m256 val1 = _mm256_set_m128(
                _mm_unpackhi_ps(other2.values, other2.values), _mm_unpacklo_ps(other2.values, other2.values));
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD6(_mm256_fmadd_ps(this->values, val0, val1));
            } else {
                return SIMD6(_mm256_add_ps(_mm256_mul_ps(this->values, val0), val1));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD6(_mm_fmadd_ps(this->values0, _mm_unpacklo_ps(other1.values, other1.values),
                                 _mm_unpacklo_ps(other2.values, other2.values)),
                    _mm_fmadd_ps(this->values1, _mm_unpackhi_ps(other1.values, other1.values),
                        _mm_unpackhi_ps(other2.values, other2.values)));
            } else {
                return SIMD6(_mm_add_ps(_mm_mul_ps(this->values0, _mm_unpacklo_ps(other1.values, other1.values)),
                                 _mm_unpacklo_ps(other2.values, other2.values)),
                    _mm_add_ps(_mm_mul_ps(this->values1, _mm_unpackhi_ps(other1.values, other1.values)),
                        _mm_unpackhi_ps(other2.values, other2.values)));
            }
        } else
#endif
        {
            return SIMD6(Shift::fma<T>(this->values0, other1.values0, other2.values0),
                Shift::fma<T>(this->values1, other1.values0, other2.values0),
                Shift::fma<T>(this->values2, other1.values1, other2.values1),
                Shift::fma<T>(this->values3, other1.values1, other2.values1),
                Shift::fma<T>(this->values4, other1.values2, other2.values2),
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
    XS_FUNCTION SIMD6 msub(const SIMD6& other1, const SIMD6& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD6(_mm256_fmsub_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD6(_mm256_sub_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD6(_mm_fmsub_ps(this->values0, other1.values0, other2.values0),
                    _mm_fmsub_ps(this->values1, other1.values1, other2.values1));
            } else {
                return SIMD6(_mm_sub_ps(_mm_mul_ps(this->values0, other1.values0), other2.values0),
                    _mm_sub_ps(_mm_mul_ps(this->values1, other1.values1), other2.values1));
            }
        } else
#endif
        {
            return SIMD6(Shift::fma<T>(this->values0, other1.values0, -other2.values0),
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
    XS_FUNCTION Mask equalMask(const SIMD6& other) const noexcept
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
    XS_FUNCTION Mask lessOrEqualMask(const SIMD6& other) const noexcept
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
    XS_FUNCTION Mask lessThanMask(const SIMD6& other) const noexcept
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
    XS_FUNCTION Mask notEqualMask(const SIMD6& other) const noexcept
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
    XS_FUNCTION Mask equalMask(BaseDef other) const noexcept
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
    XS_FUNCTION Mask lessOrEqualMask(BaseDef other) const noexcept
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
    XS_FUNCTION Mask lessThanMask(BaseDef other) const noexcept
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
    XS_FUNCTION Mask greaterOrEqualMask(BaseDef other) const noexcept
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
    XS_FUNCTION Mask greaterThanMask(BaseDef other) const noexcept
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
    XS_FUNCTION Mask notEqualMask(BaseDef other) const noexcept
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
    XS_FUNCTION SIMD6 sign(const SIMD6& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(_mm256_xor_ps(this->values, _mm256_and_ps(other.values, _mm256_set1_ps(-0.0f))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(_mm_xor_ps(this->values0, _mm_and_ps(_mm_set1_ps(-0.0f), other.values0)),
                _mm_xor_ps(this->values1, _mm_and_ps(_mm_set1_ps(-0.0f), other.values1)));
        } else
#endif
        {
            return SIMD6(Shift::sign<T>(this->values0, other.values0), Shift::sign<T>(this->values1, other.values1),
                Shift::sign<T>(this->values2, other.values2), Shift::sign<T>(this->values3, other.values3),
                Shift::sign<T>(this->values4, other.values4), Shift::sign<T>(this->values5, other.values5));
        }
    }

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD6 abs() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(_mm256_andnot_ps(_mm256_set1_ps(-0.0f), this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 abs = _mm_set1_ps(-0.0f);
            return SIMD6(_mm_andnot_ps(abs, this->values0), _mm_andnot_ps(abs, this->values1));
        } else
#endif
        {
            return SIMD6(Shift::abs<T>(this->values0), Shift::abs<T>(this->values1), Shift::abs<T>(this->values2),
                Shift::abs<T>(this->values3), Shift::abs<T>(this->values4), Shift::abs<T>(this->values5));
        }
    }

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMD6 max(const SIMD6& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(_mm256_max_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(_mm_max_ps(this->values0, other.values0), _mm_max_ps(this->values1, other.values1));
        } else
#endif
        {
            return SIMD6(Shift::max<T>(this->values0, other.values0), Shift::max<T>(this->values1, other.values1),
                Shift::max<T>(this->values2, other.values2), Shift::max<T>(this->values3, other.values3),
                Shift::max<T>(this->values4, other.values4), Shift::max<T>(this->values5, other.values5));
        }
    }

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMD6 min(const SIMD6& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(_mm256_min_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(_mm_min_ps(this->values0, other.values0), _mm_min_ps(this->values1, other.values1));
        } else
#endif
        {
            return SIMD6(Shift::min<T>(this->values0, other.values0), Shift::min<T>(this->values1, other.values1),
                Shift::min<T>(this->values2, other.values2), Shift::min<T>(this->values3, other.values3),
                Shift::min<T>(this->values4, other.values4), Shift::min<T>(this->values5, other.values5));
        }
    }

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMD6 max(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(_mm256_max_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(_mm_max_ps(this->values0, other.values), _mm_max_ps(this->values1, other.values));
        } else
#endif
        {
            return SIMD6(Shift::max<T>(this->values0, other.value), Shift::max<T>(this->values1, other.value),
                Shift::max<T>(this->values2, other.value), Shift::max<T>(this->values3, other.value),
                Shift::max<T>(this->values4, other.value), Shift::max<T>(this->values5, other.value));
        }
    }

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMD6 min(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(_mm256_min_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(_mm_min_ps(this->values0, other.values), _mm_min_ps(this->values1, other.values));
        } else
#endif
        {
            return SIMD6(Shift::min<T>(this->values0, other.value), Shift::min<T>(this->values1, other.value),
                Shift::min<T>(this->values2, other.value), Shift::min<T>(this->values3, other.value),
                Shift::min<T>(this->values4, other.value), Shift::min<T>(this->values5, other.value));
        }
    }

    /**
     * Maximum of each internally stored SIMD2.
     * @returns SIMD2 set to the largest value in each internal SIMD2.
     */
    XS_FUNCTION SIMD2Def max2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val = _mm_max_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values));
            return SIMD2Def(_mm_max_ps(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values)), val));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_max_ps(this->values0, this->values1);
            return SIMD2Def(_mm_max_ps(_mm_shuffle3232_ps(this->values0), val));
        } else
#endif
        {
            const T other1 = Shift::max<T>(this->values0, this->values2);
            const T other2 = Shift::max<T>(this->values1, this->values3);
            return SIMD2Def(Shift::max<T>(other1, this->values4), Shift::max<T>(other2, this->values5));
        }
    }

    /**
     * Minimum of each internally stored SIMD2.
     * @returns SIMD2 set to the largest value in each internal SIMD2.
     */
    XS_FUNCTION SIMD2Def min2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val = _mm_min_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values));
            return SIMD2Def(_mm_min_ps(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values)), val));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_min_ps(this->values0, this->values1);
            return SIMD2Def(_mm_min_ps(_mm_shuffle3232_ps(this->values0), val));
        } else
#endif
        {
            const T other1 = Shift::min<T>(this->values0, this->values2);
            const T other2 = Shift::min<T>(this->values1, this->values3);
            return SIMD2Def(Shift::min<T>(other1, this->values4), Shift::min<T>(other2, this->values5));
        }
    }

    /**
     * Maximum of each internally stored SIMD3.
     * @returns SIMD3 set to the largest value in each internal SIMD3.
     */
    XS_FUNCTION SIMD3Def max3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 other = _mm256_extractf128_ps(this->values, 1);
            return SIMD3Def(
                _mm_max_ps(_mm_shuffle_ps(_mm256_castps256_ps128(this->values), other, _MM_SHUFFLE(0, 0, 2, 0)),
                    _mm_shuffle_ps(_mm256_castps256_ps128(this->values), other, _MM_SHUFFLE(0, 1, 3, 1))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3Def(_mm_max_ps(_mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(0, 0, 2, 0)),
                _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(0, 1, 3, 1))));
        } else
#endif
        {
            return SIMD3Def(Shift::max<T>(this->values0, this->values1), Shift::max<T>(this->values2, this->values3),
                Shift::max<T>(this->values4, this->values5));
        }
    }

    /**
     * Minimum of each internally stored SIMD3.
     * @returns SIMD3 set to the largest value in each internal SIMD3.
     */
    XS_FUNCTION SIMD3Def min3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 other = _mm256_extractf128_ps(this->values, 1);
            return SIMD3Def(
                _mm_min_ps(_mm_shuffle_ps(_mm256_castps256_ps128(this->values), other, _MM_SHUFFLE(0, 0, 2, 0)),
                    _mm_shuffle_ps(_mm256_castps256_ps128(this->values), other, _MM_SHUFFLE(0, 1, 3, 1))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3Def(_mm_min_ps(_mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(0, 0, 2, 0)),
                _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(0, 1, 3, 1))));
        } else
#endif
        {
            return SIMD3Def(Shift::min<T>(this->values0, this->values1), Shift::min<T>(this->values2, this->values3),
                Shift::min<T>(this->values4, this->values5));
        }
    }

    /**
     * Maximum element of this object.
     * @returns SIMDBase set to the largest value.
     */
    XS_FUNCTION BaseDef hmax() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val = _mm256_max_ps(_mm256_shuffle32107654_ps(this->values), this->values);
            val = _mm256_max_ps(_mm256_shuffle1_ps(this->values, _MM_SHUFFLE(1, 0, 3, 2)), val);
            val = _mm256_max_ps(_mm256_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val);
            return BaseDef(_mm256_shuffle1010_ps(val));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val = _mm_max_ps(this->values0, this->values1);
            val = _mm_max_ps(_mm_shuffle3232_ps(this->values0), _mm_shuffle1010_ps(val));
            return BaseDef(_mm_max_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
        } else
#endif
        {
            T other = Shift::max<T>(this->values0, this->values1);
            const T other2 = Shift::max<T>(this->values2, this->values3);
            const T other3 = Shift::max<T>(this->values4, this->values5);
            other = Shift::max<T>(other, other2);
            return BaseDef(Shift::max<T>(other, other3));
        }
    }

    /**
     * Minimum element of this object.
     * @returns SIMDBase set to the smallest value.
     */
    XS_FUNCTION BaseDef hmin() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val = _mm256_min_ps(_mm256_shuffle32107654_ps(this->values), this->values);
            val = _mm256_min_ps(_mm256_shuffle1_ps(this->values, _MM_SHUFFLE(1, 0, 3, 2)), val);
            val = _mm256_min_ps(_mm256_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val);
            return BaseDef(_mm256_shuffle1010_ps(val));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val = _mm_min_ps(this->values0, this->values1);
            val = _mm_min_ps(_mm_shuffle3232_ps(this->values0), _mm_shuffle1010_ps(val));
            return BaseDef(_mm_min_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
        } else
#endif
        {
            T other = Shift::min<T>(this->values0, this->values1);
            const T other2 = Shift::min<T>(this->values2, this->values3);
            const T other3 = Shift::min<T>(this->values4, this->values5);
            other = Shift::min<T>(other, other2);
            return BaseDef(Shift::min<T>(other, other3));
        }
    }

    /**
     * Maximum element of this object.
     * @returns SIMDInBase set to the largest value.
     */
    XS_FUNCTION InBaseDef hmaxInBase() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m128 val = _mm_max_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values));
            val = _mm_max_ps(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values)),
                val);                                                   //(x,x,3,2)
            return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val = _mm_max_ps(this->values0, this->values1);
            val = _mm_max_ps(_mm_shuffle3232_ps(this->values0), val);   //(x,x,3,2)
            return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
        } else
#endif
        {
            return InBaseDef(hmax());
        }
    }

    /**
     * Minimum element this object returned as a SIMDInBase.
     * @returns SIMDInBase set to the smallest value.
     */
    XS_FUNCTION InBaseDef hminInBase() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m128 val = _mm_min_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values));
            val = _mm_min_ps(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values)), val);
            return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val = _mm_min_ps(this->values0, this->values1);
            val = _mm_min_ps(_mm_shuffle3232_ps(this->values0), val);   //(x,x,3,2)
            return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
        } else
#endif
        {
            return InBaseDef(hmin());
        }
    }

    /**
     * Maximum element of this object along with the max elements index.
     * @param [out] index Return value used to pass back the index of the returned element.
     * @returns SIMDBase set to the largest value.
     */
    XS_FUNCTION BaseDef hmaxIndex(uint32& index) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val = _mm256_max_ps(_mm256_shuffle32107654_ps(this->values), this->values);
            val = _mm256_max_ps(_mm256_shuffle1_ps(this->values, _MM_SHUFFLE(1, 0, 3, 2)), val);
            val = _mm256_max_ps(_mm256_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val);
            val = _mm256_shuffle1010_ps(val);
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                index = _cvtmask8_u32(_mm256_cmp_ps_mask(this->values, val, _CMP_EQ_OQ));
            } else {
                index = _mm256_movemask_ps(_mm256_cmp_ps(this->values, val, _CMP_EQ_OQ));
            }
            index = ctz(index);
            return BaseDef(val);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val = _mm_max_ps(this->values0, this->values1);
            val = _mm_max_ps(_mm_shuffle3232_ps(this->values0), _mm_shuffle1010_ps(val));
            val = _mm_max_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val);
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                index = _cvtmask8_u32(_mm256_cmp_ps_mask(
                    _mm256_set_m128(this->values1, this->values0), _mm256_broadcastf128_ps(val), _CMP_EQ_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                index = _mm256_movemask_ps(_mm256_cmp_ps(
                    _mm256_set_m128(this->values1, this->values0), _mm256_broadcastf128_ps(val), _CMP_EQ_OQ));
            } else {
                index = _mm_movemask_ps(_mm_cmpeq_ps(this->values0, val));
                index |= _mm_movemask_ps(_mm_cmpeq_ps(this->values1, val)) << 4;
            }
            index = ctz(index);
            return BaseDef(val);
        } else
#endif
        {
            T other = Shift::max<T>(this->values0, this->values1);
            index = (other == this->values0) ? 0 : 1;
            T other2 = Shift::max<T>(this->values2, this->values3);
            uint32 index2 = (other2 == this->values2) ? 2 : 3;
            const T other3 = Shift::max<T>(this->values4, this->values5);
            const uint32 index3 = (other3 == this->values4) ? 4 : 5;
            other2 = Shift::max<T>(other2, other);
            index2 = (other2 == other) ? index : index2;
            other = Shift::max<T>(other2, other3);
            index = (other == other2) ? index2 : index3;
            return BaseDef(other);
        }
    }

    /**
     * Minimum element of this object along with the max elements index.
     * @param [out] index Return value used to pass back the index of the returned element.
     * @returns SIMDBase set to the smallest value.
     */
    XS_FUNCTION BaseDef hminIndex(uint32& index) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val = _mm256_min_ps(_mm256_shuffle32107654_ps(this->values), this->values);
            val = _mm256_min_ps(_mm256_shuffle1_ps(this->values, _MM_SHUFFLE(1, 0, 3, 2)), val);
            val = _mm256_min_ps(_mm256_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val);
            val = _mm256_shuffle1010_ps(val);
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                index = _cvtmask8_u32(_mm256_cmp_ps_mask(this->values, val, _CMP_EQ_OQ));
            } else {
                index = _mm256_movemask_ps(_mm256_cmp_ps(this->values, val, _CMP_EQ_OQ));
            }
            index = ctz(index);
            return BaseDef(val);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val = _mm_min_ps(this->values0, this->values1);
            val = _mm_min_ps(_mm_shuffle3232_ps(this->values0), _mm_shuffle1010_ps(val));
            val = _mm_min_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val);
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                index = _cvtmask8_u32(_mm256_cmp_ps_mask(
                    _mm256_set_m128(this->values1, this->values0), _mm256_broadcastf128_ps(val), _CMP_EQ_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                index = _mm256_movemask_ps(_mm256_cmp_ps(
                    _mm256_set_m128(this->values1, this->values0), _mm256_broadcastf128_ps(val), _CMP_EQ_OQ));
            } else {
                index = _mm_movemask_ps(_mm_cmpeq_ps(this->values0, val));
                index |= _mm_movemask_ps(_mm_cmpeq_ps(this->values1, val)) << 4;
            }
            index = ctz(index);
            return BaseDef(val);
        } else
#endif
        {
            T other = Shift::min<T>(this->values0, this->values1);
            index = (other == this->values0) ? 0 : 1;
            T other2 = Shift::min<T>(this->values2, this->values3);
            uint32 index2 = (other2 == this->values2) ? 2 : 3;
            const T other3 = Shift::min<T>(this->values4, this->values5);
            const uint32 index3 = (other3 == this->values4) ? 4 : 5;
            other2 = Shift::min<T>(other2, other);
            index2 = (other2 == other) ? index : index2;
            other = Shift::min<T>(other2, other3);
            index = (other == other2) ? index2 : index3;
            return BaseDef(other);
        }
    }

    /**
     * Maximum element from each SIMD2.
     * @returns SIMD3 set to the largest value in each SIMD2.
     */
    XS_FUNCTION SIMD3Def hmax2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 other = _mm256_extractf128_ps(this->values, 1);
            return SIMD3Def(
                _mm_max_ps(_mm_shuffle_ps(_mm256_castps256_ps128(this->values), other, _MM_SHUFFLE(0, 0, 2, 0)),
                    _mm_shuffle_ps(_mm256_castps256_ps128(this->values), other, _MM_SHUFFLE(0, 1, 3, 1))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3Def(_mm_max_ps(_mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(0, 0, 2, 0)),
                _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(0, 1, 3, 1))));
        } else
#endif
        {
            return SIMD3Def(Shift::max<T>(this->values0, this->values1), Shift::max<T>(this->values2, this->values3),
                Shift::max<T>(this->values4, this->values5));
        }
    }

    /**
     * Minimum element from each SIMD2.
     * @returns SIMD3 set to the smallest value in each SIMD2.
     */
    XS_FUNCTION SIMD3Def hmin2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 other = _mm256_extractf128_ps(this->values, 1);
            return SIMD3Def(
                _mm_min_ps(_mm_shuffle_ps(_mm256_castps256_ps128(this->values), other, _MM_SHUFFLE(0, 0, 2, 0)),
                    _mm_shuffle_ps(_mm256_castps256_ps128(this->values), other, _MM_SHUFFLE(0, 1, 3, 1))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3Def(_mm_min_ps(_mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(0, 0, 2, 0)),
                _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(0, 1, 3, 1))));
        } else
#endif
        {
            return SIMD3Def(Shift::min<T>(this->values0, this->values1), Shift::min<T>(this->values2, this->values3),
                Shift::min<T>(this->values4, this->values5));
        }
    }

    /**
     * Maximum element from each SIMD3.
     * @returns SIMD2 set to the largest value in each SIMD3.
     */
    XS_FUNCTION SIMD2Def hmax3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val = _mm_max_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values));
            return SIMD2Def(_mm_max_ps(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values)), val));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_max_ps(this->values0, this->values1);
            return SIMD2Def(_mm_max_ps(_mm_shuffle3232_ps(this->values0), val));
        } else
#endif
        {
            const T other1 = Shift::max<T>(this->values0, this->values2);
            const T other2 = Shift::max<T>(this->values1, this->values3);
            return SIMD2Def(Shift::max<T>(this->values4, other1), Shift::max<T>(this->values5, other2));
        }
    }

    /**
     * Minimum element from each SIMD3.
     * @returns SIMD2 set to the largest value in each SIMD3.
     */
    XS_FUNCTION SIMD2Def hmin3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val = _mm_min_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values));
            return SIMD2Def(_mm_min_ps(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values)), val));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_min_ps(this->values0, this->values1);
            return SIMD2Def(_mm_min_ps(_mm_shuffle3232_ps(this->values0), val));
        } else
#endif
        {
            const T other1 = Shift::min<T>(this->values0, this->values2);
            const T other2 = Shift::min<T>(this->values1, this->values3);
            return SIMD2Def(Shift::min<T>(this->values4, other1), Shift::min<T>(this->values5, other2));
        }
    }

    /**
     * Approximate reciprocal (1/other) of SIMD6 per element.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD6 reciprocal() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 recip = _mm256_recip_ps(this->values);
            return SIMD6(_mm256_fnmadd_ps(_mm256_mul_ps(recip, recip), this->values, _mm256_add_ps(recip, recip)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 recip0 = _mm_recip_ps(this->values0);
            const __m128 recip1 = _mm_recip_ps(this->values1);
            return SIMD6(_mm_fnmadd_ps(_mm_mul_ps(recip0, recip0), this->values0, _mm_add_ps(recip0, recip0)),
                _mm_fnmadd_ps(_mm_mul_ps(recip1, recip1), this->values1, _mm_add_ps(recip1, recip1)));
        } else
#endif
        {
            return SIMD6(Shift::recip<T>(this->values0), Shift::recip<T>(this->values1), Shift::recip<T>(this->values2),
                Shift::recip<T>(this->values3), Shift::recip<T>(this->values4), Shift::recip<T>(this->values5));
        }
    }

    /**
     * Compute the sum of each internally stored SIMD2.
     * @returns SIMD2 set to the sum of all elements between each internal SIMD2.
     */
    XS_FUNCTION SIMD2Def add2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val = _mm_add_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values));
            return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values)),
                val)); //(x,x,3,2)
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_add_ps(this->values0, this->values1);
            return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(this->values0), val)); //(x,x,3,2)
        } else
#endif
        {
            T res1 = (this->values0 + this->values2);
            T res2 = (this->values1 + this->values3);
            res1 += this->values4;
            res2 += this->values5;
            return SIMD2Def(res1, res2);
        }
    }

    /**
     * Compute the sum of each internally stored SIMD3.
     * @returns SIMD3 set to the sum of all elements between each internal SIMD3.
     */
    XS_FUNCTION SIMD3Def add3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 top = _mm256_extractf128_ps(this->values, 1);
            return SIMD3Def(_mm_hadd_ps(_mm256_castps256_ps128(this->values), top));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3Def(_mm_hadd_ps(this->values0, this->values1));
        } else
#endif
        {
            const T res1 = (this->values0 + this->values1);
            const T res2 = (this->values2 + this->values3);
            const T res3 = (this->values4 + this->values5);
            return SIMD3Def(res1, res2, res3);
        }
    }

    /**
     * Compute the difference of each internally stored SIMD3.
     * @returns SIMD3 set to the difference of all elements between each internal SIMD3.
     */
    XS_FUNCTION SIMD3Def sub3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 top = _mm256_extractf128_ps(this->values, 1);
            return SIMD3Def(_mm_hsub_ps(_mm256_castps256_ps128(this->values), top));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3Def(_mm_hsub_ps(this->values0, this->values1));
        } else
#endif
        {
            const T res1 = (this->values0 - this->values1);
            const T res2 = (this->values2 - this->values3);
            const T res3 = (this->values4 - this->values5);
            return SIMD3Def(res1, res2, res3);
        }
    }

    /**
     * Compute the sum of all elements.
     * @returns BaseDef set to the sum of all elements.
     */
    XS_FUNCTION BaseDef hadd() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val0 = _mm256_shuffle32103210_ps(this->values); // prevent second high from affecting result
            __m256 val1 = _mm256_add_ps(_mm256_shuffle32107654_ps(this->values), this->values);
            val1 = _mm256_add_ps(_mm256_shuffle1_ps(val0, _MM_SHUFFLE(3, 2, 3, 2)),
                _mm256_shuffle1_ps(val1, _MM_SHUFFLE(1, 0, 1, 0))); // additional permutes avoid second high affecting
            return BaseDef(_mm256_add_ps(_mm256_shuffle1_ps(val1, _MM_SHUFFLE(2, 3, 0, 1)), val1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val = _mm_add_ps(this->values0, this->values1);
            val = _mm_add_ps(_mm_shuffle3232_ps(this->values0), _mm_shuffle1010_ps(val));
            return BaseDef(_mm_add_ps(_mm_shuffle1_ps(val, _MM_SHUFFLE(2, 3, 0, 1)), val));
        } else
#endif
        {
            T res = (this->values0 + this->values1);
            T res2 = (this->values2 + this->values3);
            T res3 = (this->values4 + this->values5);
            res += res2;
            res += res3;
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
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m128 val = _mm_add_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values));
            val = _mm_add_ps(_mm_movehl_ps(_mm256_castps256_ps128(this->values), _mm256_castps256_ps128(this->values)),
                val);                                                   //(x,x,3,2)
            return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val = _mm_add_ps(this->values0, this->values1);
            val = _mm_add_ps(_mm_shuffle3232_ps(this->values0), val);   //(x,x,3,2)
            return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(val), val)); //(x,x,x,1)
        } else
#endif
        {
            return InBaseDef(hadd());
        }
    }

    /**
     * Compute the sum of each SIMD2.
     * @returns SIMD3 set to the sum of all elements in each SIMD2.
     */
    XS_FUNCTION SIMD3Def hadd2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 top = _mm256_extractf128_ps(this->values, 1);
            return SIMD3Def(_mm_hadd_ps(_mm256_castps256_ps128(this->values), top));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3Def(_mm_hadd_ps(this->values0, this->values1));
        } else
#endif
        {
            return SIMD3Def(
                this->values0 + this->values1, this->values2 + this->values3, this->values4 + this->values5);
        }
    }

    /**
     * Compute the sum of each SIMD3.
     * @returns SIMD2 set to the sum of all elements in each SIMD3.
     */
    XS_FUNCTION SIMD2Def hadd3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 val = _mm_add_ps(_mm256_extractf128_ps(this->values, 1), _mm256_castps256_ps128(this->values));
            return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values)),
                val)); //(x,x,3,2)
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_add_ps(this->values0, this->values1);
            return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(this->values0), val)); //(x,x,3,2)
        } else
#endif
        {
            T res = (this->values0 + this->values2);
            T res2 = (this->values1 + this->values3);
            res += this->values4;
            res2 += this->values5;
            return SIMD2Def(res, res2);
        }
    }

    /**
     * Compute the difference of each SIMD2.
     * @returns SIMD6 set to the difference of all elements in each SIMD2.
     */
    XS_FUNCTION SIMD3Def hsub2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 top = _mm256_extractf128_ps(this->values, 1);
            return SIMD3Def(_mm_hsub_ps(_mm256_castps256_ps128(this->values), top));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3Def(_mm_hsub_ps(this->values0, this->values1));
        } else
#endif
        {
            return SIMD3Def(
                this->values0 - this->values1, this->values2 - this->values3, this->values4 - this->values5);
        }
    }

    /**
     * Round up each element of the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD6 ceil() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(_mm256_round_ps(this->values, FROUND_CEIL));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(_mm_ceil_ps(this->values0), _mm_ceil_ps(this->values1));
        } else
#endif
        {
            return SIMD6(Shift::ceil<T>(this->values0), Shift::ceil<T>(this->values1), Shift::ceil<T>(this->values2),
                Shift::ceil<T>(this->values3), Shift::ceil<T>(this->values4), Shift::ceil<T>(this->values5));
        }
    }

    /**
     * Round down each element of the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD6 floor() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(_mm256_round_ps(this->values, FROUND_FLOOR));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(_mm_floor_ps(this->values0), _mm_floor_ps(this->values1));
        } else
#endif
        {
            return SIMD6(Shift::floor<T>(this->values0), Shift::floor<T>(this->values1), Shift::floor<T>(this->values2),
                Shift::floor<T>(this->values3), Shift::floor<T>(this->values4), Shift::floor<T>(this->values5));
        }
    }

    /**
     * Round toward zero the object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD6 trunc() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(_mm256_round_ps(this->values, FROUND_TRUNC));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(_mm_trunc_ps(this->values0), _mm_trunc_ps(this->values1));
        } else
#endif
        {
            return SIMD6(Shift::trunc<T>(this->values0), Shift::trunc<T>(this->values1), Shift::trunc<T>(this->values2),
                Shift::trunc<T>(this->values3), Shift::trunc<T>(this->values4), Shift::trunc<T>(this->values5));
        }
    }

    /**
     * Returns the sqrt of each element in the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD6 sqrt() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(_mm256_sqrt_ps(this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(_mm_sqrt_ps(this->values0), _mm_sqrt_ps(this->values1));
        } else
#endif
        {
            return SIMD6(Shift::sqrt<T>(this->values0), Shift::sqrt<T>(this->values1), Shift::sqrt<T>(this->values2),
                Shift::sqrt<T>(this->values3), Shift::sqrt<T>(this->values4), Shift::sqrt<T>(this->values5));
        }
    }

    /**
     * Approximate reciprocal square root of each element in the object.
     * @note Useful as the reciprocal square root is faster to determine.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD6 rsqrt() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 recip = _mm256_recipsqrt_ps(this->values);
            const __m256 val1 = _mm256_mul_ps(_mm256_mul_ps(_mm256_set1_ps(0.5f), this->values), recip);
            const __m256 val2 = _mm256_fnmadd_ps(recip, val1, _mm256_set1_ps(1.5f));
            return SIMD6(_mm256_mul_ps(recip, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 recip0 = _mm_recipsqrt_ps(this->values0);
            const __m128 recip1 = _mm_recipsqrt_ps(this->values1);
            const __m128 half = _mm_set1_ps(0.5f);
            const __m128 threeHalf = _mm_set1_ps(1.5f);
            const __m128 val01 = _mm_mul_ps(_mm_mul_ps(half, this->values0), recip0);
            const __m128 val11 = _mm_mul_ps(_mm_mul_ps(half, this->values1), recip1);
            const __m128 val02 = _mm_fnmadd_ps(recip0, val01, threeHalf);
            const __m128 val12 = _mm_fnmadd_ps(recip1, val11, threeHalf);
            return SIMD6(_mm_mul_ps(recip0, val02), _mm_mul_ps(recip1, val12));
        } else
#endif
        {
            return SIMD6(Shift::rsqrt<T>(this->values0), Shift::rsqrt<T>(this->values1), Shift::rsqrt<T>(this->values2),
                Shift::rsqrt<T>(this->values3), Shift::rsqrt<T>(this->values4), Shift::rsqrt<T>(this->values5));
        }
    }

    /**
     * Compute the 3 dimensional dot product of each SIMD3 in two objects.
     * @param other The second object.
     * @returns The value of the dot products.
     */
    XS_FUNCTION SIMD2Def dot3(const SIMD6& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 sq = _mm256_mul_ps(this->values, other.values);
            const __m128 val = _mm_add_ps(_mm256_extractf128_ps(sq, 1), _mm256_castps256_ps128(sq));
            return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(_mm256_castps256_ps128(sq)), val));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 sq0 = _mm_mul_ps(this->values0, other.values0);
            const __m128 sq1 = _mm_mul_ps(this->values1, other.values1);
            const __m128 val = _mm_add_ps(sq0, sq1);
            return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(sq0), val));
        } else
#endif
        {
            T sq0 = this->values0 * other.values0;
            T sq1 = this->values1 * other.values1;
            sq0 = Shift::fma<T>(this->values2, other.values2, sq0);
            sq1 = Shift::fma<T>(this->values3, other.values3, sq1);
            return SIMD2Def(
                Shift::fma<T>(this->values4, other.values4, sq0), Shift::fma<T>(this->values5, other.values5, sq1));
        }
    }

    /**
     * Compute the 3 dimensional cross product of each SIMD3 in two objects.
     * @note Uses Right-handed coordinate system.
     * @param other The second object.
     * @returns The cross product of the inputs.
     */
    XS_FUNCTION SIMD6 cross3(const SIMD6& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            __m256 val0 =
                _mm256_shuffle_ps(other.values, _mm256_shuffle32107654_ps(other.values), _MM_SHUFFLE(1, 0, 3, 2));
            val0 = _mm256_permute2f128_ps(val0, other.values, _MM256_PERMUTE(2, 0));
            val0 = _mm256_mul_ps(val0, this->values);
            __m256 val1 =
                _mm256_shuffle_ps(this->values, _mm256_shuffle32107654_ps(this->values), _MM_SHUFFLE(1, 0, 3, 2));
            val1 = _mm256_permute2f128_ps(val1, this->values, _MM256_PERMUTE(2, 0));
            val1 = _mm256_mul_ps(val1, other.values);
            val0 = _mm256_sub_ps(val0, val1);
            val1 = _mm256_shuffle_ps(val0, _mm256_shuffle32107654_ps(val0), _MM_SHUFFLE(1, 0, 3, 2));
            return SIMD6(_mm256_permute2f128_ps(val1, val0, _MM256_PERMUTE(2, 0)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val0 = _mm_shuffle_ps(other.values0, other.values1, _MM_SHUFFLE(1, 0, 3, 2));
            val0 = _mm_mul_ps(val0, this->values0);
            __m128 val1 = _mm_mul_ps(this->values1, other.values0);
            __m128 val2 = _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(1, 0, 3, 2));
            val2 = _mm_mul_ps(val2, other.values0);
            const __m128 val3 = _mm_mul_ps(this->values0, other.values1);
            val0 = _mm_sub_ps(val0, val2);
            val1 = _mm_sub_ps(val1, val3);
            return SIMD6(_mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 0, 3, 2)), val0);
        } else
#endif
        {
            const T crossX0 = -(this->values4 * other.values2);
            const T crossX1 = -(this->values5 * other.values3);
            const T crossY0 = -(this->values0 * other.values4);
            const T crossY1 = -(this->values1 * other.values5);
            const T crossZ0 = -(this->values2 * other.values0);
            const T crossZ1 = -(this->values3 * other.values1);
            return SIMD6(Shift::fma<T>(this->values2, other.values4, crossX0),
                Shift::fma<T>(this->values3, other.values5, crossX1),
                Shift::fma<T>(this->values4, other.values0, crossY0),
                Shift::fma<T>(this->values5, other.values1, crossY1),
                Shift::fma<T>(this->values0, other.values2, crossZ0),
                Shift::fma<T>(this->values1, other.values3, crossZ1));
        }
    }

    /**
     * Compute the square of the length of each internal SIMD3.
     * @returns Squared lengths.
     */
    XS_FUNCTION SIMD2Def lengthSqr3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 sq = _mm256_mul_ps(this->values, this->values);
            const __m128 val = _mm_add_ps(_mm256_extractf128_ps(sq, 1), _mm256_castps256_ps128(sq));
            return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(_mm256_castps256_ps128(sq)), val));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 sq0 = _mm_mul_ps(this->values0, this->values0);
            const __m128 sq1 = _mm_mul_ps(this->values1, this->values1);
            const __m128 val = _mm_add_ps(sq0, sq1);
            return SIMD2Def(_mm_add_ps(_mm_shuffle3232_ps(sq0), val));
        } else
#endif
        {
            T sq0 = this->values0 * this->values0;
            T sq1 = this->values1 * this->values1;
            sq0 = Shift::fma<T>(this->values2, this->values2, sq0);
            sq1 = Shift::fma<T>(this->values3, this->values3, sq1);
            return SIMD2Def(
                Shift::fma<T>(this->values4, this->values4, sq0), Shift::fma<T>(this->values5, this->values5, sq1));
        }
    }

    /**
     * Compute the length of each internal SIMD3.
     * @returns The length.
     */
    XS_FUNCTION SIMD2Def length3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 sq = _mm256_mul_ps(this->values, this->values);
            const __m128 val = _mm_add_ps(_mm256_extractf128_ps(sq, 1), _mm256_castps256_ps128(sq));
            return SIMD2Def(_mm_sqrt_ps(_mm_add_ps(_mm_shuffle3232_ps(_mm256_castps256_ps128(sq)), val)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 sq0 = _mm_mul_ps(this->values0, this->values0);
            const __m128 sq1 = _mm_mul_ps(this->values1, this->values1);
            const __m128 val = _mm_add_ps(sq0, sq1);
            return SIMD2Def(_mm_sqrt_ps(_mm_add_ps(_mm_shuffle3232_ps(sq0), val)));
        } else
#endif
        {
            T sq0 = this->values0 * this->values0;
            T sq1 = this->values1 * this->values1;
            sq0 = Shift::fma<T>(this->values2, this->values2, sq0);
            sq1 = Shift::fma<T>(this->values3, this->values3, sq1);
            return SIMD2Def(Shift::sqrt<T>(Shift::fma<T>(this->values4, this->values4, sq0)),
                Shift::sqrt<T>(Shift::fma<T>(this->values5, this->values5, sq1)));
        }
    }

    /**
     * Normalise each internal SIMD3.
     * @note The result is unpredictable when all elements are at or near zero.
     * @returns The normalised values.
     */
    XS_FUNCTION SIMD6 normalize3() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 sq = _mm256_mul_ps(this->values, this->values);
            __m128 val = _mm_add_ps(_mm256_extractf128_ps(sq, 1), _mm256_castps256_ps128(sq));
            val = _mm_add_ps(_mm_shuffle3232_ps(_mm256_castps256_ps128(sq)), _mm_shuffle1010_ps(val));
            val = _mm_sqrt_ps(val);
            return SIMD6(_mm256_div_ps(this->values, _mm256_broadcastf128_ps(val)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 sq0 = _mm_mul_ps(this->values0, this->values0);
            const __m128 sq1 = _mm_mul_ps(this->values1, this->values1);
            __m128 val = _mm_add_ps(sq0, sq1);
            val = _mm_add_ps(_mm_shuffle3232_ps(sq0), _mm_shuffle1010_ps(val));
            val = _mm_sqrt_ps(val);
            return SIMD6(_mm_div_ps(this->values0, val), _mm_div_ps(this->values1, val));
        } else
#endif
        {
            T sq0 = this->values0 * this->values0;
            T sq1 = this->values1 * this->values1;
            sq0 = Shift::fma<T>(this->values2, this->values2, sq0);
            sq1 = Shift::fma<T>(this->values3, this->values3, sq1);
            sq0 = Shift::sqrt<T>(Shift::fma<T>(this->values4, this->values4, sq0));
            sq1 = Shift::sqrt<T>(Shift::fma<T>(this->values5, this->values5, sq1));
            return SIMD6(this->values0 / sq0, this->values1 / sq1, this->values2 / sq0, this->values3 / sq1,
                this->values4 / sq0, this->values5 / sq1);
        }
    }

    /**
     * Returns binary exponential of each element in the object.
     * @returns Object containing the binary exponential of the input.
     */
    XS_FUNCTION SIMD6 exp2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(NoExport::exp2f8(this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
            const __m256 res = NoExport::exp2f8(_mm256_set_m128(this->values1, this->values0));
            return SIMD6(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(NoExport::exp2f4(this->values0), NoExport::exp2f4(this->values1));
        } else
#endif
        {
            return SIMD6(Shift::exp2<T>(this->values0), Shift::exp2<T>(this->values1), Shift::exp2<T>(this->values2),
                Shift::exp2<T>(this->values3), Shift::exp2<T>(this->values4), Shift::exp2<T>(this->values5));
        }
    }

    /**
     * Returns exponential of each element in the object.
     * @returns Object containing the exponential of the input.
     */
    XS_FUNCTION SIMD6 exp() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(NoExport::expf8(this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
            const __m256 res = NoExport::expf8(_mm256_set_m128(this->values1, this->values0));
            return SIMD6(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(NoExport::expf4(this->values0), NoExport::expf4(this->values1));
        } else
#endif
        {
            return SIMD6(Shift::exp<T>(this->values0), Shift::exp<T>(this->values1), Shift::exp<T>(this->values2),
                Shift::exp<T>(this->values3), Shift::exp<T>(this->values4), Shift::exp<T>(this->values5));
        }
    }

    /**
     * Returns binary logarithm of each element in the object.
     * @returns Object containing the binary logarithm of the input.
     */
    XS_FUNCTION SIMD6 log2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(NoExport::log2f8(this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
            const __m256 res = NoExport::log2f8(_mm256_set_m128(this->values1, this->values0));
            return SIMD6(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(NoExport::log2f4(this->values0), NoExport::log2f4(this->values1));
        } else
#endif
        {
            return SIMD6(Shift::log2<T>(this->values0), Shift::log2<T>(this->values1), Shift::log2<T>(this->values2),
                Shift::log2<T>(this->values3), Shift::log2<T>(this->values4), Shift::log2<T>(this->values5));
        }
    }

    /**
     * Returns natural logarithm of each element in the object.
     * @returns Object containing the logarithm of the input.
     */
    XS_FUNCTION SIMD6 log() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(NoExport::logf8(this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
            const __m256 res = NoExport::logf8(_mm256_set_m128(this->values1, this->values0));
            return SIMD6(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(NoExport::logf4(this->values0), NoExport::logf4(this->values1));
        } else
#endif
        {
            return SIMD6(Shift::log<T>(this->values0), Shift::log<T>(this->values1), Shift::log<T>(this->values2),
                Shift::log<T>(this->values3), Shift::log<T>(this->values4), Shift::log<T>(this->values5));
        }
    }

    /**
     * Returns object to the power of the elements in another object.
     * @param other The object containing the exponents.
     * @returns Object containing the input values to the power of the second inputs.
     */
    XS_FUNCTION SIMD6 pow(const SIMD6& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(NoExport::powf8(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
            const __m256 res = NoExport::powf8(
                _mm256_set_m128(this->values1, this->values0), _mm256_set_m128(other.values1, other.values0));
            return SIMD6(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(NoExport::powf4(this->values0, other.values0), NoExport::powf4(this->values1, other.values1));
        } else
#endif
        {
            return SIMD6(Shift::pow<T>(this->values0, other.values0), Shift::pow<T>(this->values1, other.values1),
                Shift::pow<T>(this->values2, other.values2), Shift::pow<T>(this->values3, other.values3),
                Shift::pow<T>(this->values4, other.values4), Shift::pow<T>(this->values5, other.values5));
        }
    }

    /**
     * Returns object (where all elements must be > 0) to the power of the elements in another object.
     * @note The current object must have all elements > 0.
     * @param other The object containing the exponents.
     * @returns Object containing the input values to the power of the second inputs.
     */
    XS_FUNCTION SIMD6 powr(const SIMD6& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(NoExport::powrf8(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
            const __m256 res = NoExport::powrf8(
                _mm256_set_m128(this->values1, this->values0), _mm256_set_m128(other.values1, other.values0));
            return SIMD6(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(
                NoExport::powrf4(this->values0, other.values0), NoExport::powrf4(this->values1, other.values1));
        } else
#endif
        {
            return SIMD6(Shift::powr<T>(this->values0, other.values0), Shift::powr<T>(this->values1, other.values1),
                Shift::powr<T>(this->values2, other.values2), Shift::powr<T>(this->values3, other.values3),
                Shift::powr<T>(this->values4, other.values4), Shift::powr<T>(this->values5, other.values5));
        }
    }

    /**
     * Returns object to the power of another value.
     * @param other The value containing the exponent.
     * @returns Object containing the input values to the power of the second input.
     */
    XS_FUNCTION SIMD6 pow(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(NoExport::powf8(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
            const __m256 res =
                NoExport::powf8(_mm256_set_m128(this->values1, this->values0), _mm256_broadcastf128_ps(other.values));
            return SIMD6(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(NoExport::powf4(this->values0, other.values), NoExport::powf4(this->values1, other.values));
        } else
#endif
        {
            return SIMD6(Shift::pow<T>(this->values0, other.value), Shift::pow<T>(this->values1, other.value),
                Shift::pow<T>(this->values2, other.value), Shift::pow<T>(this->values3, other.value),
                Shift::pow<T>(this->values4, other.value), Shift::pow<T>(this->values5, other.value));
        }
    }

    /**
     * Returns object (where all elements must be > 0) to the power of another value.
     * @note The current object must have all elements > 0.
     * @param other The value containing the exponent.
     * @returns Object containing the input values to the power of the second input.
     */
    XS_FUNCTION SIMD6 powr(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(NoExport::powrf8(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
            const __m256 res =
                NoExport::powrf8(_mm256_set_m128(this->values1, this->values0), _mm256_broadcastf128_ps(other.values));
            return SIMD6(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(NoExport::powrf4(this->values0, other.values), NoExport::powrf4(this->values1, other.values));
        } else
#endif
        {
            return SIMD6(Shift::powr<T>(this->values0, other.value), Shift::powr<T>(this->values1, other.value),
                Shift::powr<T>(this->values2, other.value), Shift::powr<T>(this->values3, other.value),
                Shift::powr<T>(this->values4, other.value), Shift::powr<T>(this->values5, other.value));
        }
    }

    /**
     * Returns sine of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the sined values.
     */
    XS_FUNCTION SIMD6 sin() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(NoExport::sinf8(this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
            const __m256 res = NoExport::sinf8(_mm256_set_m128(this->values1, this->values0));
            return SIMD6(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(NoExport::sinf4(this->values0), NoExport::sinf4(this->values1));
        } else
#endif
        {
            return SIMD6(Shift::sin<T>(this->values0), Shift::sin<T>(this->values1), Shift::sin<T>(this->values2),
                Shift::sin<T>(this->values3), Shift::sin<T>(this->values4), Shift::sin<T>(this->values5));
        }
    }

    /**
     * Returns cosine of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the cosined values.
     */
    XS_FUNCTION SIMD6 cos() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(NoExport::cosf8(this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
            const __m256 res = NoExport::cosf8(_mm256_set_m128(this->values1, this->values0));
            return SIMD6(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(NoExport::cosf4(this->values0), NoExport::cosf4(this->values1));
        } else
#endif
        {
            return SIMD6(Shift::cos<T>(this->values0), Shift::cos<T>(this->values1), Shift::cos<T>(this->values2),
                Shift::cos<T>(this->values3), Shift::cos<T>(this->values4), Shift::cos<T>(this->values5));
        }
    }

    /**
     * Returns tangent of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the tangented values.
     */
    XS_FUNCTION SIMD6 tan() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(NoExport::tanf8(this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
            const __m256 res = NoExport::tanf8(_mm256_set_m128(this->values1, this->values0));
            return SIMD6(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(NoExport::tanf4(this->values0), NoExport::tanf4(this->values1));
        } else
#endif
        {
            return SIMD6(Shift::tan<T>(this->values0), Shift::tan<T>(this->values1), Shift::tan<T>(this->values2),
                Shift::tan<T>(this->values3), Shift::tan<T>(this->values4), Shift::tan<T>(this->values5));
        }
    }

    /**
     * Returns sine and cosine of each element in the object.
     * @note This is provided because it is much quicker to determine the cosine of a set of values if the sine is
     * already known.
     * @param [out] cosReturn The cosine return values.
     * @returns Object containing the sined values.
     */
    XS_FUNCTION SIMD6 sincos(SIMD6& cosReturn) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(NoExport::sincosf8(this->values, cosReturn.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
            __m256 tempReturn;
            const __m256 res = NoExport::sincosf8(_mm256_set_m128(this->values1, this->values0), tempReturn);
            cosReturn.values0 = _mm256_castps256_ps128(tempReturn);
            cosReturn.values1 = _mm256_extractf128_ps(tempReturn, 1);
            return SIMD6(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(NoExport::sincosf4(this->values0, cosReturn.values0),
                NoExport::sincosf4(this->values1, cosReturn.values1));
        } else
#endif
        {
            return SIMD6(Shift::sincos<T>(this->values0, cosReturn.values0),
                Shift::sincos<T>(this->values1, cosReturn.values1), Shift::sincos<T>(this->values2, cosReturn.values2),
                Shift::sincos<T>(this->values3, cosReturn.values3), Shift::sincos<T>(this->values4, cosReturn.values4),
                Shift::sincos<T>(this->values5, cosReturn.values5));
        }
    }

    /**
     * Returns arcsine of each element in the object.
     * @returns Object containing the arcsined values (results in radians).
     */
    XS_FUNCTION SIMD6 asin() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(NoExport::asinf8(this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
            const __m256 res = NoExport::asinf8(_mm256_set_m128(this->values1, this->values0));
            return SIMD6(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(NoExport::asinf4(this->values0), NoExport::asinf4(this->values1));
        } else
#endif
        {
            return SIMD6(Shift::asin<T>(this->values0), Shift::asin<T>(this->values1), Shift::asin<T>(this->values2),
                Shift::asin<T>(this->values3), Shift::asin<T>(this->values4), Shift::asin<T>(this->values5));
        }
    }

    /**
     * Returns arccosine of each element in the object.
     * @returns Object containing the arccosined values (results in radians).
     */
    XS_FUNCTION SIMD6 acos() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(NoExport::acosf8(this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
            const __m256 res = NoExport::acosf8(_mm256_set_m128(this->values1, this->values0));
            return SIMD6(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(NoExport::acosf4(this->values0), NoExport::acosf4(this->values1));
        } else
#endif
        {
            return SIMD6(Shift::acos<T>(this->values0), Shift::acos<T>(this->values1), Shift::acos<T>(this->values2),
                Shift::acos<T>(this->values3), Shift::acos<T>(this->values4), Shift::acos<T>(this->values5));
        }
    }

    /**
     * Returns arctangent of each element in the object.
     * @returns Object containing the arctangented values (results in radians).
     */
    XS_FUNCTION SIMD6 atan() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(NoExport::atanf8(this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
            const __m256 res = NoExport::atanf8(_mm256_set_m128(this->values1, this->values0));
            return SIMD6(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(NoExport::atanf4(this->values0), NoExport::atanf4(this->values1));
        } else
#endif
        {
            return SIMD6(Shift::atan<T>(this->values0), Shift::atan<T>(this->values1), Shift::atan<T>(this->values2),
                Shift::atan<T>(this->values3), Shift::atan<T>(this->values4), Shift::atan<T>(this->values5));
        }
    }

    /**
     * Returns the distance between a planes positive x-axis and the points given by the input objects.
     * @param other The object containing the second parameter of each of the three input points.
     * @returns Object containing the angles (results in radians).
     */
    XS_FUNCTION SIMD6 atan2(const SIMD6& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(NoExport::atan2f8(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16)) {
            const __m256 res = NoExport::atan2f8(
                _mm256_set_m128(this->values1, this->values0), _mm256_set_m128(other.values1, other.values0));
            return SIMD6(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD6(
                NoExport::atan2f4(this->values0, other.values0), NoExport::atan2f4(this->values1, other.values1));
        } else
#endif
        {
            return SIMD6(Shift::atan2<T>(this->values0, other.values0), Shift::atan2<T>(this->values1, other.values1),
                Shift::atan2<T>(this->values2, other.values2), Shift::atan2<T>(this->values3, other.values3),
                Shift::atan2<T>(this->values4, other.values4), Shift::atan2<T>(this->values5, other.values5));
        }
    }

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index.
     * @tparam Index0 The index in the first object to insert the element (range is 0-5).
     * @tparam Index1 The index in the second object to copy element from (range is 0-5).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD6 insert(const SIMD6& other) const noexcept
    {
        static_assert(Index0 < 6 && Index1 < 6, "Invalid Index: Indexes must be <6");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == Index1) {
                return SIMD6(_mm256_blend_ps(this->values, other.values, 1UL << Index0));
            } else if constexpr ((Index0 / 4) == (Index1 / 4)) {
                return SIMD6(_mm256_blend_ps(this->values,
                    _mm256_shuffle1_ps(other.values, _MM_SHUFFLE(Index1 % 4, Index1 % 4, Index1 % 4, Index1 % 4)),
                    _MM256_BLEND(Index0 == 7, Index0 == 6, Index0 == 5, Index0 == 4, Index0 == 3, Index0 == 2,
                        Index0 == 1, Index0 == 0)));
            } else if constexpr ((Index0 % 4) == (Index1 % 4)) {
                const __m256 val = _mm256_shuffle32107654_ps(other.values);
                return SIMD6(_mm256_blend_ps(this->values, val,
                    _MM256_BLEND(Index0 == 7, Index0 == 6, Index0 == 5, Index0 == 4, Index0 == 3, Index0 == 2,
                        Index0 == 1, Index0 == 0)));
            } else {
                const __m256 val = _mm256_shuffle32107654_ps(other.values);
                return SIMD6(_mm256_blend_ps(this->values,
                    _mm256_shuffle1_ps(val, _MM_SHUFFLE(Index1 % 4, Index1 % 4, Index1 % 4, Index1 % 4)),
                    _MM256_BLEND(Index0 == 7, Index0 == 6, Index0 == 5, Index0 == 4, Index0 == 3, Index0 == 2,
                        Index0 == 1, Index0 == 0)));
            }
        } else if constexpr ((isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) ||
            (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32))) {
            __m128 half0 = (&this->values0)[Index0 / 4];
            const __m128 half1 = (&other.values0)[Index1 / 4];
            constexpr uint32 index0 = Index0 % 4;
            constexpr uint32 index1 = Index1 % 4;
            if constexpr (index0 == 0 && index1 == 0) {
                half0 = _mm_move_ss(half0, half1);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41> && index0 == index1) {
                half0 = _mm_blend_ps(half0, half1, 1UL << index0);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                half0 = _mm_insert_ps(half0, half1, _MM_MK_INSERTPS_NDX(index1, index0, 0));
            } else if constexpr (index0 == 0 && index1 == 1) {
                half0 = _mm_move_ss(half0, _mm_shuffle3311_ps(half1)); //(x,x,x,1)
            } else if constexpr (index0 == 0 && index1 == 2) {
                half0 = _mm_move_ss(half0, _mm_shuffle3232_ps(half1));
            } else if constexpr (index0 == 0 && index1 == 3) {
                half0 = _mm_move_ss(half0, _mm_shuffle3333_ps(half1)); /*(x,x,x,3)*/
            } else if constexpr (index0 == 1 && index1 == 0) {
                const __m128 val = _mm_movelh_ps(half0, half1); /*(x,0,x,0)*/
                half0 = _mm_shuffle_ps(val, half0, _MM_SHUFFLE(3, 2, 2, 0));
            } else if constexpr (index0 == 1 && index1 == 1) {
                const __m128 val = _mm_movelh_ps(half0, half1); //(b1,x,x,a0)
                half0 = _mm_shuffle_ps(val, half0, _MM_SHUFFLE(3, 2, 3, 0));
            } else if constexpr (index0 == 2 && index1 == 2) {
                const __m128 val = _mm_movehl_ps(half0, half1); /*(3,x,x,2)*/
                half0 = _mm_shuffle_ps(half0, val, _MM_SHUFFLE(3, 0, 1, 0));
            } else if constexpr (index0 == 2 && index1 == 3) {
                const __m128 val = _mm_movehl_ps(half0, half1); /*(3,x,3,x)*/
                half0 = _mm_shuffle_ps(half0, val, _MM_SHUFFLE(3, 1, 1, 0));
            } else if constexpr (index0 == 3 && index1 == 2) {
                const __m128 val = _mm_movehl_ps(half0, half1); /*(x,2,x,2)*/
                half0 = _mm_shuffle_ps(half0, val, _MM_SHUFFLE(0, 2, 1, 0));
            } else if constexpr (index0 == 3 && index1 == 3) {
                const __m128 val = _mm_movehl_ps(half0, half1); /*(x,2,3,x)*/
                half0 = _mm_shuffle_ps(half0, val, _MM_SHUFFLE(1, 2, 1, 0));
            } else if constexpr (index0 == 0) {
                const __m128 val = _mm_shuffle_ps(half0, half1, _MM_SHUFFLE(index1, index1, 1, 1)); /*(x,index1,x,1)*/
                half0 = _mm_shuffle_ps(val, half0, _MM_SHUFFLE(3, 2, 0, 2));
            } else if constexpr (index0 == 1) {
                const __m128 val = _mm_shuffle_ps(half0, half1, _MM_SHUFFLE(index1, index1, 0, 0)); /*(x,index1,x,0)*/
                half0 = _mm_shuffle_ps(val, half0, _MM_SHUFFLE(3, 2, 2, 0));
            } else if constexpr (index0 == 2) {
                const __m128 val = _mm_shuffle_ps(half0, half1, _MM_SHUFFLE(index1, index1, 3, 3)); /*(x,index1,x,3)*/
                half0 = _mm_shuffle_ps(half0, val, _MM_SHUFFLE(0, 2, 1, 0));
            } else /*index0 == 3*/ {
                const __m128 val = _mm_shuffle_ps(half0, half1, _MM_SHUFFLE(index1, index1, 2, 2)); /*(x,index1,x,2)*/
                half0 = _mm_shuffle_ps(half0, val, _MM_SHUFFLE(2, 0, 1, 0));
            }

            if constexpr (Index0 / 4 == 0) {
                return SIMD6(half0, this->values1);
            } else {
                return SIMD6(this->values0, half0);
            }
        } else
#endif
        {
            SIMD6 ret(*this);
            (&ret.values0)[Index0] = (&other.values0)[Index1];
            return ret;
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
    XS_FUNCTION SIMD6 insert2(const SIMD6& other) const noexcept
    {
        static_assert(Index0 < 2 && Index1 < 2, "Invalid Index: Indexes must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD6(_mm256_blend_ps(this->values, other.values, _MM256_BLEND(0, 0, 0, 1, 0, 1, 0, 1)));
            } else if constexpr (Index0 == 0 && Index1 == 1) {
                const __m256 ret = _mm256_shuffle_ps(this->values, other.values, _MM_SHUFFLE(3, 1, 3, 1));
                return SIMD6(_mm256_shuffle1_ps(ret, _MM_SHUFFLE(1, 3, 0, 2)));
            } else if constexpr (Index0 == 1 && Index1 == 0) {
                const __m256 ret = _mm256_shuffle_ps(this->values, other.values, _MM_SHUFFLE(2, 0, 2, 0));
                return SIMD6(_mm256_shuffle1_ps(ret, _MM_SHUFFLE(3, 1, 2, 0)));
            } else /*Index0 == 1 && Index1 == 1*/ {
                return SIMD6(_mm256_blend_ps(this->values, other.values, _MM256_BLEND(0, 0, 1, 0, 1, 0, 1, 0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                if constexpr (Index0 == 0 && Index1 == 0) {
                    return SIMD6(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(0, 1, 0, 1)),
                        _mm_blend_ps(this->values1, other.values1, _MM_BLEND(0, 0, 0, 1)));
                } else if constexpr (Index0 == 1 && Index1 == 1) {
                    return SIMD6(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(1, 0, 1, 0)),
                        _mm_blend_ps(this->values1, other.values1, _MM_BLEND(0, 0, 1, 0)));
                } else {
                    const __m128 val0 =
                        _mm_insert_ps(this->values0, other.values0, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                    const __m128 val1 =
                        _mm_insert_ps(this->values1, other.values1, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                    return SIMD6(
                        _mm_insert_ps(val0, other.values0, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)), val1);
                }
            } else {
                const __m128 val00 = _mm_movelh_ps(other.values0, this->values0);
                const __m128 val01 = _mm_movehl_ps(this->values0, other.values0);
                if constexpr (Index0 == 1 && Index1 == 0) {
                    return SIMD6(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(0, 2, 0, 2)),
                        _mm_unpacklo_ps(this->values1, other.values1));
                } else {
                    const __m128 val10 = _mm_movelh_ps(other.values1, this->values1);
                    if constexpr (Index0 == 0 && Index1 == 0) {
                        return SIMD6(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(3, 0, 3, 0)),
                            _mm_shuffle_ps(val10, val10, _MM_SHUFFLE(3, 0, 3, 0)));
                    } else if constexpr (Index0 == 0 && Index1 == 1) {
                        return SIMD6(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(3, 1, 3, 1)),
                            _mm_shuffle_ps(val10, val10, _MM_SHUFFLE(3, 1, 3, 1)));
                    } else /*Index0 == 1 && Index1 == 1*/ {
                        return SIMD6(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(1, 2, 1, 2)),
                            _mm_shuffle_ps(val10, val10, _MM_SHUFFLE(1, 2, 1, 2)));
                    }
                }
            }
        } else
#endif
        {
            SIMD6 ret(*this);
            (&ret.values0)[Index0] = (&other.values0)[Index1];
            (&ret.values0)[Index0 + 2] = (&other.values0)[Index1 + 2];
            (&ret.values0)[Index0 + 4] = (&other.values0)[Index1 + 4];
            return ret;
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
    XS_FUNCTION SIMD6 insert3(const SIMD6& other) const noexcept
    {
        static_assert(Index0 < 3 && Index1 < 3, "Invalid Index: Indexes must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD6(_mm256_blend_ps(this->values, other.values, _MM256_BLEND(0, 0, 0, 0, 0, 0, 1, 1)));
            } else if constexpr (Index0 == 0 && Index1 == 1) {
                return SIMD6(_mm256_blend_ps(this->values, _mm256_shuffle1_ps(other.values, _MM_SHUFFLE(3, 2, 3, 2)),
                    _MM256_BLEND(0, 0, 0, 0, 0, 0, 1, 1))); //(x,x,3,2)
            } else if constexpr (Index0 == 0 && Index1 == 2) {
                return SIMD6(_mm256_blend_ps(
                    this->values, _mm256_shuffle32107654_ps(other.values), _MM256_BLEND(0, 0, 0, 0, 0, 0, 1, 1)));
            } else if constexpr (Index0 == 1 && Index1 == 0) {
                return SIMD6(_mm256_blend_ps(this->values, _mm256_shuffle1_ps(other.values, _MM_SHUFFLE(1, 0, 3, 2)),
                    _MM256_BLEND(0, 0, 0, 0, 1, 1, 0, 0))); //(1,0,x,x)
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD6(_mm256_blend_ps(this->values, other.values, _MM256_BLEND(0, 0, 0, 0, 1, 1, 0, 0)));
            } else if constexpr (Index0 == 1 && Index1 == 2) {
                const __m256 val = _mm256_shuffle32107654_ps(other.values);
                return SIMD6(_mm256_blend_ps(this->values, _mm256_shuffle1_ps(val, _MM_SHUFFLE(1, 0, 3, 2)),
                    _MM256_BLEND(0, 0, 0, 0, 1, 1, 0, 0))); //(1,0,x,x)
            } else if constexpr (Index0 == 2 && Index1 == 0) {
                return SIMD6(_mm256_blend_ps(
                    this->values, _mm256_shuffle32107654_ps(other.values), _MM256_BLEND(0, 0, 1, 1, 0, 0, 0, 0)));
            } else if constexpr (Index0 == 2 && Index1 == 1) {
                const __m256 val = _mm256_shuffle32107654_ps(other.values);
                return SIMD6(_mm256_blend_ps(this->values, _mm256_shuffle1_ps(val, _MM_SHUFFLE(3, 2, 3, 2)),
                    _MM256_BLEND(1, 1, 1, 1, 0, 0, 0, 0))); //(3,2,3,2)
            } else /*Index0 == 2 && Index1 == 2*/ {
                return SIMD6(_mm256_blend_ps(this->values, other.values, _MM256_BLEND(1, 1, 1, 1, 0, 0, 0, 0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD6(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(0, 0, 1, 1)), this->values1);
            } else if constexpr (Index0 == 0 && Index1 == 1) {
                return SIMD6(_mm_movehl_ps(this->values0, other.values0), this->values1);
            } else if constexpr (Index0 == 0 && Index1 == 2) {
                if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                    return SIMD6(_mm_blend_ps(this->values0, other.values1, _MM_BLEND(0, 0, 1, 1)), this->values1);
                } else {
                    return SIMD6(_mm_shuffle_ps(other.values1, this->values0, _MM_SHUFFLE(3, 2, 1, 0)), this->values1);
                }
            } else if constexpr (Index0 == 1 && Index1 == 0) {
                return SIMD6(_mm_movelh_ps(this->values0, other.values0), this->values1);
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                    return SIMD6(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(1, 1, 0, 0)), this->values1);
                } else {
                    return SIMD6(_mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(3, 2, 1, 0)), this->values1);
                }
            } else if constexpr (Index0 == 1 && Index1 == 2) {
                return SIMD6(_mm_movelh_ps(this->values0, other.values1), this->values1);
            } else if constexpr (Index0 == 2 && Index1 == 0) {
                return SIMD6(this->values0, other.values0);
            } else if constexpr (Index0 == 2 && Index1 == 1) {
                return SIMD6(this->values0, _mm_movehl_ps(other.values0, other.values0));
            } else /*Index0 == 2 && Index1 == 2*/ {
                return SIMD6(this->values0, other.values1);
            }
        } else
#endif
        {
            SIMD6 ret(*this);
            (&ret.values0)[Index0 * 2] = (&other.values0)[Index1 * 2];
            (&ret.values0)[Index0 * 2 + 1] = (&other.values0)[Index1 * 2 + 1];
            return ret;
        }
    }

    /**
     * Blends values from the second object into the first based on a mask.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @tparam Elem2 Whether to copy the third element of the first input into the returned object.
     * @tparam Elem3 Whether to copy the fourth element of the first input into the returned object.
     * @tparam Elem4 Whether to copy the fifth element of the first input into the returned object.
     * @tparam Elem5 Whether to copy the sixth element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns Result of operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5>
    XS_FUNCTION SIMD6 blend(const SIMD6& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5) {
            return *this;
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5) {
            return other;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(_mm256_blend_ps(
                this->values, other.values, _MM256_BLEND(0, 0, Elem5, Elem4, Elem3, Elem2, Elem1, Elem0)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 lo;
            if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
                lo = other.values0;
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
                lo = this->values0;
            } else if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
                lo = _mm_move_ss(this->values0, other.values0);
            } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
                lo = _mm_move_ss(other.values0, this->values0);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                lo = _mm_blend_ps(this->values0, other.values0, _MM_BLEND(Elem3, Elem2, Elem1, Elem0));
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && Elem3) {
                lo = _mm_movehl_ps(this->values0, other.values0); //(x,a2,b3,x)
                lo = _mm_shuffle_ps(this->values0, lo, _MM_SHUFFLE(1, 2, 1, 0));
            } else if constexpr (!Elem0 && !Elem1 && Elem2 && !Elem3) {
                lo = _mm_movehl_ps(this->values0, other.values0); //(a3,x,x,b2)
                lo = _mm_shuffle_ps(this->values0, lo, _MM_SHUFFLE(3, 0, 1, 0));
            } else if constexpr (!Elem0 && !Elem1 && Elem2 && Elem3) {
                lo = _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(3, 2, 1, 0));
            } else if constexpr (!Elem0 && Elem1 && !Elem2 && !Elem3) {
                lo = _mm_movelh_ps(this->values0, other.values0); //(b1,x,x,a0)
                lo = _mm_shuffle_ps(lo, this->values0, _MM_SHUFFLE(3, 2, 3, 0));
            } else if constexpr (!Elem0 && Elem1 && !Elem2 && Elem3) {
                lo = _mm_movelh_ps(other.values0, this->values0);                //(x,a0,b1,x)
                const __m128 val1 = _mm_movehl_ps(this->values0, other.values0); //(x,a2,b3,x)
                lo = _mm_shuffle_ps(lo, val1, _MM_SHUFFLE(1, 2, 1, 2));
            } else if constexpr (!Elem0 && Elem1 && Elem2 && !Elem3) {
                lo = _mm_movelh_ps(other.values0, this->values0);                //(x,a0,b1,x)
                const __m128 val1 = _mm_movehl_ps(this->values0, other.values0); //(a3,x,x,b2)
                lo = _mm_shuffle_ps(lo, val1, _MM_SHUFFLE(3, 0, 1, 2));
            } else if constexpr (!Elem0 && Elem1 && Elem2 && !Elem3) {
                lo = _mm_movelh_ps(other.values0, this->values0); //(x,a0,b1,x)
                lo = _mm_shuffle_ps(lo, other.values0, _MM_SHUFFLE(3, 2, 1, 2));
            } else if constexpr (Elem0 && !Elem1 && !Elem2 && Elem3) {
                lo = _mm_movelh_ps(other.values0, this->values0);                //(a1,x,x,b0)
                const __m128 val1 = _mm_movehl_ps(this->values0, other.values0); //(x,a2,b3,x)
                lo = _mm_shuffle_ps(lo, val1, _MM_SHUFFLE(1, 2, 3, 0));
            } else if constexpr (Elem0 && !Elem1 && Elem2 && !Elem3) {
                lo = _mm_movelh_ps(other.values0, this->values0);                //(a1,x,x,b0)
                const __m128 val1 = _mm_movehl_ps(this->values0, other.values0); //(a3,x,x,b2)
                lo = _mm_shuffle_ps(lo, val1, _MM_SHUFFLE(3, 0, 3, 0));
            } else if constexpr (Elem0 && !Elem1 && Elem2 && Elem3) {
                lo = _mm_movelh_ps(other.values0, this->values0); //(a1,x,x,b0)
                lo = _mm_shuffle_ps(lo, other.values0, _MM_SHUFFLE(3, 2, 3, 0));
            } else if constexpr (Elem0 && Elem1 && !Elem2 && !Elem3) {
                lo = _mm_shuffle_ps(other.values0, this->values0, _MM_SHUFFLE(3, 2, 1, 0));
            } else if constexpr (Elem0 && Elem1 && !Elem2 && Elem3) {
                lo = _mm_movehl_ps(this->values0, other.values0); //(x,a2,b3,x)
                lo = _mm_shuffle_ps(other.values0, lo, _MM_SHUFFLE(1, 2, 1, 0));
            } else /*Elem0 && Elem1 && Elem2 && !Elem3*/ {
                lo = _mm_movehl_ps(this->values0, other.values0); //(a3,x,x,b2)
                lo = _mm_shuffle_ps(other.values0, lo, _MM_SHUFFLE(3, 0, 1, 0));
            }

            __m128 hi;
            if constexpr (Elem4 && Elem5) {
                hi = other.values1;
            } else if constexpr (!Elem4 && !Elem5) {
                hi = this->values1;
            } else if constexpr (Elem4 && !Elem5) {
                hi = _mm_move_ss(this->values1, other.values1);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                hi = _mm_blend_ps(this->values1, other.values1, _MM_BLEND(0, 0, Elem5, Elem4));
            } else if constexpr (!Elem4 && Elem5) {
                hi = _mm_movelh_ps(this->values1, other.values1); //(b1,x,x,a0)
                hi = _mm_shuffle_ps(hi, this->values1, _MM_SHUFFLE(3, 2, 3, 0));
            } else /*Elem4 && !Elem5*/ {
                hi = _mm_movelh_ps(other.values1, this->values1); //(a1,x,x,b0)
                hi = _mm_shuffle_ps(hi, other.values1, _MM_SHUFFLE(3, 2, 3, 0));
            }

            return SIMD6(lo, hi);
        }
#endif
        else {
            return SIMD6(Elem0 ? other.values0 : this->values0, Elem1 ? other.values1 : this->values1,
                Elem2 ? other.values2 : this->values2, Elem3 ? other.values3 : this->values3,
                Elem4 ? other.values4 : this->values4, Elem5 ? other.values5 : this->values5);
        }
    }

    /**
     * Blends values from the second object into the first based on a dynamic mask.
     * @note A value form the second object is copied into the first based on the corresponding input elements of the
     * mask.
     * @param other The object to blend into the first.
     * @param mask The object mask used to define which elements to blend.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD6 blendVar(const SIMD6& other, const Mask& mask) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return SIMD6(_mm256_mask_blend_ps(mask.values, this->values, other.values));
            } else {
                return SIMD6(_mm256_blendv_ps(this->values, other.values, mask.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return SIMD6(_mm_mask_blend_ps(mask.values0, this->values0, other.values0),
                    _mm_mask_blend_ps(mask.values1, this->values1, other.values1));
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                return SIMD6(_mm_blendv_ps(this->values0, other.values0, mask.values0),
                    _mm_blendv_ps(this->values1, other.values1, mask.values1));
            } else {
                const __m128 val00 = _mm_and_ps(other.values0, mask.values0);
                const __m128 val01 = _mm_andnot_ps(mask.values0, this->values0);
                const __m128 val10 = _mm_and_ps(other.values1, mask.values1);
                const __m128 val11 = _mm_andnot_ps(mask.values1, this->values1);
                return SIMD6(_mm_or_ps(val00, val01), _mm_or_ps(val10, val11));
            }
        } else
#endif
        {
            return SIMD6((mask.values0) ? other.values0 : this->values0, (mask.values1) ? other.values1 : this->values1,
                (mask.values2) ? other.values2 : this->values2, (mask.values3) ? other.values3 : this->values3,
                (mask.values4) ? other.values4 : this->values4, (mask.values5) ? other.values5 : this->values5);
        }
    }

    /**
     * Swaps values between the second object and the first based on a mask.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to swap the first elements.
     * @tparam Elem1 Whether to swap the second elements.
     * @tparam Elem2 Whether to swap the third elements.
     * @tparam Elem3 Whether to swap the fourth elements.
     * @tparam Elem4 Whether to swap the fifth elements.
     * @tparam Elem5 Whether to swap the sixth elements.
     * @param [in,out] other The object to swap elements with the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3, bool Elem4, bool Elem5>
    XS_FUNCTION SIMD6 blendSwap(SIMD6& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3 && !Elem4 && !Elem5) {
            return *this;
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3 && Elem4 && Elem5) {
            const SIMD6 backup = other;
            other = *this;
            return backup;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 backup = other.values;
            other.values = _mm256_blend_ps(
                other.values, this->values, _MM256_BLEND(0, 0, Elem5, Elem4, Elem3, Elem2, Elem1, Elem0));
            return SIMD6(
                _mm256_blend_ps(this->values, backup, _MM256_BLEND(0, 0, Elem5, Elem4, Elem3, Elem2, Elem1, Elem0)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 lo;
            if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
                lo = other.values0;
                other.values0 = this->values0;
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
                lo = this->values0;
            } else if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
                const __m128 backup = other.values0;
                other.values0 = _mm_move_ss(other.values0, this->values0);
                lo = _mm_move_ss(this->values0, backup);
            } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
                const __m128 backup = other.values0;
                other.values0 = _mm_move_ss(this->values0, other.values0);
                lo = _mm_move_ss(backup, this->values0);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                const __m128 backup = other.values0;
                other.values0 = _mm_blend_ps(other.values0, this->values0, _MM_BLEND(Elem3, Elem2, Elem1, Elem0));
                lo = _mm_blend_ps(this->values0, backup, _MM_BLEND(Elem3, Elem2, Elem1, Elem0));
            } else if constexpr (!Elem0 && !Elem1 && !Elem2 && Elem3) {
                const __m128 val = _mm_movehl_ps(this->values0, other.values0); //(a3,a2,b3,b2)
                other.values0 = _mm_shuffle_ps(other.values0, val, _MM_SHUFFLE(3, 0, 1, 0));
                lo = _mm_shuffle_ps(this->values0, val, _MM_SHUFFLE(1, 2, 1, 0));
            } else if constexpr (!Elem0 && !Elem1 && Elem2 && !Elem3) {
                const __m128 val = _mm_movehl_ps(this->values0, other.values0); //(a3,a2,b3,b2)
                other.values0 = _mm_shuffle_ps(other.values0, val, _MM_SHUFFLE(1, 2, 1, 0));
                lo = _mm_shuffle_ps(this->values0, val, _MM_SHUFFLE(3, 0, 1, 0));
            } else if constexpr (!Elem0 && !Elem1 && Elem2 && Elem3) {
                const __m128 backup = other.values0;
                other.values0 = _mm_shuffle_ps(other.values0, this->values0, _MM_SHUFFLE(3, 2, 1, 0));
                lo = _mm_shuffle_ps(this->values0, backup, _MM_SHUFFLE(3, 2, 1, 0));
            } else if constexpr (!Elem0 && Elem1 && !Elem2 && !Elem3) {
                const __m128 val = _mm_movelh_ps(this->values0, other.values0); //(b1,b0,a1,a0)
                other.values0 = _mm_shuffle_ps(val, other.values0, _MM_SHUFFLE(3, 2, 1, 2));
                lo = _mm_shuffle_ps(val, this->values0, _MM_SHUFFLE(3, 2, 3, 0));
            } else if constexpr (!Elem0 && Elem1 && !Elem2 && Elem3) {
                const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(b1,b0,a1,a0)
                const __m128 val1 = _mm_movehl_ps(this->values0, other.values0); //(a3,a2,b3,b2)
                other.values0 = _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 0, 1, 2));
                lo = _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 2, 3, 0));
            } else if constexpr (!Elem0 && Elem1 && Elem2 && !Elem3) {
                const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(b1,b0,a1,a0)
                const __m128 val1 = _mm_movehl_ps(this->values0, other.values0); //(a3,a2,b3,b2)
                other.values0 = _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 2, 1, 2));
                lo = _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 0, 3, 0));
            } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
                const __m128 backup = other.values0;
                const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(b1,b0,a1,a0)
                other.values0 = _mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 1, 2));
                lo = _mm_shuffle_ps(val0, backup, _MM_SHUFFLE(3, 2, 3, 0));
            } else if constexpr (Elem0 && !Elem1 && !Elem2 && Elem3) {
                const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(b1,b0,a1,a0)
                const __m128 val1 = _mm_movehl_ps(this->values0, other.values0); //(a3,a2,b3,b2)
                other.values0 = _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 0, 3, 0));
                lo = _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 2, 1, 2));
            } else if constexpr (Elem0 && !Elem1 && Elem2 && !Elem3) {
                const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(b1,b0,a1,a0)
                const __m128 val1 = _mm_movehl_ps(this->values0, other.values0); //(a3,a2,b3,b2)
                other.values0 = _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(1, 2, 3, 0));
                lo = _mm_shuffle_ps(val0, val1, _MM_SHUFFLE(3, 0, 1, 2));
            } else if constexpr (Elem0 && !Elem1 && Elem2 && Elem3) {
                const __m128 backup = other.values0;
                const __m128 val0 = _mm_movelh_ps(this->values0, other.values0); //(b1,b0,a1,a0)
                other.values0 = _mm_shuffle_ps(val0, this->values0, _MM_SHUFFLE(3, 2, 3, 0));
                lo = _mm_shuffle_ps(val0, backup, _MM_SHUFFLE(3, 2, 1, 2));
            } else if constexpr (Elem0 && Elem1 && !Elem2 && !Elem3) {
                const __m128 backup = other.values0;
                other.values0 = _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(3, 2, 1, 0));
                lo = _mm_shuffle_ps(backup, this->values0, _MM_SHUFFLE(3, 2, 1, 0));
            } else if constexpr (Elem0 && Elem1 && !Elem2 && Elem3) {
                const __m128 backup = other.values0;
                const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); //(a3,a2,b3,b2)
                other.values0 = _mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(3, 0, 1, 0));
                lo = _mm_shuffle_ps(backup, val0, _MM_SHUFFLE(1, 2, 1, 0));
            } else /*Elem0 && Elem1 && Elem2 && !Elem3*/ {
                const __m128 backup = other.values0;
                const __m128 val0 = _mm_movehl_ps(this->values0, other.values0); //(a3,a2,b3,b2)
                other.values0 = _mm_shuffle_ps(this->values0, val0, _MM_SHUFFLE(1, 2, 1, 0));
                lo = _mm_shuffle_ps(backup, val0, _MM_SHUFFLE(3, 0, 1, 0));
            }

            __m128 hi;
            if constexpr (Elem4 && Elem5) {
                hi = other.values1;
                other.values1 = this->values1;
            } else if constexpr (!Elem4 && !Elem5) {
                hi = this->values1;
            } else if constexpr (Elem4 && !Elem5) {
                const __m128 backup = other.values1;
                other.values1 = _mm_move_ss(other.values1, this->values1);
                hi = _mm_move_ss(this->values1, backup);
            } else if constexpr (!Elem4 && Elem5) {
                const __m128 backup = other.values1;
                other.values1 = _mm_move_ss(this->values1, other.values1);
                hi = _mm_move_ss(backup, this->values1);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                const __m128 backup = other.values1;
                other.values1 = _mm_blend_ps(other.values1, this->values1, _MM_BLEND(0, 0, Elem5, Elem4));
                hi = _mm_blend_ps(this->values1, backup, _MM_BLEND(0, 0, Elem5, Elem4));
            } else if constexpr (!Elem4 && Elem5) {
                const __m128 val = _mm_movelh_ps(this->values1, other.values1); //(a1,a0,b1,b0)
                other.values1 = _mm_shuffle1_ps(val, _MM_SHUFFLE(2, 1, 3, 0));  //(x,x,3,0)
                hi = _mm_shuffle1_ps(val, _MM_SHUFFLE(3, 0, 2, 1));             //(x,x,2,1)
            } else /*Elem4 && !Elem5*/ {
                const __m128 val = _mm_movelh_ps(this->values1, other.values1);   //(a1,a0,b1,b0)
                other.values1 = _mm_shuffle1_ps(val, _MM_SHUFFLE(23, 0, 2, 1));   //(x,x,2,1)
                hi = _mm_shuffle_ps(val, other.values1, _MM_SHUFFLE(2, 1, 3, 0)); //(x,x,3,0)
            }

            return SIMD6(lo, hi);
        }
#endif
        else {
            const SIMD6 backup = other;
            other.values0 = (Elem0) ? this->values0 : other.values0;
            other.values1 = (Elem1) ? this->values1 : other.values1;
            other.values2 = (Elem2) ? this->values2 : other.values2;
            other.values3 = (Elem3) ? this->values3 : other.values3;
            other.values4 = (Elem4) ? this->values4 : other.values4;
            other.values5 = (Elem5) ? this->values5 : other.values5;
            return SIMD6((Elem0) ? backup.values0 : this->values0, (Elem1) ? backup.values1 : this->values1,
                (Elem2) ? backup.values2 : this->values2, (Elem3) ? backup.values3 : this->values3,
                (Elem4) ? backup.values4 : this->values4, (Elem5) ? backup.values5 : this->values5);
        }
    }

    /**
     * Swaps values between the second object and the first based on a dynamic mask.
     * @note A value form the second object is swapped with the first based on the corresponding input elements.
     * @param [in,out] other The object to swap elements with the first.
     * @param          mask The object mask used to define which elements to swap.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD6 blendSwapVar(SIMD6& other, const Mask& mask) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                const __m256 backup = other.values;
                other.values = _mm256_mask_blend_ps(mask.values, other.values, this->values);
                return SIMD6(_mm256_mask_blend_ps(mask.values, this->values, backup));
            } else {
                const __m256 backup = other.values;
                other.values = _mm256_blendv_ps(other.values, this->values, mask.values);
                return SIMD6(_mm256_blendv_ps(this->values, backup, mask.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                const __m128 backup0 = other.values0;
                const __m128 backup1 = other.values1;
                other.values0 = _mm_mask_blend_ps(mask.values0, other.values0, this->values0);
                other.values1 = _mm_mask_blend_ps(mask.values1, other.values1, this->values1);
                return SIMD6(_mm_mask_blend_ps(mask.values0, this->values0, backup0),
                    _mm_mask_blend_ps(mask.values1, this->values1, backup1));
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                const __m128 backup0 = other.values0;
                const __m128 backup1 = other.values1;
                other.values0 = _mm_blendv_ps(other.values0, this->values0, mask.values0);
                other.values1 = _mm_blendv_ps(other.values1, this->values1, mask.values1);
                return SIMD6(_mm_blendv_ps(this->values0, backup0, mask.values0),
                    _mm_blendv_ps(this->values1, backup1, mask.values1));
            } else {
                const __m128 val00A = _mm_and_ps(this->values0, mask.values0);
                const __m128 val00B = _mm_and_ps(other.values0, mask.values0);
                const __m128 val01A = _mm_andnot_ps(mask.values0, other.values0);
                const __m128 val01B = _mm_andnot_ps(mask.values0, this->values0);
                const __m128 val10A = _mm_and_ps(this->values1, mask.values1);
                const __m128 val10B = _mm_and_ps(other.values1, mask.values1);
                const __m128 val11A = _mm_andnot_ps(mask.values1, other.values1);
                const __m128 val11B = _mm_andnot_ps(mask.values1, this->values1);
                other.values0 = _mm_or_ps(val00A, val01A);
                other.values1 = _mm_or_ps(val10A, val11A);
                return SIMD6(_mm_or_ps(val00B, val01B), _mm_or_ps(val10B, val11B));
            }
        } else
#endif
        {
            const SIMD6 backup = other;
            other.values0 = (mask.values0) ? this->values0 : other.values0;
            other.values1 = (mask.values1) ? this->values1 : other.values1;
            other.values2 = (mask.values2) ? this->values2 : other.values2;
            other.values3 = (mask.values3) ? this->values3 : other.values3;
            other.values4 = (mask.values4) ? this->values4 : other.values4;
            other.values5 = (mask.values5) ? this->values5 : other.values5;
            return SIMD6((mask.values0) ? backup.values0 : this->values0,
                (mask.values1) ? backup.values1 : this->values1, (mask.values2) ? backup.values2 : this->values2,
                (mask.values3) ? backup.values3 : this->values3, (mask.values4) ? backup.values4 : this->values4,
                (mask.values5) ? backup.values5 : this->values5);
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
    XS_FUNCTION SIMD6 blend2(const SIMD6& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1) {
            return *this;
        } else if constexpr (Elem0 && Elem1) {
            return other;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(_mm256_blend_ps(
                this->values, other.values, _MM256_BLEND(Elem1, Elem0, Elem1, Elem0, Elem1, Elem0, Elem1, Elem0)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                return SIMD6(_mm_blend_ps(this->values0, other.values0, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)),
                    _mm_blend_ps(this->values1, other.values1, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)));
            } else {
                const __m128 val00 = _mm_movelh_ps(other.values0, this->values0); //(a1,a0,b1,b0)
                const __m128 val01 = _mm_movehl_ps(this->values0, other.values0); //(a3,a2,b3,b2)
                const __m128 val10 = _mm_movelh_ps(other.values1, this->values1);
                const __m128 val11 = _mm_movehl_ps(this->values1, other.values1);
                if constexpr (!Elem0 && Elem1) {
                    return SIMD6(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(1, 2, 1, 2)),
                        _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(1, 2, 1, 2)));
                } else /*Elem0 && !Elem1*/ {
                    return SIMD6(_mm_shuffle_ps(val00, val01, _MM_SHUFFLE(3, 0, 3, 0)),
                        _mm_shuffle_ps(val10, val11, _MM_SHUFFLE(3, 0, 3, 0)));
                }
            }
        }
#endif
        else {
            const SIMD3Def temp0 = (Elem0) ? SIMD3Def(other.values0, other.values2, other.values4) :
                                             SIMD3Def(this->values0, this->values2, this->values4);
            const SIMD3Def temp1 = (Elem1) ? SIMD3Def(other.values1, other.values3, other.values5) :
                                             SIMD3Def(this->values1, this->values3, this->values5);
            return SIMD6(temp0.values0, temp1.values0, temp0.values1, temp1.values1, temp0.values2, temp1.values2);
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
    XS_FUNCTION SIMD6 blend3(const SIMD6& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2) {
            return *this;
        } else if constexpr (Elem0 && Elem1 && Elem2) {
            return other;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMD6(_mm256_blend_ps(
                this->values, other.values, _MM256_BLEND(0, 0, Elem2, Elem2, Elem1, Elem1, Elem0, Elem0)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 lo;
            if constexpr (Elem0 && Elem1) {
                lo = other.values0;
            } else if constexpr (!Elem0 && !Elem1) {
                lo = this->values0;
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                lo = _mm_blend_ps(this->values0, other.values0, _MM_BLEND(Elem1, Elem1, Elem0, Elem0));
            } else if constexpr (!Elem0 && Elem1) {
                lo = _mm_shuffle_ps(this->values0, other.values0, _MM_SHUFFLE(3, 2, 1, 0));
            } else /*Elem0 && !Elem1*/ {
                lo = _mm_shuffle_ps(other.values0, this->values0, _MM_SHUFFLE(3, 2, 1, 0));
            }

            __m128 hi;
            if constexpr (Elem2) {
                hi = other.values1;
            } else /*!Elem2*/ {
                hi = this->values1;
            }

            return SIMD6(lo, hi);
        }
#endif
        else {
            const SIMD2Def temp0 =
                (Elem0) ? SIMD2Def(other.values0, other.values1) : SIMD2Def(this->values0, this->values1);
            const SIMD2Def temp1 =
                (Elem1) ? SIMD2Def(other.values2, other.values3) : SIMD2Def(this->values2, this->values3);
            const SIMD2Def temp2 =
                (Elem2) ? SIMD2Def(other.values4, other.values5) : SIMD2Def(this->values4, this->values5);
            return SIMD6(temp0.values0, temp0.values1, temp1.values0, temp1.values1, temp2.values0, temp2.values1);
        }
    }

    /**
     * Dynamically shuffles the elements of the object.
     * @param shuffle The shuffle mask used to shuffle.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD6 shuffleVar(const Shuffle& shuffle) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            XS_ASSERT((_mm256_movemask_epi8(_mm256_cmpgt_epi32(shuffle.values, _mm256_set1_epi32(5))) & 0xFFFFFF) == 0);
            if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                return SIMD6(_mm256_permutevar8x32_ps(this->values, shuffle.values));
            } else {
                const __m256i lanes = _mm256_cmpgt_epi32(shuffle.values, _mm256_set1_epi32(0x3));
                const __m256i shuff0 = _mm256_andnot_si256(lanes, shuffle.values);
                const __m256i shuff1 = _mm256_and_si256(lanes, shuffle.values);
                const __m256 low = _mm256_permutevar_ps(_mm256_shuffle32103210_ps(this->values), shuff0);
                const __m256 hi = _mm256_permutevar_ps(_mm256_shuffle76547654_ps(this->values), shuff1);
                return SIMD6(_mm256_blendv_ps(low, hi, _mm256_castsi256_ps(lanes)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX>) {
                XS_ASSERT(_mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values0, _mm_set1_epi32(5))) == 0 &&
                    (_mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values1, _mm_set1_epi32(5))) & 0xFF) == 0);
                const __m256 val0 = _mm256_set_m128(this->values1, this->values0);
                const __m256i val1 = _mm256_set_m128i(shuffle.values1, shuffle.values0);
                __m256 res;
                if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                    res = _mm256_permutevar8x32_ps(val0, val1);
                } else {
                    const __m256i lanes = _mm256_cmpgt_epi32(val1, _mm256_set1_epi32(0x3));
                    const __m256i shuff0 = _mm256_andnot_si256(lanes, val1);
                    const __m256i shuff1 = _mm256_and_si256(lanes, val1);
                    const __m256 low = _mm256_permutevar_ps(_mm256_shuffle32103210_ps(val0), shuff0);
                    const __m256 hi = _mm256_permutevar_ps(_mm256_shuffle76547654_ps(val0), shuff1);
                    res = _mm256_blendv_ps(low, hi, _mm256_castsi256_ps(lanes));
                }
                return SIMD6(_mm256_castps256_ps128(res), _mm256_extractf128_ps(res, 1));
            } else {
                XS_ASSERT(_mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values0, _mm_set1_epi32(0x17161514))) == 0 &&
                    (_mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values1, _mm_set1_epi32(0x17161514))) & 0xFF) == 0);
                const __m128i lanes = _mm_set1_epi32(0x0F0E0D0C);
                const __m128i lanes0 = _mm_cmpgt_epi32(shuffle.values0, lanes);
                const __m128i lanes1 = _mm_cmpgt_epi32(shuffle.values1, lanes);
                // Mask out shuffles to each 128bit lane
                const __m128i shuff00 = _mm_andnot_si128(lanes0, shuffle.values0);
                const __m128i shuff01 = _mm_andnot_si128(lanes1, shuffle.values1);
                const __m128i shuff10 = _mm_and_si128(lanes0, shuffle.values0);
                const __m128i shuff11 = _mm_and_si128(lanes1, shuffle.values1);
                const __m128i low0 = _mm_shuffle_epi8(_mm_castps_si128(this->values0), shuff00);
                const __m128i low1 = _mm_shuffle_epi8(_mm_castps_si128(this->values0), shuff01);
                const __m128i hi0 = _mm_shuffle_epi8(_mm_castps_si128(this->values1), shuff10);
                const __m128i hi1 = _mm_shuffle_epi8(_mm_castps_si128(this->values1), shuff11);
                if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                    return SIMD6(_mm_castsi128_ps(_mm_blendv_epi8(low0, hi0, lanes0)),
                        _mm_castsi128_ps(_mm_blendv_epi8(low1, hi1, lanes1)));
                } else {
                    const __m128i valLow0 = _mm_and_si128(hi0, lanes0);
                    const __m128i valLow1 = _mm_andnot_si128(lanes0, low0);
                    const __m128i valHi0 = _mm_and_si128(hi1, lanes1);
                    const __m128i valHi1 = _mm_andnot_si128(lanes1, low1);
                    return SIMD6(_mm_castsi128_ps(_mm_or_si128(valLow0, valLow1)),
                        _mm_castsi128_ps(_mm_or_si128(valHi0, valHi1)));
                }
            }
        } else
#endif
        {
            XS_ASSERT(shuffle.values0 < 6 && shuffle.values1 < 6 && shuffle.values2 < 6 && shuffle.values3 < 6 &&
                shuffle.values4 < 6 && shuffle.values5 < 6);
            return SIMD6((&this->values0)[shuffle.values0], (&this->values0)[shuffle.values1],
                (&this->values0)[shuffle.values2], (&this->values0)[shuffle.values3], (&this->values0)[shuffle.values4],
                (&this->values0)[shuffle.values5]);
        }
    }

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD2.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-1).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-1).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD6 shuffle2() const noexcept
    {
        static_assert(Index0 < 2 && Index1 < 2, "Invalid Index: Indexes must be <2");
        if constexpr (Index0 == 0 && Index1 == 1) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD6(_mm256_shuffle2200_ps(this->values));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD6(_mm256_shuffle3311_ps(this->values));
            } else {
                return SIMD6(_mm256_shuffle1_ps(this->values, _MM_SHUFFLE(Index1 + 2, Index0 + 2, Index1, Index0)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD6(_mm_moveldup_ps(this->values0), _mm_moveldup_ps(this->values1));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD6(_mm_movehdup_ps(this->values0), _mm_movehdup_ps(this->values1));
            } else /*Index0 == 1 && Index1 == 0*/ {
                return SIMD6(_mm_shuffle1_ps(this->values0, _MM_SHUFFLE(2, 3, 0, 1)),
                    _mm_shuffle1_ps(this->values1, _MM_SHUFFLE(2, 3, 0, 1)));
            }
        }
#endif
        else {
            return SIMD6((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index0 + 2],
                (&this->values0)[Index1 + 2], (&this->values0)[Index0 + 4], (&this->values0)[Index1 + 4]);
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
    XS_FUNCTION SIMD6 shuffle3() const noexcept
    {
        static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3, "Invalid Index: Indexes must be <3");
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 0) {
                if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                    return SIMD6(_mm256_broadcastf64_ps(_mm256_castps256_ps128(this->values)));
                } else {
                    return SIMD6(_mm256_set_m128(_mm256_castps256_ps128(this->values),
                        _mm_shuffle1010_ps(_mm256_castps256_ps128(this->values))));
                }
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
                return SIMD6(_mm256_set_m128(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values)),
                    _mm_shuffle1010_ps(_mm256_castps256_ps128(this->values))));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
                return SIMD6(_mm256_shuffle1010_ps(this->values));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
                return SIMD6(_mm256_insertf128_ps(this->values, _mm256_castps256_ps128(this->values), 1));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 1) {
                return SIMD6(
                    _mm256_insertf128_ps(this->values, _mm_shuffle3232_ps(_mm256_castps256_ps128(this->values)), 1));
            } else if constexpr (Index0 == 0 && Index1 == 2 && Index2 == 0) {
                const __m128 val = _mm_shuffle_ps(_mm256_castps256_ps128(this->values),
                    _mm256_extractf128_ps(this->values, 1), _MM_SHUFFLE(1, 0, 1, 0));
                return SIMD6(_mm256_set_m128(_mm256_castps256_ps128(this->values), val));
            } else if constexpr (Index0 == 0 && Index1 == 2 && Index2 == 1) {
                const __m128 val = _mm_shuffle_ps(_mm256_castps256_ps128(this->values),
                    _mm256_extractf128_ps(this->values, 1), _MM_SHUFFLE(1, 0, 1, 0));
                return SIMD6(_mm256_set_m128(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values)), val));
            } else if constexpr (Index0 == 0 && Index1 == 2 && Index2 == 2) {
                const __m128 val = _mm_shuffle_ps(_mm256_castps256_ps128(this->values),
                    _mm256_extractf128_ps(this->values, 1), _MM_SHUFFLE(1, 0, 1, 0));
                return SIMD6(_mm256_permute2f128_ps(_mm256_castps128_ps256(val), this->values, _MM256_PERMUTE(3, 0)));
            } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 0) {
                return SIMD6(_mm256_set_m128(_mm256_castps256_ps128(this->values),
                    _mm_shuffle1_ps(_mm256_castps256_ps128(this->values), _MM_SHUFFLE(1, 0, 3, 2))));
            } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 1) {
                const __m128 val = _mm_shuffle1_ps(_mm256_castps256_ps128(this->values), _MM_SHUFFLE(1, 0, 3, 2));
                return SIMD6(_mm256_broadcastf128_ps(val));
            } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 2) {
                const __m128 val = _mm_shuffle1_ps(_mm256_castps256_ps128(this->values), _MM_SHUFFLE(1, 0, 3, 2));
                return SIMD6(_mm256_permute2f128_ps(_mm256_castps128_ps256(val), this->values, _MM256_PERMUTE(3, 0)));
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 0) {
                return SIMD6(_mm256_set_m128(
                    _mm256_castps256_ps128(this->values), _mm_shuffle3232_ps(_mm256_castps256_ps128(this->values))));
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 1) {
                const __m128 val = _mm_shuffle3232_ps(_mm256_castps256_ps128(this->values));
                return SIMD6(_mm256_broadcastf128_ps(val));
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 2) {
                const __m128 val = _mm_shuffle3232_ps(_mm256_castps256_ps128(this->values));
                return SIMD6(_mm256_permute2f128_ps(_mm256_castps128_ps256(val), this->values, _MM256_PERMUTE(3, 0)));
            } else if constexpr (Index0 == 1 && Index1 == 2 && Index2 == 0) {
                const __m128 val = _mm_shuffle_ps(_mm256_castps256_ps128(this->values),
                    _mm256_extractf128_ps(this->values, 1), _MM_SHUFFLE(1, 0, 3, 2));
                return SIMD6(_mm256_set_m128(_mm256_castps256_ps128(this->values), val));
            } else if constexpr (Index0 == 1 && Index1 == 2 && Index2 == 1) {
                const __m128 val = _mm_shuffle_ps(_mm256_castps256_ps128(this->values),
                    _mm256_extractf128_ps(this->values, 1), _MM_SHUFFLE(1, 0, 3, 2));
                return SIMD6(_mm256_broadcastf128_ps(val));
            } else if constexpr (Index0 == 1 && Index1 == 2 && Index2 == 2) {
                const __m128 val = _mm_shuffle_ps(_mm256_castps256_ps128(this->values),
                    _mm256_extractf128_ps(this->values, 1), _MM_SHUFFLE(1, 0, 3, 2));
                return SIMD6(_mm256_permute2f128_ps(_mm256_castps128_ps256(val), this->values, _MM256_PERMUTE(3, 0)));
            } else if constexpr (Index0 == 2 && Index1 == 0 && Index2 == 0) {
                const __m128 val = _mm_shuffle_ps(_mm256_extractf128_ps(this->values, 1),
                    _mm256_castps256_ps128(this->values), _MM_SHUFFLE(1, 0, 1, 0));
                return SIMD6(_mm256_set_m128(_mm256_castps256_ps128(this->values), val));
            } else if constexpr (Index0 == 2 && Index1 == 0 && Index2 == 1) {
                const __m128 val = _mm_shuffle_ps(_mm256_extractf128_ps(this->values, 1),
                    _mm256_castps256_ps128(this->values), _MM_SHUFFLE(1, 0, 1, 0));
                return SIMD6(_mm256_set_m128(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values)), val));
            } else if constexpr (Index0 == 2 && Index1 == 0 && Index2 == 2) {
                const __m128 val = _mm_shuffle_ps(_mm256_extractf128_ps(this->values, 1),
                    _mm256_castps256_ps128(this->values), _MM_SHUFFLE(1, 0, 1, 0));
                return SIMD6(_mm256_broadcastf128_ps(val));
            } else if constexpr (Index0 == 2 && Index1 == 1 && Index2 == 0) {
                const __m128 val = _mm_shuffle_ps(_mm256_extractf128_ps(this->values, 1),
                    _mm256_castps256_ps128(this->values), _MM_SHUFFLE(3, 2, 1, 0));
                return SIMD6(_mm256_set_m128(_mm256_castps256_ps128(this->values), val));
            } else if constexpr (Index0 == 2 && Index1 == 1 && Index2 == 1) {
                const __m128 val = _mm_shuffle_ps(_mm256_extractf128_ps(this->values, 1),
                    _mm256_castps256_ps128(this->values), _MM_SHUFFLE(3, 2, 1, 0));
                return SIMD6(_mm256_set_m128(_mm_shuffle3232_ps(_mm256_castps256_ps128(this->values)), val));
            } else if constexpr (Index0 == 2 && Index1 == 1 && Index2 == 2) {
                const __m128 val = _mm_shuffle_ps(_mm256_extractf128_ps(this->values, 1),
                    _mm256_castps256_ps128(this->values), _MM_SHUFFLE(3, 2, 1, 0));
                return SIMD6(_mm256_broadcastf128_ps(val));
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 0) {
                return SIMD6(_mm256_shuffle1010_ps(_mm256_shuffle32107654_ps(this->values)));
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 1) {
                const __m256 val0 = _mm256_shuffle1010_ps(this->values);
                const __m256 val1 = _mm256_shuffle1_ps(this->values, _MM_SHUFFLE(3, 2, 3, 2));
                return SIMD6(_mm256_permute2f128_ps(val0, val1, _MM256_PERMUTE(2, 1)));
            } else /*Index0 == 2 && Index1 == 2 && Index2 == 2*/ {
                return SIMD6(_mm256_shuffle1010_ps(_mm256_shuffle76547654_ps(this->values)));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 0) {
                return SIMD6(_mm_shuffle1010_ps(this->values0), this->values0);
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
                return SIMD6(_mm_shuffle1010_ps(this->values0), _mm_shuffle3232_ps(this->values0));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
                return SIMD6(_mm_shuffle1010_ps(this->values0), this->values1);
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
                return SIMD6(this->values0, this->values0);
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 1) {
                return SIMD6(this->values0, _mm_shuffle3232_ps(this->values0));
            } else if constexpr (Index0 == 0 && Index1 == 2 && Index2 == 0) {
                return SIMD6(_mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(1, 0, 1, 0)), this->values0);
            } else if constexpr (Index0 == 0 && Index1 == 2 && Index2 == 1) {
                return SIMD6(_mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(1, 0, 1, 0)),
                    _mm_shuffle3232_ps(this->values0));
            } else if constexpr (Index0 == 0 && Index1 == 2 && Index2 == 2) {
                return SIMD6(_mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(1, 0, 1, 0)), this->values1);
            } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 0) {
                return SIMD6(_mm_shuffle1_ps(this->values0, _MM_SHUFFLE(1, 0, 3, 2)), this->values0);
            } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 1) {
                const __m128 val = _mm_shuffle1_ps(this->values0, _MM_SHUFFLE(1, 0, 3, 2));
                return SIMD6(val, val);
            } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 2) {
                return SIMD6(_mm_shuffle1_ps(this->values0, _MM_SHUFFLE(1, 0, 3, 2)), this->values1);
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 0) {
                return SIMD6(_mm_shuffle3232_ps(this->values0), this->values0);
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 1) {
                const __m128 val = _mm_shuffle3232_ps(this->values0);
                return SIMD6(val, val);
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 2) {
                return SIMD6(_mm_shuffle3232_ps(this->values0), this->values1);
            } else if constexpr (Index0 == 1 && Index1 == 2 && Index2 == 0) {
                return SIMD6(_mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(1, 0, 3, 2)), this->values0);
            } else if constexpr (Index0 == 1 && Index1 == 2 && Index2 == 1) {
                const __m128 val = _mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(1, 0, 3, 2));
                return SIMD6(val, val);
            } else if constexpr (Index0 == 1 && Index1 == 2 && Index2 == 2) {
                return SIMD6(_mm_shuffle_ps(this->values0, this->values1, _MM_SHUFFLE(1, 0, 3, 2)), this->values1);
            } else if constexpr (Index0 == 2 && Index1 == 0 && Index2 == 0) {
                return SIMD6(_mm_shuffle_ps(this->values1, this->values0, _MM_SHUFFLE(1, 0, 1, 0)), this->values0);
            } else if constexpr (Index0 == 2 && Index1 == 0 && Index2 == 1) {
                return SIMD6(_mm_shuffle_ps(this->values1, this->values0, _MM_SHUFFLE(1, 0, 1, 0)),
                    _mm_shuffle3232_ps(this->values0));
            } else if constexpr (Index0 == 2 && Index1 == 0 && Index2 == 2) {
                return SIMD6(_mm_shuffle_ps(this->values1, this->values0, _MM_SHUFFLE(1, 0, 1, 0)), this->values1);
            } else if constexpr (Index0 == 2 && Index1 == 1 && Index2 == 0) {
                return SIMD6(_mm_shuffle_ps(this->values1, this->values0, _MM_SHUFFLE(3, 2, 1, 0)), this->values0);
            } else if constexpr (Index0 == 2 && Index1 == 1 && Index2 == 1) {
                return SIMD6(_mm_shuffle_ps(this->values1, this->values0, _MM_SHUFFLE(3, 2, 1, 0)),
                    _mm_shuffle3232_ps(this->values0));
            } else if constexpr (Index0 == 2 && Index1 == 1 && Index2 == 2) {
                return SIMD6(_mm_shuffle_ps(this->values1, this->values0, _MM_SHUFFLE(3, 2, 1, 0)), this->values1);
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 0) {
                return SIMD6(_mm_shuffle1010_ps(this->values1), this->values0);
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 1) {
                return SIMD6(_mm_shuffle1010_ps(this->values1), _mm_shuffle3232_ps(this->values0));
            } else /*Index0 == 2 && Index1 == 2 && Index2 == 2*/
            {
                return SIMD6(_mm_shuffle1010_ps(this->values1), this->values1);
            }
        }
#endif
        else {
            return SIMD6((&this->values0)[Index0 * 2], (&this->values0)[Index0 * 2 + 1], (&this->values0)[Index1 * 2],
                (&this->values0)[Index1 * 2 + 1], (&this->values0)[Index2 * 2], (&this->values0)[Index2 * 2 + 1]);
        }
    }
};

/**
 * Add two SIMD6s.
 * @param other1 The first SIMD6.
 * @param other2 SIMD6 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator+(const SIMD6<T, Width>& other1, const SIMD6<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD6<T, Width>(_mm256_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD6<T, Width>(_mm_add_ps(other1.values0, other2.values0), _mm_add_ps(other1.values1, other2.values1));
    } else
#endif
    {
        return SIMD6<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values2, other1.values3 + other2.values3, other1.values4 + other2.values4,
            other1.values5 + other2.values5);
    }
}

/**
 * Add a value to all elements of a SIMD6.
 * @param other1 The SIMD6.
 * @param other2 Value to add to the SIMD6.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator+(const SIMD6<T, Width>& other1, typename SIMD6<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD6<T, Width>(_mm256_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD6<T, Width>(_mm_add_ps(other1.values0, other2.values), _mm_add_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD6<T, Width>(other1.values0 + other2.value, other1.values1 + other2.value,
            other1.values2 + other2.value, other1.values3 + other2.value, other1.values4 + other2.value,
            other1.values5 + other2.value);
    }
}

/**
 * Add a SIMD2 to each pair of elements in a SIMD6.
 * @param other1 The SIMD6.
 * @param other2 Values to add to the SIMD6.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator+(
    const SIMD6<T, Width>& other1, const typename SIMD6<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            return SIMD6<T, Width>(_mm256_add_ps(other1.values, _mm256_broadcastf64_ps(other2.values)));
        } else {
            const __m128 val = _mm_shuffle1010_ps(other2.values);
            return SIMD6<T, Width>(_mm256_add_ps(other1.values, _mm256_set_m128(other2.values, val)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        return SIMD6<T, Width>(_mm_add_ps(other1.values0, val), _mm_add_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD6<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values1,
            other1.values2 + other2.values0, other1.values3 + other2.values1, other1.values4 + other2.values0,
            other1.values5 + other2.values1);
    }
}

/**
 * Add a SIMD3 to each triple of elements in a SIMD6.
 * @param other1 The SIMD6.
 * @param other2 Values to add to the SIMD6.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator+(
    const SIMD6<T, Width>& other1, const typename SIMD6<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set_m128(_mm_shuffle3322_ps(other2.values), _mm_shuffle1100_ps(other2.values));
        return SIMD6<T, Width>(_mm256_add_ps(other1.values, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle1100_ps(other2.values);
        const __m128 val1 = _mm_shuffle3322_ps(other2.values);
        return SIMD6<T, Width>(_mm_add_ps(other1.values0, val0), _mm_add_ps(other1.values1, val1));
    } else
#endif
    {
        return SIMD6<T, Width>(other1.values0 + other2.values0, other1.values1 + other2.values0,
            other1.values2 + other2.values1, other1.values3 + other2.values1, other1.values4 + other2.values2,
            other1.values5 + other2.values2);
    }
}

/**
 * Subtract a SIMD6 from another SIMD6.
 * @param other1 The first SIMD6.
 * @param other2 SIMD6 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator-(const SIMD6<T, Width>& other1, const SIMD6<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD6<T, Width>(_mm256_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD6<T, Width>(_mm_sub_ps(other1.values0, other2.values0), _mm_sub_ps(other1.values1, other2.values1));
    } else
#endif
    {
        return SIMD6<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values2, other1.values3 - other2.values3, other1.values4 - other2.values4,
            other1.values5 - other2.values5);
    }
}

/**
 * Subtract a value from all elements of a SIMD6.
 * @param other1 The SIMD6.
 * @param other2 Value to subtract from the SIMD6.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator-(const SIMD6<T, Width>& other1, typename SIMD6<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD6<T, Width>(_mm256_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD6<T, Width>(_mm_sub_ps(other1.values0, other2.values), _mm_sub_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD6<T, Width>(other1.values0 - other2.value, other1.values1 - other2.value,
            other1.values2 - other2.value, other1.values3 - other2.value, other1.values4 - other2.value,
            other1.values5 - other2.value);
    }
}

/**
 * Subtract all elements of a SIMD6 from a value.
 * @param other1 Value to subtract from the SIMD6.
 * @param other2 The SIMD6.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator-(typename SIMD6<T, Width>::BaseDef other1, const SIMD6<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD6<T, Width>(_mm256_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD6<T, Width>(_mm_sub_ps(other1.values, other2.values0), _mm_sub_ps(other1.values, other2.values1));
    } else
#endif
    {
        return SIMD6<T, Width>(other1.value - other2.values0, other1.value - other2.values1,
            other1.value - other2.values2, other1.value - other2.values3, other1.value - other2.values4,
            other1.value - other2.values5);
    }
}

/**
 * Subtract a SIMD2 from each pair of elements in a SIMD6.
 * @param other1 The SIMD6.
 * @param other2 Values to add to the SIMD6.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator-(
    const SIMD6<T, Width>& other1, const typename SIMD6<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            return SIMD6<T, Width>(_mm256_sub_ps(other1.values, _mm256_broadcastf64_ps(other2.values)));
        } else {
            const __m128 val = _mm_shuffle1010_ps(other2.values);
            return SIMD6<T, Width>(_mm256_sub_ps(other1.values, _mm256_set_m128(other2.values, val)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        return SIMD6<T, Width>(_mm_sub_ps(other1.values0, val), _mm_sub_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD6<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values1,
            other1.values2 - other2.values0, other1.values3 - other2.values1, other1.values4 - other2.values0,
            other1.values5 - other2.values1);
    }
}

/**
 * Subtract a SIMD3 from each triple of elements in a SIMD6.
 * @param other1 The SIMD6.
 * @param other2 Values to add to the SIMD6.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator-(
    const SIMD6<T, Width>& other1, const typename SIMD6<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set_m128(_mm_shuffle3322_ps(other2.values), _mm_shuffle1100_ps(other2.values));
        return SIMD6<T, Width>(_mm256_sub_ps(other1.values, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle1100_ps(other2.values);
        const __m128 val1 = _mm_shuffle3322_ps(other2.values);
        return SIMD6<T, Width>(_mm_sub_ps(other1.values0, val0), _mm_sub_ps(other1.values1, val1));
    } else
#endif
    {
        return SIMD6<T, Width>(other1.values0 - other2.values0, other1.values1 - other2.values0,
            other1.values2 - other2.values1, other1.values3 - other2.values1, other1.values4 - other2.values2,
            other1.values5 - other2.values2);
    }
}

/**
 * Multiply two SIMD6s per element.
 * @param other1 The first SIMD6.
 * @param other2 The second SIMD6 to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator*(const SIMD6<T, Width>& other1, const SIMD6<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD6<T, Width>(_mm256_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD6<T, Width>(_mm_mul_ps(other1.values0, other2.values0), _mm_mul_ps(other1.values1, other2.values1));
    } else
#endif
    {
        return SIMD6<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values2, other1.values3 * other2.values3, other1.values4 * other2.values4,
            other1.values5 * other2.values5);
    }
}

/**
 * Multiply a SIMD6 by a scalar.
 * @param other1 The SIMD6.
 * @param other2 Value to multiply the SIMD6 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator*(const SIMD6<T, Width>& other1, typename SIMD6<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD6<T, Width>(_mm256_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD6<T, Width>(_mm_mul_ps(other1.values0, other2.values), _mm_mul_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD6<T, Width>(other1.values0 * other2.value, other1.values1 * other2.value,
            other1.values2 * other2.value, other1.values3 * other2.value, other1.values4 * other2.value,
            other1.values5 * other2.value);
    }
}

/**
 * Multiply each pair of elements in a SIMD6 by a SIMD2.
 * @param other1 The SIMD6.
 * @param other2 Values to multiply the SIMD6 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator*(
    const SIMD6<T, Width>& other1, const typename SIMD6<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            return SIMD6<T, Width>(_mm256_mul_ps(other1.values, _mm256_broadcastf64_ps(other2.values)));
        } else {
            const __m128 val = _mm_shuffle1010_ps(other2.values);
            return SIMD6<T, Width>(_mm256_mul_ps(other1.values, _mm256_set_m128(other2.values, val)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        return SIMD6<T, Width>(_mm_mul_ps(other1.values0, val), _mm_mul_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD6<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values1,
            other1.values2 * other2.values0, other1.values3 * other2.values1, other1.values4 * other2.values0,
            other1.values5 * other2.values1);
    }
}

/**
 * Multiply each triple of elements in a SIMD6 by a SIMD3.
 * @param other1 The SIMD6.
 * @param other2 Values to multiply the SIMD6 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator*(
    const SIMD6<T, Width>& other1, const typename SIMD6<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set_m128(_mm_shuffle3322_ps(other2.values), _mm_shuffle1100_ps(other2.values));
        return SIMD6<T, Width>(_mm256_mul_ps(other1.values, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle1100_ps(other2.values);
        const __m128 val1 = _mm_shuffle3322_ps(other2.values);
        return SIMD6<T, Width>(_mm_mul_ps(other1.values0, val0), _mm_mul_ps(other1.values1, val1));
    } else
#endif
    {
        return SIMD6<T, Width>(other1.values0 * other2.values0, other1.values1 * other2.values0,
            other1.values2 * other2.values1, other1.values3 * other2.values1, other1.values4 * other2.values2,
            other1.values5 * other2.values2);
    }
}

/**
 * Divide two SIMD6s per element.
 * @param other1 The first SIMD6.
 * @param other2 The second SIMD6 to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator/(const SIMD6<T, Width>& other1, const SIMD6<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD6<T, Width>(_mm256_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD6<T, Width>(_mm_div_ps(other1.values0, other2.values0), _mm_div_ps(other1.values1, other2.values1));
    } else
#endif
    {
        return SIMD6<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values2, other1.values3 / other2.values3, other1.values4 / other2.values4,
            other1.values5 / other2.values5);
    }
}

/**
 * Divide a SIMD6 by a scalar.
 * @param other1 The SIMD6.
 * @param other2 Value to divide the SIMD6 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator/(const SIMD6<T, Width>& other1, typename SIMD6<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD6<T, Width>(_mm256_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD6<T, Width>(_mm_div_ps(other1.values0, other2.values), _mm_div_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD6<T, Width>(other1.values0 / other2.value, other1.values1 / other2.value,
            other1.values2 / other2.value, other1.values3 / other2.value, other1.values4 / other2.value,
            other1.values5 / other2.value);
    }
}

/**
 * Divide a scalar by all elements of a SIMD6.
 * @param other1 Value to divide the SIMD6 by.
 * @param other2 The SIMD6.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator/(typename SIMD6<T, Width>::BaseDef other1, const SIMD6<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD6<T, Width>(_mm256_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD6<T, Width>(_mm_div_ps(other1.values, other2.values0), _mm_div_ps(other1.values, other2.values1));
    } else
#endif
    {
        return SIMD6<T, Width>(other1.value / other2.values0, other1.value / other2.values1,
            other1.value / other2.values2, other1.value / other2.values3, other1.value / other2.values4,
            other1.value / other2.values5);
    }
}

/**
 * Divide each pair of elements in a SIMD6 by a SIMD2.
 * @param other1 The SIMD6.
 * @param other2 Values to divide the SIMD6 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator/(
    const SIMD6<T, Width>& other1, const typename SIMD6<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            return SIMD6<T, Width>(_mm256_div_ps(other1.values, _mm256_broadcastf64_ps(other2.values)));
        } else {
            const __m128 val = _mm_shuffle1010_ps(other2.values);
            return SIMD6<T, Width>(_mm256_div_ps(other1.values, _mm256_set_m128(other2.values, val)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        return SIMD6<T, Width>(_mm_div_ps(other1.values0, val), _mm_div_ps(other1.values1, other2.values));
    } else
#endif
    {
        return SIMD6<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values1,
            other1.values2 / other2.values0, other1.values3 / other2.values1, other1.values4 / other2.values0,
            other1.values5 / other2.values1);
    }
}

/**
 * Divide each triple of elements in a SIMD6 by a SIMD3.
 * @param other1 The SIMD6.
 * @param other2 Values to divide the SIMD6 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator/(
    const SIMD6<T, Width>& other1, const typename SIMD6<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set_m128(_mm_shuffle3322_ps(other2.values), _mm_shuffle1100_ps(other2.values));
        return SIMD6<T, Width>(_mm256_div_ps(other1.values, val));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle1100_ps(other2.values);
        const __m128 val1 = _mm_shuffle3322_ps(other2.values);
        return SIMD6<T, Width>(_mm_div_ps(other1.values0, val0), _mm_div_ps(other1.values1, val1));
    } else
#endif
    {
        return SIMD6<T, Width>(other1.values0 / other2.values0, other1.values1 / other2.values0,
            other1.values2 / other2.values1, other1.values3 / other2.values1, other1.values4 / other2.values2,
            other1.values5 / other2.values2);
    }
}

/**
 * Negate all elements of a SIMD6.
 * @param other The SIMD6 to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator-(const SIMD6<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD6<T, Width>(_mm256_sub_ps(_mm256_setzero_ps(), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_setzero_ps();
        return SIMD6<T, Width>(_mm_sub_ps(val, other.values0), _mm_sub_ps(val, other.values1));
    } else
#endif
    {
        return SIMD6<T, Width>(
            -other.values0, -other.values1, -other.values2, -other.values3, -other.values4, -other.values5);
    }
}

/**
 * Perform compound assignment and addition with a SIMD6.
 * @param [in,out] other1 The first SIMD6.
 * @param          other2 SIMD6 to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width>& operator+=(SIMD6<T, Width>& other1, const SIMD6<T, Width>& other2) noexcept
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
 * Perform compound assignment and addition with a SIMD6.
 * @param [in,out] other1 The SIMD6.
 * @param          other2 Value to add to the first SIMD6.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width>& operator+=(SIMD6<T, Width>& other1, typename SIMD6<T, Width>::BaseDef other2) noexcept
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
 * Perform compound assignment and addition of a SIMD2 to each pair of elements in a SIMD6.
 * @param [in,out] other1 The SIMD6.
 * @param          other2 Values to add to the SIMD6.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width>& operator+=(
    SIMD6<T, Width>& other1, const typename SIMD6<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            other1.values = _mm256_add_ps(other1.values, _mm256_broadcastf64_ps(other2.values));
        } else {
            const __m128 val = _mm_shuffle1010_ps(other2.values);
            other1.values = _mm256_add_ps(other1.values, _mm256_set_m128(other2.values, val));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        other1.values0 = _mm_add_ps(other1.values0, val);
        other1.values1 = _mm_add_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
        other1.values2 += other2.values0;
        other1.values3 += other2.values1;
        other1.values4 += other2.values0;
        other1.values5 += other2.values1;
    }
    return other1;
}

/**
 * Perform compound assignment and addition of a SIMD3 to each triple of elements in a SIMD6.
 * @param [in,out] other1 The SIMD6.
 * @param          other2 Values to add to the SIMD6.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width>& operator+=(
    SIMD6<T, Width>& other1, const typename SIMD6<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set_m128(_mm_shuffle3322_ps(other2.values), _mm_shuffle1100_ps(other2.values));
        other1.values = _mm256_add_ps(other1.values, val);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle1100_ps(other2.values);
        const __m128 val1 = _mm_shuffle3322_ps(other2.values);
        other1.values0 = _mm_add_ps(other1.values0, val0);
        other1.values1 = _mm_add_ps(other1.values1, val1);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values0;
        other1.values2 += other2.values1;
        other1.values3 += other2.values1;
        other1.values4 += other2.values2;
        other1.values5 += other2.values2;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction by a SIMD6.
 * @param [in,out] other1 The first SIMD6.
 * @param          other2 SIMD6 to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width>& operator-=(SIMD6<T, Width>& other1, const SIMD6<T, Width>& other2) noexcept
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
 * Perform compound assignment and subtraction with a SIMD6.
 * @param [in,out] other1 The SIMD6.
 * @param          other2 Value to subtract from the first SIMD6.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width>& operator-=(SIMD6<T, Width>& other1, typename SIMD6<T, Width>::BaseDef other2) noexcept
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
 * Perform compound assignment and subtraction of a SIMD2 from each pair of elements in a SIMD6.
 * @param [in,out] other1 The SIMD6.
 * @param          other2 Values to subtract from the SIMD6.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width>& operator-=(
    SIMD6<T, Width>& other1, const typename SIMD6<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            other1.values = _mm256_sub_ps(other1.values, _mm256_broadcastf64_ps(other2.values));
        } else {
            const __m128 val = _mm_shuffle1010_ps(other2.values);
            other1.values = _mm256_sub_ps(other1.values, _mm256_set_m128(other2.values, val));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        other1.values0 = _mm_sub_ps(other1.values0, val);
        other1.values1 = _mm_sub_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
        other1.values2 -= other2.values0;
        other1.values3 -= other2.values1;
        other1.values4 -= other2.values0;
        other1.values5 -= other2.values1;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction of a SIMD3 from each triple of elements in a SIMD6.
 * @param [in,out] other1 The SIMD6.
 * @param          other2 Values to subtract from the SIMD6.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width>& operator-=(
    SIMD6<T, Width>& other1, const typename SIMD6<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set_m128(_mm_shuffle3322_ps(other2.values), _mm_shuffle1100_ps(other2.values));
        other1.values = _mm256_sub_ps(other1.values, val);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle1100_ps(other2.values);
        const __m128 val1 = _mm_shuffle3322_ps(other2.values);
        other1.values0 = _mm_sub_ps(other1.values0, val0);
        other1.values1 = _mm_sub_ps(other1.values1, val1);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values0;
        other1.values2 -= other2.values1;
        other1.values3 -= other2.values1;
        other1.values4 -= other2.values2;
        other1.values5 -= other2.values2;
    }
    return other1;
}

/**
 * Perform SIMD6 compound assignment and multiplication.
 * @param [in,out] other1 The first SIMD6.
 * @param          other2 SIMD6 to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width>& operator*=(SIMD6<T, Width>& other1, const SIMD6<T, Width>& other2) noexcept
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
 * @param [in,out] other1 The SIMD6.
 * @param          other2 Value to multiply the SIMD6 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width>& operator*=(SIMD6<T, Width>& other1, typename SIMD6<T, Width>::BaseDef other2) noexcept
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
 * Perform compound assignment and multiplication of each pair of elements in a SIMD6 by a SIMD2.
 * @param [in,out] other1 The SIMD6.
 * @param          other2 Values to multiply the SIMD6 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width>& operator*=(
    SIMD6<T, Width>& other1, const typename SIMD6<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            other1.values = _mm256_mul_ps(other1.values, _mm256_broadcastf64_ps(other2.values));
        } else {
            const __m128 val = _mm_shuffle1010_ps(other2.values);
            other1.values = _mm256_mul_ps(other1.values, _mm256_set_m128(other2.values, val));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        other1.values0 = _mm_mul_ps(other1.values0, val);
        other1.values1 = _mm_mul_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
        other1.values2 *= other2.values0;
        other1.values3 *= other2.values1;
        other1.values4 *= other2.values0;
        other1.values5 *= other2.values1;
    }
    return other1;
}

/**
 * Perform compound assignment and multiplication of each triple of elements in a SIMD6 by a SIMD3.
 * @param [in,out] other1 The SIMD6.
 * @param          other2 Values to multiply the SIMD6 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width>& operator*=(
    SIMD6<T, Width>& other1, const typename SIMD6<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set_m128(_mm_shuffle3322_ps(other2.values), _mm_shuffle1100_ps(other2.values));
        other1.values = _mm256_mul_ps(other1.values, val);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle1100_ps(other2.values);
        const __m128 val1 = _mm_shuffle3322_ps(other2.values);
        other1.values0 = _mm_mul_ps(other1.values0, val0);
        other1.values1 = _mm_mul_ps(other1.values1, val1);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values0;
        other1.values2 *= other2.values1;
        other1.values3 *= other2.values1;
        other1.values4 *= other2.values2;
        other1.values5 *= other2.values2;
    }
    return other1;
}

/**
 * Perform compound assignment and division.
 * @param [in,out] other1 The SIMD6.
 * @param          other2 Value to divide the SIMD6 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width>& operator/=(SIMD6<T, Width>& other1, const SIMD6<T, Width>& other2) noexcept
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
 * @param [in,out] other1 The SIMD6.
 * @param          other2 Value to divide the SIMD6 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width>& operator/=(SIMD6<T, Width>& other1, typename SIMD6<T, Width>::BaseDef other2) noexcept
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
 * Perform compound assignment and division of each pair of elements in a SIMD6 by a SIMD2.
 * @param [in,out] other1 The SIMD6.
 * @param          other2 Values to divide the SIMD6 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width>& operator/=(
    SIMD6<T, Width>& other1, const typename SIMD6<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
            other1.values = _mm256_div_ps(other1.values, _mm256_broadcastf64_ps(other2.values));
        } else {
            const __m128 val = _mm_shuffle1010_ps(other2.values);
            other1.values = _mm256_div_ps(other1.values, _mm256_set_m128(other2.values, val));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_shuffle1010_ps(other2.values);
        other1.values0 = _mm_div_ps(other1.values0, val);
        other1.values1 = _mm_div_ps(other1.values1, other2.values);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
        other1.values2 /= other2.values0;
        other1.values3 /= other2.values1;
        other1.values4 /= other2.values0;
        other1.values5 /= other2.values1;
    }
    return other1;
}

/**
 * Perform compound assignment and division of each triple of elements in a SIMD6 by a SIMD3.
 * @param [in,out] other1 The SIMD6.
 * @param          other2 Values to divide the SIMD6 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width>& operator/=(
    SIMD6<T, Width>& other1, const typename SIMD6<T, Width>::SIMD3Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_set_m128(_mm_shuffle3322_ps(other2.values), _mm_shuffle1100_ps(other2.values));
        other1.values = _mm256_div_ps(other1.values, val);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val0 = _mm_shuffle1100_ps(other2.values);
        const __m128 val1 = _mm_shuffle3322_ps(other2.values);
        other1.values0 = _mm_div_ps(other1.values0, val0);
        other1.values1 = _mm_div_ps(other1.values1, val1);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values0;
        other1.values2 /= other2.values1;
        other1.values3 /= other2.values1;
        other1.values4 /= other2.values2;
        other1.values5 /= other2.values2;
    }
    return other1;
}

/**
 * Compare two SIMD6s are equal.
 * @param other1 The first SIMD6.
 * @param other2 The second SIMD6 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator==(const SIMD6<T, Width>& other1, const SIMD6<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NEQ_UQ);
        return ((_mm256_movemask_ps(val) & 0x3F) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 val0 = _mm_cmp_ps(other1.values0, other2.values0, _CMP_NEQ_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values1, _CMP_NEQ_UQ);
            val1 = _mm_shuffle1010_ps(val1);
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values0, other2.values0)) |
                (_mm_movemask_ps(_mm_cmpneq_ps(other1.values1, other2.values1)) & 3));
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
 * Compare a SIMD6 to a value.
 * @param other1 The SIMD6.
 * @param other2 The value to compare to the SIMD6.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator==(const SIMD6<T, Width>& other1, typename SIMD6<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NEQ_UQ);
        return ((_mm256_movemask_ps(val) & 0x3F) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 val0 = _mm_cmp_ps(other1.values0, other2.values, _CMP_NEQ_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values, _CMP_NEQ_UQ);
            val1 = _mm_shuffle1010_ps(val1);
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values0, other2.values)) |
                (_mm_movemask_ps(_mm_cmpneq_ps(other1.values1, other2.values)) & 3));
        }
    } else
#endif
    {
        return ((other1.values0 == other2.value) & (other1.values1 == other2.value) & (other1.values2 == other2.value) &
            (other1.values3 == other2.value) & (other1.values4 == other2.value) & (other1.values5 == other2.value));
    }
}

/**
 * Compare two SIMD6s are less or equal.
 * @param other1 The first SIMD6.
 * @param other2 The second SIMD6 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<=(const SIMD6<T, Width>& other1, const SIMD6<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ);
        return ((_mm256_movemask_ps(val) & 0x3F) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 val0 = _mm_cmp_ps(other1.values0, other2.values0, _CMP_NLE_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values1, _CMP_NLE_UQ);
            val1 = _mm_shuffle1010_ps(val1);
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values0, other2.values0)) |
                (_mm_movemask_ps(_mm_cmpnle_ps(other1.values1, other2.values1)) & 3));
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
 * Compare SIMD6 is less than or equal to a value.
 * @param other1 The SIMD6.
 * @param other2 The value to compare to the SIMD6.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<=(const SIMD6<T, Width>& other1, typename SIMD6<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ);
        return ((_mm256_movemask_ps(val) & 0x3F) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 val0 = _mm_cmp_ps(other1.values0, other2.values, _CMP_NLE_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values, _CMP_NLE_UQ);
            val1 = _mm_shuffle1010_ps(val1);
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values0, other2.values)) |
                (_mm_movemask_ps(_mm_cmpnle_ps(other1.values1, other2.values)) & 3));
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.value) & (other1.values1 <= other2.value) & (other1.values2 <= other2.value) &
            (other1.values3 <= other2.value) & (other1.values4 <= other2.value) & (other1.values5 <= other2.value));
    }
}

/**
 * Compare two SIMD6s are less than.
 * @param other1 The first SIMD6.
 * @param other2 The second SIMD6 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<(const SIMD6<T, Width>& other1, const SIMD6<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ);
        return ((_mm256_movemask_ps(val) & 0x3F) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 val0 = _mm_cmp_ps(other1.values0, other2.values0, _CMP_NLT_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values1, _CMP_NLT_UQ);
            val1 = _mm_shuffle1010_ps(val1);
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values0, other2.values0)) |
                (_mm_movemask_ps(_mm_cmpnlt_ps(other1.values1, other2.values1)) & 3));
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
 * Compare SIMD6 is less than a value.
 * @param other1 The SIMD6.
 * @param other2 The value to compare to the SIMD6.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<(const SIMD6<T, Width>& other1, typename SIMD6<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ);
        return ((_mm256_movemask_ps(val) & 0x3F) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 val0 = _mm_cmp_ps(other1.values0, other2.values, _CMP_NLT_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values, _CMP_NLT_UQ);
            val1 = _mm_shuffle1010_ps(val1);
            return static_cast<bool>(_mm_testz_ps(val0, val0) & _mm_testz_ps(val1, val1));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values0, other2.values)) |
                (_mm_movemask_ps(_mm_cmpnlt_ps(other1.values1, other2.values)) & 3));
        }
    } else
#endif
    {
        return ((other1.values0 < other2.value) & (other1.values1 < other2.value) & (other1.values2 < other2.value) &
            (other1.values3 < other2.value) & (other1.values4 < other2.value) & (other1.values5 < other2.value));
    }
}

/**
 * Compare two SIMD6s are not equal.
 * @param other1 The first SIMD6.
 * @param other2 The second SIMD6 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator!=(const SIMD6<T, Width>& other1, const SIMD6<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_EQ_OQ);
        return ((_mm256_movemask_ps(val) & 0x3F) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 val0 = _mm_cmp_ps(other1.values0, other2.values0, _CMP_EQ_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values1, _CMP_EQ_UQ);
            val1 = _mm_shuffle1010_ps(val1);
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
 * Compare SIMD6 is not equal to a value.
 * @param other1 The SIMD6.
 * @param other2 The value to compare to the SIMD6.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator!=(const SIMD6<T, Width>& other1, typename SIMD6<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_cmp_ps(other1.values, other2.values, _CMP_EQ_OQ);
        return ((_mm256_movemask_ps(val) & 0x3F) == 0);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 val0 = _mm_cmp_ps(other1.values0, other2.values, _CMP_EQ_UQ);
            __m128 val1 = _mm_cmp_ps(other1.values1, other2.values, _CMP_EQ_UQ);
            val1 = _mm_shuffle1010_ps(val1);
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
 * And 2 SIMD6s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD6.
 * @param other2 SIMD6 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator&(const SIMD6<T, Width>& other1, const SIMD6<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD6<T, Width>(_mm256_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD6<T, Width>(_mm_and_ps(other1.values0, other2.values0), _mm_and_ps(other1.values1, other2.values1));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD6<T, Width>(other1.values0 & other2.values0, other1.values1 & other2.values1,
                other1.values2 & other2.values2, other1.values3 & other2.values3, other1.values4 & other2.values4,
                other1.values5 & other2.values5);
        } else {
            return SIMD6<T, Width>(Shift::bitAnd(other1.values0, other2.values0),
                Shift::bitAnd(other1.values1, other2.values1), Shift::bitAnd(other1.values2, other2.values2),
                Shift::bitAnd(other1.values3, other2.values3), Shift::bitAnd(other1.values4, other2.values4),
                Shift::bitAnd(other1.values5, other2.values5));
        }
    }
}

/**
 * And a SIMD6s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD6.
 * @param other2 Value to and with the SIMD6.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator&(const SIMD6<T, Width>& other1, typename SIMD6<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD6<T, Width>(_mm256_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD6<T, Width>(_mm_and_ps(other1.values0, other2.values), _mm_and_ps(other1.values1, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD6<T, Width>(other1.values0 & other2.value, other1.values1 & other2.value,
                other1.values2 & other2.value, other1.values3 & other2.value, other1.values4 & other2.value,
                other1.values5 & other2.value);
        } else {
            return SIMD6<T, Width>(Shift::bitAnd(other1.values0, other2.value),
                Shift::bitAnd(other1.values1, other2.value), Shift::bitAnd(other1.values2, other2.value),
                Shift::bitAnd(other1.values3, other2.value), Shift::bitAnd(other1.values4, other2.value),
                Shift::bitAnd(other1.values5, other2.value));
        }
    }
}

/**
 * Or 2 SIMD6s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD6.
 * @param other2 SIMD6 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator|(const SIMD6<T, Width>& other1, const SIMD6<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD6<T, Width>(_mm256_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD6<T, Width>(_mm_or_ps(other1.values0, other2.values0), _mm_or_ps(other1.values1, other2.values1));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD6<T, Width>(other1.values0 | other2.values0, other1.values1 | other2.values1,
                other1.values2 | other2.values2, other1.values3 | other2.values3, other1.values4 | other2.values4,
                other1.values5 | other2.values5);
        } else {
            return SIMD6<T, Width>(Shift::bitOr(other1.values0, other2.values0),
                Shift::bitOr(other1.values1, other2.values1), Shift::bitOr(other1.values2, other2.values2),
                Shift::bitOr(other1.values3, other2.values3), Shift::bitOr(other1.values4, other2.values4),
                Shift::bitOr(other1.values5, other2.values5));
        }
    }
}

/**
 * Or a SIMD6s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD6.
 * @param other2 Value to and with the SIMD6.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator|(const SIMD6<T, Width>& other1, typename SIMD6<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD6<T, Width>(_mm256_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD6<T, Width>(_mm_or_ps(other1.values0, other2.values), _mm_or_ps(other1.values1, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD6<T, Width>(other1.values0 | other2.value, other1.values1 | other2.value,
                other1.values2 | other2.value, other1.values3 | other2.value, other1.values4 | other2.value,
                other1.values5 | other2.value);
        } else {
            return SIMD6<T, Width>(Shift::bitOr(other1.values0, other2.value),
                Shift::bitOr(other1.values1, other2.value), Shift::bitOr(other1.values2, other2.value),
                Shift::bitOr(other1.values3, other2.value), Shift::bitOr(other1.values4, other2.value),
                Shift::bitOr(other1.values5, other2.value));
        }
    }
}

/**
 * Xor 2 SIMD6s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD6.
 * @param other2 SIMD6 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator^(const SIMD6<T, Width>& other1, const SIMD6<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD6<T, Width>(_mm256_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD6<T, Width>(_mm_xor_ps(other1.values0, other2.values0), _mm_xor_ps(other1.values1, other2.values1));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD6<T, Width>(other1.values0 ^ other2.values0, other1.values1 ^ other2.values1,
                other1.values2 ^ other2.values2, other1.values3 ^ other2.values3, other1.values4 ^ other2.values4,
                other1.values5 ^ other2.values5);
        } else {
            return SIMD6<T, Width>(Shift::bitXor(other1.values0, other2.values0),
                Shift::bitXor(other1.values1, other2.values1), Shift::bitXor(other1.values2, other2.values2),
                Shift::bitXor(other1.values3, other2.values3), Shift::bitXor(other1.values4, other2.values4),
                Shift::bitXor(other1.values5, other2.values5));
        }
    }
}

/**
 * Xor a SIMD6s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD6.
 * @param other2 Value to and with the SIMD6.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator^(const SIMD6<T, Width>& other1, typename SIMD6<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMD6<T, Width>(_mm256_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD6<T, Width>(_mm_xor_ps(other1.values0, other2.values), _mm_xor_ps(other1.values1, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD6<T, Width>(other1.values0 ^ other2.value, other1.values1 ^ other2.value,
                other1.values2 ^ other2.value, other1.values3 ^ other2.value, other1.values4 ^ other2.value,
                other1.values5 ^ other2.value);
        } else {
            return SIMD6<T, Width>(Shift::bitXor(other1.values0, other2.value),
                Shift::bitXor(other1.values1, other2.value), Shift::bitXor(other1.values2, other2.value),
                Shift::bitXor(other1.values3, other2.value), Shift::bitXor(other1.values4, other2.value),
                Shift::bitXor(other1.values5, other2.value));
        }
    }
}

/**
 * Not a SIMD6.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other The SIMD6.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD6<T, Width> operator~(const SIMD6<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_undefined_ps();
        return SIMD6<T, Width>(_mm256_xor_ps(other.values, _mm256_cmp_ps(val, val, _CMP_EQ_UQ)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        __m128 val = _mm_undefined_ps();
        val = _mm_cmpeq_ps(val, val);
        return SIMD6<T, Width>(_mm_xor_ps(other.values0, val), _mm_xor_ps(other.values1, val));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD6<T, Width>(
                ~other.values0, ~other.values1, ~other.values2, ~other.values3, ~other.values4, ~other.values5);
        } else {
            return SIMD6<T, Width>(Shift::bitNot(other.values0), Shift::bitNot(other.values1),
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
XS_FUNCTION typename SIMD6<T, Width>::Mask operator&(
    const typename SIMD6<T, Width>::Mask& mask1, const typename SIMD6<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD6<T, Width>::Mask(_kand_mask8(mask1.values, mask2.values));
        } else {
            return SIMD6<T, Width>::Mask(_mm256_and_ps(mask1.values, mask2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD6<T, Width>::Mask(
                _kand_mask8(mask1.values0, mask2.values0), _kand_mask8(mask1.values1, mask2.values1));
        } else {
            return SIMD6<T, Width>::Mask(
                _mm_and_ps(mask1.values0, mask2.values0), _mm_and_ps(mask1.values1, mask2.values1));
        }
    } else
#endif
    {
        return SIMD6<T, Width>::Mask(mask1.values0 & mask2.values0, mask1.values1 & mask2.values1,
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
XS_FUNCTION typename SIMD6<T, Width>::Mask operator|(
    const typename SIMD6<T, Width>::Mask& mask1, const typename SIMD6<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD6<T, Width>::Mask(_kor_mask8(mask1.values, mask2.values));
        } else {
            return SIMD6<T, Width>::Mask(_mm256_or_ps(mask1.values, mask2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD6<T, Width>::Mask(
                _kor_mask8(mask1.values0, mask2.values0), _kor_mask8(mask1.values1, mask2.values1));
        } else {
            return SIMD6<T, Width>::Mask(
                _mm_or_ps(mask1.values0, mask2.values0), _mm_or_ps(mask1.values1, mask2.values1));
        }
    } else
#endif
    {
        return SIMD6<T, Width>::Mask(mask1.values0 | mask2.values0, mask1.values1 | mask2.values1,
            mask1.values2 | mask2.values2, mask1.values3 | mask2.values3, mask1.values4 | mask2.values4,
            mask1.values5 | mask2.values5);
    }
}

/**
 * Xor 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to xor with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD6<T, Width>::Mask operator^(
    const typename SIMD6<T, Width>::Mask& mask1, const typename SIMD6<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD6<T, Width>::Mask(_kxor_mask8(mask1.values, mask2.values));
        } else {
            return SIMD6<T, Width>::Mask(_mm256_xor_ps(mask1.values, mask2.values));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD6<T, Width>::Mask(
                _kxor_mask8(mask1.values0, mask2.values0), _kxor_mask8(mask1.values1, mask2.values1));
        } else {
            return SIMD6<T, Width>::Mask(
                _mm_xor_ps(mask1.values0, mask2.values0), _mm_xor_ps(mask1.values1, mask2.values1));
        }
    } else
#endif
    {
        return SIMD6<T, Width>::Mask(mask1.values0 ^ mask2.values0, mask1.values1 ^ mask2.values1,
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
XS_FUNCTION typename SIMD6<T, Width>::Mask operator~(const typename SIMD6<T, Width>::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD6<T, Width>::Mask(_knot_mask8(mask.values));
        } else {
            return SIMD6<T, Width>::Mask(
                _mm256_xor_ps(mask.values, _mm256_cmp_ps(mask.values, mask.values, _CMP_EQ_OQ)));
        }
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD6<T, Width>::Mask(_knot_mask8(mask.values0), _knot_mask8(mask.values1));
        } else {
            return SIMD6<T, Width>::Mask(_mm_xor_ps(mask.values0, _mm_cmpeq_ps(mask.values0, mask.values0)),
                _mm_xor_ps(mask.values1, _mm_cmpeq_ps(mask.values1, mask.values1)));
        }
    } else
#endif
    {
        return SIMD6<T, Width>::Mask(
            ~mask.values0, ~mask.values1, ~mask.values2, ~mask.values3, ~mask.values4, ~mask.values5);
    }
}
} // namespace Shift
