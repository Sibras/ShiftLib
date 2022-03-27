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

namespace Shift {
template<typename T>
class Expected
{
    using Type = T;

    union
    {
        Type value;
        char c = 0;
    };

    bool valid = false; /**< Specifies if 'value' contains valid data */

public:
    /** Default constructor. */
    constexpr Expected() = default;

    /**
     * Constructor.
     * @param valueIn The value to use.
     */
    template<typename = require<isNothrowCopyConstructible<T>>>
    XS_REQUIRES((isNothrowCopyConstructible<T>))
    constexpr explicit Expected(const Type& valueIn) noexcept
        : value(valueIn)
        , valid(true)
    {}

    /**
     * Constructor.
     * @param valueIn The value to use.
     */
    template<typename = require<isNothrowMoveConstructible<T>>>
    XS_REQUIRES((isNothrowMoveConstructible<T>))
    constexpr explicit Expected(Type&& valueIn) noexcept
        : value(forward<Type>(valueIn))
        , valid(true)
    {}

    /**
     * Constructor from type args.
     * @param valueIn The values to use.
     */
    template<typename... Args, typename = require<isNothrowConstructible<T, Args...>>>
    XS_REQUIRES((isNothrowConstructible<T, Args...>))
    constexpr explicit Expected(Args&&... valueIn) noexcept
        : value(forward<Args>(valueIn)...)
        , valid(true)
    {}

    /**
     * Constructor.
     * @param other The other.
     */
    constexpr Expected(const Expected& other) noexcept = default;

    /**
     * Forwarding Constructor.
     * @param other The other.
     */
    constexpr Expected(Expected&& other) noexcept = default;

    /** Destructor. */
    constexpr ~Expected() noexcept
    {
        if (valid) {
            value.~T();
        }
    }

    /**
     * Assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    template<typename = require<isNothrowCopyAssignable<T>>>
    XS_REQUIRES((isNothrowCopyAssignable<T>))
    constexpr Expected& operator=(const Expected& other) noexcept
    {
        if (valid) {
            if (other.valid) {
                this->value = other.value;
            } else {
                value.~T();
                valid = false;
            }
        } else {
            if (other.valid) {
                valid = true;
                new (&value) Type(other.value);
            }
        }
        return *this;
    }

    /**
     * Move assignment operator.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    template<typename = require<isNothrowMoveAssignable<T>>>
    XS_REQUIRES((isNothrowMoveAssignable<T>))
    constexpr Expected& operator=(Expected&& other) noexcept
    {
        if (valid) {
            if (other.valid) {
                swap(this->value, other.value);
            } else {
                valid = false;
                new (&other.value) Type(forward<Type>(value));
                other.valid = true;
            }
        } else {
            if (other.valid) {
                valid = true;
                new (&value) Type(forward<Type>(other.value));
                other.valid = false;
            }
        }
        return *this;
    }

    /**
     * Assignment operator directly on value type.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    template<typename = require<isNothrowCopyAssignable<T>>>
    XS_REQUIRES((isNothrowCopyAssignable<T>))
    constexpr Expected& operator=(const Type& other) noexcept
    {
        if (valid) {
            this->value = other.value;
        } else {
            valid = true;
            new (&value) Type(other.value);
        }
        return *this;
    }

    /**
     * Move assignment operator directly on value type.
     * @param other The other.
     * @returns A shallow copy of this object.
     */
    template<typename = require<isNothrowMoveAssignable<T>>>
    XS_REQUIRES((isNothrowMoveAssignable<T>))
    constexpr Expected& operator=(Type&& other) noexcept
    {
        if (valid) {
            swap(this->value, other.value);
        } else {
            valid = true;
            new (&value) Type(forward<Type>(other.value));
        }
        return *this;
    }

    /**
     * Swap data between objects.
     * @param other The other.
     */
    template<typename = require<isNothrowCopyAssignable<T> && isNothrowMoveConstructible<T>>>
    XS_REQUIRES((isNothrowCopyAssignable<T> && isNothrowMoveConstructible<T>))
    constexpr void swap(Expected& other) noexcept
    {
        *this = forward<Expected>(other);
    }

    /**
     * Create a Expected set to an invalid value.
     * @return The request Expected.
     */
    static constexpr Expected Fail() noexcept
    {
        return Expected();
    }

    /**
     * Query if this object has valid internal value.
     * @returns True if valid, false if not.
     */
    [[nodiscard]] constexpr bool isValid() const noexcept
    {
        return valid;
    }

    /**
     * Get the internal value.
     * @note Care should be taken to only call this when the internal value is valid. Behaviour is undefined otherwise.
     * @returns The internal value.
     */
    [[nodiscard]] constexpr const Type& getValue() const&
    {
        XS_ASSERT(valid);
        return value;
    }

    /**
     * Get the internal value.
     * @note Care should be taken to only call this when the internal value is valid. Behaviour is undefined otherwise.
     * @returns The internal value.
     */
    [[nodiscard]] constexpr Type& getValue() &
    {
        XS_ASSERT(valid);
        return value;
    }

    /**
     * Get the internal value.
     * @note Care should be taken to only call this when the internal value is valid. Behaviour is undefined otherwise.
     * @returns The internal value.
     */
    [[nodiscard]] constexpr Type&& getValue() &&
    {
        XS_ASSERT(valid);
        return move(value);
    }

    /**
     * Query if this object has valid internal value.
     * @returns True if valid, false if not.
     */
    [[nodiscard]] explicit constexpr operator bool() const noexcept
    {
        return valid;
    }

    /**
     * Get the internal value.
     * @note Care should be taken to only call this when the internal value is valid. Behaviour is undefined otherwise.
     * @returns The internal value.
     */
    [[nodiscard]] constexpr const Type& operator*() const&
    {
        return getValue();
    }

    /**
     * Get the internal value.
     * @note Care should be taken to only call this when the internal value is valid. Behaviour is undefined otherwise.
     * @returns The internal value.
     */
    [[nodiscard]] constexpr Type& operator*() &
    {
        return getValue();
    }

    /**
     * Get the internal value.
     * @note Care should be taken to only call this when the internal value is valid. Behaviour is undefined otherwise.
     * @returns The internal value.
     */
    [[nodiscard]] constexpr Type&& operator*() &&
    {
        return move(getValue());
    }
};
} // namespace Shift
