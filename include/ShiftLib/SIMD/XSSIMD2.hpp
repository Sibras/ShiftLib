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

#include "SIMD/XSSIMDBase.hpp"
#include "SIMD/XSSIMDBool.hpp"
#include "SIMD/XSSIMDInBase.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class SIMD2;

template<typename T>
class alignas(maxAlignment<T, 2>) SIMD2Data
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    T value0, value1;

    /** Default constructor. */
    XS_FUNCTION SIMD2Data() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMD2Data(const SIMD2<T, Width>& other) noexcept
    {
        store(other);
    }

    /**
     * Directly set the contents this object.
     * @note This function is used to set data using pre-calculated values.
     * This removes the necessity to load and then store.
     * @param other0 The first pre-calculated value.
     * @param other1 The second pre-calculated value.
     */
    XS_FUNCTION void setData(T other0, T other1) noexcept
    {
        value0 = other0;
        value1 = other1;
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(const SIMD2<T, Width>& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            _mm_storel_pi(reinterpret_cast<__m64*>(this), other.values);
        } else
#endif
        {
            value0 = other.values0;
            value1 = other.values1;
        }
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width>
    XS_FUNCTION SIMD2<T, Width> load() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2<T, Width>(_mm_loadl_pi(_mm_undefined_ps(), reinterpret_cast<const __m64*>(this)));
        } else
#endif
        {
            return SIMD2<T, Width>(value0, value1);
        }
    }
};

template<typename T>
using SIMD2DataPad = SIMD2Data<T>;

template<typename T, SIMDWidth Width = widthSIMD<T>>
class SIMD2 : public NoExport::SIMDData<T, 2, 0, Width>
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    using Type = T;
    using Data = NoExport::SIMDData<T, 2, 0, Width>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Data::width;
    using BaseDef = SIMDBase<T, SIMDBase<T, widthImpl>::widthImpl>;
    using InBaseDef = SIMDInBase<T, SIMDInBase<T, widthImpl>::widthImpl>;
    using Data::SIMDData;

    /** Dual Mask object used to store 2 different masks at once. */
    class Mask : public NoExport::SIMDMaskData<T, 2, 0, Width>
    {
    public:
        using Type = T;
        using Data = NoExport::SIMDMaskData<T, 2, 0, Width>;
        static constexpr SIMDWidth width = Width;
        static constexpr SIMDWidth widthImpl = Data::width;
        using Data::SIMDMaskData;
        using Bool = Bool2<hasSIMD<T> && (Width > SIMDWidth::Scalar)>;

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
                    auto values = _mm_shuffle0000_epi32(_mm_cvtsi32_si128(mask));
                    const __m128i bitMask = _mm_set_epi32(0x0, 0x0, 0x2, 0x1);
                    values = _mm_and_si128(values, bitMask);
                    values = _mm_cmpeq_epi32(values, bitMask);
                    this->values = _mm_castsi128_ps(values);
                }
            } else
#endif
            {
                this->values0 = mask & 0x1;
                this->values1 = mask & 0x2;
            }
        }

        /**
         * Construct a mask from individual booleans.
         * @param bool0 The first boolean value.
         * @param bool1 The second boolean value.
         */
        XS_FUNCTION Mask(bool bool0, bool bool1) noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    this->values = _cvtu32_mask8((static_cast<uint32>(bool1) << 1) | static_cast<uint32>(bool0));
                } else {
                    this->values = _mm_cmpneq_ps(_mm_set_ps(0, 0, bool1, bool0), _mm_setzero_ps());
                }
            } else
#endif
            {
                this->values0 = bool0;
                this->values1 = bool1;
            }
        }

        /**
         * Get the boolean equivalent of the mask.
         * @returns The bool.
         */
        XS_FUNCTION Bool getBool2() const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    return Bool2<true>(static_cast<uint8>(_cvtmask8_u32(this->values)));
                } else {
                    return Bool2<true>(static_cast<uint8>(_mm_movemask_ps(this->values)));
                }
            } else
#endif
            {
                return Bool2<false>(this->values0, this->values1);
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
                    return static_cast<bool>(_cvtmask8_u32(this->values) & 0x3);
                } else {
                    return static_cast<bool>(_mm_movemask_ps(this->values) & 0x3);
                }
            } else
#endif
            {
                return this->values0 | this->values1;
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
                    return ((_cvtmask8_u32(this->values) & 0x3) == 0x3);
                } else {
                    return ((_mm_movemask_ps(this->values) & 0x3) == 0x3);
                }
            } else
#endif
            {
                return this->values0 & this->values1;
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
                    return ((_cvtmask8_u32(this->values) & 0x3) == 0x0);
                } else {
                    return ((_mm_movemask_ps(this->values) & 0x3) == 0x0);
                }
            } else
#endif
            {
                return !getAny();
            }
        }

    private:
#if XS_ISA == XS_X86
        class SIMDMasker2X86
        {
        public:
            XS_INLINE static SIMD2& ToType(SIMD2 other)
            {
                return other;
            }
        };
#endif
        template<uint32 Index>
        class SIMDMasker2
        {
        public:
            XS_INLINE static InBaseDef ToType(SIMD2& other)
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
        XS_FUNCTION void mask2Function(MaskOperator& maskFunc) const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                auto val = maskFunc.template expression<SIMD2, SIMDMasker2X86>();
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    val.values = _mm_mask_blend_ps(this->values, _mm_setzero_ps(), val.values);
                } else {
                    val.values = _mm_and_ps(val.values, this->values);
                }
                maskFunc.template finalExpression<SIMD2, SIMDMasker2X86>(val);
            } else
#endif
            {
                if (this->values0) {
                    auto val = maskFunc.template expression<InBaseDef, SIMDMasker2<0>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker2<0>>(val);
                }
                if (this->values1) {
                    auto val = maskFunc.template expression<InBaseDef, SIMDMasker2<1>>();
                    maskFunc.template finalExpression<InBaseDef, SIMDMasker2<1>>(val);
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
        XS_FUNCTION void mask2ElseFunction(MaskOperator& maskFunc) const noexcept
        {
#if XS_ISA == XS_X86
            if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
                auto val1 = maskFunc.template expression1<SIMD2<T, Width>, SIMDMasker2X86>();
                auto val2 = maskFunc.template expression2<SIMD2<T, Width>, SIMDMasker2X86>();
                const auto final(val1.blendVar(val2, *this));
                maskFunc.template finalExpression<SIMD2<T, Width>, SIMDMasker2X86>(final);
            } else
#endif
            {
                auto val = (this->values0) ? maskFunc.template expression1<InBaseDef, SIMDMasker2<T, Width, 0>>() :
                                             maskFunc.template expression2<InBaseDef, SIMDMasker2<T, Width, 0>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker2<T, Width, 0>>(val);
                val = (this->values1) ? maskFunc.template expression1<InBaseDef, SIMDMasker2<T, Width, 1>>() :
                                        maskFunc.template expression2<InBaseDef, SIMDMasker2<T, Width, 1>>();
                maskFunc.template finalExpression<InBaseDef, SIMDMasker2<T, Width, 1>>(val);
            }
        }
    };

    /** Default constructor. */
    XS_FUNCTION SIMD2() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION SIMD2(const SIMD2& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_FUNCTION SIMD2(SIMD2&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD2& operator=(const SIMD2& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMD2& operator=(SIMD2&& other) noexcept = default;

    /**
     * Construct from 2 values.
     * @param other0 The first value.
     * @param other1 The second value.
     */
    XS_FUNCTION SIMD2(T other0, T other1) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values = _mm_set_ps(other1, other0, other1, other0);
        } else
#endif
        {
            this->values0 = other0;
            this->values1 = other1;
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param val Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD2(T val) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values = _mm_set1_ps(val);
        } else
#endif
        {
            this->values0 = val;
            this->values1 = val;
        }
    }

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_FUNCTION explicit SIMD2(const SIMD2<T, Width2>& other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar) && (Width2 > SIMDWidth::Scalar)) {
            this->values = other.values;
        } else if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            *this = SIMD2(other.values0, other.values1);
        } else if constexpr (Width2 > SIMDWidth::Scalar) {
            this->values0 = other.template getValueInBase<0>().getValue();
            this->values1 = other.template getValueInBase<1>().getValue();
        } else
#endif
        {
            this->values0 = other.values0;
            this->values1 = other.values1;
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD2(BaseDef other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values = other.values;
        } else
#endif
        {
            this->values0 = other.value;
            this->values1 = other.value;
        }
    }

    /**
     * Constructor to set all elements to the same scalar value.
     * @param other Value to set all elements to.
     */
    XS_FUNCTION explicit SIMD2(InBaseDef other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                this->values = _mm_shuffle0000_ps(other.values);
            } else {
                this->values = _mm_shuffle2200_ps(other.values);
            }
        } else
#endif
        {
            this->values0 = other.value;
            this->values1 = other.value;
        }
    }

    /**
     * Construct from 2 values.
     * @param other0 The first value.
     * @param other1 The second value.
     */
    XS_FUNCTION SIMD2(InBaseDef other0, InBaseDef other1) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values = _mm_unpacklo_ps(other0.values, other1.values);
        } else
#endif
        {
            this->values0 = other0.value;
            this->values1 = other1.value;
        }
    }

    /**
     * Construct from 2 values.
     * @param other0 The first value.
     * @param other1 The second value.
     */
    XS_FUNCTION SIMD2(BaseDef other0, BaseDef other1) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX>) {
                this->values = _mm_blend_ps(other0.values, other1.values,
                    _MM_BLEND(1, 0, 1, 0)); // Blend has higher throughput on SandyBridge and above
            } else {
                this->values = _mm_unpacklo_ps(other0.values, other1.values);
            }
        } else
#endif
        {
            this->values0 = other0.value;
            this->values1 = other1.value;
        }
    }

    /**
     * Constructor to build set to 0.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD2 Zero() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(_mm_setzero_ps());
        } else
#endif
        {
            return SIMD2(T{0});
        }
    }

    /**
     * Constructor to build set to 1.
     * @returns Newly constructed object with required attributes.
     */
    XS_FUNCTION static SIMD2 One() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(_mm_set1_ps(1.0f));
        } else
#endif
        {
            return SIMD2(T{1});
        }
    }

    /**
     * Get an element of the object.
     * @note Optimised for getting value from register.
     * @tparam Index The index of the element to retrieve (range is 0-1).
     * @returns SIMDInBase containing the desired value.
     */
    template<uint32 Index>
    XS_FUNCTION InBaseDef getValueInBase() const noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                return InBaseDef(this->values);
            } else {
                return InBaseDef(_mm_shuffle3311_ps(this->values)); //(x,x,x,1)
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
     * @tparam Index The index of the element to retrieve (range is 0-1).
     * @returns SIMDBase containing the desired value.
     */
    template<uint32 Index>
    XS_FUNCTION BaseDef getValue() const noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                return BaseDef(_mm_shuffle0000_ps(this->values));
            } else {
                return BaseDef(_mm_shuffle1111_ps(this->values));
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
     * @param index The index of the element to retrieve (range is 0-1).
     * @returns SIMDInBase containing the desired value.
     */
    XS_FUNCTION InBaseDef getValueInBase(uint32 index) const noexcept
    {
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
     * @param index The index of the element to retrieve (range is 0-1).
     * @returns SIMDBase containing the desired value.
     */
    XS_FUNCTION BaseDef getValue(uint32 index) const noexcept
    {
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
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-1).
     * @param other The new value.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue(InBaseDef other) noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                this->values = _mm_move_ss(this->values, other.values);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                this->values = _mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(0, Index, 0));
            } else {
                const __m128 val = _mm_movelh_ps(this->values, other.values); /*(x,0,x,0)*/
                this->values = _mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 2, 0));
            }
        } else
#endif
        {
            (&this->values0)[Index] = other.value;
        }
    }

    /**
     * Set an element of the object.
     * @tparam Index The index of the element to set (range is 0-1)
     * @param other The new value.
     */
    template<uint32 Index>
    XS_FUNCTION void setValue(BaseDef other) noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                this->values = _mm_move_ss(this->values, other.values);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                this->values = _mm_blend_ps(this->values, other.values, (1 << Index));
            } else {
                const __m128 val = _mm_movelh_ps(this->values, other.values); /*(x,0,x,0)*/
                this->values = _mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 2, 0));
            }
        } else
#endif
        {
            (&this->values0)[Index] = other.value;
        }
    }

    /**
     * Dynamically set an element of the object.
     * @param index The index of the element to retrieve (range is 0-1).
     * @param other The new value.
     */
    XS_FUNCTION void setValue(uint32 index, BaseDef other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                this->values = _mm_mask_blend_ps(_cvtu32_mask8(1UL << index), this->values, other.values);
            } else {
                const __m128 mask = _mm_castsi128_ps(
                    _mm_cmpeq_epi32(_mm_shuffle3200_epi32(_mm_loadu_si32(&index)), _mm_set_epi32(3, 2, 1, 0)));
                if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                    this->values = _mm_blendv_ps(this->values, other.values, mask);
                } else {
                    const __m128 val0 = _mm_and_ps(other.values, mask);
                    const __m128 val1 = _mm_andnot_ps(mask, this->values);
                    this->values = _mm_or_ps(val0, val1);
                }
            }
        } else
#endif
        {
            (&this->values0)[index] = other.value;
        }
    }

    /**
     * Add a value to an element of the object.
     * @tparam Index The index of the element to modify (range is 0-1).
     * @param other The value to add.
     */
    template<uint32 Index>
    XS_FUNCTION void addValue(InBaseDef other) noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                this->values = _mm_add_ss(this->values, other.values);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                const __m128 val = _mm_shuffle2200_ps(other.values);
                this->values = _mm_blend_add_ps(this->values, 1 << Index, this->values, val);
            } else {
                __m128 val = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(3, 2, 0, 1));
                val = _mm_add_ss(val, other.values);
                this->values = _mm_shuffle1_ps(val, _MM_SHUFFLE(3, 2, 0, 1));
            }
        } else
#endif
        {
            (&this->values0)[Index] += other.value;
        }
    }

    /**
     * Subtract a value from an element of the object.
     * @tparam Index The index of the element to modify (range is 0-1).
     * @param other The value to subtract.
     */
    template<uint32 Index>
    XS_FUNCTION void subValue(InBaseDef other) noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                this->values = _mm_sub_ss(this->values, other.values);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                const __m128 val = _mm_shuffle2200_ps(other.values);
                this->values = _mm_blend_sub_ps(this->values, 1 << Index, this->values, val);
            } else {
                __m128 val = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(3, 2, 0, 1));
                val = _mm_sub_ss(val, other.values);
                this->values = _mm_shuffle1_ps(val, _MM_SHUFFLE(3, 2, 0, 1));
            }
        } else
#endif
        {
            (&this->values0)[Index] -= other.value;
        }
    }

    /**
     * Multiply an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-1).
     * @param other The value to multiply by.
     */
    template<uint32 Index>
    XS_FUNCTION void mulValue(InBaseDef other) noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                this->values = _mm_mul_ss(this->values, other.values);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                const __m128 val = _mm_shuffle2200_ps(other.values);
                this->values = _mm_blend_mul_ps(this->values, 1 << Index, this->values, val);
            } else {
                __m128 val = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(3, 2, 0, 1));
                val = _mm_mul_ss(val, other.values);
                this->values = _mm_shuffle1_ps(val, _MM_SHUFFLE(3, 2, 0, 1));
            }
        } else
#endif
        {
            (&this->values0)[Index] *= other.value;
        }
    }

    /**
     * Divide an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-1).
     * @param other The value to divide by.
     */
    template<uint32 Index>
    XS_FUNCTION void divValue(InBaseDef other) noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                this->values = _mm_div_ss(this->values, other.values);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                const __m128 val = _mm_shuffle2200_ps(other.values);
                this->values = _mm_blend_div_ps(this->values, 1 << Index, this->values, val);
            } else {
                __m128 val = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(3, 2, 0, 1));
                val = _mm_div_ss(val, other.values);
                this->values = _mm_shuffle1_ps(val, _MM_SHUFFLE(3, 2, 0, 1));
            }
        } else
#endif
        {
            (&this->values0)[Index] /= other.value;
        }
    }

    /**
     * Multiply then add to an element of the object by a value.
     * @tparam Index The index of the element to modify (range is 0-1).
     * @param other1 The value to multiply by.
     * @param other2 The value to add.
     */
    template<uint32 Index>
    XS_FUNCTION void madValue(InBaseDef other1, InBaseDef other2) noexcept
    {
        static_assert(Index < 2, "Invalid Index: Index must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index == 0) {
                this->values = _mm_fmadd_ss(this->values, other1.values, other2.values);
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                const __m128 value1 = _mm_shuffle2200_ps(other1.values);
                const __m128 value2 = _mm_shuffle2200_ps(other2.values);
                this->values = _mm_blend_fmadd_ps(this->values, 1 << (Index % 4), value1, value2);
            } else {
                __m128 val = _mm_shuffle1_ps(this->values, _MM_SHUFFLE(3, 2, 0, 1));
                val = _mm_fmadd_ss(val, other1.values, other2.values);
                this->values = _mm_shuffle1_ps(val, _MM_SHUFFLE(3, 2, 0, 1));
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
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1>
    XS_FUNCTION SIMD2 negate() const noexcept
    {
        if constexpr (!Elem0 && !Elem1) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Elem0 && !Elem1) {
                return SIMD2(_mm_xor_ps(this->values, _mm_set_ss(-0.0f)));
            } else if constexpr (Elem0 && Elem1) {
                return SIMD2(_mm_xor_ps(this->values, _mm_set1_ps(-0.0f)));
            } else {
                return SIMD2(
                    _mm_xor_ps(this->values, _mm_set_ps(0.0f, 0.0f, Elem1 ? -0.0f : 0.0f, Elem0 ? -0.0f : 0.0f)));
            }
        }
#endif
        else {
            return SIMD2(Elem0 ? -this->values0 : this->values0, Elem1 ? -this->values1 : this->values1);
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
    XS_FUNCTION SIMD2 mad(const SIMD2& other1, const SIMD2& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD2(_mm_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD2(_mm_add_ps(_mm_mul_ps(this->values, other1.values), other2.values));
            }
        } else
#endif
        {
            return SIMD2(fma<T>(this->values0, other1.values0, other2.values0),
                fma<T>(this->values1, other1.values1, other2.values1));
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
    XS_FUNCTION SIMD2 mad(BaseDef other1, const SIMD2<T, Width>& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD2(_mm_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD2(_mm_add_ps(_mm_mul_ps(this->values, other1.values), other2.values));
            }
        } else
#endif
        {
            return SIMD2(fma<T>(this->values0, other1.value, other2.values0),
                fma<T>(this->values1, other1.value, other2.values1));
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
    XS_FUNCTION SIMD2 mad(const SIMD2<T, Width>& other1, BaseDef other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD2(_mm_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD2(_mm_add_ps(_mm_mul_ps(this->values, other1.values), other2.values));
            }
        } else
#endif
        {
            return SIMD2(fma<T>(this->values0, other1.values0, other2.value),
                fma<T>(this->values1, other1.values1, other2.value));
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
    XS_FUNCTION SIMD2 msub(const SIMD2& other1, const SIMD2& other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMD2(_mm_fmsub_ps(this->values, other1.values, other2.values));
            } else {
                return SIMD2(_mm_sub_ps(_mm_mul_ps(this->values, other1.values), other2.values));
            }
        } else
#endif
        {
            if constexpr (hasFMA<T>) {
                return SIMD2(fma<T>(this->values0, other1.values0, -other2.values0),
                    fma<T>(this->values1, other1.values1, -other2.values1));
            } else {
                return SIMD2((this->values0 * other1.values0) - other2.values0,
                    (this->values1 * other1.values1) - other2.values1);
            }
        }
    }

    /**
     * Alternately subtract and add 2 objects.
     * @param other The second object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD2 subAdd(const SIMD2& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(_mm_addsub_ps(this->values, other.values));
        } else
#endif
        {
            return SIMD2(this->values0 - other.values0, this->values1 + other.values1);
        }
    }

    /**
     * Compare two objects are equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask equalMask(const SIMD2& other) const noexcept
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
            return Mask(this->values0 == other.values0, this->values1 == other.values1);
        }
    }

    /**
     * Compare two objects are less or equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask lessOrEqualMask(const SIMD2& other) const noexcept
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
            return Mask(this->values0 <= other.values0, this->values1 <= other.values1);
        }
    }

    /**
     * Compare two objects are less than.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask lessThanMask(const SIMD2& other) const noexcept
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
            return Mask(this->values0 < other.values0, this->values1 < other.values1);
        }
    }

    /**
     * Compare two objects are not equal.
     * @param other The second object to compare to the first.
     * @returns Mask containing comparison applied to each element of the object.
     */
    XS_FUNCTION Mask notEqualMask(const SIMD2& other) const noexcept
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
            return Mask(this->values0 != other.values0, this->values1 != other.values1);
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
            return Mask(this->values0 == other.value, this->values1 == other.value);
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
            return Mask(this->values0 <= other.value, this->values1 <= other.value);
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
            return Mask(this->values0 < other.value, this->values1 < other.value);
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
            return Mask(this->values0 != other.value, this->values1 != other.value);
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
            return Mask(this->values0 >= other.value, this->values1 >= other.value);
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
            return Mask(this->values0 > other.value, this->values1 > other.value);
        }
    }

    /**
     * Negate this object based on the sign of another.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @param other The second object whose sign to check.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD2 sign(const SIMD2& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 mask = _mm_and_ps(_mm_set1_ps(-0.0f), other.values);
            return SIMD2(_mm_xor_ps(this->values, mask));
        } else
#endif
        {
            return SIMD2(Shift::sign<T>(this->values0, other.values0), Shift::sign<T>(this->values1, other.values1));
        }
    }

    /**
     * Negate this object based on the sign of another.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @param other The second object whose sign to check.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD2 sign(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 mask = _mm_and_ps(_mm_set1_ps(-0.0f), other.values);
            return SIMD2(_mm_xor_ps(this->values, mask));
        } else
#endif
        {
            return SIMD2(Shift::sign<T>(this->values0, other.value), Shift::sign<T>(this->values1, other.value));
        }
    }

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD2 abs() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(_mm_andnot_ps(_mm_set1_ps(-0.0f), this->values));
        } else
#endif
        {
            return SIMD2(Shift::abs<T>(this->values0), Shift::abs<T>(this->values1));
        }
    }

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMD2 max(const SIMD2& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(_mm_max_ps(this->values, other.values));
        } else
#endif
        {
            return SIMD2(Shift::max<T>(this->values0, other.values0), Shift::max<T>(this->values1, other.values1));
        }
    }

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMD2 min(const SIMD2& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(_mm_min_ps(this->values, other.values));
        } else
#endif
        {
            return SIMD2(Shift::min<T>(this->values0, other.values0), Shift::min<T>(this->values1, other.values1));
        }
    }

    /**
     * Maximum of two objects per element.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMD2 max(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(_mm_max_ps(this->values, other.values));
        } else
#endif
        {
            return SIMD2(Shift::max<T>(this->values0, other.value), Shift::max<T>(this->values1, other.value));
        }
    }

    /**
     * Minimum of two objects per element.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMD2 min(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(_mm_min_ps(this->values, other.values));
        } else
#endif
        {
            return SIMD2(Shift::min<T>(this->values0, other.value), Shift::min<T>(this->values1, other.value));
        }
    }

    /**
     * Maximum element of this object
     * @returns BaseDef set to the largest value.
     */
    XS_FUNCTION BaseDef hmax() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 other = _mm_max_ss(_mm_shuffle3311_ps(this->values), this->values); //(x,x,x,1)
            return BaseDef(_mm_shuffle0000_ps(other));
        } else
#endif
        {
            return BaseDef(Shift::max<T>(this->values0, this->values1));
        }
    }

    /**
     * Minimum element of this object
     * @returns BaseDef set to the smallest value.
     */
    XS_FUNCTION BaseDef hmin() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 other = _mm_min_ss(_mm_shuffle3311_ps(this->values), this->values); //(x,x,x,1)
            return BaseDef(_mm_shuffle0000_ps(other));
        } else
#endif
        {
            return BaseDef(Shift::min<T>(this->values0, this->values1));
        }
    }

    /**
     * Maximum element of this object
     * @returns SIMDInBase set to the largest value.
     */
    XS_FUNCTION InBaseDef hmaxInBase() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return InBaseDef(_mm_max_ss(_mm_shuffle3311_ps(this->values), this->values)); //(x,x,x,1)
        } else
#endif
        {
            return InBaseDef(hmax());
        }
    }

    /**
     * Minimum element of this object
     * @returns SIMDInBase set to the smallest value.
     */
    XS_FUNCTION InBaseDef hminInBase() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return InBaseDef(_mm_min_ss(_mm_shuffle3311_ps(this->values), this->values)); //(x,x,x,1)
        } else
#endif
        {
            return InBaseDef(hmin());
        }
    }

    /**
     * Maximum element of this object along with the max elements index.
     * @param [out] index Return value used to pass back the index of the returned element.
     * @returns BaseDef set to the largest value.
     */
    XS_FUNCTION BaseDef hmaxIndex(uint32& index) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 other = _mm_max_ss(_mm_shuffle3311_ps(this->values), this->values); //(x,x,x,1)
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
            return BaseDef(other);
        }
    }

    /**
     * Minimum element of this object along with the max elements index.
     * @param [out] index Return value used to pass back the Index of the returned element.
     * @returns BaseDef set to the smallest value.
     */
    XS_FUNCTION BaseDef hminIndex(uint32& index) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            __m128 other = _mm_min_ss(_mm_shuffle3311_ps(this->values), this->values); //(x,x,x,1)
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
            return BaseDef(other);
        }
    }

    /**
     * Approximate reciprocal (1/other) of object per element.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD2 reciprocal() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 recip = _mm_recip_ps(this->values);
            return SIMD2(_mm_fnmadd_ps(_mm_mul_ps(recip, recip), this->values, _mm_add_ps(recip, recip)));
        } else
#endif
        {
            return SIMD2(Shift::recip<T>(this->values0), Shift::recip<T>(this->values1));
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
            const __m128 val = _mm_add_ss(_mm_shuffle3311_ps(this->values), this->values); //(x,x,x,1)
            return BaseDef(_mm_shuffle0000_ps(val));
        } else
#endif
        {
            return BaseDef(this->values0 + this->values1);
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
            return InBaseDef(_mm_add_ss(_mm_shuffle3311_ps(this->values), this->values)); //(x,x,x,1)
        } else
#endif
        {
            return InBaseDef(hadd());
        }
    }

    /**
     * Compute the difference of all elements.
     * @returns BaseDef set to the difference of all elements.
     */
    XS_FUNCTION BaseDef hsub() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 val = _mm_sub_ss(this->values, _mm_shuffle3311_ps(this->values)); //(x,x,x,1)
            return BaseDef(_mm_shuffle0000_ps(val));
        } else
#endif
        {
            return BaseDef(this->values0 - this->values1);
        }
    }

    /**
     * Compute the difference of all elements.
     * @returns SIMDInBase set to the difference of all elements.
     */
    XS_FUNCTION InBaseDef hsubInBase() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return InBaseDef(_mm_sub_ss(this->values, _mm_shuffle3311_ps(this->values))); //(x,x,x,1)
        } else
#endif
        {
            return InBaseDef(hsub());
        }
    }

    /**
     * Round up each element of the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD2 ceil() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(_mm_ceil_ps(this->values));
        } else
#endif
        {
            return SIMD2(Shift::ceil<T>(this->values0), Shift::ceil<T>(this->values1));
        }
    }

    /**
     * Round down each element of the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD2 floor() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(_mm_floor_ps(this->values));
        } else
#endif
        {
            return SIMD2(Shift::floor<T>(this->values0), Shift::floor<T>(this->values1));
        }
    }

    /**
     * Round toward zero the object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMD2 trunc() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(_mm_trunc_ps(this->values));
        } else
#endif
        {
            return SIMD2(Shift::trunc<T>(this->values0), Shift::trunc<T>(this->values1));
        }
    }

    /**
     * Returns the sqrt of each element in the object.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD2 sqrt() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(_mm_sqrt_ps(this->values));
        } else
#endif
        {
            return SIMD2(Shift::sqrt<T>(this->values0), Shift::sqrt<T>(this->values1));
        }
    }

    /**
     * Approximate reciprocal square root of each element in the object.
     * @note Useful as the reciprocal square root is faster to determine.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD2 rsqrt() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 recip = _mm_recipsqrt_ps(this->values);
            const __m128 val1 = _mm_mul_ps(_mm_mul_ps(_mm_set1_ps(0.5f), this->values), recip);
            const __m128 val2 = _mm_fnmadd_ps(recip, val1, _mm_set1_ps(1.5f));
            return SIMD2(_mm_mul_ps(recip, val2));
        } else
#endif
        {
            return SIMD2(Shift::rsqrt<T>(this->values0), Shift::rsqrt<T>(this->values1));
        }
    }

    /**
     * Returns binary exponential of each element in the object.
     * @returns Object containing the binary exponential of the input.
     */
    XS_FUNCTION SIMD2 exp2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(NoExport::exp2f4(this->values));
        } else
#endif
        {
            return SIMD2(Shift::exp2<T>(this->values0), Shift::exp2<T>(this->values1));
        }
    }

    /**
     * Returns exponential of each element in the object.
     * @returns Object containing the exponential of the input.
     */
    XS_FUNCTION SIMD2 exp() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(NoExport::expf4(this->values));
        } else
#endif
        {
            return SIMD2(Shift::exp<T>(this->values0), Shift::exp<T>(this->values1));
        }
    }

    /**
     * Returns binary logarithm of each element in the object.
     * @returns Object containing the binary logarithm of the input.
     */
    XS_FUNCTION SIMD2 log2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(NoExport::log2f4(this->values));
        } else
#endif
        {
            return SIMD2(Shift::log2<T>(this->values0), Shift::log2<T>(this->values1));
        }
    }

    /**
     * Returns natural logarithm of each element in the object.
     * @returns Object containing the logarithm of the input.
     */
    XS_FUNCTION SIMD2 log() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(NoExport::logf4(this->values));
        } else
#endif
        {
            return SIMD2(Shift::log<T>(this->values0), Shift::log<T>(this->values1));
        }
    }

    /**
     * Returns object to the power of the elements in another object.
     * @param other The object containing the exponents.
     * @returns Object containing the input values to the power of the second inputs.
     */
    XS_FUNCTION SIMD2 pow(const SIMD2& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(NoExport::powf4(this->values, other.values));
        } else
#endif
        {
            return SIMD2(Shift::pow<T>(this->values0, other.values0), Shift::pow<T>(this->values1, other.values1));
        }
    }

    /**
     * Returns object (where all elements must be > 0) to the power of the elements in another object.
     * @note The current object must have all elements > 0.
     * @param other The object containing the exponents.
     * @returns Object containing the input values to the power of the second inputs.
     */
    XS_FUNCTION SIMD2 powr(const SIMD2& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(NoExport::powrf4(this->values, other.values));
        } else
#endif
        {
            return SIMD2(Shift::powr<T>(this->values0, other.values0), Shift::powr<T>(this->values1, other.values1));
        }
    }

    /**
     * Returns object to the power of another value.
     * @param other The value containing the exponent.
     * @returns Object containing the input values to the power of the second input.
     */
    XS_FUNCTION SIMD2 pow(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(NoExport::powf4(this->values, other.values));
        } else
#endif
        {
            return SIMD2(Shift::pow<T>(this->values0, other.value), Shift::pow<T>(this->values1, other.value));
        }
    }

    /**
     * Returns object (where all elements must be > 0) to the power of another value.
     * @note The current object must have all elements > 0.
     * @param other The value containing the exponent.
     * @returns Object containing the input values to the power of the second input.
     */
    XS_FUNCTION SIMD2 powr(BaseDef other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(NoExport::powrf4(this->values, other.values));
        } else
#endif
        {
            return SIMD2(Shift::powr<T>(this->values0, other.value), Shift::powr<T>(this->values1, other.value));
        }
    }

    /**
     * Returns sine of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the sined values.
     */
    XS_FUNCTION SIMD2 sin() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(NoExport::sinf4(this->values));
        } else
#endif
        {
            return SIMD2(Shift::sin<T>(this->values0), Shift::sin<T>(this->values1));
        }
    }

    /**
     * Returns cosine of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the cosined values.
     */
    XS_FUNCTION SIMD2 cos() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(NoExport::cosf4(this->values));
        } else
#endif
        {
            return SIMD2(Shift::cos<T>(this->values0), Shift::cos<T>(this->values1));
        }
    }

    /**
     * Returns tangent of each element in the object.
     * @note Current object must have values in radians.
     * @returns object containing the tangented values.
     */
    XS_FUNCTION SIMD2 tan() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(NoExport::tanf4(this->values));
        } else
#endif
        {
            return SIMD2(Shift::tan<T>(this->values0), Shift::tan<T>(this->values1));
        }
    }

    /**
     * Returns sine and cosine of each element in the object.
     * @note This is provided because it is much quicker to determine the cosine of a set of values if the sine is
     * already known.
     * @param [out] cosReturn The cosine return values.
     * @returns Object containing the sined values.
     */
    XS_FUNCTION SIMD2 sincos(SIMD2& cosReturn) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(NoExport::sincosf4(this->values, cosReturn.values));
        } else
#endif
        {
            SIMD2 ret;
            ret.values0 = Shift::sincos<T>(this->values0, cosReturn.values0);
            ret.values1 = Shift::sincos<T>(this->values1, cosReturn.values1);
            return ret;
        }
    }

    /**
     * Returns arcsine of each element in the object.
     * @returns Object containing the arcsined values (results in radians).
     */
    XS_FUNCTION SIMD2 asin() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(NoExport::asinf4(this->values));
        } else
#endif
        {
            return SIMD2(Shift::asin<T>(this->values0), Shift::asin<T>(this->values1));
        }
    }

    /**
     * Returns arccosine of each element in the object.
     * @returns Object containing the arccosined values (results in radians).
     */
    XS_FUNCTION SIMD2 acos() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(NoExport::acosf4(this->values));
        } else
#endif
        {
            return SIMD2(Shift::acos<T>(this->values0), Shift::acos<T>(this->values1));
        }
    }

    /**
     * Returns arctangent of each element in the object.
     * @returns Object containing the arctangented values (results in radians).
     */
    XS_FUNCTION SIMD2 atan() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(NoExport::atanf4(this->values));
        } else
#endif
        {
            return SIMD2(Shift::atan<T>(this->values0), Shift::atan<T>(this->values1));
        }
    }

    /**
     * Returns the distance between a planes positive x-axis and the points given by the input objects.
     * @param other The object containing the second parameter of each of the three input points.
     * @returns Object containing the angles (results in radians).
     */
    XS_FUNCTION SIMD2 atan2(const SIMD2& other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMD2(NoExport::atan2f4(this->values, other.values));
        } else
#endif
        {
            return SIMD2(Shift::atan2<T>(this->values0, other.values0), Shift::atan2<T>(this->values1, other.values1));
        }
    }

    /**
     * Extracts an element from a object at an index and inserts it into another object at a specified index.
     * @tparam Index0 The index in the first object to insert the element (range is 0-1).
     * @tparam Index1 The index in the second object to copy element from (range is 0-1).
     * @param other The object to extract an element from to insert into the first.
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD2 insert(const SIMD2& other) const noexcept
    {
        static_assert(Index0 < 2 && Index1 < 2, "Invalid Index: Indexes must be <2");
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD2(_mm_move_ss(this->values, other.values));
            } else if constexpr (hasISAFeature<ISAFeature::SSE41> && Index0 == Index1) {
                return SIMD2(_mm_blend_ps(this->values, other.values, 1UL << Index0));
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                return SIMD2(_mm_insert_ps(this->values, other.values, _MM_MK_INSERTPS_NDX(Index1, Index0, 0)));
            } else if constexpr (Index0 == 0 && Index1 == 1) {
                return SIMD2(_mm_move_ss(this->values, _mm_shuffle3311_ps(other.values))); //(x,x,x,1)
            } else if constexpr (Index0 == 1 && Index1 == 0) {
                const __m128 val = _mm_movelh_ps(this->values, other.values); //(x,b0,x,a0)
                return SIMD2(_mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 2, 0)));
            } else /*Index0 == 1, Index1 == 1*/ {
                const __m128 val = _mm_movelh_ps(this->values, other.values); //(b1,x,x,a0)
                return SIMD2(_mm_shuffle_ps(val, this->values, _MM_SHUFFLE(3, 2, 3, 0)));
            }
        } else
#endif
        {
            return SIMD2(Index0 == 0 ? (&other.values0)[Index1] : this->values0,
                Index0 == 1 ? (&other.values0)[Index1] : this->values1);
        }
    }

    /**
     * Blends values from the second object into the first based on a mask.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to copy the first element of the first input into the returned object.
     * @tparam Elem1 Whether to copy the second element of the first input into the returned object.
     * @param other The object to blend into the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1>
    XS_FUNCTION SIMD2 blend(const SIMD2& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1) {
            return *this;
        } else if constexpr (Elem0 && Elem1) {
            return other;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Elem0 && !Elem1) {
                return SIMD2(_mm_move_ss(this->values, other.values));
            } else /*!Elem0 && Elem1*/ {
                return SIMD2(_mm_move_ss(other.values, this->values));
            }
        }
#endif
        else {
            return SIMD2(Elem0 ? other.values0 : this->values0, Elem1 ? other.values1 : this->values1);
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
    XS_FUNCTION SIMD2 blendVar(const SIMD2& other, const Mask& mask) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                return SIMD2(_mm_mask_blend_ps(mask.values, this->values, other.values));
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                return SIMD2(_mm_blendv_ps(this->values, other.values, mask.values));
            } else {
                const __m128 val0 = _mm_and_ps(other.values, mask.values);
                const __m128 val1 = _mm_andnot_ps(mask.values, this->values);
                return SIMD2(_mm_or_ps(val0, val1));
            }
        } else
#endif
        {
            return SIMD2<T, Width>(
                (mask.values0) ? other.values0 : this->values0, (mask.values1) ? other.values1 : this->values1);
        }
    }

    /**
     * Swaps values between the second object and the first based on a mask.
     * @note A value form the second object is copied into the first based on the corresponding input elements.
     * @tparam Elem0 Whether to swap the first elements.
     * @tparam Elem1 Whether to swap the second elements.
     * @param [in,out] other The object to swap elements with the first.
     * @returns The result of the operation.
     */
    template<bool Elem0, bool Elem1>
    XS_FUNCTION SIMD2 blendSwap(SIMD2& other) const noexcept
    {
        if constexpr (!Elem0 && !Elem1) {
            return *this;
        } else if constexpr (Elem0 && Elem1) {
            const SIMD2 backup = other;
            other = *this;
            return backup;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Elem0 && !Elem1) {
                const __m128 backup = other.values;
                other.values = _mm_move_ss(other.values, this->values);
                return SIMD2(_mm_move_ss(this->values, backup));
            } else /*!Elem0 && Elem1*/ {
                const __m128 backup = other.values;
                other.values = _mm_move_ss(this->values, other.values);
                return SIMD2(_mm_move_ss(backup, this->values));
            }
        }
#endif
        else {
            const SIMD2 backup = other;
            other.values0 = (Elem0) ? this->values0 : other.values0;
            other.values1 = (Elem1) ? this->values1 : other.values1;
            return SIMD2((Elem0) ? backup.values0 : this->values0, (Elem1) ? backup.values1 : this->values1);
        }
    }

    /**
     * Swaps values between the second object and the first based on a dynamic mask.
     * @note A value form the second object is swapped with the first based on the corresponding input elements.
     * @param [in,out] other The object to swap elements with the first.
     * @param          mask The object mask used to define which elements to swap.
     * @returns The result of the operation.
     */
    XS_FUNCTION SIMD2 blendSwapVar(SIMD2& other, const Mask& mask) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                const __m128 backup = other.values;
                other.values = _mm_mask_blend_ps(mask.values, other.values, this->values);
                return SIMD2(_mm_mask_blend_ps(mask.values, this->values, backup));
            } else if constexpr (hasISAFeature<ISAFeature::SSE41>) {
                const __m128 backup = other.values;
                other.values = _mm_blendv_ps(other.values, this->values, mask.values);
                return SIMD2(_mm_blendv_ps(this->values, backup, mask.values));
            } else {
                const __m128 val0A = _mm_and_ps(this->values, mask.values);
                const __m128 val0B = _mm_and_ps(other.values, mask.values);
                const __m128 val1A = _mm_andnot_ps(mask.values, other.values);
                const __m128 val1B = _mm_andnot_ps(mask.values, this->values);
                other.values = _mm_or_ps(val0A, val1A);
                return SIMD2(_mm_or_ps(val0B, val1B));
            }
        } else
#endif
        {
            const SIMD2<T, Width> back = other;
            other.values0 = (mask.values0) ? this->values0 : other.values0;
            other.values1 = (mask.values1) ? this->values1 : other.values1;
            return SIMD2<T, Width>(
                (mask.values0) ? back.values0 : this->values0, (mask.values1) ? back.values1 : this->values1);
        }
    }

    /**
     * Shuffles the elements of the object.
     * @tparam Index0 The Index of the first element to insert into the returned object (range is 0-1).
     * @tparam Index1 The Index of the second element to insert into the returned object (range is 0-1).
     * @returns The result of the operation.
     */
    template<uint32 Index0, uint32 Index1>
    XS_FUNCTION SIMD2 shuffle() const noexcept
    {
        static_assert(Index0 < 2 && Index1 < 2, "Invalid Index: Indexes must be <2");
        if constexpr (Index0 == 0 && Index1 == 1) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (Index0 == 0 && Index1 == 0) {
                return SIMD2(_mm_shuffle1100_ps(this->values));
            } else if constexpr (Index0 == 1 && Index1 == 1) {
                return SIMD2(_mm_shuffle3311_ps(this->values));
            } else {
                return SIMD2(_mm_shuffle1_ps(this->values, _MM_SHUFFLE(3, 2, Index1, Index0)));
            }
        }
#endif
        else {
            return SIMD2<T, Width>((&this->values0)[Index0], (&this->values0)[Index1]);
        }
    }
};

/**
 * Add two SIMD2s.
 * @param other1 The first SIMD2.
 * @param other2 SIMD2 to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator+(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_add_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.values0 + other2.values0), (other1.values1 + other2.values1));
    }
}

/**
 * Add a value to all elements of the SIMD2.
 * @param other1 The SIMD2.
 * @param other2 Value to add to the SIMD2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator+(const SIMD2<T, Width>& other1, typename SIMD2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_add_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.values0 + other2.value), (other1.values1 + other2.value));
    }
}

/**
 * Subtract a SIMD2 from another SIMD2.
 * @param other1 The first SIMD2.
 * @param other2 SIMD2 to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator-(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.values0 - other2.values0), (other1.values1 - other2.values1));
    }
}

/**
 * Subtract a value from all elements of the SIMD2.
 * @param other1 The SIMD2.
 * @param other2 Value to subtract from the SIMD2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator-(const SIMD2<T, Width>& other1, typename SIMD2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.values0 - other2.value), (other1.values1 - other2.value));
    }
}

/**
 * Subtract all elements of a SIMD2 from a value.
 * @param other1 The SIMD2.
 * @param other2 Value to subtract from the SIMD2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator-(typename SIMD2<T, Width>::BaseDef other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_sub_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.value - other2.values0), (other1.value - other2.values1));
    }
}

/**
 * Multiply two SIMD2s per element.
 * @param other1 The first SIMD2.
 * @param other2 The second SIMD2 to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator*(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_mul_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.values0 * other2.values0), (other1.values1 * other2.values1));
    }
}

/**
 * Multiply a SIMD2 by a value.
 * @param other1 The SIMD2.
 * @param other2 Value to multiply the SIMD2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator*(const SIMD2<T, Width>& other1, typename SIMD2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_mul_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.values0 * other2.value), (other1.values1 * other2.value));
    }
}

/**
 * Divide two SIMD2s per element.
 * @param other1 The first SIMD2.
 * @param other2 The second SIMD2 to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator/(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.values0 / other2.values0), (other1.values1 / other2.values1));
    }
}

/**
 * Divide a SIMD2 by a value.
 * @param other1 The SIMD2.
 * @param other2 Value to divide the SIMD2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator/(const SIMD2<T, Width>& other1, typename SIMD2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.values0 / other2.value), (other1.values1 / other2.value));
    }
}

/**
 * Divide a value by all elements of the SIMD2.
 * @param other1 The SIMD2.
 * @param other2 Value to divide the SIMD2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator/(typename SIMD2<T, Width>::BaseDef other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_div_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>((other1.value / other2.values0), (other1.value / other2.values1));
    }
}

/**
 * Negate all elements of the SIMD2.
 * @param other The SIMD2 to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator-(const SIMD2<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2<T, Width>(_mm_sub_ps(_mm_setzero_ps(), other.values));
    } else
#endif
    {
        return SIMD2<T, Width>(-other.values0, -other.values1);
    }
}

/**
 * Perform compound assignment and addition with the SIMD2.
 * @param [in,out] other1 The first SIMD2.
 * @param          other2 Object to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width>& operator+=(SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_add_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 += other2.values0;
        other1.values1 += other2.values1;
    }
    return other1;
}

/**
 * Perform compound assignment and addition with the SIMD2.
 * @param [in,out] other1 The SIMD2.
 * @param          other2 Value to add to the first SIMD2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width>& operator+=(SIMD2<T, Width>& other1, typename SIMD2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_add_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 += other2.value;
        other1.values1 += other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction by the SIMD2.
 * @param [in,out] other1 The first SIMD2.
 * @param          other2 Object to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width>& operator-=(SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_sub_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 -= other2.values0;
        other1.values1 -= other2.values1;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction by a value.
 * @param [in,out] other1 The SIMD2.
 * @param          other2 Value to subtract from the first SIMD2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width>& operator-=(SIMD2<T, Width>& other1, typename SIMD2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_sub_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 -= other2.value;
        other1.values1 -= other2.value;
    }
    return other1;
}

/**
 * Perform SIMD2 compound assignment and multiplication.
 * @param [in,out] other1 The first SIMD2.
 * @param          other2 Object to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width>& operator*=(SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_mul_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 *= other2.values0;
        other1.values1 *= other2.values1;
    }
    return other1;
}

/**
 * Perform compound assignment and multiplication by a value.
 * @param [in,out] other1 The SIMD2.
 * @param          other2 Value to multiply the SIMD2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width>& operator*=(SIMD2<T, Width>& other1, typename SIMD2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_mul_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 *= other2.value;
        other1.values1 *= other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and division.
 * @param [in,out] other1 The SIMD2.
 * @param          other2 Value to divide the SIMD2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width>& operator/=(SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_div_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 /= other2.values0;
        other1.values1 /= other2.values1;
    }
    return other1;
}

/**
 * Perform compound assignment and division by a value.
 * @param [in,out] other1 The SIMD2.
 * @param          other2 Value to divide the SIMD2 by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width>& operator/=(SIMD2<T, Width>& other1, typename SIMD2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_div_ps(other1.values, other2.values);
    } else
#endif
    {
        other1.values0 /= other2.value;
        other1.values1 /= other2.value;
    }
    return other1;
}

/**
 * Compare two SIMD2s are equal.
 * @param other1 The first SIMD2.
 * @param other2 The second SIMD2 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator==(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_NEQ_UQ);
            val = _mm_shuffle1010_ps(val);
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values, other2.values)) & 0x3);
        }
    } else
#endif
    {
        return ((other1.values0 == other2.values0) & (other1.values1 == other2.values1));
    }
}

/**
 * Compare a SIMD2 to a value.
 * @param other1 The SIMD2.
 * @param other2 The value to compare to the SIMD2.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator==(const SIMD2<T, Width>& other1, typename SIMD2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_NEQ_UQ);
            val = _mm_shuffle1010_ps(val);
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpneq_ps(other1.values, other2.values)) & 0x3);
        }
    } else
#endif
    {
        return ((other1.values0 == other2.value) & (other1.values1 == other2.value));
    }
}

/**
 * Compare two SIMD2s are less or equal.
 * @param other1 The first SIMD2.
 * @param other2 The second SIMD2 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<=(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ);
            val = _mm_shuffle1010_ps(val);
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values, other2.values)) & 0x3);
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.values0) & (other1.values1 <= other2.values1));
    }
}

/**
 * Compare SIMD2 is less than or equal to a value.
 * @param other1 The SIMD2.
 * @param other2 The value to compare to the SIMD2.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<=(const SIMD2<T, Width>& other1, typename SIMD2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_NLE_UQ);
            val = _mm_shuffle1010_ps(val);
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnle_ps(other1.values, other2.values)) & 0x3);
        }
    } else
#endif
    {
        return ((other1.values0 <= other2.value) & (other1.values1 <= other2.value));
    }
}

/**
 * Compare two SIMD2s are less than.
 * @param other1 The first SIMD2.
 * @param other2 The second SIMD2 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ);
            val = _mm_shuffle1010_ps(val);
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values, other2.values)) & 0x3);
        }
    } else
#endif
    {
        return ((other1.values0 < other2.values0) & (other1.values1 < other2.values1));
    }
}

/**
 * Compare SIMD2 is less than a value.
 * @param other1 The SIMD2.
 * @param other2 The value to compare to the SIMD2.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator<(const SIMD2<T, Width>& other1, typename SIMD2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_NLT_UQ);
            val = _mm_shuffle1010_ps(val);
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpnlt_ps(other1.values, other2.values)) & 0x3);
        }
    } else
#endif
    {
        return ((other1.values0 < other2.value) & (other1.values1 < other2.value));
    }
}

/**
 * Compare two SIMD2s are not equal.
 * @param other1 The first SIMD2.
 * @param other2 The second SIMD2 to compare to the first.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator!=(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_EQ_UQ);
            val = _mm_shuffle1010_ps(val);
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpeq_ps(other1.values, other2.values)) & 0x3);
        }
    } else
#endif
    {
        return ((other1.values0 != other2.values0) & (other1.values1 != other2.values1));
    }
}

/**
 * Compare SIMD2 is not equal to a value.
 * @param other1 The SIMD2.
 * @param other2 The value to compare to the SIMD2.
 * @returns Boolean signalling if compare was valid for every element of the input.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION bool operator!=(const SIMD2<T, Width>& other1, typename SIMD2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX>) {
            __m128 val = _mm_cmp_ps(other1.values, other2.values, _CMP_EQ_UQ);
            val = _mm_shuffle1010_ps(val);
            return static_cast<bool>(_mm_testz_ps(val, val));
        } else {
            return !static_cast<bool>(_mm_movemask_ps(_mm_cmpeq_ps(other1.values, other2.values)) & 0x3);
        }
    } else
#endif
    {
        return ((other1.values0 != other2.value) & (other1.values1 != other2.value));
    }
}

/**
 * And 2 SIMD2s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD2.
 * @param other2 Object to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator&(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_and_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>(
            Shift::bitAnd<T>(other1.values0, other2.values0), Shift::bitAnd<T>(other1.values1, other2.values1));
    }
}

/**
 * And a SIMD2 with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD2.
 * @param other2 Value to and with the SIMD2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator&(const SIMD2<T, Width>& other1, typename SIMD2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_and_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>(
            Shift::bitAnd<T>(other1.values0, other2.value), Shift::bitAnd<T>(other1.values1, other2.value));
    }
}

/**
 * Or 2 SIMD2s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD2.
 * @param other2 Object to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator|(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_or_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>(
            Shift::bitOr<T>(other1.values0, other2.values0), Shift::bitOr<T>(other1.values1, other2.values1));
    }
}

/**
 * Or a SIMD2s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD2.
 * @param other2 Value to and with the SIMD2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator|(const SIMD2<T, Width>& other1, typename SIMD2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_or_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>(
            Shift::bitOr<T>(other1.values0, other2.value), Shift::bitOr<T>(other1.values1, other2.value));
    }
}

/**
 * Xor 2 SIMD2s.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first SIMD2.
 * @param other2 Object to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator^(const SIMD2<T, Width>& other1, const SIMD2<T, Width>& other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_xor_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>(
            Shift::bitXor<T>(other1.values0, other2.values0), Shift::bitXor<T>(other1.values1, other2.values1));
    }
}

/**
 * Xor a SIMD2s with a value.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The SIMD2.
 * @param other2 Value to and with the SIMD2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator^(const SIMD2<T, Width>& other1, typename SIMD2<T, Width>::BaseDef other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMD2(_mm_xor_ps(other1.values, other2.values));
    } else
#endif
    {
        return SIMD2<T, Width>(
            Shift::bitXor<T>(other1.values0, other2.value), Shift::bitXor<T>(other1.values1, other2.value));
    }
}

/**
 * Not the SIMD2.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other The SIMD2.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMD2<T, Width> operator~(const SIMD2<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_undefined_ps();
        return SIMD2(_mm_xor_ps(other.values, _mm_cmpeq_ps(val, val)));
    } else
#endif
    {
        return SIMD2<T, Width>(Shift::bitNot<T>(other.values0), Shift::bitNot<T>(other.values1));
    }
}

/**
 * And 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD2<T, Width>::Mask operator&(
    const typename SIMD2<T, Width>::Mask& mask1, const typename SIMD2<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD2<T, Width>::Mask(_kand_mask8(mask1.values, mask2.values));
        } else {
            return SIMD2<T, Width>::Mask(_mm_and_ps(mask1.values, mask2.values));
        }
    } else
#endif
    {
        return SIMD2<T, Width>::Mask(mask1.values0 & mask2.values0, mask1.values1 & mask2.values1);
    }
}

/**
 * Or 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to or with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD2<T, Width>::Mask operator|(
    const typename SIMD2<T, Width>::Mask& mask1, const typename SIMD2<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD2<T, Width>::Mask(_kor_mask8(mask1.values, mask2.values));
        } else {
            return SIMD2<T, Width>::Mask(_mm_or_ps(mask1.values, mask2.values));
        }
    } else
#endif
    {
        return SIMD2<T, Width>::Mask(mask1.values0 | mask2.values0, mask1.values1 | mask2.values1);
    }
}

/**
 * Xor 2 Masks.
 * @param mask1 The first mask.
 * @param mask2 Mask to xor with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD2<T, Width>::Mask operator^(
    const typename SIMD2<T, Width>::Mask& mask1, const typename SIMD2<T, Width>::Mask& mask2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD2<T, Width>::Mask(_kxor_mask8(mask1.values, mask2.values));
        } else {
            return SIMD2<T, Width>::Mask(_mm_xor_ps(mask1.values, mask2.values));
        }
    } else
#endif
    {
        return SIMD2<T, Width>::Mask(mask1.values0 ^ mask2.values0, mask1.values1 ^ mask2.values1);
    }
}

/**
 * Not a Mask.
 * @param mask The mask.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION typename SIMD2<T, Width>::Mask operator~(const typename SIMD2<T, Width>::Mask& mask) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
            return SIMD2<T, Width>::Mask(_knot_mask8(mask.values));
        } else {
            return SIMD2<T, Width>::Mask(_mm_xor_ps(mask.values, _mm_cmpeq_ps(mask.values, mask.values)));
        }
    } else
#endif
    {
        return SIMD2<T, Width>::Mask(!mask.values0, !mask.values1);
    }
}
} // namespace Shift
