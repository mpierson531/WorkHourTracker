//
// Created by Micah on 12/15/2023.
//

#ifndef BESTBUYTRACKER_TRACKER_HPP
#define BESTBUYTRACKER_TRACKER_HPP

#include <cwchar>
#include <string>
#include <format>
#include <vector>
#include <string_view>
using std::string, std::vector, std::string_view;

#include "../gutils/ENUM_CLASS.hpp"
#include "../gutils/gutils.hpp"
#include "../gutils/Result.hpp"
#include "../gutils/Array.hpp"
#include "../gtiming/TimeSegment.hpp"
#include "../gtiming/TimeFormat.hpp"
#include "../gtiming/TimeStamp.hpp"
using gutils::Array, gutils::Result;
using gtiming::TimeSegment, gtiming::TimeFormat, gtiming::TimeStamp;

#include "Token.hpp"
#include "Tokenizer.hpp"
#include "Parser.hpp"

// TODO: IMPLEMENT TWELVE AND TWENTY_FOUR-HOUR FORMATS

namespace tracker {
//    ENUM_CLASS(DateFormat, uint8_t, MONTH_DAY_YEAR, DAY_MONTH_YEAR)
//    END

// TODO: FLESH-OUT DATE HANDLING
struct Validator {
private:
    const Array<Day>& days;
    vector<string>& errors;

    void validateHours(const uint8_t hours) {
        if (hours > 12) {
            errors.emplace_back("Invalid start m_hours: " + gutils::toString(hours));
        }
    }

    void validateMinutes(const uint8_t minutes) {
        if (minutes > 59) {
            errors.emplace_back("Invalid start m_minutes: " + gutils::toString(minutes));
        }
    }

    void validateDate(const string_view& view) {
        string month { view[0] };
        int dayIndex;

        if (isdigit(view[1])) {
            month.push_back(view[1]);
            dayIndex = 3;
        } else {
            dayIndex = 2;
        }

        string dayString {view[dayIndex] };

        if (isdigit(view[dayIndex + 1])) {
            dayString.push_back(view[dayIndex + 1]);
        }

        const uint8_t monthNumber = (uint8_t) std::stoull(month);
        const uint8_t dayNumber = (uint8_t) std::stoull(dayString);

        if (monthNumber > 12) {
            errors.emplace_back("Invalid month: " + month);
        }

        if (dayNumber > 31) {
            errors.emplace_back("Invalid dayString: " + dayString);
        }
    }

public:
    Validator(const Array<Day>& days, vector<string>& errors)
    : days(days), errors(errors) { }

    bool validate() {
        for (const Day& day : days) {
            const TimeStamp<uint8_t>& start = day.start();
            const TimeStamp<uint8_t>& end = day.end();

            validateHours(start.hours());
            validateMinutes(start.minutes());

            validateHours(end.hours());
            validateMinutes(end.minutes());

            validateDate(string_view { day.date() });
        }

        return errors.empty();
    }
};

    Result<Array<Token>, Array<string>> tokenize(const string& source) {
        return Tokenizer().tokenize(source);
    }

    template<bool validate>
    Result<Array<Day>, Array<string>> parse(const Array<Token>& tokens) {
        Parser parser {tokens};

        auto parseResult = parser.parse();

        if (!parseResult) {
            return parseResult;
        }

        const Array<Day> days = parseResult.unwrap();

        if constexpr (validate) {
            vector<string> errors;

            Validator validator { days, errors };

            if (!validator.validate()) {
                return Result<Array<Day>, Array<string>>::errOf(Array<string> { errors });
            }
        }

        return Result<Array<Day>, Array<string>>::okOf(days);
    }

    double evaluate(const Array<Day>& days) {
        double total = 0;

        for (const Day& day : days) {
            std::cout << day.hourlyRate() << '\n';
            total += day.calculateTotal();
        }

        return total;
    }

    constexpr Array<int> findAllDollarSigns(const string& source) {
        vector<int> indices;

        const size_t length = source.length();

        for (int index = 0; index != length; index++) {
            if (source[index] == '$')
                indices.push_back(index);
        }

        return Array<int> { indices };
    }

    // TODO: CHANGE BELOW TO COUNT TIME
    /*double basicTrack(const string& source) {
        string sanitizedSource;

        for (const char c : source) {
            if (!isblank(c)) {
                sanitizedSource.push_back(c);
            }
        }

        const Array<int> indices = findAllDollarSigns(sanitizedSource);
        const size_t indicesLength = indices.length();
        const size_t sourceLength = sanitizedSource.length();

        int index = 0;

        double perHour = 0;
        double total = 0;

        while (index != indicesLength) {
            int numberIndex = indices[index++] + 1;

            string numberString;

            while (numberIndex != sourceLength && isdigit(sanitizedSource[numberIndex])) {
                numberString.push_back(sanitizedSource[numberIndex++]);
            }

            if (source[numberIndex] == '{') {
                perHour = gutils::toDouble(numberString).unwrap();
            } else {
                total +=
            }

            if (index + 3 < indicesLength && source[index + 3] == '{') {
                int perHourEndIndex = index + 1;
                string perHourString;

                while (isdigit(source[perHourEndIndex])) {
                    perHourString.push_back(source[perHourEndIndex++]);
                }



                perHour = gutils::toDouble(perHourString).unwrap();
            }
        }
    }*/

    Result<double, void*> basicTrack(const string& source) {
        string sanitizedSource;

        for (const char c : source) {
            if (!isblank(c)) {
                sanitizedSource.push_back(c);
            }
        }

        const Array<int> indices = findAllDollarSigns(sanitizedSource);
        const size_t indicesLength = indices.length();
        const size_t sourceLength = sanitizedSource.length();

        int indicesIndex = 0;
        double total = 0;

        while (indicesIndex != indicesLength) {
            int numberIndex = indices[indicesIndex] + 1;

            string numberString;

            while (numberIndex != sourceLength && isdigit(sanitizedSource[numberIndex])) {
                numberString.push_back(sanitizedSource[numberIndex++]);
            }

            if (sanitizedSource[numberIndex] != '{') {
                auto result = gutils::toDouble(numberString);

                if (!result) {
                    return result;
                }

                total += result.unwrap();
            }

            indicesIndex++;
        }

        return Result<double, void*>::okOf(total);
    }
}

#endif //BESTBUYTRACKER_TRACKER_HPP