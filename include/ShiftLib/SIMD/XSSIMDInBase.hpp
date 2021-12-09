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
#include "XSBit.hpp"
#include "XSMath.hpp"

namespace Shift {
template<typename T, SIMDWidth Width>
class SIMDInBase;
template<typename T, SIMDWidth Width>
class SIMDBase;

template<typename T>
class SIMDInBaseData
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    T value;

    /** Default constructor. */
    XS_INLINE SIMDInBaseData() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE SIMDInBaseData(const SIMDInBaseData& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    XS_INLINE SIMDInBaseData(SIMDInBaseData&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMDInBaseData& operator=(const SIMDInBaseData& other) noexcept = default;

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMDInBaseData& operator=(SIMDInBaseData&& other) noexcept = default;

    /**
     * Construct from non-data type.
     * @tparam Width Type of SIMD being used.
     * @param other The non-data type to construct from.
     */
    template<SIMDWidth Width>
    XS_INLINE explicit SIMDInBaseData(SIMDInBase<T, Width> other) noexcept
    {
        store(other);
    }

    /**
     * Directly set the contents of this object.
     * @note This function is used to set data using a pre-calculated value.
     * This removes the necessity to load and then store.
     * @param other The pre-calculated value.
     */
    XS_INLINE void setData(T other) noexcept
    {
        value = other;
    }

    /**
     * Save to memory.
     * @tparam Width Type of SIMD being used.
     * @param other The object to store.
     */
    template<SIMDWidth Width>
    XS_INLINE void store(SIMDInBase<T, Width> other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            _mm_store_ss(reinterpret_cast<float32*>(this), other.values);
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
    XS_INLINE SIMDInBase<T, Width> load() const noexcept
    {
        return SIMDInBase<T, Width>(value);
    }
};

template<typename T>
using SIMDInBaseDataPad = SIMDInBaseData<T>;

template<typename T, SIMDWidth Width = defaultWidthSIMD<T>>
class SIMDInBase : public NoExport::SIMDData<T, 1, 0, Width>
{
    static_assert(
        isArithmetic<T> && !isCOrV<T>, "Invalid Type: Only arithmetic types without any qualifiers can be used");

public:
    using Type = T;
    using InternalData = NoExport::SIMDData<T, 1, 0, Width>;
    using Data = SIMDInBaseData<T>;
    using DataPad = SIMDInBaseDataPad<T>;
    static constexpr SIMDWidth width = Width;
    static constexpr SIMDWidth widthImpl = InternalData::width;
    static constexpr uint32 numValues = 1;
    static constexpr uint32 size = 1;
    using InternalData::SIMDData;

    /** Default constructor. */
    XS_INLINE SIMDInBase() noexcept = default;

    /**
     * Constructor.
     * @param other The other.
     */
    XS_INLINE SIMDInBase(const SIMDInBase& other) = default;

    /**
     * Constructor.
     * @param [in,out] other The other.
     */
    XS_INLINE SIMDInBase(SIMDInBase&& other) noexcept = default;

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMDInBase& operator=(const SIMDInBase& other) = default;

    /**
     * Move assignment operator.
     * @param [in,out] other The other.
     * @returns A shallow copy of this object.
     */
    XS_INLINE SIMDInBase& operator=(SIMDInBase&& other) noexcept = default;

    /**
     * Constructor.
     * @tparam Width2 Type of SIMD being used.
     * @param other The other.
     */
    template<SIMDWidth Width2>
    XS_INLINE explicit SIMDInBase(SIMDInBase<T, Width2> other) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = other.values;
        } else
#endif
        {
            this->value = other.value;
        }
    }

    /**
     * Construct from a value.
     * @param val The value.
     */
    XS_INLINE explicit SIMDInBase(T val) noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            this->values = _mm_set_ss(val);
        } else
#endif
        {
            this->value = val;
        }
    }

    /**
     * Constructor to build set to 0.
     * @returns Newly constructed object with required attributes.
     */
    XS_INLINE static SIMDInBase Zero() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMDInBase(_mm_setzero_ps());
        } else
#endif
        {
            return SIMDInBase(T{0});
        }
    }

    /**
     * Constructor to build set to 1.
     * @returns Newly constructed object with required attributes.
     */
    XS_INLINE static SIMDInBase One() noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMDInBase(_mm_set_ss(1.0_f32));
        } else
#endif
        {
            return SIMDInBase(T{1});
        }
    }

    /**
     * Access the object as a normal value.
     * @returns The value.
     */
    XS_INLINE T getValue() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
    XS_INLINE SIMDInBase mad(SIMDInBase other1, SIMDInBase other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMDInBase(_mm_fmadd_ss(this->values, other1.values, other2.values));
            } else {
                return SIMDInBase(_mm_add_ss(_mm_mul_ss(this->values, other1.values), other2.values));
            }
        } else
#endif
        {
            return SIMDInBase(Shift::fma<T>(this->value, other1.value, other2.value));
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
    XS_INLINE SIMDInBase msub(SIMDInBase other1, SIMDInBase other2) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            if constexpr (hasFMA<T> && (EvenIfNotFree || hasFMAFree<T>)) {
                return SIMDInBase(_mm_fmsub_ss(this->values, other1.values, other2.values));
            } else {
                return SIMDInBase(_mm_sub_ss(_mm_mul_ss(this->values, other1.values), other2.values));
            }
        } else
#endif
        {
            return SIMDInBase(Shift::fma<T>(this->value, other1.value, -other2.value));
        }
    }

    /**
     * Negate this object based on the sign of another.
     * @note If the sign of the second input is negative the first input will be negated.
     * If the sign of the second input is positive then the first input will be returned unchanged.
     * @param other The second object whose sign to check.
     * @returns Result of operation.
     */
    XS_INLINE SIMDInBase sign(SIMDInBase other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            const __m128 mask = _mm_and_ps(_mm_set_ss(-0.0f), other.values);
            return SIMDInBase(_mm_xor_ps(this->values, mask));
        } else
#endif
        {
            return SIMDInBase(Shift::sign<T>(this->value, other.value));
        }
    }

    /**
     * Compute the absolute value of this object.
     * @returns Result of operation.
     */
    XS_INLINE SIMDInBase abs() const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMDInBase(_mm_andnot_ps(_mm_set_ss(-0.0f), this->values));
        } else
#endif
        {
            return SIMDInBase(Shift::abs<T>(this->value));
        }
    }

    /**
     * Maximum of two objects.
     * @param other The second object.
     * @returns The maximum value.
     */
    XS_INLINE SIMDInBase max(SIMDInBase other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMDInBase(_mm_max_ss(this->values, other.values));
        } else
#endif
        {
            return SIMDInBase(Shift::max<T>(this->value, other.value));
        }
    }

    /**
     * Minimum of two objects.
     * @param other The second object.
     * @returns The minimum value.
     */
    XS_INLINE SIMDInBase min(SIMDInBase other) const noexcept
    {
#if XS_ISA == XS_X86
        if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
            return SIMDInBase(_mm_min_ss(this->values, other.values));
        } else
#endif
        {
            return SIMDInBase(Shift::min<T>(this->value, other.value));
        }
    }
};

/**
 * Increment an object.
 * @param [in,out] other object to perform operation on.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width>& operator++(SIMDInBase<T, Width>& other) noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other += SIMDInBase<T, Width>::One();
    } else {
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
XS_INLINE SIMDInBase<T, Width> operator++(SIMDInBase<T, Width>& other, int32) noexcept
{
    const auto ret = other;
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other += SIMDInBase<T, Width>::One();
    } else {
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
XS_INLINE SIMDInBase<T, Width>& operator--(SIMDInBase<T, Width>& other) noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other -= SIMDInBase<T, Width>::One();
    } else {
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
XS_INLINE SIMDInBase<T, Width> operator--(SIMDInBase<T, Width>& other, int32) noexcept
{
    const auto ret = other;
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other -= SIMDInBase<T, Width>::One();
    } else {
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
XS_INLINE SIMDInBase<T, Width> operator+(SIMDInBase<T, Width> other1, SIMDInBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase<T, Width>(_mm_add_ss(other1.values, other2.values));
    } else
#endif
    {
        return SIMDInBase<T, Width>(other1.value + other2.value);
    }
}

/**
 * Subtract a object from another object.
 * @param other1 The first object.
 * @param other2 object to subtract from the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> operator-(SIMDInBase<T, Width> other1, SIMDInBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase<T, Width>(_mm_sub_ss(other1.values, other2.values));
    } else
#endif
    {
        return SIMDInBase<T, Width>(other1.value - other2.value);
    }
}

/**
 * Multiply two objects.
 * @param other1 The first object.
 * @param other2 The second object to multiply the first.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> operator*(SIMDInBase<T, Width> other1, SIMDInBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase<T, Width>(_mm_mul_ss(other1.values, other2.values));
    } else
#endif
    {
        return SIMDInBase<T, Width>(other1.value * other2.value);
    }
}

/**
 * Divide two objects.
 * @param other1 The first object.
 * @param other2 The second object to divide the first with.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> operator/(SIMDInBase<T, Width> other1, SIMDInBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase<T, Width>(_mm_div_ss(other1.values, other2.values));
    } else
#endif
    {
        return SIMDInBase<T, Width>(other1.value / other2.value);
    }
}

/**
 * Negate a object.
 * @param other The object to negate.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width> operator-(SIMDInBase<T, Width> other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase<T, Width>::Zero() - other;
    } else
#endif
    {
        return SIMDInBase<T, Width>(-other.value);
    }
}

/**
 * Perform compound assignment and addition with a object.
 * @param [in,out] other1 The first object.
 * @param          other2 object to add to first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width>& operator+=(SIMDInBase<T, Width>& other1, SIMDInBase<T, Width> other2) noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_add_ss(other1.values, other2.values);
    } else {
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
XS_INLINE SIMDInBase<T, Width>& operator-=(SIMDInBase<T, Width>& other1, SIMDInBase<T, Width> other2) noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_sub_ss(other1.values, other2.values);
    } else {
        other1.value -= other2.value;
    }
    return other1;
}

/**
 * Perform compound assignment and multiplication with a object.
 * @param [in,out] other1 The first object.
 * @param          other2 SIMDInBase<T, Width> to multiply the first one.
 * @returns The result of the operation.
 */
template<typename T, SIMDWidth Width>
XS_INLINE SIMDInBase<T, Width>& operator*=(SIMDInBase<T, Width>& other1, SIMDInBase<T, Width> other2) noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_mul_ss(other1.values, other2.values);
    } else {
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
XS_INLINE SIMDInBase<T, Width>& operator/=(SIMDInBase<T, Width>& other1, SIMDInBase<T, Width> other2) noexcept
{
    if constexpr (hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        other1.values = _mm_div_ss(other1.values, other2.values);
    } else {
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
XS_INLINE bool operator==(SIMDInBase<T, Width> other1, SIMDInBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
XS_INLINE bool operator<=(SIMDInBase<T, Width> other1, SIMDInBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
XS_INLINE bool operator<(SIMDInBase<T, Width> other1, SIMDInBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
XS_INLINE bool operator!=(SIMDInBase<T, Width> other1, SIMDInBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
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
XS_INLINE SIMDInBase<T, Width> operator&(SIMDInBase<T, Width> other1, SIMDInBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase<T, Width>(_mm_and_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMDInBase<T, Width>(other1.value & other2.value);
        } else {
            return SIMDInBase<T, Width>(Shift::bitAnd(other1.value, other2.value));
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
XS_INLINE SIMDInBase<T, Width> operator|(SIMDInBase<T, Width> other1, SIMDInBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase<T, Width>(_mm_or_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMDInBase<T, Width>(other1.value | other2.value);
        } else {
            return SIMDInBase<T, Width>(Shift::bitOr(other1.value, other2.value));
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
XS_INLINE SIMDInBase<T, Width> operator^(SIMDInBase<T, Width> other1, SIMDInBase<T, Width> other2) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        return SIMDInBase<T, Width>(_mm_xor_ps(other1.values, other2.values));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMDInBase<T, Width>(other1.value ^ other2.value);
        } else {
            return SIMDInBase<T, Width>(Shift::bitXor(other1.value, other2.value));
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
XS_INLINE SIMDInBase<T, Width> operator~(SIMDInBase<T, Width> other) noexcept
{
#if XS_ISA == XS_X86
    if constexpr (isSame<T, float32> && hasSIMD<T> && (Width > SIMDWidth::Scalar)) {
        const __m128 val = _mm_undefined_ps();
        return SIMDInBase<T, Width>(_mm_xor_ps(other.values, _mm_cmpeq_ps(val, val)));
    } else
#endif
    {
        if constexpr (isInteger<T>) {
            return SIMDInBase<T, Width>(~other.value);
        } else {
            return SIMDInBase<T, Width>(Shift::bitNot(other.value));
        }
    }
}
} // namespace Shift
