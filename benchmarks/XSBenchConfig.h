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

/** A macro that defines whether the default c runtime memmove function should be benched. */
#define XS_BENCH_MEMMOVE_CLIB 0

/** A macro that defines whether the memMove function should be benched. */
#define XS_BENCH_MEMMOVE 1

/** A macro that defines whether the memMove function should be benched in IA32 configuration. */
#define XS_BENCH_MEMMOVE_IA32 1

/** A macro that defines whether the memMove function should be benched in SSE2 configuration. */
#define XS_BENCH_MEMMOVE_SSE2 1

/** A macro that defines whether the memMove function should be benched in AVX2 configuration. */
#define XS_BENCH_MEMMOVE_AVX2 1

/** A macro that defines whether the memMove function should be benched in AVX512 configuration. */
#define XS_BENCH_MEMMOVE_AVX512 1

/** A macro that defines whether the memMoveBackwards function should be benched. */
#define XS_BENCH_MEMMOVEBACK 1

/** A macro that defines whether the memMoveBackwards function should be benched in IA32 configuration. */
#define XS_BENCH_MEMMOVEBACK_IA32 1

/** A macro that defines whether the memMoveBackwards function should be benched in SSE2 configuration. */
#define XS_BENCH_MEMMOVEBACK_SSE2 1

/** A macro that defines whether the memMoveBackwards function should be benched in AVX2 configuration. */
#define XS_BENCH_MEMMOVEBACK_AVX2 1

/** A macro that defines whether the memMoveBackwards function should be benched in AVX512 configuration. */
#define XS_BENCH_MEMMOVEBACK_AVX512 1
