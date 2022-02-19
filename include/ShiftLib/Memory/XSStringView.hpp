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
#include "XSCompiler.h"

#include <string_view>

namespace Shift {

template<typename T>
struct StringViewHelper
{
    using Type = std::string_view;
};
#ifdef __cpp_char8_t
template<>
struct StringViewHelper<char8>
{
    using Type = std::u8string_view;
};
#endif
template<>
struct StringViewHelper<char16>
{
    using Type = std::u16string_view;
};
template<>
struct StringViewHelper<char32>
{
    using Type = std::u32string_view;
};

template<typename T>
using StringView = typename StringViewHelper<T>::Type;

XS_INLINE constexpr StringView<char> operator"" _sv(const char* string, const uint0 length) noexcept
{
    return StringView<char>(string, length);
}

#ifdef __cpp_char8_t
XS_INLINE constexpr StringView<char8> operator"" _sv(const char8* string, const uint0 length) noexcept
{
    return StringView<Shift::char8>(string, length);
}
#endif // __cpp_char8_t

XS_INLINE constexpr StringView<char16> operator"" _sv(const char16* string, const uint0 length) noexcept
{
    return StringView<char16>(string, length);
}

XS_INLINE constexpr StringView<char32> operator"" _sv(const char32* string, const uint0 length) noexcept
{
    return StringView<char32>(string, length);
}
} // namespace Shift
