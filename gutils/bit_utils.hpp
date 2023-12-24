//
// Created by Micah on 11/5/2023.
//

#ifndef GEO_CPP_BIT_UTILS_HPP
#define GEO_CPP_BIT_UTILS_HPP

#include "gutils.hpp"
#include "Array.hpp"

namespace gutils::bitutils {
    template<typename T>
    consteval uint8_t bitCount() noexcept {
        return sizeof(T) * CHAR_BIT;
    }

    template<gutils::unsigned_integer T>
    constexpr uint8_t getBit(const T value, const uint8_t bitIndex) {
        return (uint8_t) ((uint8_t)(value >> bitIndex) & (uint8_t)1);
    }

    template<gutils::unsigned_integer T>
    constexpr void getBit(const T value, const uint8_t bitIndex, uint8_t* bitPointer) {
        *bitPointer = getBit(value, bitIndex);
    }

    template<gutils::unsigned_integer T>
    constexpr void setBit(T* value, const uint8_t bitIndex, const uint8_t newValue) {
        *value |= (T) (newValue << bitIndex);
    }

    template<gutils::unsigned_integer T>
    constexpr void clearBit(T* value, const uint8_t bitIndex) {
        *value &= (T) ~((uint8_t) 1 << bitIndex);
    }

    template<gutils::unsigned_integer T>
    gutils::Array<uint8_t> getAllBits(const T value) {
        gutils::Array<uint8_t> bits{bitCount<T>()};

        for (int i = 0; i < bitCount<T>(); i++)
            bits[i] = getBit(value, i);

        return bits;
    }

    template<gutils::unsigned_integer T>
    constexpr void setAllBits(T* value, const uint8_t newValue) {
        for (int i = 0; i < bitCount<T>(); i++)
            setBit(value, i, newValue);
    }

    template<gutils::unsigned_integer T>
    constexpr void clearAllBits(T* value, const uint8_t) {
        for (int i = 0; i < bitCount<T>(); i++)
            clearBit(value, i);
    }

    template<gutils::unsigned_integer T>
    constexpr bool bitToBool(const T bit) {
        return bit == 1;
    }

    constexpr uint8_t boolToBit(const bool value) {
        return uint8_t(value ? 1 : 0);
    }
}

#endif //GEO_CPP_BIT_UTILS_HPP