//
// Created by Micah on 10/28/2023.
//

#ifndef GEO_CPP_SHAPES_HPP
#define GEO_CPP_SHAPES_HPP

#include "Shape.hpp"

namespace gmath::shapes {
    class AABB;

    class Circle : public Shape {
    private:
        float radius;
    public:
        Circle() : Shape({0.0f}, {0.0f}), radius(0.0f) {}

        Circle(const float x, const float y, const float radius) : Shape({x, y}, {radius * 2}), radius(radius) {}

        Circle(const Vector2 &position, const float radius) : Shape(position, {radius * 2}), radius(radius) {}

        void Set(const float x, const float y, const float radius) {
            this->position = {x, y};
            this->size = {radius * 2.0f};
            this->radius = radius;
        }

        void Set(const Vector2 &position, const float radius) {
            this->position = position;
            this->size = {radius * 2.0f};
            this->radius = radius;
        }

        [[nodiscard]] float Radius() const {
            return radius;
        }

        void Radius(const float radius) {
            this->radius = radius;
        }

        [[nodiscard]] float Diameter() const {
            return Shape::Width();
        }

        [[nodiscard]] float Area() const {
            return gmath::PI * gmath::gPow(radius, 2.0f);
        }

        [[nodiscard]] float Circumference() const {
            return gmath::gCircumference(radius);
        }

        [[nodiscard]] Vector2 Center() const override {
            return position;
        }

        [[nodiscard]] std::pair<Vector2, Vector2> Bounds() const {
            return {{X() - radius, Y() - radius},
                    {X() + radius, Y() + radius}};
        }

        [[nodiscard]] inline AABB BoundingBox() const;

        [[nodiscard]] string ToString() const override {
            return std::format("Circle [{}, {}]", position.ToString(), size.ToString());
        }
    };

    class AABB : public Shape {
    public:
        AABB() : Shape(0.0f, 0.0f, 0.0f, 0.0f) {}

        AABB(const Vector2 &position, const Vector2 &size)
                : Shape(position, size) {}

        AABB(const float x, const float y, const float width, const float height)
                : Shape(x, y, width, height) {}

        void Set(const float x, const float y, const float width, const float height) {
            position = {x, y};
            size = {width, height};
        }

        void Set(const Vector2 &position, const Vector2 &size) {
            this->position = position;
            this->size = size;
        }

        [[nodiscard]] float Area() const {
            return gmath::gArea(size.x, size.y);
        }

        [[nodiscard]] float Perimeter() const {
            return (Width() * 2) + (Height() * 2);
        }

        [[nodiscard]] bool IsSquare() const {
            return Width() == Height();
        }

        [[nodiscard]] Vector2 Max() const {
            return position + size;
        }

        [[nodiscard]] float MaxX() const {
            return X() + Width();
        }

        [[nodiscard]] float MaxY() const {
            return Y() + Height();
        }

        [[nodiscard]] Vector2 Center() const override {
            // TODO: multiply instead
            return position + (size / 2.0f);
        }

        [[nodiscard]] std::pair<Vector2, Vector2> Bounds() const {
            return {position, position + size};
        }

        [[nodiscard]] std::pair<AABB, AABB> SplitHorizontally() {
            const Vector2 position = Position();
            const Vector2 size = Size();

            // TODO: multiply instead
            const float newHeight = size.y / 2.0f;

            const AABB bottom{position.x, position.y, size.x, newHeight};
            const AABB top{position.x, position.y + newHeight, size.x, newHeight};

            return {bottom, top};
        }

        [[nodiscard]] std::pair<AABB, AABB> SplitVertically() {
            const Vector2 position = Position();
            const Vector2 size = Size();

            const float newWidth = size.x / 2;

            const AABB left{position.x, position.y, newWidth, size.y};
            const AABB right{position.x + newWidth, position.y, newWidth, size.y};

            return {left, right};
        }

        [[nodiscard]] string ToString() const override {
            return std::format("AABB [{}, {}]", position.ToString(), size.ToString());
        }
    };

    class Triangle : public Shape {
    public:
        // TODO: Implement Triangle
    };

    shapes::AABB Circle::BoundingBox() const {
        return {position - radius, position + radius};
    }
}

#endif //GEO_CPP_SHAPES_HPP