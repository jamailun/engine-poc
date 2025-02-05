
#include <iostream>

#include <engine/registry.hh>


class component_wif {
public:
    void update() { std::cout << "WIF update" << std::endl; }
};

class component_waf {
public:
    void action() { std::cout << "WAF action" << std::endl; }
};

class component_woof {
public:
    void update() { std::cout << "WOOF update" << std::endl; }
    void action() { std::cout << "WOOF action" << std::endl; }
};


int main() {
    engine::component_registry registry;

    std::shared_ptr<component_wif> wif = std::make_shared<component_wif>();
    std::shared_ptr<component_waf> waf = std::make_shared<component_waf>();
    std::shared_ptr<component_woof> woof = std::make_shared<component_woof>();

    const engine::component_index wif_idx = registry.add(wif);
    const engine::component_index waf_idx = registry.add(waf);
    registry.update_all();
    registry.action_all();
    std::cout << "============================" << std::endl;

    std::ignore = registry.add(woof);
    registry.update_all();
    registry.action_all();
    std::cout << "============================" << std::endl;

    registry.remove(wif_idx);
    registry.remove(waf_idx);
    registry.update_all();
    registry.action_all();
    std::cout << "============================" << std::endl;

}
