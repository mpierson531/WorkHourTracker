//
// Created by Micah on 10/28/2023.
//

#ifndef GEO_CPP_COLLISIONS_HPP
#define GEO_CPP_COLLISIONS_HPP

#include "../Shape.hpp"
#include "../shapes.hpp"

namespace gmath::shapes::collisions {
    bool collides(const AABB& one, const AABB& two) {
        const auto distance = two.Center() - one.Center();
        const float sumHalfWidth = one.Width() / 2 + two.Width() / 2;
        const float sumHalfHeight = one.Height() / 2 + two.Height() / 2;

        return !(gmath::gAbs(distance.x) < sumHalfWidth && gmath::gAbs(distance.y) < sumHalfHeight);
    }

    bool Collides(const Circle& one, const Circle& two) {
        const float sumRadii = one.Radius() + two.Radius();
        const Vector2 distanceDiff = two.Position() - one.Position();

        if (distanceDiff.Length2() - gmath::gPow(sumRadii, 2.0f) > 0)
            return false;

        return true;
    }

    bool Collides(const AABB& one, const Circle& two) {
        const Vector2 distanceDiff = two.Center() - one.Center();
        Vector2 closest{distanceDiff};

        const float xExtent = one.Width() * 0.5f;
        const float yExtent = one.Height() * 0.5f;

        closest.x = gmath::gClamp(-xExtent, xExtent, closest.x);
        closest.y = gmath::gClamp(-yExtent, yExtent, closest.y);
        const bool inside = distanceDiff == closest;

        if (distanceDiff == closest) {
            if (gmath::gAbs(distanceDiff.x) > gmath::gAbs(distanceDiff.y))
                closest.x = closest.x > 0.0f ? xExtent : -xExtent;
            else
                closest.y = closest.y > 0.0f ? yExtent : -yExtent;
        }

        if ((distanceDiff - closest).Length2() > gmath::gPow(two.Radius(), 2.0f) && !inside)
            return false;

        return true;
    }
}

#endif //GEO_CPP_COLLISIONS_HPP