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

#include "XSBenchConfig.h"
#include "XSCompiler.h"

#include <benchmark/benchmark.h>

#define BENCHMARK_TEMPLATET(n, p) BENCHMARK_TEMPLATE(n, p)

constexpr size_t startRange = 1 << 7; // 32 bytes
constexpr size_t endRange = 1 << 12;  // 128 MiB

class alignas(16) Align16
{
public:
    size_t data[16 / sizeof(size_t)]; // NOLINT(modernize-avoid-c-arrays)

    Align16() noexcept = default;
};

class alignas(32) Align32
{
public:
    size_t data[32 / sizeof(size_t)]; // NOLINT(modernize-avoid-c-arrays)

    Align32() noexcept = default;
};

class alignas(64) Align64
{
public:
    size_t data[64 / sizeof(size_t)]; // NOLINT(modernize-avoid-c-arrays)

    Align64() noexcept = default;
};

class alignas(128) Align128
{
public:
    size_t data[128 / sizeof(size_t)]; // NOLINT(modernize-avoid-c-arrays)

    Align128() noexcept = default;
};

using Align1 = uint8_t;
using Align2 = uint16_t;
using Align4 = uint32_t;
using Align8 = uint64_t;

template<typename T>
XS_CONSTEVAL size_t maxSize(const size_t size)
{
    return size > sizeof(T) ? size : sizeof(T);
}

#define ENABLE_MEMMOVE_TEST                                                \
    (XS_BENCH_MEMMOVE &&                                                   \
        ((XS_BENCH_MEMMOVE_SSE2 && XS_ARCH_SSE2 && !XS_ARCH_AVX) ||        \
            (XS_BENCH_MEMMOVE_AVX2 && XS_ARCH_AVX2 && !XS_ARCH_AVX512F) || \
            (XS_BENCH_MEMMOVE_IA32 && XS_IGNORE_ISA_OPT) || (XS_BENCH_MEMMOVE_AVX512 && XS_ARCH_AVX512F)))
#define ENABLE_MEMMOVEBACK_TEST                                                \
    (XS_BENCH_MEMMOVEBACK &&                                                   \
        ((XS_BENCH_MEMMOVEBACK_SSE2 && XS_ARCH_SSE2 && !XS_ARCH_AVX) ||        \
            (XS_BENCH_MEMMOVEBACK_AVX2 && XS_ARCH_AVX2 && !XS_ARCH_AVX512F) || \
            (XS_BENCH_MEMMOVEBACK_IA32 && XS_IGNORE_ISA_OPT) || (XS_BENCH_MEMMOVEBACK_AVX512 && XS_ARCH_AVX512F)))
#ifndef XSBENCHMAIN
#    if ENABLE_MEMMOVE_TEST || ENABLE_MEMMOVEBACK_TEST
#        include "../tests/XSCompilerOptions.h"

#        define XS_OVERRIDE_SHIFT_NS TESTISA(MemoryTest)

namespace XS_OVERRIDE_SHIFT_NS {
#        include "Memory/XSMemory.hpp"
}

using namespace XS_OVERRIDE_SHIFT_NS;
using namespace XS_OVERRIDE_SHIFT_NS::Shift;

#        if ENABLE_MEMMOVE_TEST
#            define BENCH_NAME TESTISA(memMove)

template<typename T>
void TESTISA(memMove)(benchmark::State& state)
{
    T* srcRegion = new T[state.range(0) + 1];
    T* dstRegion = new T[state.range(0) + 1];
    // force alignment to be exactly (and no more than requested)
    T* src = srcRegion;
    if (reinterpret_cast<size_t>(srcRegion) % (2 * alignof(T)) == 0) {
        src = srcRegion + 1;
    }
    T* dst = dstRegion;
    if (reinterpret_cast<size_t>(dstRegion) % (2 * alignof(T)) == 0) {
        dst = dstRegion + 1;
    }
    for (auto _ : state) {
        memMove(dst, src, state.range(0));
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
    delete[] srcRegion;
    delete[] dstRegion;
}
#        endif
#        if ENABLE_MEMMOVEBACK_TEST
template<typename T>
void TESTISA(memMoveBack)(benchmark::State& state)
{
    T* srcRegion = new T[state.range(0) + 1];
    T* dstRegion = new T[state.range(0) + 1];
    // force alignment to be exactly (and no more than requested)
    T* src = srcRegion;
    if (reinterpret_cast<size_t>(srcRegion) % (2 * alignof(T)) == 0) {
        src = srcRegion + 1;
    }
    T* dst = dstRegion;
    if (reinterpret_cast<size_t>(dstRegion) % (2 * alignof(T)) == 0) {
        dst = dstRegion + 1;
    }
    for (auto _ : state) {
        memMoveBackwards(dst, src, state.range(0));
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
    delete[] srcRegion;
    delete[] dstRegion;
}
#        endif
#    endif
#else
#    if XS_BENCH_MEMMOVE_CLIB
#        define BENCH_NAME memMove_clib

template<typename T>
void memMove_clib(benchmark::State& state)
{
    T* srcRegion = new T[state.range(0) + 1];
    T* dstRegion = new T[state.range(0) + 1];
    // force alignment to be exactly (and no more than requested)
    T* src = srcRegion;
    if (reinterpret_cast<size_t>(srcRegion) % (2 * alignof(T)) == 0) {
        src = srcRegion + 1;
    }
    T* dst = dstRegion;
    if (reinterpret_cast<size_t>(dstRegion) % (2 * alignof(T)) == 0) {
        dst = dstRegion + 1;
    }
    for (auto _ : state) {
        memmove(dst, src, state.range(0));
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
    delete[] srcRegion;
    delete[] dstRegion;
}
#    endif
#endif

#if (ENABLE_MEMMOVE_TEST && !defined(XSBENCHMAIN)) || (XS_BENCH_MEMMOVE_CLIB && defined(XSBENCHMAIN))
BENCHMARK_TEMPLATET(BENCH_NAME, Align1)->RangeMultiplier(2)->Range(maxSize<Align1>(startRange), endRange);
BENCHMARK_TEMPLATET(BENCH_NAME, Align2)->RangeMultiplier(2)->Range(maxSize<Align2>(startRange), endRange);
BENCHMARK_TEMPLATET(BENCH_NAME, Align4)->RangeMultiplier(2)->Range(maxSize<Align4>(startRange), endRange);
BENCHMARK_TEMPLATET(BENCH_NAME, Align8)->RangeMultiplier(2)->Range(maxSize<Align8>(startRange), endRange);
BENCHMARK_TEMPLATET(BENCH_NAME, Align16)->RangeMultiplier(2)->Range(maxSize<Align16>(startRange), endRange);
BENCHMARK_TEMPLATET(BENCH_NAME, Align32)->RangeMultiplier(2)->Range(maxSize<Align32>(startRange), endRange);
BENCHMARK_TEMPLATET(BENCH_NAME, Align64)->RangeMultiplier(2)->Range(maxSize<Align64>(startRange), endRange);
BENCHMARK_TEMPLATET(BENCH_NAME, Align128)->RangeMultiplier(2)->Range(maxSize<Align128>(startRange), endRange);
#endif
#if ENABLE_MEMMOVEBACK_TEST && !defined(XSBENCHMAIN)
BENCHMARK_TEMPLATET(TESTISA(memMoveBack), Align1)->RangeMultiplier(2)->Range(maxSize<Align1>(startRange), endRange);
BENCHMARK_TEMPLATET(TESTISA(memMoveBack), Align2)->RangeMultiplier(2)->Range(maxSize<Align2>(startRange), endRange);
BENCHMARK_TEMPLATET(TESTISA(memMoveBack), Align4)->RangeMultiplier(2)->Range(maxSize<Align4>(startRange), endRange);
BENCHMARK_TEMPLATET(TESTISA(memMoveBack), Align8)->RangeMultiplier(2)->Range(maxSize<Align8>(startRange), endRange);
BENCHMARK_TEMPLATET(TESTISA(memMoveBack), Align16)->RangeMultiplier(2)->Range(maxSize<Align16>(startRange), endRange);
BENCHMARK_TEMPLATET(TESTISA(memMoveBack), Align32)->RangeMultiplier(2)->Range(maxSize<Align32>(startRange), endRange);
BENCHMARK_TEMPLATET(TESTISA(memMoveBack), Align64)->RangeMultiplier(2)->Range(maxSize<Align64>(startRange), endRange);
BENCHMARK_TEMPLATET(TESTISA(memMoveBack), Align128)->RangeMultiplier(2)->Range(maxSize<Align128>(startRange), endRange);
#endif
