#pragma once

#include "engine/utils/fmt_utils.hh"

#include <math.h>
#include <SDL2/SDL_rect.h>

namespace engine {
namespace math {

const float PI = 3.14159;

struct Vector;

struct Point {
    float x, y;
    Point(float x, float y) : x(x), y(y) {}
    Point(float value) : Point(value, value) {}
    Point() : Point(0) {}

    inline SDL_FPoint to_sdl() const { return SDL_FPoint{x, y}; }
    float distance(Point other);
    float squared_distance(Point other);
    Point operator+(Vector vec) const;
};

struct Vector {
    float dx, dy;
    Vector(float dx, float dy) : dx(dx), dy(dy) {}
    Vector(float value) : Vector(value, value) {}
    Vector() : Vector(0) {}

    inline float squared_length() const {
        return dx*dx + dy*dy;
    }
    inline float length() const {
        return std::sqrt(squared_length());
    }

    static Vector diff(Point from, Point to) {
        return Vector(to.x - from.x, to.y - from.y);
    }
    inline Vector normalized() const {
        float len = length();
        if(len == 0) return Vector(0);
        return Vector(dx/len, dy/len);
    }
    Vector normalized_r() const;

    inline Vector rotate_rad(float theta_rad) const {
        float cosTheta = std::cos(theta_rad);
        float sinTheta = std::sin(theta_rad);
        return Vector(
            dx * cosTheta - dy * sinTheta,
            dx * sinTheta + dy * cosTheta
        );
    }
    inline Vector rotate_deg(float theta_deg) const {
        float theta = theta_deg * PI / 180;
        return rotate_rad(theta);
    }

    inline Vector operator*(const float lambda) {
        return Vector(dx * lambda, dy * lambda);
    }
    inline Vector operator+(const Vector other) {
        return Vector(dx + other.dx, dy + other.dy);
    }
    inline Vector operator-(const Vector other) {
        return Vector(dx - other.dx, dy - other.dy);
    }
};

struct Rect {
    float x, y;
    float w, h;
    Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
    Rect() : Rect(0, 0, 0, 0) {}

    static Rect between(Point a, Point b) {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        return Rect {
            // Position
            dx < 0 ? b.x : a.x,
            dy < 0 ? b.y : a.y,
            // Size
            std::fabs(dx),
            std::fabs(dy)
        };
    }

    inline bool contains(Point point) const {
        return (point.x >= x) && (point.x < (x + w))
            && (point.y >= y) && (point.y < (y + h));
    }

    bool intersects(Rect other) const;

    inline SDL_FRect to_sdl() const { return SDL_FRect{x, y, w, h}; }
};

} // namespace math
} // namespace engine

// Can I move that into a source file ??
ENGINE__FMT_UTILS__HANDLE_FMT_OSTREAM(engine::math::Point, "({}, {})", object.x, object.y);
ENGINE__FMT_UTILS__HANDLE_FMT_OSTREAM(engine::math::Vector, "({}, {})", object.dx, object.dy);
ENGINE__FMT_UTILS__HANDLE_FMT_OSTREAM(engine::math::Rect, "({}, {}; w={}, h={})", object.x, object.y, object.w, object.h);
