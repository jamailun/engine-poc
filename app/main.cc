
#include <iostream>

#include <engine/registry.hh>
#include <engine/entity.hh>

class component_waf : public engine::base_component {
public:
    component_waf(entity_ptr entity) : base_component(entity) {}
    void action() { std::cout << "WAF action" << std::endl; }
};

class component_woof : public engine::base_component {
public:
    component_woof(entity_ptr entity) : base_component(entity) {}
    void update() { std::cout << "WOOF update" << std::endl; }
    void action() { std::cout << "WOOF action" << std::endl; }
};

class component_pos : public engine::base_component {
public:
    float x, y;
    explicit component_pos(entity_ptr entity) : base_component(entity), x(0), y(0) {}
    explicit component_pos(entity_ptr entity, float x, float y) : base_component(entity), x(x), y(y) {}
    void update() {
        x += 3;
        y -= 1;
    }
};

class component_wif : public engine::base_component {
public:
    component_wif(entity_ptr entity) : base_component(entity) {}
    void action() {
        auto pos = get_entity()->get_component<component_pos>();
        if(pos) {
            std::cout << "WIF update. pos is = (" << pos->x << ", " << pos->y << ")." << std::endl;
        }
    }
};

/*
void test_1() {
    engine::component_registry registry;

    std::shared_ptr<component_wif> wif = std::make_shared<component_wif>();
    std::shared_ptr<component_waf> waf = std::make_shared<component_waf>();
    std::shared_ptr<component_woof> woof = std::make_shared<component_woof>();

    registry.add(wif);
    registry.add(waf);
    registry.update_all();
    registry.action_all();
    std::cout << "============================" << std::endl;

    registry.add(woof);
    registry.add(wif); // Does nothing because this address is already in the registry
    registry.update_all();
    registry.action_all();
    std::cout << "============================" << std::endl;

    registry.remove(wif);
    registry.remove(waf);
    registry.update_all();
    registry.action_all();
    std::cout << "============================" << std::endl;
}*/

void test_2() {
    engine::component_registry registry;
    std::shared_ptr<engine::entity> object = std::make_shared<engine::entity>();

    std::shared_ptr<component_wif> wif = std::make_shared<component_wif>(object);
    std::shared_ptr<component_pos> pos = std::make_shared<component_pos>(object, 69, 42);
    registry.add(wif);
    object->register_component(wif);
    registry.add(pos);
    object->register_component(pos);

    // "game-loop"
    std::cout << "============================" << std::endl;
    for(size_t i = 0; i < 4; i++) {
        registry.update_all();
        registry.action_all();
        std::cout << "============================" << std::endl;
    }

    object->destroy_now();
    object = nullptr;
}

int main() {
    test_2();
}
