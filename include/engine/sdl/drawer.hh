#pragma once

#include "engine/math.hh"
#include "engine/sdl/colors.hh"

namespace engine {
namespace drawer {

void enable_color(Color color);

void draw_rect_center(math::Point center, float width, float height, Color color);
void draw_rect(math::Point a, math::Point b, Color color);
void draw_rect(math::Rect rect, Color color);
void draw_rect_center_r(math::Point center, float width, float height, float theta, Color color);

void fill_rect_center(math::Point center, float width, float height, Color color);
void fill_rect(math::Point a, math::Point b, Color color);
void fill_rect(math::Rect rect, Color color);

void draw_arrow(math::Point from, math::Point to, Color color);
void draw_arrow(math::Point from, math::Point to, Color color, float arrow_point_length);

} // namespace drawer
} // namespace engine
