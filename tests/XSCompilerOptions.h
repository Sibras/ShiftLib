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

#include "XSCompiler.h"

#if XS_ARCH_AVX512F
#    define XS_COMPILER_ARCH AVX512
#elif XS_ARCH_AVX2
#    define XS_COMPILER_ARCH AVX2
#elif XS_ARCH_AVX
#    define XS_COMPILER_ARCH AVX
#elif XS_ARCH_SSE4_2
#    define XS_COMPILER_ARCH SSE42
#elif XS_ARCH_SSE4_1
#    define XS_COMPILER_ARCH SSE41
#elif XS_ARCH_SSE3
#    define XS_COMPILER_ARCH SSE3
#elif XS_ISA == XS_X86
#    define XS_COMPILER_ARCH IA32
#else
#    define XS_COMPILER_ARCH Scalar
#endif

#define CONCATTOKENIMPL(foo, bar) foo##bar
#define CONCATTOKEN(foo, bar) CONCATTOKENIMPL(foo, bar)

#define STRINGIFYVALUE(x) #x
#define STRINGIFY(y) STRINGIFYVALUE(y)

#define TESTISA(name) CONCATTOKEN(name, XS_COMPILER_ARCH)
