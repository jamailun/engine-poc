#include <engine/math.hh>

using namespace engine;
using namespace math;

// math

Point Point::operator+(Vector vec) const {
    return Point(x + vec.dx, y + vec.dy);
}

float Point::distance(Point other) {
    return Vector::diff(*this, other).length();
}

float Point::squared_distance(Point other) {
    return Vector::diff(*this, other).squared_length();
}

bool Rect::intersects(Rect other) const {
    float x1 = x;
    float x2 = x + w;
    float y1 = y;
    float y2 = y + h;
    float ox1 = other.x;
    float ox2 = other.x + other.w;
    float oy1 = other.y;
    float oy2 = other.y + other.h;

    bool noOverlap = ox2 < x1 || x2 < ox1
                  || oy2 < y1 || y2 < oy1;
    return !noOverlap;
}

Rect Point::to_rect(float w, float h) const {
    return Rect(x, y, w, h);
}

// math / random

#include <random>
static std::uniform_real_distribution<double> UNIFORM_RANDOM(0, 1);
std::default_random_engine RANDOM_ENGINE;

Vector Vector::normalized_r() const {
    float len = length();
    if(len == 0) {
        float x = UNIFORM_RANDOM(RANDOM_ENGINE) - 0.5;
        float y = UNIFORM_RANDOM(RANDOM_ENGINE) - 0.5;
        return Vector(x, y);
    }
    return Vector(dx/len, dy/len);
}

// math / utils
#include <engine/math_utils.hh>

Point engine::math::average_position(std::vector<Point> points) {
    Point center (0, 0);
    for(Point point : points) {
        center.x += point.x;
        center.y += point.y;
    }
    return center;
}
