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

/**
 * Sets whether assert checks should always be performed even when not in debug mode. Default: No (0)
 */
#define XS_ALWAYS_ASSERT 1
