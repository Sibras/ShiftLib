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

#include "SIMD/XSSIMDData.hpp"

#if XS_ISA == XS_X86
#    include "SIMD/XSSIMDx86.hpp"
#    include "SIMD/XSSIMDx86Functions.hpp"
#endif
#include "SIMD/XSSIMDInBase.hpp"
#include "XSBit.hpp"
#include "XSMath.hpp"

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
    XS_FUNCTION SIMDBaseData() noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_FUNCTION explicit SIMDBaseData(SIMDBase<T, Width> other) noexcept
    {
        store(other);
    }

    /**
     * Directly set the contents of this object.
     * @note This function is used to set data using a pre-calculated value.
     * This removes the necessity to load and then store.
     * @param other The pre-calculated value.
     */
    XS_FUNCTION void setData(T other) noexcept
    {
        value = other;
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_FUNCTION void store(SIMDBase<T, Width> other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            _mm_store_ss(&value, _mm512_castps512_ps128(other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            _mm_store_ss(&value, _mm256_castps256_ps128(other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            _mm_store_ss(&value, other.values);
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
    template<SIMDWidth Width>
    XS_FUNCTION SIMDBase<T, Width> load() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase<T, Width>(_mm512_set1_ps(value));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase<T, Width>(_mm256_broadcast_ss(&value));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasISAFeature<ISAFeature::AVX>) {
                return SIMDBase<T, Width>(_mm_broadcast_ss(&value));
            } else {
                const __m128 val = _mm_load_ss(&value);
                return SIMDBase<T, Width>(_mm_shuffle1_ps(val, _MM_SHUFFLE(0, 0, 0, 0)));
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

template<typename T, SIMDWidth Width = widthSIMD<T>>
class SIMDBase : public NoExport::SIMDData<T, 1, numValues<T, Width> - 1, Width>
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    using Type = T;
    using Data = NoExport::SIMDData<T, 1, numValues<T, Width> - 1, Width>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = Data::width;
    using InBaseDef = SIMDInBase<T, SIMDInBase<T, widthImpl>::widthImpl>;
    using Data::SIMDData;

    /** Default constructor. */
    XS_FUNCTION SIMDBase() noexcept = default;

    /**
     * Constructor
     * @param other The other.
     */
    XS_FUNCTION SIMDBase(const SIMDBase& other) = default;

    /**
     * Constructor
     * @param [in,out] other The other.
     */
    XS_FUNCTION SIMDBase(SIMDBase&& other) noexcept = default;

    /**
     * Assignment operator
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMDBase& operator=(const SIMDBase& other) = default;

    /**
     * Move assignment operator
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_FUNCTION SIMDBase& operator=(SIMDBase&& other) noexcept = default;

    /**
     * Construct from a value.
     * @param val The value.
     */
    XS_FUNCTION explicit SIMDBase(T val) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            this->values = _mm512_set1_ps(val);
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            this->values = _mm256_set1_ps(val);
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            this->values = _mm_set1_ps(val);
        } else
#endif
        {
            this->value = val;
        }
    }

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_FUNCTION explicit SIMDBase(SIMDBase<T, Width2> other) noexcept
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
            } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
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
     * Constructor.
     * @param other The other.
     */
    XS_FUNCTION explicit SIMDBase(InBaseDef other) noexcept
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
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
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
    XS_FUNCTION explicit operator InBaseDef() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return InBaseDef(_mm512_castps512_ps128(this->values));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return InBaseDef(_mm256_castps256_ps128(this->values));
        } else if constexpr (hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
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
    XS_FUNCTION static SIMDBase Zero() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_setzero_ps());
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_setzero_ps());
        } else if constexpr (hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(_mm_setzero_ps());
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
    XS_FUNCTION static SIMDBase One() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_set1_ps(1.0f));
        } else if constexpr (hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_set1_ps(1.0f));
        } else if constexpr (hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(_mm_set1_ps(1.0f));
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
    XS_FUNCTION T getValue() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return _mm_cvtss_f32(_mm512_castps512_ps128(this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return _mm_cvtss_f32(_mm256_castps256_ps128(this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
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
    XS_FUNCTION SIMDBase mad(SIMDBase other1, SIMDBase other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMDBase(_mm512_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMDBase(_mm512_add_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMDBase(_mm256_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMDBase(_mm256_add_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMDBase(_mm_fmadd_ps(this->values, other1.values, other2.values));
            } else {
                return SIMDBase(_mm_add_ps(_mm_mul_ps(this->values, other1.values), other2.values));
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
    XS_FUNCTION SIMDBase msub(SIMDBase other1, SIMDBase other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMDBase(_mm512_fmsub_ps(this->values, other1.values, other2.values));
            } else {
                return SIMDBase(_mm512_sub_ps(_mm512_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMDBase(_mm256_fmsub_ps(this->values, other1.values, other2.values));
            } else {
                return SIMDBase(_mm256_sub_ps(_mm256_mul_ps(this->values, other1.values), other2.values));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMDBase(_mm_fmsub_ps(this->values, other1.values, other2.values));
            } else {
                return SIMDBase(_mm_sub_ps(_mm_mul_ps(this->values, other1.values), other2.values));
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
    XS_FUNCTION SIMDBase sign(SIMDBase other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_xor_ps(this->values, _mm512_and_ps(other.values, _mm512_set1_ps(-0.0f))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_xor_ps(this->values, _mm256_and_ps(other.values, _mm256_set1_ps(-0.0f))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(_mm_xor_ps(this->values, _mm_and_ps(_mm_set1_ps(-0.0f), other.values)));
        } else
#endif
        {
            return SIMDBase(Shift::sign<T>(this->value, other.value));
        }
    }

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDBase abs() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_andnot_ps(_mm512_set1_ps(-0.0f), this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_andnot_ps(_mm256_set1_ps(-0.0f), this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(_mm_andnot_ps(_mm_set1_ps(-0.0f), this->values));
        } else
#endif
        {
            return SIMDBase(Shift::abs<T>(this->value));
        }
    }

    /**
     * Maximum of two objects.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_FUNCTION SIMDBase max(SIMDBase other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_max_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_max_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(_mm_max_ps(this->values, other.values));
        } else
#endif
        {
            return SIMDBase(Shift::max<T>(this->value, other.value));
        }
    }

    /**
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_FUNCTION SIMDBase min(SIMDBase other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_min_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_min_ps(this->values, other.values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(_mm_min_ps(this->values, other.values));
        } else
#endif
        {
            return SIMDBase(Shift::min<T>(this->value, other.value));
        }
    }

    /**
     * Approximate reciprocal (1/this) of object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDBase reciprocal() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 recip = _mm512_rcp14_ps(this->values);
            return SIMDBase(_mm512_fnmadd_ps(_mm512_mul_ps(recip, recip), this->values, _mm512_add_ps(recip, recip)));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 recip = _mm256_recip_ps(this->values);
            return SIMDBase(_mm256_fnmadd_ps(_mm256_mul_ps(recip, recip), this->values, _mm256_add_ps(recip, recip)));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 recip = _mm_recip_ps(this->values);
            return SIMDBase(_mm_fnmadd_ps(_mm_mul_ps(recip, recip), this->values, _mm_add_ps(recip, recip)));
        } else
#endif
        {
            return SIMDBase(Shift::recip<T>(this->value));
        }
    }

    /**
     * Round up a object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDBase ceil() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_roundscale_ps(this->values, FROUND_CEIL));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_round_ps(this->values, FROUND_CEIL));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(_mm_ceil_ps(this->values));
        } else
#endif
        {
            return SIMDBase(Shift::ceil<T>(this->value));
        }
    }

    /**
     * Round down a object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDBase floor() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_roundscale_ps(this->values, FROUND_FLOOR));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_round_ps(this->values, FROUND_FLOOR));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(_mm_floor_ps(this->values));
        } else
#endif
        {
            return SIMDBase(Shift::floor<T>(this->value));
        }
    }

    /**
     * Round toward zero a object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDBase trunc() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_roundscale_ps(this->values, FROUND_TRUNC));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_round_ps(this->values, FROUND_TRUNC));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(_mm_trunc_ps(this->values));
        } else
#endif
        {
            return SIMDBase(Shift::trunc<T>(this->value));
        }
    }

    /**
     * Returns the sqrt of a object.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDBase sqrt() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_sqrt_ps(this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_sqrt_ps(this->values));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(_mm_sqrt_ps(this->values));
        } else
#endif
        {
            return SIMDBase(Shift::sqrt<T>(this->value));
        }
    }

    /**
     * Approximate reciprocal square root of a object.
     * @note Useful as the reciprocal square root is faster to determine.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDBase rsqrt() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m512 recip = _mm512_rsqrt14_ps(this->values);
            const __m512 val1 = _mm512_mul_ps(_mm512_mul_ps(_mm512_set1_ps(0.5f), this->values), recip);
            const __m512 val2 = _mm512_fnmadd_ps(recip, val1, _mm512_set1_ps(1.5f));
            return SIMDBase(_mm512_mul_ps(recip, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m256 recip = _mm256_recipsqrt_ps(this->values);
            const __m256 val1 = _mm256_mul_ps(_mm256_mul_ps(_mm256_set1_ps(0.5f), this->values), recip);
            const __m256 val2 = _mm256_fnmadd_ps(recip, val1, _mm256_set1_ps(1.5f));
            return SIMDBase(_mm256_mul_ps(recip, val2));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 recip = _mm_recipsqrt_ps(this->values);
            const __m128 val1 = _mm_mul_ps(_mm_mul_ps(_mm_set1_ps(0.5f), this->values), recip);
            const __m128 val2 = _mm_fnmadd_ps(recip, val1, _mm_set1_ps(1.5f));
            return SIMDBase(_mm_mul_ps(recip, val2));
        } else
#endif
        {
            return SIMDBase(Shift::rsqrt<T>(this->value));
        }
    }

    /**
     * Returns binary exponential of a object.
     * @returns object containing the binary exponential of the input.
     */
    XS_FUNCTION SIMDBase exp2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_broadcastf128_ps(NoExport::exp2f4(_mm512_castps512_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_broadcastf128_ps(NoExport::exp2f4(_mm256_castps256_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(NoExport::exp2f4(this->values));
        } else
#endif
        {
            return SIMDBase(Shift::exp2<T>(this->value));
        }
    }

    /**
     * Returns exponential of a object.
     * @returns object containing the exponential of the input.
     */
    XS_FUNCTION SIMDBase exp() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_broadcastf128_ps(NoExport::expf4(_mm512_castps512_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_broadcastf128_ps(NoExport::expf4(_mm256_castps256_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(NoExport::expf4(this->values));
        } else
#endif
        {
            return SIMDBase(Shift::exp<T>(this->value));
        }
    }

    /**
     * Returns binary logarithm of a object.
     * @returns object containing the binary logarithm of the input.
     */
    XS_FUNCTION SIMDBase log2() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_broadcastf128_ps(NoExport::log2f4(_mm512_castps512_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_broadcastf128_ps(NoExport::log2f4(_mm256_castps256_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(NoExport::log2f4(this->values));
        } else
#endif
        {
            return SIMDBase(Shift::log2<T>(this->value));
        }
    }

    /**
     * Returns natural logarithm of a object.
     * @returns object containing the logarithm of the input.
     */
    XS_FUNCTION SIMDBase log() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_broadcastf128_ps(NoExport::logf4(_mm512_castps512_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_broadcastf128_ps(NoExport::logf4(_mm256_castps256_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(NoExport::logf4(this->values));
        } else
#endif
        {
            return SIMDBase(Shift::log<T>(this->value));
        }
    }

    /**
     * Returns object to the power of another object.
     * @param other The object containing the exponent.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDBase pow(SIMDBase other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_broadcastf128_ps(
                NoExport::powf4(_mm512_castps512_ps128(this->values), _mm512_castps512_ps128(other.values))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_broadcastf128_ps(
                NoExport::powf4(_mm256_castps256_ps128(this->values), _mm256_castps256_ps128(other.values))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(NoExport::powf4(this->values, other.values));
        } else
#endif
        {
            return SIMDBase(Shift::pow<T>(this->value, other.value));
        }
    }

    /**
     * Returns object to the power of another object.
     * @note The current object must be > 0.
     * @param other The object containing the exponent.
     * @returns Result of operation.
     */
    XS_FUNCTION SIMDBase powr(SIMDBase other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_broadcastf128_ps(
                NoExport::powrf4(_mm512_castps512_ps128(this->values), _mm512_castps512_ps128(other.values))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_broadcastf128_ps(
                NoExport::powrf4(_mm256_castps256_ps128(this->values), _mm256_castps256_ps128(other.values))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(NoExport::powrf4(this->values, other.values));
        } else
#endif
        {
            return SIMDBase(Shift::powr<T>(this->value, other.value));
        }
    }

    /**
     * Returns sine of a object.
     * @note Current object must have values in radians.
     * @returns Object containing the sined value.
     */
    XS_FUNCTION SIMDBase sin() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_broadcastf128_ps(NoExport::sinf4(_mm512_castps512_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_broadcastf128_ps(NoExport::sinf4(_mm256_castps256_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(NoExport::sinf4(this->values));
        } else
#endif
        {
            return SIMDBase(Shift::sin<T>(this->value));
        }
    }

    /**
     * Returns cosine of a object.
     * @note Current object must have values in radians.
     * @returns Object containing the cosined value.
     */
    XS_FUNCTION SIMDBase cos() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_broadcastf128_ps(NoExport::cosf4(_mm512_castps512_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_broadcastf128_ps(NoExport::cosf4(_mm256_castps256_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(NoExport::cosf4(this->values));
        } else
#endif
        {
            return SIMDBase(Shift::cos<T>(this->value));
        }
    }

    /**
     * Returns tangent of a object.
     * @note Current object must have values in radians.
     * @returns Object containing the tangented value.
     */
    XS_FUNCTION SIMDBase tan() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_broadcastf128_ps(NoExport::tanf4(_mm512_castps512_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_broadcastf128_ps(NoExport::tanf4(_mm256_castps256_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(NoExport::tanf4(this->values));
        } else
#endif
        {
            return SIMDBase(Shift::tan<T>(this->value));
        }
    }

    /**
     * Returns sine and cosine of a object.
     * @note This is provided because it is much quicker to determine the cosine of value if the sine is already
     * known. Current object must have values in radians.
     * @param [out] cosReturn The cosine return values.
     * @returns Object containing the sine value.
     */
    XS_FUNCTION SIMDBase sincos(SIMDBase& cosReturn) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            const __m128 sinCos = NoExport::sinf4(
                _mm_add_ps(_mm512_castps512_ps128(this->values), _mm_set_ps(0.0f, 0.0f, valPi2<float32>, 0.0f)));
            cosReturn.values = _mm512_broadcastf128_ps(_mm_shuffle1111_ps(sinCos));
            return SIMDBase(_mm512_broadcastss_ps(sinCos));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            const __m128 sinCos = NoExport::sinf4(
                _mm_add_ps(_mm256_castps256_ps128(this->values), _mm_set_ps(0.0f, 0.0f, valPi2<float32>, 0.0f)));
            cosReturn.values = _mm256_broadcastf128_ps(_mm_shuffle1111_ps(sinCos));
            if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                return SIMDBase(_mm256_broadcastss_ps(sinCos));
            } else {
                const __m128 val = _mm_shuffle0000_ps(sinCos);
                return SIMDBase(_mm256_broadcastf128_ps(val));
            }
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            const __m128 sinCos =
                NoExport::sinf4(_mm_add_ps(this->values, _mm_set_ps(0.0f, 0.0f, valPi2<float32>, 0.0f)));
            cosReturn.values = _mm_shuffle1111_ps(sinCos);
            return SIMDBase(_mm_shuffle0000_ps(sinCos));
        } else
#endif
        {
            return SIMDBase(Shift::sincos<T>(this->value, cosReturn.value));
        }
    }

    /**
     * Returns arcsine of a object.
     * @returns Object containing the arcsined value (result in radians).
     */
    XS_FUNCTION SIMDBase asin() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_broadcastf128_ps(NoExport::asinf4(_mm512_castps512_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_broadcastf128_ps(NoExport::asinf4(_mm256_castps256_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(NoExport::asinf4(this->values));
        } else
#endif
        {
            return SIMDBase(Shift::asin<T>(this->value));
        }
    }

    /**
     * Returns arccosine of a object.
     * @returns Object containing the arccosined value (result in radians).
     */
    XS_FUNCTION SIMDBase acos() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_broadcastf128_ps(NoExport::acosf4(_mm512_castps512_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_broadcastf128_ps(NoExport::acosf4(_mm256_castps256_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(NoExport::acosf4(this->values));
        } else
#endif
        {
            return SIMDBase(Shift::acos<T>(this->value));
        }
    }

    /**
     * Returns arctangent of a object.
     * @returns Object containing the arctangented value (result in radians).
     */
    XS_FUNCTION SIMDBase atan() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_broadcastf128_ps(NoExport::atanf4(_mm512_castps512_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_broadcastf128_ps(NoExport::atanf4(_mm256_castps256_ps128(this->values))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(NoExport::atanf4(this->values));
        } else
#endif
        {
            return SIMDBase(Shift::atan<T>(this->value));
        }
    }

    /**
     * Returns the distance between a planes positive x-axis and the points given by 2 input object.
     * @param other The object containing the second param of the input points.
     * @returns Object containing the angle (result in radians).
     */
    XS_FUNCTION SIMDBase atan2(SIMDBase other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
            return SIMDBase(_mm512_broadcastf128_ps(
                NoExport::atan2f4(_mm512_castps512_ps128(this->values), _mm512_castps512_ps128(other.values))));
        } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
            return SIMDBase(_mm256_broadcastf128_ps(
                NoExport::atan2f4(_mm256_castps256_ps128(this->values), _mm256_castps256_ps128(other.values))));
        } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
            return SIMDBase(NoExport::atan2f4(this->values, other.values));
        } else
#endif
        {
            return SIMDBase(Shift::atan2<T>(this->value, other.value));
        }
    }
};

/**
 * Increment an object.
 * @param [in,out] other object to perform operation on.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_FUNCTION SIMDBase<T, Width>& operator++(SIMDBase<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other.values = _mm512_add_ps(other.values, _mm512_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other.values = _mm256_add_ps(other.values, _mm256_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other.values = _mm_add_ps(other.values, _mm_set1_ps(1.0f));
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
XS_FUNCTION SIMDBase<T, Width> operator++(SIMDBase<T, Width>& other, int32) noexcept
{
    const auto ret = other;
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other.values = _mm512_add_ps(other.values, _mm512_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other.values = _mm256_add_ps(other.values, _mm256_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other.values = _mm_add_ps(other.values, _mm_set1_ps(1.0f));
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
XS_FUNCTION SIMDBase<T, Width>& operator--(SIMDBase<T, Width>& other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other.values = _mm512_sub_ps(other.values, _mm512_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other.values = _mm256_sub_ps(other.values, _mm256_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other.values = _mm_sub_ps(other.values, _mm_set1_ps(1.0f));
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
XS_FUNCTION SIMDBase<T, Width> operator--(SIMDBase<T, Width>& other, int32) noexcept
{
    const auto ret = other;
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other.values = _mm512_sub_ps(other.values, _mm512_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other.values = _mm256_sub_ps(other.values, _mm256_set1_ps(1.0f));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other.values = _mm_sub_ps(other.values, _mm_set1_ps(1.0f));
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
XS_FUNCTION SIMDBase<T, Width> operator+(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase<T, Width>(_mm256_add_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase<T, Width>(_mm_add_ps(other1.values, other2.values));
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
XS_FUNCTION SIMDBase<T, Width> operator-(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase<T, Width>(_mm256_sub_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase<T, Width>(_mm_sub_ps(other1.values, other2.values));
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
XS_FUNCTION SIMDBase<T, Width> operator*(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase<T, Width>(_mm256_mul_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase<T, Width>(_mm_mul_ps(other1.values, other2.values));
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
XS_FUNCTION SIMDBase<T, Width> operator/(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase<T, Width>(_mm256_div_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase<T, Width>(_mm_div_ps(other1.values, other2.values));
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
XS_FUNCTION SIMDBase<T, Width> operator-(SIMDBase<T, Width> other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_sub_ps(_mm512_setzero_ps(), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase<T, Width>(_mm256_sub_ps(_mm256_setzero_ps(), other.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase<T, Width>(_mm_sub_ps(_mm_setzero_ps(), other.values));
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
XS_FUNCTION SIMDBase<T, Width>& operator+=(SIMDBase<T, Width>& other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_add_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_add_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_add_ps(other1.values, other2.values);
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
XS_FUNCTION SIMDBase<T, Width>& operator-=(SIMDBase<T, Width>& other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_sub_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_sub_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_sub_ps(other1.values, other2.values);
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
XS_FUNCTION SIMDBase<T, Width>& operator*=(SIMDBase<T, Width>& other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_mul_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_mul_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_mul_ps(other1.values, other2.values);
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
XS_FUNCTION SIMDBase<T, Width>& operator/=(SIMDBase<T, Width>& other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        other1.values = _mm512_div_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        other1.values = _mm256_div_ps(other1.values, other2.values);
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        other1.values = _mm_div_ps(other1.values, other2.values);
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
XS_FUNCTION bool operator==(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return static_cast<bool>(
            _mm_ucomieq_ss(_mm512_castps512_ps128(other1.values), _mm512_castps512_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return static_cast<bool>(
            _mm_ucomieq_ss(_mm256_castps256_ps128(other1.values), _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return static_cast<bool>(_mm_ucomieq_ss(other1.values, other2.values));
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
XS_FUNCTION bool operator<=(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return static_cast<bool>(
            _mm_ucomile_ss(_mm512_castps512_ps128(other1.values), _mm512_castps512_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return static_cast<bool>(
            _mm_ucomile_ss(_mm256_castps256_ps128(other1.values), _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return static_cast<bool>(_mm_ucomile_ss(other1.values, other2.values));
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
XS_FUNCTION bool operator<(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return static_cast<bool>(
            _mm_ucomilt_ss(_mm512_castps512_ps128(other1.values), _mm512_castps512_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return static_cast<bool>(
            _mm_ucomilt_ss(_mm256_castps256_ps128(other1.values), _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return static_cast<bool>(_mm_ucomilt_ss(other1.values, other2.values));
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
XS_FUNCTION bool operator!=(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return static_cast<bool>(
            _mm_ucomineq_ss(_mm512_castps512_ps128(other1.values), _mm512_castps512_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return static_cast<bool>(
            _mm_ucomineq_ss(_mm256_castps256_ps128(other1.values), _mm256_castps256_ps128(other2.values)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return static_cast<bool>(_mm_ucomineq_ss(other1.values, other2.values));
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
XS_FUNCTION SIMDBase<T, Width> operator&(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase<T, Width>(_mm256_and_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase<T, Width>(_mm_and_ps(other1.values, other2.values));
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
XS_FUNCTION SIMDBase<T, Width> operator|(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase<T, Width>(_mm256_or_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase<T, Width>(_mm_or_ps(other1.values, other2.values));
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
XS_FUNCTION SIMDBase<T, Width> operator^(SIMDBase<T, Width> other1, SIMDBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        return SIMDBase<T, Width>(_mm256_xor_ps(other1.values, other2.values));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        return SIMDBase<T, Width>(_mm_xor_ps(other1.values, other2.values));
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
XS_FUNCTION SIMDBase<T, Width> operator~(SIMDBase<T, Width> other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD512<T> && (Width >= SIMDWidth::B64)) {
        return SIMDBase<T, Width>(_mm512_xor_ps(other.values, _mm512_castsi512_ps(_mm512_set1_epi32(-1))));
    } else if constexpr (isSame<T, float32> && hasSIMD256<T> && (Width >= SIMDWidth::B32)) {
        const __m256 val = _mm256_undefined_ps();
        return SIMDBase<T, Width>(_mm256_xor_ps(other.values, _mm256_cmp_ps(val, val, _CMP_EQ_UQ)));
    } else if constexpr (isSame<T, float32> && hasSIMD128<T> && (Width >= SIMDWidth::B16)) {
        const __m128 val = _mm_undefined_ps();
        return SIMDBase<T, Width>(_mm_xor_ps(other.values, _mm_cmpeq_ps(val, val)));
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
