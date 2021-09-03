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

#include "XSArchitecture.hpp"
#include "XSTraits.hpp"

#if XS_ISA == XS_X86
#    include <immintrin.h> //tzcnt
#    include <nmmintrin.h> //popcnt
#endif
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_CLANGWIN)
#    include <intrin.h>
#endif

namespace Shift {
/**
 * Reverse the order of each byte in an integer.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isSameAny<T, int16, uint16, int32, uint32, int64, uint64>))
XS_INLINE T bswap(T param) noexcept
{
    static_assert(isSameAny<T, int16, uint16, int32, uint32, int64, uint64>);
    if constexpr (isSameAny<T, int16, uint16>) {
#if (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_CLANGWIN)
        return static_cast<T>(_rotr16(static_cast<uint16>(param), 8));
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG)
        return static_cast<T>(__builtin_bswap16(static_cast<uint16>(param)));
#else
        return (param >> 8) | (param << 8);
#endif
    } else if constexpr (isSameAny<T, int32, uint32>) {
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
        return static_cast<T>(_bswap(static_cast<uint32>(param)));
#elif (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_CLANGWIN)
        return static_cast<T>(_byteswap_ulong(static_cast<uint32>(param)));
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG)
        return static_cast<T>(__builtin_bswap32(static_cast<uint32>(param)));
#else
        T temp = ((param << 8) & 0xFF00FF00) | ((param >> 8) & 0x00FF00FF);
        return (temp >> 16) | (temp << 16);
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
        if constexpr (currentArch == Architecture::Bit64) {
            return static_cast<T>(_bswap64(static_cast<uint64>(param)));
        } else {
            return (static_cast<T>(_bswap(static_cast<uint32_t>(param))) << 32) |
                _bswap(static_cast<uint32_t>(param >> 32));
        }
#elif (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_CLANGWIN)
        return static_cast<T>(_byteswap_uint64(static_cast<uint64>(param)));
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG)
        return T{__builtin_bswap64(static_cast<uint64>(param))};
#else
        T temp = ((param << 8) & 0xFF00FF00FF00FF00ULL) | ((param >> 8) & 0x00FF00FF00FF00FFULL);
        temp = ((temp << 16) & 0xFFFF0000FFFF0000ULL) | ((temp >> 16) & 0x0000FFFF0000FFFFULL);
        return (temp >> 32) | (temp << 32);
#endif
    } else {
        return T{0};
    }
}

/**
 * Returns binary logarithm of an integer.
 * @note The return is undefined if the input is zero.
 * @param param The integer.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isSameAny<T, int32, uint32, int64, uint64>))
XS_INLINE uint32 bsr(T param) noexcept
{
    static_assert(isSameAny<T, int32, uint32, int64, uint64>);
    if constexpr (isSameAny<T, int32, uint32>) {
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
        // return is undefined if input is zero
        return _bit_scan_reverse(
            static_cast<uint32_t>(param)); // faster than _BitScanReverse as it avoids memory indirection
#elif XS_COMPILER == XS_MSVC
        // return is undefined if input is zero (function returns 0 but we are not checking that)
        unsigned long res;
        _BitScanReverse(&res, static_cast<uint32>(param));
        return res;
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
        // return is undefined if input is zero
        uint32 res;
        asm("bsr %1,%0" : "=r"(res) : "rm"(param));
        return res;
#elif XS_COMPILER == XS_NVCC
        // return is -1 if input is zero
        return 31 - __clz(param);
#else
#    error Unsupported compiler
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
        // return is undefined if input is zero
        if constexpr (currentArch == Architecture::Bit64) {
            uint64_t res;
            __asm__("bsrq %1,%0" : "=r"(res) : "rm"(param));
            return res;
        } else {
            // faster than _BitScanReverse as it avoids memory indirection
            const auto high = static_cast<uint32>(static_cast<uint64>(param) >> 32);
            return (high) ? _bit_scan_reverse(high) + 32 : _bit_scan_reverse(static_cast<uint32>(param));
        }
#elif XS_COMPILER == XS_MSVC
        unsigned long res;
        // return is undefined if input is zero (function returns 0 but we are not checking that)
        if constexpr (currentArch == Architecture::Bit64) {
            _BitScanReverse64(&res, param);
        } else {
            if (const auto high = static_cast<uint32>(static_cast<uint64>(param) >> 32); high) {
                _BitScanReverse(&res, high);
                res += 32;
            } else {
                _BitScanReverse(&res, static_cast<uint32>(param));
            }
        }
        return res;
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
        // return is undefined if input is zero
        if constexpr (currentArch == Architecture::Bit64) {
            uint64 res;
            asm("bsrq %1,%0" : "=r"(res) : "rm"(param));
            return static_cast<uint32>(res);
        } else {
            uint32 res;
            auto high = static_cast<uint32>(static_cast<uint64>(param) >> 32);
            if (high) {
                asm("bsrl %1,%0" : "=r"(res) : "rm"(high));
                res += 32;
            } else {
                asm("bsrl %1,%0" : "=r"(res) : "rm"(static_cast<uint32>(param)));
            }
            return res;
        }
#elif XS_COMPILER == XS_NVCC
        // return is -1 if input is zero
        return 63 - __clzll(param);
#else
#    error Unsupported compiler
#endif
    } else {
        return T{0};
    }
}

/**
 * Returns number of '1' bits in an integer.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isInteger<T> && isNative<T>))
XS_INLINE uint32 popcnt(T param) noexcept
{
    static_assert(isInteger<T> && isNative<T>);
    if constexpr (isSameAny<T, int32, uint32>) {
#if XS_COMPILER == XS_NVCC
        return __popc(param);
#else
        if constexpr (hasISAFeature<ISAFeature::SSE42> || hasISAFeature<ISAFeature::ABM>) {
            return static_cast<uint32>(_mm_popcnt_u32(static_cast<uint32>(
                param))); // popcnt added to AMD with Barcelona(SSE4a) and Intel with Nehalem(SSE4.2)
        } else {
#    if (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
            return static_cast<uint32>(__builtin_popcount(static_cast<uint32>(param)));
#    else
            uint32 res = static_cast<uint32>(param) - ((static_cast<uint32>(param) >> 1_ui8) & 0x55555555_ui32);
            res = (res & 0x33333333_ui32) + ((res >> 2_ui8) & 0x33333333_ui32);
            res = (res + (res >> 4)) & 0x0F0F0F0F_ui32;
            return (res * 0x01010101_ui32) >> 24_ui8;
#    endif
        }
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if XS_COMPILER == XS_NVCC
        return __popcll(param);
#else
        if constexpr (hasISAFeature<ISAFeature::SSE42> || hasISAFeature<ISAFeature::ABM>) {
            if constexpr (currentArch == Architecture::Bit64) {
                return static_cast<uint32>(_mm_popcnt_u64(static_cast<uint64>(param)));
            } else {
                return static_cast<uint32>(_mm_popcnt_u32(static_cast<uint32>(static_cast<uint64>(param) >> 32)) +
                    _mm_popcnt_u32(static_cast<uint32>(param)));
            }
        } else {
#    if (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
            return static_cast<uint32>(__builtin_popcountll(static_cast<uint64>(param)));
#    else
            uint64 res = static_cast<uint64>(param) - ((static_cast<uint64>(param) >> 1_ui8) & 0x5555555555555555_ui64);
            res = (res & 0x3333333333333333_ui64) + ((res >> 2_ui8) & 0x3333333333333333_ui64);
            res = (res + (res >> 4)) & 0x0F0F0F0F0F0F0F0F_ui64;
            return static_cast<uint32>((res * 0x0101010101010101_ui64) >> 56_ui8);
#    endif
        }
#endif
    } else if constexpr (isSameAny<T, int8, uint8>) {
        return popcnt(static_cast<uint32>(static_cast<uint8>(param)));
    } else if constexpr (isSameAny<T, int16, uint16>) {
        return popcnt(static_cast<uint32>(static_cast<uint16>(param)));
    } else {
        return 0_ui32;
    }
}

/**
 * Count the number of trailing '0' bits after the LSB '1' in an integer.
 * @note The return is undefined if the input is zero.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isInteger<T> && isNative<T>))
XS_INLINE uint32 ctz(T param) noexcept
{
    static_assert(isInteger<T> && isNative<T>);
    if constexpr (isSameAny<T, int32, uint32>) {
#if XS_COMPILER == XS_NVCC
        // return is 0 if input is zero
        return __ffs(param) - 1;
#else
        if constexpr (hasISAFeature<ISAFeature::BMI>) {
            // return is 32 if input is zero
            return _tzcnt_u32(param); // tzcnt added to AMD with Piledriver(AVX) and Intel with Haswell(AVX2)
        } else {
#    if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
            // return is undefined if input is zero
            return _bit_scan_forward(param); // faster than _BitScanForward as it avoids memory indirection
#    elif XS_COMPILER == XS_MSVC
            // return is undefined if input is zero (function returns 0 but we are not checking that)
            unsigned long res;
            _BitScanForward(&res, param);
            return res;
#    elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
            // return is undefined if input is zero
            return static_cast<uint32>(__builtin_ctz(static_cast<uint32>(param)));
#    else
#        error Unsupported compiler
#    endif
        }
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if XS_COMPILER == XS_NVCC
        // return is 0 if input is zero
        return __ffsll(param) - 1;
#else
        if constexpr (hasISAFeature<ISAFeature::BMI>) {
            // return is 64 if input is zero
            if constexpr (currentArch == Architecture::Bit64) {
                return static_cast<uint32>(_tzcnt_u64(static_cast<uint64>(param)));
            } else {
                return (static_cast<uint32>(param) == 0) ?
                    _tzcnt_u32(static_cast<uint32>(static_cast<uint64>(param) >> 32)) + 32 :
                    _tzcnt_u32(static_cast<uint32>(param));
            }
        } else {
#    if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
            // return is undefined if input is zero
            if constexpr (currentArch == Architecture::Bit64) {
                uint64_t res;
                __asm__("bsfq %1,%0" : "=r"(res) : "rm"(param));
                return static_cast<uint32_t>(res);
            } else {
                // faster than _BitScanReverse as it avoids memory indirection
                return (static_cast<uint32_t>(param) == 0) ?
                    _bit_scan_forward(static_cast<uint32_t>(static_cast<uint64>(param) >> 32)) + 32 :
                    _bit_scan_forward(static_cast<uint32_t>(param));
            }
#    elif XS_COMPILER == XS_MSVC
            // return is undefined if input is zero (function returns 0 but we are not checking that)
            unsigned long res;
            if constexpr (currentArch == Architecture::Bit64) {
                _BitScanForward64(&res, param);
            } else {
                if (static_cast<uint32>(param) == 0) {
                    _BitScanForward(&res, static_cast<uint32>(static_cast<uint64>(param) >> 32));
                    res += 32;
                } else {
                    _BitScanForward(&res, static_cast<uint32>(param));
                }
            }
            return res;
#    elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
            // return is undefined if input is zero
            return static_cast<uint32>(__builtin_ctzll(static_cast<uint64>(param)));
#    else
#        error Unsupported compiler
#    endif
        }
#endif
    } else if constexpr (isSigned<T>) {
        return ctz(static_cast<int32>(param));
    } else if constexpr (isUnsigned<T>) {
        return ctz(static_cast<uint32>(param));
    } else {
        return 0_ui32;
    }
}

/**
 * Count the number of leading '0' bits before the MSB '1' in an integer.
 * @note The return is undefined if the input is zero.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isSameAny<T, int32, uint32, int64, uint64>))
XS_INLINE uint32 clz(T param) noexcept
{
    static_assert(isSameAny<T, int32, uint32, int64, uint64>);
    if constexpr (isSameAny<T, int32, uint32>) {
#if XS_COMPILER == XS_NVCC
        // return is 32 if input is zero
        return __clz(param);
#else
        if constexpr (hasISAFeature<ISAFeature::ABM>) {
            // return is 32 if input is zero
            return _lzcnt_u32(param); // lzcnt added to AMD with Barcelona(SSE4a) and Intel with Haswell(AVX2)
        } else {
#    if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
            // return is undefined if input is zero
            return 31 - _bit_scan_reverse(param);
#    elif XS_COMPILER == XS_MSVC
            // return is undefined if input is zero (function returns 0 but we are not checking that)
            unsigned long res;
            _BitScanReverse(&res, param);
            return 31 - res;
#    elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
            // return is undefined if input is zero
            return static_cast<uint32>(__builtin_clz(static_cast<uint32>(param)));
#    else
#        error Unsupported compiler
#    endif
        }
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if XS_COMPILER == XS_NVCC
        // return is 64 if input is zero
        return __clzll(param);
#else
        if constexpr (hasISAFeature<ISAFeature::ABM>) {
            if constexpr (currentArch == Architecture::Bit64) {
                return static_cast<uint32>(_lzcnt_u64(param));
            } else {
                const auto high = static_cast<uint32>(static_cast<uint64>(param) >> 32);
                return (high == 0) ? _lzcnt_u32(static_cast<uint32>(param)) + 32 : _lzcnt_u32(high);
            }
        } else {
#    if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
            // return is undefined if input is zero
            if constexpr (currentArch == Architecture::Bit64) {
                uint64 res;
                __asm__("bsrq %1,%0" : "=r"(res) : "rm"(param));
                return 63 - static_cast<uint32>(res);
            } else {
                // faster than _BitScanReverse as it avoids memory indirection
                const auto high = static_cast<uint32>(static_cast<uint64>(param) >> 32);
                return (high == 0) ? 63 - _bit_scan_reverse(static_cast<uint32>(param)) : 31 - _bit_scan_reverse(high);
            }
#    elif XS_COMPILER == XS_MSVC
            // return is undefined if input is zero (function returns 0 but we are not checking that)
            unsigned long res;
            if constexpr (currentArch == Architecture::Bit64) {
                _BitScanReverse64(&res, param);
                return 63 - res;
            } else {
                const auto high = static_cast<uint32>(static_cast<uint64>(param) >> 32);
                if (high == 0) {
                    _BitScanReverse(&res, static_cast<uint32>(param));
                    return 63 - res;
                }
                _BitScanReverse(&res, high);
                return 31 - res;
            }
#    elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
            // return is undefined if input is zero
            return static_cast<uint32>(__builtin_clzll(static_cast<uint64>(param)));
#    else
#        error Unsupported compiler
#    endif
        }
#endif
    } else {
        return T{0};
    }
}

/**
 * Extract the value of a specific bit from an integer.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @param bit   The zero-based index of the bit to get.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isInteger<T> && isNative<T>))
XS_INLINE uint8 bitExtract(T param, uint32 bit) noexcept
{
    static_assert(isInteger<T> && isNative<T>);
    if constexpr (isSameAny<T, int32, uint32>) {
#if (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || \
    (XS_COMPILER == XS_CLANGWIN)
        uint8 ret = 0;
        __asm__("bt %1,%2 \n\t"
                "setb %0 \n\t"
                : "=r"(ret)
                : "r"(bit), "rm"(param)
                : "flags");
        return ret;
#elif XS_COMPILER == XS_MSVC
        return static_cast<uint8>(_bittest(reinterpret_cast<const long*>(&param), bit));
#else
        return static_cast<uint8>((static_cast<uint32>(param) >> bit) & 0x1);
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || \
    (XS_COMPILER == XS_CLANGWIN)
        uint8 ret = 0;
        if constexpr (currentArch == Architecture::Bit64) {
            const uint64 bit2(bit);
            __asm__("btq %1,%2 \n\t"
                    "setb %0 \n\t"
                    : "=r"(ret)
                    : "r"(bit2), "rm"(param)
                    : "flags");
        } else {
            auto val = (bit < 32) ? static_cast<uint32>(param) : static_cast<uint32>(static_cast<uint64>(param) >> 32);
            const auto bit2 = (bit < 32) ? bit : bit - 32;
            __asm__("btl %1,%2 \n\t"
                    "setb %0 \n\t"
                    : "=r"(ret)
                    : "r"(bit2), "rm"(val)
                    : "flags");
        }
        return ret;
#elif XS_COMPILER == XS_MSVC
        if constexpr (currentArch == Architecture::Bit64) {
            return _bittest64(reinterpret_cast<const long long*>(&param), bit);
        } else {
            auto val = (bit < 32) ? static_cast<uint32>(param) : static_cast<uint32>(static_cast<uint64>(param) >> 32);
            const auto bit2 = (bit < 32) ? bit : bit - 32;
            return _bittest(reinterpret_cast<const long*>(&val), bit2);
        }
#else
        return static_cast<uint8>((static_cast<uint32>(param) >> bit) & 0x1_ui32);
#endif
    } else if constexpr (isSigned<T>) {
        return bitExtract(static_cast<int32>(param), bit);
    } else if constexpr (isUnsigned<T>) {
        return bitExtract(static_cast<uint32>(param), bit);
    } else {
        return static_cast<uint8>((param >> bit) & 0x1);
    }
}

/**
 * Set a specific bit from a integer to '1'.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @param bit   The zero-based index of the bit to get.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isInteger<T> && isNative<T>))
XS_INLINE T bitSet(T param, uint32 bit) noexcept
{
    static_assert(isInteger<T> && isNative<T>);
    T ret(param);
    if constexpr (isSameAny<T, int32, uint32>) {
#if (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || \
    (XS_COMPILER == XS_CLANGWIN)
        __asm__("bts %1,%0" : "=r"(ret) : "r"(bit), "0"(param) : "flags");
#elif XS_COMPILER == XS_MSVC
        _bittestandset(reinterpret_cast<long*>(&ret), bit);
#else
        ret = param | (1 << bit);
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || \
    (XS_COMPILER == XS_CLANGWIN)
        if constexpr (currentArch == Architecture::Bit64) {
            const uint64 bit2(bit);
            __asm__("btsq %1,%0" : "=r"(ret) : "r"(bit2), "0"(param) : "flags");
        } else {
            auto val = (bit < 32) ? static_cast<uint32>(param) : static_cast<uint32>(static_cast<uint64>(param) >> 32);
            const auto bit2 = (bit < 32) ? bit : bit - 32;
            __asm__("btsl %1,%0" : "=r"(val) : "r"(bit2), "0"(val) : "flags");
            reinterpret_cast<uint32*>(&ret)[bit >= 32] = val;
        }
#elif XS_COMPILER == XS_MSVC
        if constexpr (currentArch == Architecture::Bit64) {
            _bittestandset64(reinterpret_cast<long long*>(&ret), bit);
        } else {
            auto val = (bit < 32) ? static_cast<uint32>(param) : static_cast<uint32>(static_cast<uint64>(param) >> 32);
            const auto bit2 = (bit < 32) ? bit : bit - 32;
            _bittestandset(reinterpret_cast<long*>(&val), bit2);
            reinterpret_cast<uint32*>(&ret)[bit >= 32] = val;
        }
#else
        ret = (param | (T{1} << bit));
#endif
    } else if constexpr (isSigned<T>) {
        ret = static_cast<T>(bitSet(static_cast<int32>(param), bit));
    } else if constexpr (isUnsigned<T>) {
        ret = static_cast<T>(bitSet(static_cast<uint32>(param), bit));
    } else {
        ret = (param | (T{1} << bit));
    }
    return ret;
}

/**
 * Set a specific bit from a integer to '0'.
 * @tparam T Generic type parameter.
 * @param param The integer.
 * @param bit   The zero-based index of the bit to get.
 * @returns Resultant value.
 */
template<typename T>
XS_REQUIRES((isInteger<T> && isNative<T>))
XS_INLINE T bitClear(T param, uint32 bit) noexcept
{
    static_assert(isInteger<T> && isNative<T>);
    T ret(param);
    if constexpr (isSameAny<T, int32, uint32>) {
#if (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || \
    (XS_COMPILER == XS_CLANGWIN)
        __asm__("btr %1,%0" : "=r"(ret) : "r"(bit), "0"(param) : "flags");
#elif XS_COMPILER == XS_MSVC
        _bittestandreset(reinterpret_cast<long*>(&ret), bit);
#else
        ret = param & ~(1_ui32 << bit);
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || \
    (XS_COMPILER == XS_CLANGWIN)
        if constexpr (currentArch == Architecture::Bit64) {
            const uint64_t bit2(bit);
            __asm__("btrq %1,%0" : "=r"(ret) : "r"(bit2), "0"(param) : "flags");
        } else {
            auto val = (bit < 32) ? static_cast<uint32>(param) : static_cast<uint32>(static_cast<uint64>(param) >> 32);
            const auto bit2 = (bit < 32) ? bit : bit - 32;
            __asm__("btrl %1,%0" : "=r"(val) : "r"(bit2), "0"(val) : "flags");
            reinterpret_cast<uint32*>(&ret)[bit >= 32] = val;
        }
#elif XS_COMPILER == XS_MSVC
        if constexpr (currentArch == Architecture::Bit64) {
            _bittestandreset64(reinterpret_cast<long long*>(&ret), bit);
        } else {
            auto val = (bit < 32) ? static_cast<uint32>(param) : static_cast<uint32>(static_cast<uint64>(param) >> 32);
            const auto bit2 = (bit < 32) ? bit : bit - 32;
            _bittestandreset(reinterpret_cast<long*>(&val), bit2);
            reinterpret_cast<uint32*>(&ret)[bit >= 32] = val;
        }
#else
        ret = (param & ~(T{1} << bit));
#endif
    } else if constexpr (isSigned<T>) {
        ret = static_cast<T>(bitClear(static_cast<int32>(param), bit));
    } else if constexpr (isUnsigned<T>) {
        ret = static_cast<T>(bitClear(static_cast<uint32>(param), bit));
    } else {
        ret = (param & ~(T{1} << bit));
    }

    return ret;
}

/**
 * Bit cast from one type to another.
 * @tparam T  Generic type parameter.
 * @tparam T2 Generic type parameter.
 * @param param The parameter.
 * @returns A result.
 */
template<typename T, typename T2>
XS_REQUIRES(((isSame<T, T2> || ((sizeof(T) == sizeof(T2)) && (alignof(T) <= alignof(T2)))) && isTriviallyCopyable<T> &&
    isTriviallyCopyable<T2>))
XS_INLINE T bitCast(T2 param) noexcept
{
    static_assert((isSame<T, T2> || ((sizeof(T) == sizeof(T2)) && (alignof(T) <= alignof(T2)))) &&
        isTriviallyCopyable<T> && isTriviallyCopyable<T2>);
    if constexpr (isSame<T, T2>) {
        return param;
    } else if constexpr (isInteger<T> && isInteger<T2> && (isSigned<T> != isSigned<T2>)) {
        return static_cast<T>(param);
    } else
#if XS_ISA == XS_X86 && \
    ((XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN))
        if constexpr (isSame<T2, float32> && isSameAny<T, uint32, int32>) {
        return static_cast<T>(_castf32_u32(param));
    } else if constexpr (isSame<T2, float64> && isSameAny<T, uint64, int64>) {
        return static_cast<T>(_castf64_u64(param));
    } else if constexpr (isSameAny<T2, uint32, int32> && isSame<T, float32>) {
        return _castu32_f32(static_cast<T>(param));
    } else if constexpr (isSameAny<T2, uint64, int64> && isSame<T, float64>) {
        return _castu64_f64(static_cast<T>(param));
    } else
#endif
#if XS_ARCH_SSE
        if constexpr (isSame<T2, __m128d> && isSame<T, __m128>) {
        return _mm_castpd_ps(param);
    } else if constexpr (isSame<T2, __m128d> && isSame<T, __m128i>) {
        return _mm_castpd_si128(param);
    } else if constexpr (isSame<T2, __m128> && isSame<T, __m128d>) {
        return _mm_castps_pd(param);
    } else if constexpr (isSame<T2, __m128> && isSame<T, __m128i>) {
        return _mm_castps_si128(param);
    } else if constexpr (isSame<T2, __m128i> && isSame<T, __m128d>) {
        return _mm_castsi128_pd(param);
    } else if constexpr (isSame<T2, __m128i> && isSame<T, __m128>) {
        return _mm_castsi128_ps(param);
    } else
#endif
#if XS_ARCH_AVX
        if constexpr (isSame<T2, __m256d> && isSame<T, __m256>) {
        return _mm256_castpd_ps(param);
    } else if constexpr (isSame<T2, __m256d> && isSame<T, __m256i>) {
        return _mm256_castpd_si256(param);
    } else if constexpr (isSame<T2, __m256> && isSame<T, __m256d>) {
        return _mm256_castps_pd(param);
    } else if constexpr (isSame<T2, __m256> && isSame<T, __m256i>) {
        return _mm256_castps_si256(param);
    } else if constexpr (isSame<T2, __m256i> && isSame<T, __m256d>) {
        return _mm256_castsi256_pd(param);
    } else if constexpr (isSame<T2, __m256i> && isSame<T, __m256>) {
        return _mm256_castsi256_ps(param);
    } else
#endif
#if XS_ARCH_AVX512F
        if constexpr (isSame<T2, __m512d> && isSame<T, __m512>) {
        return _mm512_castpd_ps(param);
    } else if constexpr (isSame<T2, __m512d> && isSame<T, __m512i>) {
        return _mm512_castpd_si512(param);
    } else if constexpr (isSame<T2, __m512> && isSame<T, __m512d>) {
        return _mm512_castps_pd(param);
    } else if constexpr (isSame<T2, __m512> && isSame<T, __m512i>) {
        return _mm512_castps_si512(param);
    } else if constexpr (isSame<T2, __m512i> && isSame<T, __m512d>) {
        return _mm512_castsi512_pd(param);
    } else if constexpr (isSame<T2, __m512i> && isSame<T, __m512>) {
        return _mm512_castsi512_ps(param);
    } else
#endif
#if XS_ARCH_AVX512FP16
        if constexpr (isSame<T2, __m128h> && isSame<T, __m128d>) {
        return _mm_castph_pd(param);
    } else if constexpr (isSame<T2, __m128h> && isSame<T, __m128>) {
        return _mm_castph_ps(param);
    } else if constexpr (isSame<T2, __m128h> && isSame<T, __m128i>) {
        return _mm_castph_si128(param);
    } else if constexpr (isSame<T2, __m128> && isSame<T, __m128h>) {
        return _mm_castph_ps(param);
    } else if constexpr (isSame<T2, __m128i> && isSame<T, __m128h>) {
        return _mm_castph_si128(param);
    } else if constexpr (isSame<T2, __m128d> && isSame<T, __m128h>) {
        return _mm_castph_pd(param);
    } else if constexpr (isSame<T2, __m256h> && isSame<T, __m256d>) {
        return _mm256_castph_pd(param);
    } else if constexpr (isSame<T2, __m256h> && isSame<T, __m256>) {
        return _mm256_castph_ps(param);
    } else if constexpr (isSame<T2, __m256h> && isSame<T, __m256i>) {
        return _mm256_castph_si128(param);
    } else if constexpr (isSame<T2, __m256> && isSame<T, __m256h>) {
        return _mm256_castph_ps(param);
    } else if constexpr (isSame<T2, __m256i> && isSame<T, __m256h>) {
        return _mm256_castph_si128(param);
    } else if constexpr (isSame<T2, __m256d> && isSame<T, __m256h>) {
        return _mm256_castph_pd(param);
    } else if constexpr (isSame<T2, __m512h> && isSame<T, __m512d>) {
        return _mm512_castph_pd(param);
    } else if constexpr (isSame<T2, __m512h> && isSame<T, __m512>) {
        return _mm512_castph_ps(param);
    } else if constexpr (isSame<T2, __m512h> && isSame<T, __m512i>) {
        return _mm512_castph_si128(param);
    } else if constexpr (isSame<T2, __m512> && isSame<T, __m512h>) {
        return _mm512_castph_ps(param);
    } else if constexpr (isSame<T2, __m512i> && isSame<T, __m512h>) {
        return _mm512_castph_si128(param);
    } else if constexpr (isSame<T2, __m512d> && isSame<T, __m512h>) {
        return _mm512_castph_pd(param);
    } else
#endif
#if XS_ISA == XS_CUDA
        if constexpr (isSame<T2, float64> && isSameAny<T, int64, uint64>) {
        return static_cast<T>(__double_as_longlong(param));
    } else if constexpr (isSame<T2, float32> && isSame<T, int32>) {
        return __float_as_int(param);
    } else if constexpr (isSame<T2, float32> && isSame<T, uint32>) {
        return __float_as_uint(param);
    } else if constexpr (isSameAny<T2, int64, uint64> && isSame<T, float64>) {
        return __longlong_as_double(static_cast<int64>(param));
    } else if constexpr (isSame<T2, int32> && isSame<T, float32>) {
        return __int_as_float(param);
    } else if constexpr (isSame<T2, uint32> && isSame<T, float32>) {
        return __uint_as_float(param);
    } else
#endif
    {
        // Reinterpret cast and unions are not usable due to aliasing rules.
#ifdef __cpp_lib_bit_cast
        return __builtin_bit_cast(T, param);
#else
        T ret;
        memcpy(&ret, &param, sizeof(T));
        return ret;
#endif
    }
}

/**
 * Bitwise and of two values
 * @tparam T  Generic type parameter.
 * @tparam T2 Generic type parameter.
 * @param param1 The first parameter.
 * @param param2 The second parameter.
 * @returns The result.
 */
template<typename T, typename T2>
XS_REQUIRES(((isSame<T, T2> || ((sizeof(T) == sizeof(T2)) && (alignof(T) <= alignof(T2)))) && isTriviallyCopyable<T> &&
    isTriviallyCopyable<T2>))
XS_INLINE T bitAnd(T param1, T2 param2) noexcept
{
    static_assert((isSame<T, T2> || ((sizeof(T) == sizeof(T2)) && (alignof(T) <= alignof(T2)))) &&
        isTriviallyCopyable<T> && isTriviallyCopyable<T2>);
    if constexpr (isInteger<T> && isInteger<T2>) {
        return param1 & static_cast<T>(param2);
    } else if constexpr (isInteger<T>) {
        return param1 & bitCast<T>(param2);
    } else if constexpr (isInteger<T2>) {
        return bitCast<T>(bitCast<T2>(param1) & param2);
    } else if constexpr (sizeof(T) == sizeof(uint8)) {
        return bitCast<T>(bitCast<uint8>(param1) & bitCast<uint8>(param2));
    } else if constexpr (sizeof(T) == sizeof(uint16)) {
        return bitCast<T>(bitCast<uint16>(param1) & bitCast<uint16>(param2));
    } else if constexpr (sizeof(T) == sizeof(uint32)) {
        return bitCast<T>(bitCast<uint32>(param1) & bitCast<uint32>(param2));
    } else if constexpr (sizeof(T) == sizeof(uint64)) {
        return bitCast<T>(bitCast<uint64>(param1) & bitCast<uint64>(param2));
    } else {
        // Reinterpret cast is valid only when casting to char array
        uint8 ret1[sizeof(T)] = reinterpret_cast<uint8*>(&param1); // NOLINT(modernize-avoid-c-arrays)
        uint8 ret2[sizeof(T)] = reinterpret_cast<uint8*>(&param2); // NOLINT(modernize-avoid-c-arrays)
        for (auto i = 0; i < sizeof(T); ++i) {
            ret1[i] &= ret2[i];
        }
        return *reinterpret_cast<T*>(ret1);
    }
}

/**
 * Bitwise or of two values
 * @tparam T  Generic type parameter.
 * @tparam T2 Generic type parameter.
 * @param param1 The first parameter.
 * @param param2 The second parameter.
 * @returns The result.
 */
template<typename T, typename T2>
XS_REQUIRES(((isSame<T, T2> || ((sizeof(T) == sizeof(T2)) && (alignof(T) <= alignof(T2)))) && isTriviallyCopyable<T> &&
    isTriviallyCopyable<T2>))
XS_INLINE T bitOr(T param1, T2 param2) noexcept
{
    static_assert((isSame<T, T2> || ((sizeof(T) == sizeof(T2)) && (alignof(T) <= alignof(T2)))) &&
        isTriviallyCopyable<T> && isTriviallyCopyable<T2>);
    if constexpr (isInteger<T> && isInteger<T2>) {
        return param1 | static_cast<T>(param2);
    } else if constexpr (isInteger<T>) {
        return param1 | bitCast<T>(param2);
    } else if constexpr (isInteger<T2>) {
        return bitCast<T>(bitCast<T2>(param1) | param2);
    } else if constexpr (sizeof(T) == sizeof(uint8)) {
        return bitCast<T>(bitCast<uint8>(param1) | bitCast<uint8>(param2));
    } else if constexpr (sizeof(T) == sizeof(uint16)) {
        return bitCast<T>(bitCast<uint16>(param1) | bitCast<uint16>(param2));
    } else if constexpr (sizeof(T) == sizeof(uint32)) {
        return bitCast<T>(bitCast<uint32>(param1) | bitCast<uint32>(param2));
    } else if constexpr (sizeof(T) == sizeof(uint64)) {
        return bitCast<T>(bitCast<uint64>(param1) | bitCast<uint64>(param2));
    } else {
        // Reinterpret cast is valid only when casting to char array
        uint8 ret1[sizeof(T)] = reinterpret_cast<uint8*>(&param1); // NOLINT(modernize-avoid-c-arrays)
        uint8 ret2[sizeof(T)] = reinterpret_cast<uint8*>(&param2); // NOLINT(modernize-avoid-c-arrays)
        for (auto i = 0; i < sizeof(T); ++i) {
            ret1[i] |= ret2[i];
        }
        return *reinterpret_cast<T*>(ret1);
    }
}

/**
 * Bitwise xor of two values
 * @tparam T  Generic type parameter.
 * @tparam T2 Generic type parameter.
 * @param param1 The first parameter.
 * @param param2 The second parameter.
 * @returns The result.
 */
template<typename T, typename T2>
XS_REQUIRES(((isSame<T, T2> || ((sizeof(T) == sizeof(T2)) && (alignof(T) <= alignof(T2)))) && isTriviallyCopyable<T> &&
    isTriviallyCopyable<T2>))
XS_INLINE T bitXor(T param1, T2 param2) noexcept
{
    static_assert((isSame<T, T2> || ((sizeof(T) == sizeof(T2)) && (alignof(T) <= alignof(T2)))) &&
        isTriviallyCopyable<T> && isTriviallyCopyable<T2>);
    if constexpr (isInteger<T> && isInteger<T2>) {
        return param1 ^ static_cast<T>(param2);
    } else if constexpr (isInteger<T>) {
        return param1 ^ bitCast<T>(param2);
    } else if constexpr (isInteger<T2>) {
        return bitCast<T>(bitCast<T2>(param1) ^ param2);
    } else if constexpr (sizeof(T) == sizeof(uint8)) {
        return bitCast<T>(bitCast<uint8>(param1) ^ bitCast<uint8>(param2));
    } else if constexpr (sizeof(T) == sizeof(uint16)) {
        return bitCast<T>(bitCast<uint16>(param1) ^ bitCast<uint16>(param2));
    } else if constexpr (sizeof(T) == sizeof(uint32)) {
        return bitCast<T>(bitCast<uint32>(param1) ^ bitCast<uint32>(param2));
    } else if constexpr (sizeof(T) == sizeof(uint64)) {
        return bitCast<T>(bitCast<uint64>(param1) ^ bitCast<uint64>(param2));
    } else {
        // Reinterpret cast is valid only when casting to char array
        uint8 ret1[sizeof(T)] = reinterpret_cast<uint8*>(&param1); // NOLINT(modernize-avoid-c-arrays)
        uint8 ret2[sizeof(T)] = reinterpret_cast<uint8*>(&param2); // NOLINT(modernize-avoid-c-arrays)
        for (auto i = 0; i < sizeof(T); ++i) {
            ret1[i] ^= ret2[i];
        }
        return *reinterpret_cast<T*>(ret1);
    }
}

/**
 * Bitwise not of a values
 * @tparam T  Generic type parameter.
 * @param param The parameter.
 * @returns The result.
 */
template<typename T>
XS_INLINE T bitNot(T param) noexcept
{
    if constexpr (isInteger<T>) {
        return ~param;
    } else if constexpr (sizeof(T) == sizeof(uint8)) {
        return bitCast<T>(~bitCast<uint8>(param));
    } else if constexpr (sizeof(T) == sizeof(uint16)) {
        return bitCast<T>(~bitCast<uint16>(param));
    } else if constexpr (sizeof(T) == sizeof(uint32)) {
        return bitCast<T>(~bitCast<uint32>(param));
    } else if constexpr (sizeof(T) == sizeof(uint64)) {
        return bitCast<T>(~bitCast<uint64>(param));
    } else {
        // Reinterpret cast is valid only when casting to char array
        uint8 ret[sizeof(T)] = reinterpret_cast<uint8*>(&param); // NOLINT(modernize-avoid-c-arrays)
        for (auto i = 0; i < sizeof(T); ++i) {
            ret[i] = ~ret[i];
        }
        return *reinterpret_cast<T*>(ret);
    }
}
} // namespace Shift
