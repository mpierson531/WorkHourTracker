//
// Created by Micah on 12/8/2023.
//

#ifndef BESTBUYTRACKER_ENUM_CLASS_HPP
#define BESTBUYTRACKER_ENUM_CLASS_HPP

#include <functional>
#include "gutils.hpp"

namespace gutils {
#define VALUES(...) __VA_ARGS__
#define PRIVATE private: \

#define PUBLIC public: \

#define END };

#define ENUM_CLASS(name, type, ...) \
    struct name { \
    PUBLIC                                         \
                                                    \
    enum Value : type {                             \
        VALUES(__VA_ARGS__)                         \
    };                                            \
                                                    \
    PRIVATE                                        \
                                                    \
    Value value;                                    \
    PUBLIC                                        \
                                                  \
    constexpr name(const Value value) : value(value) {} \
                                                        \
    constexpr name& operator=(const Value value)  { this->value = value; return *this; } \
    constexpr name& operator=(const name value) { this->value = value.value; return *this; }    \
    constexpr bool operator==(const name other) const { return this->value == other.value; }    \
    constexpr bool operator==(const name::Value other) const { return this->value == other; };                                \
    constexpr operator Value() const { return value; }  \
    constexpr operator type() const { return (type) value; } \
    constexpr operator bool() const = delete;       \
                                                    \
    constexpr type underlying() const {           \
       return operator type();                      \
    }                                               \
                                                    \
    template<typename Function>                                              \
    static constexpr void forEach(Function&& function) {        \
          constexpr size_t count = gutils::vaCount(__VA_ARGS__);         \
          constexpr Value values[count] = { __VA_ARGS__ };         \
                                                                    \
          for (int index = 0; index != count; index++)                 \
              function(values[index]);                                \
    }                                                                  \
                                                                        \
    static constexpr void forEach(const std::function<void(const Value value)>& function) {    \
          constexpr size_t count = gutils::vaCount(__VA_ARGS__);  \
          constexpr name values[count] = { __VA_ARGS__ };          \
                                                                    \
          for (int index = 0; index != count; index++)               \
              function(values[index]);                                \
    }                               \
                                    \
                                    \
                                                                        \
    [[nodiscard]] constexpr string toStringRaw() const {                 \
        return gutils::toString(value);                                   \
    }                                                                      \
                                                                            \
    [[nodiscard]] static constexpr size_t count() {                          \
        return gutils::vaCount(__VA_ARGS__);                                  \
    }                                                                          \
                                                                                \
    PRIVATE
}


#endif //BESTBUYTRACKER_ENUM_CLASS_HPP