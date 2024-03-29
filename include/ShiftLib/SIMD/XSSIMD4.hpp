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
#include "SIMD/XSSIMD3.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class SIMD4;

template<typename T>
class alignas(maxAlignment<T, 4>) SIMD4Data
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    T value0, value1, value2, value3;

    /** Default constructor. */
    XS_INLINE SIMD4Data() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE SIMD4Data(const SIMD4Data& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE SIMD4Data(SIMD4Data&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMD4Data& operator=(const SIMD4Data& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMD4Data& operator=(SIMD4Data&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit SIMD4Data(const SIMD4<T, Width>& other) noexcept
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
     */
    XS_INLINE void setData(const T other0, const T other1, const T other2, const T other3) noexcept
    {
        value0 = other0;
        value1 = other1;
        value2 = other2;
        value3 = other3;
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(const SIMD4<T, Width>& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            _mm_store_ps(&value0, other.values);
        } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            _mm_storeu_si32(&value0, other.values);
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            _mm_storel_epi64(&value0, other.values);
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            _mm_store_si128(&value0, other.values);
        } else
#endif
        {
            value0 = other.values0;
            value1 = other.values1;
            value2 = other.values2;
            value3 = other.values3;
        }
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD128<T>>
    XS_INLINE SIMD4<T, Width> load() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width >= SIMDWidth::B16)) {
            return SIMD4<T, Width>(_mm_load_ps(&value0));
        } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4<T, Width>(_mm_loadu_si32(&value0));
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4<T, Width>(_mm_loadl_si64(&value0));
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4<T, Width>(_mm_load_si128(&value0));
        } else
#endif
        {
            return SIMD4<T, Width>(value0, value1, value2, value3);
        }
    }
};

template<typename T>
using SIMD4DataPad = SIMD4Data<T>;

template<typename T, SIMDWidth Width = defaultWidthSIMD128<T>>
class SIMD4 : public NoExport::SIMDData<T, 4, 0, Width>
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    using Type = T;
    using InternalData = NoExport::SIMDData<T, 4, 0, Width>;
    using Data = SIMD4Data<T>;
    using DataPad = SIMD4DataPad<T>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = InternalData::width;
    static constexpr uint32 numValues = InternalData::numValues;
    static constexpr uint32 totalValues = InternalData::totalValues;
    using BaseDef = SIMDBase<T, SIMDBase<T, widthImpl>::widthImpl>;
    using InBaseDef = SIMDInBase<T, SIMDInBase<T, widthImpl>::widthImpl>;
    using SIMD2Def = SIMD2<T, SIMD2<T, widthImpl>::widthImpl>;
    using SIMD3Def = SIMD3<T, SIMD3<T, widthImpl>::widthImpl>;
    using InternalData::SIMDData;

    /** Quad Mask object used to store 4 different masks at once. */
    class Mask : public NoExport::SIMDMaskData<T, 4, 0, Width>
    {
    public:
        using Type = T;
        using InternalData = NoExport::SIMDMaskData<T, 4, 0, Width>;
        static constexpr SIMDWidth width = Width;
        static constexpr SIMDWidth widthImpl = InternalData::width;
        using InternalData::SIMDMaskData;
        using Bool = Bool4<hasSIMD<T> && (Width > SIMDWidth::Scalar)>;

        /** Default constructor. */
        XS_INLINE Mask() noexcept = default;

        /**
         * Copy constructor.
         * @param other The other.
         */
        XS_INLINE Mask(const Mask& other) noexcept = default;

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
        XS_INLINE Mask& operator=(const Mask& other) noexcept = default;

        /**
         * Move assignment operator.
         * @param [in,out] other The other.
         * @returns A shallow copy of this object.
         */
        XS_INLINE Mask& operator=(Mask&& other) noexcept = default;

        /**
         * Construct a mask from a bitwise integer representation.
         * @note Each bit in the input is used to set the mask accordingly.
         * @param mask Input bitwise representation.
         */
        XS_INLINE explicit Mask(const uint32 mask) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    this->values = _cvtu32_mask8(mask);
                } else {
                    __m128i values = _mm_shuffle0000_epi32(_mm_cvtsi32_si128(mask));
                    const __m128i bitMask = _mm_set_epi32(0x8, 0x4, 0x2, 0x1);
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
                this->values3 = mask & 0x8;
            }
        }

        /**
         * Construct a mask from individual booleans.
         * @param bool0 The first boolean value.
         * @param bool1 The second boolean value.
         * @param bool2 The third boolean value.
         * @param bool3 The fourth boolean value.
         */
        XS_INLINE Mask(const bool bool0, const bool bool1, const bool bool2, const bool bool3) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    this->values = _cvtu32_mask8((static_cast<uint32>(bool3) << 3) | (static_cast<uint32>(bool2) << 2) |
                        (static_cast<uint32>(bool1) << 1) | static_cast<uint32>(bool0));
                } else {
                    this->values = _mm_cmpneq_ps(_mm_set_ps(bool3, bool2, bool1, bool0), _mm_setzero_ps());
                }
            } else
#endif
            {
                this->values0 = bool0;
                this->values1 = bool1;
                this->values2 = bool2;
                this->values3 = bool3;
            }
        }

        /**
         * Get the boolean equivalent of the mask.
         * @returns The bool.
         */
        XS_INLINE Bool getBool4() const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return Bool4(static_cast<uint8>(_cvtmask8_u32(this->values)));
                } else {
                    return Bool4(static_cast<uint8>(_mm_movemask_ps(this->values)));
                }
            } else
#endif
            {
                return Bool4<false>(this->values0, this->values1, this->values2, this->values3);
            }
        }

        /**
         * Get boolean signalling if any element of mask is set.
         * @returns Boolean.
         */
        XS_INLINE bool getAny() const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return static_cast<bool>(_cvtmask8_u32(this->values));
                } else {
                    return static_cast<bool>(_mm_movemask_ps(this->values));
                }
            } else
#endif
            {
                return (this->values0 | this->values1) | (this->values2 | this->values3);
            }
        }

        /**
         * Get boolean signalling if all elements of mask is set.
         * @returns Boolean.
         */
        XS_INLINE bool getAll() const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return (_cvtmask8_u32(this->values) == 0xF);
                } else {
                    return (_mm_movemask_ps(this->values) == 0xF);
                }
            } else
#endif
            {
                return (this->values0 & this->values1) & (this->values2 & this->values3);
            }
        }

        /**
         * Get boolean signalling if no elements of mask is set.
         * @returns Boolean.
         */
        XS_INLINE bool getNone() const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return (_cvtmask8_u32(this->values) == 0x0);
                } else {
                    return (_mm_movemask_ps(this->values) == 0x0);
                }
            } else
#endif
            {
                return !(this->values0 | this->values1) | (this->values2 | this->values3);
            }
        }

        /**
         * Get a SIMD2::Mask.
         * @note Optimised for getting value from register.
         * @tparam Index The index of the element to retrieve (range is 0-2).
         * @returns SIMD2::Mask containing the desired values.
         */
        template<uint32 Index>
        XS_INLINE typename SIMD2Def::Mask getMask2() const noexcept
        {
            static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (Index == 0) {
                    return SIMD2Def::Mask(this->values);
                } else {
                    if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                        return SIMD2Def::Mask(_kshiftri_mask8(this->values, 2));
                    } else {
                        return SIMD2Def::Mask(_mm_shuffle3232_ps(this->values));
                    }
                }
            } else
#endif
            {
                return SIMD2Def::Mask((&this->values0)[Index * 2], (&this->values0)[Index * 2 + 1]);
            }
        }

    private:
#if XS_ISA == XS_X86
        class SIMDMasker4X86
        {
        public:
            XS_INLINE static SIMD4& ToType(SIMD4 other)
            {
                return other;
            }
        };
#endif

        template<uint32 Index>
        class SIMDMasker4
        {
        public:
            XS_INLINE static InBaseDef ToType(SIMD4 other)
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
        XS_INLINE void maskFunction(MaskOperator& maskFunc) const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                auto value = maskFunc.template expression<SIMD4, SIMDMasker4X86>();
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    value.values = _mm_mask_blend_ps(this->values, _mm_setzero_ps(), value.values);
                } else {
                    value.values = _mm_and_ps(value.values, this->values);
                }
                maskFunc.template finalExpression<SIMD4, SIMDMasker4X86>(value);
            } else
#endif
            {
                if (this->values0) {
                    auto value = maskFunc.template expression<InBaseDef, SIMDMasker4<0>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker4<0>>(value);
                }
                if (this->values1) {
                    auto value = maskFunc.template expression<InBaseDef, SIMDMasker4<1>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker4<1>>(value);
                }
                if (this->values2) {
                    auto value = maskFunc.template expression<InBaseDef, SIMDMasker4<2>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker4<2>>(value);
                }
                if (this->values3) {
                    auto value = maskFunc.template expression<InBaseDef, SIMDMasker4<3>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker4<3>>(value);
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
         *  SRGB.lessOrEqualMask(BaseDef(0.04045)).maskElseFunction<SRGBToRGBMask>(maskFunction);
         * @tparam MaskOperator The masking function type.
         * @param [in,out] maskFunc class that contains function to execute as part of masking operation.
         */
        template<typename MaskOperator>
        XS_INLINE void maskElseFunction(MaskOperator& maskFunc) const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                auto value1 = maskFunc.template expression1<SIMD4, SIMDMasker4X86>();
                auto value2 = maskFunc.template expression2<SIMD4, SIMDMasker4X86>();
                const auto final(value1.blendVar(value2, *this));
                maskFunc.template finalExpression<SIMD4, SIMDMasker4X86>(final);
            } else
#endif
            {
                auto value = (this->values0) ? maskFunc.template expression1<InBaseDef, SIMDMasker4<0>>() :
                                               maskFunc.template expression2<InBaseDef, SIMDMasker4<0>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker4<0>>(value);
                value = (this->values1) ? maskFunc.template expression1<InBaseDef, SIMDMasker4<1>>() :
                                          maskFunc.template expression2<InBaseDef, SIMDMasker4<1>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker4<1>>(value);
                value = (this->values2) ? maskFunc.template expression1<InBaseDef, SIMDMasker4<2>>() :
                                          maskFunc.template expression2<InBaseDef, SIMDMasker4<2>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker4<2>>(value);
                value = (this->values3) ? maskFunc.template expression1<InBaseDef, SIMDMasker4<3>>() :
                                          maskFunc.template expression2<InBaseDef, SIMDMasker4<3>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker4<3>>(value);
            }
        }

        /**
         * And 2 Masks.
         * @param mask1 The first mask.
         * @param mask2 mask to and with the first one.
         * @returns The result of the operation.
         */
        XS_INLINE friend Mask operator&(const Mask mask1, const Mask mask2) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return Mask(_kand_mask8(mask1.values, mask2.values));
                } else {
                    return Mask(_mm_and_ps(mask1.values, mask2.values));
                }
            } else
#endif
            {
                return Mask(mask1.values0 & mask2.values0, mask1.values1 & mask2.values1, mask1.values2 & mask2.values2,
                    mask1.values3 & mask2.values3);
            }
        }

        /**
         * Or 2 Masks.
         * @param mask1 The first mask.
         * @param mask2 mask to or with the first one.
         * @returns The result of the operation.
         */
        XS_INLINE friend Mask operator|(const Mask mask1, const Mask mask2) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return Mask(_kor_mask8(mask1.values, mask2.values));
                } else {
                    return Mask(_mm_or_ps(mask1.values, mask2.values));
                }
            } else
#endif
            {
                return Mask(mask1.values0 | mask2.values0, mask1.values1 | mask2.values1, mask1.values2 | mask2.values2,
                    mask1.values3 | mask2.values3);
            }
        }

        /**
         * Xor 2 Masks.
         * @param mask1 The first mask.
         * @param mask2 mask to xor with the first one.
         * @returns The result of the operation.
         */
        XS_INLINE friend Mask operator^(const Mask mask1, const Mask mask2) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return Mask(_kxor_mask8(mask1.values, mask2.values));
                } else {
                    return Mask(_mm_xor_ps(mask1.values, mask2.values));
                }
            } else
#endif
            {
                return Mask(mask1.values0 ^ mask2.values0, mask1.values1 ^ mask2.values1, mask1.values2 ^ mask2.values2,
                    mask1.values3 ^ mask2.values3);
            }
        }

        /**
         * Not a Mask.
         * @param mask The mask.
         * @returns The result of the operation.
         */
        XS_INLINE friend Mask operator~(const Mask mask) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return Mask(_knot_mask8(mask.values));
                } else {
                    const auto zero = _mm_setzero_ps();
                    const auto bits = _mm_cmpeq_ps(zero, zero);
                    return Mask(_mm_xor_ps(mask.values0, bits));
                }
            } else
#endif
            {
                return Mask(!mask.values0, !mask.values1, !mask.values2, !mask.values3);
            }
        }
    };

    /** Quad shuffle class used to store shuffle information for SIMD4s. */
    class Shuffle : public NoExport::SIMDShuffleData<T, 4, 0, Width>
    {
    public:
        using Type = T;
        using InternalData = NoExport::SIMDShuffleData<T, 4, 0, Width>;
        static constexpr SIMDWidth width = Width;
        static constexpr SIMDWidth widthImpl = InternalData::width;
        using InternalData::SIMDShuffleData;

        /** Default constructor. */
        XS_INLINE Shuffle() noexcept = default;

        /**
         * Copy constructor.
         * @param other The other.
         */
        XS_INLINE Shuffle(const Shuffle& other) noexcept = default;

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
        XS_INLINE Shuffle& operator=(const Shuffle& other) noexcept = default;

        /**
         * Move assignment operator.
         * @param [in,out] other The other.
         * @returns A shallow copy of this object.
         */
        XS_INLINE Shuffle& operator=(Shuffle&& other) noexcept = default;

        /**
         * Construct a shuffle from its member variables.
         * @param shuffle0 Input integer representation of the first shuffle value (must be between 0 and 3).
         * @param shuffle1 Input integer representation of the second shuffle value (must be between 0 and 3).
         * @param shuffle2 Input integer representation of the third shuffle value (must be between 0 and 3).
         * @param shuffle3 Input integer representation of the fourth shuffle value (must be between 0 and 3).
         */
        XS_INLINE Shuffle(
            const uint32 shuffle0, const uint32 shuffle1, const uint32 shuffle2, const uint32 shuffle3) noexcept
        {
            XS_ASSERT(shuffle0 < 4);
            XS_ASSERT(shuffle1 < 4);
            XS_ASSERT(shuffle2 < 4);
            XS_ASSERT(shuffle3 < 4);
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    this->values = _mm_set_epi32(shuffle3, shuffle2, shuffle1, shuffle0);
                } else {
                    this->values =
                        _mm_set_epi32((shuffle3 * 0x04040404) + 0x03020100, (shuffle2 * 0x04040404) + 0x03020100,
                            (shuffle1 * 0x04040404) + 0x03020100, (shuffle0 * 0x04040404) + 0x03020100);
                }
            } else
#endif
            {
                this->values0 = shuffle0;
                this->values1 = shuffle1;
                this->values2 = shuffle2;
                this->values3 = shuffle3;
            }
        }

        /**
         * Construct a shuffle that broadcasts a single element to all positions.
         * @param shuffle Input element to broadcast (must be between 0 and 3).
         */
        XS_INLINE explicit Shuffle(const uint32 shuffle) noexcept
        {
            XS_ASSERT(shuffle < 4);
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
                this->values3 = shuffle;
            }
        }

        /**
         * Constructor to build a Shuffle with default ordering (i.e. does not change shuffled item).
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_INLINE static Shuffle Default() noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    return Shuffle(_mm_set_epi32(3, 2, 1, 0));
                } else {
                    return Shuffle(_mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100));
                }
            } else
#endif
            {
                return Shuffle(0, 1, 2, 3);
            }
        }

        /**
         * Constructor to build a Shuffle with reverse ordering.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_INLINE static Shuffle Reverse() noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    return Shuffle(_mm_set_epi32(0, 1, 2, 3));
                } else {
                    return Shuffle(_mm_set_epi32(0x03020100, 0x07060504, 0x0B0A0908, 0x0F0E0D0C));
                }
            } else
#endif
            {
                return Shuffle(3, 2, 1, 0);
            }
        }

        /**
         * Constructor to build a Shuffle with reverse ordering with a mask.
         * @note Where the mask is valid the shuffled value will be output else the
         * original un-shuffled value will be copied.
         * @param mask Mask used to determine which elements are reversed.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_INLINE static Shuffle Reverse(const Mask& mask) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
                    return Shuffle(_mm_mask_shuffle_epi32(norm, mask.values, norm, _MM_SHUFFLE(0, 1, 2, 3)));
                } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
                    __m128i swap = _mm_shuffle3333_epi32(norm);
                    swap = _mm_and_si128(swap, _mm_castps_si128(mask.values));
                    return Shuffle(_mm_xor_si128(swap, norm));
                } else {
                    const __m128i norm = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
                    __m128i swap = _mm_set1_epi8(0xC);
                    swap = _mm_and_si128(swap, _mm_castps_si128(mask.values));
                    return Shuffle(_mm_xor_si128(swap, norm));
                }
            } else
#endif
            {
                return Shuffle(0 ^ ((mask.values0) ? 3 : 0), 1 ^ ((mask.values1) ? 3 : 0), 2 ^ ((mask.values2) ? 3 : 0),
                    3 ^ ((mask.values3) ? 3 : 0));
            }
        }

        /**
         * Constructor to build a Shuffle that swaps high 2 elements with low 2 elements.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_INLINE static Shuffle Swap() noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    return Shuffle(_mm_set_epi32(1, 0, 3, 2));
                } else {
                    return Shuffle(_mm_set_epi32(0x07060504, 0x03020100, 0x0F0E0D0C, 0x0B0A0908));
                }
            } else
#endif
            {
                return Shuffle(2, 3, 0, 1);
            }
        }

        /**
         * Constructor to build a Shuffle that swaps high 2 elements with low 2 elements with a mask.
         * @note Where the mask is valid the shuffled value will be output else the
         * original un shuffled value will be copied.
         * @param mask Mask used to determine which elements are swapped.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_INLINE static Shuffle Swap(const Mask& mask) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
                    return Shuffle(_mm_mask_shuffle_epi32(norm, mask.values, norm, _MM_SHUFFLE(1, 0, 3, 2)));
                } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
                    __m128i swap = _mm_shuffle2222_epi32(norm);
                    swap = _mm_and_si128(swap, _mm_castps_si128(mask.values));
                    return Shuffle(_mm_xor_si128(swap, norm));
                } else {
                    const __m128i norm = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
                    __m128i swap = _mm_set1_epi8(8);
                    swap = _mm_and_si128(swap, _mm_castps_si128(mask.values));
                    return Shuffle(_mm_xor_si128(swap, norm));
                }
            } else
#endif
            {
                return Shuffle(0 ^ ((mask.values0) ? 2 : 0), 1 ^ ((mask.values1) ? 2 : 0), 2 ^ ((mask.values2) ? 2 : 0),
                    3 ^ ((mask.values3) ? 2 : 0));
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
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    return Shuffle(_mm_set_epi32(2, 3, 0, 1));
                } else {
                    return Shuffle(_mm_set_epi32(0x0B0A0908, 0x0F0E0D0C, 0x03020100, 0x07060504));
                }
            } else
#endif
            {
                return Shuffle(1, 0, 3, 2);
            }
        }

        /**
         * Constructor to build a Shuffle that swaps the high element with the low element of each internal pair of
         * values with a mask.
         * @note Where the mask is valid the shuffled value will be output else the
         * original un-shuffled value will be copied.
         * @param mask Mask used to determine which elements are swapped.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_INLINE static Shuffle Swap2(const typename SIMD2Def::Mask& mask) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
                    uint32 mask2 = _cvtmask8_u32(mask.values);
                    mask2 = _pdep_u32(mask2, 0x5);
                    mask2 = mask2 | (mask2 << 1);
                    return Shuffle(_mm_mask_shuffle_epi32(norm, _cvtu32_mask8(mask2), norm, _MM_SHUFFLE(2, 3, 0, 1)));
                } else if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    const __m128i norm = _mm_set_epi32(3, 2, 1, 0);
                    __m128i swap = _mm_shuffle1111_epi32(norm);
                    swap = _mm_and_si128(swap, _mm_castps_si128(_mm_shuffle1100_ps(mask.values)));
                    return Shuffle(_mm_xor_si128(swap, norm));
                } else {
                    const __m128i norm = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
                    __m128i swap = _mm_set1_epi8(4);
                    swap = _mm_and_si128(swap, _mm_castps_si128(_mm_shuffle1100_ps(mask.values)));
                    return Shuffle(_mm_xor_si128(swap, norm));
                }
            } else
#endif
            {
                return Shuffle(static_cast<uint32>(mask.values0), 1_ui32 ^ static_cast<uint32>(mask.values0),
                    2_ui32 ^ static_cast<uint32>(mask.values1), 3_ui32 ^ static_cast<uint32>(mask.values1));
            }
        }

        /**
         * Constructor to build a Shuffle that conditionally interleaves even and off values together.
         * @note Where the conditional is true then shuffle produces even values in the low 2 elements and odd values in
         * the high (i.e. (0,2,1,3)) otherwise odd values go first (i.e. (1.3.0.2)).
         * @param conditional Conditional value used to determine whether elements are swapped.
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_INLINE static Shuffle Interleave(const bool conditional) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    const __m128i norm = _mm_set_epi32(3, 1, 2, 0);
                    const __m128i condition = _mm_set1_epi32(conditional);
                    return Shuffle(_mm_xor_si128(condition, norm));
                } else {
                    const __m128i norm = _mm_set_epi32(0x0F0E0D0C, 0x07060504, 0x0B0A0908, 0x03020100);
                    const __m128i condition = _mm_set1_epi8(conditional * 4);
                    return Shuffle(_mm_xor_si128(condition, norm));
                }
            } else
#endif
            {
                const uint32 condition = conditional;
                return Shuffle(0 ^ condition, 2 ^ condition, 1 ^ condition, 3 ^ condition);
            }
        }

        /**
         * Constructor to build a Shuffle that rotates left a certain number of times.
         * @param rotate Number of times to rotate (must be between 0 and 3).
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_INLINE static Shuffle RotateLeft(const uint32 rotate) noexcept
        {
            XS_ASSERT(rotate < 4);
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    __m128i val0 = _mm_set_epi32(3, 2, 1, 0);
                    const __m128i val1 = _mm_set1_epi32(rotate);
                    const __m128i mask = _mm_shuffle3333_epi32(val0);
                    val0 = _mm_add_epi32(val0, val1);
                    return Shuffle(_mm_and_si128(val0, mask));
                } else {
                    __m128i val0 = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
                    const __m128i val1 = _mm_set1_epi8(static_cast<int8_t>(rotate) * 4);
                    const __m128i mask = _mm_set1_epi8(0xF);
                    val0 = _mm_add_epi8(val0, val1);
                    return Shuffle(_mm_and_si128(val0, mask));
                }
            } else
#endif
            {
                return Shuffle(rotate & 0x3, (1 + rotate) & 0x3, (2 + rotate) & 0x3, (3 + rotate) & 0x3);
            }
        }

        /**
         * Constructor to build a Shuffle that rotates right a certain number of times.
         * @param rotate Number of times to rotate (must be between 0 and 3).
         * @returns Newly constructed Shuffle with required attributes.
         */
        XS_INLINE static Shuffle RotateRight(const uint32 rotate) noexcept
        {
            XS_ASSERT(rotate < 4);
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    __m128i val0 = _mm_set_epi32(3, 2, 1, 0);
                    const __m128i val1 = _mm_set1_epi32(rotate);
                    const __m128i mask = _mm_shuffle3333_epi32(val0);
                    val0 = _mm_sub_epi32(val0, val1);
                    return Shuffle(_mm_and_si128(val0, mask));
                } else {
                    __m128i val0 = _mm_set_epi32(0x0F0E0D0C, 0x0B0A0908, 0x07060504, 0x03020100);
                    const __m128i val1 = _mm_set1_epi8(static_cast<int8_t>(rotate) * 4);
                    const __m128i mask = _mm_set1_epi8(0xF);
                    val0 = _mm_sub_epi8(val0, val1);
                    return Shuffle(_mm_and_si128(val0, mask));
                }
            } else
#endif
            {
                return Shuffle((0 - rotate) & 0x3, (1 - rotate) & 0x3, (2 - rotate) & 0x3, (3 - rotate) & 0x3);
            }
        }

        /**
         * Shuffles the elements of the shuffle.
         * @tparam Index0 The index of the first element to insert into the returned shuffle (must be between 0 and 3).
         * @tparam Index1 The index of the second element to insert into the returned shuffle (must be between 0 and 3).
         * @tparam Index2 The index of the third element to insert into the returned shuffle (must be between 0 and 3).
         * @tparam Index3 The index of the fourth element to insert into the returned shuffle (must be between 0 and 3).
         * @returns The result of the operation.
         */
        template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
        XS_INLINE Shuffle shuffle() const noexcept
        {
            static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4, "Invalid Index: Indexes must be <4");
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
                    return *this;
                } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 0 && Index3 == 0) {
                    return Shuffle(_mm_shuffle0000_epi32(this->values));
                } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 3) {
                    return Shuffle(_mm_shuffle3200_epi32(this->values));
                } else if constexpr (Index0 == 1 && Index1 == 0 && Index2 == 2 && Index3 == 3) {
                    return Shuffle(_mm_shuffle3201_epi32(this->values));
                } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
                    return Shuffle(_mm_shuffle3211_epi32(this->values));
                } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 2) {
                    return Shuffle(_mm_shuffle2210_epi32(this->values));
                } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 3 && Index3 == 2) {
                    return Shuffle(_mm_shuffle2310_epi32(this->values));
                } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 3 && Index3 == 3) {
                    return Shuffle(_mm_shuffle3310_epi32(this->values));
                } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3) {
                    return Shuffle(_mm_shuffle3322_epi32(this->values));
                } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
                    return Shuffle(_mm_shuffle1100_epi32(this->values));
                } else {
                    return Shuffle(_mm_shuffle_epi32(this->values, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
                }
            } else
#endif
            {
                return Shuffle((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2],
                    (&this->values0)[Index3]);
            }
        }
    };

    /** Default constructor. */
    XS_INLINE SIMD4() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE SIMD4(const SIMD4& other) noexcept = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE SIMD4(SIMD4&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMD4& operator=(const SIMD4& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMD4& operator=(SIMD4&& other) noexcept = default;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_INLINE explicit SIMD4(const SIMD4<T, Width2>& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar) && (Width2 > SIMDWidth::Scalar)) {
            this->values = other.values;
        } else if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            *this = SIMD4(other.values0, other.values1, other.values2, other.values3);
        } else if constexpr (Width2 > SIMDWidth::Scalar) {
            this->values0 = other.template getValueInBase<0>().getValue();
            this->values1 = other.template getValueInBase<1>().getValue();
            this->values2 = other.template getValueInBase<2>().getValue();
            this->values3 = other.template getValueInBase<3>().getValue();
        } else
#endif
        {
            this->values0 = other.values0;
            this->values1 = other.values1;
            this->values2 = other.values2;
            this->values3 = other.values3;
        }
    }

    /**
     * Construct from 4 different values.
     * @param value0 The first value.
     * @param value1 The second value.
     * @param value2 The third value.
     * @param value3 The fourth value.
     */
    XS_INLINE SIMD4(const T value0, const T value1, const T value2, const T value3) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = _mm_set_ps(value3, value2, value1, value0);
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = _mm_set_epi32(
                static_cast<T>(value3), static_cast<T>(value2), static_cast<T>(value1), static_cast<T>(value0));
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = _mm_set_epi16(0, 0, 0, 0, static_cast<T>(value3), static_cast<T>(value2),
                static_cast<T>(value1), static_cast<T>(value0));
        } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = _mm_set_epi8(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, static_cast<T>(value3),
                static_cast<T>(value2), static_cast<T>(value1), static_cast<T>(value0));
        } else
#endif
        {
            this->values0 = value0;
            this->values1 = value1;
            this->values2 = value2;
            this->values3 = value3;
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param value Value to set all elements to.
     */
    XS_INLINE explicit SIMD4(const T value) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = _mm_set1_ps(value);
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = _mm_set1_epi32(static_cast<T>(value));
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = _mm_set1_epi16(static_cast<T>(value));
        } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = _mm_set1_epi8(static_cast<T>(value));
        } else
#endif
        {
            this->values0 = value;
            this->values1 = value;
            this->values2 = value;
            this->values3 = value;
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_INLINE explicit SIMD4(const BaseDef other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = other.values;
        } else
#endif
        {
            this->values0 = other.value;
            this->values1 = other.value;
            this->values2 = other.value;
            this->values3 = other.value;
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_INLINE explicit SIMD4(const InBaseDef other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = _mm_shuffle0000_ps(other.values);
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = _mm_shuffle0000_epi32(other.values);
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = _mm_shufflelo_epi16(other.values, _MM_SHUFFLE(0, 0, 0, 0));
        } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = _mm_broadcastb_epi8(other.values);
        } else
#endif
        {
            this->values0 = other.value;
            this->values1 = other.value;
            this->values2 = other.value;
            this->values3 = other.value;
        }
    }

    /**
     * Constructor to build from a SIMD3 plus a fourth value.
     * @param other0 Input SIMD3.
     * @param other1 Input fourth element.
     */
    XS_INLINE SIMD4(const SIMD3Def& other0, const InBaseDef other1) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = _mm_insert_ps(other0.values, other1.values, _MM_MK_INSERTPS_NDX(0, 3, 0));
        } else
#endif
        {
            this->values0 = other0.values0;
            this->values1 = other0.values1;
            this->values2 = other0.values2;
            this->values3 = other1.value;
        }
    }

    /**
     * Constructor to build from a SIMD3 plus a fourth value.
     * @param other0 Input SIMD3.
     * @param other1 Input fourth element.
     */
    XS_INLINE SIMD4(const SIMD3Def& other0, const BaseDef other1) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = _mm_blend_ps(other0.values, other1.values, _MM_BLEND(1, 0, 0, 0));
        } else
#endif
        {
            this->values0 = other0.values0;
            this->values1 = other0.values1;
            this->values2 = other0.values2;
            this->values3 = other1.value;
        }
    }

    /**
     * Constructor to build from 2 SIMD2s.
     * @param other0 First input SIMD2.
     * @param other1 Second input SIMD2.
     */
    XS_INLINE SIMD4(const SIMD2Def& other0, const SIMD2Def& other1) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = _mm_movelh_ps(other0.values, other1.values);
        } else
#endif
        {
            this->values0 = other0.values0;
            this->values1 = other0.values1;
            this->values2 = other1.values0;
            this->values3 = other1.values1;
        }
    }

    /**
     * Constructor to build from a single SIMD2.
     * @param other The SIMD2.
     */
    XS_INLINE explicit SIMD4(const SIMD2Def& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = _mm_shuffle1010_ps(other.values);
        } else
#endif
        {
            this->values0 = other.values0;
            this->values1 = other.values1;
            this->values2 = other.values0;
            this->values3 = other.values1;
        }
    }

    /**
     * Constructor to build set to 0.
     * @returns Newly constructed object with required attributes.
     */
    XS_INLINE static SIMD4 Zero() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_setzero_ps());
        } else if constexpr (isSameAny<T, uint8, int8, uint16, int16, uint32, int32> && hasSIMD<T> &&
            (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_setzero_si128());
        } else
#endif
        {
            return SIMD4(T{0});
        }
    }

    /**
     * Constructor to build set to 1.
     * @returns Newly constructed object with required attributes.
     */
    XS_INLINE static SIMD4 One() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_set1_ps(1.0f));
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_set1_epi32(1.0));
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_set1_epi16(1.0));
        } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_set1_epi8(1.0));
        } else
#endif
        {
            return SIMD4(T{1});
        }
    }

    /**
     * Shuffles the elements of 2 SIMD3s together to a form a new SIMD4.
     * @note Any index in range 0-2 is taken from first object. Any index in range 3-5 is taken from second object.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-5).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-5).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-5).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-5).
     * @param other1 First input.
     * @param other2 Second input.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
    XS_INLINE static SIMD4 Combine4(const SIMD3Def& other1, const SIMD3Def& other2) noexcept
    {
        static_assert(Index0 < 6 && Index1 < 6 && Index2 < 6 && Index3 < 6, "Invalid Index: Indexes must be <6");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 3 && Index3 == 4) {
                return SIMD4(_mm_movelh_ps(other1.values, other2.values));
            } else if constexpr (Index0 == 3 && Index1 == 4 && Index2 == 0 && Index3 == 1) {
                return SIMD4(_mm_movelh_ps(other2.values, other1.values));
            } else if constexpr (Index0 == 0 && Index1 == 3 && Index2 == 1 && Index3 == 4) {
                return SIMD4(_mm_unpacklo_ps(other1.values, other2.values));
            } else if constexpr (Index0 == 3 && Index1 == 0 && Index2 == 4 && Index3 == 1) {
                return SIMD4(_mm_unpacklo_ps(other2.values, other1.values));
            } else if constexpr (Index0 < 3 && Index1 < 3 && Index2 < 3 && Index3 < 3) {
                return SIMD4(other1.values).shuffle<Index0, Index1, Index2, Index3>();
            } else if constexpr (Index0 >= 3 && Index1 >= 3 && Index2 >= 3 && Index3 >= 3) {
                return SIMD4(other2.values).shuffle<Index0 - 3, Index1 - 3, Index2 - 3, Index3 - 3>();
            } else if constexpr (Index0 < 3 && Index1 < 3 && Index2 >= 3 && Index3 >= 3) {
                return SIMD4(
                    _mm_shuffle_ps(other1.values, other2.values, _MM_SHUFFLE(Index3 - 3, Index2 - 3, Index1, Index0)));
            } else if constexpr (Index0 >= 3 && Index1 >= 3 && Index2 < 3 && Index3 < 3) {
                return SIMD4(
                    _mm_shuffle_ps(other2.values, other1.values, _MM_SHUFFLE(Index3, Index2, Index1 - 3, Index0 - 3)));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
                return SIMD4(_mm_insert_ps(other1.values, other2.values, _MM_MK_INSERTPS_NDX(Index3 - 3, 3, 0)));
            } else if constexpr (Index0 == 3 && Index1 == 4 && Index2 == 5) {
                return SIMD4(_mm_insert_ps(other2.values, other1.values, _MM_MK_INSERTPS_NDX(Index3, 3, 0)));
            } else if constexpr (Index0 >= 3 && Index1 < 3 && Index2 < 3 && Index3 >= 3) {
                if constexpr (Index1 == 1 && Index2 == 2) {
                    return SIMD4(_mm_blend_ps(other1.values,
                        _mm_shuffle_ps(other2.values, other2.values, _MM_SHUFFLE(Index3 - 3, 2, 1, Index0 - 3)),
                        _MM_BLEND(1, 0, 0, 1)));
                } else {
                    const __m128 val = _mm_shuffle_ps(
                        other1.values, other2.values, _MM_SHUFFLE(Index3 - 3, Index0 - 3, Index2, Index1));
                    return SIMD4(_mm_shuffle1_ps(val, _MM_SHUFFLE(3, 1, 0, 2)));
                }
            } else if constexpr (Index0 >= 3 && Index1 < 3 && Index2 >= 3 && Index3 < 3) {
                const __m128 val =
                    _mm_shuffle_ps(other1.values, other2.values, _MM_SHUFFLE(Index2 - 3, Index0 - 3, Index3, Index1));
                return SIMD4(_mm_shuffle1_ps(val, _MM_SHUFFLE(1, 3, 0, 2)));
            } else if constexpr (Index0 >= 3 && Index1 >= 3 && Index2 >= 3 && Index3 < 3) {
                const __m128 val = _mm_shuffle_ps(other1.values, other2.values, _MM_SHUFFLE(0, Index2 - 3, 2, Index3));
                return SIMD4(_mm_shuffle_ps(other2.values, val, _MM_SHUFFLE(0, 2, Index1 - 3, Index0 - 3)));
            } else if constexpr (Index0 >= 3 && Index1 >= 3 && Index2 < 3 && Index3 >= 3) {
                if constexpr (Index2 == 2) {
                    return SIMD4(_mm_blend_ps(other1.values,
                        _mm_shuffle_ps(
                            other2.values, other2.values, _MM_SHUFFLE(Index3 - 3, 2, Index1 - 3, Index0 - 3)),
                        _MM_BLEND(1, 0, 1, 1)));
                } else {
                    const __m128 val =
                        _mm_shuffle_ps(other1.values, other2.values, _MM_SHUFFLE(0, Index3 - 3, 2, Index2));
                    return SIMD4(_mm_shuffle_ps(other2.values, val, _MM_SHUFFLE(2, 0, Index1 - 3, Index0 - 3)));
                }
            } else if constexpr (Index0 >= 3 && Index1 < 3 && Index2 >= 3 && Index3 >= 3) {
                if constexpr (Index1 == 1) {
                    return SIMD4(_mm_blend_ps(other1.values,
                        _mm_shuffle_ps(
                            other2.values, other2.values, _MM_SHUFFLE(Index3 - 3, Index2 - 3, 1, Index0 - 3)),
                        _MM_BLEND(1, 1, 0, 1)));
                } else {
                    const __m128 val =
                        _mm_shuffle_ps(other1.values, other2.values, _MM_SHUFFLE(0, Index0 - 3, 2, Index1));
                    return SIMD4(_mm_shuffle_ps(val, other2.values, _MM_SHUFFLE(Index3 - 3, Index2 - 3, 0, 2)));
                }
            } else if constexpr (Index0 < 3 && Index1 >= 3 && Index2 >= 3 && Index3 >= 3) {
                if constexpr (Index0 == 0) {
                    return SIMD4(_mm_blend_ps(other1.values,
                        _mm_shuffle_ps(
                            other2.values, other2.values, _MM_SHUFFLE(Index3 - 3, Index2 - 3, Index1 - 3, 0)),
                        _MM_BLEND(1, 1, 1, 0)));
                } else {
                    const __m128 val =
                        _mm_shuffle_ps(other1.values, other2.values, _MM_SHUFFLE(0, Index1 - 3, 2, Index0));
                    return SIMD4(_mm_shuffle_ps(val, other2.values, _MM_SHUFFLE(Index3 - 3, Index2 - 3, 2, 0)));
                }
            } else {
                // Just use specialisation for inverse arrangement
                return Combine4<Index0 + 3 - ((Index0 >= 3) * 6), Index1 + 3 - ((Index1 >= 3) * 6),
                    Index2 + 3 - ((Index2 >= 3) * 6), Index3 + 3 - ((Index3 >= 3) * 6)>(other2, other1);
            }
        } else
#endif
        {
            return SIMD4(((Index0 < 3) ? &other1.values0 : &other2.values0)[Index0 % 3],
                ((Index1 < 3) ? &other1.values0 : &other2.values0)[Index1 % 3],
                ((Index2 < 3) ? &other1.values0 : &other2.values0)[Index2 % 3],
                ((Index3 < 3) ? &other1.values0 : &other2.values0)[Index3 % 3]);
        }
    }

    /**
     * Transpose 4 SIMD3s into 3 SIMD4s.
     * @note Returns results in the 3 otherT parameters. The transpose operation results in the first return
     * containing the first element of each of the 4 inputs. The second return contains all the second elements from
     * the inputs and so forth.
     * @param          other0  The first SIMD3 to transpose.
     * @param          other1  The second SIMD3 to transpose.
     * @param          other2  The third SIMD3 to transpose.
     * @param          other3  The fourth SIMD3 to transpose.
     * @param [in,out] otherT0 The first SIMD4 to return the transposed results in.
     * @param [in,out] otherT1 The second SIMD4 to return the transposed results in.
     * @param [in,out] otherT2 The third SIMD4 to return the transposed results in.
     */
    XS_INLINE static void Transpose(const SIMD3Def& other0, const SIMD3Def& other1, const SIMD3Def& other2,
        const SIMD3Def& other3, SIMD4& otherT0, SIMD4& otherT1, SIMD4& otherT2) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128 val1 = _mm_unpacklo_ps(other0.values, other1.values);
            const __m128 val2 = _mm_unpacklo_ps(other2.values, other3.values);
            const __m128 val3 = _mm_unpackhi_ps(other0.values, other1.values);
            const __m128 val4 = _mm_unpackhi_ps(other2.values, other3.values);

            otherT0.values = _mm_movelh_ps(val1, val2);
            otherT1.values = _mm_movehl_ps(val2, val1);
            otherT2.values = _mm_movelh_ps(val3, val4);
        } else
#endif
        {
            otherT0.values0 = other0.values0;
            otherT0.values1 = other1.values0;
            otherT0.values2 = other2.values0;
            otherT0.values3 = other3.values0;
            otherT1.values0 = other0.values1;
            otherT1.values1 = other1.values1;
            otherT1.values2 = other2.values1;
            otherT1.values3 = other3.values1;
            otherT2.values0 = other0.values2;
            otherT2.values1 = other1.values2;
            otherT2.values2 = other2.values2;
            otherT2.values3 = other3.values2;
        }
    }

    /**
     * Transpose 4 SIMD4s.
     * @note Returns results in the 4 otherT parameters. The transpose operation results in the first return
     * containing the first element of each of the 4 inputs. The second return contains all the second elements from
     * the inputs and so forth.
     * @param          other0  The first SIMD4 to transpose.
     * @param          other1  The second SIMD4 to transpose.
     * @param          other2  The third SIMD4 to transpose.
     * @param          other3  The fourth SIMD4 to transpose.
     * @param [in,out] otherT0 The first SIMD4 to return the transposed results in.
     * @param [in,out] otherT1 The second SIMD4 to return the transposed results in.
     * @param [in,out] otherT2 The third SIMD4 to return the transposed results in.
     * @param [in,out] otherT3 The fourth SIMD4 to return the transposed results in.
     */
    XS_INLINE static void Transpose(const SIMD4& other0, const SIMD4& other1, const SIMD4& other2, const SIMD4& other3,
        SIMD4& otherT0, SIMD4& otherT1, SIMD4& otherT2, SIMD4& otherT3) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128 val1 = _mm_unpacklo_ps(other0.values, other1.values);
            const __m128 val2 = _mm_unpacklo_ps(other2.values, other3.values);
            const __m128 val3 = _mm_unpackhi_ps(other0.values, other1.values);
            const __m128 val4 = _mm_unpackhi_ps(other2.values, other3.values);

            otherT0.values = _mm_movelh_ps(val1, val2);
            otherT1.values = _mm_movehl_ps(val2, val1);
            otherT2.values = _mm_movelh_ps(val3, val4);
            otherT3.values = _mm_movehl_ps(val4, val3);
        } else
#endif
        {
            otherT0.values0 = other0.values0;
            otherT0.values1 = other1.values0;
            otherT0.values2 = other2.values0;
            otherT0.values3 = other3.values0;
            otherT1.values0 = other0.values1;
            otherT1.values1 = other1.values1;
            otherT1.values2 = other2.values1;
            otherT1.values3 = other3.values1;
            otherT2.values0 = other0.values2;
            otherT2.values1 = other1.values2;
            otherT2.values2 = other2.values2;
            otherT2.values3 = other3.values2;
            otherT3.values0 = other0.values3;
            otherT3.values1 = other1.values3;
            otherT3.values2 = other2.values3;
            otherT3.values3 = other3.values3;
        }
    }

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-3).
     * @returns SIMDInBase containing the desired value.
     */
    template<uint32 Index>
    XS_INLINE InBaseDef getValueInBase() const noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index == 0) {
                return InBaseDef(this->values);
            } else if constexpr (Index == 1) {
                return InBaseDef(_mm_shuffle3311_ps(this->values)); //(x,x,x,1)
            } else if constexpr (Index == 2) {
                return InBaseDef(_mm_shuffle3232_ps(this->values)); //(x,x,x,2)
            } else /*Index == 3*/ {
                return InBaseDef(_mm_shuffle3333_ps(this->values)); //(x,x,x,3)
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
     * @tparam Index The index of the element to retrieve (range is 0-3).
     * @returns BaseDef containing the desired value.
     */
    template<uint32 Index>
    XS_INLINE BaseDef getValue() const noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index == 0) {
                return BaseDef(_mm_shuffle0000_ps(this->values));
            } else if constexpr (Index == 1) {
                return BaseDef(_mm_shuffle1111_ps(this->values));
            } else if constexpr (Index == 2) {
                return BaseDef(_mm_shuffle2222_ps(this->values));
            } else /*Index == 3*/ {
                return BaseDef(_mm_shuffle3333_ps(this->values));
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
     * @param index The index of the element to retrieve (range is 0-3).
     * @returns SIMDInBase containing the desired value.
     */
    XS_INLINE InBaseDef getValueInBase(const uint32 index) const noexcept
    {
        XS_ASSERT(index < 4);
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return InBaseDef(_mm_permutevar_ps(this->values, _mm_cvtsi32_si128(index)));
            } else {
                __m128i value = _mm_broadcastb_epi8(_mm_cvtsi32_si128(index * 4));
                value = _mm_add_epi8(value, _mm_set1_epi32(0x03020100)); // equivalent to (3,2,1,0)
                return InBaseDef(_mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(this->values), value)));
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
     * @param index The index of the element to retrieve (range is 0-3).
     * @returns BaseDef containing the desired value.
     */
    XS_INLINE BaseDef getValue(const uint32 index) const noexcept
    {
        XS_ASSERT(index < 4);
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return BaseDef(_mm_permutevar_ps(this->values, _mm_shuffle0000_epi32(_mm_cvtsi32_si128(index))));
            } else {
                __m128i value = _mm_broadcastb_epi8(_mm_cvtsi32_si128(index * 4));
                value = _mm_add_epi8(value, _mm_set1_epi32(0x03020100));
                return BaseDef(_mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(this->values), value)));
            }
        } else
#endif
        {
            return BaseDef((&this->values0)[index]);
        }
    }

    /**
     * Get a SIMD2.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-2).
     * @returns SIMD2 containing the desired values.
     */
    template<uint32 Index>
    XS_INLINE SIMD2Def getValue2() const noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index == 0) {
                return SIMD2Def(this->values);
            } else {
                return SIMD2Def(_mm_shuffle3232_ps(this->values));
            }
        } else
#endif
        {
            return SIMD2Def((&this->values0)[Index * 2], (&this->values0)[Index * 2 + 1]);
        }
    }

    /**
     * Get an 2 elements of a SIMD4 as a SIMD2.
     * @note Optimised for getting value from register.
     * @tparam Index0 The index of the first element to retrieve (range is 0-3).
     * @tparam Index1 The index of the second element to retrieve (range is 0-3).
     * @returns SIMD2 containing the desired values.
     */
    template<uint32 Index0, uint32 Index1>
    XS_INLINE SIMD2Def getValue2() const noexcept
    {
        static_assert(Index0 < 4 && Index1 < 4, "Invalid Index: Indexes must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD2Def(_mm_shuffle2200_ps(this->values));
            } else if constexpr (Index0 == 0 && Index1 == 1) {
                return SIMD2Def(this->values);
            } else if constexpr (Index0 == 2 && Index1 == 3) {
                return SIMD2Def(_mm_shuffle3232_ps(this->values));
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
     * Get any 3 elements of a SIMD4 as a SIMD3.
     * @note Optimised for getting value from register.
     * @tparam Index0 The index of the first element to retrieve (range is 0-3).
     * @tparam Index1 The index of the second element to retrieve (range is 0-3).
     * @tparam Index2 The index of the third element to retrieve (range is 0-3).
     * @returns SIMD3 containing the desired values.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2>
    XS_INLINE SIMD3Def getValue3() const noexcept
    {
        static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4, "Invalid Index: Indexes must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
                return SIMD3Def(this->values);
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0) {
                return SIMD3Def(_mm_shuffle1010_ps(this->values));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1) {
                return SIMD3Def(_mm_shuffle1100_ps(this->values));
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3) {
                return SIMD3Def(_mm_shuffle3322_ps(this->values));
            } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2) {
                return SIMD3Def(_mm_shuffle3232_ps(this->values));
            } else {
                return SIMD3Def(_mm_shuffle1_ps(this->values, _MM_SHUFFLE(3, Index2, Index1, Index0)));
            }
        } else
#endif
        {
            return SIMD3Def((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2]);
        }
    }

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-3).
     * @param other The new value.
     * @returns The current object after modification.
     */
    template<uint32 Index>
    XS_INLINE SIMD4& setValue(const BaseDef other) noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index == 0) {
                this->values = _mm_blend_ss(this->values, other.values);
            } else {
                this->values = _mm_blend_ps(this->values, other.values, (1 << Index));
            }
        } else
#endif
        {
            (&this->values0)[Index] = other.value;
        }
        return *this;
    }

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-3).
     * @param other The new value.
     * @returns The current object after modification.
     */
    template<uint32 Index>
    XS_INLINE SIMD4& setValue(const InBaseDef other) noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
        return *this;
    }

    /**
     * Dynamically set an element of the object.
     * @param index The index of the element to retrieve (range is 0-3).
     * @param other The new value.
     * @returns The current object after modification.
     */
    XS_INLINE SIMD4& setValue(const uint32 index, const BaseDef other) noexcept
    {
        XS_ASSERT(index < 4);
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
        return *this;
    }

    /**
     * Set a SIMD2.
     * @tparam Index The index of the element to set (range is 0-2).
     * @param other The new values.
     * @returns The current object after modification.
     */
    template<uint32 Index>
    XS_INLINE SIMD4& setValue2(const SIMD2Def& other) noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index == 0) {
                this->values = _mm_shuffle_ps(other.values, this->values, _MM_SHUFFLE(3, 2, 1, 0));
            } else {
                this->values = _mm_movelh_ps(this->values, other.values);
            }
        } else
#endif
        {
            (&this->values0)[Index * 2] = other.values0;
            (&this->values0)[Index * 2 + 1] = other.values1;
        }
        return *this;
    }

    /**
     * Add a value to an element of the object.
     * @tparam Index The index of the element to modify (range is 0-3).
     * @param other The value to add.
     * @returns The current object after modification.
     */
    template<uint32 Index>
    XS_INLINE SIMD4& addValue(const BaseDef other) noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index == 0) {
                this->values = _mm_add_ss(this->values, other.values);
            } else if constexpr (Index == 1) {
                this->values = _mm_blend_add_ps(this->values, 1 << Index, this->values, other.values);
            } else {
                this->values = _mm_blend_add_ps(this->values, 1 << Index, this->values, other.values);
            }
        } else
#endif
        {
            (&this->values0)[Index] += other.value;
        }
        return *this;
    }

    /**
     * Add a value to an element of the object.
     * @tparam Index The index of the element to modify (range is 0-3).
     * @param other The value to add.
     * @returns The current object after modification.
     */
    template<uint32 Index>
    XS_INLINE SIMD4& addValue(const InBaseDef other) noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
        return *this;
    }

    /**
     * Subtract a value from an element of the object.
     * @tparam Index The index of the element to modify (range is 0-3).
     * @param other The value to subtract.
     * @returns The current object after modification.
     */
    template<uint32 Index>
    XS_INLINE SIMD4& subValue(const InBaseDef other) noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
        return *this;
    }

    /**
     * Subtract a value from an element of the object.
     * @tparam Index The index of the element to modify (range is 0-3).
     * @param other The value to subtract.
     * @returns The current object after modification.
     */
    template<uint32 Index>
    XS_INLINE SIMD4& subValue(const BaseDef other) noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index == 0) {
                this->values = _mm_sub_ss(this->values, other.values);
            } else if constexpr (Index == 1) {
                this->values = _mm_blend_sub_ps(this->values, 1 << Index, this->values, other.values);
            } else {
                this->values = _mm_blend_sub_ps(this->values, 1 << Index, this->values, other.values);
            }
        } else
#endif
        {
            (&this->values0)[Index] -= other.value;
        }
        return *this;
    }

    /**
     * Multiply an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-3).
     * @param other The value to multiply by.
     * @returns The current object after modification.
     */
    template<uint32 Index>
    XS_INLINE SIMD4& mulValue(const InBaseDef other) noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
        return *this;
    }

    /**
     * Multiply an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-3).
     * @param other The value to multiply by.
     * @returns The current object after modification.
     */
    template<uint32 Index>
    XS_INLINE SIMD4& mulValue(const BaseDef other) noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index == 0) {
                this->values = _mm_mul_ss(this->values, other.values);
            } else if constexpr (Index == 1) {
                this->values = _mm_blend_mul_ps(this->values, 1 << Index, this->values, other.values);
            } else {
                this->values = _mm_blend_mul_ps(this->values, 1 << Index, this->values, other.values);
            }
        } else
#endif
        {
            (&this->values0)[Index] *= other.value;
        }
        return *this;
    }

    /**
     * Divide an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-3).
     * @param other The value to divide by.
     * @returns The current object after modification.
     */
    template<uint32 Index>
    XS_INLINE SIMD4& divValue(const InBaseDef other) noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
        return *this;
    }

    /**
     * Divide an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-3).
     * @param other The value to divide by.
     * @returns The current object after modification.
     */
    template<uint32 Index>
    XS_INLINE SIMD4& divValue(const BaseDef other) noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index == 0) {
                this->values = _mm_div_ss(this->values, other.values);
            } else if constexpr (Index == 1) {
                this->values = _mm_blend_div_ps(this->values, 1 << Index, this->values, other.values);
            } else {
                this->values = _mm_blend_div_ps(this->values, 1 << Index, this->values, other.values);
            }
        } else
#endif
        {
            (&this->values0)[Index] /= other.value;
        }
        return *this;
    }

    /**
     * Multiply then add to an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-3).
     * @param other1 The value to multiply by.
     * @param other2 The value to add.
     * @returns The current object after modification.
     */
    template<uint32 Index>
    XS_INLINE SIMD4& madValue(const InBaseDef other1, const InBaseDef other2) noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index % 4 == 0) {
                this->values = _mm_fmadd_ss(this->values, other1.values, other2.values);
            } else if constexpr (Index % 4 == 1) {
                const __m128 value1 = _mm_shuffle2200_ps(other1.values);
                const __m128 value2 = _mm_shuffle2200_ps(other2.values);
                this->values = _mm_blend_fmadd_ps(this->values, 1 << (Index % 4), value1, value2);
            } else {
                const __m128 value1 = _mm_shuffle0000_ps(other1.values);
                const __m128 value2 = _mm_shuffle0000_ps(other2.values);
                this->values = _mm_blend_fmadd_ps(this->values, 1 << (Index % 4), value1, value2);
            }
        } else
#endif
        {
            (&this->values0)[Index] = Shift::fma<T>((&this->values0)[Index], other1.value, other2.value);
        }
        return *this;
    }

    /**
     * Multiply then add to an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-3).
     * @param other1 The value to multiply by.
     * @param other2 The value to add.
     * @returns The current object after modification.
     */
    template<uint32 Index>
    XS_INLINE SIMD4& madValue(const BaseDef other1, const BaseDef other2) noexcept
    {
        static_assert(Index < 4, "Invalid Index: Index must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index % 4 == 0) {
                this->values = _mm_fmadd_ss(this->values, other1.values, other2.values);
            } else if constexpr (Index % 4 == 1) {
                this->values = _mm_blend_fmadd_ps(this->values, 1 << (Index % 4), other1.values, other2.values);
            } else {
                this->values = _mm_blend_fmadd_ps(this->values, 1 << (Index % 4), other1.values, other2.values);
            }
        } else
#endif
        {
            (&this->values0)[Index] = Shift::fma<T>((&this->values0)[Index], other1.value, other2.value);
        }
        return *this;
    }

    /**
     * Negate an element of the object.
     * @tparam Elem0 Boolean indicating if first element should be negated.
     * @tparam Elem1 Boolean indicating if second element should be negated.
     * @tparam Elem2 Boolean indicating if third element should be negated.
     * @tparam Elem3 Boolean indicating if fourth element should be negated.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3>
    XS_INLINE SIMD4 negate() const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
                return SIMD4(_mm_xor_ps(this->values, _mm_set_ss(-0.0f)));
            } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
                return SIMD4(_mm_xor_ps(this->values, _mm_set1_ps(-0.0f)));
            } else {
                return SIMD4(_mm_xor_ps(this->values,
                    _mm_set_ps(
                        Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)));
            }
        }
#endif
        else {
            return SIMD4(Elem0 ? -this->values0 : this->values0, Elem1 ? -this->values1 : this->values1,
                Elem2 ? -this->values2 : this->values2, Elem3 ? -this->values3 : this->values3);
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
    XS_INLINE SIMD4 mad(const SIMD4& other1, const SIMD4& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD4(_mm_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD4(_mm_add_ps(_mm_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_add_epi32(_mm_mullo_epi32(this->values, other1.values), other2.values));
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_add_epi16(_mm_mullo_epi16(this->values, other1.values), other2.values));
        } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128i ret =
                _mm_add_epi16(_mm_mullo_epi16(_mm_cvtepu8_epi16(this->values), _mm_cvtepu8_epi16(other1.values)),
                    _mm_cvtepu8_epi16(other2.values));
            return SIMD4(_mm_packus_epi16(ret, ret));
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128i ret =
                _mm_add_epi16(_mm_mullo_epi16(_mm_cvtepi8_epi16(this->values), _mm_cvtepi8_epi16(other1.values)),
                    _mm_cvtepi8_epi16(other2.values));
            return SIMD4(_mm_packs_epi16(ret, ret));
        } else
#endif
        {
            return SIMD4(Shift::fma<T>(this->values0, other1.values0, other2.values0),
                Shift::fma<T>(this->values1, other1.values1, other2.values1),
                Shift::fma<T>(this->values2, other1.values2, other2.values2),
                Shift::fma<T>(this->values3, other1.values3, other2.values3));
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
    XS_INLINE SIMD4 mad(const BaseDef other1, const SIMD4& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD4(_mm_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD4(_mm_add_ps(_mm_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_add_epi32(_mm_mullo_epi32(this->values, other1.values), other2.values));
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_add_epi16(_mm_mullo_epi16(this->values, other1.values), other2.values));
        } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128i ret =
                _mm_add_epi16(_mm_mullo_epi16(_mm_cvtepu8_epi16(this->values), _mm_cvtepu8_epi16(other1.values)),
                    _mm_cvtepu8_epi16(other2.values));
            return SIMD4(_mm_packus_epi16(ret, ret));
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128i ret =
                _mm_add_epi16(_mm_mullo_epi16(_mm_cvtepi8_epi16(this->values), _mm_cvtepi8_epi16(other1.values)),
                    _mm_cvtepi8_epi16(other2.values));
            return SIMD4(_mm_packs_epi16(ret, ret));
        } else
#endif
        {
            return SIMD4(Shift::fma<T>(this->values0, other1.value, other2.values0),
                Shift::fma<T>(this->values1, other1.value, other2.values1),
                Shift::fma<T>(this->values2, other1.value, other2.values2),
                Shift::fma<T>(this->values3, other1.value, other2.values3));
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
    XS_INLINE SIMD4 mad(const SIMD4& other1, const BaseDef other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD4(_mm_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD4(_mm_add_ps(_mm_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_add_epi32(_mm_mullo_epi32(this->values, other1.values), other2.values));
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_add_epi16(_mm_mullo_epi16(this->values, other1.values), other2.values));
        } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128i ret =
                _mm_add_epi16(_mm_mullo_epi16(_mm_cvtepu8_epi16(this->values), _mm_cvtepu8_epi16(other1.values)),
                    _mm_cvtepu8_epi16(other2.values));
            return SIMD4(_mm_packus_epi16(ret, ret));
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128i ret =
                _mm_add_epi16(_mm_mullo_epi16(_mm_cvtepi8_epi16(this->values), _mm_cvtepi8_epi16(other1.values)),
                    _mm_cvtepi8_epi16(other2.values));
            return SIMD4(_mm_packs_epi16(ret, ret));
        } else
#endif
        {
            return SIMD4(Shift::fma<T>(this->values0, other1.values0, other2.value),
                Shift::fma<T>(this->values1, other1.values1, other2.value),
                Shift::fma<T>(this->values2, other1.values2, other2.value),
                Shift::fma<T>(this->values3, other1.values3, other2.value));
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
    XS_INLINE SIMD4 msub(const SIMD4& other1, const SIMD4& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD4(_mm_fmsub_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD4(_mm_sub_ps(_mm_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_sub_epi32(_mm_mullo_epi32(this->values, other1.values), other2.values));
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_sub_epi16(_mm_mullo_epi16(this->values, other1.values), other2.values));
        } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128i ret =
                _mm_sub_epi16(_mm_mullo_epi16(_mm_cvtepu8_epi16(this->values), _mm_cvtepu8_epi16(other1.values)),
                    _mm_cvtepu8_epi16(other2.values));
            return SIMD4(_mm_packus_epi16(ret, ret));
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128i ret =
                _mm_sub_epi16(_mm_mullo_epi16(_mm_cvtepi8_epi16(this->values), _mm_cvtepi8_epi16(other1.values)),
                    _mm_cvtepi8_epi16(other2.values));
            return SIMD4(_mm_packs_epi16(ret, ret));
        } else
#endif
        {
            return SIMD4(Shift::fma<T>(this->values0, other1.values0, -other2.values0),
                Shift::fma<T>(this->values1, other1.values1, -other2.values1),
                Shift::fma<T>(this->values2, other1.values2, -other2.values2),
                Shift::fma<T>(this->values3, other1.values3, -other2.values3));
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
    XS_INLINE SIMD4 msub(const BaseDef other1, const SIMD4& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD4(_mm_fmsub_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD4(_mm_sub_ps(_mm_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_sub_epi32(_mm_mullo_epi32(this->values, other1.values), other2.values));
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_sub_epi16(_mm_mullo_epi16(this->values, other1.values), other2.values));
        } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128i ret =
                _mm_sub_epi16(_mm_mullo_epi16(_mm_cvtepu8_epi16(this->values), _mm_cvtepu8_epi16(other1.values)),
                    _mm_cvtepu8_epi16(other2.values));
            return SIMD4(_mm_packus_epi16(ret, ret));
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128i ret =
                _mm_sub_epi16(_mm_mullo_epi16(_mm_cvtepi8_epi16(this->values), _mm_cvtepi8_epi16(other1.values)),
                    _mm_cvtepi8_epi16(other2.values));
            return SIMD4(_mm_packs_epi16(ret, ret));
        } else
#endif
        {
            return SIMD4(Shift::fma<T>(this->values0, other1.value, -other2.values0),
                Shift::fma<T>(this->values1, other1.value, -other2.values1),
                Shift::fma<T>(this->values2, other1.value, -other2.values2),
                Shift::fma<T>(this->values3, other1.value, -other2.values3));
        }
    }

    /**
     * Multiply this object negated by another and then add another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_INLINE SIMD4 nmad(const SIMD4& other1, const SIMD4& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD4(_mm_fnmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD4(_mm_sub_ps(other2.values, _mm_mul_ps(this->values, other1.values)));
            }
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_sub_epi32(other2.values, _mm_mullo_epi32(this->values, other1.values)));
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_sub_epi16(other2.values, _mm_mullo_epi16(this->values, other1.values)));
        } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128i ret = _mm_sub_epi16(_mm_cvtepu8_epi16(other2.values),
                _mm_mullo_epi16(_mm_cvtepu8_epi16(this->values), _mm_cvtepu8_epi16(other1.values)));
            return SIMD4(_mm_packus_epi16(ret, ret));
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128i ret = _mm_sub_epi16(_mm_cvtepi8_epi16(other2.values),
                _mm_mullo_epi16(_mm_cvtepi8_epi16(this->values), _mm_cvtepi8_epi16(other1.values)));
            return SIMD4(_mm_packs_epi16(ret, ret));
        } else
#endif
        {
            return SIMD4(Shift::fma<T>(-this->values0, other1.values0, other2.values0),
                Shift::fma<T>(-this->values1, other1.values1, other2.values1),
                Shift::fma<T>(-this->values2, other1.values2, other2.values2),
                Shift::fma<T>(-this->values3, other1.values3, other2.values3));
        }
    }

    /**
     * Multiply this object negated by another and then subtract another object.
     * @tparam EvenIfNotFree Perform a fused operation even if it has higher latency the single operations.
     * @param other1 Second object to multiply by.
     * @param other2 Third object to add.
     * @returns Result of operation.
     */
    template<bool EvenIfNotFree = true>
    XS_INLINE SIMD4 nmsub(const SIMD4& other1, const SIMD4& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD4(_mm_fnmsub_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD4(
                    _mm_sub_ps(_mm_xor_ps(other2.values, _mm_set1_ps(-0.0f)), _mm_mul_ps(this->values, other1.values)));
            }
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_sub_epi32(
                _mm_sub_epi32(_mm_setzero_si128(), other2.values), _mm_mullo_epi32(this->values, other1.values)));
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_sub_epi16(
                _mm_sub_epi16(_mm_setzero_si128(), other2.values), _mm_mullo_epi16(this->values, other1.values)));
        } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128i ret = _mm_sub_epi16(_mm_sub_epi16(_mm_setzero_si128(), _mm_cvtepu8_epi16(other2.values)),
                _mm_mullo_epi16(_mm_cvtepu8_epi16(this->values), _mm_cvtepu8_epi16(other1.values)));
            return SIMD4(_mm_packus_epi16(ret, ret));
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128i ret = _mm_sub_epi16(_mm_sub_epi16(_mm_setzero_si128(), _mm_cvtepi8_epi16(other2.values)),
                _mm_mullo_epi16(_mm_cvtepi8_epi16(this->values), _mm_cvtepi8_epi16(other1.values)));
            return SIMD4(_mm_packs_epi16(ret, ret));
        } else
#endif
        {
            return SIMD4(Shift::fma<T>(-this->values0, other1.values0, -other2.values0),
                Shift::fma<T>(-this->values1, other1.values1, -other2.values1),
                Shift::fma<T>(-this->values2, other1.values2, -other2.values2),
                Shift::fma<T>(-this->values3, other1.values3, -other2.values3));
        }
    }

    /**
     * Alternately subtract and add 2 objects.
     * @param other The second object.
     * @returns The result of the operation.
     */
    XS_INLINE SIMD4 subAdd(const SIMD4& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_addsub_ps(this->values, other.values));
        } else
#endif
        {
            return SIMD4(this->values0 - other.values0, this->values1 + other.values1, this->values2 - other.values2,
                this->values3 + other.values3);
        }
    }

    /**
     * Compare two objects are equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask equalMask(const SIMD4& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
            return Mask(this->values0 == other.values0, this->values1 == other.values1, this->values2 == other.values2,
                this->values3 == other.values3);
        }
    }

    /**
     * Compare two objects are less or equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask lessOrEqualMask(const SIMD4& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
            return Mask(this->values0 <= other.values0, this->values1 <= other.values1, this->values2 <= other.values2,
                this->values3 <= other.values3);
        }
    }

    /**
     * Compare two objects are less than.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask lessThanMask(const SIMD4& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
            return Mask(this->values0 < other.values0, this->values1 < other.values1, this->values2 < other.values2,
                this->values3 < other.values3);
        }
    }

    /**
     * Compare two objects are not equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask notEqualMask(const SIMD4& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
            return Mask(this->values0 != other.values0, this->values1 != other.values1, this->values2 != other.values2,
                this->values3 != other.values3);
        }
    }

    /**
     * Compare two objects are equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask equalMask(const BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
            return Mask(this->values0 == other.value, this->values1 == other.value, this->values2 == other.value,
                this->values3 == other.value);
        }
    }

    /**
     * Compare two objects are less or equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask lessOrEqualMask(const BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
            return Mask(this->values0 <= other.value, this->values1 <= other.value, this->values2 <= other.value,
                this->values3 <= other.value);
        }
    }

    /**
     * Compare two objects are less than.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask lessThanMask(const BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
            return Mask(this->values0 < other.value, this->values1 < other.value, this->values2 < other.value,
                this->values3 < other.value);
        }
    }

    /**
     * Compare two objects are greater or equal.
     * @param other The object to compare to the object.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask greaterOrEqualMask(const BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
            return Mask(this->values0 >= other.value, this->values1 >= other.value, this->values2 >= other.value,
                this->values3 >= other.value);
        }
    }

    /**
     * Compare two objects are greater than.
     * @param other The object to compare to the object.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask greaterThanMask(const BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
            return Mask(this->values0 > other.value, this->values1 > other.value, this->values2 > other.value,
                this->values3 > other.value);
        }
    }

    /**
     * Compare two objects are not equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_INLINE Mask notEqualMask(const BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
            return Mask(this->values0 != other.value, this->values1 != other.value, this->values2 != other.value,
                this->values3 != other.value);
        }
    }

    /**
     * Negate this object based on the sign of another.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @param other The second object whose sign to check.
     * @returns Result of operation.
     */
    XS_INLINE SIMD4 sign(const SIMD4& other) const noexcept
    {
        if constexpr (isUnsigned<T>) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_xor_ps(this->values, _mm_and_ps(_mm_set1_ps(-0.0f), other.values)));
        } else if constexpr (isSame<T, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_sign_epi32(this->values, other.values));
        } else if constexpr (isSame<T, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_sign_epi16(this->values, other.values));
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_sign_epi8(this->values, other.values));
        }
#endif
        else {
            return SIMD4(Shift::sign<T>(this->values0, other.values0), Shift::sign<T>(this->values1, other.values1),
                Shift::sign<T>(this->values2, other.values2), Shift::sign<T>(this->values3, other.values3));
        }
    }

    /**
     * Negate this object based on the sign of another.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @tparam Elem0 Boolean indicating if first element should be modified.
     * @tparam Elem1 Boolean indicating if second element should be modified.
     * @tparam Elem2 Boolean indicating if third element should be modified.
     * @tparam Elem3 Boolean indicating if fourth element should be modified.
     * @param other The second object whose sign to check.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3>
    XS_INLINE SIMD4 sign(const SIMD4& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
            return *this;
        } else if constexpr (isUnsigned<T>) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128 mask = _mm_and_ps(
                _mm_set_ps(Elem3 ? -0.0f : 0.0f, Elem2 ? -0.0f : 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f),
                other.values);
            return SIMD4(_mm_xor_ps(this->values, mask));
        } else if constexpr (isSame<T, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_sign_epi32(this->values, other.values));
        } else if constexpr (isSame<T, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_sign_epi16(this->values, other.values));
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_sign_epi8(this->values, other.values));
        }
#endif
        else {
            return SIMD4(Elem0 ? Shift::sign<T>(this->values0, other.values0) : this->values0,
                Elem1 ? Shift::sign<T>(this->values1, other.values1) : this->values1,
                Elem2 ? Shift::sign<T>(this->values2, other.values2) : this->values2,
                Elem3 ? Shift::sign<T>(this->values3, other.values3) : this->values3);
        }
    }

    /**
     * Negate this object based on the sign of another.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @param other The second object whose sign to check.
     * @returns Result of operation.
     */
    XS_INLINE SIMD4 sign(const BaseDef other) const noexcept
    {
        if constexpr (isUnsigned<T>) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128 mask = _mm_and_ps(_mm_set1_ps(-0.0f), other.values);
            return SIMD4(_mm_xor_ps(this->values, mask));
        } else if constexpr (isSame<T, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_sign_epi32(this->values, other.values));
        } else if constexpr (isSame<T, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_sign_epi16(this->values, other.values));
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_sign_epi8(this->values, other.values));
        }
#endif
        else {
            return SIMD4(Shift::sign<T>(this->values0, this->value), Shift::sign<T>(this->values1, this->value),
                Shift::sign<T>(this->values2, this->value), Shift::sign<T>(this->values3, this->value));
        }
    }

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_INLINE SIMD4 abs() const noexcept
    {
        if constexpr (isUnsigned<T>) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_andnot_ps(_mm_set1_ps(-0.0f), this->values));
        } else if constexpr (isSame<T, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_abs_epi32(this->values));
        } else if constexpr (isSame<T, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_abs_epi16(this->values));
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_abs_epi8(this->values));
        }
#endif
        else {
            return SIMD4(Shift::abs<T>(this->values0), Shift::abs<T>(this->values1), Shift::abs<T>(this->values2),
                Shift::abs<T>(this->values3));
        }
    }

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_INLINE SIMD4 max(const SIMD4& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_ps(this->values, other.values));
        } else if constexpr (isSame<T, uint32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_epu32(this->values, other.values));
        } else if constexpr (isSame<T, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_epi32(this->values, other.values));
        } else if constexpr (isSame<T, uint16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_epu16(this->values, other.values));
        } else if constexpr (isSame<T, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_epi16(this->values, other.values));
        } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_epu8(this->values, other.values));
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_epi8(this->values, other.values));
        } else
#endif
        {
            return SIMD4(Shift::max<T>(this->values0, other.values0), Shift::max<T>(this->values1, other.values1),
                Shift::max<T>(this->values2, other.values2), Shift::max<T>(this->values3, other.values3));
        }
    }

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_INLINE SIMD4 min(const SIMD4& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_min_ps(this->values, other.values));
        } else if constexpr (isSame<T, uint32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_min_epu32(this->values, other.values));
        } else if constexpr (isSame<T, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_min_epi32(this->values, other.values));
        } else if constexpr (isSame<T, uint16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_min_epu16(this->values, other.values));
        } else if constexpr (isSame<T, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_min_epi16(this->values, other.values));
        } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_min_epu8(this->values, other.values));
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_min_epi8(this->values, other.values));
        } else
#endif
        {
            return SIMD4(Shift::min<T>(this->values0, other.values0), Shift::min<T>(this->values1, other.values1),
                Shift::min<T>(this->values2, other.values2), Shift::min<T>(this->values3, other.values3));
        }
    }

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_INLINE SIMD4 max(const BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_ps(this->values, other.values));
        } else if constexpr (isSame<T, uint32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_epu32(this->values, other.values));
        } else if constexpr (isSame<T, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_epi32(this->values, other.values));
        } else if constexpr (isSame<T, uint16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_epu16(this->values, other.values));
        } else if constexpr (isSame<T, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_epi16(this->values, other.values));
        } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_epu8(this->values, other.values));
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_epi8(this->values, other.values));
        } else
#endif
        {
            return SIMD4(Shift::max<T>(this->values0, other.value), Shift::max<T>(this->values1, other.value),
                Shift::max<T>(this->values2, other.value), Shift::max<T>(this->values3, other.value));
        }
    }

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_INLINE SIMD4 min(const BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_min_ps(this->values, other.values));
        } else if constexpr (isSame<T, uint32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_min_epu32(this->values, other.values));
        } else if constexpr (isSame<T, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_min_epi32(this->values, other.values));
        } else if constexpr (isSame<T, uint16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_min_epu16(this->values, other.values));
        } else if constexpr (isSame<T, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_min_epi16(this->values, other.values));
        } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_min_epu8(this->values, other.values));
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_min_epi8(this->values, other.values));
        } else
#endif
        {
            return SIMD4(Shift::min<T>(this->values0, other.value), Shift::min<T>(this->values1, other.value),
                Shift::min<T>(this->values2, other.value), Shift::min<T>(this->values3, other.value));
        }
    }

    /**
     * Clamp a value between 2 other values.
     * @param min The minimum allowed value to clamp to.
     * @param max The maximum allowed value to clamp to.
     * @returns The clamped value.
     */
    XS_INLINE SIMD4 clamp(const BaseDef min, const BaseDef max) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_ps(_mm_min_ps(this->values, max.values), min.values));
        } else if constexpr (isSame<T, uint32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_epu32(_mm_min_epu32(this->values, max.values), min.values));
        } else if constexpr (isSame<T, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_epi32(_mm_min_epi32(this->values, max.values), min.values));
        } else if constexpr (isSame<T, uint16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_epu16(_mm_min_epu16(this->values, max.values), min.values));
        } else if constexpr (isSame<T, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_epi16(_mm_min_epi16(this->values, max.values), min.values));
        } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_epu8(_mm_min_epu8(this->values, max.values), min.values));
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_max_epi8(_mm_min_epi8(this->values, max.values), min.values));
        } else
#endif
        {
            return SIMD4(Shift::max<T>(Shift::min<T>(this->values0, max.value), min.value),
                Shift::max<T>(Shift::min<T>(this->values1, max.value), min.value),
                Shift::max<T>(Shift::min<T>(this->values2, max.value), min.value),
                Shift::max<T>(Shift::min<T>(this->values3, max.value), min.value));
        }
    }

    /**
     * Maximum of each internally stored SIMD2.
     * @returns SIMD2 set to the largest value in each internal SIMD2.
     */
    XS_INLINE SIMD2Def max2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD2Def(_mm_max_ps(_mm_movehl_ps(this->values, this->values), this->values));
        } else
#endif
        {
            return SIMD2Def(Shift::max<T>(this->values0, this->values2), Shift::max<T>(this->values1, this->values3));
        }
    }

    /**
     * Minimum of each internally stored SIMD2.
     * @returns SIMD2 set to the largest value in each internal SIMD2.
     */
    XS_INLINE SIMD2Def min2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD2Def(_mm_min_ps(_mm_movehl_ps(this->values, this->values), this->values));
        } else
#endif
        {
            return SIMD2Def(Shift::min<T>(this->values0, this->values2), Shift::min<T>(this->values1, this->values3));
        }
    }

    /**
     * Maximum element of this object.
     * @returns SIMDBase set to the largest value.
     */
    XS_INLINE BaseDef hmax() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128 other = _mm_max_ps(_mm_shuffle1_ps(this->values, _MM_SHUFFLE(1, 0, 3, 2)), this->values);
            return BaseDef(_mm_max_ps(other, _mm_shuffle1_ps(other, _MM_SHUFFLE(2, 3, 0, 1))));
        } else
#endif
        {
            const T other = Shift::max<T>(this->values0, this->values1);
            const T other2 = Shift::max<T>(this->values2, this->values3);
            return BaseDef(Shift::max<T>(other2, other));
        }
    }

    /**
     * Minimum element of this object.
     * @returns SIMDBase set to the smallest value.
     */
    XS_INLINE BaseDef hmin() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128 other = _mm_min_ps(_mm_shuffle1_ps(this->values, _MM_SHUFFLE(1, 0, 3, 2)), this->values);
            return BaseDef(_mm_min_ps(other, _mm_shuffle1_ps(other, _MM_SHUFFLE(2, 3, 0, 1))));
        } else
#endif
        {
            const T other = Shift::min<T>(this->values0, this->values1);
            const T other2 = Shift::min<T>(this->values2, this->values3);
            return BaseDef(Shift::min<T>(other2, other));
        }
    }

    /**
     * Maximum element of this object.
     * @returns SIMDInBase set to the largest value.
     */
    XS_INLINE InBaseDef hmaxInBase() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128 other = _mm_max_ps(_mm_movehl_ps(this->values, this->values), this->values); //(x,x,3,2)
            return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(other), other));                           //(x,x,x,1)
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
    XS_INLINE InBaseDef hminInBase() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128 other = _mm_min_ps(_mm_movehl_ps(this->values, this->values), this->values); //(x,x,3,2)
            return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(other), other));                           //(x,x,x,1)
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
    XS_INLINE BaseDef hmaxIndex(uint32& index) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            __m128 other = _mm_max_ps(_mm_shuffle1_ps(this->values, _MM_SHUFFLE(1, 0, 3, 2)), this->values);
            other = _mm_max_ps(other, _mm_shuffle1_ps(other, _MM_SHUFFLE(2, 3, 0, 1)));
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
            T other = Shift::max<T>(this->values0, this->values1);
            index = (other == this->values0) ? 0 : 1;
            const T other2 = Shift::max<T>(this->values2, this->values3);
            const uint32 index2 = (other2 == this->values2) ? 2 : 3;
            other = Shift::max<T>(other, other2);
            index = (other2 == other) ? index2 : index;
            return BaseDef(other);
        }
    }

    /**
     * Minimum element of this object along with the max elements index.
     * @param [out] index Return value used to pass back the index of the returned element.
     * @returns SIMDBase set to the smallest value.
     */
    XS_INLINE BaseDef hminIndex(uint32& index) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            __m128 other = _mm_min_ps(_mm_shuffle1_ps(this->values, _MM_SHUFFLE(1, 0, 3, 2)), this->values);
            other = _mm_min_ps(other, _mm_shuffle1_ps(other, _MM_SHUFFLE(2, 3, 0, 1)));
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
            T other = Shift::min<T>(this->values0, this->values1);
            index = (other == this->values0) ? 0 : 1;
            const T other2 = Shift::min<T>(this->values2, this->values3);
            const uint32 index2 = (other2 == this->values2) ? 2 : 3;
            other = Shift::min<T>(other, other2);
            index = (other2 == other) ? index2 : index;
            return BaseDef(other);
        }
    }

    /**
     * Maximum element from each SIMD2.
     * @returns SIMD2 set to the largest value in each SIMD2.
     */
    XS_INLINE SIMD2Def hmax2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128 val0 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(2, 0, 3, 1)); //(x,x,3,1)
            const __m128 val1 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(3, 1, 2, 0)); //(x,x,2,0)
            return SIMD2Def(_mm_max_ps(val0, val1));
        } else
#endif
        {
            return SIMD2Def(Shift::max<T>(this->values0, this->values1), Shift::max<T>(this->values2, this->values3));
        }
    }

    /**
     * Minimum element from each SIMD2.
     * @returns SIMD2 set to the smallest value in each SIMD2.
     */
    XS_INLINE SIMD2Def hmin2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128 val0 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(2, 0, 3, 1)); //(x,x,3,1)
            const __m128 val1 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(3, 1, 2, 0)); //(x,x,2,0)
            return SIMD2Def(_mm_min_ps(val0, val1));
        } else
#endif
        {
            return SIMD2Def(Shift::min<T>(this->values0, this->values1), Shift::min<T>(this->values2, this->values3));
        }
    }

    /**
     * Compute the sum of each internally stored SIMD2.
     * @returns SIMD2 set to the sum of all elements between each internal SIMD2.
     */
    XS_INLINE SIMD2Def add2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD2Def(_mm_add_ps(_mm_movehl_ps(this->values, this->values), this->values));
        } else
#endif
        {
            return SIMD2Def(this->values0 + this->values2, this->values1 + this->values3);
        }
    }

    /**
     * Compute the difference of each internally stored SIMD2.
     * @returns SIMD2 set to the difference of all elements between each internal SIMD2.
     */
    XS_INLINE SIMD2Def sub2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD2Def(_mm_sub_ps(this->values, _mm_movehl_ps(this->values, this->values)));
        } else
#endif
        {
            return SIMD2Def(this->values0 - this->values2, this->values1 - this->values3);
        }
    }

    /**
     * Compute the sum of all elements.
     * @returns BaseDef set to the sum of all elements.
     */
    XS_INLINE BaseDef hadd() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            __m128 val2 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(2, 3, 0, 1)); //(2,3,0,1)
            val2 = _mm_add_ps(val2, this->values);
            const __m128 val3 = _mm_shuffle1_ps(val2, _MM_SHUFFLE(1, 1, 3, 3));
            return BaseDef(_mm_add_ps(val2, val3));
        } else
#endif
        {
            T res = (this->values0 + this->values1);
            const T res2 = (this->values2 + this->values3);
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
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128 other = _mm_add_ps(_mm_movehl_ps(this->values, this->values), this->values); //(x,x,3,2)
            return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(other), other));                           //(x,x,x,1)
        } else
#endif
        {
            return InBaseDef(hadd());
        }
    }

    /**
     * Compute the sum of each SIMD2.
     * @returns SIMD2 set to the sum of all elements in each SIMD2.
     */
    XS_INLINE SIMD2Def hadd2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128 val0 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(2, 0, 3, 1)); //(x,x,3,1)
            const __m128 val1 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(3, 1, 2, 0)); //(x,x,2,0)
            return SIMD2Def(_mm_add_ps(val0, val1));
        } else
#endif
        {
            return SIMD2Def(this->values0 + this->values1, this->values2 + this->values3);
        }
    }

    /**
     * Compute the difference of each SIMD2.
     * @returns SIMD2 set to the difference of all elements in each SIMD2.
     */
    XS_INLINE SIMD2Def hsub2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128 val0 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(3, 1, 2, 0)); //(x,x,2,0)
            const __m128 val1 = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(2, 0, 3, 1)); //(x,x,3,1)
            return SIMD2Def(_mm_sub_ps(val0, val1));
        } else
#endif
        {
            return SIMD2Def(this->values0 - this->values1, this->values2 - this->values3);
        }
    }

    /**
     * Compute the 4 dimensional dot product of two objects.
     * @param other The second object.
     * @returns The value of the dot product.
     */
    XS_INLINE BaseDef dot4(const SIMD4& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return BaseDef(_mm_dp_ps(this->values, other.values, 0xFF));
        } else
#endif
        {
            T res = (this->values0 * other.values0);
            const T res2 = (this->values1 * other.values1);
            T res3 = (this->values2 * other.values2);
            const T res4 = (this->values3 * other.values3);
            res += res2;
            res3 += res4;
            return BaseDef(res + res3);
        }
    }

    /**
     * Compute the 4 dimensional dot product of two objects.
     * @param other The second object.
     * @returns The value of the dot product stored in a SIMDInBase.
     */
    XS_INLINE InBaseDef dot4InBase(const SIMD4& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return InBaseDef(_mm_dp_ps(this->values, other.values, 0xF1));
        } else
#endif
        {
            return InBaseDef(dot4(other));
        }
    }

    /**
     * Compute the square of the length.
     * @returns Squared length.
     */
    XS_INLINE BaseDef lengthSqr() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return BaseDef(_mm_dp_ps(this->values, this->values, 0xFF));
        } else
#endif
        {
            T res = (this->values0 * this->values0);
            const T res2 = (this->values1 * this->values1);
            T res3 = (this->values2 * this->values2);
            const T res4 = (this->values3 * this->values3);
            res += res2;
            res3 += res4;
            return BaseDef(res + res3);
        }
    }

    /**
     * Compute the length.
     * @returns The length.
     */
    XS_INLINE BaseDef length() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return BaseDef(_mm_sqrt_ps(_mm_dp_ps(this->values, this->values, 0xFF)));
        } else
#endif
        {
            return BaseDef(Shift::sqrt<T>(lengthSqr().value));
        }
    }

    /**
     * Compute the square of the length.
     * @returns Squared length stored in SIMDInBase.
     */
    XS_INLINE InBaseDef lengthSqrInBase() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return InBaseDef(_mm_dp_ps(this->values, this->values, 0xF1));
        } else
#endif
        {
            return InBaseDef(dot4(*this));
        }
    }

    /**
     * Compute the length.
     * @returns The length stored in SIMDInBase.
     */
    XS_INLINE InBaseDef lengthInBase() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return InBaseDef(_mm_sqrt_ss(_mm_dp_ps(this->values, this->values, 0xF1)));
        } else
#endif
        {
            return InBaseDef(length().value);
        }
    }

    /**
     * Normalise each value.
     * @note The result is unpredictable when all elements are at or near zero.
     * @returns The normalised values.
     */
    XS_INLINE SIMD4 normalize() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128 value = _mm_sqrt_ps(_mm_dp_ps(this->values, this->values, 0xFF));
            return SIMD4(_mm_div_ps(this->values, value));
        } else
#endif
        {
            return *this / length();
        }
    }

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index.
     * @tparam Index0 The index in the first object to insert the element (range is 0-3).
     * @tparam Index1 The index in the second object to copy element from (range is 0-3).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_INLINE SIMD4 insert(const SIMD4& other) const noexcept
    {
        static_assert(Index0 < 4 && Index1 < 4, "Invalid Index: Indexes must be <4");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD4(_mm_blend_ss(this->values, other.values));
            } else if constexpr (Index0 == Index1) {
                return SIMD4(_mm_blend_ps(this->values, other.values, 1 << Index0));
            } else {
                return SIMD4(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)));
            }
        } else
#endif
        {
            return SIMD4(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
                Index0 != 1 ? this->values1 : (&other.values0)[Index1],
                Index0 != 2 ? this->values2 : (&other.values0)[Index1],
                Index0 != 3 ? this->values3 : (&other.values0)[Index1]);
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
    XS_INLINE SIMD4 insert2(const SIMD4& other) const noexcept
    {
        static_assert(Index0 < 2 && Index1 < 2, "Invalid Index: Indexes must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD4(_mm_blend_ps(this->values, other.values, _MM_BLEND(0, 1, 0, 1)));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD4(_mm_blend_ps(this->values, other.values, _MM_BLEND(1, 0, 1, 0)));
            } else {
                const __m128 val = _mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index1, Index0, 0));
                return SIMD4(_mm_insert_ps(val, other.values, _MM_MK_INSERTPS_NDX(Index1 + 2, Index0 + 2, 0)));
            }
        } else
#endif
        {
            return SIMD4(Index0 != 0 ? this->values0 : (&other.values0)[Index1],
                Index0 != 1 ? this->values1 : (&other.values0)[Index1],
                Index0 != 0 ? this->values2 : (&other.values0)[Index1 + 2],
                Index0 != 1 ? this->values3 : (&other.values0)[Index1 + 2]);
        }
    }

    /**
     * Blends values from the second object into the first based on a mask.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @tparam Elem2 Whether to copy the third element of the first input into the returned object.
     * @tparam Elem3 Whether to copy the fourth element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns Result of operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3>
    XS_INLINE SIMD4 blend(const SIMD4& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
            return *this;
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
            return other;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
                return SIMD4(_mm_blend_ss(this->values, other.values));
            } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
                return SIMD4(_mm_blend_ss(other.values, this->values));
            } else {
                return SIMD4(_mm_blend_ps(this->values, other.values, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)));
            }
        }
#endif
        else {
            return SIMD4(Elem0 ? other.values0 : this->values0, Elem1 ? other.values1 : this->values1,
                Elem2 ? other.values2 : this->values2, Elem3 ? other.values3 : this->values3);
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
    XS_INLINE SIMD4 blendVar(const SIMD4& other, const Mask& mask) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return SIMD4(_mm_mask_blend_ps(mask.values, this->values, other.values));
            } else {
                return SIMD4(_mm_blendv_ps(this->values, other.values, mask.values));
            }
        } else
#endif
        {
            return SIMD4((mask.values0) ? other.values0 : this->values0, (mask.values1) ? other.values1 : this->values1,
                (mask.values2) ? other.values2 : this->values2, (mask.values3) ? other.values3 : this->values3);
        }
    }

    /**
     * Swaps values between the second object and the first based on a mask.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to swap the first elements.
     * @tparam Elem1 Whether to swap the second elements.
     * @tparam Elem2 Whether to swap the third elements.
     * @tparam Elem3 Whether to swap the fourth elements.
     * @param [in,out] other The object to swap elements with the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1, bool Elem2, bool Elem3>
    XS_INLINE SIMD4 blendSwap(SIMD4& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1 && !Elem2 && !Elem3) {
            return *this;
        } else if constexpr (Elem0 && Elem1 && Elem2 && Elem3) {
            const SIMD4 backup = other;
            other = *this;
            return backup;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Elem0 && !Elem1 && !Elem2 && !Elem3) {
                const __m128 backup = other.values;
                other.values = _mm_blend_ss(other.values, this->values);
                return SIMD4(_mm_blend_ss(this->values, backup));
            } else if constexpr (!Elem0 && Elem1 && Elem2 && Elem3) {
                const __m128 backup = other.values;
                other.values = _mm_blend_ss(this->values, other.values);
                return SIMD4(_mm_blend_ss(backup, this->values));
            } else {
                const __m128 backup = other.values;
                other.values = _mm_blend_ps(other.values, this->values, _MM_BLEND(Elem3, Elem2, Elem1, Elem0));
                return SIMD4(_mm_blend_ps(this->values, backup, _MM_BLEND(Elem3, Elem2, Elem1, Elem0)));
            }
        }
#endif
        else {
            const SIMD4 backup = other;
            other.values0 = (Elem0) ? this->values0 : other.values0;
            other.values1 = (Elem1) ? this->values1 : other.values1;
            other.values2 = (Elem2) ? this->values2 : other.values2;
            other.values3 = (Elem3) ? this->values3 : other.values3;
            return SIMD4((Elem0) ? backup.values0 : this->values0, (Elem1) ? backup.values1 : this->values1,
                (Elem2) ? backup.values2 : this->values2, (Elem3) ? backup.values3 : this->values3);
        }
    }

    /**
     * Swaps values between the second object and the first based on a dynamic mask.
     * @note A value form the second object is swapped with the first based on the corresponding input elements.
     * @param [in,out] other The object to swap elements with the first.
     * @param          mask The object mask used to define which elements to swap.
     * @returns The result of the operation.
     */
    XS_INLINE SIMD4 blendSwapVar(SIMD4& other, const Mask& mask) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                const __m128 backup = other.values;
                other.values = _mm_mask_blend_ps(mask.values, other.values, this->values);
                return SIMD4(_mm_mask_blend_ps(mask.values, this->values, backup));
            } else {
                const __m128 backup = other.values;
                other.values = _mm_blendv_ps(other.values, this->values, mask.values);
                return SIMD4(_mm_blendv_ps(this->values, backup, mask.values));
            }
        } else
#endif
        {
            const SIMD4 backup = other;
            other.values0 = (mask.values0) ? this->values0 : other.values0;
            other.values1 = (mask.values1) ? this->values1 : other.values1;
            other.values2 = (mask.values2) ? this->values2 : other.values2;
            other.values3 = (mask.values3) ? this->values3 : other.values3;
            return SIMD4((mask.values0) ? backup.values0 : this->values0,
                (mask.values1) ? backup.values1 : this->values1, (mask.values2) ? backup.values2 : this->values2,
                (mask.values3) ? backup.values3 : this->values3);
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
    XS_INLINE SIMD4 blend2(const SIMD4& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1) {
            return *this;
        } else if constexpr (Elem0 && Elem1) {
            return other;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMD4(_mm_blend_ps(this->values, other.values, _MM_BLEND(Elem1, Elem0, Elem1, Elem0)));
        }
#endif
        else {
            return SIMD4(Elem0 ? other.values0 : this->values0, Elem1 ? other.values1 : this->values1,
                Elem0 ? other.values2 : this->values2, Elem1 ? other.values3 : this->values3);
        }
    }

    /**
     * Shuffles the elements of the object.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-3).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-3).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-3).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-3).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
    XS_INLINE SIMD4 shuffle() const noexcept
    {
        static_assert(Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4, "Invalid Index: Indexes must be <4");
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 0 && Index3 == 1) {
                return SIMD4(_mm_shuffle1010_ps(this->values));
            } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 2 && Index3 == 3) {
                return SIMD4(_mm_shuffle3232_ps(this->values));
            } else if constexpr (Index0 == 2 && Index1 == 2 && Index2 == 3 && Index3 == 3) {
                return SIMD4(_mm_shuffle3322_ps(this->values));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 1 && Index3 == 1) {
                return SIMD4(_mm_shuffle1100_ps(this->values));
            } else if constexpr (Index0 == 0 && Index1 == 0 && Index2 == 2 && Index3 == 2) {
                return SIMD4(_mm_shuffle2200_ps(this->values));
            } else if constexpr (Index0 == 1 && Index1 == 1 && Index2 == 3 && Index3 == 3) {
                return SIMD4(_mm_shuffle3311_ps(this->values));
            } else {
                return SIMD4(_mm_shuffle1_ps(this->values, _MM_SHUFFLE(Index3, Index2, Index1, Index0)));
            }
        }
#endif
        else {
            return SIMD4(
                (&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index2], (&this->values0)[Index3]);
        }
    }

    /**
     * Dynamically shuffles the elements of the object.
     * @param shuffle The shuffle mask used to shuffle.
     * @returns The result of the operation.
     */
    XS_INLINE SIMD4 shuffleVar(const Shuffle& shuffle) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasISAFeature<ISAFeature::AVX>) {
                XS_ASSERT(_mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values, _mm_set1_epi32(3))) == 0);
                return SIMD4(_mm_permutevar_ps(this->values, shuffle.values));
            } else {
                XS_ASSERT(_mm_movemask_epi8(_mm_cmpgt_epi32(shuffle.values, _mm_set1_epi32(0x0F0E0D0C))) == 0);
                return SIMD4(_mm_castsi128_ps(_mm_shuffle_epi8(_mm_castps_si128(this->values), shuffle.values)));
            }
        } else
#endif
        {
            XS_ASSERT(shuffle.values0 < 4 && shuffle.values1 < 4 && shuffle.values2 < 4 && shuffle.values3 < 4);
            return SIMD4((&this->values0)[shuffle.values0], (&this->values0)[shuffle.values1],
                (&this->values0)[shuffle.values2], (&this->values0)[shuffle.values3]);
        }
    }

    /**
     * Shuffles the elements of the object using the same shuffle across each SIMD2.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-1).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-1).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_INLINE SIMD4 shuffle2() const noexcept
    {
        static_assert(Index0 < 2 && Index1 < 2, "Invalid Index: Indexes must be <2");
        if constexpr (Index0 == 0 && Index1 == 1) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD4(_mm_moveldup_ps(this->values));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD4(_mm_movehdup_ps(this->values));
            } else /*Index0 == 1 && Index1 == 0*/ {
                return SIMD4(_mm_shuffle1_ps(this->values, _MM_SHUFFLE(2, 3, 0, 1)));
            }
        }
#endif
        else {
            return SIMD4((&this->values0)[Index0], (&this->values0)[Index1], (&this->values0)[Index0 + 2],
                (&this->values0)[Index1 + 2]);
        }
    }

    /**
     * Shuffles the elements of 2 SIMD4s together to a form a new SIMD4.
     * @note Any index in range 0-3 is taken from first object. Any index in range 4-7 is taken from second object.
     * @tparam Index0 The index of the first element to insert into the returned object (range is 0-7).
     * @tparam Index1 The index of the second element to insert into the returned object (range is 0-7).
     * @tparam Index2 The index of the third element to insert into the returned object (range is 0-7).
     * @tparam Index3 The index of the fourth element to insert into the returned object (range is 0-7).
     * @param other Second input.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1, uint32 Index2, uint32 Index3>
    XS_INLINE SIMD4 combine(const SIMD4& other) const noexcept
    {
        static_assert(Index0 < 8 && Index1 < 8 && Index2 < 8 && Index3 < 8, "Invalid Index: Indexes must be <8");
        if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
            return *this;
        } else if constexpr (Index0 == 4 && Index1 == 5 && Index2 == 6 && Index3 == 7) {
            return other;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (Index0 == 4 && Index1 == 1 && Index2 == 2 && Index3 == 3) {
                return SIMD4(_mm_blend_ss(this->values, other.values));
            } else if constexpr (Index0 == 0 && Index1 == 5 && Index2 == 6 && Index3 == 7) {
                return SIMD4(_mm_blend_ss(other.values, this->values));
            } else if constexpr (Index0 == 6 && Index1 == 7 && Index2 == 2 && Index3 == 3) {
                return SIMD4(_mm_movehl_ps(this->values, other.values));
            } else if constexpr (Index0 == 2 && Index1 == 3 && Index2 == 6 && Index3 == 7) {
                return SIMD4(_mm_movehl_ps(other.values, this->values));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 4 && Index3 == 5) {
                return SIMD4(_mm_movelh_ps(this->values, other.values));
            } else if constexpr (Index0 == 4 && Index1 == 5 && Index2 == 0 && Index3 == 1) {
                return SIMD4(_mm_movelh_ps(other.values, this->values));
            } else if constexpr ((Index0 == 0 || Index0 == 4) && (Index1 == 1 || Index1 == 5) &&
                (Index2 == 2 || Index2 == 6) && (Index3 == 3 || Index3 == 7)) {
                return SIMD4(_mm_blend_ps(
                    this->values, other.values, _MM_BLEND(Index3 >= 4, Index2 >= 4, Index1 >= 4, Index0 >= 4)));
            } else if constexpr (Index0 == 2 && Index1 == 6 && Index2 == 3 && Index3 == 7) {
                return SIMD4(_mm_unpackhi_ps(this->values, other.values));
            } else if constexpr (Index0 == 6 && Index1 == 2 && Index2 == 7 && Index3 == 3) {
                return SIMD4(_mm_unpackhi_ps(other.values, this->values));
            } else if constexpr (Index0 == 0 && Index1 == 4 && Index2 == 1 && Index3 == 5) {
                return SIMD4(_mm_unpacklo_ps(this->values, other.values));
            } else if constexpr (Index0 == 4 && Index1 == 0 && Index2 == 5 && Index3 == 1) {
                return SIMD4(_mm_unpacklo_ps(other.values, this->values));
            } else if constexpr (Index0 < 4 && Index1 < 4 && Index2 < 4 && Index3 < 4) {
                return shuffle<Index0, Index1, Index2, Index3>();
            } else if constexpr (Index0 >= 4 && Index1 >= 4 && Index2 >= 4 && Index3 >= 4) {
                return other.shuffle<Index0 - 4, Index1 - 4, Index2 - 4, Index3 - 4>();
            } else if constexpr (Index0 < 4 && Index1 < 4 && Index2 >= 4 && Index3 >= 4) {
                return SIMD4(
                    _mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index3 - 4, Index2 - 4, Index1, Index0)));
            } else if constexpr (Index0 >= 4 && Index1 >= 4 && Index2 < 4 && Index3 < 4) {
                return SIMD4(
                    _mm_shuffle_ps(other.values, this->values, _MM_SHUFFLE(Index3, Index2, Index1 - 4, Index0 - 4)));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index2 == 2) {
                return SIMD4(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index3 - 4, 3, 0)));
            } else if constexpr (Index0 == 0 && Index1 == 1 && Index3 == 3) {
                return SIMD4(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index2 - 4, 2, 0)));
            } else if constexpr (Index0 == 0 && Index2 == 2 && Index3 == 3) {
                return SIMD4(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index1 - 4, 1, 0)));
            } else if constexpr (Index1 == 1 && Index2 == 2 && Index3 == 3) {
                return SIMD4(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index0 - 4, 0, 0)));
            } else if constexpr (Index0 == 4 && Index1 == 5 && Index2 == 6) {
                return SIMD4(_mm_insert_ps(other.values, this->values, _MM_MK_INSERTPS_NDX(Index3, 3, 0)));
            } else if constexpr (Index0 == 4 && Index1 == 5 && Index3 == 7) {
                return SIMD4(_mm_insert_ps(other.values, this->values, _MM_MK_INSERTPS_NDX(Index2, 2, 0)));
            } else if constexpr (Index0 == 4 && Index2 == 6 && Index3 == 7) {
                return SIMD4(_mm_insert_ps(other.values, this->values, _MM_MK_INSERTPS_NDX(Index1, 1, 0)));
            } else if constexpr (Index1 == 5 && Index2 == 6 && Index3 == 7) {
                return SIMD4(_mm_insert_ps(other.values, this->values, _MM_MK_INSERTPS_NDX(Index0, 0, 0)));
            } else if constexpr (Index0 >= 4 && Index1 < 4 && Index2 < 4 && Index3 >= 4) {
                if constexpr (Index1 == 1 && Index2 == 2) {
                    return SIMD4(_mm_blend_ps(this->values,
                        _mm_shuffle_ps(other.values, other.values, _MM_SHUFFLE(Index3 - 4, 2, 1, Index0 - 4)),
                        _MM_BLEND(1, 0, 0, 1)));
                } else {
                    const __m128 val =
                        _mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index3 - 4, Index0 - 4, Index2, Index1));
                    return SIMD4(_mm_shuffle1_ps(val, _MM_SHUFFLE(3, 1, 0, 2)));
                }
            } else if constexpr (Index0 >= 4 && Index1 < 4 && Index2 >= 4 && Index3 < 4) {
                if constexpr (Index1 == 1 && Index3 == 3) {
                    return SIMD4(_mm_blend_ps(this->values,
                        _mm_shuffle_ps(other.values, other.values, _MM_SHUFFLE(3, Index2 - 4, 1, Index0 - 4)),
                        _MM_BLEND(0, 1, 0, 1)));
                } else {
                    const __m128 val =
                        _mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(Index2 - 4, Index0 - 4, Index3, Index1));
                    return SIMD4(_mm_shuffle1_ps(val, _MM_SHUFFLE(1, 3, 0, 2)));
                }
            } else if constexpr (Index0 >= 4 && Index1 >= 4 && Index2 >= 4 && Index3 < 4) {
                if constexpr (Index3 == 3) {
                    return SIMD4(_mm_blend_ps(this->values,
                        _mm_shuffle_ps(other.values, other.values, _MM_SHUFFLE(3, Index2 - 4, Index1 - 4, Index0 - 4)),
                        _MM_BLEND(0, 1, 1, 1)));
                } else {
                    const __m128 val =
                        _mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(0, Index2 - 4, 2, Index3));
                    return SIMD4(_mm_shuffle_ps(other.values, val, _MM_SHUFFLE(0, 2, Index1 - 4, Index0 - 4)));
                }
            } else if constexpr (Index0 >= 4 && Index1 >= 4 && Index2 < 4 && Index3 >= 4) {
                if constexpr (Index2 == 2) {
                    return SIMD4(_mm_blend_ps(this->values,
                        _mm_shuffle_ps(other.values, other.values, _MM_SHUFFLE(Index3 - 4, 2, Index1 - 4, Index0 - 4)),
                        _MM_BLEND(1, 0, 1, 1)));
                } else {
                    const __m128 val =
                        _mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(0, Index3 - 4, 2, Index2));
                    return SIMD4(_mm_shuffle_ps(other.values, val, _MM_SHUFFLE(2, 0, Index1 - 4, Index0 - 4)));
                }
            } else if constexpr (Index0 >= 4 && Index1 < 4 && Index2 >= 4 && Index3 >= 4) {
                if constexpr (Index1 == 1) {
                    return SIMD4(_mm_blend_ps(this->values,
                        _mm_shuffle_ps(other.values, other.values, _MM_SHUFFLE(Index3 - 4, Index2 - 4, 1, Index0 - 4)),
                        _MM_BLEND(1, 1, 0, 1)));
                } else {
                    const __m128 val =
                        _mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(0, Index0 - 4, 2, Index1));
                    return SIMD4(_mm_shuffle_ps(val, other.values, _MM_SHUFFLE(Index3 - 4, Index2 - 4, 0, 2)));
                }
            } else if constexpr (Index0 < 4 && Index1 >= 4 && Index2 >= 4 && Index3 >= 4) {
                if constexpr (Index0 == 0) {
                    return SIMD4(_mm_blend_ps(this->values,
                        _mm_shuffle_ps(other.values, other.values, _MM_SHUFFLE(Index3 - 4, Index2 - 4, Index1 - 4, 0)),
                        _MM_BLEND(1, 1, 1, 0)));
                } else {
                    const __m128 val =
                        _mm_shuffle_ps(this->values, other.values, _MM_SHUFFLE(0, Index1 - 4, 2, Index0));
                    return SIMD4(_mm_shuffle_ps(val, other.values, _MM_SHUFFLE(Index3 - 4, Index2 - 4, 2, 0)));
                }
            } else {
                // Just use specialisation for inverse arrangement
                return other.combine<Index0 + 4 - ((Index0 >= 4) * 8), Index1 + 4 - ((Index1 >= 4) * 8),
                    Index2 + 4 - ((Index2 >= 4) * 8), Index3 + 4 - ((Index3 >= 4) * 8)>(*this);
            }
        }
#endif
        else {
            return SIMD4(((Index0 < 4) ? &this->values0 : &other.values0)[Index0 % 4],
                ((Index1 < 4) ? &this->values0 : &other.values0)[Index1 % 4],
                ((Index2 < 4) ? &this->values0 : &other.values0)[Index2 % 4],
                ((Index3 < 4) ? &this->values0 : &other.values0)[Index3 % 4]);
        }
    }
};

/**
 * Add two SIMD4s.
 * @param other1 The first SIMD4.
 * @param other2 SIMD4 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator+(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_add_ps(other1.values, other2.values));
    } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_add_epi32(other1.values, other2.values));
    } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_add_epi16(other1.values, other2.values));
    } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_add_epi8(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 + other2.values0), (other1.values1 + other2.values1),
            (other1.values2 + other2.values2), (other1.values3 + other2.values3));
    }
}

/**
 * Add a value to all elements of a SIMD4.
 * @param other1 The SIMD4.
 * @param other2 Value to add to the SIMD4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator+(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_add_ps(other1.values, other2.values));
    } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_add_epi32(other1.values, other2.values));
    } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_add_epi16(other1.values, other2.values));
    } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_add_epi8(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 + other2.value), (other1.values1 + other2.value),
            (other1.values2 + other2.value), (other1.values3 + other2.value));
    }
}

/**
 * Add a SIMD2 to each pair of elements in a other.
 * @param other1 The other.
 * @param other2 Values to add to the other.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator+(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_add_ps(other1.values, _mm_shuffle1010_ps(other2.values)));
    } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_add_epi32(other1.values, other2.values));
    } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_add_epi16(other1.values, other2.values));
    } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_add_epi8(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 + other2.values0), (other1.values1 + other2.values1),
            (other1.values2 + other2.values0), (other1.values3 + other2.values1));
    }
}

/**
 * Subtract a SIMD4 from another SIMD4.
 * @param other1 The first SIMD4.
 * @param other2 SIMD4 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator-(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_sub_epi32(other1.values, other2.values));
    } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_sub_epi16(other1.values, other2.values));
    } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_sub_epi8(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 - other2.values0), (other1.values1 - other2.values1),
            (other1.values2 - other2.values2), (other1.values3 - other2.values3));
    }
}

/**
 * Subtract a value from all elements of a SIMD4.
 * @param other1 The SIMD4.
 * @param other2 Value to subtract from the SIMD4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator-(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_sub_epi32(other1.values, other2.values));
    } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_sub_epi16(other1.values, other2.values));
    } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_sub_epi8(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 - other2.value), (other1.values1 - other2.value),
            (other1.values2 - other2.value), (other1.values3 - other2.value));
    }
}

/**
 * Subtract all elements of a SIMD4 from a value.
 * @param other1 Value to subtract from the SIMD4.
 * @param other2 The SIMD4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator-(
    const typename SIMD4<T, Width>::BaseDef other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_sub_epi32(other1.values, other2.values));
    } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_sub_epi16(other1.values, other2.values));
    } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_sub_epi8(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.value - other2.values0), (other1.value - other2.values1),
            (other1.value - other2.values2), (other1.value - other2.values3));
    }
}

/**
 * Subtract a SIMD2 from each pair of elements in a other.
 * @param other1 The other.
 * @param other2 Values to subtract from the other.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator-(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_sub_ps(other1.values, _mm_shuffle1010_ps(other2.values)));
    } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_sub_epi32(other1.values, _mm_shuffle1010_epi32(other2.values)));
    } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(
            _mm_sub_epi16(other1.values, _mm_shufflelo_epi16(other2.values, _MM_SHUFFLE(1, 0, 1, 0))));
    } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(
            _mm_sub_epi8(other1.values, _mm_shufflelo_epi16(other2.values, _MM_SHUFFLE(0, 0, 0, 0))));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 - other2.values0), (other1.values1 - other2.values1),
            (other1.values2 - other2.values0), (other1.values3 - other2.values1));
    }
}

/**
 * Multiply two SIMD4s per element.
 * @param other1 The first SIMD4.
 * @param other2 The second SIMD4 to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator*(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, uint32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_mullo_epi32(other1.values, other2.values));
    } else if constexpr (isSame<T, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_mullo_epi32(other1.values, other2.values));
    } else if constexpr (isSame<T, uint16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_mullo_epi16(other1.values, other2.values));
    } else if constexpr (isSame<T, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_mullo_epi16(other1.values, other2.values));
    } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        const __m128i ret = _mm_mullo_epi16(_mm_cvtepu8_epi16(other1.values), _mm_cvtepu8_epi16(other2.values));
        return SIMD4<T, Width>(_mm_packus_epi16(ret, ret));
    } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        const __m128i ret = _mm_mullo_epi16(_mm_cvtepi8_epi16(other1.values), _mm_cvtepi8_epi16(other2.values));
        return SIMD4<T, Width>(_mm_packs_epi16(ret, ret));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 * other2.values0), (other1.values1 * other2.values1),
            (other1.values2 * other2.values2), (other1.values3 * other2.values3));
    }
}

/**
 * Multiply a SIMD4 by a scalar.
 * @param other1 The SIMD4.
 * @param other2 Value to multiply the SIMD4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator*(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, uint32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_mullo_epi32(other1.values, other2.values));
    } else if constexpr (isSame<T, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_mullo_epi32(other1.values, other2.values));
    } else if constexpr (isSame<T, uint16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_mullo_epi16(other1.values, other2.values));
    } else if constexpr (isSame<T, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_mullo_epi16(other1.values, other2.values));
    } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        const __m128i ret = _mm_mullo_epi16(_mm_cvtepu8_epi16(other1.values), _mm_cvtepu8_epi16(other2.values));
        return SIMD4<T, Width>(_mm_packus_epi16(ret, ret));
    } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        const __m128i ret = _mm_mullo_epi16(_mm_cvtepi8_epi16(other1.values), _mm_cvtepi8_epi16(other2.values));
        return SIMD4<T, Width>(_mm_packs_epi16(ret, ret));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 * other2.value), (other1.values1 * other2.value),
            (other1.values2 * other2.value), (other1.values3 * other2.value));
    }
}

/**
 * Multiply each pair of elements in a SIMD4 by a SIMD2.
 * @param other1 The SIMD4.
 * @param other2 Values to multiply the SIMD4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator*(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_mul_ps(other1.values, _mm_shuffle1010_ps(other2.values)));
    } else if constexpr (isSame<T, uint32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_mullo_epi32(other1.values, _mm_shuffle1010_epi32(other2.values)));
    } else if constexpr (isSame<T, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_mullo_epi32(other1.values, _mm_shuffle1010_epi32(other2.values)));
    } else if constexpr (isSame<T, uint16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(
            _mm_mullo_epi16(other1.values, _mm_shufflelo_epi16(other2.values, _MM_SHUFFLE(1, 0, 1, 0))));
    } else if constexpr (isSame<T, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(
            _mm_mullo_epi16(other1.values, _mm_shufflelo_epi16(other2.values, _MM_SHUFFLE(1, 0, 1, 0))));
    } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        const __m128i ret = _mm_mullo_epi16(_mm_cvtepu8_epi16(other1.values),
            _mm_cvtepu8_epi16(_mm_shufflelo_epi16(other2.values, _MM_SHUFFLE(0, 0, 0, 0))));
        return SIMD4<T, Width>(_mm_packus_epi16(ret, ret));
    } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        const __m128i ret = _mm_mullo_epi16(_mm_cvtepi8_epi16(other1.values),
            _mm_cvtepi8_epi16(_mm_shufflelo_epi16(other2.values, _MM_SHUFFLE(0, 0, 0, 0))));
        return SIMD4<T, Width>(_mm_packs_epi16(ret, ret));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 * other2.values0), (other1.values1 * other2.values1),
            (other1.values2 * other2.values0), (other1.values3 * other2.values1));
    }
}

/**
 * Divide two SIMD4s per element.
 * @param other1 The first SIMD4.
 * @param other2 The second SIMD4 to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator/(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 / other2.values0), (other1.values1 / other2.values1),
            (other1.values2 / other2.values2), (other1.values3 / other2.values3));
    }
}

/**
 * Divide a SIMD4 by a scalar.
 * @param other1 The SIMD4.
 * @param other2 Value to divide the SIMD4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator/(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 / other2.value), (other1.values1 / other2.value),
            (other1.values2 / other2.value), (other1.values3 / other2.value));
    }
}

/**
 * Divide a scalar by all elements of a SIMD4.
 * @param other1 Value to divide the SIMD4 by.
 * @param other2 The SIMD4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator/(
    const typename SIMD4<T, Width>::BaseDef other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.value / other2.values0), (other1.value / other2.values1),
            (other1.value / other2.values2), (other1.value / other2.values3));
    }
}

/**
 * Divide each pair of elements in a SIMD4 by a SIMD2.
 * @param other1 The SIMD4.
 * @param other2 Values to divide the SIMD4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator/(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_div_ps(other1.values, _mm_shuffle1010_ps(other2.values)));
    } else
#endif
    {
        return SIMD4<T, Width>((other1.values0 / other2.values0), (other1.values1 / other2.values1),
            (other1.values2 / other2.values0), (other1.values3 / other2.values1));
    }
}

/**
 * Negate all elements of a SIMD4.
 * @param other The SIMD4 to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator-(const SIMD4<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_sub_ps(_mm_setzero_ps(), other.values));
    } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_sub_epi32(_mm_setzero_si128(), other.values));
    } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_sub_epi16(_mm_setzero_si128(), other.values));
    } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_sub_epi8(_mm_setzero_si128(), other.values));
    } else
#endif
    {
        return SIMD4<T, Width>(-other.values0, -other.values1, -other.values2, -other.values3);
    }
}

/**
 * Perform compound assignment and addition with a SIMD4.
 * @param [in,out] other1 The first SIMD4.
 * @param          other2 SIMD4 to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator+=(SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_add_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
        other1.values2 += other2.values2;
        other1.values3 += other2.values3;
    }
    return other1;
}

/**
 * Perform compound assignment and addition with a SIMD4.
 * @param [in,out] other1 The SIMD4.
 * @param          other2 Value to add to the first SIMD4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator+=(SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_add_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 += other2.value;
        other1.values1 += other2.value;
        other1.values2 += other2.value;
        other1.values3 += other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and addition of a SIMD2 to each pair of elements in a SIMD4.
 * @param [in,out] other1 The SIMD4.
 * @param          other2 Values to add to the SIMD4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator+=(
    SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_add_ps(other1.values, _mm_shuffle1010_ps(other2.values));
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
        other1.values2 += other2.values0;
        other1.values3 += other2.values1;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction by a SIMD4.
 * @param [in,out] other1 The first SIMD4.
 * @param          other2 SIMD4 to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator-=(SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_sub_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
        other1.values2 -= other2.values2;
        other1.values3 -= other2.values3;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction with a SIMD4.
 * @param [in,out] other1 The SIMD4.
 * @param          other2 Value to subtract from the first SIMD4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator-=(SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_sub_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 -= other2.value;
        other1.values1 -= other2.value;
        other1.values2 -= other2.value;
        other1.values3 -= other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction of a SIMD2 to each pair of elements in a SIMD4.
 * @param [in,out] other1 The SIMD4.
 * @param          other2 Values to subtract the SIMD4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator-=(
    SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_sub_ps(other1.values, _mm_shuffle1010_ps(other2.values));
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
        other1.values2 -= other2.values0;
        other1.values3 -= other2.values1;
    }
    return other1;
}

/**
 * Perform SIMD4 compound assignment and multiplication.
 * @param [in,out] other1 The first SIMD4.
 * @param          other2 SIMD4 to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator*=(SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_mul_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
        other1.values2 *= other2.values2;
        other1.values3 *= other2.values3;
    }
    return other1;
}

/**
 * Perform compound assignment and multiplication by a scalar.
 * @param [in,out] other1 The SIMD4.
 * @param          other2 Value to multiply the SIMD4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator*=(SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_mul_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 *= other2.value;
        other1.values1 *= other2.value;
        other1.values2 *= other2.value;
        other1.values3 *= other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and multiplication of each pair of elements in a SIMD4 by a SIMD2.
 * @param [in,out] other1 The SIMD4.
 * @param          other2 Values to multiply the SIMD4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator*=(
    SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_mul_ps(other1.values, _mm_shuffle1010_ps(other2.values));
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
        other1.values2 *= other2.values0;
        other1.values3 *= other2.values1;
    }
    return other1;
}

/**
 * Perform compound assignment and division.
 * @param [in,out] other1 The SIMD4.
 * @param          other2 Value to divide the SIMD4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator/=(SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_div_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
        other1.values2 /= other2.values2;
        other1.values3 /= other2.values3;
    }
    return other1;
}

/**
 * Perform compound assignment and division by a scalar.
 * @param [in,out] other1 The SIMD4.
 * @param          other2 Value to divide the SIMD4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator/=(SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_div_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 /= other2.value;
        other1.values1 /= other2.value;
        other1.values2 /= other2.value;
        other1.values3 /= other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and division of each pair of elements in a SIMD4 by a SIMD2.
 * @param [in,out] other1 The SIMD4.
 * @param          other2 Values to divide the SIMD4 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width>& operator/=(
    SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::SIMD2Def& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_div_ps(other1.values, _mm_shuffle1010_ps(other2.values));
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
        other1.values2 /= other2.values0;
        other1.values3 /= other2.values1;
    }
    return other1;
}

/**
 * Compare two SIMD4s are equal.
 * @param other1 The first SIMD4.
 * @param other2 The second SIMD4 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator==(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 value = _mm_cmp_ps(other1.values, other2.values, _CMP_NEQ_OQ);
            return static_cast<bool>(_mm_testz_ps(value, value));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 == other2.values0) & (other1.values1 == other2.values1) &
            (other1.values2 == other2.values2) & (other1.values3 == other2.values3));
    }
}

/**
 * Compare a SIMD4 to a value.
 * @param other1 The SIMD4.
 * @param other2 The value to compare to the SIMD4.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator==(const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 value = _mm_cmp_ps(other1.values, other2.values, _CMP_NEQ_UQ);
            return static_cast<bool>(_mm_testz_ps(value, value));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 == other2.value) & (other1.values1 == other2.value) & (other1.values2 == other2.value) &
            (other1.values3 == other2.value));
    }
}

/**
 * Compare two SIMD4s are less or equal.
 * @param other1 The first SIMD4.
 * @param other2 The second SIMD4 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator<=(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 value = _mm_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ);
            return static_cast<bool>(_mm_testz_ps(value, value));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.values0) & (other1.values1 <= other2.values1) &
            (other1.values2 <= other2.values2) & (other1.values3 <= other2.values3));
    }
}

/**
 * Compare SIMD4 is less than or equal to a value.
 * @param other1 The SIMD4.
 * @param other2 The value to compare to the SIMD4.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator<=(const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 value = _mm_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ);
            return static_cast<bool>(_mm_testz_ps(value, value));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.value) & (other1.values1 <= other2.value) & (other1.values2 <= other2.value) &
            (other1.values3 <= other2.value));
    }
}

/**
 * Compare two SIMD4s are less than.
 * @param other1 The first SIMD4.
 * @param other2 The second SIMD4 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator<(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 value = _mm_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ);
            return static_cast<bool>(_mm_testz_ps(value, value));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 < other2.values0) & (other1.values1 < other2.values1) &
            (other1.values2 < other2.values2) & (other1.values3 < other2.values3));
    }
}

/**
 * Compare SIMD4 is less than a value.
 * @param other1 The SIMD4.
 * @param other2 The value to compare to the SIMD4.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator<(const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 value = _mm_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ);
            return static_cast<bool>(_mm_testz_ps(value, value));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 < other2.value) & (other1.values1 < other2.value) & (other1.values2 < other2.value) &
            (other1.values3 < other2.value));
    }
}

/**
 * Compare two SIMD4s are not equal.
 * @param other1 The first SIMD4.
 * @param other2 The second SIMD4 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator!=(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 value = _mm_cmp_ps(other1.values, other2.values, _CMP_EQ_UQ);
            return static_cast<bool>(_mm_testz_ps(value, value));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpeq_ps(other1.values, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 != other2.values0) & (other1.values1 != other2.values1) &
            (other1.values2 != other2.values2) & (other1.values3 != other2.values3));
    }
}

/**
 * Compare SIMD4 is not equal to a value.
 * @param other1 The SIMD4.
 * @param other2 The value to compare to the SIMD4.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator!=(const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            const __m128 value = _mm_cmp_ps(other1.values, other2.values, _CMP_EQ_UQ);
            return static_cast<bool>(_mm_testz_ps(value, value));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpeq_ps(other1.values, other2.values)));
        }
    } else
#endif
    {
        return ((other1.values0 != other2.value) & (other1.values1 != other2.value) & (other1.values2 != other2.value) &
            (other1.values3 != other2.value));
    }
}

/**
 * And 2 SIMD4s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD4.
 * @param other2 SIMD4 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator&(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_and_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(other1.values0 & other2.values0, other1.values1 & other2.values1,
                other1.values2 & other2.values2, other1.values3 & other2.values3);
        } else {
            return SIMD4<T, Width>(Shift::bitAnd(other1.values0, other2.values0),
                Shift::bitAnd(other1.values1, other2.values1), Shift::bitAnd(other1.values2, other2.values2),
                Shift::bitAnd(other1.values3, other2.values3));
        }
    }
}

/**
 * And a SIMD4s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD4.
 * @param other2 value to and with the SIMD4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator&(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_and_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(other1.values0 & other2.value, other1.values1 & other2.value,
                other1.values2 & other2.value, other1.values3 & other2.value);
        } else {
            return SIMD4<T, Width>(Shift::bitAnd(other1.values0, other2.value),
                Shift::bitAnd(other1.values1, other2.value), Shift::bitAnd(other1.values2, other2.value),
                Shift::bitAnd(other1.values3, other2.value));
        }
    }
}

/**
 * Or 2 SIMD4s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD4.
 * @param other2 SIMD4 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator|(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_or_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(other1.values0 | other2.values0, other1.values1 | other2.values1,
                other1.values2 | other2.values2, other1.values3 | other2.values3);
        } else {
            return SIMD4<T, Width>(Shift::bitOr(other1.values0, other2.values0),
                Shift::bitOr(other1.values1, other2.values1), Shift::bitOr(other1.values2, other2.values2),
                Shift::bitOr(other1.values3, other2.values3));
        }
    }
}

/**
 * Or a SIMD4s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD4.
 * @param other2 value to and with the SIMD4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator|(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_or_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(other1.values0 | other2.value, other1.values1 | other2.value,
                other1.values2 | other2.value, other1.values3 | other2.value);
        } else {
            return SIMD4<T, Width>(Shift::bitOr(other1.values0, other2.value),
                Shift::bitOr(other1.values1, other2.value), Shift::bitOr(other1.values2, other2.value),
                Shift::bitOr(other1.values3, other2.value));
        }
    }
}

/**
 * Xor 2 SIMD4s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD4.
 * @param other2 SIMD4 to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator^(const SIMD4<T, Width>& other1, const SIMD4<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_xor_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(other1.values0 ^ other2.values0, other1.values1 ^ other2.values1,
                other1.values2 ^ other2.values2, other1.values3 ^ other2.values3);
        } else {
            return SIMD4<T, Width>(Shift::bitXor(other1.values0, other2.values0),
                Shift::bitXor(other1.values1, other2.values1), Shift::bitXor(other1.values2, other2.values2),
                Shift::bitXor(other1.values3, other2.values3));
        }
    }
}

/**
 * Xor a SIMD4s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD4.
 * @param other2 value to and with the SIMD4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator^(
    const SIMD4<T, Width>& other1, const typename SIMD4<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMD4<T, Width>(_mm_xor_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(other1.values0 ^ other2.value, other1.values1 ^ other2.value,
                other1.values2 ^ other2.value, other1.values3 ^ other2.value);
        } else {
            return SIMD4<T, Width>(Shift::bitXor(other1.values0, other2.value),
                Shift::bitXor(other1.values1, other2.value), Shift::bitXor(other1.values2, other2.value),
                Shift::bitXor(other1.values3, other2.value));
        }
    }
}

/**
 * Not a SIMD4.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other The SIMD4.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMD4<T, Width> operator~(const SIMD4<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        const __m128 val = _mm_undefined_ps();
        return SIMD4<T, Width>(_mm_xor_ps(other.values, _mm_cmpeq_ps(val, val)));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMD4<T, Width>(~other.values0, ~other.values1, ~other.values2, ~other.values3);
        } else {
            return SIMD4<T, Width>(Shift::bitNot(other.values0), Shift::bitNot(other.values1),
                Shift::bitNot(other.values2), Shift::bitNot(other.values3));
        }
    }
}
} // namespace Shift
