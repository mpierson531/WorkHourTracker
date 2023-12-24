//
// Created by Micah on 12/22/2023.
//

#ifndef WORKHOURTRACKER_TIMESEGMENT_HPP
#define WORKHOURTRACKER_TIMESEGMENT_HPP

#include "../gutils/ENUM_CLASS.hpp"
#include "../gutils/Result.hpp"
using gutils::Result;

namespace gtiming {
    ENUM_CLASS(TimeSegment, uint8_t, AM, PM)
    PUBLIC
        TimeSegment() : value(AM) { }

        [[nodiscard]] constexpr TimeSegment opposite() const {
            return *this == AM ? PM : AM;
        }

        [[nodiscard]] constexpr string toString() const {
            switch ((uint8_t) *this) {
                case AM:
                    return "AM";
                case PM:
                    return "PM";
                default:
                    throw std::runtime_error{"Exceptional time segment: " + gutils::toString(((uint8_t) *this))};
            }
        }

        [[nodiscard]] static Result<TimeSegment, string> fromString(const std::string_view& source) {
            if (source == "AM")
                return Result<TimeSegment, string>::okOf(AM);

            if (source == "PM")
                return Result<TimeSegment, string>::okOf(PM);

            return Result<TimeSegment, string>::errOf("Could not parse 'AM' or 'PM' from source.");
        }
    END
    }

#endif //WORKHOURTRACKER_TIMESEGMENT_HPP