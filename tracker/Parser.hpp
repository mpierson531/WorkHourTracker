//
// Created by Micah on 12/14/2023.
//

#ifndef BESTBUYTRACKER_PARSER_HPP
#define BESTBUYTRACKER_PARSER_HPP

#include <string>
#include <vector>
#include <format>
using std::string, std::vector;

#include "../gutils/gutils.hpp"
#include "../gutils/Result.hpp"
#include "../gutils/Array.hpp"
#include "../gtiming/TimeStamp.hpp"
#include "../gtiming/TimeFormat.hpp"
using gutils::Result, gutils::Array;
using gtiming::TimeSegment, gtiming::TimeFormat, gtiming::TimeStamp;

#include "Token.hpp"
#include "Day.hpp"

// TODO: VALIDATE LENGTH AND VALUES OF NUMBERS (DATES/TIMES)

namespace tracker {
    class Parser {
    private:
        Array<Token> tokens;
        vector<string> m_errors;
        vector<Day> days;
        double currentHourlyPay;
        int tokenIndex;

        const Token& peek() {
            return tokens[tokenIndex];
        }

        Token eat() {
            return tokens[tokenIndex++];
        }

        Result<Token, string> expect(const Token::Type tokenType) {
            const Token next = eat();

            if (next.type != tokenType) {
                return Result<Token, string>::errOf(tokenType.toString());
            }

            return Result<Token, string>::okOf(next);
        }

        Result<string, string> parseDate(const Token& month) {
            auto slashResult = expect(Token::Type::FSlash);

            if (!slashResult) {
                return Result<string, string>::errOf("Expected forward slash after month");
            }

            auto dayResult = expect(Token::Type::Number);

            if (!dayResult) {
                return Result<string, string>::errOf("Expected number after forward slash");
            }

            return Result<string, string>::okOf(std::format("{}/{}", month.value, dayResult.unwrap().value));
        }

        Result<TimeStamp<uint8_t>, string> parseTimeStamp(const Token& hours) {
            auto colonResult = expect(Token::Type::Colon);

            if (!colonResult) {
                return Result<TimeStamp<uint8_t>, string>::errOf("Expected colon after m_hours in time stamp");
            }

            auto minutesResult = expect(Token::Type::Number);

            if (!minutesResult) {
                return Result<TimeStamp<uint8_t>, string>::errOf("Expected number after colon in time stamp");
            }

            auto timeSegmentResult = expect(Token::Type::Word);

            if (!timeSegmentResult) {
                return Result<TimeStamp<uint8_t>, string>::errOf("Expected 'AM' or 'PM' at end of time stamp");
            }

            const string timeSegmentString = gutils::toLower(timeSegmentResult.unwrap().value);

            TimeSegment timeSegment;

            if (timeSegmentString == "am") {
                timeSegment = TimeSegment::AM;
            } else if (timeSegmentString == "pm") {
                timeSegment = TimeSegment::PM;
            } else {
                return Result<TimeStamp<uint8_t>, string>::errOf("Could not determine time of day: " + timeSegmentString);
            }

            // TODO: HANDLE BOTH TWELVE AND TWENTY_FOUR-HOUR FORMATS
            return Result<TimeStamp<uint8_t>, string>::okOf(TimeStamp<uint8_t> {
                    TimeFormat::TWELVE_HOUR,
                    timeSegment,
                    (uint8_t) std::stoull(hours.value),
                    (uint8_t) std::stoull(minutesResult.unwrap().value)
            });
        }

        Result<Day, string> parseDay(const Token& month) {
            auto dateResult = parseDate(month);

            if (!dateResult) {
                return Result<Day, string>::errOf(dateResult.unwrap_err());
            }

            auto colonResult = expect(Token::Type::Colon);

            if (!colonResult) {
                return Result<Day, string>::errOf("Expected colon after date declaration");
            }

            auto startHoursResult = expect(Token::Type::Number);

            if (!startHoursResult) {
                return Result<Day, string>::errOf("Expected time stamp after date declaration");
            }

            auto startTimeResult = parseTimeStamp(startHoursResult.unwrap());

            if (!startTimeResult) {
                return Result<Day, string>::errOf(startTimeResult.unwrap_err());
            }

            auto hyphenResult = expect(Token::Type::Hyphen);

            if (!hyphenResult) {
                return Result<Day, string>::errOf("Expected hyphen after beginning time stamp");
            }

            auto endHoursResult = expect(Token::Type::Number);

            if (!endHoursResult) {
                return Result<Day, string>::errOf("Expected second time stamp after first time stamp");
            }

            auto endTimeResult = parseTimeStamp(endHoursResult.unwrap());

            if (!endTimeResult) {
                return Result<Day, string>::errOf(endTimeResult.unwrap_err());
            }

            const Day day = Day{
                    dateResult.unwrap(),
                    startTimeResult.unwrap(),
                    endTimeResult.unwrap(),
                    currentHourlyPay
            };

            return Result<Day, string>::okOf(day);
        }

/*// TODO: WORK OUT VALIDATION
        static StampResult timeStampFromString(const string_view& source) {
            const string trimmedSource = gutils::trimFromView(source);

            if (trimmedSource.empty())
                return StampResult::errOf("Source while parsing was empty");

            const size_t length = source.length();

            int start = 0;

            while (start != length && !isdigit(source[start])) {
                start++;
            }

            if (start == 0) {
                return StampResult::errOf("Did not find numbers in time stamp source");
            }



            try {
                const string_view hoursString = source.substr(0, 2);
                const string_view minutesString = source.substr(3, 5);
                const string_view timeSegmentString;

                if

                    // TODO: CONVERT NUMBER PARSING TO CATCH EXCEPTIONS
                    const uint8_t m_hours = (uint8_t) std::stoull(hoursString.data());
                const uint8_t m_minutes = (uint8_t) std::stoull(minutesString.data());
                const auto segmentResult = tracker::TimeSegment::fromString(timeSegmentString);
                const tracker::TimeFormat m_format = tracker::TimeFormat::TWELVE_HOUR; // TODO: VALIDATE/GET TIME FORMAT

                return StampResult::okOf(tracker::TimeStamp{m_format, m_timeSegment, m_hours, m_minutes});
            } catch (const std::exception& e) {
                return StampResult::errOf(e.what());
            }
        }

        void parseHourlyPay() {
            auto amountResult = expect(Token::Type::Number);

            ERROR_CHECK(amountResult)

            const Token amountToken = amountResult.unwrap();

            auto toDoubleResult = gutils::toDouble(amountToken.value);

            if (!toDoubleResult) {
                m_errors.push_back(std::m_format("Could not convert '{}' to a number", amountToken.value));
            }

            currentHourlyPay = toDoubleResult.unwrap();
        }*/

    public:
        explicit Parser(const Array<Token>& tokens)
                : tokenIndex(0), tokens(tokens), currentHourlyPay(0) { }

        Result<Array<Day>, Array<string>> parse() {
            if (tokens.length() == 0) {
                return Result<Array<Day>, Array<string>>::errOf(Array<string> { "tokens length was 0"});
            }

            while (peek().type != Token::Type::End) {
                const Token next = eat();

                switch (next.type.underlying()) {
                    case Token::Type::DollarSign: {
                        auto amountResult = expect(Token::Type::Number);

                        if (!amountResult) {
                            m_errors.emplace_back("Expected number after dollar sign");
                            continue;
                        }

                        if (!expect(Token::Type::Colon)) {
                            m_errors.emplace_back("Expected colon after hourly amount.");
                            continue;
                        }

                        const Token amountToken = amountResult.unwrap();

                        auto toDoubleResult = gutils::toDouble(amountToken.value);

                        if (!toDoubleResult) {
                            m_errors.push_back(std::format("Could not convert '{}' to a number", amountToken.value));
                            continue;
                        }

                        currentHourlyPay = toDoubleResult.unwrap();

                        continue;
                    }

                        // Declaration of new day expected
                    case Token::Type::Number: {
                        auto dayResult = parseDay(next);

                        if (!dayResult) {
                            m_errors.push_back(dayResult.unwrap_err());
                            continue;
                        }

                        days.push_back(dayResult.unwrap());

                        continue;
                    }

                    default:
                        throw std::runtime_error{std::format("Unexpected token: {}", next.toString())};
                }
            }

            if (!m_errors.empty()) {
                return Result<Array<Day>, Array<string>>::errOf(Array<string> { m_errors });
            }

            return Result<Array<Day>, Array<string>>::okOf(Array<Day> { days });
        }
    };
}

#endif //BESTBUYTRACKER_PARSER_HPP