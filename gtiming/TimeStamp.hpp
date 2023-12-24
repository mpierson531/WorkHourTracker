//
// Created by Micah on 12/18/2023.
//

#ifndef WORKHOURTRACKER_TIMESTAMP_HPP
#define WORKHOURTRACKER_TIMESTAMP_HPP

#include "../gutils/ENUM_CLASS.hpp"
#include "../gutils/Result.hpp"
#include "../gutils/gutils.hpp"
#include "timing.hpp"
#include "TimeSegment.hpp"
using gutils::Result;
using gtiming::minutesToPercent, gtiming::percentToMinutes;
using gtiming::hoursToPercent, gtiming::percentToHours;
using gtiming::twelveToTwentyFour, gtiming::twentyFourToTwelve;
using gtiming::TimeSegment;

#include "TimeFormat.hpp"

#include <string>
#include <format>
#include <string_view>
using std::string, std::string_view;

// TODO: FULLY IMPLEMENT AND SUPPORT BOTH TWELVE AND TWENTY_FOUR-HOUR FORMATS

// TODO: POSSIBLY OVERLOAD EQUALS FOR CHECKING/CONVERTING TWELVE AND TWENTY_FOUR-HOUR FORMATS

// TODO: POSSIBLY MAKE m_format A TEMPLATE PARAMETER

namespace gtiming {
    template<gutils::number T>
    struct TimeStamp {
    private:
        TimeFormat m_format;
        TimeSegment m_timeSegment;
        T m_hours;
        T m_minutes;

        consteval bool hasSameType(const TimeStamp& other) const {
            return typeid(m_hours) == typeid(other.m_hours);
        }

        constexpr bool hasSameSegment(const TimeStamp& other) const {
            return m_timeSegment == other.m_timeSegment;
        }

        constexpr bool hoursMinutesEq(const TimeStamp& other) const {
            return m_hours == other.m_hours && m_minutes == other.m_minutes;
        }

        template<gutils::number N1, gutils::number N2>
        constexpr bool hoursMinutesEq(const N1 hours, const N2 minutes) const {
            return m_hours == hours && m_minutes == minutes;
        }

    public:
        template<gutils::number N1, gutils::number N2>
        constexpr TimeStamp(const TimeFormat format, const TimeSegment timeSegment, const N1 hours, const N2 minutes)
                : m_format(format), m_timeSegment(timeSegment), m_hours(static_cast<T>(hours)), m_minutes(static_cast<T>(minutes)) {}

        constexpr TimeStamp()
                : m_format(TimeFormat::TWELVE_HOUR), m_timeSegment(TimeSegment::AM), m_hours(-1), m_minutes(-1) { }

        [[nodiscard]] constexpr bool operator==(const TimeStamp& other) const {
            return totalPercent() == other.totalPercent();
        }

        [[nodiscard]] constexpr TimeFormat format() const {
            return m_format;
        }

        [[nodiscard]] constexpr TimeSegment timeSegment() const {
            return m_timeSegment;
        }

        [[nodiscard]] constexpr T hours() const {
            return m_hours;
        }

        [[nodiscard]] constexpr T minutes() const {
            return m_minutes;
        }

        [[nodiscard]] constexpr bool isTwelveHour() const {
            return m_format == TimeFormat::TWELVE_HOUR;
        }

        [[nodiscard]] constexpr bool isTwentyFourHour() const {
            return m_format == TimeFormat::TWENTY_FOUR_HOUR;
        }

        [[nodiscard]] constexpr bool isAM() const {
            return m_timeSegment == TimeSegment::AM;
        }

        [[nodiscard]] constexpr bool isPM() const {
            return m_timeSegment == TimeSegment::PM;
        }

        [[nodiscard]] constexpr bool isZero() const {
            if (m_hours == 0 && m_minutes == 0) {
                return true;
            }

            if (isAM() && m_hours == 12 && m_minutes == 0) {
                return true;
            }

            return false;
        }

        // TODO: MAYBE RE-VISIT
        [[nodiscard]] constexpr double hoursPercent() const {
            if (isTwelveHour()) {
                if (isPM()) {
                    return hoursToPercent<T>(twelveToTwentyFour<TimeSegment::PM>(m_hours));
                }

                if (m_hours == 12) {
                    return 0.0;
                }
            }

            return hoursToPercent<T>(m_hours);

            /*if (isPM()) {
                if (isTwentyFourHour()) {
                    return hoursToPercent<T>(m_hours);
                }

                return hoursToPercent<T>(twelveToTwentyFour<TimeSegment::PM>(m_hours));
            }

            if (isTwelveHour()) {
                return hoursToPercent<T>(twelveToTwentyFour<TimeSegment::AM>(m_hours));
            }

            if (m_timeSegment == TimeSegment::PM) {
                return hoursToPercent<T>(twelveToTwentyFour<TimeSegment::PM>(m_hours));
            }

            if (m_hours == 12) {
                return 0.0;
            }

            return hoursToPercent<T>(m_hours);*/
        }

        [[nodiscard]] constexpr double minutesPercent() const {
            return minutesToPercent<T>(m_minutes);
        }

        [[nodiscard]] constexpr double totalPercent() const {
            return hoursPercent() + hoursToPercent<double>(minutesPercent());
        }

        constexpr TimeStamp* flip() {
            m_timeSegment = m_timeSegment.opposite();
            return this;
        }

        [[nodiscard]] constexpr TimeStamp flipCopy() const {
            return TimeStamp {m_format, m_timeSegment.opposite(), m_hours, m_minutes};
        }

        // TODO: HANDLE CASES:
        //  SAME TIME_SEGMENT AND SAME TIME (ZERO RESULT),
        //  SAME TIME_SEGMENT AND FIRST TimeStamp HAS A LATER TIME (NEGATIVE RESULT)

        // TODO: SUPPORT TWENTY_FOUR-HOUR HERE
        [[nodiscard]] constexpr double hoursFrom(const TimeStamp& other) const {
            if (m_timeSegment == other.m_timeSegment && hoursMinutesEq(other)) {
                return 24.0;
            }

            const double thisTotal = totalPercent();
            const double otherTotal = other.totalPercent();

            const double resultBase10 = thisTotal - otherTotal;

            if (resultBase10 < 0) {
                return (1 - (-resultBase10)) * 24;
            }

            return resultBase10 * 24;
        }

        [[nodiscard]] constexpr double hoursTo(const TimeStamp& other) const {
            return other.hoursFrom(*this);
        }

        [[nodiscard]] constexpr string debug() const {
            return "TimeStamp { m_format=" + m_format.toString() + ", m_timeSegment=" + m_timeSegment.toString()
            + ", m_hours=" + gutils::toString(m_hours) + ", m_minutes=" + gutils::toString(m_minutes) + " }";
        }

        [[nodiscard]] constexpr string toString() const {
            return std::format("{}:{} {}", m_hours, m_minutes, m_timeSegment.toString());
        }
    };
}

#endif //WORKHOURTRACKER_TIMESTAMP_HPP
