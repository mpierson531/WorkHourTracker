//
// Created by Micah on 10/28/2023.
//

#ifndef GEO_CPP_SHAPE_HPP
#define GEO_CPP_SHAPE_HPP

#include "../Vector2.hpp"

namespace gmath::shapes {

    // TODO: IMPLEMENT "Max(), MaxX(), MaxY()" for all types

    class Shape {
    protected:
        Vector2 position;
        Vector2 size;
    public:
        Shape() : position(Vector2::Zero()), size(Vector2::Zero()) { }

        Shape(const Vector2& position, const Vector2& size)
        : position(position), size(size) { }

        Shape(const float x, const float y, const float width, const float height)
        : position({x, y}), size({width, height}) { }

        // position getters
        [[nodiscard]] virtual Vector2 Position() const {
            return position;
        }

        [[nodiscard]] virtual float X() const {
            return position.x;
        }

        [[nodiscard]] virtual float Y() const {
            return position.y;
        }

        // position setters
        virtual void Position(const float x, const float y) {
            position.set(x, y);
        }

        virtual void Position(const Vector2 &position) {
            this->position.set(position);
        }

        virtual void X(const float x) {
            position.x = x;
        }

        virtual void Y(const float y) {
            position.y = y;
        }

        // TEST_SIZE getters
        [[nodiscard]] virtual Vector2 Size() const {
            return size;
        }

        [[nodiscard]] virtual float Width() const {
            return size.x;
        }

        [[nodiscard]] virtual float Height() const {
            return size.y;
        }

        // TEST_SIZE setters
        virtual void Size(const float x, const float y) {
            size.set(x, y);
        }

        virtual void Size(const Vector2 &size) {
            this->size.set(size);
        }

        virtual void Width(const float width) {
            size.x = width;
        }

        virtual void Height(const float height) {
            size.y = height;
        }

        [[nodiscard]] virtual Vector2 Center() const {
            throw std::runtime_error{"Cannot call Center method on base shape instance."};
        }

        [[nodiscard]] virtual Vector2 HalfSize() const {
            return size / 2.0f;
        }

        [[nodiscard]] virtual string ToString() const {
            return "Base Shape [ " + position.ToString() + ", " + size.ToString() + "]";
        }
    };
}

#endif //GEO_CPP_SHAPE_HPP