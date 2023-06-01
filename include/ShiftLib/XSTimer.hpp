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

#include "SIMD/XSSIMDInBase.hpp"
#include "XSArchitecture.hpp"

#include <profileapi.h>

namespace Shift {
class Timer
{
    uint64 startTime;
    uint64 frequency;
    uint64 pausedTime;

public:
    /** Default constructor. */
    XS_INLINE Timer() noexcept
    {
        QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startTime));
        pausedTime = 0;
    }

    /** Resets the timer back to zero. */
    XS_INLINE void resetTime() noexcept
    {
        XS_ASSERT(pausedTime == 0);
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startTime));
    }

    /**
     * Gets the current timer value.
     * @return Timer value in microseconds.
     */
    XS_INLINE [[nodiscard]] uint64 getTime() const noexcept
    {
        XS_ASSERT(pausedTime == 0);
        uint64 currentTime;
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentTime));
        uint64 elapsedTime(currentTime - startTime);
        // Convert to microseconds
        elapsedTime *= 1000000;
        elapsedTime /= frequency;
        return elapsedTime;
    }

    /**
     * Gets the current absolute timer value.
     * @return Timer value in seconds.
     */
    XS_INLINE [[nodiscard]] uint64 getWallTime() const noexcept
    {
        XS_ASSERT(pausedTime == 0);
        uint64 currentTime;
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentTime));
        // Convert to microseconds
        currentTime *= 1000000;
        currentTime /= frequency;
        return currentTime;
    }

    /**
     * Gets the elapsed time since last timer check.
     * @note This will automatically reset the timer value to 0 afterward.
     * @return Elapsed timer value in microseconds.
     */
    XS_INLINE [[nodiscard]] uint64 getTimeElapsed() noexcept
    {
        XS_ASSERT(pausedTime == 0);
        uint64 currentTime;
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentTime));
        uint64 elapsedTime(currentTime - startTime);
        startTime = currentTime;
        // Convert to microseconds
        elapsedTime *= 1000000;
        elapsedTime /= frequency;
        return elapsedTime;
    }

    /**
     * Pause the current timer
     * @note This will prevent the timer for counting further from this time onwards.
     */
    XS_INLINE void pauseTimer() noexcept
    {
        XS_ASSERT(pausedTime == 0);
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&pausedTime));
    }

    /**
     * Resumes the timer from a paused state
     * @note This will cause the timer to continue counting from the time it was paused.
     */
    XS_INLINE void resumeTimer() noexcept
    {
        // get time elapsed since last paused
        uint64_t currentTime;
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentTime));
        // add paused time to start time
        startTime += (currentTime - pausedTime);
#ifdef _DEBUG
        pausedTime = 0;
#endif
    }
};
} // namespace Shift
