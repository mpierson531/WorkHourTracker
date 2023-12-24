//
// Created by Micah on 10/21/2023.
//

#ifndef CPP_TESTING_LEARNING_TIMING_HPP
#define CPP_TESTING_LEARNING_TIMING_HPP

#include "../gutils/gutils.hpp"
#include "TimeSegment.hpp"

#include <chrono>
using std::chrono::high_resolution_clock, std::chrono::steady_clock, std::chrono::duration;

namespace gtiming {
    enum class TimeUnit {
        Nano,
        Milli,
        Second
    };

    static constexpr float NANO_TO_MILLI = 1.0f / 1000.0f;
    static constexpr float NANO_TO_SECOND = 1.0f / 1000000000.0f;

    static constexpr float MILLI_TO_NANO = 1000.0f;
    static constexpr float MILLI_TO_SECOND = 1.0f / 1000.0f;

    static constexpr float SECOND_TO_MILLI = 1000.0f;
    static constexpr float SECOND_TO_NANO = 1000000000.0f;

    template<gutils::number T = double, gutils::floating_point R = double>
    constexpr R minutesToPercent(const T minutes) {
        return (R) minutes / 60.0;
    }

    template<gutils::number T = double, gutils::floating_point R = double>
    constexpr R percentToMinutes(const T percent) {
        return (R) percent * 60.0;
    }

    template<gutils::number T = double, gutils::floating_point R = double>
    constexpr R hoursToPercent(const T hours) {
        return (R) hours / 24.0;
    }

    template<gutils::number T = double, gutils::floating_point R = double>
    constexpr R percentToHours(const T percent) {
        return (R) percent * 24.0;
    }

    template<TimeSegment::Value segment, gutils::number T = uint8_t, gutils::number R = uint8_t>
    constexpr R twelveToTwentyFour(const T value) {
        if (value == 12) {
            if constexpr (segment == TimeSegment::AM) {
                return (R) 0;
            }

            return (R) 12;
        }

        return (R) value + (R) 12;
    }

    template<gutils::number T = uint8_t, gutils::number R = uint8_t>
    constexpr R twentyFourToTwelve(const T value) {
        if (value == 24) {
            return (R) 0;
        }

        if (value <= 12) {
            return (R) value;
        }

        return (R) value - (R) 12;
    }

    template<gutils::floating_point FloatType>
    FloatType toNanos(const TimeUnit fromUnit, const FloatType value) {
        switch (fromUnit) {
            case TimeUnit::Nano: return value;
            case TimeUnit::Milli: return value * MILLI_TO_NANO;
            case TimeUnit::Second: return value * SECOND_TO_NANO;
        }
    }

    template<gutils::floating_point FloatType>
    FloatType toMillis(const TimeUnit fromUnit, const FloatType value) {
        switch (fromUnit) {
            case TimeUnit::Nano: return value * NANO_TO_MILLI;
            case TimeUnit::Milli: return value;
            case TimeUnit::Second: return value * SECOND_TO_MILLI;
        }
    }

    template<gutils::floating_point FloatType>
    FloatType toSeconds(const TimeUnit fromUnit, const FloatType value) {
        switch (fromUnit) {
            case TimeUnit::Nano: return value * NANO_TO_SECOND;
            case TimeUnit::Milli: return value * MILLI_TO_SECOND;
            case TimeUnit::Second: return value;
        }
    }

    template<gutils::floating_point FloatType>
    FloatType rawTimeNanos() {
        return duration<FloatType, std::nano>(steady_clock::now().time_since_epoch()).count();
    }

    template<gutils::floating_point FloatType>
    FloatType rawTimeMillis() {
        return duration<FloatType, std::milli>(steady_clock::now().time_since_epoch()).count();
    }

    template<gutils::floating_point FloatType>
    FloatType rawTimeSecs() {
        return duration<FloatType, std::milli>(steady_clock::now().time_since_epoch()).count() * MILLI_TO_SECOND;
    }
}

#endif //CPP_TESTING_LEARNING_TIMING_HPP