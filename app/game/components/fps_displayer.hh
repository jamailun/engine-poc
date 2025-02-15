#pragma once

#include <memory>
#include <engine/component.hh>
#include <engine/components/text_renderer.hh>

namespace game {

/**
 *  Update a text_renderer.
 */
class FpsDisplayer : public engine::base_component {
private:
    std::shared_ptr<engine::text_renderer> _text_renderer;
public:
    FpsDisplayer(entity_ptr entity) : engine::base_component(entity) {}
    ~FpsDisplayer() = default;

    virtual void update(float);
};

} // namespace game
