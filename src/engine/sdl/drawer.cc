#include <engine/sdl/drawer.hh>
#include <engine/engine.hh>

#include <engine/sdl/game_window.hh>

#include <SDL.h>
#include <spdlog/spdlog.h>

using namespace engine;
using namespace math;

const float SQRT3_2 = std::sqrt(3)/2;

void draw_line(math::Point cam_a, math::Point cam_b) {
    SDL_RenderDrawLineF(sdl::get_renderer(), cam_a.x, cam_a.y, cam_b.x, cam_b.y);
}
void draw_line(math::Point cam_point, math::Vector vec) {
    draw_line(cam_point, cam_point + vec);
}


void drawer::enable_color(Color color) {
    SDL_SetRenderDrawColor(sdl::get_renderer(), color.r, color.g, color.b, color.a);
}


void drawer::draw_rect(Point a, Point b, Color color) {
    drawer::draw_rect(Rect::between(a, b), color);
}
void drawer::draw_rect_center(math::Point center, float width, float height, Color color) {
    float hw = width * 0.5; 
    float hh = height * 0.5;
    drawer::draw_rect(Rect(center.x - hw, center.y - hh, width, height), color);
}
void drawer::draw_rect(math::Rect rect, Color color) {
    // point to world ?
    math::Point ref = get_engine().world_to_camera(rect.x, rect.y);

    // Color
    enable_color(color);

    // Draw
    SDL_RenderDrawLineF(sdl::get_renderer(), ref.x, ref.y, ref.x + rect.w, ref.y);
    SDL_RenderDrawLineF(sdl::get_renderer(), ref.x, ref.y, ref.x, ref.y + rect.h);
    SDL_RenderDrawLineF(sdl::get_renderer(), ref.x, ref.y + rect.h, ref.x + rect.w, ref.y + rect.h);
    SDL_RenderDrawLineF(sdl::get_renderer(), ref.x + rect.w, ref.y, ref.x + rect.w, ref.y + rect.h);
}

void drawer::draw_rect_center_r(math::Point center, float width, float height, float theta_deg, Color color) {
    enable_color(color);

    math::Vector vec (width / 2, height / 2);
    math::Point a = center + vec.rotate_deg(theta_deg);
    math::Point b = center + vec.rotate_deg(theta_deg + 90);
    math::Point c = center + vec.rotate_deg(theta_deg + 180);
    math::Point d = center + vec.rotate_deg(theta_deg + 270);

    draw_line(a, b);
    draw_line(b, c);
    draw_line(c, d);
    draw_line(d, a);
}


void drawer::fill_rect(Point a, Point b, Color color) {
    drawer::fill_rect(Rect::between(a, b), color);
}
void drawer::fill_rect_center(Point center, float width, float height, Color color) {
    float hw = width * 0.5; 
    float hh = height * 0.5;
    drawer::fill_rect(Rect(center.x - hw, center.y - hh, width, height), color);
}
void drawer::fill_rect(Rect rect, Color color) {
    // point to world ?
    Point ref = get_engine().world_to_camera(rect.x, rect.y);
    SDL_FRect sdl_rect = {
        ref.x,
        ref.y,
        rect.w,
        rect.h
    };
    spdlog::trace("fill_rect[{}] -> ({},{},{},{}) :::: {:p}", rect, sdl_rect.x, sdl_rect.y, sdl_rect.w, sdl_rect.h, (void*)sdl::get_renderer());

    // Color
    enable_color(color);

    // Fill
    SDL_RenderFillRectF(sdl::get_renderer(), &sdl_rect);
}

void drawer::draw_arrow(math::Point from_raw, math::Point to_raw, Color color) {
    return drawer::draw_arrow(from_raw, to_raw, color, 10);
}
void drawer::draw_arrow(math::Point from_raw, math::Point to_raw, Color color, float point_length) {
    enable_color(color);

    // to camera
    Point from = get_engine().world_to_camera(from_raw);
    Point to = get_engine().world_to_camera(to_raw);
;
    // arrow body
    draw_line(from, to);
    
    // arrow head
    Vector norm = Vector::diff(to, from).normalized();

    Vector da = norm.rotate_deg(-45) * point_length;
    Vector db = norm.rotate_deg(45) * point_length;
    draw_line(to, da);
    draw_line(to, db);
}
