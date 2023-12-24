//
// Created by Micah on 10/15/2023.
//

#ifndef GEO_CPP_MATH_HPP
#define GEO_CPP_MATH_HPP

// TODO: More gmath functions

#include <cmath>
#include "../gutils/gutils.hpp"

namespace gmath {
    constexpr float PI = 3.14159265359f;
    constexpr float HALF_PI = PI / 2;
    constexpr float PI_2 = PI * 2;

    constexpr float PI_SQRT = 1.772f;
    constexpr float PI_SQUARED = 9.8696044010906577398881f;

    constexpr float RAD_DEG = 180.0f / PI;
    constexpr float DEG_RAD = PI / 180.f;

    CONST_NUMBER_TEMP NType gInterp(const NType from, const NType to, const NType progress) {
        return from + (to - from) * progress;
    }

    CONST_FLOAT_TEMP FType gRound(const FType value) {
        return std::round(value);
    }

    template<gutils::floating_point FType, gutils::number NType>
    constexpr NType gRoundT(const FType value) {
        return static_cast<NType>(std::round(value));
    }

    CONST_NUMBER_TEMP NType gSum(NType* data, const size_t count) {
        NType sum;

        const NType* end = data + count;

        for (NType* start = data; start != end; start++) {
            sum += *start;
        }

       return sum;
    }

    CONST_FLOAT_TEMP FType gArea(const FType width, const FType height) {
        return width * height;
    }

    CONST_FLOAT_TEMP FType gCircumference(const FType radius) {
        return PI_2 * radius;
    }

    CONST_FLOAT_TEMP FType gAverage(FType* data, const size_t count) {
        FType sum = gSum<FType>(data, count);
        return sum / count;
    }

    CONST_FLOAT_TEMP FType gPow(const FType value, const FType power) {
        return std::pow(value, power);
    }

    CONST_NUMBER_TEMP NType gSqrt(const NType value) {
        return std::sqrt(value);
    }

    CONST_FLOAT_TEMP FType gMod(const FType one, const FType two) {
        return std::fmod(one, two);
    }

    CONST_FLOAT_TEMP FType gAbs(const FType value) {
        return std::abs(value);
    }

    template<gutils::number NType>
    NType gClamp(const NType min, const NType max, const NType value) {
        return value < min ? min : value > max ? max : value;
    }

    FLOAT_TEMP std::pair<FType, FType> gSplit(FType value) {
        FType whole;
        FType fractional = std::modf(value, &whole);
        return std::make_pair<FType, FType>(whole, fractional);
    }

    CONST_NUMBER_TEMP NType gAtan(const NType value) {
        return std::atan(value);
    }

    CONST_FLOAT_TEMP FType gAtan2(const FType x, const FType y) {
        return std::atan2(y, x);
    }

    CONST_FLOAT_TEMP FType gCos(const FType value) {
        return std::cos(value);
    }

    CONST_FLOAT_TEMP FType gSin(const FType value) {
        return std::sin(value);
    }
}

#endif //GEO_CPP_MATH_HPP
