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

#include "Memory/XSAllocatorStack.hpp"
#include "Memory/XSArray.hpp"

namespace Shift {
/**
 * Static Array template class used to store sections of memory with a fixed size.
 * @tparam T           Type of element stored within array.
 * @tparam NumElements Number of elements of type 'T' to allocate memory for.
 */
template<class T, uint0 NumElements>
using StaticArray = Array<T, AllocRegionStack<T, NumElements>>;
} // namespace Shift
