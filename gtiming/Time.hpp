//
// Created by Micah on 10/21/2023.
//

#ifndef CPP_TESTING_LEARNING_TIME_HPP
#define CPP_TESTING_LEARNING_TIME_HPP

#include "timing.hpp"
#include "../gutils/gutils.hpp"

namespace gtiming {
    template<gutils::floating_point FloatType = float, gtiming::TimeUnit Unit = gtiming::TimeUnit::Milli>
    struct Time {
    public:
        static Time<FloatType, Unit> Now() {
            if constexpr (Unit == TimeUnit::Milli) {
                return rawTimeMillis<FloatType>();
            }

            if constexpr (Unit == TimeUnit::Nano) {
                return rawTimeNanos<FloatType>();
            }

            return rawTimeSecs<FloatType>();
        }

        FloatType value;

        Time(const FloatType value) : value(value) { }

        gtiming::TimeUnit TimeUnit() {
            return Unit;
        }

        explicit operator double() const {
            return double{value};
        }

        explicit operator float() const {
            return float{value};
        }
        
        Time operator+(const FloatType value) const {
            return {this->value + value};
        }

        template<gutils::floating_point OtherFloatType, gtiming::TimeUnit OtherUnit>
        Time<FloatType, Unit> operator+(const Time<OtherFloatType, OtherUnit>& other) const {
            return value + other.to(Unit).value;
        }

        Time& operator+=(const FloatType value) {
            this->value += value;
            return *this;
        }

        template<gutils::floating_point OtherFloatType, gtiming::TimeUnit OtherUnit>
        Time<FloatType, Unit>& operator+=(const Time<OtherFloatType, OtherUnit>& other) {
            value += other.to(Unit).value;
            return *this;
        }

        Time<FloatType, Unit> operator-(const FloatType value) const {
            return this->value - value;
        }

        template<gutils::floating_point OtherFloatType, gtiming::TimeUnit OtherUnit>
        Time<FloatType, Unit> operator-(const Time<OtherFloatType, OtherUnit>& other) const {
            return value - other.to(Unit).value;
        }

        Time& operator-=(const FloatType value) {
            this->value -= value;
            return *this;
        }

        template<gutils::floating_point OtherFloatType, gtiming::TimeUnit OtherUnit>
        Time<FloatType, Unit>& operator-=(const Time<OtherFloatType, OtherUnit>& other) {
            value -= other.to(Unit).value;
            return *this;
        }

        Time operator*(const FloatType value) const {
            return this->value * value;
        }

        template<gutils::floating_point OtherFloatType, gtiming::TimeUnit OtherUnit>
        Time<FloatType, Unit> operator*(const Time<OtherFloatType, OtherUnit>& other) const {
            return value * other.to(Unit).value;
        }

        Time& operator*=(const FloatType value) {
            this->value *= value;
            return *this;
        }

        template<gutils::floating_point OtherFloatType, gtiming::TimeUnit OtherUnit>
        Time<FloatType, Unit>& operator*=(const Time<OtherFloatType, OtherUnit>& other) {
            value *= other.to(Unit).value;
            return *this;
        }

        Time<FloatType, Unit> operator/(const FloatType value) const {
            return this->value / value;
        }

        template<gutils::floating_point OtherFloatType, gtiming::TimeUnit OtherUnit>
        Time<FloatType, Unit> operator/(const Time<OtherFloatType, OtherUnit>& other) const {
            return value / other.to(Unit).value;
        }

        Time& operator/=(const FloatType value) {
            this->value /= value;
            return *this;
        }

        template<gutils::floating_point OtherFloatType, gtiming::TimeUnit OtherUnit>
        Time<FloatType, Unit>& operator/=(const Time<OtherFloatType, OtherUnit>& other) {
            value /= other.to(Unit).value;
            return *this;
        }

        Time<FloatType, Unit> operator^(const FloatType value) const {
            return static_cast<FloatType>(powf(this->value, value));
        }

        template<gutils::floating_point OtherFloatType, gtiming::TimeUnit OtherUnit>
        Time<FloatType, Unit> operator^(const Time<OtherFloatType, OtherUnit>& other) const {
            return static_cast<FloatType>(powf(value, other.to(Unit).value));
        }

        Time& operator^=(const FloatType value) {
            this->value = powf(this->value, value);
            return *this;
        }

        template<gutils::floating_point OtherFloatType, gtiming::TimeUnit OtherUnit>
        Time<FloatType, Unit>& operator^=(const Time<OtherFloatType, OtherUnit>& other) {
            value = powf(value, other.to(Unit).value);
            return *this;
        }

        Time<FloatType, Unit> operator%(const FloatType value) const {
            return modf(this->value, value);
        }

        template<gutils::floating_point OtherFloatType, gtiming::TimeUnit OtherUnit>
        Time<FloatType, Unit> operator%(const Time<OtherFloatType, OtherUnit>& other) const {
            return modf(value, other.to(Unit).value);
        }

        Time& operator%=(const FloatType value) {
            this->value = fmod(this->value, value);
            return *this;
        }

        template<gutils::floating_point OtherFloatType, gtiming::TimeUnit OtherUnit>
        Time<FloatType, Unit>& operator%=(const Time<OtherFloatType, OtherUnit>& other) {
            value = fmod(this->value, other.to(Unit).value);
            return *this;
        }

        Time<FloatType, Unit> to(const gtiming::TimeUnit unit) const {
            switch (unit) {
                case gtiming::TimeUnit::Nano:
                    return toNanos(Unit,this->value);
                case gtiming::TimeUnit::Milli:
                    return toMillis(Unit, this->value);
                case gtiming::TimeUnit::Second:
                    return toSeconds(Unit, this->value);
            }
        }

        Time<FloatType, gtiming::TimeUnit::Nano> ToNanos() const {
            return to(gtiming::TimeUnit::Nano);
        }

        Time<FloatType, gtiming::TimeUnit::Milli> ToMillis() const {
            return to(gtiming::TimeUnit::Milli);
        }

        Time<FloatType, gtiming::TimeUnit::Second> ToSeconds() const {
            return to(gtiming::TimeUnit::Second);
        }

        [[nodiscard]] string ToString() const {
            string suffix;

            switch (Unit) {
                case TimeUnit::Nano: suffix = "ns"; break;
                case TimeUnit::Milli: suffix = "ms"; break;
                case TimeUnit::Second: suffix = "s"; break;
            }

            return std::to_string(value) + suffix;
        }
    };
}

#endif //CPP_TESTING_LEARNING_TIME_HPP