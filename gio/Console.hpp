#pragma once

#include <iostream>
#include <string>
#include <format>
using std::string, std::cin, std::cout;

#include "../gutils/Result.hpp"
#include "../gutils/gutils.hpp"
#include "../gcolors/gcolors.hpp"
using gutils::Result, gutils::toString;

// TODO: WORK OUT ACCESS TO INDIVIDUAL COLOR COMPONENTS

namespace gio {
    class Console {
    private:
        template<gutils::unsigned_integer T>
        constexpr static std::string RGB_FG(const gcolors::rgb<T> &color) {
            return RGB_FG(color.r, color.g, color.b, color.a);
        }

        template<gutils::unsigned_integer T>
        constexpr static std::string RGB_FG(const T r, const T g, const T b) {
            return std::format("\u001b[38;2;{};{};{}m", r, g, b);
        }

        constexpr static std::string FG(const uint8_t color) {
            return std::format("\u001b[38;5;{}m", color);
        }

        gcolors::rgb_ui m_color;
        std::string colorTag;
        constexpr static std::string resetTag = "\u001b[0m";

        constexpr void SetColorTag() {
            colorTag = RGB_FG(m_color.r, m_color.g, m_color.b);
        }

        template<typename T>
        constexpr void SetColorTag(T r, T g, T b) requires(gutils::unsigned_integer<T>) {
            colorTag = RGB_FG(r, g, b);
        }

        template<typename T>
        constexpr void SetColorTag(T r, T g, T b) requires(gutils::signed_integer<T>) {
            SetColorTag((uint8_t) r, (uint8_t) g, (uint8_t) b);
        }

    public:
        Console() : Console(gcolors::rgb_ui{200, 200, 200}) {}

        template<gutils::number T>
        explicit Console(const gcolors::rgb<T>& color) {
            this->color(color);
        }

        template<typename T>
        Console &println(const T& value) {
            cout << colorTag << toString(value) << std::endl;
            return *this;
        }

        template<typename T>
        Console &println(const T& value, const bool reset) {
            println(value);

            if (reset)
                this->reset();

            return *this;
        }

        // TODO: BELOW
        /*template<typename T>
        Console& fmt_println(string m_format, T... args) {
            println(std::m_format(m_format, args));
        }*/

        Console &println() {
            cout << std::endl;
            return *this;
        }

        template<typename T>
        Console &print(const T& value) {
            cout << colorTag << toString(value);
            return *this;
        }

        template<typename T>
        Console &print(const T& value, const bool reset) {
            cout << colorTag << toString(value);

            if (reset)
                this->reset();

            return *this;
        }

        // TODO: THIS
        Console& precision(const ptrdiff_t precision) {
            cout.precision(precision);
            return *this;
        }

        Console &print() {
            cout << '\0';
            return *this;
        }

        [[nodiscard]] Result<bool, void *> readBool() {
            return gutils::toBool(readln());
        }

        [[nodiscard]] Result<float, void *> readFloat() {
            return gutils::toFloat(readln());
        }

        [[nodiscard]] Result<double, void *> readDouble() {
            return gutils::toDouble(readln());
        }

        [[nodiscard]] int read() {
            return cin.get();
        }

        [[nodiscard]] char readChar() {
            char c;
            cin.get(c);
            return c;
        }

        [[nodiscard]] std::string readln() {
            string input;
            std::getline(cin, input);
            return std::move(input);
        }

        Console &reset() {
            std::cout << resetTag;
            return *this;
        }

        template<gutils::number T>
        Console &color(const gcolors::rgb<T> &newColor) {
            return color(newColor.r, newColor.g, newColor.b);
        }

        template<gutils::number T>
        constexpr Console &color(const T r, const T g, const T b) {
            m_color = gcolors::rgb<T>(r, g, b).to_rgb_ui();
            SetColorTag();
            return *this;
        }

        [[nodiscard]] gcolors::rgb_ui color() const {
            return m_color;
        }

        template<gutils::number T>
        constexpr void r(const T r) {
            if (gutils::floating_point<T>) {
                m_color.r = (uint16_t) r * 100;
            } else if (typeid(r) != typeid(uint16_t)) {
                m_color.r = (uint16_t) r;
            } else {
                m_color.r = r;
            }
        }

        template<gutils::number T>
        constexpr void g(const T g) {
            if (gutils::floating_point<T>) {
                m_color.g = (uint16_t) g * 100;
            } else if (typeid(g) != typeid(uint16_t)) {
                m_color.g = (uint16_t) g;
            } else {
                m_color.g = g;
            }
        }

        template<gutils::number T>
        constexpr void b(const T b) {
            if (gutils::floating_point<T>) {
                m_color.b = (uint16_t) b * 100;
            } else if (typeid(b) != typeid(uint16_t)) {
                m_color.b = (uint16_t) b;
            } else {
                m_color.g = b;
            }
        }

        [[nodiscard]] constexpr uint8_t r() const {
            return m_color.r;
        }

        [[nodiscard]] constexpr uint8_t g() const {
            return m_color.g;
        }

        [[nodiscard]] constexpr uint8_t b() const {
            return m_color.b;
        }
    };
}