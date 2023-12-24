//
// Created by Micah on 11/5/2023.
//

#ifndef CPP_TESTING_LEARNING_AUTOBITBOOLARRAY_HPP
#define CPP_TESTING_LEARNING_AUTOBITBOOLARRAY_HPP

#include <cstddef>
#include <cstdint>
#include "bit_utils.hpp"

using namespace gutils::bitutils;

template<const size_t size, const bool checked = true>
struct AutoBitBoolArray {
private:
    const size_t dataLength;
    uint8_t* data;

    void CheckIndices(const size_t userIndex, const size_t dataIndex, const uint8_t bitIndex) {
        if constexpr (checked) {
            if (dataIndex >= dataLength)
                throw std::runtime_error {"Index out of bounds."};
        }

        if (userIndex >= size)
            throw std::runtime_error {"Index out of bounds."};

        if (bitIndex >= CHAR_BIT)
            throw std::runtime_error {"Index out of bounds."};
    }

    static constexpr size_t GetDataIndex(const size_t index) noexcept {
        if (index > CHAR_BIT) {
            if (index % CHAR_BIT == 0)
                return (size_t) std::ceil((float) index / (float) CHAR_BIT);

            return (size_t) std::ceil((float) index / (float) CHAR_BIT) - 1;
        } else if (index < CHAR_BIT)
            return 0;
        else
            return 1;
    }

    static constexpr uint8_t GetBitIndex(const size_t dataIndex, const size_t userIndex) noexcept {
        return dataIndex == 0 ? 0 : (uint8_t)abs((int)userIndex - ((int)CHAR_BIT * (int)dataIndex));
    }

public:
    AutoBitBoolArray() : AutoBitBoolArray(false) { }

    explicit AutoBitBoolArray(const bool value) : dataLength((size_t) std::ceil((float) size / CHAR_BIT)), data(nullptr) {
        data = new uint8_t[dataLength];
        SetAll(value);
    }

    ~AutoBitBoolArray() {
        delete[] data;
    }

    template<const bool check = checked>
    bool Get(const size_t index) const {
        const size_t dataIndex = GetDataIndex(index);
        const uint8_t bitIndex = GetBitIndex(dataIndex, index);

        if constexpr (check) {
            CheckIndices(index, dataIndex, bitIndex);
        }

        return getBit(data[dataIndex], bitIndex);
    }

    template<const bool check = checked>
    void Set(const uint8_t index, const bool value) {
        const size_t dataIndex = GetDataIndex(index);
        const uint8_t bitIndex = GetBitIndex(dataIndex, index);

        if constexpr (check) {
            CheckIndices(index, dataIndex, bitIndex);
        }

        uint8_t* pointer = data + dataIndex;

        clearBit(pointer, bitIndex);
        setBit(pointer, bitIndex, value);
    }

    void SetAll(const bool value) {
        const size_t dataLength = this->dataLength;

        if (value)
            for (int i = 0; i < dataLength; i++)
                data[i] = 255;
        else
            for (int i = 0; i < dataLength; i++)
                data[i] = 0;
    }

    [[nodiscard]] gutils::Array<uint8_t> Raw() const noexcept {
        gutils::Array<uint8_t> raw{Length()};

        for (int i = 0; i < Length(); i++)
            raw[i] = Get<false>(i);

        return raw;
    }

    void ForEach(const auto& functor) const noexcept {
        for (int i = 0; i < Length(); i++)
            functor(Get<false>(i));
    }

    [[nodiscard]] static consteval bool IsChecked() noexcept {
        return checked;
    }

    [[nodiscard]] static consteval size_t Length() noexcept {
        return size;
    }
};

#endif //CPP_TESTING_LEARNING_AUTOBITBOOLARRAY_HPP