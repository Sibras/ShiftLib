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

#define XS_SCALAR 0      /**< Use standard scalar non-vector operations. */
#define XS_SSE3 1        /**< Use x86 SSE3 128bit vector operations. */
#define XS_SSE41 2       /**< Use x86 SSE4.1 128bit vector operations. */
#define XS_SSE42 3       /**< Use x86 SSE4.2 128bit vector operations. */
#define XS_AVX 4         /**< Use x86 AVX 256bit vector operations. */
#define XS_AVX_128 5     /**< Use x86 AVX vector operations but limited to 128bit registers. */
#define XS_AVX2 6        /**< Use x86 AVX2 256bit vector operations. */
#define XS_AVX2_128 7    /**< Use x86 AVX2 vector operations but limited to 128bit registers. */
#define XS_AVX512 8      /**< Use x86 AVX512 512bit vector operations. */
#define XS_AVX512_128 9  /**< Use x86 AVX512 vector operations but limited to 128bit registers. */
#define XS_AVX512_256 10 /**< Use x86 AVX512 vector operations but limited to 256bit registers. */

/**
 * Sets the use of any vector instructions that should be used as default. This can be used to improve performance
 * provided the host computer can support the requested instructions. This is used to set the default optimisation that
 * will be used should one not be explicitly specified. Default: XS_AVX2_128 (7)
 */
#ifndef XS_SIMD
#    define XS_SIMD XS_AVX2_128
#endif

/**
 * Sets the use of faster but less precise floating point operations. This will enable several faster mathematical
 * approximations such as the faster reciprocal operations when using divide operations. i.e. replaces a / b -> a *
 * (1/b). When set to precise will use slower but more precise IEEE compliant methods. Default: XS_FFAST (1)
 */
#define XS_FPRECISE 0 /**< Use standard IEEE numerical precision operations. */
#define XS_FFAST 1    /**< Use faster optimised operations with slight increase in error. */
#define XS_FFAST2 2   /**< Use fastest operations with even increased error. */

#ifndef XS_FPRECISION
#    define XS_FPRECISION XS_FFAST
#endif
