//
// Created by Micah on 12/18/2023.
//

#ifndef WORKHOURTRACKER_TIMEFORMAT_HPP
#define WORKHOURTRACKER_TIMEFORMAT_HPP

#include "../gutils/ENUM_CLASS.hpp"
#include "../gutils/gutils.hpp"
#include <string>
using std::string;

namespace gtiming {
    ENUM_CLASS(TimeFormat, uint8_t, TWENTY_FOUR_HOUR, TWELVE_HOUR)
    PUBLIC
    [[nodiscard]] constexpr string toString() const {
        return *this == TWELVE_HOUR ? "12" : "24";
    }
    END
}

#endif //WORKHOURTRACKER_TIMEFORMAT_HPP