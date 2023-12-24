//
// Created by Micah on 12/19/2023.
//

#ifndef WORKHOURTRACKER_DAY_HPP
#define WORKHOURTRACKER_DAY_HPP

#include "../gtiming/TimeStamp.hpp"
#include "../gtiming/TimeFormat.hpp"
using gtiming::TimeFormat, gtiming::TimeStamp;

namespace tracker {
    struct Day {
    private:
        string m_date;
        TimeStamp<uint8_t> m_start;
        TimeStamp<uint8_t> m_end;
        double m_hourlyRate;

    public:
        constexpr Day(const string& date, const TimeStamp<uint8_t> start, const TimeStamp<uint8_t> end, const double hourlyRate)
        : m_date(date), m_start(start), m_end(end), m_hourlyRate(hourlyRate) { }

        constexpr Day() = default;
        constexpr Day(const Day& other) = default;

        constexpr Day& operator=(const Day& other) = default;
        constexpr bool operator==(const Day& other) const = default;

        [[nodiscard]] constexpr double calculateTotal() const {
            return m_start.hoursTo(m_end) * m_hourlyRate;
        }

        [[nodiscard]] constexpr const string& date() const {
            return m_date;
        }

        [[nodiscard]] constexpr const TimeStamp<uint8_t>& start() const {
            return m_start;
        }

        [[nodiscard]] constexpr const TimeStamp<uint8_t>& end() const {
            return m_end;
        }

        [[nodiscard]] constexpr double hourlyRate() const {
            return m_hourlyRate;
        }
    };
}

#endif //WORKHOURTRACKER_DAY_HPP