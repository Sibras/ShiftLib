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

#include "SIMD/XSSIMD2.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class SIMD3;

template<typename T>
class SIMD3Data
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    T value0, value1, value2;

    /** Default constructor. */
    XS_FUNCTION SIMD3Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMD3Data(const SIMD3<T, Width>& other) noexcept
    {
        store(other);
    }

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param other0 The first pre-calculated value.
     * @param other1 The second pre-calculated value.
     * @param other2 The third pre-calculated value.
     */
    XS_FUNCTION void setData(T other0, T other1, T other2) noexcept
    {
        value0 = other0;
        value1 = other1;
        value2 = other2;
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const SIMD3<T, Width>& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                _mm_mask_storeu_ps(&value0, _cvtu32_mask8(0x7), other.values);
            } else {
                // Need to backup the 4th element to prevent memory corruption by using the 4 element store
                const __m128 back = _mm_load_ss(&this[1].value0);
                _mm_storeu_ps(&value0, other.values);
                _mm_store_ss(&this[1].value0, back);
            }
        } else
#endif
        {
            value0 = other.values0;
            value1 = other.values1;
            value2 = other.values2;
        }
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION SIMD3<T, Width> load() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3<T, Width>(_mm_loadu_ps(&value0));
        } else
#endif
        {
            return SIMD3<T, Width>(value0, value1, value2);
        }
    }
};

template<typename T>
class alignas(maxAlignment<T, 3>) SIMD3DataPad
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    T value0, value1, value2;
    T pad;

    /** Default constructor. */
    XS_FUNCTION SIMD3DataPad() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMD3DataPad(const SIMD3<T, Width>& other) noexcept
    {
        store(other);
    }

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param other0 The first pre-calculated value.
     * @param other1 The second pre-calculated value.
     * @param other2 The third pre-calculated value.
     */
    XS_FUNCTION void setData(T other0, T other1, T other2) noexcept
    {
        value0 = other0;
        value1 = other1;
        value2 = other2;
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const SIMD3<T, Width>& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            _mm_stream_ps(&value0, other.values);
        } else
#endif
        {
            value0 = other.values0;
            value1 = other.values1;
            value2 = other.values2;
        }
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION SIMD3<T, Width> load() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3<T, Width>(_mm_load_ps(&value0));
        } else
#endif
        {
            return SIMD3<T, Width>(value0, value1, value2);
        }
    }
};

template<typename T, SIMDWidth Width = widthSIMD<T>>
class SIMD3 : public NoExport::SIMDData<T, 3, 0, Width>
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    using Type = T;
    using InternalData = NoExport::SIMDData<T, 3, 0, Width>;
    using Data = SIMD3Data<T>;
    using DataPad = SIMD3DataPad<T>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = InternalData::width;
    static constexpr uint32 numValues = 3;
    static constexpr uint32 size = InternalData::size;
    using BaseDef = SIMDBase<T, SIMDBase<T, widthImpl>::widthImpl>;
    using InBaseDef = SIMDInBase<T, SIMDInBase<T, widthImpl>::widthImpl>;
    using SIMD2Def = SIMD2<T, SIMD2<T, widthImpl>::widthImpl>;
    using InternalData::SIMDData;

    /** Tri Mask object used to store 3 different masks at once. */
    class Mask : public NoExport::SIMDMaskData<T, 3, 0, Width>
    {
    public:
        using Type = T;
        using InternalData = NoExport::SIMDMaskData<T, 3, 0, Width>;
        static constexpr SIMDWidth width = Width;
        static constexpr SIMDWidth widthImpl = InternalData::width;
        using InternalData::SIMDMaskData;
        using Bool = Bool3<hasSIMD<T> && (Width > SIMDWidth::Scalar)>;

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
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    this->values = _cvtu32_mask8(mask);
                } else {
                    __m128i values = _mm_shuffle0000_epi32(_mm_cvtsi32_si128(mask));
                    const __m128i bitMask = _mm_set_epi32(0x40, 0x3, 0x2, 0x1);
                    values = _mm_and_si128(values, bitMask);
                    values = _mm_cmpeq_epi32(values, bitMask);
                    this->values = _mm_castsi128_ps(values);
                }
            } else
#endif
            {
                this->values0 = mask & 0x1;
                this->values1 = mask & 0x2;
                this->values2 = mask & 0x4;
            }
        }

        /**
         * Construct a mask from individual booleans.
         * @param bool0 The first boolean value.
         * @param bool1 The second boolean value.
         * @param bool2 The third boolean value.
         */
        XS_FUNCTION Mask(bool bool0, bool bool1, bool bool2) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    this->values = _cvtu32_mask8((static_cast<uint32>(bool2) << 2) | (static_cast<uint32>(bool1) << 1) |
                        static_cast<uint32>(bool0));
                } else {
                    this->values = _mm_cmpneq_ps(_mm_set_ps(0, bool2, bool1, bool0), _mm_setzero_ps());
                }
            } else
#endif
            {
                this->values0 = bool0;
                this->values1 = bool1;
                this->values2 = bool2;
            }
        }

        /**
         * Get the boolean equivalent of the mask.
         * @returns The bool.
         */
        XS_FUNCTION Bool getBool3() const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return Bool3<true>(static_cast<uint8>(_cvtmask8_u32(this->values)));
                } else {
                    return Bool3<true>(static_cast<uint8>(_mm_movemask_ps(this->values)));
                }
            } else
#endif
            {
                return Bool3<false>(this->values0, this->values1, this->values2);
            }
        }

        /**
         * Get boolean signalling if any element of mask is set.
         * @returns Boolean.
         */
        XS_FUNCTION bool getAny() const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return static_cast<bool>(_cvtmask8_u32(this->values) & 0x7);
                } else {
                    return static_cast<bool>(_mm_movemask_ps(this->values) & 0x7);
                }
            } else
#endif
            {
                return this->values0 | this->values1 | this->values2;
            }
        }

        /**
         * Get boolean signalling if all elements of mask is set.
         * @returns Boolean.
         */
        XS_FUNCTION bool getAll() const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return ((_cvtmask8_u32(this->values) & 0x7) == 0x7);
                } else {
                    return ((_mm_movemask_ps(this->values) & 0x7) == 0x7);
                }
            } else
#endif
            {
                return (this->values0 & this->values1 & this->values2);
            }
        }

        /**
         * Get boolean signalling if no elements of mask is set.
         * @returns Boolean.
         */
        XS_FUNCTION bool getNone() const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return ((_cvtmask8_u32(this->values) & 0x7) == 0x0);
                } else {
                    return ((_mm_movemask_ps(this->values) & 0x7) == 0x0);
                }
            } else
#endif
            {
                return !((this->values0 & this->values1 & this->values2));
            }
        }

    private:
#if XS_ISA == XS_X86
        class SIMDMasker3X86
        {
        public:
            XS_INLINE static SIMD3& ToType(SIMD3 other)
            {
                return other;
            }
        };
#endif

        template<uint32 Index>
        class SIMDMasker3
        {
        public:
            XS_INLINE static InBaseDef ToType(SIMD3 other)
            {
                return reinterpret_cast<InBaseDef*>(other.values)[Index];
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
        XS_FUNCTION void mask3Function(MaskOperator& maskFunc) const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                auto val = maskFunc.template expression<SIMD3, SIMDMasker3X86>();
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    val.values = _mm_mask_blend_ps(this->values, _mm_setzero_ps(), val.values);
                } else {
                    val.values = _mm_and_ps(val.values, this->values);
                }
                maskFunc.template finalExpression<SIMD3, SIMDMasker3X86>(val);
            } else
#endif
            {
                if (this->values0) {
                    auto val = maskFunc.template expression<InBaseDef, SIMDMasker3<0>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker3<0>>(val);
                }
                if (this->values1) {
                    auto val = maskFunc.template expression<InBaseDef, SIMDMasker3<1>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker3<1>>(val);
                }
                if (this->values2) {
                    auto val = maskFunc.template expression<InBaseDef, SIMDMasker3<2>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker3<2>>(val);
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
        XS_FUNCTION void mask3ElseFunction(MaskOperator& maskFunc) const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                auto value1 = maskFunc.template expression1<SIMD3, SIMDMasker3X86>();
                auto value2 = maskFunc.template expression2<SIMD3, SIMDMasker3X86>();
                const auto final(value1.blendVar(value2, *this));
                maskFunc.template finalExpression<SIMD3, SIMDMasker3X86>(final);
            } else
#endif
            {
                auto val = (this->values0) ? maskFunc.template expression1<InBaseDef, SIMDMasker3<0>>() :
                                             maskFunc.template expression2<InBaseDef, SIMDMasker3<0>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker3<0>>(val);
                val = (this->values1) ? maskFunc.template expression1<InBaseDef, SIMDMasker3<1>>() :
                                        maskFunc.template expression2<InBaseDef, SIMDMasker3<1>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker3<1>>(val);
                val = (this->values1) ? maskFunc.template expression1<InBaseDef, SIMDMasker3<2>>() :
                                        maskFunc.template expression2<InBaseDef, SIMDMasker3<2>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker3<2>>(val);
            }
        }
    };

    /** Tri shuffle class used to store shuffle information for SIMD3s. */
    class Shuffle : public NoExport::SIMDShuffleData<T, 3, 0, Width>
    {
    public:
        using Type = T;
        using InternalData = NoExport::SIMDShuffleData<T, 3, 0, Width>;
        static constexpr SIMDWidth width = Width;
        static constexpr SIMDWidth widthImpl = InternalData::width;
        using InternalData::SIMDShuffleData;

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
         * Construct a shuffle from its member variables.
         * @param shuff0 Input integer representation of the first shuffle value (must be between 0 and 2).
         * @param shuff1 Input integer representation of the second shuffle value (must be between 0 and 2).
         * @param shuff2 Input integer representation of the third shuffle value (must be between 0 and 2).
         */
        XS_FUNCTION Shuffle(uint32 shuff0, uint32 shuff1, uint32 shuff2) noexcept
        {
            XS_ASSERT(shuff0 < 3);
            XS_ASSERT(shuff1 < 3);
            XS_ASSERT(shuff2 < 3);
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    this->values = _mm_set_epi32(3, shuff2, shuff1, shuff0);
                } else {
                    this->values = _mm_set_epi32(3, (shuff2 * 0x04040404) + 0x03020100,
                        (shuff1 * 0x04040404) + 0x03020100, (shuff0 * 0x04040404) + 0x03020100);
                }
            } else
#endif
            {
                this->values0 = shuff0;
                this->values1 = shuff1;
                this->values2 = shuff2;
            }
        }

        /**
         * Construct a shuffle that broadcasts a single element to all positions.
         * @param shuffle Input element to broadcast (must be between 0 and 2).
         */
        XS_FUNCTION explicit Shuffle(uint32 shuffle) noexcept
        {
            XS_ASSERT(shuffle < 3);
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    this->values = _mm_set1_epi32(shuffle);
                } else {
                    this->values = _mm_set1_epi32((shuffle * 0x04040404) + 0x03020100);
                }
            } else
#endif
            {
                this->values0 = shuffle;
                this->values1 = shuffle;
                this->values2 = shuffle;
            }
        }

        /**
         * Constructor to build a Shuffle with default ordering (i.e. does not change shuffled item).
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle Default() noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    return Shuffle(_mm_set_epi32(3, 2, 1, 0));
                } else {
                    return Shuffle(_mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100));
                }
            } else
#endif
            {
                return Shuffle(0, 1, 2);
            }
        }

        /**
         * Constructor to build a Shuffle with reverse ordering.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle Reverse() noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    return Shuffle(_mm_set_epi32(3, 0, 1, 2));
                } else {
                    return Shuffle(_mm_set_epi32(0x0F0E0D0C, 0x03020100, 0x07060504, 0x0B0A0908));
                }
            } else
#endif
            {
                return Shuffle(2, 1, 0);
            }
        }

        /**
         * Constructor to build a Shuffle that rotates left a certain number of times.
         * @param rotate Number of times to rotate (must be between 0 and 2).
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle RotateLeft(uint32 rotate) noexcept
        {
            XS_ASSERT(rotate < 3);
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    __m128i val0 = _mm_set_epi32(3, 2, 1, 0);
                    const __m128i val1 = _mm_set1_epi32(rotate);
                    __m128i mask = _mm_shuffle3333_epi32(val0);
                    val0 = _mm_add_epi32(val0, val1);
                    mask = _mm_andnot_si128(_mm_cmplt_epi32(val0, mask), mask);
                    return Shuffle(_mm_sub_epi32(val0, mask));
                } else {
                    __m128i val0 = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
                    const __m128i val1 = _mm_set1_epi8(static_cast<int8_t>(rotate) * 4);
                    __m128i mask = _mm_set1_epi8(0xC);
                    val0 = _mm_add_epi8(val0, val1);
                    mask = _mm_andnot_si128(_mm_cmplt_epi8(val0, mask), mask);
                    return Shuffle(_mm_sub_epi8(val0, mask));
                }
            } else
#endif
            {
                switch (rotate) {
                    case 0:
                        return Shuffle(0, 1, 2);
                    case 1:
                        return Shuffle(1, 2, 0);
                    case 2:
                        return Shuffle(2, 0, 1);
                    default:
                        XS_UNREACHABLE;
                }
            }
        }

        /**
         * Constructor to build a Shuffle that rotates right a certain number of times.
         * @param rotate Number of times to rotate (must be between 0 and 2).
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle RotateRight(uint32 rotate) noexcept
        {
            XS_ASSERT(rotate < 3);
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    __m128i val0 = _mm_set_epi32(3, 2, 1, 0);
                    const __m128i val1 = _mm_set1_epi32(rotate);
                    __m128i mask = _mm_shuffle3333_epi32(val0);
                    val0 = _mm_sub_epi32(val0, val1);
                    mask = _mm_and_si128(mask, _mm_cmplt_epi32(val0, _mm_setzero_si128()));
                    return Shuffle(_mm_add_epi32(val0, mask));
                } else {
                    __m128i val0 = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
                    const __m128i val1 = _mm_set1_epi8(static_cast<int8_t>(rotate) * 4);
                    __m128i mask = _mm_set1_epi8(12);
                    val0 = _mm_sub_epi8(val0, val1);
                    mask = _mm_and_si128(mask, _mm_cmplt_epi8(val0, _mm_setzero_si128()));
                    return Shuffle(_mm_add_epi8(val0, mask));
                }
            } else
#endif
            {
                const uint32 u = (4 >> rotate) & 3;
                const uint32 w = 2 - rotate;
                const uint32 v = 3 - u - w;
                return Shuffle(u, v, w);
            }
        }

        /**
         * Constructor to build a Shuffle that extracts a specific element and moves it to the last position.
         * @param extract Index of the item to move to end (must be between 0 and 2).
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_FUNCTION static Shuffle Extract(uint32 extract) noexcept
        {
            XS_ASSERT(extract < 3);
            const uint32 u = (2 - extract) >> 1;
            const uint32 v = 3 - u - extract;
            return Shuffle(u, v, extract);
        }

        /**
         * Shuffles the elements of the shuffle.
         * @tparam Index0 The index of the first element to insert into the returned shuffle (range is 0-2).
         * @tparam Index1 The index of the second element to insert into the returned shuffle (range is 0-2).
         * @tparam Index2 The index of the third element to insert into the returned shuffle (range is 0-2).
         * @returns The result of the operation.
         */
        template<uint32 Index0, uint32 Index1, uint32 Index2>
        XS_FUNCTION Shuffle shuffle() const noexcept
        {
            static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3, "Invalid Index: Indexes must be <3");
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
                    return *this;
                } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 0) {
                    return Shuffle(_mm_shuffle0000_epi32(this->values));
                } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
                    return Shuffle(_mm_shuffle3200_epi32(this->values));
                } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 2) {
                    return Shuffle(_mm_shuffle3201_epi32(this->values));
                } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 2) {
                    return Shuffle(_mm_shuffle3211_epi32(this->values));
                } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
                    return Shuffle(_mm_shuffle2210_epi32(this->values));
                } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
                    return Shuffle(_mm_shuffle1100_epi32(this->values));
                } else {
                    return Shuffle(_mm_shuffle_epi32(this->values, _MM_SHUFFLE(3, Index2, Index1, Index0)));
                }
            } else
#endif
            {
                return Shuffle((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2]);
            }
        }
    };

    /** Default constructor. */
    XS_FUNCTION SIMD3() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION SIMD3(const SIMD3& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION SIMD3(SIMD3&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD3& operator=(const SIMD3& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD3& operator=(SIMD3&& other) noexcept = default;

    /**
     * Construct from 4 different values.
     * @param value0 The first value.
     * @param value1 The second value.
     * @param value2 The third value.
     */
    XS_FUNCTION SIMD3(T value0, T value1, T value2) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values = _mm_set_ps(0, value2, value1, value0);
        } else
#endif
        {
            this->values0 = value0;
            this->values1 = value1;
            this->values2 = value2;
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param val Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD3(T val) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values = _mm_set1_ps(val);
        } else
#endif
        {
            this->values0 = val;
            this->values1 = val;
            this->values2 = val;
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD3(BaseDef other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values = other.values;
        } else
#endif
        {
            this->values0 = other.value;
            this->values1 = other.value;
            this->values2 = other.value;
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD3(InBaseDef other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values = _mm_shuffle0000_ps(other.values);
        } else
#endif
        {
            this->values0 = other.value;
            this->values1 = other.value;
            this->values2 = other.value;
        }
    }

    /**
     * Construct from a SIMD2 and a single scalar value.
     * @param other0 The input SIMD2.
     * @param other1 The third value.
     */
    XS_FUNCTION SIMD3(const SIMD2Def& other0, InBaseDef other1) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values = _mm_movelh_ps(other0.values, other1.values); //(x,0,1,0)
        } else
#endif
        {
            this->values0 = other0.values0;
            this->values1 = other0.values1;
            this->values2 = other1.value;
        }
    }

    /**
     * Construct by inserting an element into a SIMD2.
     * @param other0 The input SIMD2.
     * @param other1 The new value to insert.
     * @param index  The position to insert the new value (must be between 0 and 2).
     */
    XS_FUNCTION SIMD3(const SIMD2Def& other0, InBaseDef other1, uint32 index) noexcept
    {
        XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if (index == 0) {
                const __m128 val = _mm_movelh_ps(other0.values, other1.values);
                this->values = _mm_shuffle1_ps(val, _MM_SHUFFLE(3, 1, 0, 2));
            } else if (index == 1) {
                this->values = _mm_unpacklo_ps(other0.values, other1.values); //(x,1,0,0)
            } else if (index == 2) {
                this->values = _mm_movelh_ps(other0.values, other1.values); //(x,0,1,0)
            }
        } else
#endif
        {
            if (index == 0) {
                this->values0 = other1.value;
                this->values1 = other0.values0;
                this->values2 = other0.values1;
            } else if (index == 1) {
                this->values0 = other0.values0;
                this->values1 = other1.value;
                this->values2 = other0.values1;
            } else if (index == 2) {
                this->values0 = other0.values0;
                this->values1 = other0.values1;
                this->values2 = other1.value;
            }
        }
    }

    /**
     * Constructor to build set to 0.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD3 Zero() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3(_mm_setzero_ps());
        } else
#endif
        {
            return SIMD3(T{0});
        }
    }

    /**
     * Constructor to build set to 1.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD3 One() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3(_mm_set1_ps(1.0f));
        } else
#endif
        {
            return SIMD3(T{1});
        }
    }

    /**
     * Transpose 3 SIMD3s.
     * @note Returns results in the 3 otherT parameters. The transpose operation results in the first return
     * containing the first element of each of the 3 inputs. The second return contains all the second elements from
     * the inputs and so forth.
     * @param       other0  The first SIMD3 to transpose.
     * @param       other1  The second SIMD3 to transpose.
     * @param       other2  The third SIMD3 to transpose.
     * @param [out] otherT0 The first SIMD3 to return the transposed results in.
     * @param [out] otherT1 The second SIMD3 to return the transposed results in.
     * @param [out] otherT2 The third SIMD3 to return the transposed results in.
     */
    XS_FUNCTION static void Transpose(const SIMD3& other0, const SIMD3& other1, const SIMD3& other2, SIMD3& otherT0,
        SIMD3& otherT1, SIMD3& otherT2) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val1 = _mm_unpacklo_ps(other0.values, other1.values);
            const __m128 val2 = _mm_shuffle1_ps(other2.values, _MM_SHUFFLE(3, 1, 3, 0)); //(0,x,1,x))
            const __m128 val3 = _mm_unpackhi_ps(other0.values, other1.values);

            otherT0.values = _mm_movelh_ps(val1, val2);
            otherT1.values = _mm_movehl_ps(val2, val1);
            otherT2.values = _mm_blend_ps(val3, other2.values, _MM_BLEND(1, 1, 0, 0));
        } else
#endif
        {
            otherT0.values0 = other0.values0;
            otherT0.values1 = other1.values0;
            otherT0.values2 = other2.values0;
            otherT1.values0 = other0.values1;
            otherT1.values1 = other1.values1;
            otherT1.values2 = other2.values1;
            otherT2.values0 = other0.values2;
            otherT2.values1 = other1.values2;
            otherT2.values2 = other2.values2;
        }
    }

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-2).
     * @returns SIMDInBase containing the desired value.
     */
    template<uint32 Index>
    XS_FUNCTION InBaseDef getValueInBase() const noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                return InBaseDef(this->values);
            } else if constexpr (Index == 1) {
                return InBaseDef(_mm_shuffle3311_ps(this->values)); //(x,x,x,1)
            } else /*Index == 2*/ {
                return InBaseDef(_mm_movehl_ps(this->values, this->values));
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
     * @tparam Index The index of the element to retrieve (range is 0-2).
     * @returns BaseDef containing the desired value.
     */
    template<uint32 Index>
    XS_FUNCTION BaseDef getValue() const noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                return BaseDef(_mm_shuffle0000_ps(this->values));
            } else if constexpr (Index == 1) {
                return BaseDef(_mm_shuffle1111_ps(this->values));
            } else /*Index == 2*/ {
                return BaseDef(_mm_shuffle2222_ps(this->values));
            }
        } else
#endif
        {
            return BaseDef((&this->values0)[Index]);
        }
    }

    /**
     * Dynamically get an element of the object.
     * @note Optimised for getting value from register.
     * @param index The index of the element to retrieve (range is 0-2).
     * @returns SIMDInBase containing the desired value.
     */
    XS_FUNCTION InBaseDef getValueInBase(uint32 index) const noexcept
    {
        XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return InBaseDef(_mm_permutevar_ps(this->values, _mm_cvtsi32_si128(index)));
            } else {
                __m128i val = _mm_shuffle_epi8(_mm_cvtsi32_si128(index * 4), _mm_setzero_si128());
                val = _mm_add_epi8(val, _mm_set1_epi32(0x03020100)); // equivalent to (3,2,1,0)
                return InBaseDef(_mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(this->values), val)));
            }
        } else
#endif
        {
            return InBaseDef((&this->values0)[index]);
        }
    }

    /**
     * Dynamically get an element of the object.
     * @note Optimised for getting value from register.
     * @param index The index of the element to retrieve (range is 0-2).
     * @returns BaseDef containing the desired value.
     */
    XS_FUNCTION BaseDef getValue(uint32 index) const noexcept
    {
        XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return BaseDef(_mm_permutevar_ps(this->values, _mm_shuffle0000_epi32(_mm_cvtsi32_si128(index))));
            } else {
                __m128i val = _mm_shuffle_epi8(_mm_cvtsi32_si128(index * 4), _mm_setzero_si128());
                val = _mm_add_epi8(val, _mm_set1_epi32(0x03020100));
                return BaseDef(_mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(this->values), val)));
            }
        } else
#endif
        {
            return BaseDef((&this->values0)[index]);
        }
    }

    /**
     * Get any 2 elements of a SIMD3 as a SIMD2.
     * @note Optimised for getting value from register.
     * @tparam Index0 The index of the first element to retrieve (range is 0-2).
     * @tparam Index1 The index of the second element to retrieve (range is 0-2).
     * @returns SIMD2 containing the desired values.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD2Def getValue2() const noexcept
    {
        static_assert(Index0 < 3 && Index1 < 3, "Invalid Index: Indexes must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD2Def(_mm_shuffle2200_ps(this->values));
            } else if constexpr (Index0 == 0 && Index1 == 1) {
                return SIMD2Def(this->values);
            } else if constexpr (Index0 == 2 && Index1 == 2) {
                return SIMD2Def(_mm_shuffle3322_ps(this->values));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD2Def(_mm_shuffle3311_ps(this->values));
            } else {
                return SIMD2Def(_mm_shuffle1_ps(this->values, _MM_SHUFFLE(0, 0, Index1, Index0)));
            }
        } else
#endif
        {
            return SIMD2Def((&this->values0)[Index0], (&this->values0)[Index1]);
        }
    }

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-2).
     * @param other The new value.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue(InBaseDef other) noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                this->values = _mm_blend_ss(this->values, other.values);
            } else {
                this->values = _mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(0, Index, 0));
            }
        } else
#endif
        {
            (&this->values0)[Index] = other.value;
        }
    }

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-2).
     * @param other The new value.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue(BaseDef other) noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                this->values = _mm_blend_ss(this->values, other.values);
            } else {
                this->values = _mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(0, Index, 0));
            }
        } else
#endif
        {
            (&this->values0)[Index] = other.value;
        }
    }

    /**
     * Dynamically set an element of the object.
     * @param index The index of the element to retrieve (range is 0-2).
     * @param other The new value.
     */
    XS_FUNCTION void setValue(uint32 index, BaseDef other) noexcept
    {
        XS_ASSERT(index < 3);
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                this->values = _mm_mask_blend_ps(_cvtu32_mask8(1UL << index), this->values, other.values);
            } else {
                const __m128 mask = _mm_castsi128_ps(_mm_cmpeq_epi32(_mm_set1_epi32(index), _mm_set_epi32(3, 2, 1, 0)));
                this->values = _mm_blendv_ps(this->values, other.values, mask);
            }
        } else
#endif
        {
            (&this->values0)[index] = other.value;
        }
    }

    /**
     * Add a value to an element of the object.
     * @tparam Index The index of the element to modify (range is 0-2).
     * @param other The value to add.
     */
    template<uint32 Index>
    XS_FUNCTION void addValue(InBaseDef other) noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                this->values = _mm_add_ss(this->values, other.values);
            } else if constexpr (Index == 1) {
                const __m128 value = _mm_shuffle2200_ps(other.values);
                this->values = _mm_blend_add_ps(this->values, 1 << Index, this->values, value);
            } else {
                const __m128 value = _mm_shuffle0000_ps(other.values);
                this->values = _mm_blend_add_ps(this->values, 1 << Index, this->values, value);
            }
        } else
#endif
        {
            (&this->values0)[Index] += other.value;
        }
    }

    /**
     * Subtract a value from an element of the object.
     * @tparam Index The index of the element to modify (range is 0-2).
     * @param other The value to subtract.
     */
    template<uint32 Index>
    XS_FUNCTION void subValue(InBaseDef other) noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                this->values = _mm_sub_ss(this->values, other.values);
            } else if constexpr (Index == 1) {
                const __m128 value = _mm_shuffle2200_ps(other.values);
                this->values = _mm_blend_sub_ps(this->values, 1 << Index, this->values, value);
            } else {
                const __m128 value = _mm_shuffle0000_ps(other.values);
                this->values = _mm_blend_sub_ps(this->values, 1 << Index, this->values, value);
            }
        } else
#endif
        {
            (&this->values0)[Index] -= other.value;
        }
    }

    /**
     * Multiply an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-2).
     * @param other The value to multiply by.
     */
    template<uint32 Index>
    XS_FUNCTION void mulValue(InBaseDef other) noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                this->values = _mm_mul_ss(this->values, other.values);
            } else if constexpr (Index == 1) {
                const __m128 value = _mm_shuffle2200_ps(other.values);
                this->values = _mm_blend_mul_ps(this->values, 1 << Index, this->values, value);
            } else {
                const __m128 value = _mm_shuffle0000_ps(other.values);
                this->values = _mm_blend_mul_ps(this->values, 1 << Index, this->values, value);
            }
        } else
#endif
        {
            (&this->values0)[Index] *= other.value;
        }
    }

    /**
     * Divide an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-2).
     * @param other The value to divide by.
     */
    template<uint32 Index>
    XS_FUNCTION void divValue(InBaseDef other) noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                this->values = _mm_div_ss(this->values, other.values);
            } else if constexpr (Index == 1) {
                const __m128 value = _mm_shuffle2200_ps(other.values);
                this->values = _mm_blend_div_ps(this->values, 1 << Index, this->values, value);
            } else {
                const __m128 value = _mm_shuffle0000_ps(other.values);
                this->values = _mm_blend_div_ps(this->values, 1 << Index, this->values, value);
            }
        } else
#endif
        {
            (&this->values0)[Index] /= other.value;
        }
    }

    /**
     * Multiply then add to an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-2).
     * @param other1 The value to multiply by.
     * @param other2 The value to add.
     */
    template<uint32 Index>
    XS_FUNCTION void madValue(InBaseDef other1, InBaseDef other2) noexcept
    {
        static_assert(Index < 3, "Invalid Index: Index must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                this->values = _mm_fmadd_ss(this->values, other1.values, other2.values);
            } else if constexpr (Index % 4 == 1) {
                const __m128 value1 = _mm_shuffle2200_ps(other1.values);
                const __m128 value2 = _mm_shuffle2200_ps(other2.values);
                this->values = _mm_blend_fmadd_ps(this->values, 1 << (Index % 4), value1, value2);
            } else {
                const __m128 value1 = _mm_shuffle0000_ps(other1.values);
                const __m128 value2 = _mm_shuffle0000_ps(other2.values);
                this->values = _mm_blend_fmadd_ps(this->values, 1 << Index, value1, value2);
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
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2>
    XS_FUNCTION SIMD3 negate() const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Elem0 && !Elem1 && !Elem2) {
                return SIMD3(_mm_xor_ps(this->values, _mm_set_ss(-0.0f)));
            } else if constexpr (Elem0 && Elem1 && Elem2) {
                return SIMD3(_mm_xor_ps(this->values, _mm_set1_ps(-0.0f)));
            } else {
                return SIMD3(_mm_xor_ps(
                    this->values, _mm_set_ps(0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)));
            }
        }
#endif
        else {
            return SIMD3(Elem0 ? -this->values0 : this->values0, Elem1 ? -this->values1 : this->values1,
                Elem2 ? -this->values2 : this->values2);
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
    XS_FUNCTION SIMD3 mad(const SIMD3& other1, const SIMD3& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3(_mm_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD3(_mm_add_ps(_mm_mul_ps(this->values, other1.values), other2.values));
            }
        } else
#endif
        {
            return SIMD3(Shift::fma<T>(this->values0, other1.values0, other2.values0),
                Shift::fma<T>(this->values1, other1.values1, other2.values1),
                Shift::fma<T>(this->values2, other1.values2, other2.values2));
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
    XS_FUNCTION SIMD3 mad(BaseDef other1, const SIMD3& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3(_mm_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD3(_mm_add_ps(_mm_mul_ps(this->values, other1.values), other2.values));
            }
        } else
#endif
        {
            return SIMD3(Shift::fma<T>(this->values0, other1.value, other2.values0),
                Shift::fma<T>(this->values1, other1.value, other2.values1),
                Shift::fma<T>(this->values2, other1.value, other2.values2));
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
    XS_FUNCTION SIMD3 mad(const SIMD3& other1, BaseDef other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3(_mm_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD3(_mm_add_ps(_mm_mul_ps(this->values, other1.values), other2.values));
            }
        } else
#endif
        {
            return SIMD3(Shift::fma<T>(this->values0, other1.values0, other2.value),
                Shift::fma<T>(this->values1, other1.values1, other2.value),
                Shift::fma<T>(this->values2, other1.values2, other2.value));
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
    XS_FUNCTION SIMD3 msub(const SIMD3& other1, const SIMD3& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD3(_mm_fmsub_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD3(_mm_sub_ps(_mm_mul_ps(this->values, other1.values), other2.values));
            }
        } else
#endif
        {
            return SIMD3(Shift::fma<T>(this->values0, other1.values0, -other2.values0),
                Shift::fma<T>(this->values1, other1.values1, -other2.values1),
                Shift::fma<T>(this->values2, other1.values2, -other2.values2));
        }
    }

    /**
     * Alternately subtract and add 2 objects.
     * @param other The second object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3 subAdd(const SIMD3& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3(_mm_addsub_ps(this->values, other.values));
        } else
#endif
        {
            return SIMD3(this->values0 - other.values0, this->values1 + other.values1, this->values2 - other.values2);
        }
    }

    /**
     * Compare two objects are equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask equalMask(const SIMD3& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_EQ_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values, other.values, _CMP_EQ_OQ));
            } else {
                return Mask(_mm_cmpeq_ps(this->values, other.values));
            }
        } else
#endif
        {
            return Mask(this->values0 == other.values0, this->values1 == other.values1, this->values2 == other.values2);
        }
    }

    /**
     * Compare two objects are less or equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask lessOrEqualMask(const SIMD3& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_LE_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values, other.values, _CMP_LE_OQ));
            } else {
                return Mask(_mm_cmple_ps(this->values, other.values));
            }
        } else
#endif
        {
            return Mask(this->values0 <= other.values0, this->values1 <= other.values1, this->values2 <= other.values2);
        }
    }

    /**
     * Compare two objects are less than.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask lessThanMask(const SIMD3& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_LT_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values, other.values, _CMP_LT_OQ));
            } else {
                return Mask(_mm_cmplt_ps(this->values, other.values));
            }
        } else
#endif
        {
            return Mask(this->values0 < other.values0, this->values1 < other.values1, this->values2 < other.values2);
        }
    }

    /**
     * Compare two objects are not equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask notEqualMask(const SIMD3& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_NEQ_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values, other.values, _CMP_NEQ_OQ));
            } else {
                return Mask(_mm_cmpneq_ps(this->values, other.values));
            }
        } else
#endif
        {
            return Mask(this->values0 != other.values0, this->values1 != other.values1, this->values2 != other.values2);
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
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_EQ_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values, other.values, _CMP_EQ_OQ));
            } else {
                return Mask(_mm_cmpeq_ps(this->values, other.values));
            }
        } else
#endif
        {
            return Mask(this->values0 == other.value, this->values1 == other.value, this->values2 == other.value);
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
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_LE_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values, other.values, _CMP_LE_OQ));
            } else {
                return Mask(_mm_cmple_ps(this->values, other.values));
            }
        } else
#endif
        {
            return Mask(this->values0 <= other.value, this->values1 <= other.value, this->values2 <= other.value);
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
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_LT_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values, other.values, _CMP_LT_OQ));
            } else {
                return Mask(_mm_cmplt_ps(this->values, other.values));
            }
        } else
#endif
        {
            return Mask(this->values0 < other.value, this->values1 < other.value, this->values2 < other.value);
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
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_GE_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values, other.values, _CMP_GE_OQ));
            } else {
                return Mask(_mm_cmple_ps(other.values, this->values));
            }
        } else
#endif
        {
            return Mask(this->values0 >= other.value, this->values1 >= other.value, this->values2 >= other.value);
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
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_GT_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values, other.values, _CMP_GT_OQ));
            } else {
                return Mask(_mm_cmplt_ps(other.values, this->values));
            }
        } else
#endif
        {
            return Mask(this->values0 > other.value, this->values1 > other.value, this->values2 > other.value);
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
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return Mask(_mm_cmp_ps_mask(this->values, other.values, _CMP_NEQ_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return Mask(_mm_cmp_ps(this->values, other.values, _CMP_NEQ_OQ));
            } else {
                return Mask(_mm_cmpneq_ps(this->values, other.values));
            }
        } else
#endif
        {
            return Mask(this->values0 != other.value, this->values1 != other.value, this->values2 != other.value);
        }
    }

    /**
     * Negate this object based on the sign of another.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @param other The second object whose sign to check.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD3 sign(const SIMD3& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 mask = _mm_and_ps(_mm_set1_ps(-0.0f), other.values);
            return SIMD3(_mm_xor_ps(this->values, mask));
        } else
#endif
        {
            return SIMD3(Shift::sign<T>(this->values0, other.values0), Shift::sign<T>(this->values1, other.values1),
                Shift::sign<T>(this->values2, other.values2));
        }
    }

    /**
     * Negate this object based on the sign of another.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @param other The second object whose sign to check.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD3 sign(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 mask = _mm_and_ps(_mm_set1_ps(-0.0f), other.values);
            return SIMD3(_mm_xor_ps(this->values, mask));
        } else
#endif
        {
            return SIMD3(Shift::sign<T>(this->values0, other.value), Shift::sign<T>(this->values1, other.value),
                Shift::sign<T>(this->values2, other.value));
        }
    }

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD3 abs() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3(_mm_andnot_ps(_mm_set1_ps(-0.0f), this->values));
        } else
#endif
        {
            return SIMD3(Shift::abs<T>(this->values0), Shift::abs<T>(this->values1), Shift::abs<T>(this->values2));
        }
    }

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMD3 max(const SIMD3& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3(_mm_max_ps(this->values, other.values));
        } else
#endif
        {
            return SIMD3(Shift::max<T>(this->values0, other.values0), Shift::max<T>(this->values1, other.values1),
                Shift::max<T>(this->values2, other.values2));
        }
    }

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMD3 min(const SIMD3& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3(_mm_min_ps(this->values, other.values));
        } else
#endif
        {
            return SIMD3(Shift::min<T>(this->values0, other.values0), Shift::min<T>(this->values1, other.values1),
                Shift::min<T>(this->values2, other.values2));
        }
    }

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMD3 max(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3(_mm_max_ps(this->values, other.values));
        } else
#endif
        {
            return SIMD3(Shift::max<T>(this->values0, other.value), Shift::max<T>(this->values1, other.value),
                Shift::max<T>(this->values2, other.value));
        }
    }

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMD3 min(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD3(_mm_min_ps(this->values, other.values));
        } else
#endif
        {
            return SIMD3(Shift::min<T>(this->values0, other.value), Shift::min<T>(this->values1, other.value),
                Shift::min<T>(this->values2, other.value));
        }
    }

    /**
     * Maximum element of this object.
     * @returns SIMDBase set to the largest value.
     */
    XS_FUNCTION BaseDef hmax() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 other = _mm_max_ss(_mm_shuffle3232_ps(this->values), this->values); //(x,x,x,2)
            other = _mm_max_ss(other, _mm_shuffle3311_ps(this->values));               //(x,x,x,1)
            return BaseDef(_mm_shuffle0000_ps(other));
        } else
#endif
        {
            const T other = Shift::max<T>(this->values0, this->values1);
            return BaseDef(Shift::max<T>(this->values2, other));
        }
    }

    /**
     * Minimum element of this object.
     * @returns SIMDBase set to the smallest value.
     */
    XS_FUNCTION BaseDef hmin() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 other = _mm_min_ss(_mm_shuffle3232_ps(this->values), this->values); //(x,x,x,2)
            other = _mm_min_ss(other, _mm_shuffle3311_ps(this->values));               //(x,x,x,1)
            return BaseDef(_mm_shuffle0000_ps(other));
        } else
#endif
        {
            const T other = Shift::min<T>(this->values0, this->values1);
            return BaseDef(Shift::min<T>(this->values2, other));
        }
    }

    /**
     * Maximum element of this object.
     * @returns SIMDInBase set to the largest value.
     */
    XS_FUNCTION InBaseDef hmaxInBase() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 other = _mm_max_ss(_mm_shuffle3232_ps(this->values), this->values); //(x,x,x,2)
            return InBaseDef(_mm_max_ss(other, _mm_shuffle3311_ps(this->values)));           //(x,x,x,1)
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
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 other = _mm_min_ss(_mm_shuffle3232_ps(this->values), this->values); //(x,x,x,2)
            return InBaseDef(_mm_min_ss(other, _mm_shuffle3311_ps(this->values)));           //(x,x,x,1)
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
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 other = _mm_max_ss(_mm_shuffle3232_ps(this->values), this->values); //(x,x,x,2)
            other = _mm_max_ss(other, _mm_shuffle3311_ps(this->values));               //(x,x,x,1)
            other = _mm_shuffle0000_ps(other);
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                index = _cvtmask8_u32(_mm_cmp_ps_mask(this->values, other, _CMP_EQ_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                index = _mm_movemask_ps(_mm_cmp_ps(this->values, other, _CMP_EQ_OQ));
            } else {
                index = _mm_movemask_ps(_mm_cmpeq_ps(this->values, other));
            }
            index = ctz(index);
            return BaseDef(other);
        } else
#endif
        {
            const T other = Shift::max<T>(this->values0, this->values1);
            index = (other == this->values0) ? 0 : 1;
            const T other2 = Shift::max<T>(this->values2, other);
            index = (other2 == other) ? index : 2;
            return BaseDef(other2);
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
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 other = _mm_min_ss(_mm_shuffle3232_ps(this->values), this->values); //(x,x,x,2)
            other = _mm_min_ss(other, _mm_shuffle3311_ps(this->values));               //(x,x,x,1)
            other = _mm_shuffle0000_ps(other);
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                index = _cvtmask8_u32(_mm_cmp_ps_mask(this->values, other, _CMP_EQ_OQ));
            } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                index = _mm_movemask_ps(_mm_cmp_ps(this->values, other, _CMP_EQ_OQ));
            } else {
                index = _mm_movemask_ps(_mm_cmpeq_ps(this->values, other));
            }
            index = ctz(index);
            return BaseDef(other);
        } else
#endif
        {
            const T other = Shift::min<T>(this->values0, this->values1);
            index = (other == this->values0) ? 0 : 1;
            const T other2 = Shift::min<T>(this->values2, other);
            index = (other2 == other) ? index : 2;
            return BaseDef(other2);
        }
    }

    /**
     * Compute the sum of all elements.
     * @returns BaseDef set to the sum of all elements.
     */
    XS_FUNCTION BaseDef hadd() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 other = _mm_add_ss(_mm_shuffle3232_ps(this->values), this->values); //(x,x,x,2)
            other = _mm_add_ss(_mm_shuffle3311_ps(this->values), other);               //(x,x,x,1)
            return BaseDef(_mm_shuffle0000_ps(other));
        } else
#endif
        {
            T res = (this->values0 + this->values1);
            res += this->values2;
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
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 other = _mm_add_ss(_mm_shuffle3232_ps(this->values), this->values); //(x,x,x,2)
            return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(this->values), other));           //(x,x,x,1)
        } else
#endif
        {
            return InBaseDef(hadd());
        }
    }

    /**
     * Compute the 3 dimensional dot product of two objects.
     * @param other The second object.
     * @returns The value of the dot product.
     */
    XS_FUNCTION BaseDef dot3(const SIMD3& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return BaseDef(_mm_dp_ps(this->values, other.values, 0x7F));
        } else
#endif
        {
            T res = (this->values0 * other.values0);
            const T res2 = (this->values1 * other.values1);
            const T res3 = (this->values2 * other.values2);
            res += res2;
            res += res3;
            return BaseDef(res);
        }
    }

    /**
     * Compute the 3 dimensional dot product of two objects.
     * @param other The second object.
     * @returns The value of the dot product stored in a SIMDInBase.
     */
    XS_FUNCTION InBaseDef dot3InBase(const SIMD3& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return InBaseDef(_mm_dp_ps(this->values, other.values, 0x71));
        } else
#endif
        {
            return InBaseDef(dot3(other));
        }
    }

    /**
     * Compute the 3 dimensional cross product of two objects.
     * @note Uses Right-handed coordinate system.
     * @param other The second object.
     * @returns The cross product of the inputs.
     */
    XS_FUNCTION SIMD3 cross3(const SIMD3& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 val1 = _mm_shuffle1_ps(other.values, _MM_SHUFFLE(3, 0, 2, 1));
            val1 = _mm_mul_ps(val1, this->values);
            __m128 val2 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(3, 0, 2, 1));
            val2 = _mm_mul_ps(val2, other.values);
            val1 = _mm_sub_ps(val1, val2);
            return SIMD3(_mm_shuffle1_ps(val1, _MM_SHUFFLE(3, 0, 2, 1)));
        } else
#endif
        {
            const T crossX = -(this->values2 * other.values1);
            const T crossY = -(this->values0 * other.values2);
            const T crossZ = -(this->values1 * other.values0);
            return SIMD3(Shift::fma<T>(this->values1, other.values2, crossX),
                Shift::fma<T>(this->values2, other.values0, crossY),
                Shift::fma<T>(this->values0, other.values1, crossZ));
        }
    }

    /**
     * Compute the square of the length.
     * @returns Squared length.
     */
    XS_FUNCTION BaseDef lengthSqr() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return BaseDef(_mm_dp_ps(this->values, this->values, 0x7F));
        } else
#endif
        {
            T res = (this->values0 * this->values0);
            const T res2 = (this->values1 * this->values1);
            const T res3 = (this->values2 * this->values2);
            res += res2;
            res += res3;
            return BaseDef(res);
        }
    }

    /**
     * Compute the length.
     * @returns The length.
     */
    XS_FUNCTION BaseDef length() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return BaseDef(_mm_sqrt_ps(_mm_dp_ps(this->values, this->values, 0x7F)));
        } else
#endif
        {
            T res = (this->values0 * this->values0);
            const T res2 = (this->values1 * this->values1);
            const T res3 = (this->values2 * this->values2);
            res += res2;
            res += res3;
            return BaseDef(Shift::sqrt<T>(res));
        }
    }

    /**
     * Compute the square of the length.
     * @returns Squared length stored in SIMDInBase.
     */
    XS_FUNCTION InBaseDef lengthSqrInBase() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return InBaseDef(_mm_dp_ps(this->values, this->values, 0x71));
        } else
#endif
        {
            T res = (this->values0 * this->values0);
            const T res2 = (this->values1 * this->values1);
            const T res3 = (this->values2 * this->values2);
            res += res2;
            res += res3;
            return InBaseDef(res);
        }
    }

    /**
     * Compute the length.
     * @returns The length stored in SIMDInBase.
     */
    XS_FUNCTION InBaseDef lengthInBase() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return InBaseDef(_mm_sqrt_ss(_mm_dp_ps(this->values, this->values, 0x71)));
        } else
#endif
        {
            T res = (this->values0 * this->values0);
            const T res2 = (this->values1 * this->values1);
            const T res3 = (this->values2 * this->values2);
            res += res2;
            res += res3;
            return InBaseDef(Shift::sqrt<T>(res));
        }
    }

    /**
     * Normalise each value.
     * @note The result is unpredictable when all elements are at or near zero.
     * @returns The normalised values.
     */
    XS_FUNCTION SIMD3 normalize() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_sqrt_ps(_mm_dp_ps(this->values, this->values, 0x7F));
            return SIMD3(_mm_div_ps(this->values, val));
        } else
#endif
        {
            return *this / length();
        }
    }

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index.
     * @tparam Index0 The index in the first object to insert the element (range is 0-2).
     * @tparam Index1 The index in the second object to copy element from (range is 0-2).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD3 insert(const SIMD3& other) const noexcept
    {
        static_assert(Index0 < 3 && Index1 < 3, "Invalid Index: Indexes must be <3");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD3(_mm_blend_ss(this->values, other.values));
            } else if constexpr (Index0 == Index1) {
                return SIMD3(_mm_blend_ps(this->values, other.values, 1UL << Index0));
            } else {
                return SIMD3(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)));
            }
        } else
#endif
        {
            return SIMD3(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
                Index0 != 1 ? this->values1 : (&other.values0)[Index1],
                Index0 != 2 ? this->values2 : (&other.values0)[Index1]);
        }
    }

    /**
     * Blends values from the second object into the first based on a mask.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @tparam Elem2 Whether to copy the third element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns Result of operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2>
    XS_FUNCTION SIMD3 blend(const SIMD3& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2) {
            return *this;
        } else if constexpr (Elem0 && Elem1 && Elem2) {
            return other;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Elem0 && !Elem1 && !Elem2) {
                return SIMD3(_mm_blend_ss(this->values, other.values));
            } else if constexpr (!Elem0 && Elem1 && Elem2) {
                return SIMD3(_mm_blend_ss(other.values, this->values));
            } else {
                return SIMD3(_mm_blend_ps(this->values, other.values, _MM_BLEND(0, Elem2, Elem1, Elem0)));
            }
        }
#endif
        else {
            return SIMD3(Elem0 ? other.values0 : this->values0, Elem1 ? other.values1 : this->values1,
                Elem2 ? other.values2 : this->values2);
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
    XS_FUNCTION SIMD3 blendVar(const SIMD3& other, const Mask& mask) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return SIMD3(_mm_mask_blend_ps(mask.values, this->values, other.values));
            } else {
                return SIMD3(_mm_blendv_ps(this->values, other.values, mask.values));
            }
        } else
#endif
        {
            return SIMD3((mask.values0) ? other.values0 : this->values0, (mask.values1) ? other.values1 : this->values1,
                (mask.values2) ? other.values2 : this->values2);
        }
    }

    /**
     * Swaps values between the second object and the first based on a mask.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to swap the first elements.
     * @tparam Elem1 Whether to swap the second elements.
     * @tparam Elem2 Whether to swap the third elements.
     * @param [in,out] other The object to swap elements with the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2>
    XS_FUNCTION SIMD3 blendSwap(SIMD3& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2) {
            return *this;
        } else if constexpr (Elem0 && Elem1 && Elem2) {
            const SIMD3 backup = other;
            other = *this;
            return backup;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Elem0 && !Elem1 && !Elem2) {
                const __m128 backup = other.values;
                other.values = _mm_blend_ss(other.values, this->values);
                return SIMD3(_mm_blend_ss(this->values, backup));
            } else if constexpr (!Elem0 && Elem1 && Elem2) {
                const __m128 backup = other.values;
                other.values = _mm_blend_ss(this->values, other.values);
                return SIMD3(_mm_blend_ss(backup, this->values));
            } else {
                const __m128 backup = other.values;
                other.values = _mm_blend_ps(other.values, this->values, _MM_BLEND(0, Elem2, Elem1, Elem0));
                return SIMD3(_mm_blend_ps(this->values, backup, _MM_BLEND(0, Elem2, Elem1, Elem0)));
            }
        }
#endif
        else {
            const SIMD3 backup = other;
            other.values0 = (Elem0) ? this->values0 : other.values0;
            other.values1 = (Elem1) ? this->values1 : other.values1;
            other.values2 = (Elem2) ? this->values2 : other.values2;
            return SIMD3((Elem0) ? backup.values0 : this->values0, (Elem1) ? backup.values1 : this->values1,
                (Elem2) ? backup.values2 : this->values2);
        }
    }

    /**
     * Swaps values between the second object and the first based on a dynamic mask.
     * @note A value form the second object is swapped with the first based on the corresponding input elements.
     * @param [in,out] other The object to swap elements with the first.
     * @param          mask The object mask used to define which elements to swap.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3 blendSwapVar(SIMD3& other, const Mask& mask) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                const __m128 backup = other.values;
                other.values = _mm_mask_blend_ps(mask.values, other.values, this->values);
                return SIMD3(_mm_mask_blend_ps(mask.values, this->values, backup));
            } else {
                const __m128 backup = other.values;
                other.values = _mm_blendv_ps(other.values, this->values, mask.values);
                return SIMD3(_mm_blendv_ps(this->values, backup, mask.values));
            }
        } else
#endif
        {
            const SIMD3 backup = other;
            other.values0 = (mask.values0) ? this->values0 : other.values0;
            other.values1 = (mask.values1) ? this->values1 : other.values1;
            other.values2 = (mask.values2) ? this->values2 : other.values2;
            return SIMD3((mask.values0) ? backup.values0 : this->values0,
                (mask.values1) ? backup.values1 : this->values1, (mask.values2) ? backup.values2 : this->values2);
        }
    }

    /**
     * Shuffles the elements of the object.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-2).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-2).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-2).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2>
    XS_FUNCTION SIMD3 shuffle() const noexcept
    {
        static_assert(Index0 < 3 && Index1 < 3 && Index2 < 3, "Invalid Index: Indexes must be <3");
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
                return SIMD3(_mm_shuffle1010_ps(this->values));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
                return SIMD3(_mm_shuffle1100_ps(this->values));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2) {
                return SIMD3(_mm_shuffle2200_ps(this->values));
            } else {
                return SIMD3(_mm_shuffle1_ps(this->values, _MM_SHUFFLE(0, Index2, Index1, Index0)));
            }
        }
#endif
        else {
            return SIMD3((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2]);
        }
    }

    /**
     * Dynamically shuffles the elements of the object.
     * @param shuffle The shuffle mask used to shuffle.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD3 shuffleVar(const Shuffle& shuffle) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX>) {
                XS_ASSERT((_mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values, _mm_set1_epi32(2))) & 0xFFF) == 0);
                return SIMD3(_mm_permutevar_ps(this->values, shuffle.values));
            } else {
                XS_ASSERT(
                    (_mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values, _mm_set1_epi32(0x0B0A0908))) & 0xFFF) == 0);
                return SIMD3(_mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(this->values), shuffle.values)));
            }
        } else
#endif
        {
            return SIMD3((&this->values0)[shuffle.values0], (&this->values0)[shuffle.values1],
                (&this->values0)[shuffle.values2]);
        }
    }

    /**
     * Shuffles the elements of 2 SIMD3s together to a form a new SIMD3.
     * @note Any index in range 0-2 is taken from first object. Any index in range 3-5 is taken from second object.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-5).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-5).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-5).
     * @param other Second input.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2>
    XS_FUNCTION SIMD3 combine(const SIMD3& other) const noexcept
    {
        static_assert(Index0 < 6 && Index1 < 6 && Index2 < 6, "Invalid Index: Indexes must be <6");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 3 && Index1 == 1 && Index2 == 2) {
                return SIMD3(_mm_blend_ss(this->values, other.values));
            } else if constexpr (Index0 == 0 && Index1 == 4 && Index2 == 5) {
                return SIMD3(_mm_blend_ss(other.values, this->values));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 3) {
                return SIMD3(_mm_movelh_ps(this->values, other.values));
            } else if constexpr (Index0 == 3 && Index1 == 4 && Index2 == 0) {
                return SIMD3(_mm_movelh_ps(other.values, this->values));
            } else if constexpr ((Index0 == 0 || Index0 == 3) && (Index1 == 1 || Index1 == 4) &&
                (Index2 == 2 || Index2 == 5)) {
                return SIMD3(
                    _mm_blend_ps(this->values, other.values, _MM_BLEND(0, Index2 >= 3, Index1 >= 3, Index0 >= 3)));
            } else if constexpr (Index0 == 0 && Index1 == 3 && Index2 == 1) {
                return SIMD3(_mm_unpacklo_ps(this->values, other.values));
            } else if constexpr (Index0 == 3 && Index1 == 0 && Index2 == 4) {
                return SIMD3(_mm_unpacklo_ps(other.values, this->values));
            } else if constexpr (Index0 < 3 && Index1 < 3 && Index2 < 3) {
                return this->shuffle<Index0, Index1, Index2>();
            } else if constexpr (Index0 >= 3 && Index1 >= 3 && Index2 >= 3) {
                return other.shuffle<Index0 - 3, Index1 - 3, Index2 - 3>();
            } else if constexpr (Index0 < 3 && Index1 < 3 && Index2 >= 3) {
                return SIMD3(_mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(0, Index2 - 3, Index1, Index0)));
            } else if constexpr (Index0 >= 3 && Index1 >= 3 && Index2 < 3) {
                return SIMD3(
                    _mm_shuffle_ps(other.values, this->values, _MM_SHUFFLE(0, Index2, Index1 - 3, Index0 - 3)));
            } else if constexpr (Index0 == 0 && Index1 == 1) {
                return SIMD3(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index2 - 3, 2, 0)));
            } else if constexpr (Index0 == 0 && Index2 == 2) {
                return SIMD3(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index1 - 3, 1, 0)));
            } else if constexpr (Index1 == 1 && Index2 == 2) {
                return SIMD3(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index0 - 3, 0, 0)));
            } else if constexpr (Index0 == 3 && Index1 == 4) {
                return SIMD3(_mm_insert_ps(other.values, this->values, _MM_MK_INSERTPS_NDX(Index2, 2, 0)));
            } else if constexpr (Index0 == 3 && Index2 == 5) {
                return SIMD3(_mm_insert_ps(other.values, this->values, _MM_MK_INSERTPS_NDX(Index1, 1, 0)));
            } else if constexpr (Index1 == 4 && Index2 == 5) {
                return SIMD3(_mm_insert_ps(other.values, this->values, _MM_MK_INSERTPS_NDX(Index0, 0, 0)));
            } else if constexpr (Index0 >= 3 && Index1 < 3 && Index2 >= 3) {
                if constexpr (Index1 == 1) {
                    return SIMD3(_mm_blend_ps(this->values,
                        (other.shuffle<Index0 - 3, (XS_SHUFF128_DONTCARE_1_02(Index0 - 3, Index2 - 3)) % 3,
                                  Index2 - 3>()
                                .values),
                        _MM_BLEND(0, 1, 0, 1)));
                } else {
                    return SIMD3(_mm_shuffle_ps(
                        _mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(3, Index0 - 3, 0, Index1)), other.values,
                        _MM_SHUFFLE(3, Index2 - 3, 0, 2)));
                }
            } else if constexpr (Index0 < 3 && Index1 >= 3 && Index2 >= 3) {
                if constexpr (Index0 == 0) {
                    return SIMD3(_mm_blend_ps(this->values,
                        (other.shuffle<XS_SHUFF128_DONTCARE_0_12(Index1 - 3, Index2 - 3), Index1 - 3, Index2 - 3>()
                                .values),
                        _MM_BLEND(0, 1, 1, 0)));
                } else {
                    return SIMD3(_mm_shuffle_ps(
                        _mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(3, Index1 - 3, 0, Index0)), other.values,
                        _MM_SHUFFLE(3, Index2 - 3, 2, 0)));
                }
            } else {
                // Just use specialisation for inverse arrangement
                return other.combine<Index0 + 3 - ((Index0 >= 3) * 6), Index1 + 3 - ((Index1 >= 3) * 6),
                    Index2 + 3 - ((Index2 >= 3) * 6)>(*this);
            }
        } else
#endif
        {
            return SIMD3(((Index0 < 3) ? &this->values0 : &other.values0)[Index0 % 3],
                ((Index1 < 3) ? &this->values0 : &other.values0)[Index1 % 3],
                ((Index2 < 3) ? &this->values0 : &other.values0)[Index2 % 3]);
        }
    }
};

/**
 * Add two SIMD3s.
 * @param other1 The first SIMD3.
 * @param other2 SIMD3 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator+(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_add_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.values0 + other2.values0), (other1.values1 + other2.values1), (other1.values2 + other2.values2));
    }
}

/**
 * Add a value to all elements of a SIMD3.
 * @param other1 The SIMD3.
 * @param other2 Value to add to the SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator+(const SIMD3<T, Width>& other1, typename SIMD3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_add_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.values0 + other2.value), (other1.values1 + other2.value), (other1.values2 + other2.value));
    }
}

/**
 * Subtract a SIMD3 from another SIMD3.
 * @param other1 The first SIMD3.
 * @param other2 SIMD3 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator-(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.values0 - other2.values0), (other1.values1 - other2.values1), (other1.values2 - other2.values2));
    }
}

/**
 * Subtract a value from all elements of a SIMD3.
 * @param other1 The SIMD3.
 * @param other2 Value to subtract from the SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator-(const SIMD3<T, Width>& other1, typename SIMD3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.values0 - other2.value), (other1.values1 - other2.value), (other1.values2 - other2.value));
    }
}

/**
 * Subtract all elements of a SIMD3 from a value.
 * @param other1 Value to subtract from the SIMD3.
 * @param other2 The SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator-(typename SIMD3<T, Width>::BaseDef other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.value - other2.values0), (other1.value - other2.values1), (other1.value - other2.values2));
    }
}

/**
 * Multiply two SIMD3s per element.
 * @param other1 The first SIMD3.
 * @param other2 The second SIMD3 to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator*(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_mul_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.values0 * other2.values0), (other1.values1 * other2.values1), (other1.values2 * other2.values2));
    }
}

/**
 * Multiply a SIMD3 by a scalar.
 * @param other1 The SIMD3.
 * @param other2 Value to multiply the SIMD3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator*(const SIMD3<T, Width>& other1, typename SIMD3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_mul_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.values0 * other2.value), (other1.values1 * other2.value), (other1.values2 * other2.value));
    }
}

/**
 * Divide two SIMD3s per element.
 * @param other1 The first SIMD3.
 * @param other2 The second SIMD3 to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator/(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.values0 / other2.values0), (other1.values1 / other2.values1), (other1.values2 / other2.values2));
    }
}

/**
 * Divide a SIMD3 by a scalar.
 * @param other1 The SIMD3.
 * @param other2 Value to divide the SIMD3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator/(const SIMD3<T, Width>& other1, typename SIMD3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.values0 / other2.value), (other1.values1 / other2.value), (other1.values2 / other2.value));
    }
}

/**
 * Divide a scalar by all elements of a SIMD3.
 * @param other1 Value to divide the SIMD3 by.
 * @param other2 The SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator/(typename SIMD3<T, Width>::BaseDef other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD3<T, Width>(
            (other1.value / other2.values0), (other1.value / other2.values1), (other1.value / other2.values2));
    }
}

/**
 * Negate all elements of a SIMD3.
 * @param other The SIMD3 to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator-(const SIMD3<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3<T, Width>(_mm_sub_ps(_mm_setzero_ps(), other.values));
    } else
#endif
    {
        return SIMD3<T, Width>(-other.values0, -other.values1, -other.values2);
    }
}

/**
 * Perform compound assignment and addition with a SIMD3.
 * @param [in,out] other1 The first SIMD3.
 * @param          other2 SIMD3 to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width>& operator+=(SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_add_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
        other1.values2 += other2.values2;
    }
    return other1;
}

/**
 * Perform compound assignment and addition with a SIMD3.
 * @param [in,out] other1 The SIMD3.
 * @param          other2 Value to add to the first SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width>& operator+=(SIMD3<T, Width>& other1, typename SIMD3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_add_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 += other2.value;
        other1.values1 += other2.value;
        other1.values2 += other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction by a SIMD3.
 * @param [in,out] other1 The first SIMD3.
 * @param          other2 SIMD3 to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width>& operator-=(SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_sub_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
        other1.values2 -= other2.values2;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction with a SIMD3.
 * @param [in,out] other1 The SIMD3.
 * @param          other2 Value to subtract from the first SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width>& operator-=(SIMD3<T, Width>& other1, typename SIMD3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_sub_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 -= other2.value;
        other1.values1 -= other2.value;
        other1.values2 -= other2.value;
    }
    return other1;
}

/**
 * Perform SIMD3 compound assignment and multiplication.
 * @param [in,out] other1 The first SIMD3.
 * @param          other2 SIMD3 to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width>& operator*=(SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_mul_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
        other1.values2 *= other2.values2;
    }
    return other1;
}

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] other1 The SIMD3.
 * @param          other2 Value to multiply the SIMD3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width>& operator*=(SIMD3<T, Width>& other1, typename SIMD3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_mul_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 *= other2.value;
        other1.values1 *= other2.value;
        other1.values2 *= other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and division.
 * @param [in,out] other1 The SIMD3.
 * @param          other2 Value to divide the SIMD3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width>& operator/=(SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_div_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
        other1.values2 /= other2.values2;
    }
    return other1;
}

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] other1 The SIMD3.
 * @param          other2 Value to divide the SIMD3 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width>& operator/=(SIMD3<T, Width>& other1, typename SIMD3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_div_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 /= other2.value;
        other1.values1 /= other2.value;
        other1.values2 /= other2.value;
    }
    return other1;
}

/**
 * Compare two SIMD3s are equal.
 * @param other1 The first SIMD3.
 * @param other2 The second SIMD3 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator==(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_NEQ_UQ);
            val = _mm_shuffle1_ps(val, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values, other2.values)) & 0x7);
        }
    } else
#endif
    {
        return ((other1.values0 == other2.values0) & (other1.values1 == other2.values1) &
            (other1.values2 == other2.values2));
    }
}

/**
 * Compare a SIMD3 to a value.
 * @param other1 The SIMD3.
 * @param other2 The value to compare to the SIMD3.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator==(const SIMD3<T, Width>& other1, typename SIMD3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_NEQ_UQ);
            val = _mm_shuffle1_ps(val, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values, other2.values)) & 0x7);
        }
    } else
#endif
    {
        return ((other1.values0 == other2.value) & (other1.values1 == other2.value) & (other1.values2 == other2.value));
    }
}

/**
 * Compare two SIMD3s are less or equal.
 * @param other1 The first SIMD3.
 * @param other2 The second SIMD3 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<=(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ);
            val = _mm_shuffle1_ps(val, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values, other2.values)) & 0x7);
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.values0) & (other1.values1 <= other2.values1) &
            (other1.values2 <= other2.values2));
    }
}

/**
 * Compare SIMD3 is less than or equal to a value.
 * @param other1 The SIMD3.
 * @param other2 The value to compare to the SIMD3.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<=(const SIMD3<T, Width>& other1, typename SIMD3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ);
            val = _mm_shuffle1_ps(val, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values, other2.values)) & 0x7);
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.value) & (other1.values1 <= other2.value) & (other1.values2 <= other2.value));
    }
}

/**
 * Compare two SIMD3s are less than.
 * @param other1 The first SIMD3.
 * @param other2 The second SIMD3 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ);
            val = _mm_shuffle1_ps(val, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values, other2.values)) & 0x7);
        }
    } else
#endif
    {
        return (
            (other1.values0 < other2.values0) & (other1.values1 < other2.values1) & (other1.values2 < other2.values2));
    }
}

/**
 * Compare SIMD3 is less than a value.
 * @param other1 The SIMD3.
 * @param other2 The value to compare to the SIMD3.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<(const SIMD3<T, Width>& other1, typename SIMD3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ);
            val = _mm_shuffle1_ps(val, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values, other2.values)) & 0x7);
        }
    } else
#endif
    {
        return ((other1.values0 < other2.value) & (other1.values1 < other2.value) & (other1.values2 < other2.value));
    }
}

/**
 * Compare two SIMD3s are not equal.
 * @param other1 The first SIMD3.
 * @param other2 The second SIMD3 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator!=(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_EQ_UQ);
            val = _mm_shuffle1_ps(val, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpeq_ps(other1.values, other2.values)) & 0x7);
        }
    } else
#endif
    {
        return ((other1.values0 != other2.values0) & (other1.values1 != other2.values1) &
            (other1.values2 != other2.values2));
    }
}

/**
 * Compare SIMD3 is not equal to a value.
 * @param other1 The SIMD3.
 * @param other2 The value to compare to the SIMD3.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator!=(const SIMD3<T, Width>& other1, typename SIMD3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_EQ_UQ);
            val = _mm_shuffle1_ps(val, _MM_SHUFFLE(0, 2, 1, 0));
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpeq_ps(other1.values, other2.values)) & 0x7);
        }
    } else
#endif
    {
        return ((other1.values0 != other2.value) & (other1.values1 != other2.value) & (other1.values2 != other2.value));
    }
}

/**
 * And 2 SIMD3s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD3.
 * @param other2 SIMD3 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator&(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_and_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(
                other1.values0 & other2.values0, other1.values1 & other2.values1, other1.values2 & other2.values2);
        } else {
            return SIMD3<T, Width>(Shift::bitAnd(other1.values0, other2.values0),
                Shift::bitAnd(other1.values1, other2.values1), Shift::bitAnd(other1.values2, other2.values2));
        }
    }
}

/**
 * And a SIMD3s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD3.
 * @param other2 Value to and with the SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator&(const SIMD3<T, Width>& other1, typename SIMD3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_and_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(
                other1.values0 & other2.value, other1.values1 & other2.value, other1.values2 & other2.value);
        } else {
            return SIMD3<T, Width>(Shift::bitAnd(other1.values0, other2.value),
                Shift::bitAnd(other1.values1, other2.value), Shift::bitAnd(other1.values2, other2.value));
        }
    }
}

/**
 * Or 2 SIMD3s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD3.
 * @param other2 SIMD3 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator|(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_or_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(
                other1.values0 | other2.values0, other1.values1 | other2.values1, other1.values2 | other2.values2);
        } else {
            return SIMD3<T, Width>(Shift::bitOr(other1.values0, other2.values0),
                Shift::bitOr(other1.values1, other2.values1), Shift::bitOr(other1.values2, other2.values2));
        }
    }
}

/**
 * Or a SIMD3s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD3.
 * @param other2 Value to and with the SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator|(const SIMD3<T, Width>& other1, typename SIMD3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_or_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(
                other1.values0 | other2.value, other1.values1 | other2.value, other1.values2 | other2.value);
        } else {
            return SIMD3<T, Width>(Shift::bitOr(other1.values0, other2.value),
                Shift::bitOr(other1.values1, other2.value), Shift::bitOr(other1.values2, other2.value));
        }
    }
}

/**
 * Xor 2 SIMD3s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD3.
 * @param other2 SIMD3 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator^(const SIMD3<T, Width>& other1, const SIMD3<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_xor_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(
                other1.values0 ^ other2.values0, other1.values1 ^ other2.values1, other1.values2 ^ other2.values2);
        } else {
            return SIMD3<T, Width>(Shift::bitXor(other1.values0, other2.values0),
                Shift::bitXor(other1.values1, other2.values1), Shift::bitXor(other1.values2, other2.values2));
        }
    }
}

/**
 * Xor a SIMD3s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD3.
 * @param other2 Value to and with the SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator^(const SIMD3<T, Width>& other1, typename SIMD3<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD3(_mm_xor_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(
                other1.values0 ^ other2.value, other1.values1 ^ other2.value, other1.values2 ^ other2.value);
        } else {
            return SIMD3<T, Width>(Shift::bitXor(other1.values0, other2.value),
                Shift::bitXor(other1.values1, other2.value), Shift::bitXor(other1.values2, other2.value));
        }
    }
}

/**
 * Not a SIMD3.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other The SIMD3.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD3<T, Width> operator~(const SIMD3<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_undefined_ps();
        return SIMD3(_mm_xor_ps(other.values, _mm_cmpeq_ps(val, val)));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD3<T, Width>(~other.values0, ~other.values1, ~other.values2);
        } else {
            return SIMD3<T, Width>(
                Shift::bitNot(other.values0), Shift::bitNot(other.values1), Shift::bitNot(other.values2));
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
XS_FUNCTION typename SIMD3<T, Width>::Mask operator&(
    const typename SIMD3<T, Width>::Mask& mask1, const typename SIMD3<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD3<T, Width>::Mask(_kand_mask8(mask1.values, mask2.values));
        } else {
            return SIMD3<T, Width>::Mask(_mm_and_ps(mask1.values, mask2.values));
        }
    } else
#endif
    {
        return SIMD3<T, Width>::Mask(
            mask1.values0 & mask2.values0, mask1.values1 & mask2.values1, mask1.values2 & mask2.values2);
    }
}

/**
 * Or 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to or with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD3<T, Width>::Mask operator|(
    const typename SIMD3<T, Width>::Mask& mask1, const typename SIMD3<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD3<T, Width>::Mask(_kor_mask8(mask1.values, mask2.values));
        } else {
            return SIMD3<T, Width>::Mask(_mm_or_ps(mask1.values, mask2.values));
        }
    } else
#endif
    {
        return SIMD3<T, Width>::Mask(
            mask1.values0 | mask2.values0, mask1.values1 | mask2.values1, mask1.values2 | mask2.values2);
    }
}

/**
 * Xor 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to xor with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD3<T, Width>::Mask operator^(
    const typename SIMD3<T, Width>::Mask& mask1, const typename SIMD3<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD3<T, Width>::Mask(_kxor_mask8(mask1.values, mask2.values));
        } else {
            return SIMD3<T, Width>::Mask(_mm_xor_ps(mask1.values, mask2.values));
        }
    } else
#endif
    {
        return SIMD3<T, Width>::Mask(
            mask1.values0 ^ mask2.values0, mask1.values1 ^ mask2.values1, mask1.values2 ^ mask2.values2);
    }
}

/**
 * Not a Mask.
 * @param mask The mask.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD3<T, Width>::Mask operator~(const typename SIMD3<T, Width>::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD3<T, Width>::Mask(_knot_mask8(mask.values));
        } else {
            return SIMD3<T, Width>::Mask(_mm_xor_ps(mask.values, _mm_cmpeq_ps(mask.values, mask.values)));
        }
    } else
#endif
    {
        return SIMD3<T, Width>::Mask(~mask.values0, ~mask.values1, ~mask.values2);
    }
}
} // namespace Shift
