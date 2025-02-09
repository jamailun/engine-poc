#pragma once

#include "engine/math.hh"
#include <vector>
#include <functional>

namespace engine {
namespace math {

/**
 * Get the average position of a collection of `Points`.
 */
Point average_position(std::vector<Point> points);

template<typename From, typename To>
using Producer = std::function<To(From)>;

/**
 * For collection of `T`, returns the average position of each element.
 * @param points a vector of `T`.
 * @param function a function to map `T` into `engine::math::Point`.
 */
template<typename T>
Point average_position(std::vector<T> points, Producer<T, Point> function) {
    Point center (0, 0);
    for(auto& t : points) {
        Point point = function(t);
        center.x += point.x;
        center.y += point.y;
    }
    return center;
}

} // namespace math
} // namespace engine
