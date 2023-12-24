//
// Created by Micah on 12/14/2023.
//

#ifndef BESTBUYTRACKER_DATES_HPP
#define BESTBUYTRACKER_DATES_HPP

#include <chrono>
#include <string>
using namespace std::chrono;
using std::string;

#include "../gutils/ENUM_CLASS.hpp"

// TODO: MAYBE BUILD THIS OUT

namespace gtiming {
    ENUM_CLASS(DateFormat, uint8_t, DATE, TIME, DATE_TIME)
    END

    template<DateFormat::Value format = DateFormat::DATE_TIME>
    string nowRawString() {
        std::time_t timeRaw = std::time(nullptr);

        const string dateTime = std::asctime(std::localtime(&timeRaw));

        string returnValue;

        switch (format) {
            case DateFormat::DATE_TIME:
                returnValue = dateTime;
                break;

            case DateFormat::DATE: {
                const size_t colonIndex = dateTime.find(':');

                returnValue = string { dateTime.begin(), dateTime.begin() + colonIndex - 3 };
                returnValue += ' ';
                returnValue.append(dateTime.begin() + colonIndex + 7, dateTime.end());

                break;
            }

            case DateFormat::TIME: {
                const size_t colonIndex = dateTime.find(':');
                returnValue = string { dateTime.begin() + colonIndex - 2, dateTime.begin() + colonIndex + 7 };
                break;
            }

            default:
                returnValue = { };
        }

        return returnValue;
    }
}

#endif //BESTBUYTRACKER_DATES_HPP