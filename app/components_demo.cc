#include <engine/entity.hh>
#include <engine/world.hh>

#include <spdlog/spdlog.h>

class component_pos : public engine::base_component {
public:
    float x, y;
    explicit component_pos(entity_ptr entity) : base_component(entity), x(0), y(0) {}
    explicit component_pos(entity_ptr entity, float x, float y) : base_component(entity), x(x), y(y) {}
    void update() {}
};


class component_wif : public engine::base_component {
public:
    component_wif(entity_ptr entity) : base_component(entity) {}
    void render() {
        auto pos = get_entity()->get_component<component_pos>();
        if(pos) {
            spdlog::info("wif.render. Pos-component is at ({}, {}).", pos->x, pos->y);
            spdlog::info("-- entity/world = {}.", get_entity()->get_world()->name());
        }
    }
    void update() {
        auto pos = get_entity()->get_component<component_pos>();
        if(pos) {
            pos->x += 2;
            pos->y -= 1;
        }
    }
};
