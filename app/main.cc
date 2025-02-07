#include <iostream>

#include <engine/entity.hh>
#include <engine/engine.hh>

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
            std::cout << "WIF render. pos is = (" << pos->x << ", " << pos->y << ")." << std::endl;
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


int main() {
    std::shared_ptr<engine::entity> object = std::make_shared<engine::entity>("toto");

    engine::get_engine().create_component<component_wif>(object);
    engine::get_engine().create_component<component_pos>(object, 12, 12);

    // "game-loop"
    for(size_t i = 0; i < 4; i++) {
        std::cout << "==============(" << i << ")==============" << std::endl;
        engine::get_engine().on_update();
        engine::get_engine().on_render();
    }
    std::cout << "===============================" << std::endl;

    object->destroy_now();
    object = nullptr;
}
