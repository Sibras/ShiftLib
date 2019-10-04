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

#include "XSBit.hpp"
#include "XSMath.inl"

#if XS_ISA == XS_X86
#    include <immintrin.h> //tzcnt
#    include <nmmintrin.h> //popcnt
#endif
#if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_CLANGWIN)
#    include <intrin.h>
#endif

namespace Shift {
template<typename T>
XS_INLINE T bswap(const T param) noexcept
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

template<typename T>
XS_INLINE uint32 bsr(const T param) noexcept
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
#elif XS_COMPILER == XS_CUDA
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
            const auto high = static_cast<uint32>(static_cast<uint64>(param) >> 32);
            if (high) {
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
            return res;
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
#elif XS_COMPILER == XS_CUDA
        // return is -1 if input is zero
        return 63 - __clzll(param);
#else
#    error Unsupported compiler
#endif
    } else {
        return T{0};
    }
}

template<typename T>
XS_INLINE uint32 popcnt(const T param) noexcept
{
    static_assert(isInteger<T> && isNative<T>);
    if constexpr (isSameAny<T, int32, uint32>) {
#if XS_COMPILER == XS_CUDA
        return __popc(param);
#elif (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
        // Clang doesn't allow popcnt unless popcnt is enabled during compile
        return __builtin_popcount(param);
#else
        if constexpr (currentISA == ISA::X86 && defaultSIMD >= SIMD::SSE42) {
            return _mm_popcnt_u32(
                static_cast<uint32>(param)); // popcnt added to AMD with Barcelona(SSE4a) and Intel with Nehalem(SSE4.2)
        } else {
#    if (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
            return __builtin_popcount(param);
#    else
            uint32 res = static_cast<uint32>(param) - ((static_cast<uint32>(param) >> 1_ui8) & 0x55555555_ui32);
            res = (res & 0x33333333_ui32) + ((res >> 2_ui8) & 0x33333333_ui32);
            res = (res + (res >> 4)) & 0x0F0F0F0F_ui32;
            res = (res * 0x01010101_ui32) >> 24_ui8;
            return static_cast<uint32>(res);
#    endif
        }
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if XS_COMPILER == XS_CUDA
        return __popcll(param);
#elif (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
        // Clang doesn't allow popcnt unless popcnt is enabled during compile
        return __builtin_popcountll(param);
#else
        if constexpr (currentISA == ISA::X86 && defaultSIMD >= SIMD::SSE42) {
            if constexpr (currentArch == Architecture::Bit64) {
                return static_cast<uint32>(_mm_popcnt_u64(static_cast<uint64>(param)));
            } else {
                return _mm_popcnt_u32(static_cast<uint32>(static_cast<uint64>(param) >> 32)) +
                    _mm_popcnt_u32(static_cast<uint32>(param));
            }
        } else {
#    if (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
            return __builtin_popcountll(static_cast<uint64>(param));
#    else
            uint64 res = static_cast<uint64>(param) - ((static_cast<uint64>(param) >> 1_ui8) & 0x5555555555555555_ui64);
            res = (res & 0x3333333333333333_ui64) + ((res >> 2_ui8) & 0x3333333333333333_ui64);
            res = (res + (res >> 4)) & 0x0F0F0F0F0F0F0F0F_ui64;
            res = (res * 0x0101010101010101_ui64) >> 56_ui8;
            return static_cast<uint32>(res);
#    endif
        }
#endif
#if XS_ISA == XS_X86
    } else if constexpr (isSameAny<T, int8, uint8>) {
        return static_cast<T>(popcnt(static_cast<uint32>(static_cast<uint8>(param))));
    } else if constexpr (isSameAny<T, int16, uint16>) {
        return static_cast<T>(popcnt(static_cast<uint32>(static_cast<uint16>(param))));
    }
#else
    } else {
        return T{0};
    }
#endif
} // namespace Shift

template<typename T>
XS_INLINE uint32 ctz(const T param) noexcept
{
    static_assert(isInteger<T> && isNative<T>);
    if constexpr (isSameAny<T, int32, uint32>) {
#if XS_COMPILER == XS_CUDA
        // return is 0 if input is zero
        return __ffs(param) - 1;
#elif (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
        // Clang doesn't allow tzcnt unless BMI is enabled during compile
        return __builtin_ctz(param);
#else
            if constexpr (currentISA == ISA::X86 && defaultSIMD >= SIMD::AVX2) {
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
                return __builtin_ctz(param);
#    else
#        error Unsupported compiler
#    endif
            }
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if XS_COMPILER == XS_CUDA
        // return is 0 if input is zero
        return __ffsll(param) - 1;
#elif (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
        // Clang doesn't allow tzcnt unless BMI is enabled during compile
        return __builtin_ctzll(param);
#else
            if constexpr (currentISA == ISA::X86 && defaultSIMD >= SIMD::AVX2) {
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
                return __builtin_ctzll(param);
#    else
#        error Unsupported compiler
#    endif
            }
#endif
#if XS_ISA == XS_X86
    } else if constexpr (isSigned<T>) {
        return static_cast<T>(ctz(static_cast<int32>(param)));
    } else if constexpr (isUnsigned<T>) {
        return static_cast<T>(ctz(static_cast<uint32>(param)));
    }
#else
    } else {
        return T{0};
    }
#endif
} // namespace Shift

template<typename T>
XS_INLINE uint32 clz(const T param) noexcept
{
    static_assert(isSameAny<T, int32, uint32, int64, uint64>);
    if constexpr (isSameAny<T, int32, uint32>) {
#if XS_COMPILER == XS_CUDA
        // return is 32 if input is zero
        return __clz(param);
#elif (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
        // Clang doesn't allow lzcnt unless BMI is enabled during compile
        return __builtin_clz(param);
#else
                if constexpr (currentISA == ISA::X86 && defaultSIMD >= SIMD::AVX2) {
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
                    return __builtin_clz(param);
#    else
#        error Unsupported compiler
#    endif
                }
#endif
    } else if constexpr (isSameAny<T, int64, uint64>) {
#if XS_COMPILER == XS_CUDA
        // return is 64 if input is zero
        return __clzll(param);
#elif (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
        // Clang doesn't allow lzcnt unless BMI is enabled during compile
        return __builtin_clzll(param);
#else
                if constexpr (currentISA == ISA::X86 && defaultSIMD >= SIMD::AVX2) {
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
                        return (high == 0) ? 63 - _bit_scan_reverse(static_cast<uint32>(param)) :
                                             31 - _bit_scan_reverse(high);
                    }
#    elif XS_COMPILER == XS_MSVC
                    // return is undefined if input is zero (function returns 0 but we are not checking that)
                    unsigned long res;
                    if constexpr (currentArch == Architecture::Bit64) {
                        _BitScanReverse64(&res, param);
                        return 63 - res;
                    } else {
                        auto high = static_cast<uint32>(static_cast<uint64>(param) >> 32);
                        if (high == 0) {
                            _BitScanReverse(&res, static_cast<uint32>(param));
                            return 63 - res;
                        } else {
                            _BitScanReverse(&res, high);
                            return 31 - res;
                        }
                    }
#    elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
                    // return is undefined if input is zero
                    return __builtin_clzll(param);
#    else
#        error Unsupported compiler
#    endif
                }
#endif
    } else {
        return T{0};
    }
} // namespace Shift

template<typename T>
XS_INLINE uint8 bitExtract(const T param, const uint32 bit) noexcept
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
#if XS_ISA == XS_X86
    } else if constexpr (isSigned<T>) {
        return static_cast<T>(bitExtract(static_cast<int32>(param), bit));
    } else if constexpr (isUnsigned<T>) {
        return static_cast<T>(bitExtract(static_cast<uint32>(param), bit));
    }
#else
    } else {
        return static_cast<uint8>((param >> bit) & 0x1);
    }
#endif
}

template<typename T>
XS_INLINE T bitSet(const T param, const uint32 bit) noexcept
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
#if XS_ISA == XS_X86
    } else if constexpr (isSigned<T>) {
        ret = static_cast<T>(bitSet(static_cast<int32>(param), bit));
    } else if constexpr (isUnsigned<T>) {
        ret = static_cast<T>(bitSet(static_cast<uint32>(param), bit));
    }
#else
    } else {
        ret = (param | (T{1} << bit));
    }
#endif
    return ret;
}

template<typename T>
XS_INLINE T bitClear(const T param, const uint32 bit) noexcept
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
#if XS_ISA == XS_X86
    } else if constexpr (isSigned<T>) {
        ret = static_cast<T>(bitClear(static_cast<int32>(param), bit));
    } else if constexpr (isUnsigned<T>) {
        ret = static_cast<T>(bitClear(static_cast<uint32>(param), bit));
    }
#else
    } else {
        ret = (param & ~(T{1} << bit));
    }
#endif

    return ret;
}

template<typename T, typename T2>
XS_INLINE T bitCast(const T2& param) noexcept
{
    static_assert(isSame<T, T2> || (sizeof(T) == sizeof(T2)));
    if constexpr (isSame<T, T2>) {
        return param;
    } else {
        // Reinterpret cast and unions are not usable due to aliasing rules.
        T ret;
        memcpy(&ret, &param, sizeof(T));
        return ret;
    }
}

template<typename T, typename T2>
XS_INLINE T bitAnd(const T& param1, const T2& param2) noexcept
{
    static_assert(isSame<T, T2> || (sizeof(T) == sizeof(T2)));
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

template<typename T, typename T2>
XS_INLINE T bitOr(const T& param1, const T2& param2) noexcept
{
    static_assert(isSame<T, T2> || (sizeof(T) == sizeof(T2)));
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

template<typename T, typename T2>
XS_INLINE T bitXor(const T& param1, const T2& param2) noexcept
{
    static_assert(isSame<T, T2> || (sizeof(T) == sizeof(T2)));
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

template<typename T>
XS_INLINE T bitNot(const T& param) noexcept
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
