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

#include "SIMD/XSSIMDInBase.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class SIMDBase;

template<typename T>
class SIMDBaseData
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    T value;

    /** Default constructor. */
    XS_INLINE SIMDBaseData() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE SIMDBaseData(const SIMDBaseData& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE SIMDBaseData(SIMDBaseData&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMDBaseData& operator=(const SIMDBaseData& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMDBaseData& operator=(SIMDBaseData&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit SIMDBaseData(const SIMDBase<T, Width> other) noexcept
    {
        store(other);
    }

    /**
     * Directly set the contents of this object.
     * @note This function is used to set data using a pre-calculated value.
     * This removes the necessity to load and then store.
     * @param other The pre-calculated value.
     */
    XS_INLINE void setData(const T other) noexcept
    {
        value = other;
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(SIMDBase<T, Width> other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                _mm_store_ss(&value, _mm512_castps512_ps128(other.values));
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                _mm_store_ss(&value, _mm256_castps256_ps128(other.values));
            } else {
                _mm_store_ss(&value, other.values);
            }
        } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                value = static_cast<T>(_mm_extract_epi8(_mm512_castsi512_si128(other.values), 0));
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                value = static_cast<T>(_mm_extract_epi8(_mm256_castsi256_si128(other.values), 0));
            } else {
                value = static_cast<T>(_mm_extract_epi8(other.values, 0));
            }
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                _mm_storeu_si16(&value, _mm512_castsi512_si128(other.values));
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                _mm_storeu_si16(&value, _mm256_castsi256_si128(other.values));
            } else {
                _mm_storeu_si16(&value, other.values);
            }
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                _mm_storeu_si32(&value, _mm512_castsi512_si128(other.values));
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                _mm_storeu_si32(&value, _mm256_castsi256_si128(other.values));
            } else {
                _mm_storeu_si32(&value, other.values);
            }
        } else
#endif
        {
            value = other.value;
        }
    }

    /**
     * Load from memory.
     * @tparam Width Type of SIMD being used.
     * @returns The loaded object.
     */
    template<SIMDWidth Width = defaultWidthSIMD<T>>
    XS_INLINE SIMDBase<T, Width> load() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase<T, Width>(_mm512_set1_ps(value));
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase<T, Width>(_mm256_broadcast_ss(&value));
            } else {
                if constexpr (hasISAFeature<ISAFeature::AVX>) {
                    return SIMDBase<T, Width>(_mm_broadcast_ss(&value));
                } else {
                    const __m128 val = _mm_load_ss(&value);
                    return SIMDBase<T, Width>(_mm_shuffle1_ps(val, _MM_SHUFFLE(0, 0, 0, 0)));
                }
            }
        } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase<T, Width>(_mm512_set1_8(value));
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase<T, Width>(_mm256_set1_8(&value));
            } else {
                return SIMDBase<T, Width>(_mm_set1_8(&value));
            }
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase<T, Width>(_mm512_set1_epi16(value));
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase<T, Width>(_mm256_set1_epi16(&value));
            } else {
                return SIMDBase<T, Width>(_mm_set1_epi16(&value));
            }
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase<T, Width>(_mm512_set1_epi32(value));
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase<T, Width>(_mm256_set1_epi32(&value));
            } else {
                return SIMDBase<T, Width>(_mm_set1_epi32(&value));
            }
        } else
#endif
        {
            return SIMDBase<T, Width>(value);
        }
    }
};

template<typename T>
using SIMDBaseDataPad = SIMDBaseData<T>;

template<typename T, SIMDWidth Width = defaultWidthSIMD<T>>
class SIMDBase : public NoExport::SIMDData<T, 1, numValues<T, Width> - 1, Width>
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    using Type = T;
    using InternalData = NoExport::SIMDData<T, 1, numValues<T, Width> - 1, Width>;
    using Data = SIMDBaseData<T>;
    using DataPad = SIMDBaseDataPad<T>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = InternalData::width;
    static constexpr uint32 numValues = InternalData::numValues;
    static constexpr uint32 totalValues = InternalData::totalValues;
    using InBaseDef = SIMDInBase<T, SIMDInBase<T, widthImpl>::widthImpl>;
    using InternalData::SIMDData;

    /** Default constructor. */
    XS_INLINE SIMDBase() noexcept = default;

    /**
     * Constructor
     * @param other The other.
     */
    XS_INLINE SIMDBase(const SIMDBase& other) noexcept = default;

    /**
     * Constructor
     * @param other The other.
     */
    XS_INLINE SIMDBase(SIMDBase&& other) noexcept = default;

    /**
     * Assignment operator
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMDBase& operator=(const SIMDBase& other) noexcept = default;

    /**
     * Move assignment operator
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMDBase& operator=(SIMDBase&& other) noexcept = default;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_INLINE explicit SIMDBase(const SIMDBase<T, Width2> other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar) && (Width2 > SIMDWidth::Scalar)) {
            if constexpr (Width == Width2) {
                this->values = other.values;
            } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B32) &&
                (Width2 >= SIMDWidth::B64)) {
                this->values = _mm512_castps512_ps256(other.values);
            } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B16) &&
                (Width2 >= SIMDWidth::B64)) {
                this->values = _mm512_castps512_ps128(other.values);
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B16) &&
                (Width2 >= SIMDWidth::B32)) {
                this->values = _mm256_castps256_ps128(other.values);
            } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64) &&
                (Width2 >= SIMDWidth::B16)) {
                this->values = _mm512_broadcastf128_ps(other.values);
            } else if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64) &&
                (Width2 >= SIMDWidth::B32)) {
                this->values = _mm512_broadcastf256_ps(other.values);
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32) &&
                (Width2 >= SIMDWidth::B16)) {
                this->values = _mm256_broadcastf128_ps(other.values);
            }
        } else if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                this->values = _mm512_set1_ps(other.values);
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                this->values = _mm256_set1_ps(other.values);
            } else if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
                this->values = _mm_set1_ps(other.values);
            }
        } else if constexpr (Width2 > SIMDWidth::Scalar) {
            if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width2 >= SIMDWidth::B64)) {
                this->values = _mm_cvtss_f32(_mm512_castps512_ps128(other.values));
            } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width2 >= SIMDWidth::B32)) {
                this->values = _mm_cvtss_f32(_mm256_castps256_ps128(other.values));
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width2 >= SIMDWidth::B16)) {
                this->values = _mm_cvtss_f32(other.values);
            }
        } else
#endif
        {
            this->value = other.value;
        }
    }

    /**
     * Construct from a value.
     * @param value The value.
     */
    XS_INLINE explicit SIMDBase(const T value) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                this->values = _mm512_set1_ps(value);
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                this->values = _mm256_set1_ps(value);
            } else {
                this->values = _mm_set1_ps(value);
            }
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                this->values = _mm512_set1_epi32(static_cast<T>(value));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                this->values = _mm256_set1_epi32(static_cast<T>(value));
            } else {
                this->values = _mm_set1_epi32(static_cast<T>(value));
            }
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                this->values = _mm512_set1_epi16(static_cast<T>(value));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                this->values = _mm256_set1_epi16(static_cast<T>(value));
            } else {
                this->values = _mm_set1_epi16(static_cast<T>(value));
            }
        } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                this->values = _mm512_set1_epi8(static_cast<T>(value));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                this->values = _mm256_set1_epi8(static_cast<T>(value));
            } else {
                this->values = _mm_set1_epi8(static_cast<T>(value));
            }
        } else
#endif
        {
            this->value = value;
        }
    }

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE explicit SIMDBase(const InBaseDef other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_broadcastss_ps(other.values);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                this->values = _mm256_broadcastss_ps(other.values);
            } else {
                const __m128 val = _mm_shuffle0000_ps(other.values);
                this->values = _mm256_broadcastf128_ps(val);
            }
        } else if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = _mm_shuffle0000_ps(other.values);
        } else
#endif
        {
            this->value = other.value;
        }
    }

    /**
     * Convert to SIMDInBase
     * @returns Newly constructed type..
     */
    XS_INLINE explicit operator InBaseDef() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return InBaseDef(_mm512_castps512_ps128(this->values));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return InBaseDef(_mm256_castps256_ps128(this->values));
        } else if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return InBaseDef(this->values);
        } else
#endif
        {
            return InBaseDef(this->value);
        }
    }

    /**
     * Constructor to build set to 0.
     * @returns Newly constructed object with required attributes.
     */
    XS_INLINE static SIMDBase Zero() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase(_mm512_setzero_ps());
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase(_mm256_setzero_ps());
            } else {
                return SIMDBase(_mm_setzero_ps());
            }
        } else if constexpr (isSameAny<T, uint8, int8, uint16, int16, uint32, int32> && hasSIMD<T> &&
            (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase(_mm512_setzero_si512());
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase(_mm256_setzero_si256());
            } else {
                return SIMDBase(_mm_setzero_si128());
            }
        } else
#endif
        {
            return SIMDBase(T{0});
        }
    }

    /**
     * Constructor to build set to 1.
     * @returns Newly constructed object with required attributes.
     */
    XS_INLINE static SIMDBase One() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase(_mm512_set1_ps(1.0f));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase(_mm256_set1_ps(1.0f));
            } else {
                return SIMDBase(_mm_set1_ps(1.0f));
            }
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase(_mm512_set1_epi32(1));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase(_mm256_set1_epi32(1));
            } else {
                return SIMDBase(_mm_set1_epi32(1));
            }
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase(_mm512_set1_epi16(1));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase(_mm256_set1_epi16(1));
            } else {
                return SIMDBase(_mm_set1_epi16(1));
            }
        } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase(_mm512_set1_epi8(1));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase(_mm256_set1_epi8(1));
            } else {
                return SIMDBase(_mm_set1_epi8(1));
            }
        } else
#endif
        {
            return SIMDBase(T{1});
        }
    }

    /**
     * Access the object as a normal value.
     * @returns The value.
     */
    XS_INLINE T getValue() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return _mm_cvtss_f32(_mm512_castps512_ps128(this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return _mm_cvtss_f32(_mm256_castps256_ps128(this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return _mm_cvtss_f32(this->values);
        } else
#endif
        {
            return this->value;
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
    XS_INLINE SIMDBase mad(const SIMDBase other1, const SIMDBase other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                    return SIMDBase(_mm512_fmadd_ps(this->values, other1.values, other2.values));
                } else {
                    return SIMDBase(_mm512_add_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
                }
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                    return SIMDBase(_mm256_fmadd_ps(this->values, other1.values, other2.values));
                } else {
                    return SIMDBase(_mm256_add_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
                }
            } else {
                if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                    return SIMDBase(_mm_fmadd_ps(this->values, other1.values, other2.values));
                } else {
                    return SIMDBase(_mm_add_ps(_mm_mul_ps(this->values, other1.values), other2.values));
                }
            }
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase(_mm512_add_epi32(_mm512_mullo_epi32(this->values, other1.values), other2.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase(_mm256_add_epi32(_mm256_mullo_epi32(this->values, other1.values), other2.values));
            } else {
                return SIMDBase(_mm_add_epi32(_mm_mullo_epi32(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase(_mm512_add_epi16(_mm512_mullo_epi16(this->values, other1.values), other2.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase(_mm256_add_epi16(_mm256_mullo_epi16(this->values, other1.values), other2.values));
            } else {
                return SIMDBase(_mm_add_epi16(_mm_mullo_epi16(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                __m128i ret = _mm_add_epi16(_mm_mullo_epi16(_mm_cvtepu8_epi16(_mm512_castsi256_si128(this->values)),
                                                _mm_cvtepu8_epi16(_mm512_castsi256_si128(other1.values))),
                    _mm_cvtepu8_epi16(_mm512_castsi256_si128(other2.values)));
                ret = _mm_packus_epi16(ret, ret);
                return SIMDBase(_mm512_broadcasti128(ret));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                __m128i ret = _mm_add_epi16(_mm_mullo_epi16(_mm_cvtepu8_epi16(_mm256_castsi256_si128(this->values)),
                                                _mm_cvtepu8_epi16(_mm256_castsi256_si128(other1.values))),
                    _mm_cvtepu8_epi16(_mm256_castsi256_si128(other2.values)));
                ret = _mm_packus_epi16(ret, ret);
                return SIMDBase(_mm256_broadcasti128(ret));
            } else {
                const __m128i ret =
                    _mm_add_epi16(_mm_mullo_epi16(_mm_cvtepu8_epi16(this->values), _mm_cvtepu8_epi16(other1.values)),
                        _mm_cvtepu8_epi16(other2.values));
                return SIMDBase(_mm_packus_epi16(ret, ret));
            }
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                __m128i ret = _mm_add_epi16(_mm_mullo_epi16(_mm_cvtepi8_epi16(_mm512_castsi256_si128(this->values)),
                                                _mm_cvtepi8_epi16(_mm512_castsi256_si128(other1.values))),
                    _mm_cvtepi8_epi16(_mm512_castsi256_si128(other2.values)));
                ret = _mm_packus_epi16(ret, ret);
                return SIMDBase(_mm512_broadcasti128(ret));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                __m128i ret = _mm_add_epi16(_mm_mullo_epi16(_mm_cvtepi8_epi16(_mm256_castsi256_si128(this->values)),
                                                _mm_cvtepi8_epi16(_mm256_castsi256_si128(other1.values))),
                    _mm_cvtepi8_epi16(_mm256_castsi256_si128(other2.values)));
                ret = _mm_packus_epi16(ret, ret);
                return SIMDBase(_mm256_broadcasti128(ret));
            } else {
                const __m128i ret =
                    _mm_add_epi16(_mm_mullo_epi16(_mm_cvtepi8_epi16(this->values), _mm_cvtepi8_epi16(other1.values)),
                        _mm_cvtepi8_epi16(other2.values));
                return SIMDBase(_mm_packus_epi16(ret, ret));
            }
        } else
#endif
        {
            return SIMDBase(Shift::fma<T>(this->value, other1.value, other2.value));
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
    XS_INLINE SIMDBase msub(const SIMDBase other1, const SIMDBase other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                    return SIMDBase(_mm512_fmsub_ps(this->values, other1.values, other2.values));
                } else {
                    return SIMDBase(_mm512_sub_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
                }
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                    return SIMDBase(_mm256_fmsub_ps(this->values, other1.values, other2.values));
                } else {
                    return SIMDBase(_mm256_sub_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
                }
            } else {
                if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                    return SIMDBase(_mm_fmsub_ps(this->values, other1.values, other2.values));
                } else {
                    return SIMDBase(_mm_sub_ps(_mm_mul_ps(this->values, other1.values), other2.values));
                }
            }
        } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase(_mm512_sub_epi32(_mm512_mullo_epi32(this->values, other1.values), other2.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase(_mm256_sub_epi32(_mm256_mullo_epi32(this->values, other1.values), other2.values));
            } else {
                return SIMDBase(_mm_sub_epi32(_mm_mullo_epi32(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase(_mm512_sub_epi16(_mm512_mullo_epi16(this->values, other1.values), other2.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase(_mm256_sub_epi16(_mm256_mullo_epi16(this->values, other1.values), other2.values));
            } else {
                return SIMDBase(_mm_sub_epi16(_mm_mullo_epi16(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                __m128i ret = _mm_sub_epi16(_mm_mullo_epi16(_mm_cvtepu8_epi16(_mm512_castsi256_si128(this->values)),
                                                _mm_cvtepu8_epi16(_mm512_castsi256_si128(other1.values))),
                    _mm_cvtepu8_epi16(_mm512_castsi256_si128(other2.values)));
                ret = _mm_packus_epi16(ret, ret);
                return SIMDBase(_mm512_broadcasti128(ret));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                __m128i ret = _mm_sub_epi16(_mm_mullo_epi16(_mm_cvtepu8_epi16(_mm256_castsi256_si128(this->values)),
                                                _mm_cvtepu8_epi16(_mm256_castsi256_si128(other1.values))),
                    _mm_cvtepu8_epi16(_mm256_castsi256_si128(other2.values)));
                ret = _mm_packus_epi16(ret, ret);
                return SIMDBase(_mm256_broadcasti128(ret));
            } else {
                const __m128i ret =
                    _mm_sub_epi16(_mm_mullo_epi16(_mm_cvtepu8_epi16(this->values), _mm_cvtepu8_epi16(other1.values)),
                        _mm_cvtepu8_epi16(other2.values));
                return SIMDBase(_mm_packus_epi16(ret, ret));
            }
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                __m128i ret = _mm_sub_epi16(_mm_mullo_epi16(_mm_cvtepi8_epi16(_mm512_castsi256_si128(this->values)),
                                                _mm_cvtepi8_epi16(_mm512_castsi256_si128(other1.values))),
                    _mm_cvtepi8_epi16(_mm512_castsi256_si128(other2.values)));
                ret = _mm_packus_epi16(ret, ret);
                return SIMDBase(_mm512_broadcasti128(ret));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                __m128i ret = _mm_sub_epi16(_mm_mullo_epi16(_mm_cvtepi8_epi16(_mm256_castsi256_si128(this->values)),
                                                _mm_cvtepi8_epi16(_mm256_castsi256_si128(other1.values))),
                    _mm_cvtepi8_epi16(_mm256_castsi256_si128(other2.values)));
                ret = _mm_packus_epi16(ret, ret);
                return SIMDBase(_mm256_broadcasti128(ret));
            } else {
                const __m128i ret =
                    _mm_sub_epi16(_mm_mullo_epi16(_mm_cvtepi8_epi16(this->values), _mm_cvtepi8_epi16(other1.values)),
                        _mm_cvtepi8_epi16(other2.values));
                return SIMDBase(_mm_packus_epi16(ret, ret));
            }
        } else
#endif
        {
            return SIMDBase(Shift::fma<T>(this->value, other1.value, -other2.value));
        }
    }

    /**
     * Negate this object based on the sign of another.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @param other The second object whose sign to check.
     * @returns Result of operation.
     */
    XS_INLINE SIMDBase sign(const SIMDBase other) const noexcept
    {
        if constexpr (isUnsigned<T>) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase(_mm512_xor_ps(this->values, _mm512_and_ps(other.values, _mm512_set1_ps(-0.0f))));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase(_mm256_xor_ps(this->values, _mm256_and_ps(other.values, _mm256_set1_ps(-0.0f))));
            } else {
                return SIMDBase(_mm_xor_ps(this->values, _mm_and_ps(_mm_set1_ps(-0.0f), other.values)));
            }
        } else if constexpr (isSame<T, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase(_mm512_sign_epi32(this->values, other.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase(_mm256_sign_epi32(this->values, other.values));
            } else {
                return SIMDBase(_mm_sign_epi32(this->values, other.values));
            }
        } else if constexpr (isSame<T, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase(_mm512_sign_epi16(this->values, other.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase(_mm256_sign_epi16(this->values, other.values));
            } else {
                return SIMDBase(_mm_sign_epi16(this->values, other.values));
            }
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase(_mm512_sign_epi8(this->values, other.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase(_mm256_sign_epi8(this->values, other.values));
            } else {
                return SIMDBase(_mm_sign_epi8(this->values, other.values));
            }
        }
#endif
        else {
            return SIMDBase(Shift::sign<T>(this->value, other.value));
        }
    }

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_INLINE SIMDBase abs() const noexcept
    {
        if constexpr (isUnsigned<T>) {
            return *this;
        }
#if XS_ISA == XS_X86
        else if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase(_mm512_andnot_ps(_mm512_set1_ps(-0.0f), this->values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase(_mm256_andnot_ps(_mm256_set1_ps(-0.0f), this->values));
            } else {
                return SIMDBase(_mm_andnot_ps(_mm_set1_ps(-0.0f), this->values));
            }
        } else if constexpr (isSame<T, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_abs_epi32(this->values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_abs_epi32(this->values));
            } else {
                return SIMDInBase(_mm_abs_epi32(this->values));
            }
        } else if constexpr (isSame<T, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_abs_epi16(this->values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_abs_epi16(this->values));
            } else {
                return SIMDInBase(_mm_abs_epi16(this->values));
            }
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_abs_epi8(this->values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_abs_epi8(this->values));
            } else {
                return SIMDInBase(_mm_abs_epi8(this->values));
            }
        }
#endif
        else {
            return SIMDBase(Shift::abs<T>(this->value));
        }
    }

    /**
     * Maximum of two objects.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_INLINE SIMDBase max(const SIMDBase other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase(_mm512_max_ps(this->values, other.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase(_mm256_max_ps(this->values, other.values));
            } else {
                return SIMDBase(_mm_max_ps(this->values, other.values));
            }
        } else if constexpr (isSame<T, uint32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_max_epu32(this->values, other.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_max_epu32(this->values, other.values));
            } else {
                return SIMDInBase(_mm_max_epu32(this->values, other.values));
            }
        } else if constexpr (isSame<T, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_max_epi32(this->values, other.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_max_epi32(this->values, other.values));
            } else {
                return SIMDInBase(_mm_max_epi32(this->values, other.values));
            }
        } else if constexpr (isSame<T, uint16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_max_epu16(this->values, other.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_max_epu16(this->values, other.values));
            } else {
                return SIMDInBase(_mm_max_epu16(this->values, other.values));
            }
        } else if constexpr (isSame<T, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_max_epi16(this->values, other.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_max_epi16(this->values, other.values));
            } else {
                return SIMDInBase(_mm_max_epi16(this->values, other.values));
            }
        } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_max_epu8(this->values, other.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_max_epu8(this->values, other.values));
            } else {
                return SIMDInBase(_mm_max_epu8(this->values, other.values));
            }
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_max_epi8(this->values, other.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_max_epi8(this->values, other.values));
            } else {
                return SIMDInBase(_mm_max_epi8(this->values, other.values));
            }
        } else
#endif
        {
            return SIMDBase(Shift::max<T>(this->value, other.value));
        }
    }

    /**
     * Minimum of two objects.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_INLINE SIMDBase min(const SIMDBase other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase(_mm512_min_ps(this->values, other.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase(_mm256_min_ps(this->values, other.values));
            } else {
                return SIMDBase(_mm_min_ps(this->values, other.values));
            }
        } else if constexpr (isSame<T, uint32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_min_epu32(this->values, other.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_min_epu32(this->values, other.values));
            } else {
                return SIMDInBase(_mm_min_epu32(this->values, other.values));
            }
        } else if constexpr (isSame<T, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_min_epi32(this->values, other.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_min_epi32(this->values, other.values));
            } else {
                return SIMDInBase(_mm_min_epi32(this->values, other.values));
            }
        } else if constexpr (isSame<T, uint16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_min_epu16(this->values, other.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_min_epu16(this->values, other.values));
            } else {
                return SIMDInBase(_mm_min_epu16(this->values, other.values));
            }
        } else if constexpr (isSame<T, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_min_epi16(this->values, other.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_min_epi16(this->values, other.values));
            } else {
                return SIMDInBase(_mm_min_epi16(this->values, other.values));
            }
        } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_min_epu8(this->values, other.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_min_epu8(this->values, other.values));
            } else {
                return SIMDInBase(_mm_min_epu8(this->values, other.values));
            }
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_min_epi8(this->values, other.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_min_epi8(this->values, other.values));
            } else {
                return SIMDInBase(_mm_min_epi8(this->values, other.values));
            }
        } else
#endif
        {
            return SIMDBase(Shift::min<T>(this->value, other.value));
        }
    }

    /**
     * Clamp a value between 2 other values.
     * @param min The minimum allowed value to clamp to.
     * @param max The maximum allowed value to clamp to.
     * @returns The clamped value.
     */
    XS_INLINE SIMDBase clamp(const SIMDBase min, const SIMDBase max) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDBase(_mm512_max_ps(_mm512_min_ps(this->values, max.values), min.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDBase(_mm256_max_ps(_mm256_min_ps(this->values, max.values), min.values));
            } else {
                return SIMDBase(_mm_max_ps(_mm_min_ps(this->values, max.values), min.values));
            }
        } else if constexpr (isSame<T, uint32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_max_epu32(_mm512_min_epu32(this->values, max.values), min.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_max_epu32(_mm256_min_epu32(this->values, max.values), min.values));
            } else {
                return SIMDInBase(_mm_max_epu32(_mm_min_epu32(this->values, max.values), min.values));
            }
        } else if constexpr (isSame<T, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_max_epi32(_mm512_min_epi32(this->values, max.values), min.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_max_epi32(_mm256_min_epi32(this->values, max.values), min.values));
            } else {
                return SIMDInBase(_mm_max_epi32(_mm_min_epi32(this->values, max.values), min.values));
            }
        } else if constexpr (isSame<T, uint16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_max_epu16(_mm512_min_epu16(this->values, max.values), min.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_max_epu16(_mm256_min_epu16(this->values, max.values), min.values));
            } else {
                return SIMDInBase(_mm_max_epu16(_mm_min_epu16(this->values, max.values), min.values));
            }
        } else if constexpr (isSame<T, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_max_epi16(_mm512_min_epi16(this->values, max.values), min.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_max_epi16(_mm256_min_epi16(this->values, max.values), min.values));
            } else {
                return SIMDInBase(_mm_max_epi16(_mm_min_epi16(this->values, max.values), min.values));
            }
        } else if constexpr (isSame<T, uint8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_max_epu8(_mm512_min_epu8(this->values, max.values), min.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm256_max_epu8(_mm256_min_epu8(this->values, max.values), min.values));
            } else {
                return SIMDInBase(_mm_max_epu8(_mm_min_epu8(this->values, max.values), min.values));
            }
        } else if constexpr (isSame<T, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
                return SIMDInBase(_mm512_max_epi8(_mm512_min_epi8(this->values, max.values), min.values));
            } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
                return SIMDInBase(_mm512_max_epi8(_mm512_min_epi8(this->values, max.values), min.values));
            } else {
                return SIMDInBase(_mm_max_epi8(_mm_min_epi8(this->values, max.values), min.values));
            }
        } else
#endif
        {
            return SIMDBase(Shift::max<T>(Shift::min<T>(this->value, max.value)), min.value);
        }
    }
};

/**
 * Increment an object.
 * @param [in,out] other object to perform operation on.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width>& operator++(SIMDBase<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other.values = _mm512_add_ps(other.values, _mm512_set1_ps(1.0f));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other.values = _mm256_add_ps(other.values, _mm256_set1_ps(1.0f));
        } else {
            other.values = _mm_add_ps(other.values, _mm_set1_ps(1.0f));
        }
    } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other.values = _mm512_add_epi32(other.values, _mm512_set1_epi32(1));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other.values = _mm_add256_epi32(other.values, _mm256_set1_epi32(1));
        } else {
            other.values = _mm_add_epi32(other.values, _mm_set1_epi32(1));
        }
    } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other.values = _mm512_add_epi16(other.values, _mm512_set1_epi16(1));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other.values = _mm256_add_epi16(other.values, _mm256_set1_epi16(1));
        } else {
            other.values = _mm_add_epi16(other.values, _mm_set1_epi16(1));
        }
    } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other.values = _mm512_add_epi8(other.values, _mm512_set1_epi8(1));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other.values = _mm256_add_epi8(other.values, _mm256_set1_epi8(1));
        } else {
            other.values = _mm_add_epi8(other.values, _mm_set1_epi8(1));
        }
    } else
#endif
    {
        ++other.value;
    }
    return other;
}

/**
 * Post increment operator.
 * @param [in,out] other object to perform operation on.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator++(SIMDBase<T, Width>& other, int32) noexcept
{
    const auto ret = other;
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other.values = _mm512_add_ps(other.values, _mm512_set1_ps(1.0f));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other.values = _mm256_add_ps(other.values, _mm256_set1_ps(1.0f));
        } else {
            other.values = _mm_add_ps(other.values, _mm_set1_ps(1.0f));
        }
    } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other.values = _mm512_add_epi32(other.values, _mm512_set1_epi32(1));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other.values = _mm_add256_epi32(other.values, _mm256_set1_epi32(1));
        } else {
            other.values = _mm_add_epi32(other.values, _mm_set1_epi32(1));
        }
    } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other.values = _mm512_add_epi16(other.values, _mm512_set1_epi16(1));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other.values = _mm256_add_epi16(other.values, _mm256_set1_epi16(1));
        } else {
            other.values = _mm_add_epi16(other.values, _mm_set1_epi16(1));
        }
    } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other.values = _mm512_add_epi8(other.values, _mm512_set1_epi8(1));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other.values = _mm256_add_epi8(other.values, _mm256_set1_epi8(1));
        } else {
            other.values = _mm_add_epi8(other.values, _mm_set1_epi8(1));
        }
    } else
#endif
    {
        ++other.value;
    }
    return ret;
}

/**
 * Decrement an object.
 * @param [in,out] other object to perform operation on.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width>& operator--(SIMDBase<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other.values = _mm512_sub_ps(other.values, _mm512_set1_ps(1.0f));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other.values = _mm256_sub_ps(other.values, _mm256_set1_ps(1.0f));
        } else {
            other.values = _mm_sub_ps(other.values, _mm_set1_ps(1.0f));
        }
    } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other.values = _mm512_sub_epi32(other.values, _mm512_set1_epi32(1));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other.values = _mm_sub256_epi32(other.values, _mm256_set1_epi32(1));
        } else {
            other.values = _mm_sub_epi32(other.values, _mm_set1_epi32(1));
        }
    } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other.values = _mm512_sub_epi16(other.values, _mm512_set1_epi16(1));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other.values = _mm256_sub_epi16(other.values, _mm256_set1_epi16(1));
        } else {
            other.values = _mm_sub_epi16(other.values, _mm_set1_epi16(1));
        }
    } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other.values = _mm512_sub_epi8(other.values, _mm512_set1_epi8(1));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other.values = _mm256_sub_epi8(other.values, _mm256_set1_epi8(1));
        } else {
            other.values = _mm_sub_epi8(other.values, _mm_set1_epi8(1));
        }
    } else
#endif
    {
        --other.value;
    }
    return other;
}

/**
 * Post decrement operator.
 * @param [in,out] other object to perform operation on.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator--(SIMDBase<T, Width>& other, int32) noexcept
{
    const auto ret = other;
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other.values = _mm512_sub_ps(other.values, _mm512_set1_ps(1.0f));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other.values = _mm256_sub_ps(other.values, _mm256_set1_ps(1.0f));
        } else {
            other.values = _mm_sub_ps(other.values, _mm_set1_ps(1.0f));
        }
    } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other.values = _mm512_sub_epi32(other.values, _mm512_set1_epi32(1));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other.values = _mm_sub256_epi32(other.values, _mm256_set1_epi32(1));
        } else {
            other.values = _mm_sub_epi32(other.values, _mm_set1_epi32(1));
        }
    } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other.values = _mm512_sub_epi16(other.values, _mm512_set1_epi16(1));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other.values = _mm256_sub_epi16(other.values, _mm256_set1_epi16(1));
        } else {
            other.values = _mm_sub_epi16(other.values, _mm_set1_epi16(1));
        }
    } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other.values = _mm512_sub_epi8(other.values, _mm512_set1_epi8(1));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other.values = _mm256_sub_epi8(other.values, _mm256_set1_epi8(1));
        } else {
            other.values = _mm_sub_epi8(other.values, _mm_set1_epi8(1));
        }
    } else
#endif
    {
        --other.value;
    }
    return ret;
}

/**
 * Add two objects.
 * @param other1 The first object.
 * @param other2 objects to add to the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator+(const SIMDBase<T, Width> other1, const SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase<T, Width>(_mm512_add_ps(other1.values, other2.values));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase<T, Width>(_mm256_add_ps(other1.values, other2.values));
        } else {
            return SIMDBase<T, Width>(_mm_add_ps(other1.values, other2.values));
        }
    } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase<T, Width>(_mm512_add_epi32(other1.values, other2.values));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase<T, Width>(_mm256_add_epi32(other1.values, other2.values));
        } else {
            return SIMDBase<T, Width>(_mm_add_epi32(other1.values, other2.values));
        }
    } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase<T, Width>(_mm512_add_epi16(other1.values, other2.values));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase<T, Width>(_mm256_add_epi16(other1.values, other2.values));
        } else {
            return SIMDBase<T, Width>(_mm_add_epi16(other1.values, other2.values));
        }
    } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase<T, Width>(_mm512_add_epi8(other1.values, other2.values));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase<T, Width>(_mm256_add_epi8(other1.values, other2.values));
        } else {
            return SIMDBase<T, Width>(_mm_add_epi8(other1.values, other2.values));
        }
    } else
#endif
    {
        return SIMDBase<T, Width>(other1.value + other2.value);
    }
}

/**
 * Subtract a object from another object.
 * @param other1 The first object.
 * @param other2 object to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator-(const SIMDBase<T, Width> other1, const SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase<T, Width>(_mm512_sub_ps(other1.values, other2.values));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase<T, Width>(_mm256_sub_ps(other1.values, other2.values));
        } else {
            return SIMDBase<T, Width>(_mm_sub_ps(other1.values, other2.values));
        }
    } else if constexpr (isSameAny<T, uint32, int32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase<T, Width>(_mm512_sub_epi32(other1.values, other2.values));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase<T, Width>(_mm256_sub_epi32(other1.values, other2.values));
        } else {
            return SIMDBase<T, Width>(_mm_sub_epi32(other1.values, other2.values));
        }
    } else if constexpr (isSameAny<T, uint16, int16> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase<T, Width>(_mm512_sub_epi16(other1.values, other2.values));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase<T, Width>(_mm256_sub_epi16(other1.values, other2.values));
        } else {
            return SIMDBase<T, Width>(_mm_sub_epi16(other1.values, other2.values));
        }
    } else if constexpr (isSameAny<T, uint8, int8> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase<T, Width>(_mm512_sub_epi8(other1.values, other2.values));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase<T, Width>(_mm256_sub_epi8(other1.values, other2.values));
        } else {
            return SIMDBase<T, Width>(_mm_sub_epi8(other1.values, other2.values));
        }
    } else
#endif
    {
        return SIMDBase<T, Width>(other1.value - other2.value);
    }
}

/**
 * Multiply two objects.
 * @param other1 The first object.
 * @param other2 The second object to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator*(const SIMDBase<T, Width> other1, const SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase<T, Width>(_mm512_mul_ps(other1.values, other2.values));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase<T, Width>(_mm256_mul_ps(other1.values, other2.values));
        } else {
            return SIMDBase<T, Width>(_mm_mul_ps(other1.values, other2.values));
        }
    } else
#endif
    {
        return SIMDBase<T, Width>(other1.value * other2.value);
    }
}

/**
 * Divide two objects.
 * @param other1 The first object.
 * @param other2 The second object to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator/(const SIMDBase<T, Width> other1, const SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase<T, Width>(_mm512_div_ps(other1.values, other2.values));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase<T, Width>(_mm256_div_ps(other1.values, other2.values));
        } else {
            return SIMDBase<T, Width>(_mm_div_ps(other1.values, other2.values));
        }
    } else
#endif
    {
        return SIMDBase<T, Width>(other1.value / other2.value);
    }
}

/**
 * Negate a object.
 * @param other The object to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator-(const SIMDBase<T, Width> other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase<T, Width>(_mm512_sub_ps(_mm512_setzero_ps(), other.values));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase<T, Width>(_mm256_sub_ps(_mm256_setzero_ps(), other.values));
        } else {
            return SIMDBase<T, Width>(_mm_sub_ps(_mm_setzero_ps(), other.values));
        }
    } else
#endif
    {
        return SIMDBase<T, Width>(-other.value);
    }
}

/**
 * Perform compound assignment and addition with a object.
 * @param [in,out] other1 The first object.
 * @param          other2 object to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width>& operator+=(SIMDBase<T, Width>& other1, const SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other1.values = _mm512_add_ps(other1.values, other2.values);
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other1.values = _mm256_add_ps(other1.values, other2.values);
        } else {
            other1.values = _mm_add_ps(other1.values, other2.values);
        }
    } else
#endif
    {
        other1.value += other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and subtraction by a object.
 * @param [in,out] other1 The first object.
 * @param          other2 object to subtract from first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width>& operator-=(SIMDBase<T, Width>& other1, const SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other1.values = _mm512_sub_ps(other1.values, other2.values);
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other1.values = _mm256_sub_ps(other1.values, other2.values);
        } else {
            other1.values = _mm_sub_ps(other1.values, other2.values);
        }
    } else
#endif
    {
        other1.value -= other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and multiplication with a object.
 * @param [in,out] other1 The first object.
 * @param          other2 SIMDBase<T, Width> to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width>& operator*=(SIMDBase<T, Width>& other1, const SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other1.values = _mm512_mul_ps(other1.values, other2.values);
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other1.values = _mm256_mul_ps(other1.values, other2.values);
        } else {
            other1.values = _mm_mul_ps(other1.values, other2.values);
        }
    } else
#endif
    {
        other1.value *= other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and division with a object.
 * @param [in,out] other1 The object.
 * @param          other2 Value to divide the object by.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width>& operator/=(SIMDBase<T, Width>& other1, const SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            other1.values = _mm512_div_ps(other1.values, other2.values);
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            other1.values = _mm256_div_ps(other1.values, other2.values);
        } else {
            other1.values = _mm_div_ps(other1.values, other2.values);
        }
    } else
#endif
    {
        other1.value /= other2.value;
    }
    return other1;
}

/**
 * Compare two objects are equal.
 * @param other1 The first object.
 * @param other2 The second object to compare to the first.
 * @returns true if the parameters are considered equivalent.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator==(const SIMDBase<T, Width> other1, const SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return static_cast<bool>(
                _mm_ucomieq_ss(_mm512_castps512_ps128(other1.values), _mm512_castps512_ps128(other2.values)));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return static_cast<bool>(
                _mm_ucomieq_ss(_mm256_castps256_ps128(other1.values), _mm256_castps256_ps128(other2.values)));
        } else {
            return static_cast<bool>(_mm_ucomieq_ss(other1.values, other2.values));
        }
    } else
#endif
    {
        return other1.value == other2.value;
    }
}

/**
 * Compare two objects are less or equal.
 * @param other1 The first object.
 * @param other2 The second object to compare to the first.
 * @returns true if the first parameter is less than or equal to the second.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator<=(const SIMDBase<T, Width> other1, const SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return static_cast<bool>(
                _mm_ucomile_ss(_mm512_castps512_ps128(other1.values), _mm512_castps512_ps128(other2.values)));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return static_cast<bool>(
                _mm_ucomile_ss(_mm256_castps256_ps128(other1.values), _mm256_castps256_ps128(other2.values)));
        } else {
            return static_cast<bool>(_mm_ucomile_ss(other1.values, other2.values));
        }
    } else
#endif
    {
        return other1.value <= other2.value;
    }
}

/**
 * Compare two objects are less than.
 * @param other1 The first object.
 * @param other2 The second object to compare to the first.
 * @returns true if the first parameter is less than the second.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator<(const SIMDBase<T, Width> other1, const SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return static_cast<bool>(
                _mm_ucomilt_ss(_mm512_castps512_ps128(other1.values), _mm512_castps512_ps128(other2.values)));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return static_cast<bool>(
                _mm_ucomilt_ss(_mm256_castps256_ps128(other1.values), _mm256_castps256_ps128(other2.values)));
        } else {
            return static_cast<bool>(_mm_ucomilt_ss(other1.values, other2.values));
        }
    } else
#endif
    {
        return other1.value < other2.value;
    }
}

/**
 * Compare two objects are equal.
 * @param other1 The first object.
 * @param other2 The second object to compare to the first.
 * @returns true if the parameters are not considered equivalent.
 */
template<typename T, SIMDWidth Width>
XS_INLINE bool operator!=(const SIMDBase<T, Width> other1, const SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return static_cast<bool>(
                _mm_ucomineq_ss(_mm512_castps512_ps128(other1.values), _mm512_castps512_ps128(other2.values)));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return static_cast<bool>(
                _mm_ucomineq_ss(_mm256_castps256_ps128(other1.values), _mm256_castps256_ps128(other2.values)));
        } else {
            return static_cast<bool>(_mm_ucomineq_ss(other1.values, other2.values));
        }
    } else
#endif
    {
        return other1.value != other2.value;
    }
}

/**
 * And 2 objects.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first object.
 * @param other2 object to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator&(const SIMDBase<T, Width> other1, const SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase<T, Width>(_mm512_and_ps(other1.values, other2.values));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase<T, Width>(_mm256_and_ps(other1.values, other2.values));
        } else {
            return SIMDBase<T, Width>(_mm_and_ps(other1.values, other2.values));
        }
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMDBase<T, Width>(other1.value & other2.value);
        } else {
            return SIMDBase<T, Width>(Shift::bitAnd(other1.value, other2.value));
        }
    }
}

/**
 * Or 2 objects.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first object.
 * @param other2 object to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator|(const SIMDBase<T, Width> other1, const SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase<T, Width>(_mm512_or_ps(other1.values, other2.values));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase<T, Width>(_mm256_or_ps(other1.values, other2.values));
        } else {
            return SIMDBase<T, Width>(_mm_or_ps(other1.values, other2.values));
        }
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMDBase<T, Width>(other1.value | other2.value);
        } else {
            return SIMDBase<T, Width>(Shift::bitOr(other1.value, other2.value));
        }
    }
}

/**
 * Xor 2 objects.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other1 The first object.
 * @param other2 object to and with the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator^(const SIMDBase<T, Width> other1, const SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase<T, Width>(_mm512_xor_ps(other1.values, other2.values));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase<T, Width>(_mm256_xor_ps(other1.values, other2.values));
        } else {
            return SIMDBase<T, Width>(_mm_xor_ps(other1.values, other2.values));
        }
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMDBase<T, Width>(other1.value ^ other2.value);
        } else {
            return SIMDBase<T, Width>(Shift::bitXor(other1.value, other2.value));
        }
    }
}

/**
 * Not a object.
 * @note This function is provided only for ease of optimisation. It should be used with care
 * as its operation is not defined on certain architectures.
 * @param other The object.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDBase<T, Width> operator~(const SIMDBase<T, Width> other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase<T, Width>(_mm512_xor_ps(other.values, _mm512_castsi512_ps(_mm512_set1_epi32(-1))));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 val = _mm256_undefined_ps();
            return SIMDBase<T, Width>(_mm256_xor_ps(other.values, _mm256_cmp_ps(val, val, _CMP_EQ_UQ)));
        } else {
            const __m128 val = _mm_undefined_ps();
            return SIMDBase<T, Width>(_mm_xor_ps(other.values, _mm_cmpeq_ps(val, val)));
        }
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMDBase<T, Width>(~other.value);
        } else {
            return SIMDBase<T, Width>(Shift::bitNot(other.value));
        }
    }
}
} // namespace Shift
