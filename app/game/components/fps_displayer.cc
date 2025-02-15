#include "./fps_displayer.hh"
#include <engine/entity.hh>
#include <engine/engine.hh>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

using namespace game;

void FpsDisplayer::update(float) {
    if(!_text_renderer) {
        _text_renderer = get_entity()->get_component<engine::text_renderer>();
        if(!_text_renderer) {
            spdlog::error("FPS displayer cannot find text-renderer.");
            return;
        }
    }
    //
    std::string text = fmt::format("FPS: {}", engine::get_engine().fps());
    _text_renderer->set_text(text);
}