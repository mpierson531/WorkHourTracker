//
// Created by Micah on 11/5/2023.
//

#include <cstddef>
#include <cstdint>

#ifndef CPP_TESTING_LEARNING_BOOLARRAYV2_HPP
#define CPP_TESTING_LEARNING_BOOLARRAYV2_HPP

#include "../gmath/math.hpp"
#include "bit_utils.hpp"

using namespace gutils::bitutils;

namespace gutils {
    template<gutils::unsigned_integer T>
    struct BitBoolArray {
    private:
#define DATA_BIT_COUNT gutils::bitutils::bitCount<T>()
        T data;

        template<gutils::integer I>
        static constexpr T FromInteger(const I value) noexcept {
            return gmath::gClamp(0, std::numeric_limits<T>::max(), value);
        }

        static constexpr T FromBool(const bool value) noexcept {
            return value ? std::numeric_limits<T>::max() : 0;
        }
    public:
        BitBoolArray() noexcept : BitBoolArray(false) { }

        BitBoolArray(const bool value) noexcept : data(FromBool(value)) { }

        template<gutils::integer I>
        BitBoolArray(const I value) noexcept : data(FromInteger(value)) { }

        [[nodiscard]] explicit consteval operator T() const noexcept {
            return data;
        }

        [[nodiscard]] bool Get(const uint8_t index) const {
            return (data >> index) & (uint8_t) 1;
        }

        void Set(const uint8_t index, const bool value) {
            data |= (T) (value << index);
        }

        void SetAll(const bool value) {
            for (int i = 0; i < DATA_BIT_COUNT; i++)
                Set(i, value);
        }

        [[nodiscard]] gutils::Array<uint8_t> Raw() const noexcept {
            return getAllBits(data);
        }

        void ForEach(const auto& functor) const noexcept {
            for (int i = 0; i < Length(); i++)
                functor(Get(i));
        }

        [[nodiscard]] static constexpr uint8_t Length() noexcept {
            return DATA_BIT_COUNT;
        }
    };
}

#endif //CPP_TESTING_LEARNING_BOOLARRAYV2_HPP