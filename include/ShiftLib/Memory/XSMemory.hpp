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

#include "SIMD/XSSIMDTraits.hpp"
#include "SIMD/XSSIMDx86.hpp"
#include "XSBit.hpp"

namespace Shift {
constexpr uint0 systemAlignment = maxAlignment<uint0, 128>;

namespace NoExport {
#if (XS_COMPILER == XS_MSVC) || (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC) || (XS_COMPILER == XS_CLANGWIN)
#    include <intrin.h> //required for _movsx intrinsics
#    if (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
#        define XS_REPMOVBAKSET __asm__("std")
#        define XS_REPMOVBAKCLEAR __asm__("cld")
#    elif XS_ARCH == XS_ARCH32
#        define XS_REPMOVBAKSET __asm { std}
#        define XS_REPMOVBAKCLEAR __asm { cld}
#    else
// inline asm not supported in 64bit
#        define XS_REPMOVBAKSET                     \
            const uint0 flagsBack = __readeflags(); \
            __writeeflags(flagsBack | 0x400)
#        define XS_REPMOVBAKCLEAR __writeeflags(flagsBack & ~0x400)
#    endif
#    define XS_REPMOVSB(dst, src, count) __movsb((dst), (src), (count)) // move count number of bytes
#    define XS_REPMOVSW(dst, src, count) \
        __movsw((uint16_t*)(dst), (uint16_t*)(src), (count)) // move count number of words (2xbytes)
#    define XS_REPMOVSD(dst, src, count) \
        __movsd((unsigned long*)(dst), (unsigned long*)(src), (count)) // move count number of double words (4xbytes)
#    define XS_REPMOVSQ(dst, src, count) \
        __movsq((uint64_t*)(dst), (uint64_t*)(src), (count)) // move count number of quad words (8xbytes)
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG)
#    define XS_REPMOVBAKSET __asm__("std")
#    define XS_REPMOVBAKCLEAR __asm__("cld")
#    define XS_REPMOVSB(dst, src, count) \
        asm("rep movsb" : "=D"(dst), "=S"(src), "=c"(count) : "0"(dst), "1"(src), "2"(count) : "memory");
#    define XS_REPMOVSW(dst, src, count) \
        asm("rep movsw" : "=D"(dst), "=S"(src), "=c"(count) : "0"(dst), "1"(src), "2"(count) : "memory");
#    define XS_REPMOVSD(dst, src, count) \
        asm("rep movsd" : "=D"(dst), "=S"(src), "=c"(count) : "0"(dst), "1"(src), "2"(count) : "memory");
#    define XS_REPMOVSQ(dst, src, count) \
        asm("rep movsq" : "=D"(dst), "=S"(src), "=c"(count) : "0"(dst), "1"(src), "2"(count) : "memory");
#endif

template<typename T, typename... Ts>
struct BulkData
    : BulkData<T>
    , BulkData<Ts...>
{};

template<typename... Ts>
struct BulkData<void, Ts...> : BulkData<Ts...>
{};

template<typename T, typename... Ts>
struct BulkData<T, void, Ts...> : BulkData<T, Ts...>
{};

template<typename T>
struct BulkData<T, void> : BulkData<T>
{};

template<typename T>
struct BulkData<T>
{
    T data;
};

template<typename T, uint0 Size = 1>
struct BulkHelper
{
    using Type = T[Size]; // NOLINT(modernize-avoid-c-arrays)
    static constexpr uint32 data512Size =
        (hasISAFeature<ISAFeature::AVX512F> && (alignof(T) >= 64)) ? sizeof(T) / 64 : 0;
    static constexpr uint32 data256Size =
        (hasISAFeature<ISAFeature::AVX2> && (alignof(T) >= 32)) ? (sizeof(T) - (data512Size * 64)) / 32 : 0;
    static constexpr uint32 data128Size = (hasISAFeature<ISAFeature::SSE2> && (alignof(T) >= 16)) ?
        (sizeof(T) - (data512Size * 64) - (data256Size * 32)) / 16 :
        0;
    static constexpr uint32 data64Size = ((currentArch == Architecture::Bit64) && (alignof(T) >= 8)) ?
        (sizeof(T) - (data512Size * 64) - (data256Size * 32) - (data128Size * 16)) / 8 :
        0;
    static constexpr uint32 data32Size =
        (sizeof(T) - (data512Size * 64) - (data256Size * 32) - (data128Size * 16) - (data64Size * 8)) / 4;
    static constexpr uint32 data16Size = (sizeof(T) - (data512Size * 64) - (data256Size * 32) - (data128Size * 16) -
                                             (data64Size * 8) - (data32Size * 4)) /
        2;
    static constexpr uint32 data8Size = sizeof(T) - (data512Size * 64) - (data256Size * 32) - (data128Size * 16) -
        (data64Size * 8) - (data32Size * 4) - (data16Size * 2);
};

template<typename T>
struct BulkHelper<T, 0>
{
    using Type = void;
};

template<typename T>
struct BulkBlock
    : BulkData<typename BulkHelper<__m512i, BulkHelper<T>::data512Size>::Type,
          typename BulkHelper<__m256i, BulkHelper<T>::data256Size>::Type,
          typename BulkHelper<__m128i, BulkHelper<T>::data128Size>::Type,
          typename BulkHelper<uint64, BulkHelper<T>::data64Size>::Type,
          typename BulkHelper<uint32, BulkHelper<T>::data32Size>::Type,
          typename BulkHelper<uint16, BulkHelper<T>::data16Size>::Type,
          typename BulkHelper<uint8, BulkHelper<T>::data8Size>::Type>
{};

XS_CONSTEVAL uint0 log2(const uint0 n)
{
    // TODO: Can be replaced by ctz when it is possible to make it constexpr
    return (n > 1) ? 1 + log2(n >> 1) : 0;
}
} // namespace NoExport

/**
 * Mark a pointer as being correctly aligned to the specified alignment.
 * @tparam T     Generic type parameter.
 * @tparam Align Alignment of the pointer.
 * @param  pointer The input pointer.
 * @returns The pointer with appropriate alignment flagged to the compiler.
 */
template<typename T, uint0 Align>
[[nodiscard]] XS_INLINE constexpr T* markAligned(T* const pointer) noexcept // NOLINT(clang-diagnostic-unused-parameter)
{
#if __cplusplus >= 202002L
    if (__builtin_is_constant_evaluated()) {
        return pointer;
    } else {
        return static_cast<T*>(__builtin_assume_aligned(pointer, Align));
    }
#elif (XS_COMPILER == XS_GNUC) || (XS_COMPILER == XS_CLANG) || (XS_COMPILER == XS_CLANGWIN)
    return static_cast<T*>(__builtin_assume_aligned(pointer, Align));
#elif (XS_COMPILER == XS_MSVC)
    return pointer;
#elif (XS_COMPILER == XS_ICL) || (XS_COMPILER == XS_ICC)
    __assume_aligned(pointer, Align);
    return pointer;
#endif
}

/**
 * Copy data from one location to another based on memory allocator used.
 * @note This will do just a straight copy of data from one place to another. This uses the type of
 * allocator used to create the memory to copy it. This allows for easy support of copying data between different
 * devices and memory ranges. It is a requirement that all memory locations used for the copy be aligned to at least
 * the default alignment of the type T.
 * @tparam T           Type of objects being copied.
 * @tparam AllocSource Type of the allocator used to create the source memory.
 * @tparam AllocDest   Type of the allocator used to create the destination memory.
 * @param  dest   The destination address to start moving to.
 * @param  source The source address to start moving from.
 * @param  size   The number of bytes to copy.
 */
template<typename T /*, typename AllocSource, typename AllocDest*/>
XS_INLINE void memMove(T* XS_RESTRICT dest, const T* XS_RESTRICT source, uint0 size)
{
    XS_ASSERT((reinterpret_cast<uint0>(dest) % alignof(T)) == 0);
    XS_ASSERT((reinterpret_cast<uint0>(source) % alignof(T)) == 0);
    XS_ASSERT(size % sizeof(T) == 0);
    XS_ASSERT(dest < source || dest >= (source + (size / sizeof(T))));
    // Note: sizeof(T) % alignof(T) == 0
    if constexpr (currentISA == ISA::X86) {
        auto dest2 = reinterpret_cast<uint8*>(dest);
        auto source2 = reinterpret_cast<const uint8*>(source);

        if constexpr (hasISAFeature<ISAFeature::SSE2>) {
            if (constexpr auto repMovSize = 8000 * systemAlignment; size > repMovSize) {
                // Once the movement size reaches a certain limit then 'rep movs' becomes faster
                if constexpr (currentArch == Architecture::Bit64 && alignof(T) % 8 == 0) {
                    XS_REPMOVSQ(dest2, source2, size >> 3);
                } else if constexpr (alignof(T) == 4) {
                    XS_REPMOVSD(dest2, source2, size >> 2);
                } else if constexpr (alignof(T) == 2) {
                    XS_REPMOVSW(dest2, source2, size >> 1);
                } else /*alignof(T) == 1*/ {
                    XS_REPMOVSB(dest2, source2, size);
                }
            } else {
                if constexpr (alignof(T) % systemAlignment == 0) {
                    if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                        if constexpr (sizeof(T) >= 512) {
                            for (auto count = size >> 9; count != 0; --count) {
                                const __m512i src = _mm512_load_si512(source2);
                                const __m512i src2 = _mm512_load_si512(source2 + 64);
                                const __m512i src3 = _mm512_load_si512(source2 + 128);
                                const __m512i src4 = _mm512_load_si512(source2 + 192);
                                const __m512i src5 = _mm512_load_si512(source2 + 256);
                                const __m512i src6 = _mm512_load_si512(source2 + 320);
                                const __m512i src7 = _mm512_load_si512(source2 + 384);
                                const __m512i src8 = _mm512_load_si512(source2 + 448);
                                _mm512_store_si512(dest2, src);
                                _mm512_store_si512(dest2 + 64, src2);
                                _mm512_store_si512(dest2 + 128, src3);
                                _mm512_store_si512(dest2 + 192, src4);
                                _mm512_store_si512(dest2 + 256, src5);
                                _mm512_store_si512(dest2 + 320, src6);
                                _mm512_store_si512(dest2 + 384, src7);
                                _mm512_store_si512(dest2 + 448, src8);
                                source2 += 512;
                                dest2 += 512;
                            }
                        } else {
                            const uint0 maxCount = size >> 6;
                            const uint0 loopCount = maxCount >> 3;
                            for (auto count = loopCount; count != 0; --count) {
                                const __m512i src = _mm512_load_si512(source2);
                                const __m512i src2 = _mm512_load_si512(source2 + 64);
                                const __m512i src3 = _mm512_load_si512(source2 + 128);
                                const __m512i src4 = _mm512_load_si512(source2 + 192);
                                const __m512i src5 = _mm512_load_si512(source2 + 256);
                                const __m512i src6 = _mm512_load_si512(source2 + 320);
                                const __m512i src7 = _mm512_load_si512(source2 + 384);
                                const __m512i src8 = _mm512_load_si512(source2 + 448);
                                _mm512_store_si512(dest2, src);
                                _mm512_store_si512(dest2 + 64, src2);
                                _mm512_store_si512(dest2 + 128, src3);
                                _mm512_store_si512(dest2 + 192, src4);
                                _mm512_store_si512(dest2 + 256, src5);
                                _mm512_store_si512(dest2 + 320, src6);
                                _mm512_store_si512(dest2 + 384, src7);
                                _mm512_store_si512(dest2 + 448, src8);
                                source2 += 512;
                                dest2 += 512;
                            }
                            for (auto count = maxCount - (loopCount << 3); count != 0; --count) {
                                _mm512_store_si512(dest2, _mm512_load_si512(source2));
                                source2 += 64;
                                dest2 += 64;
                            }
                        }
                    } else if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                        if constexpr (sizeof(T) >= 256) {
                            for (auto count = size >> 8; count != 0; --count) {
                                const __m256i src = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2));
                                const __m256i src2 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 32));
                                const __m256i src3 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 64));
                                const __m256i src4 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 96));
                                const __m256i src5 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 128));
                                const __m256i src6 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 160));
                                const __m256i src7 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 192));
                                const __m256i src8 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 224));
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2), src);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 32), src2);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 64), src3);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 96), src4);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 128), src5);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 160), src6);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 192), src7);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 224), src8);
                                source2 += 256;
                                dest2 += 256;
                            }
                        } else {
                            const uint0 maxCount = size >> 5;
                            const uint0 loopCount = maxCount >> 3;
                            for (auto count = loopCount; count != 0; --count) {
                                const __m256i src = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2));
                                const __m256i src2 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 32));
                                const __m256i src3 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 64));
                                const __m256i src4 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 96));
                                const __m256i src5 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 128));
                                const __m256i src6 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 160));
                                const __m256i src7 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 192));
                                const __m256i src8 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 224));
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2), src);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 32), src2);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 64), src3);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 96), src4);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 128), src5);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 160), src6);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 192), src7);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 224), src8);
                                source2 += 256;
                                dest2 += 256;
                            }
                            for (auto count = maxCount - (loopCount << 3); count != 0; --count) {
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2),
                                    _mm256_load_si256(reinterpret_cast<const __m256i*>(source2)));
                                source2 += 32;
                                dest2 += 32;
                            }
                        }
                    } else /*hasISAFeature<ISAFeature::SSE2>*/ {
                        if constexpr (sizeof(T) >= 128) {
                            for (auto count = size >> 7; count != 0; --count) {
                                const __m128i src = _mm_load_si128(reinterpret_cast<const __m128i*>(source2));
                                const __m128i src2 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 16));
                                const __m128i src3 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 32));
                                const __m128i src4 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 48));
                                const __m128i src5 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 64));
                                const __m128i src6 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 80));
                                const __m128i src7 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 96));
                                const __m128i src8 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 112));
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2), src);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 16), src2);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 32), src3);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 48), src4);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 64), src5);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 80), src6);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 96), src7);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 112), src8);
                                source2 += 128;
                                dest2 += 128;
                            }
                        } else {
                            const uint0 maxCount = size >> 4;
                            const uint0 loopCount = maxCount >> 3;
                            for (auto count = loopCount; count != 0; --count) {
                                const __m128i src = _mm_load_si128(reinterpret_cast<const __m128i*>(source2));
                                const __m128i src2 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 16));
                                const __m128i src3 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 32));
                                const __m128i src4 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 48));
                                const __m128i src5 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 64));
                                const __m128i src6 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 80));
                                const __m128i src7 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 96));
                                const __m128i src8 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 112));
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2), src);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 16), src2);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 32), src3);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 48), src4);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 64), src5);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 80), src6);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 96), src7);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 112), src8);
                                source2 += 128;
                                dest2 += 128;
                            }
                            for (auto count = maxCount - (loopCount << 3); count != 0; --count) {
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2),
                                    _mm_load_si128(reinterpret_cast<const __m128i*>(source2)));
                                source2 += 16;
                                dest2 += 16;
                            }
                        }
                    }
                } else {
                    if (constexpr uint32 copyThreshold = (alignof(T) == 1) ?
                            systemAlignment * 4 :
                            ((alignof(T) == 2) ? systemAlignment * 2 : systemAlignment);
                        XS_UNEXPECT(size < copyThreshold)) {
                        if (XS_EXPECT(size >= (alignof(T) * 32))) {
                            // rep movs is only generally faster than a normal loop when count>=32
                            if constexpr (alignof(T) == 4) {
                                XS_REPMOVSD(dest2, source2, size >> 2);
                            } else if constexpr (alignof(T) == 2) {
                                XS_REPMOVSW(dest2, source2, size >> 1);
                            } else /*alignof(T) == 1*/ {
                                XS_REPMOVSB(dest2, source2, size);
                            }
                        } else {
                            constexpr auto logSize = NoExport::log2(sizeof(T));
                            const auto loops = size >> logSize;
                            for (auto count = loops; count != 0; --count) {
                                *dest = *source;
                                ++source;
                                ++dest;
                            }
                        }
                        return;
                    }

                    // Get the difference between the current pointer alignment and the desired alignment
                    constexpr auto alignMinus1 = systemAlignment - 1_ui0;
                    uint0 outputAlignment = ((reinterpret_cast<uint0>(dest2) + alignMinus1) & ~alignMinus1) -
                        reinterpret_cast<uint0>(dest2);
                    uint0 inputAlignment = ((reinterpret_cast<uint0>(source2) + alignMinus1) & ~alignMinus1) -
                        reinterpret_cast<uint0>(source2);

                    inputAlignment += systemAlignment;
                    // Copy memory up until the required alignment for a bulk copy.
                    uint0 outputAlignment2;
                    if constexpr (alignof(T) <= 8) {
                        // Check if we need 2/4/6/8/10/12/14Byte or 4/8/12Byte or 8ByteCopy
                        if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                            outputAlignment2 = outputAlignment & 0x0F_ui0;
                        } else {
                            outputAlignment2 = outputAlignment;
                        }
                        if (XS_EXPECT(outputAlignment2)) {
                            if constexpr (alignof(T) == 1) {
                                for (auto count = outputAlignment2; count != 0; --count) {
                                    *dest2 = *source2;
                                    ++dest2;
                                    ++source2;
                                }
                            } else if constexpr (alignof(T) == 2) {
                                for (auto count = outputAlignment2 >> 1; count != 0; --count) {
                                    *reinterpret_cast<uint16*>(dest2) = *reinterpret_cast<const uint16*>(source2);
                                    dest2 += 2;
                                    source2 += 2;
                                }
                            } else if constexpr (alignof(T) == 4) {
                                for (auto count = outputAlignment2 >> 2; count != 0; --count) {
                                    *reinterpret_cast<uint32*>(dest2) = *reinterpret_cast<const uint32*>(source2);
                                    dest2 += 4;
                                    source2 += 4;
                                }
                            } else /*alignof(T) == 8*/ {
                                *reinterpret_cast<uint64*>(dest2) = *reinterpret_cast<const uint64*>(source2);
                                dest2 += 8;
                                source2 += 8;
                            }
                            inputAlignment -= outputAlignment2;
                        }
                    }
                    if constexpr (alignof(T) <= 16 && hasISAFeature<ISAFeature::AVX2>) {
                        // Check if we need 16ByteCopy
                        if constexpr (hasISAFeature<ISAFeature::AVX512F> || alignof(T) < 16) {
                            outputAlignment2 = outputAlignment & 0x10_ui0;
                        } else {
                            outputAlignment2 = outputAlignment;
                        }
                        if (outputAlignment2) {
                            // Check if source is aligned
                            __m128i src;
                            if constexpr (alignof(T) < 16) {
                                if (inputAlignment & 0x0F_ui0) {
                                    src = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2));
                                } else {
                                    src = _mm_load_si128(reinterpret_cast<const __m128i*>(source2));
                                }
                            } else {
                                src = _mm_load_si128(reinterpret_cast<const __m128i*>(source2));
                            }
                            // Dest is always aligned due to previous operations
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2), src);
                            // Update the source and destination pointers
                            dest2 += 16;
                            source2 += 16;
                            inputAlignment -= 16;
                        }
                    }
                    if constexpr (alignof(T) <= 32 && hasISAFeature<ISAFeature::AVX512F>) {
                        // Check if we need 32ByteCopy
                        if constexpr (alignof(T) < 32) {
                            outputAlignment2 = outputAlignment & 0x20_ui0;
                        } else {
                            outputAlignment2 = outputAlignment;
                        }
                        if (outputAlignment2) {
                            // Check if source is aligned
                            __m256i src;
                            if constexpr (alignof(T) < 32) {
                                if (inputAlignment & 0x1F_ui0) {
                                    src = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2));
                                } else {
                                    src = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2));
                                }
                            } else {
                                src = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2));
                            }
                            // Dest is always aligned due to previous operations
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2), src);
                            // Update the source and destination pointers
                            dest2 += 32;
                            source2 += 32;
                            inputAlignment -= 32;
                        }
                    }
                    size -= outputAlignment;

                    // Check if we have correctly aligned pointers on both the source and the destination
                    if (XS_UNEXPECT((inputAlignment & alignMinus1) == 0)) {
                        // Copy using aligned operations
                        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                            const uint0 maxCount = size >> 6;
                            const uint0 loopCount = maxCount >> 3;
                            for (auto count = loopCount; count != 0; --count) {
                                const __m512i src = _mm512_load_si512(source2);
                                const __m512i src2 = _mm512_load_si512(source2 + 64);
                                const __m512i src3 = _mm512_load_si512(source2 + 128);
                                const __m512i src4 = _mm512_load_si512(source2 + 192);
                                const __m512i src5 = _mm512_load_si512(source2 + 256);
                                const __m512i src6 = _mm512_load_si512(source2 + 320);
                                const __m512i src7 = _mm512_load_si512(source2 + 384);
                                const __m512i src8 = _mm512_load_si512(source2 + 448);
                                _mm512_store_si512(dest2, src);
                                _mm512_store_si512(dest2 + 64, src2);
                                _mm512_store_si512(dest2 + 128, src3);
                                _mm512_store_si512(dest2 + 192, src4);
                                _mm512_store_si512(dest2 + 256, src5);
                                _mm512_store_si512(dest2 + 320, src6);
                                _mm512_store_si512(dest2 + 384, src7);
                                _mm512_store_si512(dest2 + 448, src8);
                                source2 += 512;
                                dest2 += 512;
                            }
                            for (auto count = maxCount - (loopCount << 3); count != 0; --count) {
                                _mm512_store_si512(dest2, _mm512_load_si512(source2));
                                source2 += 64;
                                dest2 += 64;
                            }
                            outputAlignment = size - (maxCount << 6);
                        } else if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                            const uint0 maxCount = size >> 5;
                            const uint0 loopCount = maxCount >> 3;
                            for (auto count = loopCount; count != 0; --count) {
                                const __m256i src = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2));
                                const __m256i src2 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 32));
                                const __m256i src3 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 64));
                                const __m256i src4 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 96));
                                const __m256i src5 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 128));
                                const __m256i src6 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 160));
                                const __m256i src7 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 192));
                                const __m256i src8 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 224));
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2), src);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 32), src2);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 64), src3);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 96), src4);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 128), src5);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 160), src6);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 192), src7);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 224), src8);
                                source2 += 256;
                                dest2 += 256;
                            }
                            for (auto count = maxCount - (loopCount << 3); count != 0; --count) {
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2),
                                    _mm256_load_si256(reinterpret_cast<const __m256i*>(source2)));
                                source2 += 32;
                                dest2 += 32;
                            }
                            outputAlignment = size - (maxCount << 5);
                        } else {
                            const uint0 maxCount = size >> 4;
                            const uint0 loopCount = maxCount >> 3;
                            for (auto count = loopCount; count != 0; --count) {
                                const __m128i src = _mm_load_si128(reinterpret_cast<const __m128i*>(source2));
                                const __m128i src2 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 16));
                                const __m128i src3 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 32));
                                const __m128i src4 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 48));
                                const __m128i src5 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 64));
                                const __m128i src6 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 80));
                                const __m128i src7 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 96));
                                const __m128i src8 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 112));
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2), src);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 16), src2);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 32), src3);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 48), src4);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 64), src5);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 80), src6);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 96), src7);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 112), src8);
                                source2 += 128;
                                dest2 += 128;
                            }
                            for (auto count = maxCount - (loopCount << 3); count != 0; --count) {
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2),
                                    _mm_load_si128(reinterpret_cast<const __m128i*>(source2)));
                                source2 += 16;
                                dest2 += 16;
                            }
                            outputAlignment = size - (maxCount << 4);
                        }
                    } else {
                        // Copy using unaligned operations (destination will be aligned due to previous operations)
                        if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                            const uint0 maxCount = size >> 6;
                            const uint0 loopCount = maxCount >> 3;
                            for (auto count = loopCount; count != 0; --count) {
                                const __m512i src = _mm512_loadu_si512(source2);
                                const __m512i src2 = _mm512_loadu_si512(source2 + 64);
                                const __m512i src3 = _mm512_loadu_si512(source2 + 128);
                                const __m512i src4 = _mm512_loadu_si512(source2 + 192);
                                const __m512i src5 = _mm512_loadu_si512(source2 + 256);
                                const __m512i src6 = _mm512_loadu_si512(source2 + 320);
                                const __m512i src7 = _mm512_loadu_si512(source2 + 384);
                                const __m512i src8 = _mm512_loadu_si512(source2 + 448);
                                _mm512_store_si512(dest2, src);
                                _mm512_store_si512(dest2 + 64, src2);
                                _mm512_store_si512(dest2 + 128, src3);
                                _mm512_store_si512(dest2 + 192, src4);
                                _mm512_store_si512(dest2 + 256, src5);
                                _mm512_store_si512(dest2 + 320, src6);
                                _mm512_store_si512(dest2 + 384, src7);
                                _mm512_store_si512(dest2 + 448, src8);
                                source2 += 512;
                                dest2 += 512;
                            }
                            for (auto count = maxCount - (loopCount << 3); count != 0; --count) {
                                _mm512_store_si512(dest2, _mm512_loadu_si512(source2));
                                source2 += 64;
                                dest2 += 64;
                            }
                            outputAlignment = size - (maxCount << 6);
                        } else if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                            const uint0 maxCount = size >> 5;
                            const uint0 loopCount = maxCount >> 3;
                            for (auto count = loopCount; count != 0; --count) {
                                const __m256i src = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2));
                                const __m256i src2 = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2 + 32));
                                const __m256i src3 = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2 + 64));
                                const __m256i src4 = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2 + 96));
                                const __m256i src5 =
                                    _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2 + 128));
                                const __m256i src6 =
                                    _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2 + 160));
                                const __m256i src7 =
                                    _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2 + 192));
                                const __m256i src8 =
                                    _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2 + 224));
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2), src);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 32), src2);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 64), src3);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 96), src4);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 128), src5);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 160), src6);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 192), src7);
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 224), src8);
                                source2 += 256;
                                dest2 += 256;
                            }
                            for (auto count = maxCount - (loopCount << 3); count != 0; --count) {
                                _mm256_store_si256(reinterpret_cast<__m256i*>(dest2),
                                    _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2)));
                                source2 += 32;
                                dest2 += 32;
                            }
                            outputAlignment = size - (maxCount << 5);
                        } else {
                            const uint0 maxCount = size >> 4;
                            const uint0 loopCount = maxCount >> 3;
                            for (auto count = loopCount; count != 0; --count) {
                                const __m128i src = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2));
                                const __m128i src2 = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2 + 16));
                                const __m128i src3 = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2 + 32));
                                const __m128i src4 = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2 + 48));
                                const __m128i src5 = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2 + 64));
                                const __m128i src6 = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2 + 80));
                                const __m128i src7 = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2 + 96));
                                const __m128i src8 = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2 + 112));
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2), src);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 16), src2);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 32), src3);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 48), src4);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 64), src5);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 80), src6);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 96), src7);
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 112), src8);
                                source2 += 128;
                                dest2 += 128;
                            }
                            for (auto count = maxCount - (loopCount << 3); count != 0; --count) {
                                _mm_store_si128(reinterpret_cast<__m128i*>(dest2),
                                    _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2)));
                                source2 += 16;
                                dest2 += 16;
                            }
                            outputAlignment = size - (maxCount << 4);
                        }
                    }
                    // Check if there is any trailing element that needs to be copied
                    if constexpr (alignof(T) <= 32 && hasISAFeature<ISAFeature::AVX512F>) {
                        // Check if we need 32Byte copy
                        if constexpr (alignof(T) < 32) {
                            outputAlignment2 = outputAlignment & 0x20_ui0;
                            outputAlignment &= 0x1F_ui0;
                        } else {
                            outputAlignment2 = outputAlignment;
                        }
                        if (outputAlignment2) {
                            __m256i src;
                            if constexpr (alignof(T) < 32) {
                                if (inputAlignment & 0x1F_ui0) {
                                    src = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2));
                                } else {
                                    src = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2));
                                }
                            } else {
                                src = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2));
                            }
                            // Dest is always aligned due to previous operations
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2), src);
                            if constexpr (alignof(T) <= 16) {
                                // Update the source and destination pointers
                                dest2 += 32;
                                source2 += 32;
                                inputAlignment -= 32;
                            }
                        }
                    }
                    if constexpr (alignof(T) <= 16 && hasISAFeature<ISAFeature::AVX2>) {
                        // Check if we need 16Byte copy
                        if constexpr (hasISAFeature<ISAFeature::AVX512F> || alignof(T) < 16) {
                            outputAlignment2 = outputAlignment & 0x10_ui0;
                            outputAlignment &= 0x0F_ui0;
                        } else {
                            outputAlignment2 = outputAlignment;
                        }
                        if (outputAlignment2) {
                            __m128i src;
                            if constexpr (alignof(T) < 16) {
                                if (inputAlignment & 0x0F_ui0) {
                                    src = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2));
                                } else {
                                    src = _mm_load_si128(reinterpret_cast<const __m128i*>(source2));
                                }
                            } else {
                                src = _mm_load_si128(reinterpret_cast<const __m128i*>(source2));
                            }
                            // Dest is always aligned due to previous operations
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2), src);
                            if constexpr (alignof(T) <= 8) {
                                // Update the source and destination pointers
                                dest2 += 16;
                                source2 += 16;
                            }
                        }
                    }
                    if constexpr (alignof(T) == 8) {
                        if (outputAlignment) {
                            *reinterpret_cast<uint64*>(dest2) = *reinterpret_cast<const uint64*>(source2);
                        }
                    } else if constexpr (alignof(T) == 4) {
                        if (XS_EXPECT(outputAlignment)) {
                            const auto loops = outputAlignment >> 2;
                            for (auto count = loops - 1; count != 0; --count) {
                                *reinterpret_cast<uint32*>(dest2) = *reinterpret_cast<const uint32*>(source2);
                                dest2 += 4;
                                source2 += 4;
                            }
                            *reinterpret_cast<uint32*>(dest2) = *reinterpret_cast<const uint32*>(source2);
                        }
                    } else if constexpr (alignof(T) == 2) {
                        if (XS_EXPECT(outputAlignment)) {
                            const auto loops = outputAlignment >> 1;
                            for (auto count = loops - 1; count != 0; --count) {
                                *reinterpret_cast<uint16*>(dest2) = *reinterpret_cast<const uint16*>(source2);
                                dest2 += 2;
                                source2 += 2;
                            }
                            *reinterpret_cast<uint16*>(dest2) = *reinterpret_cast<const uint16*>(source2);
                        }
                    } else if constexpr (alignof(T) == 1) {
                        if (XS_EXPECT(outputAlignment)) {
                            const auto loops = outputAlignment;
                            for (auto count = loops - 1; count != 0; --count) {
                                *dest2 = *source2;
                                ++dest2;
                                ++source2;
                            }
                            *dest2 = *source2;
                        }
                    }
                }
            }
        } else {
            if constexpr (currentArch == Architecture::Bit64 && alignof(T) % 8 == 0) {
                XS_REPMOVSQ(dest2, source2, size >> 3);
            } else if constexpr (alignof(T) == 4) {
                XS_REPMOVSD(dest2, source2, size >> 2);
            } else if constexpr (alignof(T) == 2) {
                XS_REPMOVSW(dest2, source2, size >> 1);
            } else /*alignof(T) == 1*/ {
                XS_REPMOVSB(dest2, source2, size);
            }
        }
    } else {
        constexpr auto logSize = NoExport::log2(sizeof(T));
        const auto loops = size >> logSize;
        for (auto count = loops; count != 0; --count) {
            *dest = *source;
            ++source;
            ++dest;
        }
    }
}

/**
 * Move data from one location to another by traversing backwards (i.e. from end address to start) based on memory
 * allocator used.
 * @note This will do just a straight move of data from one place to another. This should be used to move
 * data where the source and destination exist within the same range (prevents stomping) and the destination address is
 * greater than the source. This uses the type of allocator used to create the memory to copy it. This allows for easy
 * support of copying data between different devices and memory ranges. It is a requirement that all memory locations
 * used for the copy be aligned to at least the default alignment of the type T.
 * @tparam T           Type of objects being copied.
 * @tparam AllocSource Type of the allocator used to create the source memory.
 * @tparam AllocDest   Type of the allocator used to create the destination memory.
 * @param  dest   The destination address to start moving to.
 * @param  source The source address to start moving from.
 * @param  size   The number of bytes to copy.
 */
template<typename T /*, typename AllocSource, typename AllocDest*/>
XS_INLINE void memMoveBackwards(T* const XS_RESTRICT dest, const T* XS_RESTRICT source, uint0 size)
{
    XS_ASSERT((reinterpret_cast<uint0>(dest) % alignof(T)) == 0);
    XS_ASSERT((reinterpret_cast<uint0>(source) % alignof(T)) == 0);
    XS_ASSERT(size % sizeof(T) == 0);
    XS_ASSERT(dest > source || source >= (dest + (size / sizeof(T))));
    if constexpr (currentISA == ISA::X86) {
        // Determine the end pointers for backwards traversal
        auto dest2 = reinterpret_cast<uint8*>(dest);
        auto source2 = reinterpret_cast<const uint8*>(source);
        dest2 += size;
        source2 += size;

        if constexpr (hasISAFeature<ISAFeature::SSE2>) {
            if constexpr (alignof(T) % systemAlignment == 0) {
                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    if constexpr (sizeof(T) >= 512) {
                        for (auto count = size >> 9; count != 0; --count) {
                            source2 -= 512;
                            dest2 -= 512;
                            const __m512i src = _mm512_load_si512(source2);
                            const __m512i src2 = _mm512_load_si512(source2 + 64);
                            const __m512i src3 = _mm512_load_si512(source2 + 128);
                            const __m512i src4 = _mm512_load_si512(source2 + 192);
                            const __m512i src5 = _mm512_load_si512(source2 + 256);
                            const __m512i src6 = _mm512_load_si512(source2 + 320);
                            const __m512i src7 = _mm512_load_si512(source2 + 384);
                            const __m512i src8 = _mm512_load_si512(source2 + 448);
                            _mm512_store_si512(dest2, src);
                            _mm512_store_si512(dest2 + 64, src2);
                            _mm512_store_si512(dest2 + 128, src3);
                            _mm512_store_si512(dest2 + 192, src4);
                            _mm512_store_si512(dest2 + 256, src5);
                            _mm512_store_si512(dest2 + 320, src6);
                            _mm512_store_si512(dest2 + 384, src7);
                            _mm512_store_si512(dest2 + 448, src8);
                        }
                    } else {
                        const uint0 maxCount = size >> 6;
                        const uint0 loopCount = maxCount >> 3;
                        for (auto count = loopCount; count != 0; --count) {
                            source2 -= 512;
                            dest2 -= 512;
                            const __m512i src = _mm512_load_si512(source2);
                            const __m512i src2 = _mm512_load_si512(source2 + 64);
                            const __m512i src3 = _mm512_load_si512(source2 + 128);
                            const __m512i src4 = _mm512_load_si512(source2 + 192);
                            const __m512i src5 = _mm512_load_si512(source2 + 256);
                            const __m512i src6 = _mm512_load_si512(source2 + 320);
                            const __m512i src7 = _mm512_load_si512(source2 + 384);
                            const __m512i src8 = _mm512_load_si512(source2 + 448);
                            _mm512_store_si512(dest2, src);
                            _mm512_store_si512(dest2 + 64, src2);
                            _mm512_store_si512(dest2 + 128, src3);
                            _mm512_store_si512(dest2 + 192, src4);
                            _mm512_store_si512(dest2 + 256, src5);
                            _mm512_store_si512(dest2 + 320, src6);
                            _mm512_store_si512(dest2 + 384, src7);
                            _mm512_store_si512(dest2 + 448, src8);
                        }
                        for (auto count = maxCount - (loopCount << 3); count != 0; --count) {
                            source2 -= 64;
                            dest2 -= 64;
                            _mm512_store_si512(dest2, _mm512_load_si512(source2));
                        }
                    }
                } else if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                    if constexpr (sizeof(T) >= 256) {
                        for (auto count = size >> 8; count != 0; --count) {
                            source2 -= 256;
                            dest2 -= 256;
                            const __m256i src = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2));
                            const __m256i src2 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 32));
                            const __m256i src3 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 64));
                            const __m256i src4 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 96));
                            const __m256i src5 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 128));
                            const __m256i src6 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 160));
                            const __m256i src7 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 192));
                            const __m256i src8 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 224));
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2), src);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 32), src2);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 64), src3);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 96), src4);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 128), src5);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 160), src6);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 192), src7);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 224), src8);
                        }
                    } else {
                        const uint0 maxCount = size >> 5;
                        const uint0 loopCount = maxCount >> 3;
                        for (auto count = loopCount; count != 0; --count) {
                            source2 -= 256;
                            dest2 -= 256;
                            const __m256i src = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2));
                            const __m256i src2 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 32));
                            const __m256i src3 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 64));
                            const __m256i src4 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 96));
                            const __m256i src5 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 128));
                            const __m256i src6 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 160));
                            const __m256i src7 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 192));
                            const __m256i src8 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 224));
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2), src);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 32), src2);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 64), src3);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 96), src4);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 128), src5);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 160), src6);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 192), src7);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 224), src8);
                        }
                        for (auto count = maxCount - (loopCount << 3); count != 0; --count) {
                            source2 -= 32;
                            dest2 -= 32;
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2),
                                _mm256_load_si256(reinterpret_cast<const __m256i*>(source2)));
                        }
                    }
                } else /*hasISAFeature<ISAFeature::SSE2>*/ {
                    if constexpr (sizeof(T) >= 128) {
                        for (auto count = size >> 7; count != 0; --count) {
                            source2 -= 128;
                            dest2 -= 128;
                            const __m128i src = _mm_load_si128(reinterpret_cast<const __m128i*>(source2));
                            const __m128i src2 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 16));
                            const __m128i src3 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 32));
                            const __m128i src4 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 48));
                            const __m128i src5 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 64));
                            const __m128i src6 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 80));
                            const __m128i src7 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 96));
                            const __m128i src8 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 112));
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2), src);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 16), src2);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 32), src3);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 48), src4);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 64), src5);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 80), src6);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 96), src7);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 112), src8);
                        }
                    } else {
                        const uint0 maxCount = size >> 4;
                        const uint0 loopCount = maxCount >> 3;
                        for (auto count = loopCount; count != 0; --count) {
                            source2 -= 128;
                            dest2 -= 128;
                            const __m128i src = _mm_load_si128(reinterpret_cast<const __m128i*>(source2));
                            const __m128i src2 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 16));
                            const __m128i src3 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 32));
                            const __m128i src4 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 48));
                            const __m128i src5 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 64));
                            const __m128i src6 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 80));
                            const __m128i src7 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 96));
                            const __m128i src8 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 112));
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2), src);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 16), src2);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 32), src3);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 48), src4);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 64), src5);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 80), src6);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 96), src7);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 112), src8);
                        }
                        for (auto count = maxCount - (loopCount << 3); count != 0; --count) {
                            source2 -= 16;
                            dest2 -= 16;
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2),
                                _mm_load_si128(reinterpret_cast<const __m128i*>(source2)));
                        }
                    }
                }
            } else {
                if (constexpr uint32 copyThreshold = systemAlignment; XS_UNEXPECT(size < copyThreshold)) {
                    constexpr auto logSize = NoExport::log2(sizeof(T));
                    const auto loops = size >> logSize;
                    for (auto count = loops; count != 0; --count) {
                        source2 -= sizeof(T);
                        dest2 -= sizeof(T);
                        *reinterpret_cast<T*>(dest2) = *reinterpret_cast<const T*>(source2);
                    }
                    return;
                }

                // Get the difference between the current pointer alignment and the desired alignment
                constexpr auto alignMinus1 = systemAlignment - 1_ui0;
                uint0 outputAlignment =
                    reinterpret_cast<uint0>(dest2) - (reinterpret_cast<uint0>(dest2) & ~alignMinus1);
                uint0 inputAlignment =
                    reinterpret_cast<uint0>(source2) - (reinterpret_cast<uint0>(source2) & ~alignMinus1);

                inputAlignment += systemAlignment;
                // Copy memory up until the required alignment for a bulk copy.
                uint0 outputAlignment2;
                if constexpr (alignof(T) <= 8) {
                    // Check if we need 2/4/6/8/10/12/14Byte or 4/8/12Byte or 8ByteCopy
                    if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                        outputAlignment2 = outputAlignment & 0x0F_ui0;
                    } else {
                        outputAlignment2 = outputAlignment;
                    }
                    if (XS_EXPECT(outputAlignment2)) {
                        if constexpr (alignof(T) == 1) {
                            for (auto count = outputAlignment2; count != 0; --count) {
                                --dest2;
                                --source2;
                                *dest2 = *source2;
                            }
                        } else if constexpr (alignof(T) == 2) {
                            for (auto count = outputAlignment2 >> 1; count != 0; --count) {
                                dest2 -= 2;
                                source2 -= 2;
                                *reinterpret_cast<uint16*>(dest2) = *reinterpret_cast<const uint16*>(source2);
                            }
                        } else if constexpr (alignof(T) == 4) {
                            for (auto count = outputAlignment2 >> 2; count != 0; --count) {
                                dest2 -= 4;
                                source2 -= 4;
                                *reinterpret_cast<uint32*>(dest2) = *reinterpret_cast<const uint32*>(source2);
                            }
                        } else /*alignof(T) == 8*/ {
                            dest2 -= 8;
                            source2 -= 8;
                            *reinterpret_cast<uint64*>(dest2) = *reinterpret_cast<const uint64*>(source2);
                        }
                        inputAlignment -= outputAlignment2;
                    }
                }
                if constexpr (alignof(T) <= 16 && hasISAFeature<ISAFeature::AVX2>) {
                    // Check if we need 16ByteCopy
                    if constexpr (hasISAFeature<ISAFeature::AVX512F> || alignof(T) < 16) {
                        outputAlignment2 = outputAlignment & 0x10_ui0;
                    } else {
                        outputAlignment2 = outputAlignment;
                    }
                    if (outputAlignment2) {
                        // Update the source and destination pointers
                        dest2 -= 16;
                        source2 -= 16;
                        inputAlignment -= 16;
                        // Check if source is aligned
                        __m128i src;
                        if constexpr (alignof(T) < 16) {
                            if (inputAlignment & 0x0F_ui0) {
                                src = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2));
                            } else {
                                src = _mm_load_si128(reinterpret_cast<const __m128i*>(source2));
                            }
                        } else {
                            src = _mm_load_si128(reinterpret_cast<const __m128i*>(source2));
                        }
                        // Dest is always aligned due to previous operations
                        _mm_store_si128(reinterpret_cast<__m128i*>(dest2), src);
                    }
                }
                if constexpr (alignof(T) <= 32 && hasISAFeature<ISAFeature::AVX512F>) {
                    // Check if we need 32ByteCopy
                    if constexpr (alignof(T) < 32) {
                        outputAlignment2 = outputAlignment & 0x20_ui0;
                    } else {
                        outputAlignment2 = outputAlignment;
                    }
                    if (outputAlignment2) {
                        // Update the source and destination pointers
                        dest2 -= 32;
                        source2 -= 32;
                        inputAlignment -= 32;
                        // Check if source is aligned
                        __m256i src;
                        if constexpr (alignof(T) < 32) {
                            if (inputAlignment & 0x1F_ui0) {
                                src = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2));
                            } else {
                                src = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2));
                            }
                        } else {
                            src = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2));
                        }
                        // Dest is always aligned due to previous operations
                        _mm256_store_si256(reinterpret_cast<__m256i*>(dest2), src);
                    }
                }
                size -= outputAlignment;

                // Check if we have correctly aligned pointers on both the source and the destination
                if (XS_UNEXPECT((inputAlignment & alignMinus1) == 0)) {
                    // Copy using aligned operations
                    if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                        const uint0 maxCount = size >> 6;
                        const uint0 loopCount = maxCount >> 3;
                        for (auto count = loopCount; count != 0; --count) {
                            source2 -= 512;
                            dest2 -= 512;
                            const __m512i src = _mm512_load_si512(source2);
                            const __m512i src2 = _mm512_load_si512(source2 + 64);
                            const __m512i src3 = _mm512_load_si512(source2 + 128);
                            const __m512i src4 = _mm512_load_si512(source2 + 192);
                            const __m512i src5 = _mm512_load_si512(source2 + 256);
                            const __m512i src6 = _mm512_load_si512(source2 + 320);
                            const __m512i src7 = _mm512_load_si512(source2 + 384);
                            const __m512i src8 = _mm512_load_si512(source2 + 448);
                            _mm512_store_si512(dest2, src);
                            _mm512_store_si512(dest2 + 64, src2);
                            _mm512_store_si512(dest2 + 128, src3);
                            _mm512_store_si512(dest2 + 192, src4);
                            _mm512_store_si512(dest2 + 256, src5);
                            _mm512_store_si512(dest2 + 320, src6);
                            _mm512_store_si512(dest2 + 384, src7);
                            _mm512_store_si512(dest2 + 448, src8);
                        }
                        for (auto count = maxCount - (loopCount << 3); count != 0; --count) {
                            source2 -= 64;
                            dest2 -= 64;
                            _mm512_store_si512(dest2, _mm512_load_si512(source2));
                        }
                        outputAlignment = size - (maxCount << 6);
                    } else if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                        const uint0 maxCount = size >> 5;
                        const uint0 loopCount = maxCount >> 3;
                        for (auto count = loopCount; count != 0; --count) {
                            source2 -= 256;
                            dest2 -= 256;
                            const __m256i src = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2));
                            const __m256i src2 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 32));
                            const __m256i src3 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 64));
                            const __m256i src4 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 96));
                            const __m256i src5 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 128));
                            const __m256i src6 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 160));
                            const __m256i src7 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 192));
                            const __m256i src8 = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2 + 224));
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2), src);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 32), src2);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 64), src3);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 96), src4);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 128), src5);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 160), src6);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 192), src7);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 224), src8);
                        }
                        for (auto count = maxCount - (loopCount << 3); count != 0; --count) {
                            source2 -= 32;
                            dest2 -= 32;
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2),
                                _mm256_load_si256(reinterpret_cast<const __m256i*>(source2)));
                        }
                        outputAlignment = size - (maxCount << 5);
                    } else {
                        const uint0 maxCount = size >> 4;
                        const uint0 loopCount = maxCount >> 3;
                        for (auto count = loopCount; count != 0; --count) {
                            source2 -= 128;
                            dest2 -= 128;
                            const __m128i src = _mm_load_si128(reinterpret_cast<const __m128i*>(source2));
                            const __m128i src2 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 16));
                            const __m128i src3 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 32));
                            const __m128i src4 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 48));
                            const __m128i src5 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 64));
                            const __m128i src6 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 80));
                            const __m128i src7 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 96));
                            const __m128i src8 = _mm_load_si128(reinterpret_cast<const __m128i*>(source2 + 112));
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2), src);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 16), src2);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 32), src3);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 48), src4);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 64), src5);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 80), src6);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 96), src7);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 112), src8);
                        }
                        for (auto count = maxCount - (loopCount << 3); count != 0; --count) {
                            source2 -= 16;
                            dest2 -= 16;
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2),
                                _mm_load_si128(reinterpret_cast<const __m128i*>(source2)));
                        }
                        outputAlignment = size - (maxCount << 4);
                    }
                } else {
                    // Copy using unaligned operations (destination will be aligned due to previous operations)
                    if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                        const uint0 maxCount = size >> 6;
                        const uint0 loopCount = maxCount >> 3;
                        for (auto count = loopCount; count != 0; --count) {
                            source2 -= 512;
                            dest2 -= 512;
                            const __m512i src = _mm512_loadu_si512(source2);
                            const __m512i src2 = _mm512_loadu_si512(source2 + 64);
                            const __m512i src3 = _mm512_loadu_si512(source2 + 128);
                            const __m512i src4 = _mm512_loadu_si512(source2 + 192);
                            const __m512i src5 = _mm512_loadu_si512(source2 + 256);
                            const __m512i src6 = _mm512_loadu_si512(source2 + 320);
                            const __m512i src7 = _mm512_loadu_si512(source2 + 384);
                            const __m512i src8 = _mm512_loadu_si512(source2 + 448);
                            _mm512_store_si512(dest2, src);
                            _mm512_store_si512(dest2 + 64, src2);
                            _mm512_store_si512(dest2 + 128, src3);
                            _mm512_store_si512(dest2 + 192, src4);
                            _mm512_store_si512(dest2 + 256, src5);
                            _mm512_store_si512(dest2 + 320, src6);
                            _mm512_store_si512(dest2 + 384, src7);
                            _mm512_store_si512(dest2 + 448, src8);
                        }
                        for (auto count = maxCount - (loopCount << 3); count != 0; --count) {
                            source2 -= 64;
                            dest2 -= 64;
                            _mm512_store_si512(dest2, _mm512_loadu_si512(source2));
                        }
                        outputAlignment = size - (maxCount << 6);
                    } else if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                        const uint0 maxCount = size >> 5;
                        const uint0 loopCount = maxCount >> 3;
                        for (auto count = loopCount; count != 0; --count) {
                            source2 -= 256;
                            dest2 -= 256;
                            const __m256i src = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2));
                            const __m256i src2 = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2 + 32));
                            const __m256i src3 = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2 + 64));
                            const __m256i src4 = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2 + 96));
                            const __m256i src5 = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2 + 128));
                            const __m256i src6 = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2 + 160));
                            const __m256i src7 = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2 + 192));
                            const __m256i src8 = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2 + 224));
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2), src);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 32), src2);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 64), src3);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 96), src4);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 128), src5);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 160), src6);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 192), src7);
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2 + 224), src8);
                        }
                        for (auto count = maxCount - (loopCount << 3); count != 0; --count) {
                            source2 -= 32;
                            dest2 -= 32;
                            _mm256_store_si256(reinterpret_cast<__m256i*>(dest2),
                                _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2)));
                        }
                        outputAlignment = size - (maxCount << 5);
                    } else {
                        const uint0 maxCount = size >> 4;
                        const uint0 loopCount = maxCount >> 3;
                        for (auto count = loopCount; count != 0; --count) {
                            source2 -= 128;
                            dest2 -= 128;
                            const __m128i src = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2));
                            const __m128i src2 = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2 + 16));
                            const __m128i src3 = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2 + 32));
                            const __m128i src4 = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2 + 48));
                            const __m128i src5 = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2 + 64));
                            const __m128i src6 = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2 + 80));
                            const __m128i src7 = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2 + 96));
                            const __m128i src8 = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2 + 112));
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2), src);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 16), src2);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 32), src3);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 48), src4);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 64), src5);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 80), src6);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 96), src7);
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2 + 112), src8);
                        }
                        for (auto count = maxCount - (loopCount << 3); count != 0; --count) {
                            source2 -= 16;
                            dest2 -= 16;
                            _mm_store_si128(reinterpret_cast<__m128i*>(dest2),
                                _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2)));
                        }
                        outputAlignment = size - (maxCount << 4);
                    }
                }
                // Check if there is any trailing element that needs to be copied
                if constexpr (alignof(T) <= 32 && hasISAFeature<ISAFeature::AVX512F>) {
                    // Check if we need 32Byte copy
                    if constexpr (alignof(T) < 32) {
                        outputAlignment2 = outputAlignment & 0x20_ui0;
                        outputAlignment &= 0x1F_ui0;
                    } else {
                        outputAlignment2 = outputAlignment;
                    }
                    if (outputAlignment2) {
                        // Update the source and destination pointers
                        dest2 -= 32;
                        source2 -= 32;
                        inputAlignment -= 32;
                        __m256i src;
                        if constexpr (alignof(T) < 32) {
                            if (inputAlignment & 0x1F_ui0) {
                                src = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(source2));
                            } else {
                                src = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2));
                            }
                        } else {
                            src = _mm256_load_si256(reinterpret_cast<const __m256i*>(source2));
                        }
                        // Dest is always aligned due to previous operations
                        _mm256_store_si256(reinterpret_cast<__m256i*>(dest2), src);
                    }
                }
                if constexpr (alignof(T) <= 16 && hasISAFeature<ISAFeature::AVX2>) {
                    // Check if we need 16Byte copy
                    if constexpr (hasISAFeature<ISAFeature::AVX512F> || alignof(T) < 16) {
                        outputAlignment2 = outputAlignment & 0x10_ui0;
                        outputAlignment &= 0x0F_ui0;
                    } else {
                        outputAlignment2 = outputAlignment;
                    }
                    if (outputAlignment2) {
                        // Update the source and destination pointers
                        dest2 -= 16;
                        source2 -= 16;
                        __m128i src;
                        if constexpr (alignof(T) < 16) {
                            if (inputAlignment & 0x0F_ui0) {
                                src = _mm_lddqu_si128(reinterpret_cast<const __m128i*>(source2));
                            } else {
                                src = _mm_load_si128(reinterpret_cast<const __m128i*>(source2));
                            }
                        } else {
                            src = _mm_load_si128(reinterpret_cast<const __m128i*>(source2));
                        }
                        // Dest is always aligned due to previous operations
                        _mm_store_si128(reinterpret_cast<__m128i*>(dest2), src);
                    }
                }
                if constexpr (alignof(T) == 8) {
                    if (outputAlignment) {
                        dest2 -= 8;
                        source2 -= 8;
                        *reinterpret_cast<uint64*>(dest2) = *reinterpret_cast<const uint64*>(source2);
                    }
                } else if constexpr (alignof(T) == 4) {
                    if (XS_EXPECT(outputAlignment)) {
                        const auto loops = outputAlignment >> 2;
                        for (auto count = loops; count != 0; --count) {
                            dest2 -= 4;
                            source2 -= 4;
                            *reinterpret_cast<uint32*>(dest2) = *reinterpret_cast<const uint32*>(source2);
                        }
                    }
                } else if constexpr (alignof(T) == 2) {
                    if (XS_EXPECT(outputAlignment)) {
                        const auto loops = outputAlignment >> 1;
                        for (auto count = loops; count != 0; --count) {
                            dest2 -= 2;
                            source2 -= 2;
                            *reinterpret_cast<uint16*>(dest2) = *reinterpret_cast<const uint16*>(source2);
                        }
                    }
                } else if constexpr (alignof(T) == 1) {
                    if (XS_EXPECT(outputAlignment)) {
                        const auto loops = outputAlignment;
                        for (auto count = loops; count != 0; --count) {
                            --dest2;
                            --source2;
                            *dest2 = *source2;
                        }
                    }
                }
            }
        } else {
            XS_REPMOVBAKSET;
            if constexpr (currentArch == Architecture::Bit64 && alignof(T) % 8 == 0) {
                XS_REPMOVSQ(dest2 - 8, source2 - 8, size >> 3);
            } else if constexpr (alignof(T) == 4) {
                XS_REPMOVSD(dest2 - 4, source2 - 4, size >> 2);
            } else if constexpr (alignof(T) == 2) {
                XS_REPMOVSW(dest2 - 2, source2 - 2, size >> 1);
            } else /*alignof(T) == 1*/ {
                XS_REPMOVSB(dest2 - 1, source2 - 1, size);
            }
            XS_REPMOVBAKCLEAR;
        }
    } else {
        constexpr auto logSize = NoExport::log2(sizeof(T));
        const auto loops = size >> logSize;
        source += size;
        dest += size; // Jump to end
        for (auto count = loops; count != 0; --count) {
            --source;
            --dest;
            *dest = *source;
        }
    }
}

/**
 * Construct a single memory address to default value.
 * @note This can be used to ensure that allocated memory is correctly filled with correct data.
 * @tparam T Generic type parameter.
 * @param  pointer The destination address to construct at.
 */
template<typename T>
XS_INLINE void memConstruct(T* const XS_RESTRICT pointer) noexcept
{
    static_assert(isDefaultConstructible<T>, "Memory of type 'T' can not be default constructed");
    static_assert(isNothrowDefaultConstructible<T>, "Only types that do not throw exceptions can be used");
    if constexpr (!isTriviallyConstructible<T>) {
        // Call constructor
        new (pointer) T();
    }
}

/**
 * Construct a single memory address to specific value.
 * @note This can be used to ensure that allocated memory is correctly filled with correct data.
 * @tparam T Generic type parameter.
 * @param pointer The destination address to construct at.
 * @param value   The value to construct at address.
 */
template<typename T>
XS_INLINE void memConstruct(T* const XS_RESTRICT pointer, const T& value) noexcept
{
    static_assert(isCopyConstructible<T>, "Memory of type 'T' can not be copy constructed");
    static_assert(isNothrowCopyConstructible<T>, "Only types that do not throw exceptions can be used");
    new (pointer) T(value);
}

/**
 * Construct a single memory address using direct construction.
 * @note This can be used to ensure that allocated memory is correctly filled with correct data.
 * @tparam T    Generic type parameter.
 * @tparam Args Type of the arguments.
 * @param  pointer The destination address to construct at.
 * @param  values  The values to construct element at address.
 */
template<typename T, typename... Args>
XS_INLINE void memConstruct(T* const XS_RESTRICT pointer, Args&&... values) noexcept
{
    static_assert(isConstructible<T, Args...>, "Memory of type 'T' can not be constructed from 'Args'");
    static_assert(isNothrowConstructible<T, Args...>, "Only types that do not throw exceptions can be used");
    new (pointer) T(forward<Args>(values)...);
}

/**
 * Destruct a single memory addresses.
 * @note This can be used to ensure that allocated memory is correctly filled with correct data.
 * @tparam T Generic type parameter.
 * @param  pointer The destination address to destruct at.
 */
template<typename T>
XS_INLINE void memDestruct(T* const XS_RESTRICT pointer) noexcept
{
    static_assert(isDestructible<T>, "Memory of type 'T' can not be destructed");
    static_assert(isNothrowDestructible<T>, "Only types that do not throw exceptions can be used");
    if constexpr (!isTriviallyDestructible<T>) {
        pointer->~T();
    }
}

/**
 * Construct a range of memory addresses to default values.
 * @note This can be used to ensure that allocated memory is correctly filled with correct data.
 * @tparam T Generic type parameter.
 * @param  pointer The destination address to start constructing at.
 * @param  size    The number of bytes to construct over.
 */
template<typename T>
XS_INLINE void memConstructRange(T* XS_RESTRICT pointer, const int0 size) noexcept
{
    static_assert(isDefaultConstructible<T>, "Memory of type 'T' can not be default constructed");
    static_assert(isNothrowDefaultConstructible<T>, "Only types that do not throw exceptions can be used");
    XS_ASSERT(size % sizeof(T) == 0);
    if constexpr (!isTriviallyConstructible<T>) {
        // Must loop over all elements in the range and construct as required
        constexpr auto logSize = NoExport::log2(sizeof(T));
        for (auto i = size >> logSize; i != 0; --i) {
            new (pointer) T();
            ++pointer;
        }
    }
}

/**
 * Construct a range of memory addresses to match a second input range of values.
 * @note This can be used to ensure that allocated memory is correctly filled with correct data.
 * @tparam T  Generic type parameter.
 * @tparam T2 Type of destination object.
 * @param  pointer The destination address to start constructing at.
 * @param  values  The values to construct at the addresses.
 * @param  size    The number of bytes to construct over.
 */
template<typename T, typename T2>
XS_INLINE void memConstructRange(T* XS_RESTRICT pointer, const T2* const XS_RESTRICT values, const uint0 size) noexcept
{
    static_assert(isSame<T, T2> || isConstructible<T, T2>, "Memory of type 'T' can not be constructed from 'T2'");
    static_assert(
        isSame<T, T2> || isNothrowConstructible<T, T2>, "Only types that do not throw exceptions can be used");
    XS_ASSERT(size % sizeof(T) == 0);
    if constexpr (isTriviallyCopyable<T> && isSame<T, T2>) {
        // TODO: c++20 is_layout_compatible with isTriviallyCopyable<T> && isTriviallyCopyable<T2>
        memMove<T>(pointer, reinterpret_cast<const T*>(values), size);
    } else {
        /*Must loop over all elements in the range and construct as required */
        constexpr auto logSize = NoExport::log2(sizeof(T));
        for (auto i = size >> logSize; i != 0; --i) {
            new (pointer) T(*values);
            ++pointer;
            ++values;
        }
    }
}

/**
 * Copy data from one location to another constructing if required.
 * @note This will not just copy data but also insure that appropriate
 * assignment operators are called on any non constructed areas when requested.
 * @tparam T  Generic type parameter.
 * @tparam T2 Type of destination object.
 * @param  pointer         The destination address to start copying to.
 * @param  values          The source address to start copying from.
 * @param  size            The number of bytes to copy.
 * @param  constructedSize The number of bytes at destination that have already been constructed.
 */
template<typename T, typename T2>
XS_INLINE void memCopyConstructRange(
    T* XS_RESTRICT pointer, const T2* XS_RESTRICT values, const uint0 size, const uint0 constructedSize) noexcept
{
    static_assert(isSame<T, T2> || isConstructible<T, T2>, "Memory of type 'T' can not be constructed from 'T2'");
    static_assert(isSame<T, T2> || isAssignable<T, T2>, "Memory of type 'T' can not be assigned a type 'T2'");
    static_assert(isSame<T, T2> || isNothrowConstructible<T, T2> && isNothrowAssignable<T, T2>,
        "Only types that do not throw exceptions can be used");
    XS_ASSERT(size % sizeof(T) == 0);
    XS_ASSERT(constructedSize % sizeof(T) == 0);
    if constexpr (isTriviallyCopyable<T> && isSame<T, T2>) {
        // TODO: c++20 is_layout_compatible with isTriviallyCopyable<T> && isTriviallyCopyable<T2>
        memMove<T>(pointer, reinterpret_cast<const T*>(values), size);
    } else {
        /*Copy data over the range that has been constructed already */
        constexpr auto logSize = NoExport::log2(sizeof(T));
        for (auto i = constructedSize >> logSize; i != 0; --i) {
            *pointer = *values;
            ++pointer;
            ++values;
        }
        /*Construct the remaining data*/
        const uint0 unconstructedSize = size - constructedSize;
        for (auto i = unconstructedSize >> logSize; i != 0; --i) {
            new (pointer) T(*values);
            ++pointer;
            ++values;
        }
    }
}

/**
 * Destruct a range of memory addresses.
 * @note This can be used to ensure that allocated memory is correctly filled with correct data.
 * @tparam T Generic type parameter.
 * @param  pointer The destination address to start destruct at.
 * @param  size    The number of bytes to destruct over.
 */
template<typename T>
XS_INLINE void memDestructRange(T* XS_RESTRICT pointer, const uint0 size) noexcept
{
    static_assert(isDestructible<T>, "Memory of type 'T' can not be destructed");
    static_assert(isNothrowDestructible<T>, "Only types that do not throw exceptions can be used");
    XS_ASSERT(size % sizeof(T) == 0);
    if constexpr (!isTriviallyDestructible<T>) {
        // Must loop over all elements in the range and destruct as required
        constexpr auto logSize = NoExport::log2(sizeof(T));
        for (auto i = size >> logSize; i != 0; --i) {
            pointer->~T();
            ++pointer;
        }
    }
}

/**
 * Copy data from one location to another.
 * @note This will not just copy data but also insure that appropriate
 * assignment operators are called when requested.
 * @tparam T  Generic type parameter.
 * @tparam T2 Type of destination object.
 * @param  dest   The destination address to start copying to.
 * @param  source The source address to start copying from.
 * @param  size   The number of bytes to copy.
 */
template<typename T, typename T2>
XS_INLINE void memCopy(T* XS_RESTRICT dest, const T2* XS_RESTRICT source, const uint0 size) noexcept
{
    static_assert(isSame<T, T2> || isAssignable<T, T2>, "Memory of type 'T' can not be assigned a type 'T2'");
    static_assert(isSame<T, T2> || isNothrowAssignable<T, T2>, "Only types that do not throw exceptions can be used");
    XS_ASSERT(size % sizeof(T) == 0);
    if constexpr (isTriviallyCopyable<T> && isSame<T, T2>) {
        // TODO: c++20 is_layout_compatible with isTriviallyCopyable<T> && isTriviallyCopyable<T2>
        memMove<T>(dest, reinterpret_cast<const T*>(source), size);
    } else {
        // Cant use memcopy as complex types require an overloaded equals operator
        constexpr auto logSize = NoExport::log2(sizeof(T));
        for (auto i = size >> logSize; i != 0; --i) {
            *dest = *source;
            ++dest;
            ++source;
        }
    }
}

/**
 * Reverse a sequence of data.
 * @note This reverses the order that data occurs in from within the specified range.
 * @tparam T Generic type parameter.
 * @param  start The start of the section or memory the reverse occurs within.
 * @param  end   The end of the section or memory the reverse occurs within (non inclusive).
 */
template<typename T>
XS_INLINE void memReverse(T* XS_RESTRICT start, T* XS_RESTRICT end) noexcept
{
    XS_ASSERT((reinterpret_cast<uint0>(start) % sizeof(T)) == 0);
    XS_ASSERT((reinterpret_cast<uint0>(end) % sizeof(T)) == 0);
    XS_ASSERT(end > start);
    if constexpr (hasISAFeature<ISAFeature::SSE2> && (sizeof(T) % 2 == 0) && (sizeof(T) < systemAlignment)) {
        constexpr auto logSize = NoExport::log2(sizeof(T));
        constexpr uint32 copyThreshold =
            (alignof(T) == 1) ? systemAlignment * 4 : ((alignof(T) == 2) ? systemAlignment * 2 : systemAlignment);
        if (uint0 size = (reinterpret_cast<uint0>(end) - reinterpret_cast<uint0>(start));
            XS_UNEXPECT(size < (2 * copyThreshold))) {
            // Reverse memory using default method
            while (start < end) {
                --end;
                memSwap(start, end);
                ++start;
            }
            return;
        }

        auto start2 = reinterpret_cast<uint8*>(start);
        auto end2 = reinterpret_cast<uint8*>(end);
        // Get the alignment of the end position
        constexpr auto alignMinus1 = systemAlignment - 1_ui0;
        uint0 endAlignment = reinterpret_cast<uint0>(end2) - (reinterpret_cast<uint0>(end2) & ~alignMinus1);
        uint0 startAlignment =
            ((reinterpret_cast<uint0>(start2) + alignMinus1) & ~alignMinus1) - reinterpret_cast<uint0>(start2);

        if constexpr (sizeof(T) < 16) {
            uint0 endAlignment2;
            // Copy memory up until the required alignment for a bulk copy.
            // Check if we need 1-15Byte Copy
            if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                endAlignment2 = endAlignment & 0x0F_ui0;
                endAlignment -= endAlignment2;
            } else {
                endAlignment2 = endAlignment;
            }
            // Perform outside rotate copy till alignment
            for (auto count = endAlignment2 >> logSize; count != 0; --count) {
                end2 -= sizeof(T);
                memSwap(reinterpret_cast<T*>(start2), reinterpret_cast<T*>(end2));
                start2 += sizeof(T);
            }
        }

        // Generate the shuffle mask
        __m512i shuff8X4;
        __m256i shuff8X2;
        __m128i shuff8;
        if constexpr (sizeof(T) == 1) {
            if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                shuff8X4 = _mm512_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7,
                    8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4,
                    5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
            }
            if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                shuff8X2 = _mm256_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7,
                    8, 9, 10, 11, 12, 13, 14, 15);
            }
            shuff8 = _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        }

        // Check if we have correctly aligned pointers on both the source and the destination
        if (XS_UNEXPECT(startAlignment == endAlignment)) {
            // Copy using aligned operations
            if constexpr (hasISAFeature<ISAFeature::AVX2> && (sizeof(T) <= 16)) {
                // Check if we need 16Byte copy
                uint0 check = endAlignment;
                if constexpr (sizeof(T) < 16) {
                    check &= 0x10_ui0;
                }
                if (check) {
                    end2 -= 16;
                    __m128i iStart = _mm_load_si128(reinterpret_cast<__m128i*>(start2));
                    __m128i iEnd = _mm_load_si128(reinterpret_cast<__m128i*>(end2));
                    if constexpr (sizeof(T) == 1) {
                        iStart = _mm_shuffle_epi8(iStart, shuff8);
                        iEnd = _mm_shuffle_epi8(iEnd, shuff8);
                    } else if constexpr (sizeof(T) == 2) {
                        iStart = _mm_shufflehi_epi16(
                            _mm_shufflelo_epi16(iStart, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm_shufflehi_epi16(
                            _mm_shufflelo_epi16(iEnd, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                    } else if constexpr (sizeof(T) == 4) {
                        iStart = _mm_shuffle_epi32(iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm_shuffle_epi32(iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                    } else if constexpr (sizeof(T) == 8) {
                        iStart = _mm_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                    }
                    _mm_store_si128(reinterpret_cast<__m128i*>(end2), iStart);
                    _mm_store_si128(reinterpret_cast<__m128i*>(start2), iEnd);
                    start2 += 16;
                }
            }
            if constexpr (hasISAFeature<ISAFeature::AVX512F> && (sizeof(T) <= 32)) {
                // Check if we need 32Byte copy
                uint0 check = endAlignment;
                if constexpr (sizeof(T) < 32) {
                    check &= 0x20_ui0;
                }
                if (check) {
                    end2 -= 32;
                    __m256i iStart = _mm256_load_si256(reinterpret_cast<__m256i*>(start2));
                    __m256i iEnd = _mm256_load_si256(reinterpret_cast<__m256i*>(end2));
                    if constexpr (sizeof(T) == 1) {
                        iStart = _mm256_shuffle_epi8(iStart, shuff8X2);
                        iEnd = _mm256_shuffle_epi8(iEnd, shuff8X2);
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 2) {
                        iStart = _mm256_shufflehi_epi16(
                            _mm256_shufflelo_epi16(iStart, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm256_shufflehi_epi16(
                            _mm256_shufflelo_epi16(iEnd, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm256_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm256_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 4) {
                        iStart = _mm256_shuffle_epi32(iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm256_shuffle_epi32(iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 8) {
                        iStart = _mm256_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm256_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 16) {
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    }
                    _mm256_store_si256(reinterpret_cast<__m256i*>(end2), iStart);
                    _mm256_store_si256(reinterpret_cast<__m256i*>(start2), iEnd);
                    start2 += 32;
                }
            }
            // Need to determine the number of copies required
            constexpr auto alignRight = NoExport::log2(systemAlignment) + 1_ui0;
            uint0 copies = (reinterpret_cast<uint0>(end2) - reinterpret_cast<uint0>(start2)) >> alignRight;
            for (auto count = copies; count != 0; --count) {
                end2 -= systemAlignment;

                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    __m512i iStart = _mm512_load_si512(start2);
                    __m512i iEnd = _mm512_load_si512(end2);
                    if constexpr (sizeof(T) == 1) {
                        iStart = _mm512_shuffle_epi8(iStart, shuff8X4);
                        iEnd = _mm512_shuffle_epi8(iEnd, shuff8X4);
                        iStart = _mm512_shuffle_i32x4(iStart, iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm512_shuffle_i32x4(iEnd, iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                    } else if constexpr (sizeof(T) == 2) {
                        iStart = _mm512_shufflehi_epi16(
                            _mm512_shufflelo_epi16(iStart, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm512_shufflehi_epi16(
                            _mm512_shufflelo_epi16(iEnd, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm512_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm512_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                        iStart = _mm512_shuffle_i32x4(iStart, iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm512_shuffle_i32x4(iEnd, iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                    } else if constexpr (sizeof(T) == 4) {
                        iStart = _mm512_shuffle_epi32(iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm512_shuffle_epi32(iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm512_shuffle_i32x4(iStart, iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm512_shuffle_i32x4(iEnd, iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                    } else if constexpr (sizeof(T) == 8) {
                        iStart = _mm512_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm512_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                        iStart = _mm512_shuffle_i32x4(iStart, iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm512_shuffle_i32x4(iEnd, iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                    } else if constexpr (sizeof(T) == 16) {
                        iStart = _mm512_shuffle_i32x4(iStart, iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm512_shuffle_i32x4(iEnd, iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                    } else if constexpr (sizeof(T) == 32) {
                        iStart = _mm512_shuffle_i32x4(iStart, iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm512_shuffle_i32x4(iEnd, iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                    }
                    _mm512_store_si512(end2, iStart);
                    _mm512_store_si512(start2, iEnd);
                } else if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                    __m256i iStart = _mm256_load_si256(reinterpret_cast<__m256i*>(start2));
                    __m256i iEnd = _mm256_load_si256(reinterpret_cast<__m256i*>(end2));
                    if constexpr (sizeof(T) == 1) {
                        iStart = _mm256_shuffle_epi8(iStart, shuff8X2);
                        iEnd = _mm256_shuffle_epi8(iEnd, shuff8X2);
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 2) {
                        iStart = _mm256_shufflehi_epi16(
                            _mm256_shufflelo_epi16(iStart, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm256_shufflehi_epi16(
                            _mm256_shufflelo_epi16(iEnd, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm256_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm256_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 4) {
                        iStart = _mm256_shuffle_epi32(iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm256_shuffle_epi32(iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 8) {
                        iStart = _mm256_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm256_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 16) {
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    }
                    _mm256_store_si256(reinterpret_cast<__m256i*>(end2), iStart);
                    _mm256_store_si256(reinterpret_cast<__m256i*>(start2), iEnd);
                } else {
                    __m128i iStart = _mm_load_si128(reinterpret_cast<__m128i*>(start2));
                    __m128i iEnd = _mm_load_si128(reinterpret_cast<__m128i*>(end2));
                    if constexpr (sizeof(T) == 1) {
                        iStart = _mm_shuffle_epi8(iStart, shuff8);
                        iEnd = _mm_shuffle_epi8(iEnd, shuff8);
                    } else if constexpr (sizeof(T) == 2) {
                        iStart = _mm_shufflehi_epi16(
                            _mm_shufflelo_epi16(iStart, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm_shufflehi_epi16(
                            _mm_shufflelo_epi16(iEnd, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                    } else if constexpr (sizeof(T) == 4) {
                        iStart = _mm_shuffle_epi32(iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm_shuffle_epi32(iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                    } else if constexpr (sizeof(T) == 8) {
                        iStart = _mm_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                    }
                    _mm_store_si128(reinterpret_cast<__m128i*>(end2), iStart);
                    _mm_store_si128(reinterpret_cast<__m128i*>(start2), iEnd);
                }
                start2 += systemAlignment;
            }
            // Update alignment for any remaining copies
            endAlignment = (reinterpret_cast<uint0>(end2) - reinterpret_cast<uint0>(start2)) >> 1;

            if constexpr (hasISAFeature<ISAFeature::AVX512F> && (sizeof(T) <= 32)) {
                // Check if we need 32Byte copy
                uint0 check = endAlignment;
                if constexpr (sizeof(T) < 32) {
                    check &= 0x20_ui0;
                }
                if (check) {
                    end2 -= 32;
                    __m256i iStart = _mm256_load_si256(reinterpret_cast<__m256i*>(start2));
                    __m256i iEnd = _mm256_load_si256(reinterpret_cast<__m256i*>(end2));
                    if constexpr (sizeof(T) == 1) {
                        iStart = _mm256_shuffle_epi8(iStart, shuff8X2);
                        iEnd = _mm256_shuffle_epi8(iEnd, shuff8X2);
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 2) {
                        iStart = _mm256_shufflehi_epi16(
                            _mm256_shufflelo_epi16(iStart, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm256_shufflehi_epi16(
                            _mm256_shufflelo_epi16(iEnd, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm256_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm256_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 4) {
                        iStart = _mm256_shuffle_epi32(iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm256_shuffle_epi32(iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 8) {
                        iStart = _mm256_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm256_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 16) {
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    }
                    _mm256_store_si256(reinterpret_cast<__m256i*>(end2), iStart);
                    _mm256_store_si256(reinterpret_cast<__m256i*>(start2), iEnd);
                    if constexpr (sizeof(T) < 32) {
                        start2 += 32;
                        endAlignment -= 32;
                    }
                }
            }
            if constexpr (hasISAFeature<ISAFeature::AVX2> && (sizeof(T) <= 16)) {
                // Check if we need 16Byte copy
                uint0 check = endAlignment;
                if constexpr (sizeof(T) < 16) {
                    check &= 0x10_ui0;
                }
                if (check) {
                    end2 -= 16;
                    __m128i iStart = _mm_load_si128(reinterpret_cast<__m128i*>(start2));
                    __m128i iEnd = _mm_load_si128(reinterpret_cast<__m128i*>(end2));
                    if constexpr (sizeof(T) == 1) {
                        iStart = _mm_shuffle_epi8(iStart, shuff8);
                        iEnd = _mm_shuffle_epi8(iEnd, shuff8);
                    } else if constexpr (sizeof(T) == 2) {
                        iStart = _mm_shufflehi_epi16(
                            _mm_shufflelo_epi16(iStart, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm_shufflehi_epi16(
                            _mm_shufflelo_epi16(iEnd, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                    } else if constexpr (sizeof(T) == 4) {
                        iStart = _mm_shuffle_epi32(iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm_shuffle_epi32(iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                    } else if constexpr (sizeof(T) == 8) {
                        iStart = _mm_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                    }
                    _mm_store_si128(reinterpret_cast<__m128i*>(end2), iStart);
                    _mm_store_si128(reinterpret_cast<__m128i*>(start2), iEnd);
                    if constexpr (sizeof(T) < 16) {
                        start2 += 16;
                        endAlignment -= 16;
                    }
                }
            }
        } else {
            // Copy using unaligned operations on the start (end will be aligned due to previous operation)
            if constexpr (hasISAFeature<ISAFeature::AVX2> && (sizeof(T) <= 16)) {
                // Check if we need 16Byte copy
                uint0 check = endAlignment;
                if constexpr (sizeof(T) < 16) {
                    check &= 0x10_ui0;
                }
                if (check) {
                    end2 -= 16;
                    __m128i iStart = _mm_lddqu_si128(reinterpret_cast<__m128i*>(start2));
                    __m128i iEnd = _mm_load_si128(reinterpret_cast<__m128i*>(end2));
                    if constexpr (sizeof(T) == 1) {
                        iStart = _mm_shuffle_epi8(iStart, shuff8);
                        iEnd = _mm_shuffle_epi8(iEnd, shuff8);
                    } else if constexpr (sizeof(T) == 2) {
                        iStart = _mm_shufflehi_epi16(
                            _mm_shufflelo_epi16(iStart, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm_shufflehi_epi16(
                            _mm_shufflelo_epi16(iEnd, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                    } else if constexpr (sizeof(T) == 4) {
                        iStart = _mm_shuffle_epi32(iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm_shuffle_epi32(iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                    } else if constexpr (sizeof(T) == 8) {
                        iStart = _mm_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                    }
                    _mm_store_si128(reinterpret_cast<__m128i*>(end2), iStart);
                    _mm_storeu_si128(reinterpret_cast<__m128i*>(start2), iEnd);
                    start2 += 16;
                }
            }
            if constexpr (hasISAFeature<ISAFeature::AVX512F> && (sizeof(T) <= 32)) {
                // Check if we need 32Byte copy
                uint0 check = endAlignment;
                if constexpr (sizeof(T) < 32) {
                    check &= 0x20_ui0;
                }
                if (check) {
                    end2 -= 32;
                    __m256i iStart = _mm256_lddqu_si256(reinterpret_cast<__m256i*>(start2));
                    __m256i iEnd = _mm256_load_si256(reinterpret_cast<__m256i*>(end2));
                    if constexpr (sizeof(T) == 1) {
                        iStart = _mm256_shuffle_epi8(iStart, shuff8X2);
                        iEnd = _mm256_shuffle_epi8(iEnd, shuff8X2);
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 2) {
                        iStart = _mm256_shufflehi_epi16(
                            _mm256_shufflelo_epi16(iStart, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm256_shufflehi_epi16(
                            _mm256_shufflelo_epi16(iEnd, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm256_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm256_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 4) {
                        iStart = _mm256_shuffle_epi32(iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm256_shuffle_epi32(iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 8) {
                        iStart = _mm256_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm256_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 16) {
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    }
                    _mm256_store_si256(reinterpret_cast<__m256i*>(end2), iStart);
                    _mm256_storeu_si256(reinterpret_cast<__m256i*>(start2), iEnd);
                    start2 += 32;
                }
            }
            // Need to determine the number of copies required
            constexpr auto alignRight = NoExport::log2(systemAlignment) + 1_ui0;
            uint0 copies = (reinterpret_cast<uint0>(end2) - reinterpret_cast<uint0>(start2)) >> alignRight;
            for (auto count = copies; count != 0; --count) {
                end2 -= systemAlignment;

                if constexpr (hasISAFeature<ISAFeature::AVX512F>) {
                    __m512i iStart = _mm512_loadu_si512(start2);
                    __m512i iEnd = _mm512_load_si512(end2);
                    if constexpr (sizeof(T) == 1) {
                        iStart = _mm512_shuffle_epi8(iStart, shuff8X4);
                        iEnd = _mm512_shuffle_epi8(iEnd, shuff8X4);
                        iStart = _mm512_shuffle_i32x4(iStart, iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm512_shuffle_i32x4(iEnd, iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                    } else if constexpr (sizeof(T) == 2) {
                        iStart = _mm512_shufflehi_epi16(
                            _mm512_shufflelo_epi16(iStart, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm512_shufflehi_epi16(
                            _mm512_shufflelo_epi16(iEnd, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm512_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm512_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                        iStart = _mm512_shuffle_i32x4(iStart, iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm512_shuffle_i32x4(iEnd, iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                    } else if constexpr (sizeof(T) == 4) {
                        iStart = _mm512_shuffle_epi32(iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm512_shuffle_epi32(iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm512_shuffle_i32x4(iStart, iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm512_shuffle_i32x4(iEnd, iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                    } else if constexpr (sizeof(T) == 8) {
                        iStart = _mm512_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm512_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                        iStart = _mm512_shuffle_i32x4(iStart, iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm512_shuffle_i32x4(iEnd, iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                    } else if constexpr (sizeof(T) == 16) {
                        iStart = _mm512_shuffle_i32x4(iStart, iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm512_shuffle_i32x4(iEnd, iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                    } else if constexpr (sizeof(T) == 32) {
                        iStart = _mm512_shuffle_i32x4(iStart, iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm512_shuffle_i32x4(iEnd, iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                    }
                    _mm512_store_si512(end2, iStart);
                    _mm512_storeu_si512(start2, iEnd);
                } else if constexpr (hasISAFeature<ISAFeature::AVX2>) {
                    __m256i iStart = _mm256_lddqu_si256(reinterpret_cast<__m256i*>(start2));
                    __m256i iEnd = _mm256_load_si256(reinterpret_cast<__m256i*>(end2));
                    if constexpr (sizeof(T) == 1) {
                        iStart = _mm256_shuffle_epi8(iStart, shuff8X2);
                        iEnd = _mm256_shuffle_epi8(iEnd, shuff8X2);
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 2) {
                        iStart = _mm256_shufflehi_epi16(
                            _mm256_shufflelo_epi16(iStart, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm256_shufflehi_epi16(
                            _mm256_shufflelo_epi16(iEnd, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm256_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm256_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 4) {
                        iStart = _mm256_shuffle_epi32(iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm256_shuffle_epi32(iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 8) {
                        iStart = _mm256_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm256_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 16) {
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    }
                    _mm256_store_si256(reinterpret_cast<__m256i*>(end2), iStart);
                    _mm256_storeu_si256(reinterpret_cast<__m256i*>(start2), iEnd);
                } else {
                    __m128i iStart = _mm_lddqu_si128(reinterpret_cast<__m128i*>(start2));
                    __m128i iEnd = _mm_load_si128(reinterpret_cast<__m128i*>(end2));
                    if constexpr (sizeof(T) == 1) {
                        iStart = _mm_shuffle_epi8(iStart, shuff8);
                        iEnd = _mm_shuffle_epi8(iEnd, shuff8);
                    } else if constexpr (sizeof(T) == 2) {
                        iStart = _mm_shufflehi_epi16(
                            _mm_shufflelo_epi16(iStart, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm_shufflehi_epi16(
                            _mm_shufflelo_epi16(iEnd, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                    } else if constexpr (sizeof(T) == 4) {
                        iStart = _mm_shuffle_epi32(iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm_shuffle_epi32(iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                    } else if constexpr (sizeof(T) == 8) {
                        iStart = _mm_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                    }
                    _mm_store_si128(reinterpret_cast<__m128i*>(end2), iStart);
                    _mm_storeu_si128(reinterpret_cast<__m128i*>(start2), iEnd);
                }
                start2 += systemAlignment;
            }
            // Update alignment for any remaining copies
            endAlignment = (reinterpret_cast<uint0>(end2) - reinterpret_cast<uint0>(start2)) >> 1;

            if constexpr (hasISAFeature<ISAFeature::AVX512F> && (sizeof(T) <= 32)) {
                // Check if we need 32Byte copy
                uint0 check = endAlignment;
                if constexpr (sizeof(T) < 32) {
                    check &= 0x20_ui0;
                }
                if (check) {
                    end2 -= 32;
                    __m256i iStart = _mm256_lddqu_si256(reinterpret_cast<__m256i*>(start2));
                    __m256i iEnd = _mm256_load_si256(reinterpret_cast<__m256i*>(end2));
                    if constexpr (sizeof(T) == 1) {
                        iStart = _mm256_shuffle_epi8(iStart, shuff8X2);
                        iEnd = _mm256_shuffle_epi8(iEnd, shuff8X2);
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 2) {
                        iStart = _mm256_shufflehi_epi16(
                            _mm256_shufflelo_epi16(iStart, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm256_shufflehi_epi16(
                            _mm256_shufflelo_epi16(iEnd, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm256_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm256_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 4) {
                        iStart = _mm256_shuffle_epi32(iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm256_shuffle_epi32(iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 8) {
                        iStart = _mm256_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm256_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    } else if constexpr (sizeof(T) == 16) {
                        iStart = _mm256_permute2f128_si256(iStart, iStart, _MM256_PERMUTE(0, 1));
                        iEnd = _mm256_permute2f128_si256(iEnd, iEnd, _MM256_PERMUTE(0, 1));
                    }
                    _mm256_store_si256(reinterpret_cast<__m256i*>(end2), iStart);
                    _mm256_storeu_si256(reinterpret_cast<__m256i*>(start2), iEnd);
                    if constexpr (sizeof(T) < 32) {
                        start2 += 32;
                        endAlignment -= 32;
                    }
                }
            }
            if constexpr (hasISAFeature<ISAFeature::AVX2> && (sizeof(T) <= 16)) {
                // Check if we need 16Byte copy
                uint0 check = endAlignment;
                if constexpr (sizeof(T) < 16) {
                    check &= 0x10_ui0;
                }
                if (check) {
                    end2 -= 16;
                    __m128i iStart = _mm_lddqu_si128(reinterpret_cast<__m128i*>(start2));
                    __m128i iEnd = _mm_load_si128(reinterpret_cast<__m128i*>(end2));
                    if constexpr (sizeof(T) == 1) {
                        iStart = _mm_shuffle_epi8(iStart, shuff8);
                        iEnd = _mm_shuffle_epi8(iEnd, shuff8);
                    } else if constexpr (sizeof(T) == 2) {
                        iStart = _mm_shufflehi_epi16(
                            _mm_shufflelo_epi16(iStart, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm_shufflehi_epi16(
                            _mm_shufflelo_epi16(iEnd, _MM_SHUFFLE(0, 1, 2, 3)), _MM_SHUFFLE(0, 1, 2, 3));
                        iStart = _mm_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                    } else if constexpr (sizeof(T) == 4) {
                        iStart = _mm_shuffle_epi32(iStart, _MM_SHUFFLE(0, 1, 2, 3));
                        iEnd = _mm_shuffle_epi32(iEnd, _MM_SHUFFLE(0, 1, 2, 3));
                    } else if constexpr (sizeof(T) == 8) {
                        iStart = _mm_shuffle_epi32(iStart, _MM_SHUFFLE(1, 0, 3, 2));
                        iEnd = _mm_shuffle_epi32(iEnd, _MM_SHUFFLE(1, 0, 3, 2));
                    }
                    _mm_store_si128(reinterpret_cast<__m128i*>(end2), iStart);
                    _mm_storeu_si128(reinterpret_cast<__m128i*>(start2), iEnd);
                    if constexpr (sizeof(T) < 16) {
                        start2 += 16;
                        endAlignment -= 16;
                    }
                }
            }
        }
        if constexpr (sizeof(T) < 16) {
            // Perform outside rotate copy to copy any remaining
            for (auto count = endAlignment >> logSize; count != 0; --count) {
                end2 -= sizeof(T);
                memSwap(reinterpret_cast<T*>(start2), reinterpret_cast<T*>(end2));
                start2 += sizeof(T);
            }
        }
    } else {
        while (start < end) {
            --end;
            memSwap(start, end);
            ++start;
        }
    }
}

/**
 * Swaps the data located at one location with the data from another.
 * @tparam T Generic type parameter.
 * @param  first  The first data location.
 * @param  second The second data location to swap with the first.
 */
template<typename T>
XS_INLINE void memSwap(T* const XS_RESTRICT first, T* const XS_RESTRICT second) noexcept
{
    using block = NoExport::BulkBlock<T>;
    if constexpr (hasSIMD<uint32> && (alignof(T) >= 16)) {
        // If we can do SIMD optimised bulk copy then prefer that
        static_assert(sizeof(T) == sizeof(block), "Invalid block size");
        block temp = *reinterpret_cast<block*>(first);
        *reinterpret_cast<block*>(first) = *reinterpret_cast<block*>(second);
        *reinterpret_cast<block*>(second) = temp;
    } else if constexpr (isTriviallyCopyable<T>) {
        T temp = *first;
        *first = *second;
        *second = temp;
    } else if constexpr (isSwappableMember<T>) {
        // All allocated types must define there own swap function
        first->swap(*second);
    } else if constexpr (isSwappable<T>) {
        swap(*first, *second);
    } else {
        // Brute force a memory move
        static_assert(sizeof(T) == sizeof(block), "Invalid block size");
        block temp = *reinterpret_cast<block*>(first);
        *reinterpret_cast<block*>(first) = *static_cast<block*>(second);
        *reinterpret_cast<block*>(second) = temp;
    }
}

/**
 * Rotate data around a central pivot point.
 * @note This just swaps all data on the left of the pivot point with all the data on the right of the pivot point.
 * @tparam T Generic type parameter.
 * @param  start The start of the section or memory the rotate occurs within.
 * @param  pivot The pivot point for the rotation.
 * @param  end   The end of the section or memory the rotate occurs within (non inclusive).
 * @returns The new middle point once all the data has been rotated.
 */
template<typename T>
XS_INLINE T* memRotate(T* const XS_RESTRICT start, T* const XS_RESTRICT pivot, T* const XS_RESTRICT end) noexcept
{
    XS_ASSERT(start < pivot && pivot < end);
    if (XS_UNEXPECT(start == pivot)) {
        return end;
    }
    if (XS_UNEXPECT(end == pivot)) {
        return start;
    }
    if (XS_UNEXPECT((end - start) == 2)) {
        memSwap(start, pivot);
        return pivot;
    }

    memReverse(start, pivot);
    memReverse(pivot, end);

    T* first = start;
    T* last = end;
    while (first != pivot && pivot != last) {
        memSwap(first, --last); // Make sure we use the correct swap
        ++first;
    }

    if (first == pivot) {
        memReverse(pivot, last);
        return last;
    }
    memReverse(first, pivot);
    return first;
}
} // namespace Shift
