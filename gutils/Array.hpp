//
// Created by Micah on 10/15/2023.
//

#ifndef GEO_CPP_ARRAY_HPP
#define GEO_CPP_ARRAY_HPP

#include <cstddef>
#include <initializer_list>
#include <vector>

// TODO: BUILD OUT

namespace gutils {
    template<typename T, bool aggressiveChecking = true, bool throwOnIndexOutOfBounds = aggressiveChecking>
    class Array {
    private:
        T* m_data;
        size_t m_length;

        constexpr T* checkIndex(const size_t index) const {
            if constexpr (aggressiveChecking) {
                if (isNull()) {
                    throw std::runtime_error { "Array::m_data was null." };
                }

                if constexpr (throwOnIndexOutOfBounds) {
                    if (index >= m_length) {
                        throw std::runtime_error { std::format("Index {} was out of bounds for Array::m_length = {}.", index, m_length) };
                    }
                }
            }

            return m_data + index;
        }

        void deallocateRaw() {
            delete[] m_data;
        }

        void allocateRaw(const size_t length) {
            const size_t newLength = m_length + length;
            T* temp = new T[newLength];

            gutils::copy(m_data, temp, m_length);

            deallocateRaw();

            m_data = temp;
            m_length = newLength;
        }

        void reallocateRaw(const size_t length) {
            m_data = new T[length];
        }

    public:
        constexpr explicit Array(const size_t length = 0) : m_length(length), m_data(length == 0 ? nullptr : new T[length]) { }

        constexpr Array(const std::initializer_list<T>&& initList)
        : m_length(initList.size()), m_data(gutils::copy(initList.begin(), initList.end())) { }

        constexpr Array(const std::vector<T>& vector)
        : m_length(vector.size()), m_data(gutils::copy(vector.data(), vector.data() + vector.size())) { }

        constexpr Array(const Array<T>& original)
        : m_length(original.length()), m_data(gutils::copy(original.begin(), original.end())) { }

        constexpr Array& operator=(const Array<T>& other) {
            m_data = other.copyData();
            m_length = other.m_length;
            return *this;
        }

        [[nodiscard]] constexpr size_t length() const {
            return m_length;
        }

        constexpr T& get(const size_t index) {
            return *checkIndex(index);
        }

        constexpr const T& get(const size_t index) const {
            return *checkIndex(index);
        }

        constexpr T* getRaw(const size_t index) {
            return checkIndex(index);
        }

        constexpr const T* getRaw(const size_t index) const {
            return checkIndex(index);
        }

        constexpr void set(const size_t index, const T& value) {
            m_data[index] = value;
        }

        constexpr void setRawPointer(const size_t index, const T* value) {
            (m_data + index) = value;
        }

        [[nodiscard]] constexpr T& operator[](const size_t index) {
            return get(index);
        }

        [[nodiscard]] constexpr const T& operator[](const size_t index) const {
            return get(index);
        }

        template<bool checkNull = true>
        constexpr Array<T>& deallocate() {
            if constexpr (checkNull) {
                if (!isNull()) {
                    deallocateRaw();
                }
            } else {
                deallocateRaw();
            }

            m_length = 0;

            return *this;
        }

        template<bool deleteFirst = true, bool checkNull = deleteFirst>
        constexpr Array<T>& reallocate(const size_t length) {
            if constexpr (deleteFirst) {
                deallocate<checkNull>();
            }

            reallocateRaw(length);
            m_length = length;

            return *this;
        }

        [[nodiscard]] constexpr bool isNull() const {
            return m_data == nullptr;
        }

        [[nodiscard]] constexpr T* begin() {
            return checkIndex(0);
        }

        [[nodiscard]] constexpr T* end() {
            return checkIndex(m_length - 1) + 1;
        }

        [[nodiscard]] constexpr const T* begin() const {
            return checkIndex(0);
        }

        [[nodiscard]] constexpr const T* end() const {
            return checkIndex(m_length - 1) + 1;
        }

        [[nodiscard]] constexpr const T* cbegin() const {
            return checkIndex(0);
        }

        [[nodiscard]] constexpr const T* cend() const {
            return checkIndex(m_length - 1) + 1;
        }

        [[nodiscard]] constexpr T* data() {
            return checkIndex(0);
        }

        [[nodiscard]] constexpr const T* data() const {
            return checkIndex(0);
        }

        [[nodiscard]] constexpr Array<T> copy() const {
            return Array<T> { *this };
        }

        [[nodiscard]] constexpr T* copyData() const {
            return gutils::copy(begin(), end());
        }

        [[nodiscard]] constexpr std::vector<T> toVector() const {
            return std::vector<T> { begin(), end() };
        }

        constexpr ~Array() {
            deallocate<true>();
        }
    };
}

#endif //GEO_CPP_ARRAY_HPP
