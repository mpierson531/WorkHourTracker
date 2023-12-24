#pragma once

#include <cstdint>
#include <vector>
#include <stdexcept>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>

using std::vector, std::string, std::memcpy;

#include "Result.hpp"

namespace gutils {
#define FLOAT_TEMP template<gutils::floating_point FType>
#define NUMBER_TEMP template<gutils::number NType>

#define CONST_FLOAT_TEMP FLOAT_TEMP constexpr
#define CONST_NUMBER_TEMP NUMBER_TEMP constexpr

    template<typename T>
    concept char_type = std::is_same_v<T, char> || std::is_same_v<T, const char>;

    template<typename T>
    concept is_cstring = std::is_same_v<T, char*> || std::is_same_v<T, const char*> || std::is_same_v<T, char[]>;

    template<typename T>
    concept is_char_or_cstring = is_cstring<T> || char_type<T>;

    template<typename T>
    concept is_string = std::is_same_v<T, string> || std::is_same_v<T, const string>;

    template<typename T>
    concept is_string_type = is_string<T> || is_cstring<T>;

    template<typename T>
    concept is_string_type_or_char = is_string_type<T> || char_type<T>;

    template<typename T>
    concept number = std::is_integral_v<T> || std::is_floating_point_v<T>;

    template<typename T>
    concept not_number = !number<T>;

    template<typename T>
    concept floating_point = std::floating_point<T>;

    template<typename T>
    concept integer = std::is_integral_v<T>;

    template<typename T>
    concept unsigned_integer = integer<T> && std::is_unsigned_v<T>;

    template<typename T>
    concept signed_integer = integer<T> && !std::is_unsigned_v<T>;

    template<typename T>
    concept string_container = requires(T container) {
        std::ranges::range<T> && (typeid(container.begin()) == typeid(string) || typeid(container.begin()) == typeid(string*));
    };

    static constexpr size_t charSize = sizeof(char);
    static constexpr size_t doubleSize = sizeof(double);
    static constexpr size_t size_tSize = sizeof(size_t);

    static constexpr string newline = "\r\n";

    template<typename... Args>
    consteval size_t vaCount(Args&&...) {
        return sizeof...(Args);
    }

    // TODO: RE-IMPLEMENT VIEW FUNCTIONS
    std::string_view view(const string& value, const size_t start, const size_t end) {
        return std::string_view { value.begin() + start, value.begin() + end };
    }

    std::string_view view(const string& value, const size_t start) {
        return view(value, start, size_t(value.end() - (value.begin() + start)));
    }

    constexpr string toLower(const string& value) {
        string lower(value.length(), '\0');
        std::transform(value.cbegin(), value.cend(), lower.begin(), ::tolower);
        lower.shrink_to_fit();
        return std::move(lower);
    }

    constexpr string toUpper(const string& value) {
        string upper(value.length(), '\0');
        std::transform(value.cbegin(), value.cend(), upper.begin(), ::toupper);
        upper.shrink_to_fit();
        return std::move(upper);
    }

    constexpr static inline std::string toString(const std::nullptr_t) {
        return "NULL";
    }

    constexpr static std::string toString(const char value) {
        return string{value};
    }

    constexpr static std::string toString(const string& value) {
        return value;
    }

    template<is_cstring T>
    constexpr static std::string toString(const T value) {
        return string{value};
    }

    template<number T>
    constexpr static std::string toString(const T value) {
        return std::to_string(value);
    }

    constexpr static std::string charCodeString(const char value) {
        return std::to_string(value);
    }

    template<gutils::number Number>
    constexpr bool numberToBool(const Number number) {
        return number > 0;
    }

    template<gutils::number Number>
    constexpr Number boolToNumber(const bool value) {
        return value ? Number(1) : Number(0);
    }

    static inline string trimFromView(const std::string_view& value) {
        int startIndex = 0;

        while (isblank(value[startIndex])) {
            startIndex++;
        }

        int endIndex = static_cast<int>(value.size() - 1);

        while (endIndex != startIndex && isblank(value[endIndex])) {
            endIndex--;
        }

        const size_t newSize = endIndex + 1 - startIndex;
        string trimmed;

        trimmed.reserve(newSize);
        trimmed.append(value.begin() + startIndex, value.begin() + endIndex);

        return trimmed;
    }

    static inline string trim(const string& value) {
        return trimFromView(std::string_view {value.cbegin(), value.cend()});
    }

    template<typename T>
    constexpr size_t byteOffset(const size_t count) {
        return sizeof(T) * count;
    }

    template<typename T>
    static void copyMemory(T* source, T* dest, const size_t count) {
        const T* end = source + count;

        while (source != end) {
            *dest = *source;
            source++;
            dest++;
        }
    }

    template<typename T>
    static void copyMemory(T *source, T *dest, size_t start, const size_t count, size_t destPos) {
        copyMemory(source + start, dest + destPos, count);
    }

    template<typename T>
    static void copyMemory(T* source, T *dest, size_t count, const size_t destPos) {
        copyMemory(source, dest + destPos, count);
    }

    template<typename T>
    static void vecCopy(vector <T> &source, T *dest, size_t count, size_t destPos) {
        int sourceIndexi = 0;
        int destPosIndexi = destPos;

        while (sourceIndexi < count) {
            dest[destPosIndexi++] = source[sourceIndexi++];
        }
    }

    template<typename T>
    static void appendVec(vector <T> &appendee, vector <T> &toAppend) {
        appendee.insert(std::end(appendee), std::begin(toAppend), std::end(toAppend));
    }

    static Result<bool, void *> toBool(string value) {
        string trimmed = trim(value);

        if (trimmed == "true" || trimmed == "1") {
            return Result<bool, void *>::okOf(true);
        } else if (trimmed == "false" || trimmed == "0") {
            return Result<bool, void *>::okOf(false);
        } else {
            return Result<bool, void *>(nullopt, nullopt);
        }
    }

    static Result<bool, void *> toBool(char *value) {
        return toBool(string(value));
    }

    static Result<float, void *> toFloat(string value) {
        try {
            return Result<float, void *>::okOf(std::stof(value));
        }
        catch (std::exception &e) {
            return Result<float, void *>(nullopt, nullopt);
        }
    }

    static Result<float, void *> toFloat(char *value) {
        return toFloat(string(value));
    }

    static Result<double, void *> toDouble(const string& value) {
        try {
            return Result<double, void *>::okOf(std::stod(value));
        }
        catch (std::exception &e) {
            return Result<double, void *>(nullopt, nullopt);
        }
    }

    static Result<double, void *> toDouble(char *value) {
        return toDouble(string(value));
    }

    template<typename From, typename As>
    static As* as(From* value) {
        return reinterpret_cast<As*>(value);
    }

    template<typename From, gutils::number CastTo>
    static CastTo cast(const From& value) {
        return static_cast<CastTo>(value);
    }

    template<typename From, gutils::not_number CastTo>
    static CastTo cast(const From& value) {
        return std::move(static_cast<CastTo>(value));
    }

    template<typename From, gutils::number To>
    static To convert(From* from) {
        To to;
        memcpy(&to, from, sizeof(From));
        return to;
    }

    template<typename From, gutils::not_number To>
    static To convert(From* from) {
        To to;
        memcpy(&to, from, sizeof(From));
        return std::move(to);
    }

    template<typename From, typename To>
    static void convert(From* from, To* to) {
        memcpy(to, from, sizeof(From));
    }

    template<typename T>
    static void toBytes(const T source, char* dest) {
        memcpy(dest, source, sizeof(T));
    }

    template<typename T>
    static void fromBytes(const char* source, T* dest) {
        memcpy(dest, source, sizeof(T));
    }

    template<typename T>
    static inline vector <T> vectorRange(vector <T> &vec, size_t start, size_t end) {
        vector<T> ranged(end - start);

        while (start < end) {
            ranged.push_back(vec[start++]);
        }

        return ranged;
    }

    template<typename T>
    T* copy(const T* source, const size_t length) {
        T* copy = new T[length];
        std::copy(source, source + length, copy);
        return copy;
    }

    template<typename T>
    void copy(const T* source, T* destination, const size_t length) {
        std::copy(source, source + length, destination);
    }

    template<typename T>
    T* copy(const T* begin, const T* end) {
        T* copy = new T[(size_t) (end - begin)];
        std::copy(begin, end, copy);
        return copy;
    }

    // TODO: CHANGE THIS
    template<typename Iterator>
    Iterator copy(const Iterator begin, const Iterator end) {
        Iterator copy = new decltype(*begin)[(size_t) (end - begin)];
        std::copy(begin, end, copy);
        return copy;
    }

    // TODO: AND CHANGE THIS
    template<std::ranges::range Range_T>
    auto copy(const Range_T& source) {
        typedef decltype(source.begin()) T_Iter;
        typedef decltype(*T_Iter{0}) T;

        T* copy = new T[source.end() - source.begin()];
        std::copy(source.begin(), source.end(), copy);

        return copy;
    }

    template<typename T>
    static inline T popVector(vector <T> &vec) {
        T val = std::move(vec[vec.size() - 1]);
        vec.pop_back();
        return std::move(val);
    }

    template<typename T>
    static inline T popVector(vector <T> *vec) {
        T val = vec->at(vec->size() - 1);
        vec->pop_back();
        return std::move(val);
    }

    template<typename T>
    static inline void removeVector(vector <T> &vec, const size_t index) {
        vec.erase(vec.begin() + index);
    }

    template<typename T>
    static inline void removeVector(vector <T> &vec, const size_t start, const size_t count) {
        vec.erase(vec.begin() + start, vec.end() - count);
    }
}