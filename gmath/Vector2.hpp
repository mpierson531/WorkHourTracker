//
// Created by Micah on 10/14/2023.
//

#ifndef GEO_CPP_VECTOR2_HPP
#define GEO_CPP_VECTOR2_HPP

#include <initializer_list>
#include <string>
#include <format>

using std::string;

#include "math.hpp"

namespace gmath {
    struct Vector2 {
        static Vector2 Zero() {
            return Vector2{0.0f, 0.0f};
        }

        static Vector2 One() {
            return Vector2{1.0f, 1.0f};
        }

        static Vector2 X1() {
            return Vector2{1.0f, 0.0f};
        }

        static Vector2 Y1() {
            return Vector2{0.0f, 1.0f};
        }

        float x;
        float y;

        Vector2() : x(0.0f), y(0.0f) { }
        Vector2(const float x, const float y) noexcept : x(x), y(y) {}
        Vector2(const Vector2& toCopy) = default;
        Vector2(std::initializer_list<Vector2>) noexcept { }
        Vector2(const float value) : x(value), y(value) { }

        [[nodiscard]] float Average() const {
            return (x + y) / 2.0f;
        }

        [[nodiscard]] Vector2 Reverse() const {
            return {y, x};
        }

        Vector2& swap(Vector2& rhs) {
            const Vector2 temp{rhs};

            this->x = rhs.x;
            this->y = rhs.y;

            rhs = temp;

            return *this;
        }

        Vector2& set(const float x, const float y) {
            this->x = x;
            this->y = y;
            return *this;
        }

        Vector2& set(const Vector2& other) {
            x = other.x;
            y = other.y;
            return *this;
        }

        [[nodiscard]] Vector2& Nor() {
            const float len = Length();

            if (len != 0)
                set(this->operator/(len));

            return *this;
        }

        [[nodiscard]] Vector2& Clamp(const float min, const float max) {
            const float len2 = Length2();

            if (len2 == 0.0f)
                return *this;

            const float max2 = max * max;

            if (len2 > max2) {
                (*this) = this->operator*((gSqrt(max2 / len2)));
                return *this;
            }

            const float min2 = min * min;

            if (len2 < min2) {
                (*this) = this->operator*((gSqrt(min2 / len2)));
                return *this;
            }

            return *this;
        }

        [[nodiscard]] Vector2& RotateDeg(const float degrees) {
            return this->RotateRad(degrees * DEG_RAD);
        }

        [[nodiscard]] Vector2& RotateRad(const float radians) {
            const float cos = gCos(radians);
            const float sin = gSin(radians);

            const float newX = x * cos - y * sin;
            const float newY = x * sin + y * cos;

            this->x = newX;
            this->y = newY;

            return *this;
        }

        [[nodiscard]] Vector2& RotateAroundDeg(const Vector2& reference, const float degrees) {
            Vector2 newVector = ((Vector2 { x, y }) - reference).RotateDeg(degrees) + reference;
            (*this) = newVector;
            return *this;
        }

        [[nodiscard]] float AngleDeg() const {
            const float angle = gAtan2(y, x) * RAD_DEG;
            return angle < 0 ? angle + 360 : angle;
        }

        [[nodiscard]] float AngleDeg(const Vector2& reference) const {
            return gAtan2(Cross(reference), Dot(reference)) * RAD_DEG;
        }

        [[nodiscard]] float AngleRad() const {
            return gAtan2(y, x);
        }

        [[nodiscard]] float AngleRad(const Vector2& reference) const {
            return gAtan2(reference.Cross(x, y), reference.Dot(x, y));
        }

        [[nodiscard]] float Cross(const Vector2& other) const {
            return x * other.y - y * other.x;
        }

        [[nodiscard]] float Cross(const float x2, const float y2) const {
            return x * y2 - y * x2;
        }

        [[nodiscard]] float Distance2(const float x2, const float y2) const {
            return Distance2(x, y, x2, y2);
        }

        [[nodiscard]] float Distance2(const Vector2& other) const {
            return Distance2(x, y, other.x, other.y);
        }

        [[nodiscard]] constexpr static float Distance2(const float x1, const float y1, const float x2, const float y2) {
            const float x_d = x2 - x1;
            const float y_d = y2 - y1;
            return x_d * x_d + y_d * y_d;
        }

        [[nodiscard]] float Distance(const Vector2& other) const {
            return Distance(x, y, other.x, other.y);
        }

        [[nodiscard]] float Distance(const float x2, const float y2) const {
            return Distance(x, y, x2, y2);
        }

        [[nodiscard]] constexpr static float Distance(const float x1, const float y1, const float x2, const float y2) {
            return (gSqrt(Distance2(x1, y1, x2, y2)));
        }

        [[nodiscard]] float Dot(const Vector2& other) const {
            return Dot(x, y, other.x, other.y);
        }

        [[nodiscard]] float Dot(const float x2, const float y2) const {
            return Dot(x, y, x2, y2);
        }

        [[nodiscard]] static constexpr float Dot(const float x1, const float y1, const float x2, const float y2) {
            return x1 * x2 + y1 * y2;
        }

        [[nodiscard]] float Length() const {
            return std::sqrt(Length2());
        }

        [[nodiscard]] float Length2() const {
            return x * x + y * y;
        }

        string ToString() const noexcept {
            return std::format("Vector2 [{}, {}]", x, y);
        }

        Vector2& operator=(const Vector2& other) = default;

        bool operator==(const Vector2& other) const {
            return x == other.x && y == other.y;
        }

        // start float ops
        Vector2 operator+(const float value) const {
            return Vector2 { x + value, y + value };
        }

        Vector2 operator-(const float value) const {
            return Vector2 { x - value, y + value };
        }

        Vector2 operator*(const float value) const {
            return Vector2 { x * value, y * value };
        }

        Vector2 operator/(const float value) const {
            return Vector2 { x / value, y / value };
        }

        Vector2 operator^(const float value) const {
            return Vector2 { powf(x, value), powf(y, value) };
        }

        Vector2 operator%(const float value) const {
            return Vector2 { gMod(x, value), gMod(y, value) };
        }
        //end float ops

        // start l-value ops
        Vector2 operator+(const Vector2& value) const {
            return Vector2 { x + value.x, y + value.y };
        }

        Vector2 operator-(const Vector2& value) const {
            return Vector2 { x - value.x, y + value.y };
        }

        Vector2 operator*(const Vector2& value) const {
            return Vector2 { x * value.x, y * value.y };
        }

        Vector2 operator/(const Vector2& value) const {
            return Vector2 { x / value.x, y / value.y };
        }

        Vector2 operator^(const Vector2& value) const {
            return Vector2 { powf(x, value.x), powf(y, value.y) };
        }

        Vector2 operator%(const Vector2& value) const {
            return Vector2 { (gMod(x, value.x)), (gMod(y, value.y)) };
        }
        // end l-value ops

        // begin postfix ops
        Vector2& operator++() {
            x++;
            y++;
            return *this;
        }

        Vector2& operator--() {
            x--;
            y--;
            return *this;
        }
        // end postfix ops

        float operator[](size_t index) const {
            switch(index) {
                case 0: return x;
                case 1: return y;
                default: throw std::runtime_error { "Vector2 index can only be 0 or 1!" };
            }
        }
    };
}

#endif //GEO_CPP_VECTOR2_HPP
