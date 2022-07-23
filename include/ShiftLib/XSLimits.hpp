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

#include <cfloat>
#include <climits>

namespace Shift {
template<typename T>
class Limits
{
public:
    static_assert(isArithmetic<T>, "Invalid Type: Only arithmetic types supported");

    using Type = removeCV<T>;

    /**
     * Get the smallest possible value representable by the type.
     * @returns The requested value.
     */
    XS_INLINE static constexpr Type Min() noexcept
    {
        if constexpr (isSame<T, bool>) {
            return false;
        } else if constexpr (isSame<T, int8>) {
            return (-128);
        } else if constexpr (isSame<T, uint8>) {
            return 0;
        } else if constexpr (isSame<T, int16>) {
            return (-32768);
        } else if constexpr (isSame<T, uint16>) {
            return 0;
        } else if constexpr (isSame<T, int32>) {
            return (-2147483647 - 1);
        } else if constexpr (isSame<T, uint32>) {
            return 0;
        } else if constexpr (isSame<T, int64>) {
            return (-9223372036854775807i64 - 1);
        } else if constexpr (isSame<T, uint64>) {
            return 0;
        } else if constexpr (isSame<T, float32>) {
            return 1.175494351e-38f;
        } else if constexpr (isSame<T, float64>) {
            return 2.2250738585072014e-308;
        } else if constexpr (isSame<T, char16>) {
            return 0;
        } else if constexpr (isSame<T, char32>) {
            return 0;
        }
#if defined(__cpp_char8_t)
        else if constexpr (isSame<T, char8>) {
            return 0;
        }
#endif
        else {
            return Type();
        }
    }

    /**
     * Get the largest possible value representable by the type.
     * @returns The requested value.
     */
    XS_INLINE static constexpr Type Max() noexcept
    {
        if constexpr (isSame<T, bool>) {
            return true;
        } else if constexpr (isSame<T, int8>) {
            return 127;
        } else if constexpr (isSame<T, uint8>) {
            return 0xff;
        } else if constexpr (isSame<T, int16>) {
            return 32767;
        } else if constexpr (isSame<T, uint16>) {
            return 0xffff;
        } else if constexpr (isSame<T, int32>) {
            return 2147483647;
        } else if constexpr (isSame<T, uint32>) {
            return 0xffffffff;
        } else if constexpr (isSame<T, int64>) {
            return 9223372036854775807i64;
        } else if constexpr (isSame<T, uint64>) {
            return 0xffffffffffffffffui64;
        } else if constexpr (isSame<T, float32>) {
            return 3.402823466e+38F;
        } else if constexpr (isSame<T, float64>) {
            return 1.7976931348623158e+308;
        } else if constexpr (isSame<T, char16>) {
            return 0xffff;
        } else if constexpr (isSame<T, char32>) {
            return 0xffffffff;
        }
#if defined(__cpp_char8_t)
        else if constexpr (isSame<T, char8>) {
            return 0xff;
        }
#endif
        else {
            return Type();
        }
    }

    /**
     * Get the lowest possible value representable by the type.
     * @remark This differs from the smallest in that it is the largest negative value the type can represent.
     * @returns The requested value.
     */
    XS_INLINE static constexpr Type Lowest() noexcept
    {
        if constexpr (isSame<T, bool>) {
            return false;
        } else if constexpr (isSame<T, int8>) {
            return (-128);
        } else if constexpr (isSame<T, uint8>) {
            return 0;
        } else if constexpr (isSame<T, int16>) {
            return (-32768);
        } else if constexpr (isSame<T, uint16>) {
            return 0;
        } else if constexpr (isSame<T, int32>) {
            return (-2147483647 - 1);
        } else if constexpr (isSame<T, uint32>) {
            return 0;
        } else if constexpr (isSame<T, int64>) {
            return (-9223372036854775807i64 - 1);
        } else if constexpr (isSame<T, uint64>) {
            return 0;
        } else if constexpr (isSame<T, float32>) {
            return -3.402823466e+38F;
        } else if constexpr (isSame<T, float64>) {
            return -1.7976931348623158e+308;
        } else if constexpr (isSame<T, char16>) {
            return 0;
        } else if constexpr (isSame<T, char32>) {
            return 0;
        }
#if defined(__cpp_char8_t)
        else if constexpr (isSame<T, char8>) {
            return 0;
        }
#endif
        else {
            return Type();
        }
    }

    /**
     * Get the smallest value that when added to another will result in an change in value.
     * @returns The requested value.
     */
    XS_INLINE static constexpr Type Epsilon() noexcept
    {
        if constexpr (isSame<T, bool>) {
            return 1;
        } else if constexpr (isSame<T, int8>) {
            return 1;
        } else if constexpr (isSame<T, uint8>) {
            return 1;
        } else if constexpr (isSame<T, int16>) {
            return 1;
        } else if constexpr (isSame<T, uint16>) {
            return 1;
        } else if constexpr (isSame<T, int32>) {
            return 1;
        } else if constexpr (isSame<T, uint32>) {
            return 1;
        } else if constexpr (isSame<T, int64>) {
            return 1;
        } else if constexpr (isSame<T, uint64>) {
            return 1;
        } else if constexpr (isSame<T, float32>) {
            return 1.192092896e-07F;
        } else if constexpr (isSame<T, float64>) {
            return 2.2204460492503131e-016;
        } else if constexpr (isSame<T, char16>) {
            return 1;
        } else if constexpr (isSame<T, char32>) {
            return 1;
        }
#if defined(__cpp_char8_t)
        else if constexpr (isSame<T, char8>) {
            return 1;
        }
#endif
        else {
            return Type();
        }
    }
};
} // namespace Shift
